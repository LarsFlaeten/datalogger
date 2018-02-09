#include <iostream>
#include <sstream>

#include "Datalogger.h"

Datalogger::Datalogger(const std::string& blackbox, double freq)
#ifdef ORK_API
    : Object("Datalogger")
#endif
{
    _canLog = false;
    _hasStarted = false;
    init(blackbox, freq);

}

Datalogger::~Datalogger()
{
    _blackbox.close();
}

Datalogger::Datalogger()
#ifdef ORK_API
    : Object("Datalogger")
#endif
{
    _canLog = false;
    _hasStarted = false;
}
            

void Datalogger::init(const std::string& blackbox, double freq)
{

    if(freq < 0.0)
        throw std::runtime_error("Error, freq < 0.0 not allowed");
 
    // This could be a "restart" with new file:
    if(_hasStarted)
    {
        _blackbox.close();
    }

    _registeredClasses.clear();
    _registeredValueNames.clear();
    _classToIndex.clear();
    _valueToIndex.clear();

    _hasStarted = false;
    _canLog = false;
    _step = 0;
        
   _freq = freq;
    _lastStep = -1.0E15;

    _blackbox.open(blackbox, std::fstream::out | std::fstream::binary);
    // check that he file opened ok:
    if(!_blackbox.is_open())
        throw std::runtime_error("File could not be opened, use dby other process?");


    registerValueName("TIME", "t", "DOUBLE");
}

bool Datalogger::isAllowedType(const std::string& type)
{
    bool ret = false;
    if(type.compare("INT") == 0)
        ret = true;
    if(type.compare("DOUBLE") == 0)
        ret = true;

    return ret;

}

void Datalogger::registerValueName(const std::string& cl, const std::string& valuename, const std::string& type)
{
	if(cl.compare("") == 0 || valuename.compare("")== 0)
		throw std::runtime_error("Registration of empty class or value name not allowed");

    if(_hasStarted)
        throw std::runtime_error("Registration of new classes/valuenames not allowed after start logging");

    if(!isAllowedType(type)) {
        std::ostringstream oss;
        oss << "Type \"" << type << "\" is not allowed" << std::endl;
        throw std::runtime_error(oss.str());
    }

    if(_registeredClasses.size() > 255)
        throw std::runtime_error("Reached max allowable classes (256)");

    int cli = findClassIndex(cl);

    if(cli == -1) {
        // New class
        _registeredClasses.push_back(cl);
        std::vector<std::string> v, t;
        v.push_back(valuename);
        t.push_back(type);
        _registeredValueNames.push_back(v);
        _registeredTypes.push_back(t);
    } else
    {
        if(_registeredValueNames[cli].size() > 255)
                    throw std::runtime_error("Reached max allowable values (256)");
        // Check for duplicate:
        if(findValueIndex(cli, valuename) != -1)
            throw std::runtime_error("Valuename allready exists in class");

        _registeredValueNames[cli].push_back(valuename);
        _registeredTypes[cli].push_back(type);
    }

}
        
void Datalogger::tick(double t)
{
    // Avoid multiple ticks:
    if(_canLog)
        throw std::runtime_error("No tick allowed after another tick without tock");
    // check if we are supposed ot log according to frequency
    if(t - _lastStep < _freq)
        return;

    _canLog = true;

    ++_step;

	if(!_hasStarted)
    {
        writeHeader();
        _hasStarted = true;
    }

    // Step number:
    writeInt(_step);

    // First entry for this step:
    writeValue("TIME", "t", t);
}

void Datalogger::tock()
{
    _canLog = false;
}

void Datalogger::writeValue(const std::string& cl, const std::string& valuename, int value)
{
    if(!_hasStarted)
    {
        throw std::runtime_error("Need to start with a tick");
    }

    if(!_canLog)
        return;


    // Make sure it is allready there
    if(_classToIndex.find(cl) == _classToIndex.end())
    {
        throw std::runtime_error("Tried to write unregistered class");
    }
    
    // Look up class index:
    int cli = _classToIndex[cl];

    // Make sure the value is there:
    if(_valueToIndex[cli].find(valuename) == _valueToIndex[cli].end())
    {
        throw std::runtime_error("Tried to write unregistered value");
    }

    // look up value index:
    int vli = _valueToIndex[cli][valuename];

    // Check type
    if(_registeredTypes[cli][vli].compare("INT") != 0)
    {       
        throw std::runtime_error("Tried to write wrong type");
    }

    // Write single value:
    writeChar((char)cli);
    writeChar((char)vli);
    writeInt(value);
}
    
void Datalogger::writeValue(const std::string& cl, const std::string& valuename, double value)
{
    if(!_hasStarted)
    {
		throw std::runtime_error("Need to start with a tick");

    }

    if(!_canLog)
        return;

    // Make sure it is allready there
    if(_classToIndex.find(cl) == _classToIndex.end())
    {   
        throw std::runtime_error("Tried to write to unregistered class");
    }

    // Look up class index:
    int cli = _classToIndex[cl];

    // Make sure the value is there:
    if(_valueToIndex[cli].find(valuename) == _valueToIndex[cli].end())
    {
        throw std::runtime_error("Tried to write to unregistered value");
    }

    // look up value index:
    int vli = _valueToIndex[cli][valuename];

    // Check type
	if(_registeredTypes[cli][vli].compare("DOUBLE") != 0)
    {
        throw std::runtime_error("Tried to write wrong type");
    }

    // Write single value:
    writeChar((char)cli);
    writeChar((char)vli);
    writeDouble(value);

}
            
void Datalogger::writeHeader()
{
    if(!_blackbox.is_open())
        throw std::runtime_error("Tried to write to file, but file was closed");

    // We now generate the map of indices, 
    // since the loggin is officially starting
    if(_hasStarted)
        throw std::runtime_error("Logic error, never should write header on started file");

    for(unsigned int i = 0; i < _registeredClasses.size(); ++i)
    {
		_classToIndex[_registeredClasses[i]] = i;

		std::map<std::string, unsigned int> vmap;
		unsigned int vsize = _registeredValueNames[i].size();
		for(unsigned int j = 0; j < vsize; ++j)
    	{
            vmap[_registeredValueNames[i][j]] = j;
        }
        _valueToIndex.push_back(vmap);
    }


    // Start writing the file:
    writeInt(1024); // Magic number
    writeInt(1);    // version
    writeInt(0);    // reserved
    writeInt(0);    // reserved

    // Class index:
    // Size:
    unsigned int s = _registeredClasses.size();
    writeInt(s);
    for(unsigned int i = 0; i < s; ++i)
    {
        writeInt(i); // Class index
        writeInt(_registeredClasses[i].length()); // Lenght of name
        writeString(_registeredClasses[i]); // Name     
    }

    // Value Name indices
    s = _registeredValueNames.size();
	if(s != _registeredTypes.size() || s != _registeredClasses.size())
		throw std::runtime_error("Names and types size mismatch");
	writeInt(s); // Number of classes (again.. for robustness)
    for(unsigned int i = 0; i < s; ++i)
    {
        writeInt(i); // Class index
        unsigned int vsize = _registeredValueNames[i].size();
        writeInt(vsize); // number of values
		if(vsize != _registeredTypes[i].size())
			throw std::runtime_error("Names and types size mismatch");        
        for(unsigned int j = 0; j < vsize; ++j)
    	{
			writeInt(j); // value index
			writeInt(_registeredValueNames[i][j].length()); // length of name
			writeString(_registeredValueNames[i][j]); // Name
			writeInt(_registeredTypes[i][j].length()); // length of name
            writeString(_registeredTypes[i][j]); // Name

		}
    }






    
}

void Datalogger::dumpRegister()
{
    for(unsigned int i = 0; i < _registeredClasses.size(); ++i)
    {
        std::cout << i << ": " << _registeredClasses[i] << std::endl;

        unsigned int vsize = _registeredValueNames[i].size();
        for(unsigned int j = 0; j < vsize; ++j)
        {
            std::cout << "    " << j << ": " << _registeredValueNames[i][j] << std::endl;
        }
        
    }



}

int Datalogger::findClassIndex(const std::string& cl)
{
    for(unsigned int i = 0; i < _registeredClasses.size(); ++i)
    {
        if(_registeredClasses[i].compare(cl) == 0)
            return i;
    }

    return -1; 

}

int Datalogger::findValueIndex(unsigned int cli, const std::string& valueName)
{
    if(cli >= _registeredClasses.size() && cli >= _registeredValueNames.size())
        throw std::runtime_error("Class index exceeds registered class list size");

    for(unsigned int i = 0; i < _registeredValueNames[cli].size(); ++i)
    {
        if(_registeredValueNames[cli][i].compare(valueName) == 0) 
            return i;
    }

	return -1;
}

double Datalogger::getFrequency() const
{
    return _freq;
}

int Datalogger::getStep() const
{
    return _step;
}


void    Datalogger::writeInt(int val)
{
	_blackbox.write((const char*)&val, sizeof(val));
}

void    Datalogger::writeChar(char val)
{
	_blackbox.write((const char*)&val, sizeof(val));
}

void    Datalogger::writeDouble(double val)
{
	_blackbox.write((const char*)&val, sizeof(val));
}
void    Datalogger::writeString(const std::string& val)
{
	_blackbox.write(val.c_str(), val.length());
}





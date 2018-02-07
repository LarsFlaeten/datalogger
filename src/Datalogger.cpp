#include <iostream>
#include <sstream>

#include "Datalogger.h"

Datalogger::Datalogger(const std::string& blackbox)
#ifdef ORK_API
    : Object("Datalogger")
#endif
{
    _hasStarted = false;
    init(blackbox);

}

Datalogger::~Datalogger()
{
    std::cout << "Closing blackbox" << std::endl;
    _blackbox.close();
}

Datalogger::Datalogger()
#ifdef ORK_API
    : Object("Datalogger")
#endif
{
    _hasStarted = false;
}
            

void Datalogger::init(const std::string& blackbox)
{
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
    _step = 0;

    std::cout << "Opening blackbox \"" << blackbox << "\" for binary writing" << std::endl;
    _blackbox.open(blackbox, std::fstream::out | std::fstream::binary);

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
        // Check for duplicate:
        if(findValueIndex(cli, valuename) != -1)
            throw std::runtime_error("Valuename allready exists in class");

        _registeredValueNames[cli].push_back(valuename);
        _registeredTypes[cli].push_back(type);
    }

}
        
void Datalogger::tick(double t)
{
    ++_step;

    // First entry fir this step:
    writeValue("TIME", "t", t);
}

void Datalogger::writeValue(const std::string& cl, const std::string& valuename, int value)
{
    if(!_hasStarted)
    {
        writeHeader();
        _hasStarted = true;
    }

}
    
void Datalogger::writeValue(const std::string& cl, const std::string& valuename, double value)
{

    if(!_hasStarted)
    {
        writeHeader();
        _hasStarted = true;
    }

}
            
void Datalogger::writeHeader()
{
    if(!_blackbox.is_open())
        throw std::runtime_error("Tried to write to file, but file was closed");

    
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






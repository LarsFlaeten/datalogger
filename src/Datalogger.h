#ifndef _DATALOGGER_H_
#define _DATALOGGER_H_

#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <exception>

#ifndef ORK_API
class Datalogger
#else
class Datalogger: public ork::Object
#endif
{
public:
    Datalogger(const std::string& blackbox);
    virtual ~Datalogger();

    /*
     * Registers a value name attached to a class (cl).
     * A class (cl) is a way of grouping realted values, and can have multiple values attached to it, eg
     * cl = Position, valuename = x,y,z:
     * Position.x
     * Position.y
     * Position.z
     * Type can be "INT", "DOUBLE"
     *
     * Note, the logger will automatically register TIME.t, and populate this at every tick with t.
     * Users can add other values to the TIME class if they want, eg:
     * TIME.t (system generated)
     * TIME.jd
     * TIME.whatever
     *
     * Registration is only allowed before the logger has started (i.e. before first tick or write)
     *
     * To reduce file size, the cl and valuenames are indexed with unsigned chars into a database
     * in the header of the blackbox. This gives 0 to 255 possible cl, and 0 to 255 possible valuenames
     * in each cl.
     */
    void registerValueName(const std::string& cl, const std::string& valuename, const std::string& type);

    /*
     * Advances the datalogger to next step. Any subsequent writes will be assosciated with this time step
     * (until next tick() is called. t is written to TIME.t.
     * If the logger has not "started", it will start now (locks the registration)
     */
    void tick(double t);

    // Writes a value to registered cl.valuename.
    // It is not mandatory to write a value to all cl.valuename for each time step.
    // The methods will throw an Exception if the cl.valuename was not registered.
    // If the logger has not "startes", it will start now (locks the registration)
    void writeValue(const std::string& cl, const std::string& valuename, int value);
    void writeValue(const std::string& cl, const std::string& valuename, double value);

//protected:
    
    Datalogger();
    
	void	writeHeader();
    void init(const std::string& blackbox);

    // Locates a class index, registeredClasses is the class index list
	// returns -1 if class name is not found.
	int findClassIndex(const std::string& cl);
    
    // locates a valuename index in the _registeredValuenames, cli is class index
    // returns -1 if valuename is not found
    int findValueIndex(unsigned int cli, const std::string&);

    // Checks the type string to see if value is allowed:
    bool isAllowedType(const std::string& type);
private:
    std::fstream    _blackbox;  // The blackbox file we are writing to
    unsigned int    _step;
    std::vector<std::string> _registeredClasses;
    std::vector<std::vector<std::string>> _registeredValueNames;
    std::vector<std::vector<std::string>> _registeredTypes;
    std::map<std::string, unsigned int> _classToIndex;
    std::vector<std::map<std::string, unsigned int>> _valueToIndex;

    bool            _hasStarted;
};


#endif



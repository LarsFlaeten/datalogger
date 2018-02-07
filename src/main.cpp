#include <memory>
#include <iostream>

#include "Datalogger.h"

using namespace std;


int main(int argc, char** argv) {

    int suceeded = 0;
    int failed = 0;
    std::string test;
    std::shared_ptr<Datalogger> dl;


    test = "Initialization";
    try {
        dl = std::make_shared<Datalogger>("blackbox.dat");
    
        cout << "[TEST] " << test << ": PASSED" << endl;
        suceeded++;
    } catch (...)
    {
        cout << "[TEST] " << test << ": FAILED" << endl;
        failed++;
    } 

    test = "Try multiple value&type registration";
    try{
	    dl->registerValueName("TIME", "jd", "DOUBLE");
   		dl->registerValueName("POS", "x", "DOUBLE");
    	dl->registerValueName("POS", "y", "DOUBLE");
    	dl->registerValueName("POS", "z", "DOUBLE");    
    	dl->registerValueName("MASS", "m", "DOUBLE");
    	dl->registerValueName("AP_STATE", "state", "INT");

    	cout << "[TEST] " << test << ": PASSED" << endl;
        suceeded++;
    } catch (...)
    {
        cout << "[TEST] " << test << ": FAILED" << endl;
        failed++;
    }

	test = "Try wrong type registration";
    try{
        dl->registerValueName("SOMECLASS", "somevalue", "_wrong_type_");
        
        cout << "[TEST] " << test << ": FAILED" << endl;
        failed++;
    } catch (...)
    { 
        cout << "[TEST] " << test << ": PASSED" << endl;
        suceeded++;
    }

	test = "Try empty class registration";
    try{
        dl->registerValueName("", "somevalue", "INT");

        cout << "[TEST] " << test << ": FAILED" << endl;
        failed++;
    } catch (...)
    {
        cout << "[TEST] " << test << ": PASSED" << endl;
        suceeded++;
    }

	test = "Try empty value registration";
    try{
        dl->registerValueName("SOMECLASS", "", "DOUBLE");

        cout << "[TEST] " << test << ": FAILED" << endl;
        failed++;
    } catch (...)
    {
        cout << "[TEST] " << test << ": PASSED" << endl;
        suceeded++;
    }








}

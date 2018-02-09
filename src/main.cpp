#include <memory>
#include <iostream>

#include "Datalogger.h"

using namespace std;


int main(int argc, char** argv) {

    std::shared_ptr<Datalogger> dl;


    dl = std::make_shared<Datalogger>("blackbox.dat");
    
    dl->registerValueName("TIME", "jd", "DOUBLE");
    dl->registerValueName("POS", "x", "DOUBLE");
    dl->registerValueName("POS", "y", "DOUBLE");
    dl->registerValueName("INTS", "(Step*2)+3", "INT");

    int i = 0; 
    for(double t = 0.0; t < 400.0; t += 0.1)
    {
        dl->tick(t);

        dl->writeValue("TIME", "jd", 215000.5+t/60.0/60.0/24.0);
        dl->writeValue("POS", "x" , -500.0 + t);
        dl->writeValue("POS", "y" , -100.0 + t/50.0);
        dl->writeValue("INTS", "(Step*2)+3", i*2+3);

        dl->tock();
        i++;
    }






}

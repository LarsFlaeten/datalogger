#include <memory>
#include <iostream>
#include <cmath>

#include "Datalogger.h"

using namespace std;


int main(int argc, char** argv) {

    std::shared_ptr<Datalogger> dl;


    dl = std::make_shared<Datalogger>("blackbox.dat", 2.0);
    
    dl->registerValueName("TIME", "jd", "DOUBLE");
    dl->registerValueName("POS", "x", "DOUBLE");
    dl->registerValueName("POS", "y", "DOUBLE");
    dl->registerValueName("INTS", "(Step*2)+3", "INT");

    int i = 0; 
    for(double t = 0.0; t < 40.0; t += 0.01)
    {
        dl->tick(t);

        dl->writeValue("TIME", "jd", 215000.5+t/60.0/60.0/24.0);
        dl->writeValue("POS", "x" , 20.0*sin(t/20.0));
        dl->writeValue("POS", "y" , -100.0 + t/50.0);
        dl->writeValue("INTS", "(Step*2)+3", i*2+3);

        dl->tock();
        i++;
    }






}

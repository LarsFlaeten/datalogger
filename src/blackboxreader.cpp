#include <iostream>
#include <fstream>
#include <map>

#include <cxxopts.hpp>

using namespace std;

std::ifstream file;

int readInt(){
    int r;
    file.read((char*)&r, sizeof(int));
    return r;
}

double readDouble(){
    double r;
    file.read((char*)&r, sizeof(double));
    return r;
}

void errorExit(const std::string& msg)
{
    cerr << msg << endl;
    ::exit(-1);
}

int main(int argc, char** argv) {
    string exename(argv[0]);

    cxxopts::Options options(argv[0], " - blackbox reader");
	
	options
      .positional_help("[optional args]")
      .show_positional_help();


    options.add_options()
		("positional",
        "Positional arguments: these are the arguments that are entered "
"without an option", cxxopts::value<std::vector<std::string>>())
        ("a,arg", "Argument", cxxopts::value<std::string>())
    ;

    options.parse_positional({"inputfile", "positional"});
 
    auto result = options.parse(argc, argv);

    // Filename is mandatory
    std::string filename;
    if (result.count("positional"))
    {
		auto& v = result["positional"].as<std::vector<std::string>>();
        filename = v[0];
	} else
    {
        errorExit(options.help());
    }   

    file.open(filename.c_str(), ifstream::in | ifstream::binary);
    
    if(!file.is_open())
    {
        errorExit("Could not open file \"" + filename + "\" for reading");
    }

    // Start reading the file:
    int m = readInt();    
    int v = readInt();
    int r1 = readInt();
    int r2 = readInt();
    if(m!=1024)
        errorExit("Did not recognise blackbox file format");
    if(v==1){
        if(r1!=0 || r2!=0)
            errorExit("Fileformat error: v1 & v2 shall be 0 in version 1");
    } else
    {
        cerr << "version was " << v << endl;
        errorExit("Only blackbox version 1 supported in this app");
    }

	// Classes
    unsigned int nClasses = readInt();
    if(nClasses < 1 || nClasses>255)
        errorExit("!nClasses [0,255]");
    char buf[256];
	std::vector<std::string> classes;
	for(unsigned int i = 0; i < nClasses; ++i)
    {
        unsigned int ic = readInt(); // Class index
		if(ic != i) errorExit("Wrong class index");

        int cnl = readInt(); // Lenght of name
		if(cnl < 0 || cnl>255) errorExit("Class name was too long");
        file.read(buf, cnl);
		buf[cnl] = 0;
		classes.push_back(string(buf));
    }

	// Value names
	// Same number as classes:
	std::vector<std::vector<std::string>> valueNames;
	std::vector<std::vector<std::string>> valueTypes;
	unsigned int nc2 = readInt();
	if(nc2 != nClasses) errorExit("Wrong number of classes before values");
    unsigned int cli, namelen, typelen;
	for(unsigned int i = 0; i < nClasses; ++i)
    {  
		cli = readInt(); if(cli != i) errorExit("Wrong class index in values");

		unsigned int vsize = readInt();
		std::vector<std::string> classValueNames, classValueTypes;
		
		for(unsigned int j = 0; j < vsize; ++j)
		{
            unsigned int vti = readInt();
			if(vti!=j) errorExit("Wrong value/type index");
			namelen = readInt(); if(namelen>255) errorExit("value name too long");
			file.read(buf, namelen);
			buf[namelen] = 0;
			classValueNames.push_back(string(buf));

			typelen = readInt(); if(typelen>255) errorExit("Type name too long");
			file.read(buf, typelen);
            buf[typelen] = 0;
            classValueTypes.push_back(string(buf));
		}	
        valueNames.push_back(classValueNames);
        valueTypes.push_back(classValueTypes);
	}

    unsigned int i = 0,j;
   	for (auto cl = classes.begin(); cl != classes.end(); ++cl)
    {
        j = 0;
	    std::cout << *cl << " {" << i << "}" << endl;
        for(auto vl = valueNames[i].begin(); vl != valueNames[i].end(); ++vl)
        {
            cout << "  " << *vl << " [" << valueTypes[i][j] << "] {" << i << ", " << j << "}"  << endl;
            ++j;
        }
        ++i;
    }
	
    map<string, vector<int>> ivalues;
    map<string, vector<double>> dvalues;

    file.close();
}

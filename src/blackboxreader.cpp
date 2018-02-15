#include <iostream>
#include <fstream>
#include <map>

#include <cxxopts.hpp>

#include <Klv.h>

using namespace std;

std::ifstream file;

// No error checking here...
#if 0
void readKlvPacket(KlvPacket* klv, std::ifstream& file)
{
    file.read((char*)&(klv->key), 1);
    file.read((char*)&(klv->length), sizeof(unsigned int));
    char buf[klv->length];
    file.read(buf, klv->length);
    klv->value.clear();
    klv->value.insert(klv->value.end(), &buf[0], &buf[klv->length]);
}

void dumpKlvPacket(const KlvPacket& klv)
{
    cout << "Key: " << std::dec << klv.key << " (" << std::hex << klv.key << ")" << endl;
    cout << "len: " << std::dec << klv.length << " bytes" << endl;
    cout << "val: [" << std::hex;
    for(auto& c : klv.value)
        cout << c << ", ";
    cout << "]" << endl;

}


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
#endif
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

    // Read the file into memory:
    file.seekg(0, std::ios::end);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    //cout << "File size: " << size << " (" << size/1024 << "K)" << endl;

   
    sbuf_t  buffer;
    sbuf_create(&buffer, size); 
    if (!file.read((char*)buffer.data, size))
    {
        throw std::runtime_error("Could not read file (out of memory?)");
    }
    buffer.length = size;
    //cout << "Filebuf: size=" << buffer.size() << endl;
    
    int ptr = 0;
    uint8_t key;
    uint8_t BER[5];

    while(ptr < size)
    {
        // Get key
        key = *(buffer.data + ptr);
        ptr++;
        
        // get length
        int BER_len = 0;
        int val_len = klv::decode_BER(buffer.data + ptr, &BER_len);

        cout << "Key: " << (int)key << ", length: " << val_len << endl;
        
        // Get the payload
        sbuf_t value;
        sbuf_create_stack(&value, val_len);
        ptr+= BER_len;
        sbuf_add_data(&value, buffer.data + ptr, val_len);
        ptr += val_len;
    }


#if 0
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
#endif
#if 0
    KlvPacket klv;


    while(!file.eof())
    {
        readKlvPacket(&klv, file);
        switch(klv.key)
        {
            case(7):
                dumpKlvPacket(klv);
            default:
                break;

        }
    }

#endif

    file.close();

    return 0;
}

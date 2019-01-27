#include <iostream>
#include <string>
#include "rmvlib.h"


int main(int argc, char** argv){
	std::cout << "FWIPE V0.1!\n";
	if(argc <=1){ std::cout<<"No Args!\n"; return 1; }
	std::string fn = argv[1];
	std::cout << "Deleting " << fn << std::endl;
	fwipe wiper;
	wiper.wipe_file(fn.c_str(), 3);
}

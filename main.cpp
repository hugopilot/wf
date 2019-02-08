/* Code written by HugoPilot
 
 * Licensed under GPLv3
 
 */
#include <iostream>
#include <string>
#include "rmvlib.h"

#define WIPE_ROUNDS 3

void print_intro() {
	std::cout << "FWIPE V0.1!\n";
}

int main(int argc, char** argv){
	
	if(argc <=1){ std::cout<<"No Args!\n"; return 1; }
	std::string fn;
	for (int i = 1; i < argc; i++) {
		fn = argv[i];
		std::cout << "Deleting " << fn << std::endl;
		fwipe wiper;
		wiper.wipe_file(fn.c_str(), WIPE_ROUNDS);
	}
	
	std::cout << "\n\nWe are done here!\n";
	return 0;
}

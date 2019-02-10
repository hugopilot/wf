/* Code written by HugoPilot
 * Licensed under GPLv3
 */

#include <iostream>
#include <string.h>
#include <vector>
#include <experimental/filesystem>
#include "rmvlib.h"
namespace fs = std::experimental::filesystem;

#define WIPE_ROUNDS 3

enum PATH_STATE{
        ISFILE,
        ISDIR,
        DNE
};

void print_intro() {
	std::cout << "FWIPE V0.1!\n";
}

PATH_STATE check_path(const fs::path& path){
	if(!fs::exists(path))
                return DNE;
        if(fs::is_regular_file(path))
                return ISFILE;
        if(fs::is_directory(path))
                return ISDIR;
	return DNE;
}


void compile_args(int argc, char** argv, std::vector<std::string> *files, RMV_SETTINGS *settings, bool * noc){
	*noc = false;
	for(int i = 1; i < argc; i++){
			if(strcmp(argv[i], "-s") == 0){
				settings->sector_size = std::stoi(argv[i+1]);
				i++;
			}
			else if (strcmp(argv[i],"-i") == 0){
				settings->i_wipe = std::stoi(argv[i+1]);
				i++;
			}
			else if(strcmp(argv[i], "-y") == 0)
				*noc = true;
			else{
				PATH_STATE st = check_path(argv[i]);
				if(st == DNE){
					std::cout << "File " << argv[i] << " doesn't exist\n";
					return;
				}
				if(st == ISDIR){
					for(const auto & ppp: fs::recursive_directory_iterator(argv[i])){
						std::cout << "Including file: " << ppp.path() << std::endl;
						files->push_back(ppp.path());
						}
				}
				if(st == ISFILE){
					files->push_back(argv[i]);
				}	

			}
		}
}

int main(int argc, char** argv){
	
	// Initalize some variables	
	RMV_SETTINGS s;
	std::vector<std::string> f;
	bool conf = false;

	// If there are no arguments given: give error and exit (code 1)
	if(argc <=1){ std::cout<<"No Args!\n"; return 1; }

	// Parse/compile all those arguments
	compile_args(argc, argv, &f, &s, &conf);

	// If -y was given, skip the comfimation question
	if(!conf){
		std::cout << "Confirm the operation (Y/n): ";
		char resp;
		std::cin >> resp;
		if(resp != 'Y'){
			std::cout << "Operation cancelled!\n";
			return 100;}
	}

	// Loop though all the files.
	std::cout << "Let's fuck som files!\n";
	for(unsigned int c = 0; c < f.size(); c++){
		std::cout << "Deleting: " << f[c] << std::endl;
		fwipe::wipe_file(f[c].c_str(), s.i_wipe, s.sector_size);
		if(c != f.size() - 1)
			system("clear");
	}

	// Give an outro message and exit normally
	std::cout << "\n\nWe are done here!\n";
	return 0;
}

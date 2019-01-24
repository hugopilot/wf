#include <sstream>
#include <random>
#include <climits>
#include <ios>
#include "rmvlib.h"
#include <cmath>
#include <functional>
#include <algorithm>
#include <unistd.h>
#include <sys/stat.h>

#ifdef __linux__
#define FAKE_CMD "touch -d 2060-1-1"
#define DELETE_CMD "rm " 
#else
#error "Your OS is not supporting this library!"
#endif

#define SECTOR_SIZE 1024


/* Creates a random byte */
void rnd_char_gen(unsigned char **arr){
	
	std::random_device r;
        std::seed_seq seed{r(), r(), r(), r(), r(), r(), r(), r()};
	
	auto rand = std::bind(std::uniform_int_distribution<>(0, UCHAR_MAX),
            std::mt19937(seed));
	
	unsigned char *c = new unsigned char[SECTOR_SIZE];
	std::generate_n(c, SECTOR_SIZE, rand);
	*arr = c;
}

/* Gets the file size */
double fwipe::get_file_size(const char* filename){
	std::ifstream is;
	is.open(filename, std::ios::binary);
	is.seekg(0,std::ios::end);
	// Return sector count instead of the file size
	return ceil(is.tellg()/SECTOR_SIZE);
}

/* Overwrites the file */
void fwipe::overwrite_contents(const char* fName, unsigned int l){
	std::ofstream f_stream(fName, std::ios::binary);
	unsigned char *buf;
	for(unsigned int i = 0; i <= l; i++){
		rnd_char_gen(&buf);
		// Cast to keep ostream happy :D
		f_stream.write(reinterpret_cast<char*>(buf), SECTOR_SIZE);
		
		// Show the current sector
		std::cout<<"\rCurrent sector: " << i;
	}
	// When done, print two new lines
	std::cout << "\n\n";
}

std::ostringstream ss0;
/* Fake creation date */
void fwipe::fake_attributes(const char* filename){
	ss0 << FAKE_CMD;
	ss0 << " \"" << filename << "\"";
	
	if(system(ss0.str().c_str()) != 0){
		std::cout << "Did not succeed in fucking the metadata...try again in sudo mode\n";
	}	
}
inline bool check_file_exists (const std::string& name) {
  struct stat buffer;   
  return (stat (name.c_str(), &buffer) == 0); 
}

/* Deletes file contents */
void fwipe::set_zero_size(const char* filename){
	std::ofstream ofs;
	ofs.open(filename, std::ofstream::out | std::ofstream::trunc);
	ofs.close();
}

std::ostringstream ss1;

/* Public wipe function */
void fwipe::wipe_file(const char* filename, int i_wipe = 3){
	if(!check_file_exists(filename)){std::cout<<"File does not exist\n";return;}
	
	// First determain length
	unsigned int lt = get_file_size(filename);
	std::cout<<"Number of sectors (" << SECTOR_SIZE << " Bytes) = " << lt << std::endl;

	// Then overwrite it's contents
	for(int i = 0; i <= i_wipe; i++) { std::cout<<"OVERWRITE RUN " << i << " OUT OF " << i_wipe << std::endl; overwrite_contents(filename, lt); };
	fake_attributes(filename);
	set_zero_size(filename);
	ss1 << DELETE_CMD << "\"" << filename << "\"";
	system(ss1.str().c_str());
	std::cout<<"DONE!\n";

}


#include <sstream>
#include <random>
#include <climits>
#include <ios>
#include "rmvlib.h"
#include <cmath>
#include <functional>
#include <algorithm>

#define DEF_SECTOR_SIZE 512

#ifdef __linux__
#include <unistd.h>
#include <sys/stat.h>
#define FAKE_CMD "touch -d 2060-1-1"
#define DELETE_CMD "rm " 

/* Fake creation date */
void fwipe::fake_attributes(const char* filename) {
	std::ostringstream ss0;
	ss0 << FAKE_CMD;
	ss0 << " \"" << filename << "\"";

	if (system(ss0.str().c_str()) != 0) {
		std::cout << "Did not succeed in fucking the metadata...try again in sudo mode\n";
	}
}

#elif _WIN32
#include <direct.h>
#include <windows.h>
#include <time.h>
#define DELETE_CMD "del "
#define F_DAY 1
#define F_MONTH 1
#define F_YEAR 2060
void TimetToFileTime(time_t t, LPFILETIME pft)
{
	LONGLONG ll = Int32x32To64(t, 10000000) + 116444736000000000;
	pft->dwLowDateTime = (DWORD)ll;
	pft->dwHighDateTime = ll >> 32;
}


void fwipe::fake_attributes(const char* filename) {
	FILETIME ftime;
	/*SYSTEMTIME fake_time;
	fake_time.wDay = (WORD)F_DAY;
	fake_time.wYear = (WORD)F_YEAR;
	fake_time.wMonth = (WORD)F_MONTH;
	fake_time.wMilliseconds = NULL;
	fake_time.wMinute = NULL;
	fake_time.wSecond = NULL;
	fake_time.wHour = NULL;*/
	time_t fake_time = 2840140800;
	TimetToFileTime(fake_time, &ftime);
	HANDLE file;
	file = CreateFile(filename, FILE_WRITE_ATTRIBUTES, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	int r = 0;
	r = SetFileTime(file, &ftime, &ftime, &ftime);
	if (r == 0) {
		std::cout << "Did not succeed in fucking the metadata!\n HRESULT:" << HRESULT_FROM_WIN32(GetLastError()) << std::endl;
		CloseHandle(file);
	}
	CloseHandle(file);
	std::cout << r << std::endl;
}

#pragma message ( "WARNING: Windows is not officially supported!" )
#else
#error "Your OS is not supporting this library!"
#endif




/* Creates a random byte */
void rnd_char_gen(unsigned char **arr){
	
	std::random_device r;
        std::seed_seq seed{r(), r(), r(), r(), r(), r(), r(), r()};
	
	auto rand = std::bind(std::uniform_int_distribution<>(0, UCHAR_MAX),
            std::mt19937(seed));
	
	unsigned char *c = new unsigned char[DEF_SECTOR_SIZE];
	std::generate_n(c, DEF_SECTOR_SIZE, rand);
	*arr = c;
}

/* Gets the file size */
double fwipe::get_file_size(const char* filename){
	std::ifstream is;
	is.open(filename, std::ios::binary);
	is.seekg(0,std::ios::end);
	// Return sector count instead of the file size
	return ceil(is.tellg()/DEF_SECTOR_SIZE);
}

/* Overwrites the file */
void fwipe::overwrite_contents(const char* fName, unsigned int l){
	std::ofstream f_stream(fName, std::ios::binary);
	unsigned char *buf;
	for(unsigned int i = 0; i <= l; i++){
		rnd_char_gen(&buf);
		// Cast to keep ostream happy :D
		f_stream.write(reinterpret_cast<char*>(buf), DEF_SECTOR_SIZE);
		
		// Show the current sector
		std::cout<<"\rCurrent sector: " << i;
	}
	// When done, print two new lines
	std::cout << "\n\n";
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



/* Public wipe function */
void fwipe::wipe_file(const char* filename, int i_wipe = 3){
	if(!check_file_exists(filename)){std::cout<<"File does not exist\n";return;}
	
	// First determain length
	unsigned int lt = get_file_size(filename);
	std::cout<<"Number of sectors (" << DEF_SECTOR_SIZE << " Bytes) = " << lt << std::endl;

	// Then overwrite it's contents
	std::ostringstream ss1;
	for (int i = 1; i <= i_wipe; i++) { std::cout << "OVERWRITE RUN " << i << " OUT OF " << i_wipe << std::endl; overwrite_contents(filename, lt); };
	fake_attributes(filename);
	set_zero_size(filename);
	ss1 << DELETE_CMD << "\"" << filename << "\"";
	system(ss1.str().c_str());
	std::cout<<"DONE!\n";

}


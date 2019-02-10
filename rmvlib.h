/* ****************************************************
 * rmvlib is a simple library to safely delete files. *
 * It deletes files by overwriting them and changing *
 * the metadata before deleting it from the filetable *
 *						      *
 * Written by HugoPilot.			      *
 * ****************************************************/
#ifndef RMVLIB_H
#define RMVLIB_H
#include <iostream>
#include <fstream>

#define DEBUG(x) std::cout<<x
#define DEF_SECTOR_SIZE 512
#define DEF_ITERATIONS 3

struct RMV_SETTINGS{
	int sector_size = DEF_SECTOR_SIZE;
	int i_wipe = DEF_ITERATIONS;

};

class fwipe{
public:
	static void wipe_file(const char* filename, int i_wipe, int sector_size);
private:
	static double get_file_size(const char* filename);
	static void overwrite_contents(const char* fName, unsigned int l);
	static void fake_attributes(const char* filename);
	static void set_zero_size(const char* filename);
};

#endif

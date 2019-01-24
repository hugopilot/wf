/* ****************************************************
 * rmvlib is a simple library to safely delete files. *
 * It deletes files by overwriting them, and changing *
 * the metadata before deleting it from the filetable *
 *						      *
 * Written by HugoPilot.			      *
 * ****************************************************/
#ifndef RMVLIB_H
#define RMVLIB_H
#include <iostream>
#include <fstream>

#define DEBUG(x) std::cout<<x


class fwipe{
public:
	static void wipe_file(const char* filename, int i_wipe);
private:
	static double get_file_size(const char* filename);
	static void overwrite_contents(const char* fName, unsigned int l);
	static void fake_attributes(const char* filename);
	static void set_zero_size(const char* filename);
};

#endif

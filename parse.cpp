#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <string>	
#include <cstring>
#include "parse.h"

// Reads a .tsv or .csv. Each row of the file is an entry in the returned vector, and each
// value (separated by by the "separator" character(s), generally commas or tabs) of a given
// row is an entry in that row's vector. Each cell of the file is represented as a string.
std::vector<std::vector<std::string> > parse_sheet (const char* separator, std::string file_name) {
	std::vector<std::vector<std::string> > data; // will be returned by the function
	// Name of the file to be read.
	std::ifstream file(file_name.c_str());
	// If the file stream is bad then return an empty vector
	if (!file.good()) return data;
	std::string line;
	//unsigned int start = 0;
	//unsigned int threshold = 0;
	// Loop through the lines of the file
	while (!file.eof()) { // parse the flight log .tsv into "data"
		getline(file, line);
		// only start parsing after the threshold line of the file
		//if (start < threshold) {
		//	start++;
		//	continue;
		//}
		// This code splits the string that is the current line into a vector of
		// strings, separating by "separator"
		std::vector<std::string> row;
		char * cstr = new char [line.length()+1];
		std::strcpy (cstr, line.c_str());
		char * pch;
		pch = strtok (cstr,separator);
		for (unsigned int i = 0; pch != NULL; i++)
		{
			row.push_back(pch);
			pch = strtok (NULL, separator);
		}
		
		data.push_back(row); // add current line to "data"
	}
	return data;
}

// Take the name of a file and returns a vector where each item in
// the vector is a line 
std::vector<std::string> parse_sheet (std::string file_name) {
	std::vector<std::string> data; // will be returned by the function
	// File name of the file to be read.
	std::ifstream file(file_name.c_str());
	// If the file stream is bad then return an empty vector
	if (!file.good()) return data;
	std::string line;
	//unsigned int start = 0;
	//unsigned int threshold = 0;
	// Loop through the lines of the file
	while (!file.eof()) { // parse the file into "data"
		getline(file, line);
		// only start parsing after the threshold line of the file
		//if (start < threshold) {
		//	start++;
		//	continue;
		//}
		
		data.push_back(line); // add current line to "data"
	}
	return data;
}

// Checks if a string is a number.
bool is_number(const std::string& s) {
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}
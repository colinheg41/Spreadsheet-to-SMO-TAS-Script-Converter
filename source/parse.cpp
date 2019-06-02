/* Author: Colin Hegarty
 * Date: 5/25/19
 * This program contains functions, primarily for parsing purposes.
 */

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
	if (!file.good()) {
		std::cerr << "ERROR: " << file_name << " is missing" << std::endl;
		return data;
	}
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
	if (!file.good()) {
		std::cerr << "ERROR: " << file_name << " is missing" << std::endl;
		return data;
	}
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

// Finds the name of the .tsv in the "modify" file. If the name includes ".tsv" at the end,
// then remove it.
std::string get_tsv_name(std::string modify_file) {
	std::string file_name = check_data(modify_file)[1];
	return remove_tsv_extension(file_name);
}

// Removes the ".tsv" from the end of the file name if it is there
std::string remove_tsv_extension (std::string file_name) {
	if (file_name.length() > 4 && file_name.substr(file_name.length()-4,4) == ".tsv") {
		file_name = file_name.substr(0,file_name.length()-4);
	}
	return file_name;
}

// Checks that the "modify" file is properly formatted.
std::vector<std::string> check_data(std::string file_name) {
	std::vector<std::string> modify_data = parse_sheet(file_name);
	// Checks that there are enough lines in the file
	if (modify_data.size() < 13) {
		std::cerr << "ERROR: " << file_name << " does not have enough lines" << std::endl;
		exit (EXIT_FAILURE);
	}
	// Check that the parameters that should be numbers are, in fact, numbers
	for (unsigned int i = 8; i < 13; i +=2) {
		if (!is_number(modify_data[i])) {
			std::cerr << "ERROR: Line " << i+1 << " of " << file_name << " is not a number" << std::endl;
			exit (EXIT_FAILURE);
		}
	}
	
	return modify_data;
}
/* Author: Colin Hegarty
 * Date: 5/24/19
 * This program is used to parse the input spreadsheets and output a script
 * to use for SMO TAS.
 */
 
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <string>	
#include <cstring>
#include "parse.h"

std::vector<std::string> choose_files (std::string base_tsv);
void write_scripts (std::vector<std::string> file_names, std::string modify_file_name);
void convert_to_script (std::string table_file_name, std::string modify_file_name, std::string out_file_name);
std::string increment_string (std::string num);
void print_line (std::ofstream& file, const std::vector<std::string>& line);

int main (int argc, char* argv[]) {
	std::string modify_file = "modify.txt";
	std::string base_tsv = get_tsv_name(modify_file);
	write_scripts(choose_files(base_tsv), modify_file);
}

// Make a vector of files names for the .tsv's that will be converted.
// The first one is the name in the "modify" file and will be converted into
// script0.txt. Each .tsv after that is assumed to be the same as the first
// but have "(<i>)" at the end, where <i> starts with 1 and increments. Each one
// will be converted to script<i>.txt
std::vector<std::string> choose_files (std::string base_tsv) {
	std::vector<std::string> file_names;
	std::string file_name;
	// Checks up to 10 files (only 10 scripts are supported at a time)
	for (unsigned int i = 0; i < 10; ++i) {
		// Creates the file name for the current .tsv
		if (i == 0) {
			file_name = base_tsv + ".tsv";
		} else {
			file_name = base_tsv + "(" + std::to_string(i) + ").tsv";
		}
		// Checks that the file exists before adding it to the list
		std::ifstream file(file_name.c_str());
		if (file.good()) {
			file_names.push_back(file_name);
		}
		file.close();
	}
	return file_names;
}

// Loops through the .tsv's and converts each one to a script.
void write_scripts (std::vector<std::string> file_names, std::string modify_file_name) {
	for (unsigned int i = 0; i < file_names.size(); ++i) {
		convert_to_script(file_names[i], modify_file_name, "script" + std::to_string(i) + ".txt");
	}
}

// Executes the conversion process 
void convert_to_script (std::string table_file_name, std::string modify_file_name, std::string out_file_name) {
	// Each vector is a row, each string is a cell in that row.
	std::vector<std::vector<std::string> > table = parse_sheet("\t",table_file_name);
	//std::vector<std::vector<std::string> > insert = parse_sheet("\t",modify_file_name);
	std::ofstream file(out_file_name.c_str());
	//bool inserted = false;
	bool bad;
	//std::cout << insert.size();
	// Read the entries of table, and print the desired info for relevant flights
	for (unsigned int i = 0; i < table.size(); i++) {
		// Decide if it's a line with no inputs
		if (table[i].size() < 6) continue;
		// Decide if each entry of the line is or is not a number as appropriate
		bad = false;
		for (unsigned int j = 0; j < table[i].size(); ++j) {
			if (j != 1) {
				bad |= !is_number(table[i][j]);
			} else {
				bad |= is_number(table[i][j]);
			}
		}
		if (bad) continue;
		
		print_line(file, table[i]);
	}
}

// Print the line to the file with correct formatting for the script.
void print_line (std::ofstream& file, const std::vector<std::string>& line) {
	// Frame number
	file << line[0] << " ";
	// Button presses
	file << line[1] << " ";
	// Left stick then right stick
	file << line[2] << ";" << line[3] << " ";
	file << line[4] << ";" << line[5] << std::endl;
}

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
void write_scripts (std::vector<std::string> file_names, std::string insert_name);
void convert_to_script (std::string table_file_name, std::string insert_file_name, std::string out_file_name);
void add_line (std::ofstream& file, const std::vector<std::vector<std::string> >& insert);
std::string increment_string (std::string num);
void print_line (std::ofstream& file, const std::vector<std::string>& line);

int main (int argc, char* argv[]) {
	std::string base_tsv = "SMO TAS stuff by CMFP - Sheet1";
	write_scripts(choose_files(base_tsv), "insert.txt");
}

std::vector<std::string> choose_files (std::string base_tsv) {
	std::vector<std::string> file_names;
	std::string file_name;
	for (unsigned int i = 0; i < 10; ++i) {
		if (i == 0) {
			file_name = base_tsv + ".tsv";
		} else {
			file_name = base_tsv + "(" + std::to_string(i) + ").tsv";
		}
		std::ifstream file(file_name.c_str());
		if (!file.good()) {
			file.close();
			return file_names;
		}
		file.close();
		file_names.push_back(file_name);
	}
	return file_names;
}

void write_scripts (std::vector<std::string> file_names, std::string insert_name) {
	for (unsigned int i = 0; i < file_names.size(); ++i) {
		convert_to_script(file_names[i], insert_name, "script" + std::to_string(i) + ".txt");
	}
}

void convert_to_script (std::string table_file_name, std::string insert_file_name, std::string out_file_name) {
	// Each vector is a row, each string is a cell in that row.
	std::vector<std::vector<std::string> > table = parse_sheet("\t",table_file_name);
	std::vector<std::vector<std::string> > insert = parse_sheet("\t",insert_file_name);
	std::ofstream file(out_file_name.c_str());
	bool inserted = false;
	//std::cout << insert.size();
	// Read the entries of table, and print the desired info for relevant flights
	for (unsigned int i = 0; i < table.size(); i++) {
		// Decide if it's a line with no inputs
		bool blank = true;
		blank = table[i].size() < 6;
		if (blank) continue;
		
		print_line(file, table[i]);
	}
}

void print_line (std::ofstream& file, const std::vector<std::string>& line) {
	// Frame number
	file << line[0] << " ";
	// Button presses
	file << line[1] << " ";
	// Left stick then right stick
	file << line[2] << ";" << line[3] << " ";
	file << line[4] << ";" << line[5] << std::endl;
}

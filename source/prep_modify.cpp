/* Author: Colin Hegarty
 * Date: 5/25/19
 * This program takes a .tsv file and duplicates it, with each duplicate having a slight modification.
 * The intent is to take a spreadsheet file that will later be converted into a script for
 * TAS-nx, and generate other .tsv's that will be turned into scripts that have a slight variation
 * on the original. This is useful for testing many frames in a frame window for an input in one go.
 */
 
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <string>	
#include <cstring>
#include <algorithm>
#include <map>
#include <cstdio>
#include <cerrno>
#include <direct.h>
#include "parse.h"

void prep_modify(const Parameters& params);

int main (int argc, char* argv[]) {
	std::string modify_file = "settings.txt";
	Parameters params(modify_file);
	prep_modify(params);
}

void prep_modify(const Parameters& params) {
	std::vector<std::string> base_text;
	std::string tsv_folder = "";
	
	// If .tsv's are supposed to be put in a different folder, then move the original one and make the file
	// names of all .tsv's include the directory
	if (params.tsv_folder()) {
		tsv_folder += "tsv\\";
		_mkdir(tsv_folder.c_str());
		// Move original
		if(std::rename((params.base_tsv() + ".tsv").c_str(), (tsv_folder + params.base_tsv() + ".tsv").c_str()) < 0) {
			//std::cout << "errno: " << strerror(errno) << '\n';
		}
		// Remove the original .tsv from the original folder if it's still there
		if (remove((params.base_tsv() + ".tsv").c_str()) != 0) {
			//perror("Error deleting file");
			//std::cerr << "ERROR: failed to delete " << params.base_tsv() + ".tsv" << std::endl;
		}
	}
	
	// This is all of the text in the initial .tsv
	base_text = parse_sheet(tsv_folder + params.base_tsv() + ".tsv");
	// If the file wasn't there, then the table is empty and the program should end
	if (base_text.size() == 0) {
		return;
	}
	
	// Each iteration of the for loop generates a new .tsv that can be converted
	// to a script.
	for (unsigned int i = 1; i < params.num_scripts(); ++i) {
		// This is a copy of the text of the initial .tsv. It will be modified to fit
		// the current .tsv
		std::vector<std::string> text = base_text;
		std::string file_name = tsv_folder + params.base_tsv() + "(" + std::to_string(i) + ").tsv"; // File name of the current .tsv
		std::ofstream file(file_name.c_str()); // Write to current .tsv
		int num_shift = params.num_modifies()*i; // Number of frames that will be inserted/overwritten
		bool is_modified = false; // Check for the nested loop if the insertion/overwrite has already occurred
		// Loops through the lines of the vector representing the current .tsv
		for (unsigned int j = 0; j < text.size(); ++j) {
			// Make a substring of the frame number of the current line
			int tab_index = text[j].find("\t");
			std::string frame_str = text[j].substr(0,tab_index);
			// Skip the line if the first entry isn't a (frame) number
			if (!is_number(frame_str)) continue;
			int frame = std::stoi(frame_str); // turn frame number into an int
			// Checks if the loop is at the point that lines need to be modified
			if (!is_modified && frame >= params.modify_frame()) {
				is_modified = true;
				// Output the modification line to the file the correct number of times.
				for (unsigned int k = 0; k < num_shift; ++k) {
					// If overwriting, make the loop through the vector skip the same number
					// of lines as are being added by the modification.
					if (params.action() == "overwrite") {
						j++;
					}
					file << frame+k << "\t" << params.modify_text() << std::endl;
				}
			}
			// If inserting and the insertion has already happened, increase all remaining frame
			// numbers by the number of lines that were inserted.
			if (params.action() == "insert" && is_modified) {
				text[j] = std::to_string(frame+num_shift) + text[j].substr(tab_index);
			}
			// Output the current line to the .tsv
			file << text[j] << std::endl;
		}
		file.close();
	}
}
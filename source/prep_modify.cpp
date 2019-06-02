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
#include "parse.h"

// Checks that either overwrite or insert were chosen on the command line.
// Defaults to insert if both or neither were chosen.
/*#if defined INSERT && defined OVERWRITE
#undef OVERWRITE
#endif
#if !defined INSERT && !defined OVERWRITE
#define INSERT
#endif*/

int main (int argc, char* argv[]) {
	std::string modify_file = "modify.txt";
	// This is all of the text in the "modify" file
	std::vector<std::string> modify_data = check_data(modify_file);
	std::vector<std::string> base_text;
	// These are all of the parameters specified in the "modify" file
	std::string base_tsv = modify_data[1];
	std::string action = modify_data[4]; // Either "overwrite" or "insert"
	std::string modify_text = modify_data[6];
	int modify_frame = std::stoi(modify_data[8]);
	int num_modifies = std::stoi(modify_data[10]);
	int num_scripts = std::stoi(modify_data[12]);
	
	// Make action lowercase
	std::transform(action.begin(), action.end(), action.begin(), ::tolower);
	// If the name of the .tsv in the "modify" file has ".tsv" at the end, remove it.
	base_tsv = remove_tsv_extension(base_tsv);
	// This is all of the text in the initial .tsv
	base_text = parse_sheet(base_tsv + ".tsv");
	
	// Each iteration of the for loop generates a new .tsv that can be converted
	// to a script.
	for (unsigned int i = 1; i < num_scripts; ++i) {
		// This is a copy of the text of the initial .tsv. It will be modified to fit
		// the current .tsv
		std::vector<std::string> text = base_text;
		std::string file_name = base_tsv + "(" + std::to_string(i) + ").tsv"; // File name of the current .tsv
		std::ofstream file(file_name.c_str()); // Create/overwrite current .tsv
		int num_shift = num_modifies*i; // Number of frames that will be inserted/overwritten
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
			if (!is_modified && frame >= modify_frame) {
				is_modified = true;
				// Output the modification line to the file the correct number of times.
				for (unsigned int k = 0; k < num_shift; ++k) {
					// If overwriting, make the loop through the vector skip the same number
					// of lines as are being added by the modification.
					if (action == "overwrite") {
						j++;
					}
					file << frame+k << "\t" << modify_text << std::endl;
				}
			}
			// If inserting and the insertion has already happened, increase all remaining frame
			// numbers by the number of lines that were inserted.
			if (action == "insert" && is_modified) {
				text[j] = std::to_string(frame+num_shift) + text[j].substr(tab_index);
			}
			// Output the current line to the .tsv
			file << text[j] << std::endl;
		}
	}
}
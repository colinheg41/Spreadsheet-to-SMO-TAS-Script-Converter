/* Author: Colin Hegarty
 * Date: 5/25/19
 * 
 */
 
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <string>	
#include <cstring>
#include "parse.h"

int main (int argc, char* argv[]) {
	std::string base_tsv = "SMO TAS stuff by CMFP - Sheet1";
	std::vector<std::string> base_text = parse_sheet(base_tsv + ".tsv");
	std::vector<std::string> insert_data = parse_sheet("insert.txt");
	std::string insert_text = insert_data[1];
	int insert_frame = std::stoi(insert_data[3]);
	int num_inserts = std::stoi(insert_data[5]);
	int num_scripts = std::stoi(insert_data[7]);
	
	for (unsigned int i = 1; i < num_scripts; ++i) {
		std::vector<std::string> text = base_text;
		std::string file_name = base_tsv + "(" + std::to_string(i) + ").tsv";
		std::ofstream file(file_name.c_str());
		int num_shift = num_inserts*i;
		bool is_inserted = false;
		for (unsigned int j = 0; j < text.size(); ++j) {
			int tab_index = text[j].find("\t");
			std::string frame_str = text[j].substr(0,tab_index);
/*			if (i == 1) {
				std::cout << "\"" << text[j] << "\"" << std::endl;
				std::cout << "\"" << frame_str << "\"" << std::endl;
			}*/
			if (!is_number(frame_str)) continue;
			int frame = std::stoi(frame_str);
			if (!is_inserted && frame >= insert_frame) {
				is_inserted = true;
				for (unsigned int k = 0; k < num_shift; ++k,++j) {
					file << frame+k << "\t" << insert_text << std::endl;
				}
			}
			
			file << text[j] << std::endl;
		}
	}
}
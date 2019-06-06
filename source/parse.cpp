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
#include <algorithm>
#include <map>
#include "parse.h"

//#define NEW_FEATURES

// Check that the "modify" file is configured correctly, and then assign
// values to all the variables based on the data in the file.
Parameters::Parameters(std::string file_name) {
	// Set the lines in the "modify" file where each parameter should be
	ln = make_line_nums();
	
	// Set the options for the parameters that have various options
	options_ = make_options();
	
	// Read the "modify" file and make sure it's properly formatted
	std::vector<std::string> data = check_data(file_name, ln, options_);
	
	// Assign all the parameters to variables
	base_tsv_ = remove_tsv_extension(data[ln["base_tsv"]]);
	action_ = data[ln["action"]];
	modify_text_ = data[ln["modify_text"]];
	modify_frame_ = std::stoi(data[ln["modify_frame"]]);
	num_modifies_ = std::stoi(data[ln["num_modifies"]]);
	num_scripts_ = std::stoi(data[ln["num_scripts"]]);
	delete_tsv_ = data[ln["delete_tsv"]];
	// These features still need to be implemented
	#ifdef NEW_FEATURES
	reverse_modify_ = lowercase(data[ln["reverse_modify"]]) == "yes";
	tsv_folder_ = lowercase(data[ln["tsv_folder"]]) == "yes";
	script_folder_ = lowercase(data[ln["script_folder"]]) == "yes";
	modify_before_conversion_ = lowercase(data[ln["modify_before_conversion"]]) == "before"
							|| lowercase(data[ln["modify_before_conversion"]]) == ".tsv";
	reverse_convert_ = lowercase(data[ln["reverse_convert"]]) == "yes";
	#else
	reverse_modify_ = "no";
	tsv_folder_ = "no";
	script_folder_ = "no";
	modify_before_conversion_ = "before";
	reverse_convert_ = "no";
	#endif
	file_length_ = ln["file_length"];
}

// Set the lines in the "modify" file where each parameter should be
std::map<std::string,unsigned int> make_line_nums() {
	std::map<std::string,unsigned int> line_nums;
	line_nums["base_tsv"] = 1;
	line_nums["action"] = line_nums["base_tsv"]+3;
	line_nums["modify_text"] = line_nums["action"]+2;
	line_nums["modify_frame"] = line_nums["modify_text"]+2;
	line_nums["num_modifies"] = line_nums["modify_frame"]+2;
	line_nums["num_scripts"] = line_nums["num_modifies"]+2;
	
	line_nums["delete_tsv"] = line_nums["num_scripts"]+4;
	#ifdef NEW_FEATURES
	line_nums["reverse_modify"] = line_nums["delete_tsv"]+3;
	line_nums["tsv_folder"] = line_nums["reverse_modify"]+3;
	line_nums["script_folder"] = line_nums["tsv_folder"]+3;
	line_nums["modify_before_conversion"] = line_nums["script_folder"]+3;
	line_nums["reverse_convert"] = line_nums["modify_before_conversion"]+3;
	line_nums["file_length"] = line_nums["reverse_convert"]+1;
	#else
	line_nums["file_length"] = line_nums["num_scripts"]+1;
	#endif
	
	return line_nums;
}

// Set the options for the parameters that have various options
std::map<std::string,std::vector<std::string> > make_options () {
	std::map<std::string,std::vector<std::string> > options;
	std::vector<std::string> temp;
	
	options["action"] = {"insert", "overwrite"};
	
	options["delete_tsv"] = {"all", "none", "all except original"};
	#ifdef NEW_FEATURES
	options["reverse_modify"] = {"yes", "no"};
	options["tsv_folder"] = {"yes", "no"};
	options["script_folder"] = {"yes", "no"};
	options["modify_before_conversion"] = {"script", "after", ".tsv", "before"};
	options["reverse_convert"] = {"yes", "no"};
	#endif
	
	return options;
}

// Make a string be all lowercase
std::string lowercase (std::string str) {
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	return str;
}

// Splits a string at each instance of "separator"
std::vector<std::string> split_string(std::string line, std::string delimiter) {
	std::vector<std::string> row;
	size_t pos = 0;
	std::string token;
	while ((pos = line.find(delimiter)) != std::string::npos) {
		token = line.substr(0, pos);
		row.push_back(token);
		line.erase(0, pos + delimiter.length());
	}
	row.push_back(line);
	
	return row;
}

// Check if a line from a .tsv is properly formatted
bool good_tsv_line(std::string line) {
	return good_tsv_line(split_string(line,"\t"));
}

// Check if a line from a .tsv is properly formatted
bool good_tsv_line(const std::vector<std::string>& inputs) {
	if (inputs.size() < 6) return false;
	if (!is_number(inputs[0])) return false;
	for (unsigned int i = 1; i < 6; ++i) {
		if (inputs[i] == "") return false;
	}
	
	return true;
}

// Check if a line from a .tsv is properly formatted
bool good_script_line(std::string line) {
	// needs implementation
	return true;
}

// Reads a .tsv or .csv. Each row of the file is an entry in the returned vector, and each
// value (separated by by the "separator" character(s), generally commas or tabs) of a given
// row is an entry in that row's vector. Each cell of the file is represented as a string.
std::vector<std::vector<std::string> > parse_sheet (std::string delimiter, std::string file_name) {
	std::vector<std::vector<std::string> > data;
	// make a vector of each line of the file
	std::vector<std::string> mid_data = parse_sheet(file_name);
	// Split the each line by the delimiter
	for (unsigned int i = 0; i < mid_data.size(); ++i) {
		data.push_back(split_string(mid_data[i], delimiter));
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
	// Loop through the lines of the file
	while (!file.eof()) { // parse the file into "data"
		getline(file, line);
		data.push_back(line); // add current line to "data"
	}
	file.close();
	return data;
}

// Checks if a string is a number.
bool is_number(const std::string& s) {
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

// Removes the ".tsv" from the end of the file name if it is there
std::string remove_tsv_extension (std::string file_name) {
	if (file_name.length() > 4 && file_name.substr(file_name.length()-4,4) == ".tsv") {
		file_name = file_name.substr(0,file_name.length()-4);
	}
	return file_name;
}

// Checks if the string is in the list of strings
bool is_string_in (const std::vector<std::string>& list, std::string str) {
	bool is_in = false;
	for (unsigned int i = 0; i < list.size(); ++i) {
		if (list[i] == str) {
			is_in = true;
		}
	}
	return is_in;
}

// Turns a vector of strings into a single string in list form
std::string to_string(const std::vector<std::string>& list, std::string separator) {
	std::string str = "";
	for (unsigned int i = 0; i < list.size(); ++i) {
		if (i != 0) {
			str += separator;
		}
		str += "\"" + list[i] + "\"";
	}
}

// Checks that the "modify" file is properly formatted.
std::vector<std::string> check_data(std::string file_name,
			std::map<std::string,unsigned int>& ln,
			std::map<std::string,std::vector<std::string> >& options_map) {
	
	std::vector<std::string> check;
	std::vector<std::string> options;
	std::vector<std::string> data = parse_sheet(file_name);
	bool bad = false;
	int line_num;
	
	// 'base_tsv' doesn't need checking
	
	// Checks that there are enough lines in the file
	if (data.size() < ln["file_length"]) {
		fail("lines", file_name + " does not have enough lines", data.size()-1);
	}
	
	// Check that 'modify_text' is good
	line_num = ln["modify_text"];
	if (!good_tsv_line(data[line_num]) && !good_script_line(data[line_num])) {
		fail("inputs", "", line_num+1);
	}
	
	// Check that the parameters that are supposed to be numbers are positive ints
	check = {"modify_frame", "num_modifies", "num_scripts"};
	for (unsigned int i = 0; i < check.size(); ++i) {
		line_num = ln[check[i]];
		if (!is_number(data[line_num])) {
			fail("number", "", line_num+1);
		}
	}
	
	// Check that the parameters with various options are one of the allowed options
	#ifdef NEW_FEATURES
	check = {"action", "delete_tsv", "reverse_modify", "tsv_folder", "script_folder", "modify_before_conversion", "reverse_convert"};
	#else
	check = {"action", "delete_tsv"};
	#endif
	for (unsigned int i = 0; i < check.size(); ++i) {
		line_num = ln[check[i]];
		options = options_map[check[i]];
		data[line_num] = lowercase(data[line_num]);
		if (!is_string_in(options, data[line_num])) {
			fail("option", to_string(options, ", "), line_num+1);
		}
	}
	
	return data;
}

// Outputs an error message and exits the program	
void fail (std::string condition, std::string message, int line_num) {
	std::cerr << "ERROR: ";
	if (condition == "lines") {
		std::cerr << message;
	} else if (condition == "option") {
		std::cerr << "Line " << line_num << " is not one of the allowed options: " << message;
	} else if (condition == "inputs") {
		std::cerr << "Line " << line_num << " is not a proper line of inputs";
	} else if (condition == "number") {
		std::cerr << "Line " << line_num << " is not a number";
	} else {
		std::cerr << "unknown error";
	}
	
	std::cerr << std::endl;
	exit (EXIT_FAILURE);
}
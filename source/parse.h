// File:	parse.h
// Purpose:	Header for functions that are primarily for parsing. This file
// 			also contains the Parameters class.

#ifndef __parse_h__
#define __parse_h__

class Parameters {
public:
	// Constructors
	Parameters(std::string file_name);

	// Accessors
	const std::map<std::string,unsigned int>& line_nums() const { return ln; }
	const std::map<std::string,std::vector<std::string> >& options() const { return options_; }
	const std::string& base_tsv() const { return base_tsv_; }
	const std::string& action() const { return action_; }
	const std::string& modify_text() const { return modify_text_; }
	const int& modify_frame() const { return modify_frame_; }
	const int& num_modifies() const { return num_modifies_; }
	const int& num_scripts() const { return num_scripts_; }
	const std::string& delete_tsv() const { return delete_tsv_; }
	const bool& reverse_modify() const { return reverse_modify_; }
	const bool& tsv_folder() const { return tsv_folder_; }
	const bool& script_folder() const { return script_folder_; }
	const bool& modify_before_conversion() const { return modify_before_conversion_; }
	const bool& reverse_convert() const { return reverse_convert_; }
	const int& file_length() const { return file_length_; }

private: // Representation
	std::map<std::string,unsigned int> ln; // line numbers of each parameter
	std::map<std::string,std::vector<std::string> > options_; // line numbers of each parameter
	std::string base_tsv_; // .tsv name without extension
	std::string action_; // "insert" or "overwrite"
	std::string modify_text_; // the line of inputs
	int modify_frame_; // frame to start modification at
	int num_modifies_; // number of times to do modification per script
	int num_scripts_; // number of scripts to make
	std::string delete_tsv_; // to delete .tsv's on conversion: all, none, all except original
	bool reverse_modify_; // do modification going up the inputs instead of down?
	bool tsv_folder_; // make a separate folder for the .tsv files?
	bool script_folder_; // make a separate folder for the scripts?
	bool modify_before_conversion_; // modify the inputs before converting the scripts?
	bool reverse_convert_; // convert scripts to .tsv's instead of .tsv's to scripts
	int file_length_; // expected number of lines in the "modify" file
};

std::map<std::string,unsigned int> make_line_nums();
std::map<std::string,std::vector<std::string> > make_options();
std::string lowercase (std::string str);
std::vector<std::string> split_string(std::string line, std::string delimiter);
bool good_tsv_line(std::string line);
bool good_tsv_line(const std::vector<std::string>& inputs);
bool good_script_line(std::string line);
std::vector<std::vector<std::string> > parse_sheet (std::string delimiter, std::string file_name);
std::vector<std::string> parse_sheet (std::string file_name);
bool is_number (const std::string& s);
std::string remove_tsv_extension (std::string file_name);
bool is_string_in (const std::vector<std::string>& list, std::string str);
std::string to_string(const std::vector<std::string>& list, std::string separator);
std::vector<std::string> check_data(std::string file_name,
			std::map<std::string,unsigned int>& ln,
			std::map<std::string,std::vector<std::string> >& options_map);
void fail (std::string condition, std::string message, int line_num);

#endif
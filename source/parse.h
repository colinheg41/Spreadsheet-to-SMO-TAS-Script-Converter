#ifndef __parse_h__
#define __parse_h__

std::vector<std::vector<std::string> > parse_sheet (const char* separator, std::string file_name);
std::vector<std::string> parse_sheet (std::string file_name);
bool is_number (const std::string& s);
std::string get_tsv_name(std::string modify_file);
std::string remove_tsv_extension (std::string file_name);
std::vector<std::string> check_data(std::string file_name);

#endif
#ifndef __parse_h__
#define __parse_h__

std::vector<std::vector<std::string> > parse_sheet (const char* separator, std::string file_name);
std::vector<std::string> parse_sheet (std::string file_name);
bool is_number(const std::string& s);

#endif
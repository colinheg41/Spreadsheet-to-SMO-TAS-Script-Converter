/* Author: Colin Hegarty
 * Date: 6/2/19
 * 
 */
 
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdio.h>
#include <string>	
#include <cstring>

std::vector<std::string> get_list ();
void print_list (std::ofstream& file, const std::vector<std::string>& list);
void make_script (std::string IP, std::string lcd);

int main (int argc, char* argv[]) {
	std::string IP = argv[1];
	std::string lcd = argv[2];
	make_script(IP, lcd);
}

std::vector<std::string> get_list () {
	std::vector<std::string> file_list;
	std::string file_name;
	for (unsigned int i = 0; i < 10; ++i) {
		file_name = "script" + std::to_string(i) + ".txt";
		std::ifstream file(file_name.c_str());
		if (file.good()) {
			file_list.push_back(file_name);
		}
		file.close();
	}
	return file_list;
}

void print_list (std::ofstream& file, const std::vector<std::string>& list) {
	for (unsigned int i = 0; i < list.size(); ++i) {
		file << "put " << list[i] << std::endl;
	}
}

void make_script (std::string IP, std::string lcd) {
	IP += ":5000";
	std::string script_name = "script_for_upload.txt";
	std::string beginning = "open ftp://a@" + IP + "/\n\nlcd \"" + lcd + "\"\ncd /scripts\n";
	std::string end = "\nexit\n\n# Execute the script using a command like:\n# \"C:\\Program Files (x86)\\WinSCP\\WinSCP.com\" /log=WinSCP.log /ini=nul /script=script_for_upload.txt";
	std::ofstream file(script_name.c_str());
	//std::ofstream file(out_file_name.c_str());
	
	file << beginning << std::endl;
	print_list(file, get_list());
	file << end << std::endl;
	file.close();
}
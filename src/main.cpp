/***
* Sean Vogel
* z1729629
* CSCI-490-D
* Assignment 3 - ID3 Decision Tree
*
*/
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <utility>
#include <cctype>
#include <algorithm>
#include <id3_tree.hpp>

/**
* Split string on specified delimiter
* str - string to split
* delim - delimiter
*/
std::vector<std::string> str_split(std::string str, std::string delim)
{
	std::vector<std::string> v;
	std::size_t start = str.find_first_not_of(delim);
	std::size_t end = str.find_first_of(delim);

	while (start != std::string::npos)
	{
		std::string s(str.substr(start, end - start));
		std::transform(s.begin(), s.end(), s.begin(), ::tolower);
		v.push_back(s);
		start = str.find_first_not_of(delim, end);
		end = str.find_first_of(delim, start);
	}
	return std::move(v);
}

int main()
{
	std::ifstream ifs("input.txt");

	std::string str;
	std::getline(ifs, str);

	//get the feature names
	std::size_t start = str.find_first_not_of(" \t", str.find_first_of(" \t"));
	std::vector<std::string> features = str_split(str.substr(start, str.find_last_of(" \t") - start), " \t");
	
	//build vector of sample data
	sv::sample s;
	std::vector<sv::sample> data;

	while (ifs >> s.id && std::getline(ifs, str))
	{
		s.label = str.substr(str.find_last_of(" \t")+ 1);
		std::size_t start = str.find_first_not_of(" \t");
		s.feature_values = str_split(str.substr(start, str.find_last_of(" \t") - start), " \t");
		data.push_back(s);
	}	

	std::ofstream of("output.txt");

	//create/build tree
	sv::id3_tree id3(data, features);
	id3.print_tree(of);

	//test tree
	of << "\n ********** Testing *********" << std::endl;
	of << " Testing case: Sunny Mild Normal Strong" << std::endl;
	int r = id3.apply_tree({ { "sunny", "mild", "normal", "strong" } }, of);
	of << " Will" << ((r == -1)?" not":"") << " play tennis on such a day" << std::endl;

	of << "\n ********** Testing *********" << std::endl;
	of << " Testing case: Rain Mild Normal Strong" << std::endl;
	int r2 = id3.apply_tree({ { "rain", "mild", "normal", "strong" } }, of);//don't play tennis
	of << " Will" << ((r2 == -1) ? " not" : "") << " play tennis on such a day" << std::endl;
}

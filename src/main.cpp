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

	//create/build tree
	sv::id3_tree id3(data, features);

	int r1 = id3.apply_tree({ {"rain", "mild", "normal", "strong"} });//no
	int r2 = id3.apply_tree({ { "sunny", "mild", "normal", "strong" } });//yes
	int r3 = id3.apply_tree({ { "overcast", "mild", "normal", "strong" } });//yes
	int r4 = id3.apply_tree({ { "sunny", "hot", "high", "strong" } });//no
	int r5 = id3.apply_tree({ { "rain", "mild", "normal", "weak" } });//yes

	std::cout << "Test 1: " << r1 << ", Test 2: " << r2 << ", Test 3: " << r3 << std::endl;
	std::cout << "Test 4: " << r4 << ", Test 5: " << r5 << std::endl;
}

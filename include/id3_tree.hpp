#pragma once
#ifndef SV_ID3TREE_HPP
#define SV_ID3TREE_HPP

#include <vector>
#include <memory>
#include <map>
#include <set>
#include <utility>
#include <iostream>
#include <node.hpp>

namespace sv
{
	/**
	* Represent a sample
	*/
	struct sample
	{
		sample() {}
		sample(std::vector<std::string> sv) : feature_values(sv) {}
		std::string id;
		std::vector<std::string> feature_values;
		std::string label;
	};

	/**
	*
	*
	*/
	class id3_tree
	{		
		std::unique_ptr<node> root;				
		std::vector<sample> data;
		std::vector<std::string> features;
		std::map<std::string, std::set<std::string>> feature_to_values;
		
	public:
		/**
		* Constructors
		*/
		id3_tree() = delete;//not default constructable
		id3_tree(int p, int n, int s);
		id3_tree(std::vector<sample> d, std::vector<std::string> f); 
		
		/**
		* Add a child node
		*/
		void add_node(int p, int n);

		/**
		* Build the tree
	    * d - set of samples
		* f - reduced feature set
		*/
		std::unique_ptr<node> build_tree(std::vector<sample> d, std::vector<std::string> f);		

		/**
		* Find the maximum gain value of specified features
		* d - set of sample data
		* f - feature set
		* pos - number positive values
		* neg - number of negative values
		*/
		std::string best_gain(const std::vector<sample>& d, const std::vector<std::string>& f, int pos, int neg);		

		/**
		* Print the tree structure to stdout
		*/
		void print_tree(std::ostream& os = std::cout);
		void print_tree(node* r, std::ostream& os = std::cout);

		/**
		* Apply a sample to tree
		* s - sample to test
		*/
		int apply_tree(sample s, std::ostream& os = std::cout);						

	private:
		int apply_tree(node* r, const sample s, std::ostream& os = std::cout);
	};
}
#endif

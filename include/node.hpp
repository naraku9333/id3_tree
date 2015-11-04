#pragma once
#ifndef SV_NODE_HPP
#define SV_NODE_HPP

#include <vector>
#include <string>
#include <utility>
#include <memory>

/**
* Tree node
*/
namespace sv
{
	struct node
	{
		std::vector<std::unique_ptr<node>> children;
		std::vector<std::string> branches;

		int pos, neg;//+ and - sample counts
		int num_samples;//total samples
		int label;//terminal node class label +, - or 0 if not terminal
		std::string split_rule_name;
		int split_rule_index = -1;
		int node_number;

		/**
		* Constructors
		*/
		node() : children(), pos(0), neg(0), num_samples(0), label(0) {}
		node(int p, int n, int num = 0, int l = 0);

		void add_node(int p, int n);

		/**
		* Calculate this nodes entropy
		*/
		float entropy();

		/**
		* Gain = E(root) - (Sv/S)E(f1) - (Sv/S)E(f2) ...
		* Calculate this subtree's information gain
		*/
		float gain();
	};
}
#endif


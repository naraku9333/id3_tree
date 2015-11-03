#include <id3_tree.hpp>
#include <vector>
#include <cstdint>
#include <memory>
#include <cmath>
#include <iostream>
#include <cassert>
#include <map>
#include <set>
#include <utility>
#include <algorithm>
#include <node.hpp>

sv::id3_tree::id3_tree(int p, int n, int s) : root(new node(p, n, s)) {}

sv::id3_tree::id3_tree(std::vector<sample> d, std::vector<std::string> f) : data(std::move(d)), features(std::move(f))
{
	//map features to set of values
	for (std::size_t i = 0; i < features.size(); ++i)
	{
		for (const auto& s : data) { feature_to_values[features[i]].insert(s.feature_values[i]); }
	}

	root = build_tree(data, features);
	int x = 0;
}

void sv::id3_tree::add_node(int p, int n) { root->add_node(p, n); }

std::unique_ptr<sv::node> sv::id3_tree::build_tree(std::vector<sample> d, std::vector<std::string> f)
{
	int pos = 0, neg = 0;
	for (const auto& s : d)
	{
		if (s.label == "Yes") ++pos;
		else ++neg;
	}

	//steps 1, 2 and 3			
	if (pos == d.size()) { return std::unique_ptr<node>(new node(pos, neg, 1)); }
	else if (neg == d.size()) { return std::unique_ptr<node>(new node(pos, neg, -1)); }

	//step 4
	if (f.empty()) { return std::unique_ptr<node>(new node(pos, neg, (pos > neg) ? 1 : -1)); }

	std::string selected = best_gain(d, f, pos, neg);

	std::unique_ptr<node> r(new node(pos, neg));
	r->split_rule_name = selected;

	std::size_t i = std::find(features.begin(), features.end(), selected) - features.begin();//get selected feature index

	for (const auto& v : feature_to_values[selected])
	{
		auto it = std::partition(d.begin(), d.end(), [&](const sample& s)->bool { return s.feature_values[i] == v; });
		r->children.push_back(build_tree({ d.begin(), it }, f));
		r->branches.push_back(v);
		r->split_rule_index = i;
	}

	f.erase(std::find(f.begin(), f.end(), selected));//remove already chosen feature
	return std::move(r);//return selected subtree
}

std::string sv::id3_tree::best_gain(const std::vector<sample>& d, const std::vector<std::string>& f, int pos, int neg)
{
	//KEY: feature, VALUE: { KEY: feature_value, VALUE: { pos, neg } }
	std::map<std::string, std::map<std::string, std::pair<int, int>>> values;
	std::map<float, std::string> gains;//map gain value to feature

	for (std::size_t i = 0; i < f.size(); ++i)//foreach feature
	{
		const auto& fi = f[i];
		std::unique_ptr<node> t(new node(pos, neg));

		for (const auto& s : d)//count pos and neg values
		{
			if (s.label == "Yes")values[fi][s.feature_values[i]].first++;
			else values[fi][s.feature_values[i]].second++;
		}
		for (const auto& m : values[fi])//create temp tree and calc gains
		{
			t->add_node(m.second.first, m.second.second);
		}
		gains[t->gain()] = fi;
	}

	std::string selected = gains.rbegin()->second;//get the selected feature
	
	return selected;
}

int sv::id3_tree::apply_tree(sample s, std::ostream& os)
{
	for (auto& str : s.feature_values) { std::transform(str.begin(), str.end(), str.begin(), ::tolower); }
	return apply_tree(root.get(), s, os);
}

int sv::id3_tree::apply_tree(sv::node* r, const sample s, std::ostream& os)
{
	if (r->children.size() == 0) return r->label;
	int i = std::find(r->branches.begin(), r->branches.end(),
		s.feature_values[r->split_rule_index]) - r->branches.begin();
	return apply_tree(r->children[i].get(), s, os);
}

void sv::id3_tree::print_tree() 
{

}

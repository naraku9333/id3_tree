#include <node.hpp>
#include <cmath>

sv::node::node(int p, int n, int num, int l) : 
	children(), pos(p), neg(n), num_samples(p + n), node_number(num), label(l)
{
	if (num_samples <= 0) throw std::logic_error("Number of samples must be greater than zero");
}

void sv::node::add_node(int p, int n) { children.emplace_back(new node(p, n)); }

float sv::node::entropy()
{
	float p = static_cast<float>(pos) / num_samples;
	float n = static_cast<float>(neg) / num_samples;
	float e = -((pos > 0) ? p * std::log2(p) : 0) - ((neg > 0) ? n * std::log2(n) : 0);

	return e;
}

float sv::node::gain()
{
	float g = entropy();
	for (const auto& n : children) { g -= (static_cast<float>(n->num_samples) / num_samples) * n->gain(); }

	return g;
}

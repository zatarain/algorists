#include <cstdlib>
#include <cassert>
#include "redblack.hpp"

//template<typename Type>

template<typename Type>
using class_under_test = algorists::ds::trees::redblack<Type>;

template<typename Type = int>
struct redblack: private class_under_test<Type> {
	typedef class_under_test<Type> tree;
	static void check_termination_nodes();
	static void check_output_in_order();
	static void check_breadth_first_search();
};

template<typename Type>
void redblack<Type>::check_termination_nodes() {
	assert(tree::node::end == tree::node::end->left);
	assert(tree::node::end == tree::node::end->right);
	assert(tree::node::end->colour == tree::colour_t::BLACK);
}

template<typename Type>
void redblack<Type>::check_output_in_order() {
	std::clog <<
		"Inserting a arbitrary order set of numbers to check whether they get properly sorted." <<
	std::endl;
	tree rbobject{54, 71, 34, 18, 42, 73, 98};
	std::clog << rbobject << std::endl;
	rbobject.bfs(std::clog);
}

template<typename Type>
void redblack<Type>::check_breadth_first_search() {
	std::clog <<
		"Inserting a arbitrary order set of numbers to check whether the coloring is correct." <<
	std::endl;
	tree rbobject{54, 71, 34, 18, 42, 73, 98};
	rbobject.bfs(std::clog);

	std::clog <<
		"Inserting sorted set of numbers to check whether the tree doesn't " <<
		"degenerate to a list."	<<
	std::endl;
	tree rblist{18, 34, 42, 54, 71, 73, 98};
	rblist.bfs(std::clog);

	std::clog <<
		"Inserting reversed order set of numbers to check whether the tree doesn't " <<
		"degenerate to a list."	<<
	std::endl;
	tree reverse{98, 73, 71, 54, 42, 34, 18};
	reverse.bfs(std::clog);
}

int main(int argc, char const *argv[]) {
	redblack<int>::check_termination_nodes();
	redblack<int>::check_output_in_order();
	redblack<int>::check_breadth_first_search();
	return EXIT_SUCCESS;
}

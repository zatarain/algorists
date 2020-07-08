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
};

template<typename Type>
void redblack<Type>::check_termination_nodes() {
	assert(tree::node::end == tree::node::end->left);
	assert(tree::node::end == tree::node::end->right);
	assert(tree::node::end->colour == tree::colour_t::BLACK);
}

template<typename Type>
void redblack<Type>::check_output_in_order() {
	/**
	tree rbobject;
	rbobject.insert(54);
	rbobject.insert(71);
	rbobject.insert(34);
	rbobject.insert(18);
	rbobject.insert(42);
	rbobject.insert(73);
	rbobject.insert(98);
	std::clog << rbobject << std::endl;
	rbobject.bfs(std::clog);
	/**/
	tree rblist;
	rblist.insert(18);
	rblist.insert(34);
	//rblist.insert(25);
	/**/
	rblist.insert(42);
	rblist.insert(54);
	rblist.insert(71);
	rblist.insert(73);
	rblist.insert(98);
	/**/
	tree reverse;
	reverse.insert(98);
	reverse.insert(73);
	reverse.insert(71);
	reverse.insert(54);
	reverse.insert(42);
	reverse.insert(34);
	reverse.insert(18);
	/**
	/**/
}

int main(int argc, char const *argv[]) {
	redblack<int>::check_termination_nodes();
	redblack<int>::check_output_in_order();
	return EXIT_SUCCESS;
}

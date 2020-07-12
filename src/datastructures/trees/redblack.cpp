#ifndef REDBLACK_IMPLEMENTATION
#define REDBLACK_IMPLEMENTATION

#include "redblack.hpp"

namespace algorists::ds::trees {

	template<typename Type>
	redblack<Type>::node::node():
		left(this), right(this),
		parent(this), colour(colour_t::BLACK) { }

	template<typename Type>
	typename redblack<Type>::node *const redblack<Type>::node::end = new redblack<Type>::node;

	template<typename Type>
	redblack<Type>::node::node(const Type& value, colour_t colour):
		left(end), right(end), parent(end),
		data(value), colour(colour) { }

	//template<typename Type> using node = typename redblack<Type>::node;

	template<typename Type>
	redblack<Type>::redblack(): root(node::end) { }

	template<typename Type>
	redblack<Type>::redblack(const std::initializer_list<Type>& list): root(node::end) {
		for(const Type value: list) insert(value);
	}

	template<typename Type>
	redblack<Type>::~redblack() {
		this->release(root);
	}

	template<typename Type>
	void redblack<Type>::release(node *current) {
		if(current == node::end) return;
		release(current->left);
		release(current->right);
		delete current;
		current = node::end;
	}

	template<typename Type>
	void redblack<Type>::fix(node *x) {
		node *parent = x->parent;
		if(!(parent->colour == x->colour && x->colour == colour_t::RED)) return;

		// parent and x are both red -> we need to fix the tree.
		node *grant = parent->parent;
		node *uncle = parent == grant->left ? grant->right : grant->left;
		if(uncle->colour == colour_t::RED) {
			//std::clog << "uncle is red: push blackness down from grant parent." << std::endl;
			grant->colour = colour_t::RED;
			uncle->colour = colour_t::BLACK;
			parent->colour = colour_t::BLACK;
			return;
		}

		// uncle is black -> check for zigzag
		if(grant->left == parent && x == parent->right) {
			//std::clog << "zigzag left to right: parent is left-child and x is right-child." << std::endl;
			rotate<LEFT>(parent);
			std::swap(x, parent);
		} else if(grant->right == parent && x == parent->left) {
			//std::clog << "zigzag right to left: parent is right-child and x is left-child." << std::endl;
			rotate<RIGHT>(parent);
			std::swap(x, parent);
		}

		if(x == parent->right && grant->right == parent) {
			//std::clog << "straight line right: parent is right-child and x is right-child." << std::endl;
			rotate<LEFT>(parent);
		} else if(x == parent->left && grant->left == parent) {
			//std::clog << "straight line left: parent is left-child and x is left-child." << std::endl;
			rotate<RIGHT>(parent);
		}

		if(grant != node::end) {
			grant->colour = colour_t::RED;
		}
		parent->colour = colour_t::BLACK;
		root->colour = colour_t::BLACK;
	}

	template<typename Type>
	template<direction_t direction>
	typename redblack<Type>::node *& redblack<Type>::child(node *x) {
		return direction == LEFT ? x->left : x->right;
	}

	template<typename Type>
	typename redblack<Type>::node *& redblack<Type>::sibling(node *x) {
		node *parent = x->parent;
		return x == parent->left ? parent->right : parent->left;
	}

	template<typename Type>
	template<direction_t direction>
	void redblack<Type>::rotate(node *x) {
		node *parent = x->parent;
		node *grant = parent->parent;
		node *son = child<direction>(x);
		child<direction>(x) = parent;
		x->parent = grant;
		if(grant != node::end){
			if(grant->left == parent) {
				grant->left = x;
			} else {
				grant->right = x;
			}
		} else {
			root = x;
		}
		parent->parent = x;
		child<static_cast<direction_t>(-direction)>(parent) = son;
		if(son != node::end) son->parent = parent;
		if(child<direction>(parent) == x) child<direction>(parent) = node::end;
	}

	template<typename Type>
	typename redblack<Type>::node* redblack<Type>::insert(const Type& value, node *&current, node *parent) {
		if(current == node::end) {
			current = new node(value);
			current->parent = parent;
			fix(current);
			return current;
		}

		//if(value == current->data) return node::end;

		if(value < current->data) {
			insert(value, current->left, current);
		} else {
			insert(value, current->right, current);
		}

		return node::end;
	}

	template<typename Type>
	bool redblack<Type>::remove(const Type& value, node *&current) {
		const auto& [found, where, direction] = lookup(value, current);
		if(!found) return false;
		return true;
	}

	template<typename Type>
	typename redblack<Type>::result_t redblack<Type>::lookup(const Type& value, node *&current) const {
		if(current == node::end) return {false, &node::end, CURRENT};

		if(value == current->data) return {true, &current, CURRENT};

		direction_t direction = static_cast<direction_t>(1 - 2 * (value < current->data));
		const auto& [found, where, way] = lookup(value, direction == LEFT ? current->left: current->right);

		if(where == node::end) return {found, &current, direction};

		return {found, where, way};
	}

	template<typename Type>
	bool redblack<Type>::insert(const Type& value) {
		return insert(value, root) != node::end;
	}

	template<typename Type>
	bool redblack<Type>::remove(const Type& value) {
		return remove(value, root);
	}

	template<typename Type>
	bool redblack<Type>::lookup(const Type& value) const {
		return std::get<0>(lookup(value, root));
	}

	template<typename Type>
	void redblack<Type>::traverse(std::ostream& output, node *current) const {
		if(current == node::end) return;
		traverse(output, current->left);
		*current >> output << ' ';
		traverse(output, current->right);
	}

	template<typename Type>
	void redblack<Type>::bfs(std::ostream& output) const {
		using queue = std::queue<node*>;
		queue* bag = new queue;
		queue* aux = new queue;
		bag->push(root);
		while(!bag->empty()) {
			node* current = bag->front();
			bag->pop();
			*current >> output;
			if(current != node::end) {
				aux->push(current->left);
				aux->push(current->right);
			}
			if(bag->empty()) {
				output << std::endl;
				std::swap(bag, aux);
			} else {
				output << ' ';
			}
		}
	}

	template<typename Type>
	std::ostream& redblack<Type>::node::operator>>(std::ostream& output) {
		static std::string format[] = {"\033[5;30;40m", "\033[5;31;40m"};
		output << format[static_cast<int>(colour)];
		if(this != node::end) {
			output << data;
		} else {
			output << "NL";
		}
		return output << "\033[0m";
	}

}

template<typename Type>
std::ostream& operator<<(std::ostream& output, const algorists::ds::trees::redblack<Type>& tree) {
	tree.traverse(output, tree.root);
	return output;
}

#endif
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
	redblack<Type>::~redblack() {
		std::clog << "Realeasing red-black tree..." << std::endl;
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
			std::clog << "uncle is red: push blackness down from grant parent." << std::endl;
			grant->colour = colour_t::RED;
			uncle->colour = colour_t::BLACK;
			parent->colour = colour_t::BLACK;
			return;
		}

		// uncle is black -> check for zigzag
		if(grant->left == parent && x == parent->right) {
			std::clog << "zigzag left to right: parent is left-child and x is right-child." << std::endl;
			rotate_left(x);
			bfs(std::clog);
			//fix(parent);
			std::swap(x, parent);
		} else if(grant->right == parent && x == parent->left) {
			std::clog << "zigzag right to left: parent is right-child and x is left-child." << std::endl;
			rotate_right(x);
			bfs(std::clog);
			//fix(parent);
			std::swap(x, parent);
		}

		if(x == parent->right && grant->right == parent) {
			std::clog << "straight line right: parent is right-child and x is right-child." << std::endl;
			rotate_left(parent);
			//rotate(parent, direction_t::LEFT);
		} else if(x == parent->left && grant->left == parent) {
			std::clog << "straight line left: parent is left-child and x is left-child." << std::endl;
			rotate_right(parent);
			//rotate(parent, direction_t::RIGHT);
		}

		if(grant != node::end) grant->colour = colour_t::RED;
		parent->colour = colour_t::BLACK;
	}
/**
	template<typename Type>
	void redblack<Type>::rotate(node *x, direction_t direction) {
		node *parent = x->parent;
		node *grant = parent->parent;
		node *child = direction == direction_t::LEFT ? x->left : x->right;
		//node *left = x->left;
		child(x, direction) = parent;
		x->parent = grant;
		if(grant != node::end){
			if(grant->left == parent) grant->left = x; else grant->right = x;
		} else {
			root = x;
		}
		parent->parent = x;
		brother(x) = child;
		if(child != node::end) child->parent = parent;
		//if(left != node::end) left->parent = parent;
		if(child(parent, direction) == x) child(parent, direction) = node::end;
	}
/**/
	template<typename Type>
	void redblack<Type>::rotate_left(node *x) {
		node *parent = x->parent;
		node *grant = parent->parent;
		node *left = x->left;
		x->left = parent;
		x->parent = grant;
		if(grant != node::end){
			if(grant->left == parent) grant->left = x; else grant->right = x;
		} else {
			root = x;
		}
		parent->parent = x;
		parent->right = left;
		if(left != node::end) left->parent = parent;
		if(parent->left == x) parent->left = node::end;
	}

	template<typename Type>
	void redblack<Type>::rotate_right(node *x) {
		node *parent = x->parent;
		node *grant = parent->parent;
		node *right = x->right;
		x->right = parent;
		x->parent = grant;
		if(grant != node::end) {
			if(grant->right == parent) grant->right = x; else grant->left = x;
		} else {
			root = x;
		}
		parent->parent = x;
		parent->left = right;
		if(right != node::end) right->parent = parent;
		if(parent->right == x) parent->right = node::end;
	}

	template<typename Type>
	typename redblack<Type>::node* redblack<Type>::insert(const Type& value, node *&current, node *parent) {
		if(current == node::end) {
			current = new node(value);
			current->parent = parent;
			std::clog << "before fix (" << value << "):" << std::endl;
			bfs(std::clog);
			fix(current);
			return current;
		}

		//if(value == current->data) return node::end;

		if(value < current->data) {
			insert(value, current->left, current);
			//fix(current->left);
		} else {
			insert(value, current->right, current);
			//fix(current->right);
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
		if(current == node::end) return {false, &node::end, direction_t::CURRENT};

		if(value == current->data) return {true, &current, direction_t::CURRENT};

		direction_t direction = static_cast<direction_t>(1 - 2 * (value < current->data));
		const auto& [found, where, way] = lookup(value, direction == direction_t::LEFT ? current->left: current->right);

		if(where == node::end) return {found, &current, direction};

		return {found, where, way};
	}

	template<typename Type>
	bool redblack<Type>::insert(const Type& value) {
		std::clog << "Inserting: " << value << std::endl;
		node* inserted = insert(value, root);
		root->colour = colour_t::BLACK;
		bfs(std::clog);
		return inserted != node::end;
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
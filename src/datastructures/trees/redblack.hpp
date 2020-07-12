#ifndef REDBLACK_HEADER
#define REDBLACK_HEADER

#include <iostream>
#include <tuple>
#include <queue>

namespace algorists::ds::trees {
	template<typename Type> class redblack;
}

template<typename Type>
std::ostream& operator<<(std::ostream&, const algorists::ds::trees::redblack<Type>&);

namespace algorists::ds::trees {
	enum direction_t {
		LEFT = -1,
		CURRENT = 0,
		RIGHT = 1
	};
	template<typename Type>
	class redblack {
	protected:
		enum class colour_t {
			BLACK = 0,
			RED = 1
		};

		struct node {
			static node * const end;
			node *left;
			node *right;
			node *parent;
			Type data;
			colour_t colour;
			node();
			node(const Type&, colour_t = colour_t::RED);
			std::ostream& operator>>(std::ostream&);
		};

		using result_t = std::tuple<bool, node **, direction_t>;

		node *root;

		void release(node *);
		node* insert(const Type&, node *&, node *parent = node::end);
		bool remove(const Type&, node *&);
		result_t lookup(const Type&, node *&) const;
		void traverse(std::ostream&, node *) const;
		void fix(node *);
		template<direction_t direction>
		void rotate(node *);
		void rotate_left(node *);
		void rotate_right(node *);
		template<direction_t direction>
		node *& child(node *);
		node *& sibling(node *);
	public:
		redblack();
		redblack(const std::initializer_list<Type>&);
		~redblack();
		bool insert(const Type&);
		bool remove(const Type&);
		bool lookup(const Type&) const;
		//Type min() const;
		//Type max() const;
		template<typename T>
		friend std::ostream& ::operator<<(std::ostream&, const redblack<T>&);
		void bfs(std::ostream&) const;
	};
}

#include "redblack.cpp"

#endif

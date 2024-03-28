#include "AVL-map.h"

#include <iostream>

using namespace CS280;


// static data members
template< typename KEY_TYPE, typename VALUE_TYPE >
typename CS280::AVLmap<KEY_TYPE,VALUE_TYPE>::AVLmap_iterator        
		CS280::AVLmap<KEY_TYPE,VALUE_TYPE>::end_it        = CS280::AVLmap<KEY_TYPE,VALUE_TYPE>::AVLmap_iterator(nullptr);

template< typename KEY_TYPE, typename VALUE_TYPE >
typename CS280::AVLmap<KEY_TYPE,VALUE_TYPE>::AVLmap_iterator_const  
		CS280::AVLmap<KEY_TYPE,VALUE_TYPE>::const_end_it  = CS280::AVLmap<KEY_TYPE,VALUE_TYPE>::AVLmap_iterator_const(nullptr);


//in implementation file 
//method's name and return value have to be fully qualified with
//all required namespace and class names (like CS280::AVLmap<KEY_TYPE,VALUE_TYPE>::AVLmap)
//but the argument list does not have to be:
//function argument list automatically falls into the method's namespace (Koenig lookup)
template< typename KEY_TYPE, typename VALUE_TYPE >
CS280::AVLmap<KEY_TYPE,VALUE_TYPE>::AVLmap(const AVLmap& rhs) : pRoot(nullptr), size_(rhs.size_)  // ....
{
	pRoot = rhs.pRoot->Clone(nullptr);
}

template< typename KEY_TYPE, typename VALUE_TYPE >
typename AVLmap<KEY_TYPE, VALUE_TYPE>::Node* AVLmap<KEY_TYPE, VALUE_TYPE>::Node::Clone(Node* Parent) const
{
	

	Node* ret = new Node(this->key, this->value, Parent, this->height, this->balance, nullptr, nullptr);
	Node* nl = nullptr, *nr = nullptr;
	if (this->left)
	{
		nl = this->left->Clone(ret);
	}
	if (this->right)
	{
		nr = this->right->Clone(ret);
	}
	ret->left = nl;
	ret->right = nr;
	return ret;
}

////////////////////////////////////////////////////////////
// typename is used to help compiler to parse
template< typename KEY_TYPE, typename VALUE_TYPE >
typename CS280::AVLmap<KEY_TYPE,VALUE_TYPE>::AVLmap_iterator CS280::AVLmap<KEY_TYPE,VALUE_TYPE>::begin() {
	if (pRoot) return AVLmap<KEY_TYPE,VALUE_TYPE>::AVLmap_iterator(pRoot->first());
	else       return end_it;
}

template <typename KEY_TYPE, typename VALUE_TYPE>
AVLmap<KEY_TYPE,VALUE_TYPE>::AVLmap() : pRoot(nullptr), size_(0)
{
	
}

template <typename KEY_TYPE, typename VALUE_TYPE>
typename AVLmap<KEY_TYPE,VALUE_TYPE>::AVLmap_iterator_const AVLmap<KEY_TYPE,VALUE_TYPE>::end() const
{
	return const_end_it;
}

template <typename KEY_TYPE, typename VALUE_TYPE>
typename AVLmap<KEY_TYPE,VALUE_TYPE>::AVLmap_iterator AVLmap<KEY_TYPE,VALUE_TYPE>::end()
{
	return end_it;
}

template <typename KEY_TYPE, typename VALUE_TYPE>
typename AVLmap<KEY_TYPE,VALUE_TYPE>::AVLmap_iterator_const AVLmap<KEY_TYPE,VALUE_TYPE>::begin() const
{
	if (pRoot)
	{
		return AVLmap<KEY_TYPE,VALUE_TYPE>::AVLmap_iterator_const(pRoot->first());
	}
	return AVLmap<KEY_TYPE,VALUE_TYPE>::AVLmap_iterator_const(nullptr);
}

template <typename KEY_TYPE, typename VALUE_TYPE>
typename AVLmap<KEY_TYPE,VALUE_TYPE>::AVLmap_iterator AVLmap<KEY_TYPE,VALUE_TYPE>::find(KEY_TYPE const& key)
{
	if (pRoot)
	{
		if (pRoot->key == key)
		{
			return iterator(pRoot);
		}
		auto current = pRoot;
		auto n_key = current->Key();
		while (current && n_key != key)
		{
			if (key < n_key)
			{
				current = current->left;
			}
			else if (key > n_key)
			{
				current = current->right;
			}
			if(current)
				n_key = current->Key();
		}
		return AVLmap<KEY_TYPE,VALUE_TYPE>::AVLmap_iterator(current);
	}
	else
	{
		return AVLmap<KEY_TYPE,VALUE_TYPE>::AVLmap_iterator(nullptr);
	}
}

template <typename KEY_TYPE, typename VALUE_TYPE>
typename AVLmap<KEY_TYPE,VALUE_TYPE>::AVLmap_iterator_const AVLmap<KEY_TYPE,VALUE_TYPE>::find(KEY_TYPE const& key) const
{
	if (pRoot)
	{
		if (pRoot->key == key)
		{
			return const_iterator(pRoot);
		}
		auto current = pRoot;
		auto n_key = current->Key();
		while (current && n_key != key)
		{
			if (key < n_key)
			{
				current = current->left;
			}
			else if (key > n_key)
			{
				current = current->right;
			}

			n_key = current->Key();
		}
		return AVLmap<KEY_TYPE,VALUE_TYPE>::AVLmap_iterator_const(current);
	}
	else
	{
		return AVLmap<KEY_TYPE,VALUE_TYPE>::AVLmap_iterator_const(nullptr);
	}
}


//node impl
template <typename KEY_TYPE, typename VALUE_TYPE>
AVLmap<KEY_TYPE,VALUE_TYPE>::Node::Node(KEY_TYPE k, VALUE_TYPE val, Node* p, int h, int b, Node* l, Node* r) :
	key(k), value(val), height(h), balance(b), parent(p),  left(l), right(r)
{
	
}

template <typename KEY_TYPE, typename VALUE_TYPE>
typename AVLmap<KEY_TYPE,VALUE_TYPE>::Node* AVLmap<KEY_TYPE,VALUE_TYPE>::Node::increment()
{
	if (this->right)
	{
		return right->first();
	}
	else
	{
		auto current = this;
		auto cpar = parent;
		while (cpar && current == cpar->right)
		{
			current = cpar;
			cpar = cpar->parent;
		}
		return cpar;
	}
}

template <typename KEY_TYPE, typename VALUE_TYPE>
typename AVLmap<KEY_TYPE,VALUE_TYPE>::Node* AVLmap<KEY_TYPE,VALUE_TYPE>::Node::decrement()
{
	if (this->left)
	{
		return this->left->last();
	}
	else
	{
		auto current = this;
		auto cpar = parent;
		while (cpar&& current == cpar->left)
		{
			current = cpar;
			cpar = cpar->parent;
		}
		return cpar;
	}
	
}
template <typename KEY_TYPE, typename VALUE_TYPE>
typename AVLmap<KEY_TYPE,VALUE_TYPE>::Node* AVLmap<KEY_TYPE,VALUE_TYPE>::Node::first()
{
	auto current = this;
	while (current->left != nullptr)
	{
		current = current->left;
	}
	return current;
}

template <typename KEY_TYPE, typename VALUE_TYPE>
typename AVLmap<KEY_TYPE,VALUE_TYPE>::Node* AVLmap<KEY_TYPE,VALUE_TYPE>::Node::last()
{
	auto current = this;
	while (current->right != nullptr)
	{
		current = current->right;
	}
	return current;
}

template <typename KEY_TYPE, typename VALUE_TYPE>
const KEY_TYPE& AVLmap<KEY_TYPE,VALUE_TYPE>::Node::Key() const
{
	return key;
}

template <typename KEY_TYPE, typename VALUE_TYPE>
VALUE_TYPE& AVLmap<KEY_TYPE,VALUE_TYPE>::Node::Value() {
	return value;
}


//AVL impl
template <typename KEY_TYPE, typename VALUE_TYPE>
VALUE_TYPE& AVLmap<KEY_TYPE,VALUE_TYPE>::operator[](const KEY_TYPE& key)
{
	if (!pRoot)
	{
		pRoot = new Node(key, VALUE_TYPE(), nullptr, 0, 0, nullptr, nullptr);
		size_++;
		return pRoot->value;
	}
	else
	{
		auto current = pRoot;
		auto last = current;
		while (current)
		{
			last = current;
			if (current->key > key)
			{
				current = current->left;
			}
			else if (current->key < key)
			{
				current = current->right;
			}
			else
			{
				break;
			}
		}
		if (current)
		{
			return current->value;
		}
		else
		{
			auto ret = new Node(key, VALUE_TYPE(), last, last->height+1, 0, nullptr, nullptr);
			if (key < last->key)
			{
				ret->left = last->left;
				last->left = ret;
			}
			else
			{
				ret->right = last->right;
				last->right = ret;
			}
			size_++;
			return ret->value;
		}
	}
}



//iterator impl
template <typename KEY_TYPE, typename VALUE_TYPE>
AVLmap<KEY_TYPE,VALUE_TYPE>::iterator::AVLmap_iterator(Node* p) : p_node(p)
{
	
}
template <typename KEY_TYPE, typename VALUE_TYPE>
AVLmap<KEY_TYPE,VALUE_TYPE>::const_iterator::AVLmap_iterator_const(Node* p) : p_node(p)
{
	
}

template <typename KEY_TYPE, typename VALUE_TYPE>
typename AVLmap<KEY_TYPE,VALUE_TYPE>::Node* AVLmap<KEY_TYPE,VALUE_TYPE>::iterator::operator->()
{
	return p_node;
}
template <typename KEY_TYPE, typename VALUE_TYPE>
const typename AVLmap<KEY_TYPE,VALUE_TYPE>::Node* AVLmap<KEY_TYPE,VALUE_TYPE>::const_iterator::operator->()
{
	return p_node;
}

template <typename KEY_TYPE, typename VALUE_TYPE>
typename AVLmap<KEY_TYPE,VALUE_TYPE>::Node& AVLmap<KEY_TYPE,VALUE_TYPE>::iterator::operator*()
{
	return *p_node;
}
template <typename KEY_TYPE, typename VALUE_TYPE>
const typename AVLmap<KEY_TYPE,VALUE_TYPE>::Node& AVLmap<KEY_TYPE,VALUE_TYPE>::const_iterator::operator*()
{
	return *p_node;
}

template <typename KEY_TYPE, typename VALUE_TYPE>
bool AVLmap<KEY_TYPE,VALUE_TYPE>::iterator::operator==(const AVLmap_iterator& rhs) 
{
	return this->p_node == rhs.p_node;
}
template <typename KEY_TYPE, typename VALUE_TYPE>
bool AVLmap<KEY_TYPE,VALUE_TYPE>::iterator::operator!=(const AVLmap_iterator& rhs) 
{
	return !(this->p_node == rhs.p_node);
}
template <typename KEY_TYPE, typename VALUE_TYPE>
bool AVLmap<KEY_TYPE,VALUE_TYPE>::const_iterator::operator==(const AVLmap_iterator_const& rhs) 
{
	return this->p_node == rhs.p_node;
}
template <typename KEY_TYPE, typename VALUE_TYPE>
bool AVLmap<KEY_TYPE,VALUE_TYPE>::const_iterator::operator!=(const AVLmap_iterator_const& rhs) 
{
	return !(this->p_node == rhs.p_node);
}

template <typename KEY_TYPE, typename VALUE_TYPE>
typename AVLmap<KEY_TYPE,VALUE_TYPE>::AVLmap_iterator& AVLmap<KEY_TYPE,VALUE_TYPE>::iterator::operator++()
{
	p_node = p_node->increment();
	return *this;
}
#define UNUSED(a) (void)a;
template <typename KEY_TYPE, typename VALUE_TYPE>
typename AVLmap<KEY_TYPE,VALUE_TYPE>::AVLmap_iterator AVLmap<KEY_TYPE,VALUE_TYPE>::iterator::operator++(int _) 
{
	UNUSED(_);
	iterator old(p_node);
	operator++();
	return old;
}

template <typename KEY_TYPE, typename VALUE_TYPE>
typename AVLmap<KEY_TYPE,VALUE_TYPE>::AVLmap_iterator_const& AVLmap<KEY_TYPE,VALUE_TYPE>::const_iterator::operator++()
{
	return AVLmap<KEY_TYPE,VALUE_TYPE>::iterator(p_node->increment());
}
template <typename KEY_TYPE, typename VALUE_TYPE>
typename AVLmap<KEY_TYPE,VALUE_TYPE>::AVLmap_iterator_const AVLmap<KEY_TYPE,VALUE_TYPE>::const_iterator::operator++(int _) 
{
	auto tmp = *this;
	operator++();
	return tmp;
}

template <typename KEY_TYPE, typename VALUE_TYPE>
AVLmap<KEY_TYPE, VALUE_TYPE>::AVLmap_iterator::AVLmap_iterator(const AVLmap_iterator& other) : p_node(nullptr)
{
	this->p_node = other.p_node;
}

template <typename KEY_TYPE, typename VALUE_TYPE>
typename AVLmap<KEY_TYPE,VALUE_TYPE>::AVLmap_iterator& AVLmap<KEY_TYPE,VALUE_TYPE>::iterator::operator=(const AVLmap_iterator& rhs)
{
	p_node = rhs.p_node;
	return *this;
}
template <typename KEY_TYPE, typename VALUE_TYPE>
typename AVLmap<KEY_TYPE,VALUE_TYPE>::AVLmap_iterator_const& AVLmap<KEY_TYPE,VALUE_TYPE>::const_iterator::operator=(const AVLmap_iterator_const& rhs)
{
	p_node = rhs.p_node;
}

template <typename KEY_TYPE, typename VALUE_TYPE>
void AVLmap<KEY_TYPE, VALUE_TYPE>::erase(AVLmap_iterator it)
{
	if (it == end_it)
	{
		return;
	}
	Node* current = it.p_node;
	Node** pos = nullptr;
	Node* parent = nullptr;
	if (current->parent)
	{
		parent = current->parent;
		if (current->parent->left == current)
		{
			pos = &current->parent->left;
		}
		else if (current->parent->right == current)
		{
			pos = &current->parent->right;
		}
	}
	else
	{
		pos = &pRoot;
	}

	if (it->left == nullptr && it->right == nullptr)
	{
		//leaf just remove it;
		if (pos)
		{
			*pos = nullptr;
			
		}
		delete it.p_node;
		size_--;

	}
	else if (it->left == nullptr && it->right != nullptr)
	{
		if (pos)
		{
			*pos = current->right;
			(*pos)->height = current->height;
			(*pos)->parent = parent;
		}
		delete it.p_node;
		size_--;
	}
	else if (it->left != nullptr && it->right == nullptr)
	{
		if (pos)
		{
			*pos = current->left;
			(*pos)->height = current->height;
			(*pos)->parent = parent;
		}
		delete it.p_node;
		size_--;
	}
	else
	{
		Node* pred = current->decrement();
		//current->left = pred->left;
		current->value = pred->value;
		current->key = pred->key;
		erase(iterator(pred));
		// swap "it" with the predecessor
	}
	

}




////////////////////////////////////////////////////////////
// do not change this code from here to the end of the file
/* figure out whether node is left or right child or root 
 * used in print_backwards_padded 
 */
template< typename KEY_TYPE, typename VALUE_TYPE >
char CS280::AVLmap<KEY_TYPE,VALUE_TYPE>::getedgesymbol(const Node* node) const {
	const Node* parent = node->parent;
	if ( parent == nullptr) return '-';
	else                 return ( parent->left == node)?'\\':'/';
}

/* this is another "ASCII-graphical" print, but using 
 * iterative function. 
 * Left branch of the tree is at the bottom
 */
template< typename KEY_TYPE, typename VALUE_TYPE >
std::ostream& CS280::operator<<(std::ostream& os, AVLmap<KEY_TYPE,VALUE_TYPE> const& map) {
	map.print(os);
	return os;
}



template< typename KEY_TYPE, typename VALUE_TYPE >
void CS280::AVLmap<KEY_TYPE,VALUE_TYPE>::print(std::ostream& os, bool print_value ) const {
	if (pRoot) {
		AVLmap<KEY_TYPE,VALUE_TYPE>::Node* b = pRoot->last();
		while ( b ) { 
			int depth = getdepth(b);
			int i;
			/* printf(b); */

			char edge = getedgesymbol(b);
			switch (edge) {
				case '-': 
					for (i=0; i<depth; ++i) std::printf("       ");
					os << b->key;
                    if ( print_value ) { os << " -> " << b->value; }
                    os << std::endl;
					break;
				case '\\': 
					for (i=0; i<depth; ++i) std::printf("       ");
					os << edge << std::endl;
					for (i=0; i<depth; ++i) std::printf("       ");
					os << b->key;
                    if ( print_value ) { os << " -> " << b->value; }
                    os << std::endl;
					break;
				case '/': 
					for (i=0; i<depth; ++i) std::printf("       ");
					os << b->key;
                    if ( print_value ) { os << " -> " << b->value; }
                    os << std::endl;
					for (i=0; i<depth; ++i) std::printf("       ");
					os << edge << std::endl;
					break;
			}
			b = b->decrement();
		}
	}
	std::printf("\n");
}


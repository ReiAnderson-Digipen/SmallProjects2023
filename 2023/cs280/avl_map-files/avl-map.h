#ifndef AVLMAP_H
#define AVLMAP_H
#include <algorithm>
namespace CS280 {

	template< typename KEY_TYPE, typename VALUE_TYPE >
	class AVLmap {
	public:
		class Node {
		public:
			Node(KEY_TYPE k, VALUE_TYPE val, Node* p, int h, int b, Node* l, Node* r)
				: key(k), value(val), height(h), balance(b), bval(0), parent(p), left(l), right(r)
			{
			}

			Node(const Node&) = delete;
			Node* operator=(const Node&) = delete;

			KEY_TYPE const& Key() const { return key; }   // return a const reference
			VALUE_TYPE& Value() { return value; }     // return a reference

			Node* first()
			{
				auto current = this;
				while (current->left != nullptr)
				{
					current = current->left;
				}
				return current;
			}; // minimum - follow left as far as possible
			Node* last()
			{
				auto current = this;
				while (current->right != nullptr)
				{
					current = current->right;
				}
				return current;
			}; // maximum - follow right as far as possible
			Node* increment()
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
			} // successor
			Node* decrement()
			{
				if (this->left)
				{
					return this->left->last();
				}
				else
				{
					auto current = this;
					auto cpar = parent;
					while (cpar && current == cpar->left)
					{
						current = cpar;
						cpar = cpar->parent;
					}
					return cpar;
				}

			} // predecessor
			Node* Clone(Node* Parent) const
			{


				Node* ret = new Node(this->key, this->value, Parent, this->height, this->balance, nullptr, nullptr);
				Node* nl = nullptr, * nr = nullptr;
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
			void   print(std::ostream& os) const;

		private:
			KEY_TYPE    key;
			VALUE_TYPE  value;
			int         height, balance, bval; // optional
			Node* parent;
			Node* left;
			Node* right;

			Node* GetRoot()
			{
				auto current = this;
				while (current->parent)
				{
					current = current->parent;
				}
				return current;
			}
			static void UpdateParents(Node* parent)
			{
				if (Node* l = parent->left)
				{
					if (l->parent != parent)
					{
						parent->left = l->parent;
						UpdateParents(parent);
					}
				}
				if (Node* r = parent->right)
				{
					if (r->parent != parent)
					{
						parent->right = r->parent;
						UpdateParents(parent);
					}
				}
			}

			static Node* RotateRight(Node* n)
			{
				if (n == nullptr)
				{
					return nullptr;
				}
				Node* current = n;
				Node* root = current->left;
				if (root)
				{
					root->parent = current->parent;
					current->parent = root;
					current->left = root->right;
					if (current->left) current->left->parent = current;
					root->right = current;
					//Recalc_Heights(root);
					//root->last()->Recalc_Balance();
				}
				return root;

			}
			static Node* RotateLeft(Node* n)
			{
				if (n == nullptr)
				{
					return nullptr;
				}

				Node* current = n;
				Node* root = n->right;

				if (root)
				{

					root->parent = current->parent;
					current->parent = root;
					current->right = root->left;
					if (current->right) current->right->parent = current;
					root->left = current;
					//Recalc_Heights(root);
					//root->first()->Recalc_Balance();

				}
				return root;



				// auto temp = n;
				// n = n->right;
				// if (temp->parent)
				// {
				// 	auto par = temp->parent;
				// 	if (par->left == temp)
				// 	{
				// 		par->left = n;
				// 	}
				// 	else if (par->right == temp)
				// 	{
				// 		par->right = n;
				// 	}
				// }
				// n->parent = temp->parent;
				// temp->parent = n;
				// temp->right = n->left;
				// if(temp->right)
				// 	temp->right->parent = temp;
				// n->left = temp;
				// Recalc_Heights(n);
				// n->last()->Recalc_Balance();
				// return n;
			}

			static void Recalc_Balance(Node* n)
			{
				if (n == nullptr)
				{
					return;
				}
				n->balance = 0;
				if (n->left)
				{

					n->balance -= n->left->height;
				}
				else
				{
					n->balance += 1;
				}
				if (n->right)
				{
					n->balance += n->right->height;
				}
				else
				{
					n->balance -= 1;
				}

				if (n->balance > 1)
				{

					Node* center = n;
					Node* r = center->right;
					if (r->right)
					{
						RotateLeft(center);
					}
					else if (r->left)
					{
						RotateRight(r);
						UpdateParents(center);
						RotateLeft(center);
					}

				}
				else if (n->balance < -1)
				{
					Node* center = n;
					Node* l = center->left;
					if (l->left)
					{
						RotateRight(center);
					}
					else if (l->right)
					{
						RotateLeft(l);
						UpdateParents(center);
						RotateRight(center);
					}

				}

				// if (parent)
				// {
				// 	parent->Recalc_Balance();
				// }


				// if (balance > 1)
				// {
				// 	ret = RotateLeft(this);
				// }
				// else if (balance < -1)
				// {
				// 	ret = RotateRight(this);
				// }
				// if (parent)
				// {
				// 	parent->Recalc_Balance();
				// }
			}
			void DeleteChildren()
			{

				if (this->left)
				{
					this->left->DeleteChildren();
					delete this->left;
					this->left = nullptr;
				}
				if (this->right)
				{
					this->right->DeleteChildren();
					delete this->right;
					this->right = nullptr;
				}
			}
			friend class AVLmap;
		};
	private:
		struct AVLmap_iterator {
		private:
			Node* p_node;
		public:
			AVLmap_iterator(Node* p = nullptr) : p_node(p) {}
			AVLmap_iterator(const AVLmap_iterator& other) : p_node(other.p_node) {};
			AVLmap_iterator& operator=(const AVLmap_iterator& rhs)
			{
				p_node = rhs.p_node;
				return *this;
			}
			AVLmap_iterator& operator++()
			{
				iterator(p_node->increment());
			}
			AVLmap_iterator operator++(int)
			{
				auto old = *this;
				operator++();
				return old;
			}
			Node& operator*()
			{
				return *p_node;
			}
			Node* operator->()
			{
				return p_node;
			}
			bool operator!=(const AVLmap_iterator& rhs)
			{
				return p_node != rhs.p_node;
			}
			bool operator==(const AVLmap_iterator& rhs)
			{
				return p_node == rhs.p_node;
			}
			friend class AVLmap;
		};
		struct AVLmap_iterator_const {
		private:
			Node* p_node;
		public:
			AVLmap_iterator_const(Node* p = nullptr) : p_node(p) {};
			AVLmap_iterator_const& operator=(const AVLmap_iterator_const& rhs)
			{
				p_node = rhs.p_node;
				return *this;
			};
			AVLmap_iterator_const& operator++()
			{
				return const_iterator(p_node->increment());
			}
			AVLmap_iterator_const operator++(int)
			{
				auto old = *this;
				operator++();
				return old;
			}
			Node const& operator*()
			{
				return *p_node;
			}
			Node const* operator->()
			{
				return p_node;
			}
			bool operator!=(const AVLmap_iterator_const& rhs)
			{
				return p_node != rhs.p_node;
			}
			bool operator==(const AVLmap_iterator_const& rhs)
			{
				return p_node == rhs.p_node;
			}
			friend class AVLmap;
		};
		// AVLmap implementation
		Node* pRoot = nullptr;
		unsigned int size_ = 0;
		// end iterators are same for all AVLmaps, thus static
		// make AVLmap_iterator a friend
		// to allow AVLmap_iterator to access end iterators 
		static AVLmap_iterator end_it;
		static AVLmap_iterator_const const_end_it;
	public:
		//BIG FOUR
		AVLmap() : pRoot(nullptr), size_(0) {};
		AVLmap(const AVLmap& rhs) : pRoot(nullptr), size_(rhs.size_)
		{
			pRoot = rhs.pRoot->Clone(nullptr);
		};
		AVLmap& operator=(const AVLmap& rhs)
		{
			if (pRoot)
			{
				pRoot->DeleteChildren();
				delete pRoot;
			}
			size_ = rhs.size_;
			pRoot = rhs.pRoot->Clone(nullptr);
			return *this;

		};
		virtual ~AVLmap()
		{
			if (pRoot)
			{
				pRoot->DeleteChildren();
				delete pRoot;
			}
		};

		unsigned int size()
		{
			return size_;
		}
		int getdepth(const Node* n) const
		{
			int depth = 0;
			auto current = n->parent;
			while (current)
			{
				depth++;
				current = current->parent;
			}
			return depth;
		}



		Node* insertNode(const KEY_TYPE& key, Node* root, Node* parent)
		{
			if (root == nullptr)
			{
				Node* ret = new Node(key, VALUE_TYPE(), parent, 0, 0, nullptr, nullptr);
				return ret;
			}

			if (key < root->key)
			{
				root->left = insertNode(key, root->left, root);
				Recalc_Heights(root);
				Node::UpdateParents(root);
				Node::Recalc_Balance(root);
			}
			else if (key > root->key)
			{
				root->right = insertNode(key, root->right, root);
				Recalc_Heights(root);
				Node::UpdateParents(root);
				Node::Recalc_Balance(root);
			}

			return root;

		}


		//value setter and getter
		VALUE_TYPE& operator[](KEY_TYPE const& key)
		{
			iterator it = find(key);
			if (it != end_it)
			{
				return it->value;
			}
			else
			{
				pRoot = insertNode(key, pRoot, nullptr)->GetRoot();
				size_++;
				return operator[](key);
			}
		}
		//next method doesn't make sense
		//because operator[] inserts a non-existing element
		//which is not allowed on const maps
		//VALUE_TYPE operator[](int key) const;


		//standard names for iterator types
		typedef AVLmap_iterator       iterator;
		typedef AVLmap_iterator_const const_iterator;

		//AVLmap methods dealing with non-const iterator 
		AVLmap_iterator begin()
		{
			if (pRoot) return AVLmap<KEY_TYPE, VALUE_TYPE>::AVLmap_iterator(pRoot->first());
			else       return end_it;
		}
		AVLmap_iterator end()
		{
			return end_it;
		}
		AVLmap_iterator find(KEY_TYPE const& key)
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
					if (current)
						n_key = current->Key();
				}
				return AVLmap<KEY_TYPE, VALUE_TYPE>::AVLmap_iterator(current);
			}
			else
			{
				return AVLmap<KEY_TYPE, VALUE_TYPE>::AVLmap_iterator(nullptr);
			}
		}
		void erase(AVLmap_iterator it)
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
				if (parent)
				{
					Recalc_Heights(parent);
					Node::Recalc_Balance(parent);
					Node::UpdateParents(parent);
					pRoot = parent->GetRoot();
				}

			}
			else if (it->left == nullptr && it->right != nullptr)
			{
				if (pos)
				{
					*pos = current->right;
					//(*pos)->height = current->height;
					(*pos)->parent = parent;
				}
				delete it.p_node;
				size_--;
				if (parent)
				{
					Node::Recalc_Balance(parent);
					Node::UpdateParents(parent);
					pRoot = parent->GetRoot();
				}
			}
			else if (it->left != nullptr && it->right == nullptr)
			{
				if (pos)
				{
					*pos = current->left;
					//(*pos)->height = current->height;
					(*pos)->parent = parent;
				}
				delete it.p_node;
				size_--;
				if (parent)
				{
					Node::Recalc_Balance(parent);
					Node::UpdateParents(parent);
					pRoot = parent->GetRoot();
				}
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

			Recalc_Heights(pRoot);


		}

		//AVLmap methods dealing with const iterator 
		AVLmap_iterator_const begin() const
		{
			if (pRoot) return const_iterator(pRoot->first());
			else return const_end_it;
		}
		AVLmap_iterator_const end() const
		{
			return const_end_it;
		}
		AVLmap_iterator_const find(KEY_TYPE const& key) const
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
				return AVLmap<KEY_TYPE, VALUE_TYPE>::AVLmap_iterator_const(current);
			}
			else
			{
				return AVLmap<KEY_TYPE, VALUE_TYPE>::AVLmap_iterator_const(nullptr);
			}
		}

		//do not need this one (why)
		//AVLmap_iterator_const erase(AVLmap_iterator& it) const;

		void print(std::ostream& os, bool print_value = false) const;
		bool sanityCheck();
		char getedgesymbol(const Node* node) const;


		//inner class (AVLmap_iterator) doesn't have any special priveleges
		//in accessing private data/methods of the outer class (AVLmap)
		//so need friendship to allow AVLmap_iterator to access private "AVLmap::end_it"
		//BTW - same is true for outer class accessing inner class private data
		friend class AVLmap_iterator;
		friend class AVLmap_iterator_const;
	private:

		static int Recalc_Heights(Node* node)
		{
			if (!node)
			{
				return -1;
			}
			if (!node->left && !node->right)
			{
				node->height = 0;
				return 0;
			}
			int lh = 0, rh = 0;
			if (node->left)
			{
				lh = node->left->height;
			}
			if (node->right)
			{
				rh = node->right->height;
			}
			return (node->height = 1 + std::max(lh, rh));
		}
		// ...
	};

	//notice that it doesn't need to be friend
	template< typename KEY_TYPE, typename VALUE_TYPE >
	std::ostream& operator<<(std::ostream& os, AVLmap<KEY_TYPE, VALUE_TYPE> const& map);
}

#include "avl-map.cpp"
#endif

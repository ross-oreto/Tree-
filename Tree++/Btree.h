#pragma once

#include <iostream>
#include <sstream>
#include <vector>
#include <list>
using namespace std;

namespace tree {
	template <typename X> int cmp(X s1, X s2)
	{
		if (s1 < s2) return -1;
		else if (s1 > s2) return 1;
		else return 0;
	}

	template <typename K, typename V>
	class Btree {
	public:
		class Node {
			friend std::ostream& operator<<(std::ostream& os, Node& obj) {
				string sep = ", ";
				os << "[ " << obj.key << sep << "type: ";
				switch (obj.type)
				{
				case Type::LEFT: os << "left child"; break;
				case Type::RIGHT: os << "right child"; break;
				case Type::ROOT: os << "root"; break;
				default:
					break;
				}
				if (!obj.isRoot()) {
					os << sep << "parent: " << obj.parent->key;
				}
				os << sep << "val: " << obj.val;
				os << sep << "balance: " << obj.balance << " ]";
				return os;
			}
		public:
			enum Type
			{
				ROOT, LEFT, RIGHT
			};

			K getKey();
			V getVal();
			Type getType();
			Node* next();
			Node* previous();

		protected:
			K key;
			V val;
			Node *left;
			Node *right;
			Node *parent;
			int balance;
			Type type;

			Node(K key, V val);
			~Node();
			int insert(Node*);
			Node* get(K);
			Node* getLeft();
			Node* getRight();
			Node* getParent();

			void init();
			bool hasNoChildren();
			bool hasTwoChildren();
			bool hasChild();
			bool hasRightChild();
			bool hasLeftChild();
			bool isRightChild();
			bool isLeftChild();
			bool isRoot();
			bool hasParent();
			void replace(Node*);
			bool leansRight();
			bool leansLeft();

			Node* beginning();
			Node* end();
			Node* remove();
		private:
			Node* attachLeftNode(Node*, bool addWeight = true);
			Node* attachRightNode(Node*, bool addWeight = true);
			Node* attachNode(Node*, Type);
			Node* detachLeftNode();
			Node* detachRightNode();
			Node* detachNode(Type);
			Node* rotateLeft();
			Node* rotateRight();
			void setBalance();
			Node* notifyParents(Node*);
			friend class Btree;
		};

		Btree();
		~Btree();
		bool isEmpty();
		int size();
		int depth();
		Btree insert(K, V);
		Btree insertAll(vector<std::pair<K, V>>);
		Node* getRoot();
		Node* beginning();
		Node* end();
		Node* get(K);
		Node* remove(Node*);
		Node* remove(K);
		Node* pop();
		Node* pull();
		V getVal(K);
		bool contains(K);
		bool containsAll(vector<K>);
		void print();
		void clear();

	protected:
		Node *root;
		void init();
		void count(Node* node, int *count);
		void count_depth(Node* node, int *depth, int *high);
		void print(Node* node);
		void detatchAll(Node* node);
		bool pair_cmp(std::pair<K, V>, std::pair<K, V>);
	};

	template <typename K, typename V>
	Btree<K, V>::Btree() {
		init();
	}

	template <typename K, typename V>
	Btree<K, V>::~Btree() {
	}

	template <typename K, typename V>
	void Btree<K, V>::init() {
		root = nullptr;
	}

	template <typename K, typename V>
	int Btree<K, V>::size() {
		int nodes = 0;
		if (!isEmpty()) {
			count(root, &nodes);
		}
		return nodes;
	}

	template <typename K, typename V>
	int Btree<K, V>::depth() {
		int depth = 0;
		int high = 0;
		if (!isEmpty()) {
			depth = high = 1;
			if(root->balance <= 0)
				count_depth(root->left, &depth, &high);
			else 
				count_depth(root->right, &depth, &high);
		}
		return high;
	}

	template <typename K, typename V>
	bool Btree<K, V>::isEmpty() {
		return root == nullptr;
	}

	template <typename K, typename V>
	void Btree<K, V>::count(Node* node, int *nodes) {
		if (node != nullptr) {
			count(node->left, nodes);
			*nodes += 1;
			count(node->right, nodes);
		}
	}

	template <typename K, typename V>
	void Btree<K, V>::count_depth(Node* node, int *depth, int *high) {
		if (node != nullptr) {
			*depth += 1;
			count_depth(node->left, depth, high);
			count_depth(node->right, depth, high);
			if (*depth > *high) *high = *depth;
			*depth -= 1;
		}
	}

	template <typename K, typename V>
	void Btree<K, V>::print() {
		print(root);
	}

	template <typename K, typename V>
	void Btree<K, V>::clear() {
		if (!isEmpty()) {
			detatchAll(root);
			init();
		}
	}

	template <typename K, typename V>
	typename Btree<K, V>::Node* Btree<K, V>::remove(Node* node) {
		if (node != nullptr) {
			Node *newNode = node->remove();
			if ((newNode != nullptr && newNode->isRoot()) || node->isRoot()) {
				root = newNode;
			}
		}
		return node;
	}

	template <typename K, typename V>
	typename Btree<K, V>::Node* Btree<K, V>::remove(K key) {
		return remove(get(key));
	}

	template <typename K, typename V>
	typename Btree<K, V>::Node* Btree<K, V>::pop() {
		return remove(end());
	}

	template <typename K, typename V>
	typename Btree<K, V>::Node* Btree<K, V>::pull() {
		return remove(beginning());
	}

	template <typename K, typename V>
	void Btree<K, V>::detatchAll(Node* node) {
		if (node != nullptr) {
			detatchAll(node->left);
			detatchAll(node->right);
			init();
		}
	}

	template <typename K, typename V>
	void Btree<K, V>::print(Node* node) {
		if (node != nullptr) {
			print(node->left);
			cout << *node << endl;
			print(node->right);
		}
	}

	/************************************************
	Description: inserts key and val into the tree
	key: The value to key the node on.
	val: Value associated with key.
	************************************************/
	template <typename K, typename V>
	Btree<K, V> Btree<K, V>::insert(K key, V value) {
		Btree<K, V>::Node *newNode = new Node(key, value);
		if (isEmpty()) {
			this->root = newNode;
		} 
		else {
			root->insert(newNode);
			while (root->type != Btree<K, V>::Node::Type::ROOT) {
				root = root->parent;
			}
		}
		return *this;
	}

	template <typename K, typename V>
	bool Btree<K, V>::pair_cmp(std::pair<K, V> i, std::pair<K, V> j) { 
		return (i.first < j.first); 
	}

	template <typename K, typename V>
	Btree<K, V> Btree<K, V>::insertAll(vector<std::pair<K, V>> v) {
		for (auto &p : v) {
			insert(p.first, p.second);
		}
		return *this;
	}

	template <typename K, typename V>
	int Btree<K, V>::Node::insert(Node *newNode) {
		if (cmp<K>(newNode->key, key) < 0) {
			if (left == nullptr) {
				attachLeftNode(newNode);
			}
			else {
				int weight = abs(left->insert(newNode));
				if (weight == 0) return weight;
				balance = balance - weight;
			}
		}
		else if (cmp<K>(newNode->key, key) > 0) {
			if (right == nullptr) {
				attachRightNode(newNode);
			}
			else {
				int weight = abs(right->insert(newNode));
				if (weight == 0) return weight;
				balance = balance + weight;
			}
		}
		else {
			replace(newNode);
			return 0;
		}
		if (balance < -1) rotateRight(); 
		if (balance > 1) rotateLeft();
		return balance;
	}

	template <typename K, typename V>
	typename Btree<K, V>::Node* Btree<K, V>::Node::rotateLeft() {
		Node *node = detachRightNode();
		if (node->leansLeft()) {
			node = node->rotateRight();
		}
		if (isRoot()) node->type = Type::ROOT;
		else if(hasParent()){
			parent->attachNode(node, type);
		}
		if (node->hasLeftChild()) {
			attachRightNode(node->detachLeftNode());
		}
		node->attachLeftNode(this);
		if (abs(balance) < 2) node->balance = balance * -1;
		setBalance();
		return node;
	}

	template <typename K, typename V>
	typename Btree<K, V>::Node* Btree<K, V>::Node::rotateRight() {
		Node *node = detachLeftNode();
		if (node->leansRight()) {
			node = node->rotateLeft();
		}
		if (isRoot()) node->type = Type::ROOT;
		else if (hasParent()) {
			parent->attachNode(node, type);
		}
		if (node->hasRightChild()) {
			attachLeftNode(node->detachRightNode());
		} 
		node->attachRightNode(this);
		if (abs(balance) < 2) node->balance = balance * -1;
		setBalance();
		return node;
	}

	template <typename K, typename V>
	void Btree<K, V>::Node::setBalance() {
		if (hasNoChildren() || hasTwoChildren()) balance = 0;
		else if (hasLeftChild()) balance = -1;
		else balance = 1;
	}

	template <typename K, typename V>
	Btree<K, V>::Node::Node(K key, V val) {
		init();
		type = Type::ROOT;
		this->key = key;
		this->val = val;
	}

	template <typename K, typename V>
	Btree<K, V>::Node::~Node() {
	}

	template <typename K, typename V>
	void Btree<K, V>::Node::init() {
		left = nullptr;
		right = nullptr;
		parent = nullptr;
		balance = 0;
	}

	template <typename K, typename V>
	typename Btree<K, V>::Node* Btree<K, V>::get(K key) {
		Node *node = nullptr;
		if (!isEmpty()) node = root->get(key);
		return node;
	}

	template <typename K, typename V>
	V Btree<K, V>::getVal(K key) {
		Node *node = get(key);
		if (node == nullptr) {
			std::stringstream ostr;
			ostr << "key not found exception: " << std::to_string(key);
			throw(exception(ostr.str().c_str()));
		}
		return node->val;
	}

	template <typename K, typename V>
	bool Btree<K, V>::contains(K key) {
		Node *node = nullptr;
		bool contains = false;
		if (!isEmpty()) node = root->get(key);
		if (node != nullptr) contains = true;
		return contains;
	}

	template <typename K, typename V>
	bool Btree<K, V>::containsAll(vector<K> keys) {
		bool all = true;
		for (auto &key : keys) {
			if (!contains(key)) {
				all = false;
				break;
			}
		}
		return all;
	}

	template <typename K, typename V>
	typename Btree<K, V>::Node* Btree<K, V>::Node::get(K k) {
		Node *node = nullptr;
		if (cmp<K>(k, key) < 0) {
			if(hasLeftChild()) node = left->get(k);
		}
		else if (cmp<K>(k, key) > 0) {
			if (hasRightChild()) node = right->get(k);
		}
		else {
			node = this;
		}
		return node;
	}

	template <typename K, typename V>
	typename Btree<K, V>::Node* Btree<K, V>::getRoot() {
		return this->root;
	}

	template <typename K, typename V>
	typename Btree<K, V>::Node* Btree<K, V>::beginning() {
		if (isEmpty()) return nullptr;
		return root->beginning();
	}

	template <typename K, typename V>
	typename Btree<K, V>::Node* Btree<K, V>::end() {
		if (isEmpty()) return nullptr;
		return root->end();
	}

	template <typename K, typename V>
	typename V Btree<K, V>::Node::getVal() {
		return val;
	}

	template <typename K, typename V>
	typename K Btree<K, V>::Node::getKey() {
		return key;
	}

	template <typename K, typename V>
	typename Btree<K, V>::Node::Type Btree<K, V>::Node::getType() {
		return type;
	}

	template <typename K, typename V>
	typename Btree<K, V>::Node* Btree<K, V>::Node::next() {
		Node *next = nullptr;
		if (hasRightChild()) {
			next = right->beginning();
		}
		else if (hasParent()) {
			Node* node = this;
			while (node->parent != nullptr) {
				node = node->parent;
				if (cmp(node->val, val) > 0) {
					next = node;
					break;
				}
			}
		}
		return next;
	}

	template <typename K, typename V>
	typename Btree<K, V>::Node* Btree<K, V>::Node::beginning() {
		Node *beginning = this;
		while (beginning->left != nullptr) {
			beginning = beginning->left;
		}
		return beginning;
	}

	template <typename K, typename V>
	typename Btree<K, V>::Node* Btree<K, V>::Node::end() {
		Node *end = this;
		while (end->right != nullptr) {
			end = end->right;
		}
		return end;
	}

	template <typename K, typename V>
	typename Btree<K, V>::Node* Btree<K, V>::Node::previous() {
		Node *last = nullptr;
		if (hasLeftChild()) {
			last = left->end();
		}
		else if (hasParent()) {
			Node* node = this;
			while (node->parent != nullptr) {
				node = node->parent;
				if (cmp(node->val, val) < 0) {
					last = node;
					break;
				}
			}
		}
		return last;
	}

	template <typename K, typename V>
	typename Btree<K, V>::Node* Btree<K, V>::Node::remove() {
		Node *node = parent;
		Node *replace = nullptr;
		if (node == nullptr) {
			replace = previous();
			if (replace == nullptr) replace = next();
			if (replace != nullptr) {
				Node *replaceParent = replace->parent;
				replace->attachLeftNode(detachLeftNode(), false);
				replace->attachRightNode(detachRightNode(), false);
				replace->type = Type::ROOT;
				if (replace->right == replace) replace->right = nullptr;
				if (replace->left == replace) replace->left = nullptr;
				node = replaceParent;
			}
		}
		else if (hasTwoChildren()) {
			node->detachNode(type);
			replace = previous();
			Node *previousParent = replace->parent;
			node->attachNode(previousParent->detachNode(replace->type), type);
			replace->attachLeftNode(detachLeftNode(), false);
			replace->attachRightNode(detachRightNode(), false);
			node = previousParent;
		}
		else if (hasNoChildren()) {
			node->detachNode(type);
		}
		else if (hasLeftChild()) {
			node->detachNode(type);
			replace = detachLeftNode();
			node->attachNode(replace, type);
		}
		else {
			node->detachNode(type);
			replace = detachRightNode();
			node->attachNode(replace, type);
		}
		init();
		Node *newRoot = notifyParents(node);
		if (newRoot != nullptr) replace = newRoot;
		return replace;
	}

	template <typename K, typename V>
	typename Btree<K, V>::Node* Btree<K, V>::Node::notifyParents(Node* node) {
		Type nodeType = type;
		while (node != nullptr) {
			if (nodeType == Type::LEFT) node->balance += 1;
			else if (nodeType == Type::RIGHT) node->balance -= 1;

			if (node->balance < -1) {
				node = node->rotateRight();
				if (node->type == Type::ROOT) {
					return node;
				}
			}
			if (node->balance > 1) {
				node = node->rotateLeft();
				if (node->type == Type::ROOT) {
					return node;
				}
			}
			nodeType = node->type;
			node = node->balance == 0 ? node->parent : nullptr;
		}
		return nullptr;
	}

	template <typename K, typename V>
	typename Btree<K, V>::Node* Btree<K, V>::Node::attachLeftNode(Node *node, bool addWeight) {
		left = node;
		if (node != nullptr) {
			node->type = Type::LEFT;
			node->parent = this;
			if (addWeight) {
				balance -= 1;
			}
		}
		return this;
	}

	template <typename K, typename V>
	typename Btree<K, V>::Node* Btree<K, V>::Node::attachRightNode(Node *node, bool addWeight) {
		right = node;
		if (node != nullptr) {
			node->type = Type::RIGHT;
			node->parent = this;
			if (addWeight) {
				balance += 1;
			}
		}
		return this;
	}

	template <typename K, typename V>
	typename Btree<K, V>::Node* Btree<K, V>::Node::attachNode(Node *node, Type type) {
		switch (type)
		{
		case Type::LEFT:
			attachLeftNode(node, false);
			break;
		case Type::RIGHT:
			attachRightNode(node, false);
			break;
		default:
			break;
		}
		return this;
	}

	template <typename K, typename V>
	typename Btree<K, V>::Node* Btree<K, V>::Node::detachLeftNode() {
		Node *node = nullptr;
		if (hasLeftChild()) {
			node = left;
			left = nullptr;
			node->parent = nullptr;
			return node;
		}
		return node;
	}

	template <typename K, typename V>
	typename Btree<K, V>::Node* Btree<K, V>::Node::detachRightNode() {
		Node *node = nullptr;
		if (hasRightChild()) {
			node = right;
			right = nullptr;
			node->parent = nullptr;
		}
		return node;
	}

	template <typename K, typename V>
	typename Btree<K, V>::Node* Btree<K, V>::Node::detachNode(Type type) {
		Node *node = nullptr;
		if (type == Type::LEFT) node = detachLeftNode();
		else if (type == Type::RIGHT) node = detachRightNode();
		return node;
	}

	template <typename K, typename V>
	void Btree<K, V>::Node::replace(Node *node) {
		val = node->val;
	}

	template <typename K, typename V>
	typename Btree<K, V>::Node* Btree<K, V>::Node::getLeft() {
		return left;
	}

	template <typename K, typename V>
	typename Btree<K, V>::Node* Btree<K, V>::Node::getRight() {
		return right;
	}

	template <typename K, typename V>
	typename Btree<K, V>::Node* Btree<K, V>::Node::getParent() {
		return parent;
	}

	template <typename K, typename V>
	bool Btree<K, V>::Node::hasParent() {
		return parent != nullptr;
	}

	template <typename K, typename V>
	bool Btree<K, V>::Node::hasTwoChildren() {
		return right != nullptr && left != nullptr;
	}

	template <typename K, typename V>
	bool Btree<K, V>::Node::hasChild() {
		return right != nullptr || left != nullptr;
	}

	template <typename K, typename V>
	bool Btree<K, V>::Node::hasNoChildren() {
		return right == nullptr && left == nullptr;
	}

	template <typename K, typename V>
	bool Btree<K, V>::Node::hasRightChild() {
		return right != nullptr;
	}

	template <typename K, typename V>
	bool Btree<K, V>::Node::hasLeftChild() {
		return left != nullptr;
	}

	template <typename K, typename V>
	bool Btree<K, V>::Node::isRightChild() {
		return type == Type::RIGHT;
	}

	template <typename K, typename V>
	bool Btree<K, V>::Node::isLeftChild() {
		return type == Type::LEFT;
	}

	template <typename K, typename V>
	bool Btree<K, V>::Node::isRoot() {
		return type == Type::ROOT;
	}

	template <typename K, typename V>
	bool Btree<K, V>::Node::leansLeft() {
		return balance < 0;
	}

	template <typename K, typename V>
	bool Btree<K, V>::Node::leansRight() {
		return balance > 0;
	}
}

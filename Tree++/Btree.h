#pragma once

#include <iostream>
#include <sstream>
#include <vector>
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
		public:
			enum Type
			{
				ROOT, LEFT, RIGHT
			};

			K getKey();
			V getVal();
			Type getType();
			string toString();
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
			friend class Btree;
		};

		Btree();
		bool isEmpty();
		int size();
		int depth();
		Btree insert(K, V);
		Node* getRoot();
		Node* beginning();
		Node* end();
		Node* get(K);
		Node* remove(K);
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
	};

	template <typename K, typename V>
	Btree<K, V>::Btree() {
		init();
	}

	template <typename K, typename V>
	void Btree<K, V>::init() {
		root = NULL;
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
		return root == NULL;
	}

	template <typename K, typename V>
	void Btree<K, V>::count(Node* node, int *nodes) {
		if (node != NULL) {
			count(node->getLeft(), nodes);
			*nodes += 1;
			count(node->getRight(), nodes);
		}
	}

	template <typename K, typename V>
	void Btree<K, V>::count_depth(Node* node, int *depth, int *high) {
		if (node != NULL) {
			*depth += 1;
			count_depth(node->getLeft(), depth, high);
			count_depth(node->getRight(), depth, high);
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
	typename Btree<K, V>::Node* Btree<K, V>::remove(K key) {
		Node* node = get(key);
		if (node != NULL) {
			if (node->isRoot()) {
				node->init();
				init();
			}
			else {
				node->remove();
			}
		}
		return node;
	}

	template <typename K, typename V>
	void Btree<K, V>::detatchAll(Node* node) {
		if (node != NULL) {
			detatchAll(node->getLeft());
			detatchAll(node->getRight());
			init();
		}
	}

	template <typename K, typename V>
	void Btree<K, V>::print(Node* node) {
		if (node != NULL) {
			print(node->getLeft());
			cout << node->toString().append("\n");
			print(node->getRight());
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
			while (root->getType() != Btree<K, V>::Node::Type::ROOT) {
				root = root->getParent();
			}
		}
		return *this;
	}

	template <typename K, typename V>
	int Btree<K, V>::Node::insert(Node *newNode) {
		if (cmp<K>(newNode->getKey(), getKey()) < 0) {
			if (getLeft() == NULL) {
				attachLeftNode(newNode);
			}
			else {
				int weight = abs(getLeft()->insert(newNode));
				if (weight == 0) return weight;
				balance = balance - weight;
			}
		}
		else if (cmp<K>(newNode->getKey(), getKey()) > 0) {
			if (getRight() == NULL) {
				attachRightNode(newNode);
			}
			else {
				int weight = abs(getRight()->insert(newNode));
				if (weight == 0) return weight;
				balance = balance + weight;
			}
		}
		else {
			replace(newNode);
			return 0;
		}
		if (balance < -1) rotateRight()->balance; 
		if (balance > 1) rotateLeft()->balance;
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
		this->key = key;
		this->val = val;
	}

	template <typename K, typename V>
	void Btree<K, V>::Node::init() {
		left = NULL;
		right = NULL;
		parent = NULL;
		type = Type::ROOT;
		balance = 0;
	}

	template <typename K, typename V>
	string Btree<K, V>::Node::toString() {
		string sep = ", ";
		string s = "[ "+std::to_string(key).append(sep).append("type: ");

		switch (type)
		{
		case Type::LEFT: s.append("left child"); break;
		case Type::RIGHT: s.append("right child"); break;
		case Type::ROOT: s.append("root"); break;
		default:
			break;
		}
		if (!isRoot()) {
			s.append(sep).append("parent: ").append(std::to_string(getParent()->getKey()));
		}
		s.append(sep).append("balance: ").append(std::to_string(balance)).append(" ]");
		return s;
	}

	template <typename K, typename V>
	typename Btree<K, V>::Node* Btree<K, V>::get(K key) {
		Node *node = NULL;
		if (!isEmpty()) node = root->get(key);
		return node;
	}

	template <typename K, typename V>
	V Btree<K, V>::getVal(K key) {
		Node *node = get(key);
		if (node == NULL) {
			std::stringstream ostr;
			ostr << "key not found exception: " << std::to_string(key);
			throw(exception(ostr.str().c_str()));
		}
		return node->getVal();
	}

	template <typename K, typename V>
	bool Btree<K, V>::contains(K key) {
		Node *node = NULL;
		bool contains = false;
		if (!isEmpty()) node = root->get(key);
		if (node != NULL) contains = true;
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
	typename Btree<K, V>::Node* Btree<K, V>::Node::get(K key) {
		Node *node = NULL;
		if (cmp<K>(key, getKey()) < 0) {
			if(hasLeftChild()) node = left->get(key);
		}
		else if (cmp<K>(key, getKey()) > 0) {
			if (hasRightChild()) node = right->get(key);
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
		if (isEmpty()) return NULL;
		return root->beginning();
	}

	template <typename K, typename V>
	typename Btree<K, V>::Node* Btree<K, V>::end() {
		if (isEmpty()) return NULL;
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
		Node *next = NULL;
		if (hasRightChild()) {
			next = right->beginning();
		}
		else if (hasParent()) {
			Node* node = this;
			while (node->parent != NULL) {
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
		while (beginning->left != NULL) {
			beginning = beginning->left;
		}
		return beginning;
	}

	template <typename K, typename V>
	typename Btree<K, V>::Node* Btree<K, V>::Node::end() {
		Node *end = this;
		while (end->right != NULL) {
			end = end->right;
		}
		return end;
	}

	template <typename K, typename V>
	typename Btree<K, V>::Node* Btree<K, V>::Node::previous() {
		Node *last = NULL;
		if (hasLeftChild()) {
			last = left->end();
		}
		else if (hasParent()) {
			Node* node = this;
			while (node->parent != NULL) {
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
		Node *node = NULL;
		if (hasNoChildren()) {
			node = parent->detachNode(type);
			node->init();
		}
		else {
			Node *node = left;
			node = right;
			cout << "TEST";
		}
		return node;
	}

	template <typename K, typename V>
	typename Btree<K, V>::Node* Btree<K, V>::Node::attachLeftNode(Node *node, bool addWeight) {
		left = node;
		node->type = Type::LEFT;
		node->parent = this;
		if (addWeight) {
			balance -= 1;
		}
		return this;
	}

	template <typename K, typename V>
	typename Btree<K, V>::Node* Btree<K, V>::Node::attachRightNode(Node *node, bool addWeight) {
		right = node;
		node->type = Type::RIGHT;
		node->parent = this;
		if (addWeight) {
			balance += 1;
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
		Node *node = NULL;
		if (hasLeftChild()) {
			node = left;
			left = NULL;
			node->parent = NULL;
			return node;
		}
		return node;
	}

	template <typename K, typename V>
	typename Btree<K, V>::Node* Btree<K, V>::Node::detachRightNode() {
		Node *node = NULL;
		if (hasRightChild()) {
			node = right;
			right = NULL;
			node->parent = NULL;
		}
		return node;
	}

	template <typename K, typename V>
	typename Btree<K, V>::Node* Btree<K, V>::Node::detachNode(Type type) {
		Node *node = NULL;
		if (type == Type::LEFT) node = detachLeftNode();
		else if (type == Type::RIGHT) node = detachRightNode();
		return node;
	}

	template <typename K, typename V>
	typename void Btree<K, V>::Node::replace(Node *node) {
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
	typename bool Btree<K, V>::Node::hasParent() {
		return parent != NULL;
	}

	template <typename K, typename V>
	typename bool Btree<K, V>::Node::hasTwoChildren() {
		return right != NULL && left != NULL;
	}

	template <typename K, typename V>
	typename bool Btree<K, V>::Node::hasChild() {
		return right != NULL || left != NULL;
	}

	template <typename K, typename V>
	typename bool Btree<K, V>::Node::hasNoChildren() {
		return right == NULL && left == NULL;
	}

	template <typename K, typename V>
	typename bool Btree<K, V>::Node::hasRightChild() {
		return right != NULL;
	}

	template <typename K, typename V>
	typename bool Btree<K, V>::Node::hasLeftChild() {
		return left != NULL;
	}

	template <typename K, typename V>
	typename bool Btree<K, V>::Node::isRightChild() {
		return type == Type::RIGHT;
	}

	template <typename K, typename V>
	typename bool Btree<K, V>::Node::isLeftChild() {
		return type == Type::LEFT;
	}

	template <typename K, typename V>
	typename bool Btree<K, V>::Node::isRoot() {
		return type == Type::ROOT;
	}

	template <typename K, typename V>
	typename bool Btree<K, V>::Node::leansLeft() {
		return balance < 0;
	}

	template <typename K, typename V>
	typename bool Btree<K, V>::Node::leansRight() {
		return balance > 0;
	}
}

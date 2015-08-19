#pragma once

#include <iostream>
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

			Node(K key, V val);
			K getKey();
			V getVal();
			Node* getLeft();
			Node* getRight();
			int insert(Node*);
			Node* getParent();
			Type getType();
			string toString();

		protected:
			K key;
			V val;
			Node *left;
			Node *right;
			Node *parent;
			int balance;
			Type type;

			bool hasNoChildren();
			bool hasTwoChildren();
			bool hasRightChild();
			bool hasLeftChild();
			bool isRightChild();
			bool isLeftChild();
			bool isRoot();
			bool hasParent();
			void replace(Node*);
		private:
			void attachLeftNode(Node*);
			void attachRightNode(Node*);
			Node* detachLeftNode();
			Node* detachRightNode();
			void rotateLeft();
			void rotateRight();
			void setBalance();
		};

		Btree();
		bool isEmpty();
		int size();
		Btree insert(K, V);
		Node* getRoot();
		void print();

	protected:
		bool DEL = false;
		Node *root;
		void count(Node* node, int *count);
		void print(Node* node);
	};

	template <typename K, typename V>
	Btree<K, V>::Btree() {
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
	void Btree<K, V>::print() {
		print(root);
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
		this->DEL = false;
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
				cout << "attach left\n";
				attachLeftNode(newNode);
			}
			else {
				balance -= abs(getLeft()->insert(newNode));
			}
		}
		else if (cmp<K>(newNode->getKey(), getKey()) > 0) {
			if (getRight() == NULL) {
				cout << "attach right\n";
				attachRightNode(newNode);
			}
			else {
				balance += abs(getRight()->insert(newNode));
			}
		}
		else {
			cout << "equal\n";
			replace(newNode);
		}
		if (balance < -1) rotateRight();
		if (balance > 1) rotateLeft();
		return balance;
	}

	template <typename K, typename V>
	void Btree<K, V>::Node::rotateLeft() {
		cout << "rotate left\n";
		Node *node = detachRightNode();
		node->type = type;
		node->attachLeftNode(this);
		setBalance();
	}

	template <typename K, typename V>
	void Btree<K, V>::Node::rotateRight() {
		cout << "rotate right\n";
		Node *node = detachLeftNode();
		node->type = type;
		node->attachRightNode(this);
		setBalance();
	}

	template <typename K, typename V>
	void Btree<K, V>::Node::setBalance() {
		if (hasNoChildren() || hasTwoChildren()) balance = 0;
		else if (hasLeftChild()) balance = -1;
		else balance = 1;
	}

	template <typename K, typename V>
	Btree<K, V>::Node::Node(K key, V val) {
		this->key = key;
		this->val = val;
		left = right = parent = NULL;
		type = Type::ROOT;
		balance = 0;
	}

	template <typename K, typename V>
	string Btree<K, V>::Node::toString() {
		string sep = ", ";
		string s = std::to_string(key).append(sep).append("type: ");

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
		s.append(sep).append("balance: ").append(std::to_string(balance));
		return s;
	}

	template <typename K, typename V>
	typename Btree<K, V>::Node* Btree<K, V>::getRoot() {
		return this->root;
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
	typename void Btree<K, V>::Node::attachLeftNode(Node *node) {
		left = node;
		node->type = Type::LEFT;
		node->parent = this;
		balance -= 1;
	}

	template <typename K, typename V>
	typename void Btree<K, V>::Node::attachRightNode(Node *node) {
		right = node;
		node->type = Type::RIGHT;
		node->parent = this;
		balance += 1;
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
}

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
		Btree();
		bool isEmpty();
		int size();
		Btree insert(K, V);

		class Node {
		public:
			Node(K key, V val);
			K getKey();
			V getVal();
			Node* getLeft();
			Node* getRight();
			int insert(Node*);
		protected:
			K key;
			V val;
			Node *left;
			Node *right;
			Node *bRlink;
			Node *bLlink;

			bool hasNoChildren();
			bool hasTwoChildren();
			bool hasRightChild();
			bool hasLeftChild();
			bool isRightChild();
			bool isLeftChild();
			bool isRoot();
		private:
			void attachLeftNode(Node*);
			void attachRightNode(Node*);
		};

		Node* getRoot();

	protected:
		int balance;
		int nodes = 0;
		bool DEL = false;
		Node *root;
	};

	template <typename K, typename V>
	Btree<K, V>::Btree() {
		nodes = 0;
		balance = 0;
	}

	template <typename K, typename V>
	bool Btree<K, V>::isEmpty() {
		return nodes == 0;
	}

	template <typename K, typename V>
	int Btree<K, V>::size() {
		return nodes;
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
		}
		this->nodes++;
		return *this;
	}

	template <typename K, typename V>
	int Btree<K, V>::Node::insert(Node *newNode) {
		if (cmp<K>(newNode->getKey(), getKey()) < 0) {
			cout << "go left\n";
			if (getLeft() == NULL) {
				attachLeftNode(newNode);
			}
		}
		else if (cmp<K>(newNode->getKey(), getKey()) > 0) {
			cout << "go right\n";
		}
		else {
			cout << "equal\n";
		}
		return 1;
	}

	template <typename K, typename V>
	Btree<K, V>::Node::Node(K key, V val) {
		this->key = key;
		this->val = val;
		left = right = bRlink = bLlink = NULL;
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
	typename void Btree<K, V>::Node::attachLeftNode(Node *node) {
		left = node;
		node->bLlink = left;
	}

	template <typename K, typename V>
	typename void Btree<K, V>::Node::attachRightNode(Node *node) {
		right = node;
		node->bRlink = right;
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
		return bRlink != NULL;
	}

	template <typename K, typename V>
	typename bool Btree<K, V>::Node::isLeftChild() {
		return bLlink != NULL;
	}

	template <typename K, typename V>
	typename bool Btree<K, V>::Node::isRoot() {
		return bLlink == NULL && bRlink == NULL;
	}
}

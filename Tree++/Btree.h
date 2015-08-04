#pragma once

#include <iostream>
using namespace std;

namespace tree {
	template <typename K, typename V>
	class Btree {
	public:
		Btree();
		Btree(K, V);
		Btree insert(K, V);
		Btree remove();
		Btree removeAt(K);
		Btree get(K);
	protected:
		K key;
		V val;
		int root;
		int balance;
		Btree *left;
		Btree *right;
		Btree *bRlink;
		Btree *bLlink;
		int nodes = 0;
		bool DEL = false;

		Btree insert(Btree*);
	};

	template <typename K, typename V>
	Btree<K, V>::Btree() {
		root = true;
		left = right = bRlink = bLlink = NULL;
		nodes = 0;
		balance = 0;
	}

	template <typename K, typename V>
	Btree<K, V>::Btree(K key, V val) {
		this->key = key;
		this->val = val;
		root = true;
		left = right = bRlink = bLlink = NULL;
		nodes = 0;
		balance = 0;
	}

	/************************************************
	Description: inserts key and val into the tree
	key: The value to key the node on.
	val: Value associated with key.
	************************************************/
	template <typename K, typename V>
	Btree<K, V> Btree<K, V>::insert(K key, V value) {
		if (this->root)
		{
			this->DEL = false; this->nodes++;
			Btree<K, V> *newNode = new Btree<K, V>(key, value);
			return this->insert(newNode);
		}
		return *this;
	}

	/************************************************
	Description: Inserts key and value into the tree
	node: The tree node to insert
	************************************************/
	template <typename K, typename V>
	Btree<K, V> Btree<K, V>::insert(Btree *node) {
		cout << "insert. ";
		this->right = node;
		return *this;
	}

	template<typename K, typename V>
	Btree<K, V> Btree<K, V>::remove()
	{
		return this->right;
	}

	template<typename K, typename V>
	Btree<K, V> Btree<K, V>::removeAt(K key)
	{
		return this->right;
	}

	template<typename K, typename V>
	Btree<K, V> Btree<K, V>::get(K key)
	{
		return this->right;
	}
}

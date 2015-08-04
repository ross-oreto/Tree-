#pragma once

#include <iostream>
using namespace std;

namespace tree {
	template <typename K, typename V>
	class Btree {
	public:
		Btree();
		Btree(K, V);
		int insert(K, V);
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

	template <typename K, typename V>
	int Btree<K, V>::insert(K key, V value) {
		cout << "hello world";
		return 0;
	}
}

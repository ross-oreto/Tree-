// test.cpp : Defines the entry point for the console application.
//
#include <string>
#include <iostream>
#include <vector>
#include "stdafx.h"
#include <time.h>
#include "Btree.h"
#include <map>
#include <assert.h>  
#include <functional>

using namespace tree;

Btree<int, int> insertInts(std::vector<int> v) {
	Btree<int, int> btree = Btree<int, int>();
	for (auto &i : v) {
		btree.insert(i, i);
	}
	return btree;
}

void example() {
	// create
	Btree<int, string> btree = Btree<int, string>();

	// inserts
	btree.insert(1, "a").insert(2, "b").insert(3, "c");

	// query
	Btree<int, string>::Node *node = btree.get(1);
	cout << node->getVal();

	// loop
	for (node = btree.beginning(); node != nullptr; node = node->next()) {
		cout << node->getKey() + ":" + node->getVal();
	}

	// deletions
	btree.remove(1);
	btree.pop();  // removes last node
	btree.pull(); // removes first node

	// clear tree
	btree.clear();
}

class ex1 {
public:
	string name;
	ex1() {

	}
	ex1(string s) {
		name = s;
	}
	friend bool operator<(const ex1& l, const ex1& r) {
		return l.name < r.name;
	}
	friend bool operator>(const ex1& l, const ex1& r) {
		return l.name > r.name;
	}
	friend std::ostream& operator<<(std::ostream& os, ex1& obj) {
		os << obj.name;
		return os;
	}
};

void example2() {
	// create
	Btree<ex1, string> btree = Btree<ex1, string>();
	btree.insert(ex1("a"), "val1").insert(ex1("b"), "val2");
	btree.print();
}

map<int, int> mapInsertInts(std::vector<int> v) {
	map<int, int> rbtree = map<int, int>();
	for (auto &i : v) {
		rbtree.insert(std::pair<int, int>(i, i));
	}
	return rbtree;
}

vector<int> randomInts(int size, int max) {
	std::vector<int> v;
	for (int i = 0; i < size; i++) {
		int r = rand() % max;
		v.push_back(r);
	}
	return v;
}

void runOps(int size, int max) {
	std::vector<int> v = randomInts(size, max);
	Btree<int, int> btree = insertInts(v);
	int queries = size / 2;
	for (int i = 0; i < queries; i++) {
		btree.get(v.at(rand() % size));
	}
	for (Btree<int, int>::Node *node = btree.beginning(); node != nullptr; node = node->next()) {
		assert(node->getKey() == node->getVal());
	}
	btree.clear();
}

void mapRunOps(int size, int max) {
	std::vector<int> v = randomInts(size, max);
	map<int, int> rbtree = mapInsertInts(v);
	int queries = size / 2;
	for (int i = 0; i < queries; i++) {
		rbtree.find(v.at(rand() % size));
	}
	map<int, int>::iterator it;
	for (it = rbtree.begin(); it != rbtree.end(); it++) {
		assert(it->first == it->second);
	}
	rbtree.clear();
}

double diffclock(clock_t clock1, clock_t clock2) {

	double diffticks = clock1 - clock2;
	double diffms = diffticks / (CLOCKS_PER_SEC / 1000);

	return diffms;
}

double benchmark(std::function<void (int, int)> func, int size, int max) {
	clock_t time_a = clock();
	func(size, max);
	clock_t time_b = clock();
	return diffclock(time_b, time_a);
}

void benchmarkInserts() {
	int size = 20000;
	int max = 10000000;
	double total_time_ticks1 = benchmark(&mapRunOps, size, max);
	double total_time_ticks2 = benchmark(&runOps, size, max);

	assert(total_time_ticks2 < total_time_ticks1);
	cout << "benchmark insert " << size << " random integers with 3000 queries iterated and cleared" << endl;
	cout << "map: " + std::to_string(total_time_ticks1) << " ms" << endl;
	cout << "btree: " + std::to_string(total_time_ticks2) << " ms" << endl;
}

int main()
{
	_set_error_mode(_OUT_TO_MSGBOX);
	try {
		srand(time(nullptr));
		benchmarkInserts();
	}
	catch (const std::exception &e) {
		cout << e.what();
	}
    return 0;
}



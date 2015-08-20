// test.cpp : Defines the entry point for the console application.
//
#include <string>
#include <iostream>
#include <vector>
#include "stdafx.h"
#include <time.h>
#include "Btree.h"
#include <map>

using namespace tree;

Btree<int, int> insertInts(std::vector<int> v) {
	Btree<int, int> btree = Btree<int, int>();
	for (auto &i : v) {
		btree.insert(i, i);
	}
	btree.print();
	cout << std::string("Size: ").append(std::to_string(btree.size())).append("\n");
	return btree;
}

void test1() {
	std::vector<int> v{ 10, 4, 7 };
	Btree<int, int> btree = insertInts(v);
	btree.getVal(1);
}

void test2() {
	std::vector<int> v{ 0, 2, 5, 10, 15, 20, 12, 14, 13, 25, 0, 2, 5, 10, 15, 20, 12, 14, 13, 25 };
	insertInts(v);
}

void test3() {
	std::vector<int> v;
	/* initialize random seed: */
	srand(time(NULL));
	for (int i = 0; i < 1000; i++) {
		int r = rand() % 2000;
		v.push_back(r);
	}
	insertInts(v);
}

int main()
{
	std::map<char, int>::iterator i;
	try {
		test1();
		//test2();
		//test3();
	}
	catch (exception e) {
		cout << e.what();
	}
    return 0;
}



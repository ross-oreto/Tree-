// test.cpp : Defines the entry point for the console application.
//
#include <string>
#include <iostream>
#include <vector>
#include "stdafx.h"
#include "Btree.h"

using namespace tree;

int main()
{
	//std::vector<double> v{ 4, 2, 0, 3, 3.5, 7, 20 };
	//std::vector<double> v{ 0, 2, 5 };
	std::vector<double> v{ 5, 2, 0 };
	Btree<double, double> btree = Btree<double, double>();
	for (auto &i : v) {
		btree.insert(i, i);
	}
	Btree<double, double>::Node *root = btree.getRoot();
	btree.print();
	cout << std::string("Size: ").append(std::to_string(btree.size()));
    return 0;
}



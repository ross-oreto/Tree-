// test.cpp : Defines the entry point for the console application.
//
#include <string>
#include <iostream>
#include "stdafx.h"
#include "Btree.h"

using namespace tree;

int main()
{
	Btree<int, string> btree = Btree<int, string>();
	btree.insert(5, "5");
	btree.insert(3, "3");
	btree.insert(7, "7");
	btree.insert(5, "5");
	Btree<int, string>::Node *root = btree.getRoot();
	cout << std::string("Size: ").append(std::to_string(btree.size()));
    return 0;
}



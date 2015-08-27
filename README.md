# tree-plus
* AVL tree implementation in c++.
* Performs well for heavy searching on large data sets and small amounts of tree modification.
* 400% faster than c++ map in queries, inserts, iteration, clearing.

### Example usage
	// create
	Btree<int, string> btree = Btree<int, string>();

	// inserts
	btree.insert(1, "a").insert(2, "b").insert(3, "c");

	// query
	Btree<int, string>::Node *node = btree.get(1);
	cout << node->getVal();

	// loop
	for (node = btree.beginning(); node != NULL; node = node->next()) {
		cout << node->getKey() + ":" + node->getVal();
	}

	// deletions
	btree.remove(1);
	btree.pop();  // removes last node
	btree.pull(); // removes first node

	// clear tree
	btree.clear();

### VS 2015 setup
* After checking out and opening the project in VS, right click on the test projects and choose properites.
* Under C/C++ -> General, edit the "Additional Include Directories" path and select the Tree++ project directory.
* Click apply and OK, you should now be able to build both the solution.



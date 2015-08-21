#include "stdafx.h"
#include "CppUnitTest.h"
#include "Btree.h"
#include <time.h>
using namespace tree;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest
{		
	Btree<int, int> insertInts(std::vector<int> v) {
		Btree<int, int> btree = Btree<int, int>();
		for (auto &i : v) {
			btree.insert(i, i);
		}
		return btree;
	}
	Btree<int, int> btree;

	TEST_CLASS(UnitTest1)
	{
	public:
		TEST_CLASS_INITIALIZE(init)
		{
			btree = insertInts({ 10, 4, 7 });
		}
		TEST_METHOD(TestTreeSimple)
		{
			Assert::AreEqual(btree.size(), 3);
			Assert::IsNotNull(btree.get(4));
			Assert::IsNull(btree.get(44));
			Assert::IsTrue(btree.contains(7));
			Assert::IsFalse(btree.contains(77));
			Assert::IsTrue(btree.containsAll({ 10, 4, 7 }));
			Assert::AreEqual(btree.getVal(10), 10);
			wchar_t message[200];
			Assert::ExpectException<exception>([&]
			{
				btree.getVal(1);
			});
		}
		TEST_METHOD(TestTree)
		{
			btree = insertInts({ 0, 2, 5, 10, 15, 20, 12, 14, 13, 25, 0, 2, 5, 10, 15, 20, 12, 14, 13, 25 });
			Assert::AreEqual(btree.size(), 10);
		}
		TEST_METHOD(TestTreeRandom)
		{
			std::vector<int> v;
			/* initialize random seed: */
			srand(time(NULL));
			for (int i = 0; i < 1000; i++) {
				int r = rand() % 2000;
				v.push_back(r);
			}
			btree = insertInts(v);
			Assert::IsFalse(btree.isEmpty());
		}
	};
}
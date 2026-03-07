#include "pch.h"
#include "CppUnitTest.h"
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

std::vector<int> deletedrand(std::vector<int> arr) {
    if (arr.empty()) return arr;

    int random_value = arr[rand() % arr.size()];
    std::vector<int> new_arr;

    for (int i = 0; i < arr.size(); i++) {
        if (arr[i] != random_value) {
            new_arr.push_back(arr[i]);
        }
    }
    return new_arr;
}

namespace UnitTest1
{
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
            std::vector<int> empty;
            auto result = deletedrand(empty);
            Assert::IsTrue(result.empty());
		}
        TEST_METHOD(TestMethod2)
        {
            std::vector<int> arr = { 5 };
            auto result = deletedrand(arr);
            Assert::IsTrue(result.empty());
        }

        TEST_METHOD(TestMethod3)
        {
            std::vector<int> arr = { 7, 7, 7, 7 };
            auto result = deletedrand(arr);
            Assert::IsTrue(result.empty());
        }

        TEST_METHOD(TestMethod)
        {
            std::vector<int> arr = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
            auto result = deletedrand(arr);
            Assert::IsTrue(result.size() < arr.size());
        }
	};
}

#include "pch.h"
#include "CppUnitTest.h"
#include <vector>


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

void gnomsort(std::vector<int>& arr) {
	int i = 1;
	while (i < (int)arr.size()) {
		if (i > 0 && arr[i - 1] > arr[i]) {
			std::swap(arr[i - 1], arr[i]);
			i--;
		}
		else {
			i++;
		}
	}
}


void sortporazryd(std::vector<int>& arr, int razr) {
	std::vector<std::vector<int>> korziny(10);

	for (int x : arr) {
		int cifra = (x / razr) % 10;
		korziny[cifra].push_back(x);
	}

	std::vector<int> noviy;
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < (int)korziny[i].size(); j++) {
			noviy.push_back(korziny[i][j]);
		}
	}
	arr = noviy;
}

void porazrydsort(std::vector<int>& arr) {
	if (arr.empty()) return;

	int maxx = arr[0];
	for (int i = 1; i < (int)arr.size(); i++) {
		if (arr[i] > maxx) maxx = arr[i];
	}

	int razr = 1;
	while (maxx / razr > 0) {
		sortporazryd(arr, razr);
		razr = razr * 10;
	}
}


int razdel(std::vector<int>& arr, int d, int h) {
	int opora = arr[h];
	int i = d - 1;

	for (int j = d; j <= h - 1; j++) {
		if (arr[j] <= opora) {
			i++;
			std::swap(arr[i], arr[j]);
		}
	}
	std::swap(arr[i + 1], arr[h]);
	return i + 1;
}

void fastsort(std::vector<int>& arr, int d, int h) {
	if (d < h) {
		int opori = razdel(arr, d, h);
		fastsort(arr, d, opori - 1);
		fastsort(arr, opori + 1, h);
	}
}

namespace UnitTest1
{
	TEST_CLASS(UnitTest1)
	{
	public:
		
		TEST_METHOD(GnomeSort_1)
		{
			std::vector<int> arr = { 3, 1, 2 };
			gnomsort(arr);

			Assert::AreEqual(1, arr[0]);
			Assert::AreEqual(2, arr[1]);
			Assert::AreEqual(3, arr[2]);
		}
		
		TEST_METHOD(GnomeSort_2)
		{
			std::vector<int> arr = { 1, 2, 3 };
			gnomsort(arr);

			Assert::AreEqual(1, arr[0]);
			Assert::AreEqual(2, arr[1]);
			Assert::AreEqual(3, arr[2]);
		}

		
		TEST_METHOD(RadixSort_1)
		{
			std::vector<int> arr = { 2, 3, 1 };
			porazrydsort(arr);

			Assert::AreEqual(1, arr[0]);
			Assert::AreEqual(2, arr[1]);
			Assert::AreEqual(3, arr[2]);
		}

		
		TEST_METHOD(RadixSort_2)
		{
			std::vector<int> arr = { 2, 1, 2, 1 };
			porazrydsort(arr);

			Assert::AreEqual(1, arr[0]);
			Assert::AreEqual(1, arr[1]);
			Assert::AreEqual(2, arr[2]);
			Assert::AreEqual(2, arr[3]);
		}

		
		TEST_METHOD(QuickSort_1)
		{
			std::vector<int> arr = { 3, 1, 2 };
			fastsort(arr, 0, (int)arr.size() - 1);

			Assert::AreEqual(1, arr[0]);
			Assert::AreEqual(2, arr[1]);
			Assert::AreEqual(3, arr[2]);
		}

		
		TEST_METHOD(QuickSort_2)
		{
			std::vector<int> arr = {0};
			fastsort(arr, 0, (int)arr.size() - 1);

			Assert::AreEqual(0, arr[0]);
			
		}
	};
}

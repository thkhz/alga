#include "pch.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

double f1(double x) {
    return 3 - x;
}

double f2(double x) {
    return -3 + x;
}

double f3(double x) {
    return -3 - x;
}

double f4(double x) {
    return 3 + x;
}


bool is_point_inside(double x, double y) {
    if (x >= 0) {
        return (y <= f1(x) && y >= f2(x));
    }
    else {
        return (y <= f4(x) && y >= f3(x));
    }
}


double monte_carlo_area(double x_min, double x_max, double y_min, double y_max, int num_points) {
    int inside_points = 0;

    for (int i = 0; i < num_points; i++) {
        double x = x_min + (x_max - x_min) * ((double)rand() / RAND_MAX);
        double y = y_min + (y_max - y_min) * ((double)rand() / RAND_MAX);

        if (is_point_inside(x, y)) {
            inside_points++;
        }
    }

    double rectarea = (x_max - x_min) * (y_max - y_min);
    double area = (inside_points / (double)num_points) * rectarea;

    return area;
}

namespace UnitTest1
{
	TEST_CLASS(UnitTest1)
	{
	public:
		
        TEST_METHOD(Inside)
        {
            Assert::AreEqual(true, is_point_inside(1, 1));
        }

        TEST_METHOD(Outside)
        {
            Assert::AreEqual(false, is_point_inside(10, 10));
        }

        TEST_METHOD(granica)
        {
            Assert::AreEqual(true, is_point_inside(3, 0));
        }

        TEST_METHOD(Arean)
        {
            double area = monte_carlo_area(-4, 4, -4, 4, 1000);
            Assert::AreEqual(18.0, area, 2.0); 
        }

        TEST_METHOD(Areat)
        {
            double area = monte_carlo_area(-4, 4, -4, 4, 100000);
            Assert::AreEqual(18.0, area, 0.3); 
        }
	};
}



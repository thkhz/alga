#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <cmath>



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

int main() {
    setlocale(LC_ALL, "Russian");
    srand(time(0));

    double x_min = -4, x_max = 4;
    double y_min = -4, y_max = 4;
    double s = 18.0;


    int N[] = { 100, 500, 1000, 5000, 10000, 50000, 100000, 500000, 1000000, 10000000 };
    

    std::cout << std::setw(12) << "N (точек)"
        << std::setw(15) << "Площадь"
        << std::setw(15) << "Ошибка"
        << std::setw(15) << "Отн. ошибка %" << std::endl;
    std::cout << "---------------------------------------------------------" << std::endl;

    
    for (int i = 0; i < 10; i++) {
        int Nn = N[i];
        double estimated_area = monte_carlo_area(x_min, x_max, y_min, y_max, Nn);
        double error = abs(estimated_area - s);
        double relative_error = (error / s) * 100;

        
        std::cout << std::setw(12) << Nn
            << std::setw(15) << estimated_area
            << std::setw(15) << error
            << std::setw(14) <<  relative_error << "%" << std::endl;
    }

    return 0;
}












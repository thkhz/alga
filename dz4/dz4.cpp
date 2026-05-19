#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <locale>
#include <chrono>
#include <thread>


template <typename Func, typename... Args> auto measure_execution_time(Func&& func, Args&&... args) {
    auto start_time = std::chrono::high_resolution_clock::now();  // Начинаем отсчет времени
    func(std::forward<Args>(args)...);  // Выполняем переданную функцию с аргументами
    auto end_time = std::chrono::high_resolution_clock::now();  // Останавливаем отсчет времени

    auto duration = std::chrono::duration<double, std::milli>(end_time - start_time);  // Вычисляем продолжительность в микросекундах
    return duration.count();  // Возвращаем результат и время выполнения
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


void parallsort(std::vector<int>& arr, int d, int h, int num_threads) {
    if (d >= h) {
        return;
    }

    if (h - d < 1000 || num_threads <= 1) {
        fastsort(arr, d, h);
        return;
    }

    int opori = razdel(arr, d, h);

    if (num_threads > 1) {
        int left_threads = num_threads / 2;
        int right_threads = num_threads - left_threads;

        std::thread t1(parallsort, std::ref(arr), d, opori - 1, left_threads);
        std::thread t2(parallsort, std::ref(arr), opori + 1, h, right_threads);

        t1.join();
        t2.join();
    }
    else {
        fastsort(arr, d, opori - 1);
        fastsort(arr, opori + 1, h);
    }
}





int main() {
    setlocale(LC_ALL, "Russian");

    std::vector<int> sizes = { 1000, 10000, 50000, 100000, 250000, 500000 };


    std::cout << std::setw(12) << "n"
        << std::setw(20) << "Быстрая сортировка (мс)"
        << std::setw(20) << "БС П 2 (мс)"
        << std::setw(20) << "БС П 4 (мс)"
        << std::setw(20) << "БС П 8 (мс)"
        << "\n";

    for (int n : sizes) {

        std::vector<int> arr(n);
        for (int i = 0; i < n; i++) {
            arr[i] = rand() % 10000;
        }

        std::vector<int >arr1 = arr;
        double time1 = measure_execution_time(fastsort, arr1, 0, (int)arr1.size() - 1);
        std::vector<int >arr2 = arr;
        double time2 = measure_execution_time(parallsort, arr2, 0, (int)arr2.size() - 1, 2);
        std::vector<int >arr3 = arr;
        double time3 = measure_execution_time(parallsort, arr3, 0, (int)arr3.size() - 1, 4);
        std::vector<int >arr4 = arr;
        double time4 = measure_execution_time(parallsort, arr4, 0, (int)arr4.size() - 1, 8);


        std::cout << std::setw(12) << n
            << std::setw(20) << time1
            << std::setw(20) << time2
            << std::setw(20) << time3
            << std::setw(20) << time4
            << "\n";


    }

    return 0;
}


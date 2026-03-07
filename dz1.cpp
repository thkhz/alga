// практика1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <limits>
#include <vector>
#include <chrono>
#include <iomanip>

template <typename Func, typename... Args> auto measure_execution_time(Func&& func, Args&&... args) {
    auto start_time = std::chrono::high_resolution_clock::now();  // Начинаем отсчет времени
    auto result = func(std::forward<Args>(args)...);  // Выполняем переданную функцию с аргументами
    auto end_time = std::chrono::high_resolution_clock::now();  // Останавливаем отсчет времени

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);  // Вычисляем продолжительность в миллисекундах
    return std::make_pair(result, duration.count());  // Возвращаем результат и время выполнения
}


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

size_t pamyt(int n) {
    return n * sizeof(int);
}

int main() {
    setlocale(LC_ALL, "Russian");

    std::vector<int> sizes = { 10, 100, 1000, 10000, 50000, 100000, 250000, 500000, 1000000,10000000 };

    std::cout << "Размер int: " << sizeof(int) << " байт\n\n";

    std::cout << std::setw(12) << "n"
        << std::setw(20) << "Время (мс)"
        << std::setw(20) << "Память (КБ)"
        << "\n";

    for (int n : sizes) {

        std::vector<int> arr(n);
        for (int i = 0; i < n; i++) {
            arr[i] = rand() % 10 + 1;
        }

        auto result_pair = measure_execution_time(deletedrand, arr);
        std::vector<int> result = result_pair.first;
        auto time = result_pair.second;

        size_t memory = pamyt(n);

        std::cout << std::setw(12) << n
            << std::setw(20) << time
            << std::setw(20) << memory / 1024.0
            << "\n";
    }

    return 0;
}


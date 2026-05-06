#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
#include <locale>
#include <chrono>


template <typename Func, typename... Args> auto measure_execution_time(Func&& func, Args&&... args) {
    auto start_time = std::chrono::high_resolution_clock::now();  // Начинаем отсчет времени
    func(std::forward<Args>(args)...);  // Выполняем переданную функцию с аргументами
    auto end_time = std::chrono::high_resolution_clock::now();  // Останавливаем отсчет времени

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);  // Вычисляем продолжительность в миллисекундах
    return duration.count();  // Возвращаем результат и время выполнения
}


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

    for (int j = d; j <= h-1; j++) {
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


int main() {
    setlocale(LC_ALL, "Russian");

    std::vector<int> sizes = {1000, 10000, 20000, 30000, 40000, 50000, 60000, 70000, 80000, 90000, 100000};


    std::cout << std::setw(12) << "n"
        << std::setw(20) << "Гномья сортировка (мс)"
        << std::setw(20) << "Поразрядная сортировка (мс)"
        << std::setw(20) << "Быстрая сортировка (мс)"
        << "\n";

    for (int n : sizes) {

        std::vector<int> arr(n);
        for (int i = 0; i < n; i++) {
            arr[i] = rand() % 10000;
        }

        std::vector<int >arr1 = arr;
        double time1 = measure_execution_time(gnomsort, arr1);

        std::vector<int >arr2 = arr;
        double time2 = measure_execution_time(porazrydsort, arr2);

        std::vector<int >arr3 = arr;
        double time3 = measure_execution_time(fastsort, arr3, 0, (int)arr3.size() - 1);
        

        std::cout << std::setw(12) << n
            << std::setw(20) << time1
            << std::setw(20) << time2
            << std::setw(20) << time3
            << "\n";


    }
    
    return 0;
}


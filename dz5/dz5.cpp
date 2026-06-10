#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <map>
#include <vector>
#include <tuple>
#include <queue>
#include <limits>
#include <string>
#include <algorithm>
#include <windows.h>
#include <cstdlib>
#include <chrono>
#include <clocale>

template <typename Func, typename... Args>
auto measure_execution_time(Func&& func, Args&&... args) {
    auto start_time = std::chrono::high_resolution_clock::now();
    auto result = func(std::forward<Args>(args)...);
    auto end_time = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    return std::make_pair(result, duration.count());
}

// TinyXML-2 dependency
#include "tinyxml2.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

using std::string;
using std::vector;
using std::map;
using std::tuple;
using std::pair;
using std::make_pair;
using std::make_tuple;
using std::cout;
using std::endl;

typedef pair<double, double> Coord;
typedef tuple<Coord, Coord, string> EdgeItem;
typedef map<Coord, vector<pair<Coord, double>>> Graph;

// Функция haversine: вычисляет расстояние между двумя точками на поверхности Земли (в километрах)
double haversine(const Coord& coord1, const Coord& coord2) {
    // Вычисляем расстояние между точками на поверхности Земли (в километрах)
    double lon1 = coord1.first, lat1 = coord1.second;
    double lon2 = coord2.first, lat2 = coord2.second;
    double R = 6371.0;  // Радиус Земли в км

    double phi1 = lat1 * M_PI / 180.0;
    double phi2 = lat2 * M_PI / 180.0;
    double dphi = (lat2 - lat1) * M_PI / 180.0;
    double dlambda = (lon2 - lon1) * M_PI / 180.0;

    double a = std::sin(dphi / 2.0) * std::sin(dphi / 2.0) +
        std::cos(phi1) * std::cos(phi2) *
        std::sin(dlambda / 2.0) * std::sin(dlambda / 2.0);
    return 2 * R * std::atan2(std::sqrt(a), std::sqrt(1 - a));
}

// Функция dijkstra: поиск кратчайшего пути в графе
// graph: Граф представленный в виде словаря: {координата: список {(соседняя координата, расстояние)}}
// start: стартовая координата
// end: конечная координата
// Возвращает кортеж (путь, общее расстояние, список названий улиц)
tuple<vector<Coord>, double, vector<string>> dijkstra(const Graph& graph, const Coord& start, const Coord& end) {
    // Приоритетная очередь для хранения (расстояние, узел)
    typedef pair<double, Coord> QueueItem;
    auto cmp = [](const QueueItem& left, const QueueItem& right) {
        return left.first > right.first;
        };
    std::priority_queue<QueueItem, vector<QueueItem>, decltype(cmp)> priority_queue(cmp);
    priority_queue.push(make_pair(0.0, start));

    // Словарь для хранения кратчайшего расстояния до каждого узла и предыдущего узла
    map<Coord, pair<Coord, double>> shortest_paths;
    shortest_paths[start] = make_pair(start, 0.0);

    // Множество посещённых узлов
    vector<Coord> visited;

    while (!priority_queue.empty()) {
        double current_distance = priority_queue.top().first;
        Coord current_node = priority_queue.top().second;
        priority_queue.pop();

        // Если достигли конечного узла, прерываем поиск
        if (current_node == end) {
            break;
        }

        // Если узел уже посещён, пропускаем
        if (std::find(visited.begin(), visited.end(), current_node) != visited.end()) {
            continue;
        }

        visited.push_back(current_node);

        // Получаем список соседей, если он существует
        auto it = graph.find(current_node);
        if (it != graph.end()) {
            for (auto& neighbor_pair : it->second) {
                Coord neighbor = neighbor_pair.first;
                double distance = neighbor_pair.second;
                double total_distance = current_distance + distance;

                if (shortest_paths.find(neighbor) == shortest_paths.end() ||
                    total_distance < shortest_paths[neighbor].second) {
                    shortest_paths[neighbor] = make_pair(current_node, total_distance);
                    priority_queue.push(make_pair(total_distance, neighbor));
                }
            }
        }
    }

    // Восстановление пути
    vector<Coord> path;
    vector<string> street_names;
    if (shortest_paths.find(end) == shortest_paths.end()) {
        return make_tuple(path, std::numeric_limits<double>::infinity(), street_names);
    }

    // Реконструкция пути от конца к началу
    Coord current_node = end;
    while (true) {
        path.push_back(current_node);
        auto pred = shortest_paths[current_node].first;
        // Если предшественник равен (0,0) и расстояние равно 0, то это стартовый узел
        if (current_node == start) {
            break;
        }
        current_node = pred;
    }
    std::reverse(path.begin(), path.end());

    // Общее расстояние от начала до конца
    double total_distance = shortest_paths[end].second;
    return make_tuple(path, total_distance, street_names);
}

// Функция build_graph: строит граф из рёбер
// edges: список рёбер формата ((x1, y1), (x2, y2), название_улицы)
Graph build_graph(const vector<EdgeItem>& edges) {
    // Строим граф
    Graph graph;
    for (const auto& edge : edges) {
        Coord start, end;
        string street;
        std::tie(start, end, street) = edge;
        double dist = haversine(start, end);
        graph[start].push_back(make_pair(end, dist));
        graph[end].push_back(make_pair(start, dist));
    }
    return graph;
}

// кодировка
std::string utf8_to_cp1251(const std::string& utf8_str) {
    int wide_len = MultiByteToWideChar(CP_UTF8, 0, utf8_str.c_str(), -1, nullptr, 0);
    std::wstring wide_str(wide_len, L'\0');
    MultiByteToWideChar(CP_UTF8, 0, utf8_str.c_str(), -1, &wide_str[0], wide_len);

    int cp1251_len = WideCharToMultiByte(1251, 0, wide_str.c_str(), -1, nullptr, 0, nullptr, nullptr);
    std::string cp1251_str(cp1251_len, '\0');
    WideCharToMultiByte(1251, 0, wide_str.c_str(), -1, &cp1251_str[0], cp1251_len, nullptr, nullptr);

    return cp1251_str;
}
// Функция read_graphml: читает GraphML файл и возвращает узлы и ребра с названиями улиц
// file_path: путь к файлу .graphml
// Возвращает кортеж (nodes, edges), где:
// - nodes: словарь {node_id: (x, y)}
// - edges: список [((x1, y1), (x2, y2), название_улицы), ...]


tuple<map<string, Coord>, vector<EdgeItem>> read_graphml(const string& file_path) {
    map<string, Coord> nodes;
    vector<EdgeItem> edges;
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(file_path.c_str()) != tinyxml2::XML_SUCCESS) {
        std::cerr << "Error loading XML file: " << file_path << endl;
        return make_tuple(nodes, edges);
    }

    tinyxml2::XMLElement* graphml = doc.FirstChildElement("graphml");
    if (!graphml) {
        std::cerr << "Нет файла" << endl;
        return make_tuple(nodes, edges);
    }

    tinyxml2::XMLElement* graph = graphml->FirstChildElement("graph");
    if (!graph) {
        std::cerr << "Нет элементов графа" << endl;
        return make_tuple(nodes, edges);
    }

    for (tinyxml2::XMLElement* node = graph->FirstChildElement("node"); node; node = node->NextSiblingElement("node")) {
        const char* id = node->Attribute("id");
        if (!id) continue;

        double x = 0, y = 0;
        bool has_coords = false;

        for (tinyxml2::XMLElement* data = node->FirstChildElement("data"); data; data = data->NextSiblingElement("data")) {
            const char* key = data->Attribute("key");
            if (!key) continue;

            if (strcmp(key, "d5") == 0) {
                x = atof(data->GetText());
                has_coords = true;
            }
            else if (strcmp(key, "d4") == 0) {
                y = atof(data->GetText());
                has_coords = true;
            }
        }

        if (has_coords) {
            nodes[id] = make_pair(x, y);
        }
    }

    for (tinyxml2::XMLElement* edge = graph->FirstChildElement("edge"); edge; edge = edge->NextSiblingElement("edge")) {
        const char* source_id = edge->Attribute("source");
        const char* target_id = edge->Attribute("target");
        if (!source_id || !target_id) continue;

        auto source_it = nodes.find(source_id);
        auto target_it = nodes.find(target_id);
        if (source_it == nodes.end() || target_it == nodes.end()) continue;

        string street_name;
        for (tinyxml2::XMLElement* data = edge->FirstChildElement("data"); data; data = data->NextSiblingElement("data")) {
            const char* key = data->Attribute("key");
            if (!key) continue;

            if (strcmp(key, "d15") == 0 && data->GetText()) {
                street_name = utf8_to_cp1251(data->GetText());
            }
        }

        edges.emplace_back(source_it->second, target_it->second, street_name);
    }

    printf("Количество вершин (Nodes) %d ребер (Edges) %d \n", static_cast<int>(nodes.size()), static_cast<int>(edges.size()));

    return make_tuple(nodes, edges);
}

std::string convert_cp1251_to_utf8(const std::string& input) {
    // 1. CP1251 → UTF-16 (Windows-1251 → wchar_t)
    int wlen = MultiByteToWideChar(1251, 0, input.c_str(), -1, nullptr, 0);
    if (wlen <= 0) return "";
    std::wstring wstr(wlen, L'\0');
    MultiByteToWideChar(1251, 0, input.c_str(), -1, &wstr[0], wlen);

    // 2. UTF-16 → UTF-8
    int ulen = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    if (ulen <= 0) return "";
    std::string utf8_str(ulen, '\0');
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &utf8_str[0], ulen, nullptr, nullptr);

    return utf8_str;
}

// Функция find_street_index: Возвращает индекс (номер) и название улицы по заданному имени
// edges: список рёбер с названиями улиц
// street_name_query: название улицы для поиска
// Возвращает кортеж (индекс, название_улицы), если найдено, иначе (-1, "")
pair<int, string> find_street_index(const vector<EdgeItem>& edges, const string& street_name_query) {
    string query_cp1251 = utf8_to_cp1251(street_name_query);
    for (size_t i = 0; i < edges.size(); i++) {
        Coord start, end;
        string name;
        std::tie(start, end, name) = edges[i];

        if (!name.empty()) {
            // Прямое и безопасное сравнение байтов строк без использования ломающего кириллицу tolower
            if (name == query_cp1251 || name == street_name_query) {
                return make_pair(static_cast<int>(i), name);
            }
        }
    }
    return make_pair(-1, "");
}

// Функция visualize_path_with_network: Визуализация всей дорожной сети + маршрута красным.
// Если передан список street_names, то названия улиц выводятся вдоль маршрута.
// Используется SFML для визуализации
void visualize_path_with_network(const map<string, Coord>& nodes,
    const vector<EdgeItem>& edges,
    const vector<Coord>& path,
    const vector<string>& street_names = vector<string>(),
    const sf::Vector2u& figsize = sf::Vector2u(800, 800)) {
    // Создаём окно с заданным размером
    sf::RenderWindow window(sf::VideoMode(figsize.x, figsize.y), "Кратчайший маршрут - София");

    // Определяем границы для масштабирования на основе всех координат
    double minX = std::numeric_limits<double>::max(), maxX = -std::numeric_limits<double>::max();
    double minY = std::numeric_limits<double>::max(), maxY = -std::numeric_limits<double>::max();
    // Обрабатываем узлы
    for (auto& node : nodes) {
        double x = node.second.first;
        double y = node.second.second;
        if (x < minX) minX = x;
        if (x > maxX) maxX = x;
        if (y < minY) minY = y;
        if (y > maxY) maxY = y;
    }
    // Функция для преобразования координат
    auto transform_coord = [=](const Coord& c) -> sf::Vector2f {
        double scaleX = (figsize.x - 40) / (maxX - minX + 1e-6);
        double scaleY = (figsize.y - 40) / (maxY - minY + 1e-6);
        float x = static_cast<float>((c.first - minX) * scaleX + 20);
        float y = static_cast<float>(figsize.y - ((c.second - minY) * scaleY + 20));
        return sf::Vector2f(x, y);
        };
    // Основной цикл окна
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::White);

        // Рисуем все рёбра — серыми
        for (const auto& edge : edges) {
            Coord start, end;
            string dummy;
            std::tie(start, end, dummy) = edge;
            sf::Vertex line[] = {
                sf::Vertex(transform_coord(start), sf::Color(200, 200, 200)),
                sf::Vertex(transform_coord(end), sf::Color(200, 200, 200))
            };
            window.draw(line, 2, sf::Lines);
        }

        // Рисуем путь — красным
        if (!path.empty() && path.size() > 1) {
            for (size_t i = 0; i < path.size() - 1; i++) {
                sf::Vertex line[] = {
                    sf::Vertex(transform_coord(path[i]), sf::Color::Red),
                    sf::Vertex(transform_coord(path[i + 1]), sf::Color::Red)
                };
                window.draw(line, 2, sf::Lines);
            }
        }
        window.display();
    }
}

int main() {
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);

    // 1. Загрузка данных
    map<string, Coord> nodes;
    vector<EdgeItem> edges;

    std::tie(nodes, edges) = read_graphml("sofia_road_network.graphml");

    string start_street_query = "Слатинска";
    string end_street_query = "Слатинска";

    // 2. Задаём названия улиц для начала и конца маршрута
    Coord start, end;
    string name;
    std::tie(start, end, name) = edges[edges.size() - 1];

    // 3. Используем find_street_index для определения нужных рёбер
    pair<int, string> start_result = find_street_index(edges, start_street_query);
    pair<int, string> end_result = find_street_index(edges, end_street_query);

    int start_index = start_result.first;
    int end_index = end_result.first;

    // 4. Определяем стартовый и конечный узлы:
    // Используем первую точку ребра для старта и вторую точку ребра для финиша.
    Coord start_node = std::get<0>(edges[start_index]);
    Coord end_node = std::get<1>(edges[end_index]);

    // 5. Строим граф и ищем кратчайший путь
    Graph graph = build_graph(edges);
    vector<Coord> path;
    double distance;
    vector<string> street_names;

    auto [result, time_ms] = measure_execution_time(dijkstra, graph, start_node, end_node);
    std::cout << "Dijkstra time: " << time_ms << " ms\n";
    std::tie(path, distance, street_names) = dijkstra(graph, start_node, end_node);

    if (path.empty()) {
        std::cout << "Путь не найден" << std::endl;
    }
    else {
        std::cout << "Найден путь длиной (lenght)" << distance << " км (km)" << std::endl;
        std::cout << "Улицы на пути: ";
        bool first = true;
        for (const auto& s : street_names) {
            if (!s.empty()) {
                if (!first)
                    std::cout << ", ";
                std::cout << s;
                first = false;
            }
        }
        std::cout << std::endl;

        // 6. Визуализация маршрута
        visualize_path_with_network(nodes, edges, path, street_names);
    }

    return 0;
}

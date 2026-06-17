#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <algorithm>
#include <random>

using namespace std;
using namespace sf;

const int GRID_SIZE = 10; // Размер поля NxN
const int CELL_SIZE = 80;  // Размер одной ячейки в пикселях
const int WINDOW_SIZE = GRID_SIZE * CELL_SIZE; // размер окна

// Типы ячеек
enum CellType {
    EMPTY, // пустота
    OBSTACLE, // ограждение
    START,// начало
    END,// конец
    PATH,// путь
    VISITED// посещённые
};

// структура ячейки
struct Cell {
    int x, y; // координаты
    CellType type; // тип ячейки
    int f, g, h; // Для алгоритма A*
    Cell* parent; // указатель на родителей

    Cell(int x, int y) : x(x), y(y), type(EMPTY), f(0), g(0), h(0), parent(nullptr) {}

    // для сравнения ячеек
    bool operator==(const Cell& other) const {
        return x == other.x && y == other.y;
    }
};



//функция для вычисления эвристики (манхэттенское расстояние)
int heuristic(const Cell& a, const Cell& b) {
    return abs(a.x - b.x) + abs(a.y - b.y);
}

// функция для проверки, находится ли ячейка в пределах сетки
bool isValid(int x, int y) {
    return (x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE);
}


//функция для создания сетки
std::vector<std::vector<Cell>> make_grid(int GRID_SIZE) {
    vector<vector<Cell>> grid;
    for (int x = 0; x < GRID_SIZE; x++) {
        std::vector<Cell> row;
        for (int y = 0; y < GRID_SIZE; y++) {
            Cell cell(x, y);
            row.push_back(cell);
        }
        grid.push_back(row);
    }
    return grid;
}

//функция для отрисовки сетки
void draw_grid(sf::RenderWindow& win, std::vector<std::vector<Cell>>& grid) {
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            RectangleShape cell(Vector2f(CELL_SIZE - 1, CELL_SIZE - 1));
            cell.setPosition(i * CELL_SIZE, j * CELL_SIZE);

            switch (grid[i][j].type) {
            case EMPTY: cell.setFillColor(Color::White); break;
            case OBSTACLE: cell.setFillColor(Color::Black); break;
            case START: cell.setFillColor(Color::Green); break;
            case END: cell.setFillColor(Color::Red); break;
            case PATH: cell.setFillColor(Color::Blue); break;
            case VISITED: cell.setFillColor(Color::Cyan); break;
            }

            win.draw(cell);
        }
    }
}

// сортирует ячейки так, чтобы первым всегда была ячейка с наименьшим значением f
struct CompareCells {
    bool operator()(Cell const* c1, Cell const* c2) {
        return c1->f > c2->f;
    }
};

// функция восстановления пути
void reconstruct_path(Cell* current) {
    if (current && current->type == END) {
        current = current->parent;
    }
    while (current != nullptr && current->type != START) {
        current->type = PATH;
        current = current->parent;
    }
}

//алгоритм A*
void a_star_algorithm(std::vector<std::vector<Cell>>& grid, Cell* start, Cell* end) {
    for (int x = 0; x < GRID_SIZE; x++) {
        for (int y = 0; y < GRID_SIZE; y++) {
            grid[x][y].parent = nullptr;
            grid[x][y].g = 999999;
            if (grid[x][y].type == PATH || grid[x][y].type == VISITED) {
                grid[x][y].type = EMPTY;
            }
        }
    }

    // очередь для хранения доступных ячеек
    std::priority_queue<Cell*, std::vector<Cell*>, CompareCells> open_list;

    // двумерный массив для посещенных и обработанных ячеек
    std::vector<std::vector<Cell*>> closed_list(GRID_SIZE, std::vector<Cell*>(GRID_SIZE, nullptr));

    // начальная клетка
    start->g = 0;
    start->f = heuristic(*start, *end);
    open_list.push(start);

    // основной цикл алгоритма
    while (!open_list.empty()) {
        // берётся ячейка с наименьшим f
        Cell* n = open_list.top();
        open_list.pop();

        // если конец, то строится путь
        if (*n == *end) {
            reconstruct_path(&grid[end->x][end->y]);
            return;
        }

        // если ячейка находится в закрытом списке — пропускаем её
        if (closed_list[n->x][n->y] != nullptr) continue;

        // добавляем ячейку в закрытый список
        closed_list[n->x][n->y] = n;

        // помечаем пустую ячейку как посещенную
        if (n->type == EMPTY) {
            n->type = VISITED;
        }

        // смещения для 4-х соседних ячеек 
        int dx[] = { 0, 0, 1, -1 };
        int dy[] = { 1, -1, 0, 0 };

        for (int i = 0; i < 4; i++) {
            int nx = n->x + dx[i];
            int ny = n->y + dy[i];

            // проверка 
            if (!isValid(nx, ny) || grid[nx][ny].type == OBSTACLE || closed_list[nx][ny] != nullptr) {
                continue;
            }

            // рассчитываем предполагаемую стоимость этого соседа
            Cell* m = &grid[nx][ny];
            int tentative_g = n->g + 1;

            // если найден более короткий путь
            if (tentative_g < m->g) {
                m->parent = n;
                m->g = tentative_g;
                m->h = heuristic(*m, *end);
                m->f = m->g + m->h; // обновляем стоимость

                // добавляем соседа в открытй список
                open_list.push(m);
            }
        }
    }
}

// генерация поля по заданию
void generate_grid(vector<vector<Cell>>& grid) {
    grid[0][3].type = START; //начало
    grid[GRID_SIZE - 1][GRID_SIZE - 1].type = END; // конец
    // координаты непроходимых клеток
    vector<Vector2i> obstacles = {
        {9,0},
        {1,1}, {5,1}, {6,1}, {8,1},
        {5,2}, {7,2},
        {5,3}, {6,3}, {7,3},
        {4,4},
        {5,5},
        {4, 7}, {8, 7},
        {2, 8},{4, 8}, {7, 8},
        {4,9}, {6,9}, {8,9}
    };

    // обозначение непроходимых клеток
    for (const auto& p : obstacles) {
        if (p.x < GRID_SIZE && p.y < GRID_SIZE) {
            grid[p.x][p.y].type = OBSTACLE;
        }
    }

}

int main() {
    // окно и поле по заданию
    RenderWindow window(VideoMode(WINDOW_SIZE, WINDOW_SIZE), "A*");
    vector<vector<Cell>> grid = make_grid(GRID_SIZE);
    generate_grid(grid);

    // клетки начала и конца
    Cell* start = nullptr;
    Cell* end = nullptr;

    for (auto& row : grid) {
        for (auto& cell : row) {
            if (cell.type == START) {
                start = &grid[cell.x][cell.y];
            }
            if (cell.type == END) {
                end = &grid[cell.x][cell.y];
            }
        }
    }

    while (window.isOpen()) {
        // обработка событий
        Event event;
        while (window.pollEvent(event)) {
            // закрытие окна
            if (event.type == Event::Closed) {
                window.close();
            }
            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Space) {
                    a_star_algorithm(grid, start, end);
                }
            }
        }

        window.clear(); // очистка экрана
        draw_grid(window, grid); // отрисовка всех ячеек
        window.display(); // отображение
    }
    return 0;
}
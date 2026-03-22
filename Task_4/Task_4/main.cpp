#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <sstream>
#include <locale>

using namespace std;

/**
 * @brief Структура point с целочисленными членами x, y и строковым членом s
 */
struct point {
    int x;
    int y;
    string s;

    /**
     * @brief Конструктор по умолчанию
     */
    point() : x(0), y(0), s("") {}

    /**
     * @brief Конструктор с параметрами
     */
    point(int x_val, int y_val, const string& s_val) : x(x_val), y(y_val), s(s_val) {}
};

/**
 * @brief Операция сложения для объектов point
 * @param a Первый объект point
 * @param b Второй объект point
 * @return Новый объект point с суммой соответствующих полей
 */
point operator+(const point& a, const point& b) {
    return point(a.x + b.x, a.y + b.y, a.s + b.s);
}

/**
 * @brief Операция чтения point из потока
 * @param is Входной поток
 * @param p Объект point для заполнения
 * @return Ссылка на входной поток
 */
istream& operator>>(istream& is, point& p) {
    is >> p.x >> p.y >> p.s;
    return is;
}

/**
 * @brief Операция записи point в поток
 * @param os Выходной поток
 * @param p Объект point для вывода
 * @return Ссылка на выходной поток
 */
ostream& operator<<(ostream& os, const point& p) {
    os << p.x << " " << p.y << " " << p.s;
    return os;
}

/**
 * @brief Пользовательский итератор ввода для чтения объектов point из потока
 */
class ptin_iterator {
    std::istream* in_stream;
    point value;
    bool end_marker;

public:
    using iterator_category = std::input_iterator_tag;
    using value_type = point;
    using difference_type = std::ptrdiff_t;
    using pointer = const point*;
    using reference = const point&;

    ptin_iterator() : in_stream(nullptr), end_marker(true) {}

    ptin_iterator(std::istream& s) : in_stream(&s), end_marker(false) {
        ++(*this);
    }

    reference operator*() const { return value; }
    pointer operator->() const { return &value; }

    ptin_iterator& operator++() {
        if (in_stream && !(*in_stream >> value)) {
            end_marker = true;
        }
        return *this;
    }

    ptin_iterator operator++(int) {
        ptin_iterator temp = *this;
        ++(*this);
        return temp;
    }

    bool operator==(const ptin_iterator& other) const {
        return (end_marker && other.end_marker) ||
            (end_marker == other.end_marker && in_stream == other.in_stream);
    }

    bool operator!=(const ptin_iterator& other) const {
        return !(*this == other);
    }
};

/**
 * @brief Пользовательский итератор вывода для записи объектов point в поток
 */
class ptout_iterator {
    std::ostream* out_stream;

public:
    using iterator_category = std::output_iterator_tag;
    using value_type = void;
    using difference_type = void;
    using pointer = void;
    using reference = void;

    ptout_iterator(std::ostream& s) : out_stream(&s) {}

    ptout_iterator& operator*() { return *this; }

    ptout_iterator& operator=(const point& value) {
        if (out_stream) {
            *out_stream << value << endl;
        }
        return *this;
    }

    ptout_iterator& operator++() { return *this; }
    ptout_iterator& operator++(int) { return *this; }
};

/**
 * @brief Выводит содержимое вектора точек
 */
void printVector(const vector<point>& v, const string& message) {
    cout << message << endl;
    for (size_t i = 0; i < v.size(); i++) {
        cout << "  " << v[i] << endl;
    }
}

/**
 * @brief Главная функция программы
 * Читает точки из двух файлов, складывает соответствующие точки
 * и записывает результат в первый файл
 */
int main() {
    setlocale(LC_ALL, "Russian");

    string name1, name2;
    cout << "Введите имя первого файла: ";
    cin >> name1;
    cout << "Введите имя второго файла: ";
    cin >> name2;

    ifstream file1(name1);
    if (!file1.is_open()) {
        cerr << "Ошибка: Не удалось открыть файл " << name1 << endl;
        return 1;
    }

    ptin_iterator it1_begin(file1);
    ptin_iterator it1_end;

    vector<point> V1;
    copy(it1_begin, it1_end, back_inserter(V1));
    file1.close();

    ifstream file2(name2);
    if (!file2.is_open()) {
        cerr << "Ошибка: Не удалось открыть файл " << name2 << endl;
        return 1;
    }

    ptin_iterator it2_begin(file2);
    ptin_iterator it2_end;

    vector<point> V2;
    copy(it2_begin, it2_end, back_inserter(V2));
    file2.close();

    if (V1.empty() || V2.empty()) {
        cerr << "Ошибка: Один из файлов пуст" << endl;
        return 1;
    }

    if (V1.size() != V2.size()) {
        cerr << "Ошибка: Файлы содержат разное количество элементов" << endl;
        cerr << "V1 size: " << V1.size() << ", V2 size: " << V2.size() << endl;
        return 1;
    }

    printVector(V1, "Вектор V1 (из файла " + name1 + "):");
    cout << endl;
    printVector(V2, "Вектор V2 (из файла " + name2 + "):");
    cout << endl;

    transform(V1.begin(), V1.end(), V2.begin(), V1.begin(), plus<point>());

    printVector(V1, "Преобразованный вектор V1 (V1 = V1 + V2):");
    cout << endl;

    ofstream outFile(name1);
    if (!outFile.is_open()) {
        cerr << "Ошибка: Не удалось открыть файл " << name1 << " для записи" << endl;
        return 1;
    }
    copy(V1.begin(), V1.end(), ptout_iterator(outFile));
    outFile.close();


    return 0;
}
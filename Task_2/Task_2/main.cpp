#include <iostream>
#include <deque>
#include <list>
#include <algorithm>
#include <iterator>
#include <fstream>
#include <string>
#include <locale>

using namespace std;

/**
 * @brief Пользовательский итератор ввода для чтения целых чисел из потока
 */
class ptin_iterator {
    std::istream* in_stream;
    int value;
    bool end_marker;

public:
    using iterator_category = std::input_iterator_tag;
    using value_type = int;
    using difference_type = std::ptrdiff_t;
    using pointer = const int*;
    using reference = const int&;

    ptin_iterator() : in_stream(nullptr), value(0), end_marker(true) {}

    ptin_iterator(std::istream& s) : in_stream(&s), value(0), end_marker(false) {
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
 * @brief Пользовательский итератор вывода для записи целых чисел в поток
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

    ptout_iterator& operator=(int value) {
        if (out_stream) {
            *out_stream << value << " ";
        }
        return *this;
    }

    ptout_iterator& operator++() { return *this; }
    ptout_iterator& operator++(int) { return *this; }
};

/**
 * @brief Вспомогательная функция для вывода контейнера
 */
template<typename Container>
void printContainer(const string& name, const Container& container) {
    cout << name << ": ";
    copy(container.begin(), container.end(), ptout_iterator(cout));
    cout << endl;
}

/**
 * @brief Главная функция программы
 * Вставляет перед пятым с конца элементом списка последние 5 элементов дека в обратном порядке
 */
int main() {
    setlocale(LC_ALL, "Russian");

    string dequeFile, listFile;
    cout << "Введите имя файла с данными для дека: ";
    cin >> dequeFile;
    cout << "Введите имя файла с данными для списка: ";
    cin >> listFile;

    ifstream dFile(dequeFile);
    if (!dFile.is_open()) {
        cerr << "Ошибка: Не удалось открыть файл " << dequeFile << endl;
        return 1;
    }

    ptin_iterator d_begin(dFile);
    ptin_iterator d_end;

    deque<int> D;
    copy(d_begin, d_end, back_inserter(D));
    dFile.close();

    ifstream lFile(listFile);
    if (!lFile.is_open()) {
        cerr << "Ошибка: Не удалось открыть файл " << listFile << endl;
        return 1;
    }

    ptin_iterator l_begin(lFile);
    ptin_iterator l_end;

    list<int> L;
    copy(l_begin, l_end, back_inserter(L));
    lFile.close();

    if (D.size() < 5 || L.size() < 5) {
        cerr << "Ошибка: Контейнеры должны содержать не менее 5 элементов" << endl;
        return 1;
    }

    printContainer("Дек D", D);
    printContainer("Список L", L);

    auto it = L.end();
    advance(it, -5);
    L.insert(it, D.rbegin(), D.rbegin() + 5);

    printContainer("Список L после вставки", L);

    return 0;
}
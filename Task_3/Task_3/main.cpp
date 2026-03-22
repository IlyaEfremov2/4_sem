#include <iostream>
#include <vector>
#include <set>
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
 * @brief Главная функция программы
 * Определяет количество повторений каждого числа в векторе
 * с использованием вспомогательного мультимножества
 */
int main() {
    setlocale(LC_ALL, "Russian");

    string filename;
    cout << "Введите имя файла с вектором V (например, vector.txt): ";
    cin >> filename;

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Ошибка: Не удалось открыть файл " << filename << endl;
        return 1;
    }

    ptin_iterator it_begin(file);
    ptin_iterator it_end;

    vector<int> V;
    copy(it_begin, it_end, back_inserter(V));
    file.close();

    if (V.empty()) {
        cerr << "Ошибка: Вектор пуст" << endl;
        return 1;
    }

    cout << "\nИсходный вектор V: ";
    copy(V.begin(), V.end(), ptout_iterator(cout));
    cout << endl;

    multiset<int> M;
    for (const auto& val : V) {
        M.insert(val);
    }

    cout << "\nВспомогательное мультимножество: ";
    copy(M.begin(), M.end(), ptout_iterator(cout));
    cout << endl << endl;

    auto itM = M.begin();
    while (itM != M.end()) {
        int value = *itM;

        auto upper = M.upper_bound(value);

        int count = distance(itM, upper);
        cout << "Число " << value << " встречается " << count << " раз(а)" << endl;
        itM = upper;
    }
    return 0;
}
#include <iostream>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <string>
#include <locale>
#include <cctype>

using namespace std;

/**
 * @brief Предикат для проверки, является ли символ цифрой
 */
bool is_digit(char c) {
    return isdigit(static_cast<unsigned char>(c));
}

/**
 * @brief Пользовательский итератор ввода для чтения символов из потока
 */
class ptin_iterator {
    std::istream* in_stream;
    char value;
    bool end_marker;

public:
    using iterator_category = std::input_iterator_tag;
    using value_type = char;
    using difference_type = std::ptrdiff_t;
    using pointer = const char*;
    using reference = const char&;

    ptin_iterator() : in_stream(nullptr), value('\0'), end_marker(true) {}

    ptin_iterator(std::istream& s) : in_stream(&s), value('\0'), end_marker(false) {
        ++(*this);
    }

    reference operator*() const { return value; }
    pointer operator->() const { return &value; }

    ptin_iterator& operator++() {
        if (in_stream && in_stream->get(value)) {
        }
        else {
            end_marker = true;
            value = '\0';
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
 * @brief Пользовательский итератор вывода для записи символов в поток
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

    ptout_iterator& operator=(char value) {
        if (out_stream) {
            *out_stream << value;
        }
        return *this;
    }

    ptout_iterator& operator++() { return *this; }
    ptout_iterator& operator++(int) { return *this; }
};

int main() {
    setlocale(LC_ALL, "Russian");

    string filename;

    cout << "Имя файла: ";
    cin >> filename;

    ifstream input_file(filename);

    if (!input_file.is_open()) {
        cerr << "Ошибка: Не удалось открыть файл " << filename << endl;
        return 1;
    }

    cout << "\nИсходное содержимое файла:" << endl;

    string line;
    while (getline(input_file, line)) {
        cout << line << endl;
    }

    input_file.clear();
    input_file.seekg(0);

    cout << "\nРезультат после замены цифр на подчеркивание:" << endl;

    ptin_iterator it_begin(input_file);
    ptin_iterator it_end;

    replace_copy_if(it_begin, it_end,
        ptout_iterator(cout),is_digit,'_');

    input_file.close();

    return 0;
}
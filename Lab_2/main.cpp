#include <iostream>
#include <clocale>
#include <cstdlib>
#include <ctime>

using namespace std;

const int GENOME_LETTERS_COUNT = 4;

/*
 * Выделяет память под квадратную матрицу N×N.
 *
 * @param p_matrix ссылка на указатель на указатель для матрицы.
 * @param n размер матрицы.
 */
void allocateMatrix(char**& p_matrix, int n) {
    p_matrix = new char* [n];
    for (int i = 0; i < n; i++) {
        p_matrix[i] = new char[n];
    }
}

/*
 * Освобождает память, выделенную под матрицу.
 *
 * @param p_matrix ссылка на указатель на указатель для матрицы.
 * @param n размер матрицы.
 */
void freeMatrix(char**& p_matrix, int n) {
    for (int i = 0; i < n; i++) {
        delete[] p_matrix[i];
    }
    delete[] p_matrix;
    p_matrix = nullptr;
}

/*
 * Заполняет матрицу случайными буквами A, T, G, C.
 *
 * @param p_matrix указатель на матрицу.
 * @param n размер матрицы.
 */
void fillMatrixRandom(char** p_matrix, int n) {
    const char genome_letters[GENOME_LETTERS_COUNT] = { 'A', 'T', 'G', 'C' };
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            p_matrix[i][j] = genome_letters[rand() % GENOME_LETTERS_COUNT];
        }
    }
}

/*
 * Выводит матрицу в терминал.
 *
 * @param p_matrix указатель на константную матрицу.
 * @param n размер матрицы.
 */
void printMatrix(const char* const* p_matrix, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cout << p_matrix[i][j];
            if (j < n - 1) {
                cout << " ";
            }
        }
        cout << endl;
    }
}

/*
 * Подсчитывает количество вхождений каждой буквы A, T, G, C.
 *
 * @param p_matrix указатель на константную матрицу.
 * @param n размер матрицы.
 */
void countLetters(const char* const* p_matrix, int n) {
    int count_a = 0;
    int count_t = 0;
    int count_g = 0;
    int count_c = 0;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (p_matrix[i][j] == 'A') {
                count_a++;
            }
            else if (p_matrix[i][j] == 'T') {
                count_t++;
            }
            else if (p_matrix[i][j] == 'G') {
                count_g++;
            }
            else if (p_matrix[i][j] == 'C') {
                count_c++;
            }
        }
    }

    cout << "A: " << count_a << endl;
    cout << "T: " << count_t << endl;
    cout << "G: " << count_g << endl;
    cout << "C: " << count_c << endl;
}

/*
 * Находит самую длинную последовательность одинаковых символов
 * в строках и столбцах матрицы.
 *
 * @param p_matrix указатель на константную матрицу.
 * @param n размер матрицы.
 */
void findLongestSequence(const char* const* p_matrix, int n) {
    int max_length = 0;
    char max_char = ' ';
    bool is_row = true;
    int max_line_index = 0;
    int max_start_index = 0;

    for (int i = 0; i < n; i++) {
        int current_length = 1;
        for (int j = 1; j < n; j++) {
            if (p_matrix[i][j] == p_matrix[i][j - 1]) {
                current_length++;
            }
            else {
                if (current_length > max_length) {
                    max_length = current_length;
                    max_char = p_matrix[i][j - 1];
                    is_row = true;
                    max_line_index = i;
                    max_start_index = j - current_length;
                }
                current_length = 1;
            }
        }
        if (current_length > max_length) {
            max_length = current_length;
            max_char = p_matrix[i][n - 1];
            is_row = true;
            max_line_index = i;
            max_start_index = n - current_length;
        }
    }

    for (int j = 0; j < n; j++) {
        int current_length = 1;
        for (int i = 1; i < n; i++) {
            if (p_matrix[i][j] == p_matrix[i - 1][j]) {
                current_length++;
            }
            else {
                if (current_length > max_length) {
                    max_length = current_length;
                    max_char = p_matrix[i - 1][j];
                    is_row = false;
                    max_line_index = j;
                    max_start_index = i - current_length;
                }
                current_length = 1;
            }
        }
        if (current_length > max_length) {
            max_length = current_length;
            max_char = p_matrix[n - 1][j];
            is_row = false;
            max_line_index = j;
            max_start_index = n - current_length;
        }
    }

    cout << "Самая длинная последовательность: " << max_length << endl;
    cout << "Символ: " << max_char << endl;
    if (is_row) {
        cout << "Направление: строка " << max_line_index + 1 << endl;
    }
    else {
        cout << "Направление: столбец " << max_line_index + 1 << endl;
    }
    cout << "Начальный индекс: " << max_start_index + 1 << endl;
}

/*
 * Строит обратную комплементарную матрицу (A↔T, G↔C).
 *
 * @param p_matrix указатель на константную исходную матрицу.
 * @param p_complement указатель на матрицу для результата.
 * @param n размер матрицы.
 */
void buildComplementMatrix(const char* const* p_matrix, char** p_complement, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (p_matrix[i][j] == 'A') {
                p_complement[i][j] = 'T';
            }
            else if (p_matrix[i][j] == 'T') {
                p_complement[i][j] = 'A';
            }
            else if (p_matrix[i][j] == 'G') {
                p_complement[i][j] = 'C';
            }
            else if (p_matrix[i][j] == 'C') {
                p_complement[i][j] = 'G';
            }
        }
    }
}

int main() {
    setlocale(LC_ALL, "Russian");
    srand(static_cast<unsigned int>(time(nullptr)));

    int matrix_size = 0;

    cout << "Введите размер матрицы N: ";
    cin >> matrix_size;

    char** p_genome_matrix = nullptr;

    allocateMatrix(p_genome_matrix, matrix_size);
    fillMatrixRandom(p_genome_matrix, matrix_size);

    cout << "Исходная матрица:" << endl;
    printMatrix(p_genome_matrix, matrix_size);

    int operation_choice = 0;

    while (operation_choice != 4) {
        cout << "\nВыберите операцию:" << endl;
        cout << "1. Подсчитать количество вхождений каждой буквы" << endl;
        cout << "2. Найти самую длинную последовательность одинаковых символов" << endl;
        cout << "3. Построить обратную комплементарную матрицу" << endl;
        cout << "4. Выход" << endl;
        cout << "Ваш выбор: ";
        cin >> operation_choice;

        if (operation_choice == 1) {
            cout << "\nКоличество вхождений:" << endl;
            countLetters(p_genome_matrix, matrix_size);
        }
        else if (operation_choice == 2) {
            cout << "\nРезультат поиска:" << endl;
            findLongestSequence(p_genome_matrix, matrix_size);
        }
        else if (operation_choice == 3) {
            char** p_complement_matrix = nullptr;
            allocateMatrix(p_complement_matrix, matrix_size);
            buildComplementMatrix(p_genome_matrix, p_complement_matrix, matrix_size);

            cout << "\nКомплементарная матрица:" << endl;
            printMatrix(p_complement_matrix, matrix_size);

            freeMatrix(p_complement_matrix, matrix_size);
        }
        else if (operation_choice == 4) {
            cout << "Выход из программы." << endl;
        }
        else {
            cout << "Неверный выбор операции! Попробуйте снова." << endl;
        }
    }

    freeMatrix(p_genome_matrix, matrix_size);

    return 0;
}
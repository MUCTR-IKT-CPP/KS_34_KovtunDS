#include <iostream>
#include <clocale>
#include <cstdlib>
#include <ctime>

using namespace std;

const int MAX_ARRAY_SIZE = 200;
const int MIN_GRADE = 1;
const int MAX_GRADE = 5;

/*
 * Заполняет массив случайными оценками от MIN_GRADE до MAX_GRADE.
 *
 * @param p_arr указатель на массив для заполнения.
 * @param n размер массива.
 */
void generateGradesArray(int* p_arr, int n) {
    for (int i = 0; i < n; i++) {
        p_arr[i] = rand() % (MAX_GRADE - MIN_GRADE + 1) + MIN_GRADE;
    }
}

/*
 * Выводит массив в формате [a,b,c].
 *
 * @param p_arr указатель на константный массив.
 * @param n размер массива.
 */
void printArray(const int* p_arr, int n) {
    cout << "[";
    for (int i = 0; i < n; i++) {
        cout << p_arr[i];
        if (i < n - 1) {
            cout << ",";
        }
    }
    cout << "]";
}

/*
 * Сортирует массив по возрастанию методом пузырька.
 * На каждом проходе сравнивает соседние элементы и меняет их местами,
 * если левый больше правого.
 *
 * @param p_arr указатель на массив для сортировки.
 * @param n размер массива.
 */
void bubbleSort(int* p_arr, int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (p_arr[j] > p_arr[j + 1]) {
                int temp_value = p_arr[j];
                p_arr[j] = p_arr[j + 1];
                p_arr[j + 1] = temp_value;
            }
        }
    }
}

/*
 * Вычисляет средний балл по массиву оценок.
 *
 * @param p_arr указатель на константный массив оценок.
 * @param n размер массива.
 * @return возвращает средний балл в виде числа с плавающей точкой.
 */
double calculateAverage(const int* p_arr, int n) {
    double total_sum = 0;
    for (int i = 0; i < n; i++) {
        total_sum += p_arr[i];
    }
    return total_sum / n;
}

int main() {
    setlocale(LC_ALL, "Russian");
    srand((time(0)));

    int array_size = 0;

    cout << "Введите количество оценок N: ";
    cin >> array_size;

    int grades_array[MAX_ARRAY_SIZE] = { 0 };

    generateGradesArray(grades_array, array_size);

    cout << "Ввод: ";
    printArray(grades_array, array_size);
    cout << endl;

    bubbleSort(grades_array, array_size);

    cout << "Вывод: ";
    printArray(grades_array, array_size);
    cout << endl;

    double average_grade = calculateAverage(grades_array, array_size);
    cout << "Средний балл: " << average_grade << endl;

    return 0;
}
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string> 
#include <clocale>

using namespace std;

enum Device_Type
{
    Light,
    Thermostat,
    Camera,
    Speaker,
    Sensor
};

struct SmartDevice
{
    int device_id;
    string name;
    Device_Type type;
    bool is_online;
    unsigned int last_active;
};

/*
 * Заполняет массив устройств случайными данными.
 *
 * @param p_devices указатель на массив устройств.
 * @param n количество устройств.
 */
void generateDevices(SmartDevice* p_devices, int n)
{
    for (int i = 0; i < n; i++)
    {
        p_devices[i].device_id = rand() % 100001 + 10000;
        p_devices[i].type = static_cast<Device_Type>(rand() % 5);
        p_devices[i].is_online = rand() % 2;

        int num = rand() % 99 + 1;
        string num_str = (num < 10 ? "0" : "") + to_string(num);
        p_devices[i].name = string("dev_") + num_str;

        if (p_devices[i].is_online) {
            p_devices[i].last_active = 0;
        }
        else {
            p_devices[i].last_active = rand() % 1001;
        }
    }
}

/*
 * Возвращает строковое название типа устройства.
 *
 * @param type значение перечисления Device_Type.
 * @return строка с названием типа.
 */
string getDeviceTypeName(Device_Type type) {
    const string names[] = { "Light", "Thermostat", "Camera", "Speaker", "Sensor" };
    return names[type];
}

/*
 * Возвращает строковое название типа устройства по целочисленному значению.
 *
 * @param type целочисленное значение типа устройства.
 * @return строка с названием типа.
 */
string getDeviceTypeName(int type) {
    const string names[] = { "Light", "Thermostat", "Camera", "Speaker", "Sensor" };
    return names[type];
}

/*
 * Выводит список всех устройств, которые находятся offline.
 *
 * @param p_devices указатель на константный массив устройств.
 * @param n количество устройств.
 */
void printOfflineDevices(const SmartDevice* p_devices, int n)
{
    int cnt = 0;
    for (int i = 0; i < n; i++)
    {
        if (!p_devices[i].is_online)
        {
            cnt++;
            cout << "\nID: " << p_devices[i].device_id
                << "\nИмя: " << p_devices[i].name
                << "\nТип: " << getDeviceTypeName(p_devices[i].type)
                << "\nСтатус: оффлайн"
                << "\nПоследняя активность: " << p_devices[i].last_active << " мин. назад\n";
        }
    }
    if (cnt == 0)
    {
        cout << "\nОффлайн устройств не найдено.\n";
    }
}

/*
 * Выводит информацию обо всех устройствах в массиве.
 *
 * @param p_devices указатель на константный массив устройств.
 * @param n количество устройств.
 */
void printDevices(const SmartDevice* p_devices, int n)
{
    for (int i = 0; i < n; i++)
    {
        cout << "\nID: " << p_devices[i].device_id;
        cout << "\nИмя: " << p_devices[i].name;
        cout << "\nТип: " << getDeviceTypeName(p_devices[i].type);
        cout << "\nСтатус: " << (p_devices[i].is_online ? "онлайн" : "оффлайн");
        cout << "\nПоследняя активность: " << p_devices[i].last_active << " мин. назад\n";
    }
}

/*
 * Находит устройства заданного типа, сортирует их по времени
 * последней активности (от старых к свежим) и выводит результат.
 *
 * @param p_devices указатель на константный исходный массив.
 * @param n количество устройств в исходном массиве.
 * @param target_type искомый тип устройства (целое число).
 */
void searchByType(const SmartDevice* p_devices, int n, int target_type) {
    int found_count = 0;
    for (int i = 0; i < n; i++) {
        if (p_devices[i].type == target_type) {
            found_count++;
        }
    }

    if (found_count == 0) {
        cout << "\nУстройства типа " << getDeviceTypeName(target_type) << " не найдены.\n";
        return;
    }

    SmartDevice* p_filtered = new SmartDevice[found_count];

    int current_index = 0;
    for (int i = 0; i < n; i++) {
        if (p_devices[i].type == target_type) {
            p_filtered[current_index] = p_devices[i];
            current_index++;
        }
    }

    // Сортировка пузырьком по last_active (по убыванию)
    for (int i = 0; i < found_count - 1; i++) {
        for (int j = 0; j < found_count - i - 1; j++) {
            if (p_filtered[j].last_active < p_filtered[j + 1].last_active) {
                SmartDevice temp = p_filtered[j];
                p_filtered[j] = p_filtered[j + 1];
                p_filtered[j + 1] = temp;
            }
        }
    }

    cout << "\n--- Устройства типа " << getDeviceTypeName(target_type) << endl;
    cout << "(отсортированы по last_active) ---\n";

    for (int i = 0; i < found_count; i++)
    {
        cout << "\nID: " << p_filtered[i].device_id;
        cout << "\nИмя: " << p_filtered[i].name;
        cout << "\nТип: " << getDeviceTypeName(p_filtered[i].type);
        cout << "\nСтатус: " << (p_filtered[i].is_online ? "онлайн" : "оффлайн");
        cout << "\nПоследняя активность: " << p_filtered[i].last_active << " мин. назад\n";
    }

    delete[] p_filtered;
    p_filtered = nullptr;
}

/*
 * Выводит статистику системы: общее количество, онлайн/оффлайн
 * и количество устройств каждого типа.
 *
 * @param p_devices указатель на константный массив устройств.
 * @param n количество устройств.
 */
void printStatistics(const SmartDevice* p_devices, int n)
{
    int l = 0, t = 0, c = 0, sp = 0, se = 0, offline_count = 0;
    for (int i = 0; i < n; i++)
    {
        if (!p_devices[i].is_online)
            offline_count++;

        if (p_devices[i].type == Light) l++;
        else if (p_devices[i].type == Thermostat) t++;
        else if (p_devices[i].type == Camera) c++;
        else if (p_devices[i].type == Speaker) sp++;
        else if (p_devices[i].type == Sensor) se++;
    }

    cout << "\nКоличество устройств: " << n;
    cout << "\nКоличество онлайн-устройств: " << (n - offline_count);
    cout << "\nКоличество оффлайн-устройств: " << offline_count;
    cout << "\nКоличество устройств типа Light = " << l;
    cout << "\nКоличество устройств типа Thermostat = " << t;
    cout << "\nКоличество устройств типа Camera = " << c;
    cout << "\nКоличество устройств типа Speaker = " << sp;
    cout << "\nКоличество устройств типа Sensor = " << se << endl;
}

/*
 * Имитирует перезагрузку offline-устройств:
 * меняет статус на онлайн и сбрасывает last_active.
 *
 * @param p_devices указатель на массив устройств.
 * @param n количество устройств.
 */
void rebootOfflineDevices(SmartDevice* p_devices, int n)
{
    for (int i = 0; i < n; i++)
    {
        if (!p_devices[i].is_online)
        {
            p_devices[i].is_online = true;
            p_devices[i].last_active = 0;
        }
    }
}

/*
 * Сортирует массив устройств: сначала по типу (в порядке enum),
 * а внутри одного типа — по имени устройства (по алфавиту).
 *
 * @param p_devices указатель на массив устройств.
 * @param n количество устройств.
 */
void sortDevices(SmartDevice* p_devices, int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            bool need_swap = false;

            if (p_devices[j].type > p_devices[j + 1].type) {
                need_swap = true;
            }
            else if (p_devices[j].type == p_devices[j + 1].type) {
                if (p_devices[j].name > p_devices[j + 1].name) {
                    need_swap = true;
                }
            }

            if (need_swap) {
                SmartDevice temp = p_devices[j];
                p_devices[j] = p_devices[j + 1];
                p_devices[j + 1] = temp;
            }
        }
    }
}

int main()
{
    setlocale(LC_ALL, "Russian");
    srand((unsigned int)time(nullptr));

    int N;
    cout << "Введите количество устройств: ";
    cin >> N;

    SmartDevice* p_devices = new SmartDevice[N];
    generateDevices(p_devices, N);

    int operation_choice = 0;

    while (operation_choice != 6) {
        cout << "\nВыберите операцию:" << endl;
        cout << "1. Проверка состояния (показать offline-устройства)" << endl;
        cout << "2. Поиск по типу" << endl;
        cout << "3. Статистика системы" << endl;
        cout << "4. Отсортировать все устройства" << endl;
        cout << "5. Перезагрузить все offline-устройства" << endl;
        cout << "6. Выход" << endl;
        cout << "Ваш выбор: ";
        cin >> operation_choice;

        switch (operation_choice) {
        case 1:
            printOfflineDevices(p_devices, N);
            break;

        case 2: {
            int type_input = 0;
            cout << "\nДоступные типы: 0-Light, 1-Thermostat, 2-Camera, 3-Speaker, 4-Sensor" << endl;
            cout << "Введите номер типа: ";
            cin >> type_input;

            if (type_input >= 0 && type_input <= 4) {
                searchByType(p_devices, N, type_input);
            }
            else {
                cout << "Неверный тип устройства!" << endl;
            }
            break;
        }

        case 3:
            printStatistics(p_devices, N);
            break;

        case 4:
            sortDevices(p_devices, N);
            cout << "\nМассив отсортирован. Результат:" << endl;
            printDevices(p_devices, N);
            break;

        case 5:
            rebootOfflineDevices(p_devices, N);
            cout << "\nУстройства перезагружены. Результат:" << endl;
            printDevices(p_devices, N);
            break;

        case 6:
            cout << "Выход из программы." << endl;
            break;

        default:
            cout << "Неверный выбор операции! Попробуйте снова." << endl;
            break;
        }
    }

    delete[] p_devices;
    p_devices = nullptr;

    return 0;
}
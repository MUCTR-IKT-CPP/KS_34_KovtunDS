#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <clocale>

using namespace std;

// Структуры данных
struct CSVData
{
    int rows;
    int columns;
};

struct JSONData
{
    int objects_count;
    int arrays_count;
};

struct XMLData
{
    int tags_count;
    int attributes_count;
};

// Перечисление типов источников данных
enum DataSourceType
{
    CSV,
    JSON,
    XML
};

/*
 * Возвращает строковое название типа источника данных.
 *
 * @param type значение перечисления DataSourceType.
 * @return строка с названием типа.
 */
string getDataSourceTypeName(DataSourceType type)
{
    const string names[] = { "CSV", "JSON", "XML" };
    return names[type];
}

// Класс DataSource — элемент коллекции
class DataSource
{
private:
    void* p_source_data;
    DataSourceType type;
    string name;

    void allocateData()
    {
        if (p_source_data != nullptr)
        {
            deallocateData();
        }

        switch (type)
        {
        case CSV:
        {
            CSVData* p_csv = new CSVData;
            p_csv->rows = rand() % 1000 + 1;
            p_csv->columns = rand() % 20 + 1;
            p_source_data = p_csv;
            break;
        }
        case JSON:
        {
            JSONData* p_json = new JSONData;
            p_json->objects_count = rand() % 500 + 1;
            p_json->arrays_count = rand() % 100 + 1;
            p_source_data = p_json;
            break;
        }
        case XML:
        {
            XMLData* p_xml = new XMLData;
            p_xml->tags_count = rand() % 2000 + 1;
            p_xml->attributes_count = rand() % 500 + 1;
            p_source_data = p_xml;
            break;
        }
        }
    }

    void deallocateData()
    {
        if (p_source_data == nullptr) return;

        switch (type)
        {
        case CSV:
            delete static_cast<CSVData*>(p_source_data);
            break;
        case JSON:
            delete static_cast<JSONData*>(p_source_data);
            break;
        case XML:
            delete static_cast<XMLData*>(p_source_data);
            break;
        }
        p_source_data = nullptr;
    }

    void copyDataFrom(const DataSource& other)
    {
        if (p_source_data == nullptr || other.p_source_data == nullptr) return;

        switch (type)
        {
        case CSV:
        {
            CSVData* p_src = static_cast<CSVData*>(other.p_source_data);
            CSVData* p_dst = static_cast<CSVData*>(p_source_data);
            p_dst->rows = p_src->rows;
            p_dst->columns = p_src->columns;
            break;
        }
        case JSON:
        {
            JSONData* p_src = static_cast<JSONData*>(other.p_source_data);
            JSONData* p_dst = static_cast<JSONData*>(p_source_data);
            p_dst->objects_count = p_src->objects_count;
            p_dst->arrays_count = p_src->arrays_count;
            break;
        }
        case XML:
        {
            XMLData* p_src = static_cast<XMLData*>(other.p_source_data);
            XMLData* p_dst = static_cast<XMLData*>(p_source_data);
            p_dst->tags_count = p_src->tags_count;
            p_dst->attributes_count = p_src->attributes_count;
            break;
        }
        }
    }

public:
    DataSource() : p_source_data(nullptr), type(CSV), name("empty")
    {
        cout << "[DataSource] Default constructor\n";
    }

    DataSource(DataSourceType node_type, string node_name)
        : p_source_data(nullptr), type(node_type), name(node_name)
    {
        cout << "[DataSource] Parameterized constructor: " << name << "\n";
        allocateData();
    }

    DataSource(const DataSource& other)
        : p_source_data(nullptr), type(other.type), name(other.name)
    {
        cout << "[DataSource] Copy constructor: " << name << "\n";
        if (other.p_source_data != nullptr)
        {
            allocateData();
            copyDataFrom(other);
        }
    }

    DataSource& operator=(const DataSource& other)
    {
        cout << "[DataSource] Copy assignment: " << name << " <- " << other.name << "\n";
        if (this == &other) return *this;

        deallocateData();
        type = other.type;
        name = other.name;

        if (other.p_source_data != nullptr)
        {
            allocateData();
            copyDataFrom(other);
        }

        return *this;
    }

    DataSource(DataSource&& other)
        : p_source_data(other.p_source_data), type(other.type), name(other.name)
    {
        cout << "[DataSource] Move constructor: " << name << "\n";
        other.p_source_data = nullptr;
        other.type = CSV;
        other.name = "moved";
    }

    DataSource& operator=(DataSource&& other)
    {
        cout << "[DataSource] Move assignment: " << name << " <- " << other.name << "\n";
        if (this == &other) return *this;

        deallocateData();
        p_source_data = other.p_source_data;
        type = other.type;
        name = other.name;

        other.p_source_data = nullptr;
        other.type = CSV;
        other.name = "moved";

        return *this;
    }

    ~DataSource()
    {
        cout << "[DataSource] Destructor: " << name << "\n";
        deallocateData();
    }

    DataSourceType getType() const { return type; }
    string getName() const { return name; }

    int getElementCount() const
    {
        if (p_source_data == nullptr) return 0;

        switch (type)
        {
        case CSV:
        {
            CSVData* p_csv = static_cast<CSVData*>(p_source_data);
            return p_csv->rows * p_csv->columns;
        }
        case JSON:
        {
            JSONData* p_json = static_cast<JSONData*>(p_source_data);
            return p_json->objects_count + p_json->arrays_count;
        }
        case XML:
        {
            XMLData* p_xml = static_cast<XMLData*>(p_source_data);
            return p_xml->tags_count + p_xml->attributes_count;
        }
        default:
            return 0;
        }
    }

    void print() const
    {
        cout << "  Имя: " << name << "\n";
        cout << "  Тип: " << getDataSourceTypeName(type) << "\n";

        if (p_source_data == nullptr)
        {
            cout << "  Данные: отсутствуют\n";
            return;
        }

        switch (type)
        {
        case CSV:
        {
            CSVData* p_csv = static_cast<CSVData*>(p_source_data);
            cout << "  Строки: " << p_csv->rows
                << ", Столбцы: " << p_csv->columns << "\n";
            cout << "  Всего элементов: "
                << p_csv->rows * p_csv->columns << "\n";
            break;
        }
        case JSON:
        {
            JSONData* p_json = static_cast<JSONData*>(p_source_data);
            cout << "  Объекты: " << p_json->objects_count
                << ", Массивы: " << p_json->arrays_count << "\n";
            cout << "  Всего элементов: "
                << p_json->objects_count + p_json->arrays_count << "\n";
            break;
        }
        case XML:
        {
            XMLData* p_xml = static_cast<XMLData*>(p_source_data);
            cout << "  Теги: " << p_xml->tags_count
                << ", Атрибуты: " << p_xml->attributes_count << "\n";
            cout << "  Всего элементов: "
                << p_xml->tags_count + p_xml->attributes_count << "\n";
            break;
        }
        }
    }
};

// Класс DataAnalyzer — менеджер коллекции (правило пяти)
class DataAnalyzer
{
private:
    DataSource* p_nodes;
    int count;
    int capacity;

    void grow()
    {
        int new_capacity = capacity * 2;
        DataSource* p_new_nodes = new DataSource[new_capacity];

        for (int i = 0; i < count; i++)
        {
            p_new_nodes[i] = p_nodes[i];
        }

        delete[] p_nodes;
        p_nodes = p_new_nodes;
        capacity = new_capacity;
    }

public:
    DataAnalyzer() : p_nodes(nullptr), count(0), capacity(0)
    {
        cout << "[DataAnalyzer] Default constructor\n";
    }

    DataAnalyzer(int initial_capacity)
        : p_nodes(new DataSource[initial_capacity]),
        count(0), capacity(initial_capacity)
    {
        cout << "[DataAnalyzer] Parameterized constructor (capacity="
            << initial_capacity << ")\n";
    }

    DataAnalyzer(const DataAnalyzer& other)
        : p_nodes(new DataSource[other.capacity]),
        count(other.count), capacity(other.capacity)
    {
        cout << "[DataAnalyzer] Copy constructor\n";
        for (int i = 0; i < count; i++)
        {
            p_nodes[i] = other.p_nodes[i];
        }
    }

    DataAnalyzer& operator=(const DataAnalyzer& other)
    {
        cout << "[DataAnalyzer] Copy assignment\n";
        if (this == &other) return *this;

        delete[] p_nodes;
        count = other.count;
        capacity = other.capacity;
        p_nodes = new DataSource[capacity];

        for (int i = 0; i < count; i++)
        {
            p_nodes[i] = other.p_nodes[i];
        }

        return *this;
    }

    DataAnalyzer(DataAnalyzer&& other)
        : p_nodes(other.p_nodes), count(other.count), capacity(other.capacity)
    {
        cout << "[DataAnalyzer] Move constructor\n";
        other.p_nodes = nullptr;
        other.count = 0;
        other.capacity = 0;
    }

    DataAnalyzer& operator=(DataAnalyzer&& other)
    {
        cout << "[DataAnalyzer] Move assignment\n";
        if (this == &other) return *this;

        delete[] p_nodes;
        p_nodes = other.p_nodes;
        count = other.count;
        capacity = other.capacity;

        other.p_nodes = nullptr;
        other.count = 0;
        other.capacity = 0;

        return *this;
    }

    ~DataAnalyzer()
    {
        cout << "[DataAnalyzer] Destructor\n";
        delete[] p_nodes;
    }

    void loadSource(DataSourceType type, string name)
    {
        if (count >= capacity)
        {
            if (capacity == 0) capacity = 4;
            grow();
        }

        p_nodes[count] = DataSource(type, name);
        count++;
        cout << "Источник \"" << name << "\" ("
            << getDataSourceTypeName(type) << ") загружен.\n";
    }

    void convertSources() const
    {
        cout << "\nКонвертация источников:\n\n";
        const DataSourceType targets[] = { JSON, XML, CSV };

        for (int i = 0; i < count; i++)
        {
            DataSourceType current_type = p_nodes[i].getType();
            DataSourceType target_type = targets[static_cast<int>(current_type) % 3];

            cout << "  " << getDataSourceTypeName(current_type)
                << " -> " << getDataSourceTypeName(target_type)
                << " [" << p_nodes[i].getName() << "]\n";
        }
    }

    void comparativeAnalysis() const
    {
        cout << "\nСравнительный анализ:\n\n";

        if (count == 0)
        {
            cout << "  Источники отсутствуют.\n";
            return;
        }

        int max_count = 0;
        int max_index = 0;

        for (int i = 0; i < count; i++)
        {
            int elem_count = p_nodes[i].getElementCount();
            cout << "  " << p_nodes[i].getName() << " ("
                << getDataSourceTypeName(p_nodes[i].getType()) << "): "
                << elem_count << " элементов\n";

            if (elem_count > max_count)
            {
                max_count = elem_count;
                max_index = i;
            }
        }

        cout << "\n  Источник с максимальным объёмом: "
            << p_nodes[max_index].getName()
            << " (" << max_count << " элементов)\n";
    }

    bool removeSource(string name)
    {
        for (int i = 0; i < count; i++)
        {
            if (p_nodes[i].getName() == name)
            {
                for (int j = i; j < count - 1; j++)
                {
                    p_nodes[j] = p_nodes[j + 1];
                }
                count--;
                cout << "Источник \"" << name << "\" удалён.\n";
                return true;
            }
        }
        cout << "Источник \"" << name << "\" не найден.\n";
        return false;
    }

    void printReport() const
    {
        cout << "\nОТЧЁТ: Источники данных (" << count << " шт.)\n\n";

        if (count == 0)
        {
            cout << "  Источники отсутствуют.\n";
        }
        else
        {
            for (int i = 0; i < count; i++)
            {
                cout << "[" << i + 1 << "]\n";
                p_nodes[i].print();
                cout << "\n";
            }
        }
    }

    int getCount() const { return count; }
};

int main()
{
    setlocale(LC_ALL, "Russian");
    srand((unsigned int)time(nullptr));

    DataAnalyzer analyzer(4);

    analyzer.loadSource(CSV, "users_data");
    analyzer.loadSource(JSON, "api_responses");
    analyzer.loadSource(XML, "config_file");

    int operation_choice = 0;

    while (operation_choice != 7)
    {
        cout << "\nВыберите операцию:\n";
        cout << "1. Загрузить новый источник данных\n";
        cout << "2. Конвертация источников\n";
        cout << "3. Сравнительный анализ\n";
        cout << "4. Удалить источник по имени\n";
        cout << "5. Отчёт (список всех источников)\n";
        cout << "6. Демонстрация копирования/перемещения\n";
        cout << "7. Выход\n";
        cout << "Ваш выбор: ";
        cin >> operation_choice;

        switch (operation_choice)
        {
        case 1:
        {
            int type_input = 0;
            string source_name;

            cout << "Доступные типы: 0-CSV, 1-JSON, 2-XML\n";
            cout << "Введите тип источника: ";
            cin >> type_input;
            cout << "Введите имя источника: ";
            cin >> source_name;

            if (type_input >= 0 && type_input <= 2)
            {
                analyzer.loadSource(
                    static_cast<DataSourceType>(type_input),
                    source_name);
            }
            else
            {
                cout << "Неверный тип источника!\n";
            }
            break;
        }

        case 2:
            analyzer.convertSources();
            break;

        case 3:
            analyzer.comparativeAnalysis();
            break;

        case 4:
        {
            string source_name;
            cout << "Введите имя источника для удаления: ";
            cin >> source_name;
            analyzer.removeSource(source_name);
            break;
        }

        case 5:
            analyzer.printReport();
            break;

        case 6:
        {
            cout << "\nДемонстрация правила пяти:\n\n";
            cout << "Создание копии анализатора:\n";
            DataAnalyzer copy_analyzer = analyzer;

            cout << "\nСоздание перемещённого анализатора:\n";
            DataAnalyzer moved_analyzer = DataAnalyzer(2);

            cout << "\nИсходный анализатор после копирования:\n";
            analyzer.printReport();
            break;
        }

        case 7:
            cout << "Выход из программы.\n";
            break;

        default:
            cout << "Неверный выбор операции! Попробуйте снова.\n";
            break;
        }
    }

    return 0;
}
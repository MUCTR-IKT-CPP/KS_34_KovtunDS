#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <clocale>
#include <iomanip>
#include <vector>

using namespace std;

/*
 * Базовый класс медицинского препарата.
 * Содержит общие поля и виртуальные методы для полиморфизма.
 */
class MedicalPreparation
{
protected:
    string name;
    int shelfLifeDays;
    double price;
    double cost;
    int quantity;
    int initialQuantity;
    int productionDate;

    int totalSold;
    double totalItemRevenue;
    double totalItemCost;

public:
    MedicalPreparation(string p_name, int p_shelfLife, double p_price, double p_cost, int p_quantity, int p_prodDate)
        : name(p_name), shelfLifeDays(p_shelfLife), price(p_price), cost(p_cost),
        quantity(p_quantity), initialQuantity(p_quantity), productionDate(p_prodDate),
        totalSold(0), totalItemRevenue(0.0), totalItemCost(0.0) {
    }

    virtual ~MedicalPreparation() {}

    virtual void apply() const = 0;

    virtual bool checkExpiration(int currentDate) const {
        return (currentDate - productionDate) < shelfLifeDays;
    }

    virtual string getStorageRules() const = 0;
    virtual string getTypeName() const = 0;
    virtual MedicalPreparation* clone() const = 0;

    virtual bool isColdRemedy() const {
        return name == "Аспирин" || name == "Антигриппин" || name == "Лазолван";
    }

    virtual void restock(int quantity_to_add, int new_production_date) {
        quantity += quantity_to_add;
        productionDate = new_production_date;
    }

    virtual MedicalPreparation* createFreshBatch(int currentDate) const = 0;

    void recordSale(double sale_price, double item_cost) {
        totalSold++;
        totalItemRevenue += sale_price;
        totalItemCost += item_cost;
    }

    int getDaysSinceProduction(int currentDate) const {
        return currentDate - productionDate;
    }

    int getDaysUntilExpiration(int currentDate) const {
        return shelfLifeDays - (currentDate - productionDate);
    }

    void printItemProfitability() const {
        double profit = totalItemRevenue - totalItemCost;
        double margin = totalItemRevenue > 0 ? (profit / totalItemRevenue) * 100.0 : 0.0;

        cout << "  - " << name << " (" << getTypeName() << "):\n";
        cout << "    Продано: " << totalSold << " шт.\n";
        cout << "    Выручка: " << fixed << setprecision(2) << totalItemRevenue << " руб.\n";
        cout << "    Себестоимость: " << totalItemCost << " руб.\n";
        cout << "    Чистая прибыль: " << profit << " руб.\n";
        cout << "    Рентабельность: " << margin << "%\n\n";
    }

    string getName() const { return name; }
    int getQuantity() const { return quantity; }
    double getPrice() const { return price; }
    double getCost() const { return cost; }
    int getInitialQuantity() const { return initialQuantity; }
    int getProductionDate() const { return productionDate; }
    int getShelfLifeDays() const { return shelfLifeDays; }

    void decrementQuantity() {
        if (quantity > 0) quantity--;
    }
};

class Tablets : public MedicalPreparation
{
private:
    double dosage;

public:
    Tablets(string p_name, int p_shelfLife, double p_price, double p_cost, int p_quantity, int p_prodDate, double p_dosage)
        : MedicalPreparation(p_name, p_shelfLife, p_price, p_cost, p_quantity, p_prodDate), dosage(p_dosage) {
    }

    void apply() const override {
        cout << "  Способ применения: принять внутрь, запивая водой.\n";
    }
    string getStorageRules() const override {
        return "Хранить в сухом, защищенном от света месте при t < 25C";
    }
    string getTypeName() const override { return "Таблетки"; }
    MedicalPreparation* clone() const override { return new Tablets(*this); }
    MedicalPreparation* createFreshBatch(int currentDate) const override {
        return new Tablets(name, shelfLifeDays, price, cost, initialQuantity, currentDate, dosage);
    }
};

class Syrup : public MedicalPreparation
{
private:
    double volume;

public:
    Syrup(string p_name, int p_shelfLife, double p_price, double p_cost, int p_quantity, int p_prodDate, double p_volume)
        : MedicalPreparation(p_name, p_shelfLife, p_price, p_cost, p_quantity, p_prodDate), volume(p_volume) {
    }

    void apply() const override {
        cout << "  Способ применения: отмерить мерной ложкой, принимать до еды.\n";
    }
    string getStorageRules() const override {
        return "Хранить при t от +5C до +15C, не замораживать";
    }
    string getTypeName() const override { return "Сироп"; }
    MedicalPreparation* clone() const override { return new Syrup(*this); }
    MedicalPreparation* createFreshBatch(int currentDate) const override {
        return new Syrup(name, shelfLifeDays, price, cost, initialQuantity, currentDate, volume);
    }
};

class Ointment : public MedicalPreparation
{
private:
    double weight;

public:
    Ointment(string p_name, int p_shelfLife, double p_price, double p_cost, int p_quantity, int p_prodDate, double p_weight)
        : MedicalPreparation(p_name, p_shelfLife, p_price, p_cost, p_quantity, p_prodDate), weight(p_weight) {
    }

    void apply() const override {
        cout << "  Способ применения: нанести тонким слоем на пораженный участок.\n";
    }
    string getStorageRules() const override {
        return "Хранить в прохладном месте, избегать прямых солнечных лучей";
    }
    string getTypeName() const override { return "Мазь"; }
    MedicalPreparation* clone() const override { return new Ointment(*this); }
    MedicalPreparation* createFreshBatch(int currentDate) const override {
        return new Ointment(name, shelfLifeDays, price, cost, initialQuantity, currentDate, weight);
    }
};

/*
 * Класс-менеджер аптеки.
 */
class Pharmacy
{
private:
    MedicalPreparation** p_inventory;
    int count;
    int capacity;
    double balance;
    double totalRevenue;
    double totalCostOfGoodsSold;

    void grow()
    {
        int new_capacity = capacity == 0 ? 10 : capacity * 2;
        MedicalPreparation** p_new_inventory = new MedicalPreparation * [new_capacity];

        for (int i = 0; i < count && i < new_capacity; i++)
        {
            p_new_inventory[i] = p_inventory[i];
        }

        delete[] p_inventory;
        p_inventory = p_new_inventory;
        capacity = new_capacity;
    }

public:
    Pharmacy() : p_inventory(nullptr), count(0), capacity(0), balance(50000.0), totalRevenue(0.0), totalCostOfGoodsSold(0.0)
    {
    }

    Pharmacy(int initial_capacity)
        : p_inventory(new MedicalPreparation* [initial_capacity]), count(0), capacity(initial_capacity),
        balance(50000.0), totalRevenue(0.0), totalCostOfGoodsSold(0.0)
    {
    }

    Pharmacy(const Pharmacy& other)
        : p_inventory(new MedicalPreparation* [other.capacity]), count(other.count), capacity(other.capacity),
        balance(other.balance), totalRevenue(other.totalRevenue), totalCostOfGoodsSold(other.totalCostOfGoodsSold)
    {
        for (int i = 0; i < count && i < capacity; i++)
        {
            p_inventory[i] = other.p_inventory[i]->clone();
        }
    }

    Pharmacy& operator=(const Pharmacy& other)
    {
        if (this == &other) return *this;

        for (int i = 0; i < count; i++) delete p_inventory[i];
        delete[] p_inventory;

        count = other.count;
        capacity = other.capacity;
        balance = other.balance;
        totalRevenue = other.totalRevenue;
        totalCostOfGoodsSold = other.totalCostOfGoodsSold;
        p_inventory = new MedicalPreparation * [capacity];

        for (int i = 0; i < count && i < capacity; i++)
        {
            p_inventory[i] = other.p_inventory[i]->clone();
        }

        return *this;
    }

    Pharmacy(Pharmacy&& other) noexcept
        : p_inventory(other.p_inventory), count(other.count), capacity(other.capacity),
        balance(other.balance), totalRevenue(other.totalRevenue), totalCostOfGoodsSold(other.totalCostOfGoodsSold)
    {
        other.p_inventory = nullptr;
        other.count = 0;
        other.capacity = 0;
    }

    Pharmacy& operator=(Pharmacy&& other) noexcept
    {
        if (this == &other) return *this;

        for (int i = 0; i < count; i++) delete p_inventory[i];
        delete[] p_inventory;

        p_inventory = other.p_inventory;
        count = other.count;
        capacity = other.capacity;
        balance = other.balance;
        totalRevenue = other.totalRevenue;
        totalCostOfGoodsSold = other.totalCostOfGoodsSold;

        other.p_inventory = nullptr;
        other.count = 0;
        other.capacity = 0;

        return *this;
    }

    ~Pharmacy()
    {
        for (int i = 0; i < count; i++)
        {
            delete p_inventory[i];
        }
        delete[] p_inventory;
    }

    void addPreparation(MedicalPreparation* p_prep)
    {
        if (count >= capacity) grow();
        p_inventory[count] = p_prep;
        count++;
    }

    void generateInitialStock(int currentDate)
    {
        for (int i = 0; i < count; i++) delete p_inventory[i];
        count = 0;

        string tabNames[] = { "Парацетамол", "Ибупрофен", "Аспирин", "Антигриппин" };
        string syrNames[] = { "Лазолван", "Гербион", "Амбробене", "Стодаль" };
        string oinNames[] = { "Звездочка", "Диклофенак", "Левомеколь", "Акридерм" };

        for (int i = 0; i < 4; i++)
        {
            int tabCount = rand() % 16 + 15;
            int shelfLife = rand() % 336 + 30;
            int prodDate = currentDate - shelfLife + rand() % 40 - 10;
            if (prodDate < 1) prodDate = 1;
            addPreparation(new Tablets(tabNames[i], shelfLife, rand() % 200 + 100, rand() % 50 + 50, tabCount, prodDate, rand() % 3 + 1));

            int syrCount = rand() % 16 + 15;
            shelfLife = rand() % 336 + 30;
            prodDate = currentDate - shelfLife + rand() % 40 - 10;
            if (prodDate < 1) prodDate = 1;
            addPreparation(new Syrup(syrNames[i], shelfLife, rand() % 300 + 150, rand() % 80 + 70, syrCount, prodDate, rand() % 100 + 100));

            int oinCount = rand() % 16 + 15;
            shelfLife = rand() % 336 + 30;
            prodDate = currentDate - shelfLife + rand() % 40 - 10;
            if (prodDate < 1) prodDate = 1;
            addPreparation(new Ointment(oinNames[i], shelfLife, rand() % 250 + 120, rand() % 60 + 40, oinCount, prodDate, rand() % 30 + 15));
        }
    }

    /*
     * Полный сброс аптеки
     */
    void resetAll(int newCurrentDate)
    {
        balance = 50000.0;
        totalRevenue = 0.0;
        totalCostOfGoodsSold = 0.0;
        generateInitialStock(newCurrentDate);
    }

    bool isWinter(int day)
    {
        int d = day % 365;
        return d >= 335 || d <= 59;
    }

    void simulateDay(int currentDate)
    {
        cout << "\nДень симуляции: " << currentDate;
        cout << " (Сезон: " << (isWinter(currentDate) ? "Зима" : "Не зима") << ")\n\n";

        double dailyRevenue = 0.0;
        int expiredCount = 0;

        cout << "Списание просроченных препаратов:\n";
        for (int i = 0; i < count; )
        {
            if (!p_inventory[i]->checkExpiration(currentDate))
            {
                MedicalPreparation* p_replacement = p_inventory[i]->createFreshBatch(currentDate);

                cout << "  - " << p_inventory[i]->getName()
                    << " (" << p_inventory[i]->getTypeName() << ") — списан\n";

                delete p_inventory[i];
                for (int j = i; j < count - 1; j++)
                {
                    p_inventory[j] = p_inventory[j + 1];
                }
                count--;

                addPreparation(p_replacement);
                cout << "  + Закуплена новая партия: " << p_replacement->getName()
                    << " (" << p_replacement->getQuantity() << " шт.)\n";

                expiredCount++;
            }
            else
            {
                i++;
            }
        }
        if (expiredCount == 0) cout << "  Просроченных препаратов нет.\n";

        int customers = rand() % 16 + 5;
        cout << "\nОбслужено покупателей: " << customers << "\n";
        cout << "Продажи:\n";

        bool winter = isWinter(currentDate);

        for (int c = 0; c < customers; c++)
        {
            int itemsWanted = rand() % 3 + 1;
            for (int w = 0; w < itemsWanted; w++)
            {
                if (count == 0) break;

                int idx = -1;

                if (winter && rand() % 4 != 0)
                {
                    vector<int> coldRemedyIndices;
                    for (int k = 0; k < count; k++)
                    {
                        if (p_inventory[k]->isColdRemedy() && p_inventory[k]->getQuantity() > 0)
                        {
                            coldRemedyIndices.push_back(k);
                        }
                    }

                    if (!coldRemedyIndices.empty())
                    {
                        idx = coldRemedyIndices[rand() % coldRemedyIndices.size()];
                    }
                }

                if (idx == -1)
                {
                    vector<int> availableIndices;
                    for (int k = 0; k < count; k++)
                    {
                        if (p_inventory[k]->getQuantity() > 0)
                        {
                            availableIndices.push_back(k);
                        }
                    }

                    if (!availableIndices.empty())
                    {
                        idx = availableIndices[rand() % availableIndices.size()];
                    }
                }

                if (idx != -1)
                {
                    MedicalPreparation* p_item = p_inventory[idx];

                    if (p_item->checkExpiration(currentDate) && p_item->getQuantity() > 0)
                    {
                        double discount = (rand() % 11 + 5) / 100.0;
                        double finalPrice = p_item->getPrice() * (1.0 - discount);

                        p_item->decrementQuantity();
                        p_item->recordSale(finalPrice, p_item->getCost());

                        dailyRevenue += finalPrice;
                        totalCostOfGoodsSold += p_item->getCost();

                        cout << "  - " << p_item->getName() << " (скидка " << (int)(discount * 100) << "%, цена: "
                            << fixed << setprecision(2) << finalPrice << " руб.)\n";
                    }
                }
            }
        }

        totalRevenue += dailyRevenue;
        balance += dailyRevenue;

        cout << "\nПополнение запасов (остаток < 10% от начального):\n";
        int restockedCount = 0;
        for (int i = 0; i < count; i++)
        {
            if (p_inventory[i]->getQuantity() < p_inventory[i]->getInitialQuantity() * 0.1)
            {
                int restock_quantity = rand() % 20 + 10;
                double restockCost = restock_quantity * p_inventory[i]->getCost();

                if (balance >= restockCost)
                {
                    balance -= restockCost;
                    p_inventory[i]->restock(restock_quantity, currentDate);
                    cout << "  + Закуплено " << restock_quantity << " шт. препарата: " << p_inventory[i]->getName() << "\n";
                    restockedCount++;
                }
                else
                {
                    cout << "  ! Недостаточно средств для закупки: " << p_inventory[i]->getName() << "\n";
                }
            }
        }
        if (restockedCount == 0) cout << "  Пополнение не требовалось.\n";

        cout << "\nЕжедневный отчет:\n";
        cout << "  Выручка за день: " << fixed << setprecision(2) << dailyRevenue << " руб.\n";
        cout << "  Текущий баланс: " << balance << " руб.\n";
        cout << "  Остатков на складе: " << count << " позиций.\n";
    }

    void printDetailedProfitabilityAnalysis() const
    {
        cout << "\nДетальный анализ рентабельности каждого препарата:\n\n";
        if (count == 0)
        {
            cout << "  Склад пуст, анализ невозможен.\n";
            return;
        }

        for (int i = 0; i < count; i++)
        {
            p_inventory[i]->printItemProfitability();
        }

        cout << "Общая сводка:\n";
        cout << "  Общая выручка: " << fixed << setprecision(2) << totalRevenue << " руб.\n";
        cout << "  Общая себестоимость проданного: " << totalCostOfGoodsSold << " руб.\n";
        cout << "  Итоговый баланс: " << balance << " руб.\n";
    }

    void printInventorySummary() const
    {
        cout << "\nСписок медикаментов на складе (" << count << " позиций):\n\n";
        if (count == 0)
        {
            cout << "  Склад пуст.\n";
            return;
        }

        for (int i = 0; i < count; i++)
        {
            cout << "[" << i + 1 << "]\n";
            cout << "  Название: " << p_inventory[i]->getName() << "\n";
            cout << "  Тип: " << p_inventory[i]->getTypeName() << "\n";
            cout << "  Количество: " << p_inventory[i]->getQuantity() << " шт.\n";
            cout << "  Цена: " << p_inventory[i]->getPrice() << " руб.\n";
            cout << "  Себестоимость: " << p_inventory[i]->getCost() << " руб.\n";
            cout << "  Хранение: " << p_inventory[i]->getStorageRules() << "\n";
            p_inventory[i]->apply();
            cout << "\n";
        }
    }

    /*
     * Выводит даты производства всех препаратов заданного типа.
     *
     * @param typeName название типа препарата.
     * @param currentDate текущая дата симуляции.
     */
    void printProductionDatesByType(string typeName, int currentDate) const
    {
        cout << "\nДаты производства препаратов типа \"" << typeName << "\":\n\n";

        int foundCount = 0;
        for (int i = 0; i < count; i++)
        {
            if (p_inventory[i]->getTypeName() == typeName)
            {
                foundCount++;
                int daysSinceProd = p_inventory[i]->getDaysSinceProduction(currentDate);
                int daysUntilExp = p_inventory[i]->getDaysUntilExpiration(currentDate);

                cout << "  [" << foundCount << "] " << p_inventory[i]->getName() << "\n";
                cout << "    Дата производства: день " << p_inventory[i]->getProductionDate() << "\n";
                cout << "    Прошло дней: " << daysSinceProd << "\n";
                cout << "    Срок годности: " << p_inventory[i]->getShelfLifeDays() << " дней\n";
                cout << "    Осталось дней: " << daysUntilExp;

                if (daysUntilExp <= 0) {
                    cout << " (ПРОСРОЧЕН на " << -daysUntilExp << " дн.)";
                }
                else if (daysUntilExp <= 30) {
                    cout << " (скоро истечёт!)";
                }
                cout << "\n\n";
            }
        }

        if (foundCount == 0)
        {
            cout << "  Препараты типа \"" << typeName << "\" не найдены.\n";
        }
    }

    int getCount() const { return count; }
    double getBalance() const { return balance; }
};

int main()
{
    setlocale(LC_ALL, "Russian");
    srand(static_cast<unsigned int>(time(nullptr)));

    Pharmacy pharmacy(100);

    int currentDate = rand() % 500 + 1;
    pharmacy.generateInitialStock(currentDate);

    cout << "Начальный баланс: 50000.00 руб.\n";
    cout << "Текущая дата симуляции: " << currentDate << "\n";
    cout << "Препаратов на складе: " << pharmacy.getCount() << "\n";

    int operation_choice = 0;

    while (operation_choice != 7)
    {
        cout << "\nВыберите операцию:\n";
        cout << "1. Вывести список медикаментов на складе\n";
        cout << "2. Симуляция одного дня\n";
        cout << "3. Полная симуляция (90 дней)\n";
        cout << "4. Детальный анализ рентабельности каждого препарата\n";
        cout << "5. Сгенерировать новый склад (сброс)\n";
        cout << "6. Вывести даты производства по типу препарата\n";
        cout << "7. Выход\n";
        cout << "Ваш выбор: ";
        cin >> operation_choice;

        switch (operation_choice)
        {
        case 1:
            pharmacy.printInventorySummary();
            break;

        case 2:
            pharmacy.simulateDay(currentDate);
            currentDate++;
            break;

        case 3:
        {
            int days = 90;
            cout << "\nЗапуск симуляции на " << days << " дней...\n";
            for (int day = 1; day <= days; day++)
            {
                pharmacy.simulateDay(currentDate);
                currentDate++;
            }
            cout << "\nСимуляция завершена. Текущая дата: " << currentDate << "\n";
            break;
        }

        case 4:
            pharmacy.printDetailedProfitabilityAnalysis();
            break;

        case 5:
        {
            cout << "\nПолный сброс аптеки...\n";
            currentDate = rand() % 500 + 1;
            pharmacy.resetAll(currentDate);
            cout << "Новая дата симуляции: " << currentDate << "\n";
            cout << "Начальный баланс: 50000.00 руб.\n";
            cout << "Препаратов на складе: " << pharmacy.getCount() << "\n";
            cout << "Выручка и статистика продаж обнулены.\n";
            break;
        }

        case 6:
        {
            int type_choice = 0;
            cout << "\nДоступные типы:\n";
            cout << "  1. Таблетки\n";
            cout << "  2. Сироп\n";
            cout << "  3. Мазь\n";
            cout << "Ваш выбор: ";
            cin >> type_choice;

            string typeName;
            if (type_choice == 1) typeName = "Таблетки";
            else if (type_choice == 2) typeName = "Сироп";
            else if (type_choice == 3) typeName = "Мазь";
            else {
                cout << "Неверный тип препарата!\n";
                break;
            }

            pharmacy.printProductionDatesByType(typeName, currentDate);
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
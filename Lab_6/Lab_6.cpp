#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <clocale>
#include <iomanip>

using namespace std;

class Account;

class ITransferable
{
public:
    virtual ~ITransferable() = default;
    virtual bool transfer(Account* to, double amount) = 0;
};

class Account : public ITransferable
{
protected:
    string accountNumber;
    double balance;

public:
    Account(string accNum, double initialBalance)
        : accountNumber(accNum), balance(initialBalance) {
    }

    virtual ~Account() = default;

    virtual void deposit(double amount) = 0;
    virtual bool withdraw(double amount) = 0;
    virtual double getBalance() const = 0;
    virtual string getTypeName() const = 0;
    virtual void printDetails() const = 0;
    virtual Account* clone() const = 0;
    virtual void applyInterest() {}

    string getAccountNumber() const { return accountNumber; }
};

class SavingsAccount : public Account
{
private:
    double interestRate;

public:
    SavingsAccount(string accNum, double initialBalance, double rate)
        : Account(accNum, initialBalance), interestRate(rate) {
    }

    void deposit(double amount) override {
        if (amount > 0) balance += amount;
    }

    bool withdraw(double amount) override {
        if (amount > 0 && amount <= balance) {
            balance -= amount;
            return true;
        }
        return false;
    }

    double getBalance() const override { return balance; }
    string getTypeName() const override { return "Сберегательный"; }

    void printDetails() const override {
        cout << "  Номер: " << accountNumber << "\n";
        cout << "  Тип: " << getTypeName() << "\n";
        cout << "  Баланс: " << fixed << setprecision(2) << balance << " руб.\n";
        cout << "  Процентная ставка: " << (interestRate * 100) << "%\n";
    }

    Account* clone() const override {
        return new SavingsAccount(*this);
    }

    bool transfer(Account* to, double amount) override {
        if (withdraw(amount)) {
            to->deposit(amount);
            return true;
        }
        return false;
    }

    void applyInterest() override {
        double interest = balance * interestRate;
        balance += interest;
    }
};

class CheckingAccount : public Account
{
private:
    double overdraftLimit;

public:
    CheckingAccount(string accNum, double initialBalance, double limit)
        : Account(accNum, initialBalance), overdraftLimit(limit) {
    }

    void deposit(double amount) override {
        if (amount > 0) balance += amount;
    }

    bool withdraw(double amount) override {
        if (amount > 0 && amount <= (balance + overdraftLimit)) {
            balance -= amount;
            return true;
        }
        return false;
    }

    double getBalance() const override { return balance; }
    string getTypeName() const override { return "Расчетный"; }

    void printDetails() const override {
        cout << "  Номер: " << accountNumber << "\n";
        cout << "  Тип: " << getTypeName() << "\n";
        cout << "  Баланс: " << fixed << setprecision(2) << balance << " руб.\n";
        cout << "  Лимит овердрафта: " << overdraftLimit << " руб.\n";
    }

    Account* clone() const override {
        return new CheckingAccount(*this);
    }

    bool transfer(Account* to, double amount) override {
        if (withdraw(amount)) {
            to->deposit(amount);
            return true;
        }
        return false;
    }
};

class CreditAccount : public Account
{
private:
    double creditLimit;
    double interestRate;

public:
    CreditAccount(string accNum, double initialBalance, double limit, double rate)
        : Account(accNum, initialBalance), creditLimit(limit), interestRate(rate) {
    }

    void deposit(double amount) override {
        if (amount > 0) balance += amount;
    }

    bool withdraw(double amount) override {
        double availableFunds = creditLimit + balance;
        if (amount > 0 && amount <= availableFunds) {
            balance -= amount;
            return true;
        }
        return false;
    }

    double getBalance() const override { return balance; }
    string getTypeName() const override { return "Кредитный"; }

    void printDetails() const override {
        cout << "  Номер: " << accountNumber << "\n";
        cout << "  Тип: " << getTypeName() << "\n";
        cout << "  Баланс: " << fixed << setprecision(2) << balance << " руб.\n";
        cout << "  Кредитный лимит: " << creditLimit << " руб.\n";
        cout << "  Процентная ставка по долгу: " << (interestRate * 100) << "%\n";
    }

    Account* clone() const override {
        return new CreditAccount(*this);
    }

    bool transfer(Account* to, double amount) override {
        if (withdraw(amount)) {
            to->deposit(amount);
            return true;
        }
        return false;
    }

    void applyInterest() override {
        if (balance < 0) {
            balance *= (1.0 + interestRate);
        }
    }
};

class Bank
{
private:
    Account** p_accounts;
    int count;
    int capacity;

    void grow()
    {
        int new_capacity = capacity == 0 ? 10 : capacity * 2;
        Account** p_new_accounts = new Account * [new_capacity];

        for (int i = 0; i < count && i < new_capacity; i++)
        {
            p_new_accounts[i] = p_accounts[i];
        }

        delete[] p_accounts;
        p_accounts = p_new_accounts;
        capacity = new_capacity;
    }

    string generateAccountNumber(string prefix)
    {
        int num = rand() % 9000 + 1000;
        return prefix + "-" + to_string(num);
    }

public:
    Bank() : p_accounts(nullptr), count(0), capacity(0) {}

    Bank(int initial_capacity)
        : p_accounts(new Account* [initial_capacity]), count(0), capacity(initial_capacity) {
    }

    Bank(const Bank& other)
        : p_accounts(new Account* [other.capacity]), count(other.count), capacity(other.capacity)
    {
        for (int i = 0; i < count && i < capacity; i++)
        {
            p_accounts[i] = other.p_accounts[i]->clone();
        }
    }

    Bank& operator=(const Bank& other)
    {
        if (this == &other) return *this;

        for (int i = 0; i < count; i++) delete p_accounts[i];
        delete[] p_accounts;

        count = other.count;
        capacity = other.capacity;
        p_accounts = new Account * [capacity];

        for (int i = 0; i < count && i < capacity; i++)
        {
            p_accounts[i] = other.p_accounts[i]->clone();
        }

        return *this;
    }

    Bank(Bank&& other) noexcept
        : p_accounts(other.p_accounts), count(other.count), capacity(other.capacity)
    {
        other.p_accounts = nullptr;
        other.count = 0;
        other.capacity = 0;
    }

    Bank& operator=(Bank&& other) noexcept
    {
        if (this == &other) return *this;

        for (int i = 0; i < count; i++) delete p_accounts[i];
        delete[] p_accounts;

        p_accounts = other.p_accounts;
        count = other.count;
        capacity = other.capacity;

        other.p_accounts = nullptr;
        other.count = 0;
        other.capacity = 0;

        return *this;
    }

    ~Bank()
    {
        for (int i = 0; i < count; i++)
        {
            delete p_accounts[i];
        }
        delete[] p_accounts;
    }

    void addAccount(Account* acc)
    {
        if (count >= capacity) grow();
        p_accounts[count] = acc;
        count++;
    }

    void generateInitialAccounts()
    {
        for (int i = 0; i < count; i++) delete p_accounts[i];
        count = 0;

        int numAccounts = rand() % 4 + 3;

        for (int i = 0; i < numAccounts; i++)
        {
            int accountType = rand() % 3;

            if (accountType == 0)
            {
                double initialBalance = rand() % 200000 + 10000;
                double rate = (rand() % 10 + 3) / 100.0;
                addAccount(new SavingsAccount(generateAccountNumber("SAV"), initialBalance, rate));
            }
            else if (accountType == 1)
            {
                double initialBalance = rand() % 50000 + 5000;
                double overdraft = rand() % 30000 + 5000;
                addAccount(new CheckingAccount(generateAccountNumber("CHK"), initialBalance, overdraft));
            }
            else
            {
                double initialBalance = -(rand() % 20000 + 1000);
                double creditLimit = rand() % 100000 + 30000;
                double creditRate = (rand() % 20 + 10) / 100.0;
                addAccount(new CreditAccount(generateAccountNumber("CRD"), initialBalance, creditLimit, creditRate));
            }
        }
    }

    void applyInterest()
    {
        cout << "\nНачисление процентов:\n\n";
        int affectedCount = 0;
        for (int i = 0; i < count; i++)
        {
            double oldBalance = p_accounts[i]->getBalance();
            p_accounts[i]->applyInterest();
            double newBalance = p_accounts[i]->getBalance();

            if (newBalance != oldBalance)
            {
                double diff = newBalance - oldBalance;
                if (diff > 0) {
                    cout << "  На счет " << p_accounts[i]->getAccountNumber()
                        << " начислено: " << fixed << setprecision(2) << diff << " руб.\n";
                }
                else {
                    cout << "  Со счета " << p_accounts[i]->getAccountNumber()
                        << " списано процентов по долгу: " << fixed << setprecision(2) << -diff << " руб.\n";
                }
                affectedCount++;
            }
        }
        if (affectedCount == 0) cout << "  Операций по начислению процентов не произошло.\n";
    }

    void printAllAccounts() const
    {
        cout << "\nСписок счетов в банке (" << count << " шт.):\n\n";
        if (count == 0)
        {
            cout << "  В банке нет открытых счетов.\n";
            return;
        }

        for (int i = 0; i < count; i++)
        {
            cout << "[" << i + 1 << "]\n";
            p_accounts[i]->printDetails();
            cout << "\n";
        }
    }

    int getCount() const { return count; }

    Account* getAccount(int index)
    {
        if (index >= 0 && index < count) return p_accounts[index];
        return nullptr;
    }
};

int main()
{
    setlocale(LC_ALL, "Russian");
    srand(static_cast<unsigned int>(time(nullptr)));

    Bank bank(10);
    bank.generateInitialAccounts();

    int operation_choice = 0;

    while (operation_choice != 7)
    {
        cout << "\nВыберите операцию:\n";
        cout << "1. Вывести список всех счетов\n";
        cout << "2. Пополнить счет\n";
        cout << "3. Снять средства со счета\n";
        cout << "4. Перевести средства между счетами\n";
        cout << "5. Начислить проценты\n";
        cout << "6. Сгенерировать новые счета\n";
        cout << "7. Выход\n";
        cout << "Ваш выбор: ";
        cin >> operation_choice;

        switch (operation_choice)
        {
        case 1:
            bank.printAllAccounts();
            break;

        case 2:
        {
            bank.printAllAccounts();
            int idx;
            double amount;
            cout << "Введите номер счета (1-" << bank.getCount() << "): ";
            cin >> idx;
            cout << "Введите сумму пополнения: ";
            cin >> amount;

            Account* acc = bank.getAccount(idx - 1);
            if (acc && amount > 0)
            {
                acc->deposit(amount);
                cout << "\nУспешно пополнено. Новый баланс: " << acc->getBalance() << " руб.\n";
            }
            else
            {
                cout << "\nНеверный номер счета или сумма.\n";
            }
            break;
        }

        case 3:
        {
            bank.printAllAccounts();
            int idx;
            double amount;
            cout << "Введите номер счета (1-" << bank.getCount() << "): ";
            cin >> idx;
            cout << "Введите сумму снятия: ";
            cin >> amount;

            Account* acc = bank.getAccount(idx - 1);
            if (acc && amount > 0)
            {
                if (acc->withdraw(amount))
                {
                    cout << "\nУспешно снято. Новый баланс: " << acc->getBalance() << " руб.\n";
                }
                else
                {
                    cout << "\nОшибка: недостаточно средств или превышен лимит.\n";
                }
            }
            else
            {
                cout << "\nНеверный номер счета или сумма.\n";
            }
            break;
        }

        case 4:
        {
            bank.printAllAccounts();
            int fromIdx, toIdx;
            double amount;
            cout << "Введите номер счета отправителя (1-" << bank.getCount() << "): ";
            cin >> fromIdx;
            cout << "Введите номер счета получателя (1-" << bank.getCount() << "): ";
            cin >> toIdx;
            cout << "Введите сумму перевода: ";
            cin >> amount;

            Account* fromAcc = bank.getAccount(fromIdx - 1);
            Account* toAcc = bank.getAccount(toIdx - 1);

            if (fromAcc && toAcc && amount > 0 && fromIdx != toIdx)
            {
                if (fromAcc->transfer(toAcc, amount))
                {
                    cout << "\nПеревод успешно выполнен.\n";
                    cout << "  Баланс отправителя: " << fromAcc->getBalance() << " руб.\n";
                    cout << "  Баланс получателя: " << toAcc->getBalance() << " руб.\n";
                }
                else
                {
                    cout << "\nОшибка: недостаточно средств для перевода.\n";
                }
            }
            else
            {
                cout << "\nНеверные номера счетов, сумма или попытка перевода самому себе.\n";
            }
            break;
        }

        case 5:
            bank.applyInterest();
            break;

        case 6:
        {
            cout << "\nГенерация новых счетов...\n";
            bank.generateInitialAccounts();
            cout << "Создано счетов: " << bank.getCount() << "\n";
            break;
        }

        case 7:
            cout << "\nВыход из программы.\n";
            break;

        default:
            cout << "\nНеверный выбор операции! Попробуйте снова.\n";
            break;
        }
    }

    return 0;
}
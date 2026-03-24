#include <stdexcept>
#include <iostream>

class BankAccount
{
private:
    double balance;

public:
    BankAccount(double amount)
    {
        setBalance(amount);
    }

    void setBalance(double amount)
    {
        if (amount < 0)
            throw std::invalid_argument("Balance must be positive");
        balance = amount;
    }

    void deposit(double amount)
    {
        if (amount < 0)
            throw std::invalid_argument("Can not deposit with negative values");
        balance += amount;
    }

    void withdraw(double amount)
    {
        if (amount < 0)
            throw std::invalid_argument("Can not withdraw with negative values");
        if (amount > balance)
            throw std::invalid_argument("Can not withdraw more than you have");
        balance -= amount;
    }

    double getBalance() const {
        return balance;
    }
};
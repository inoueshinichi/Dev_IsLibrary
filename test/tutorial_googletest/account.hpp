#pragma once


// まだ実装されていないAccountクラス
class Account
{
public:
    virtual bool deposit(int amount) = 0;
    virtual bool withdraw(int amount) = 0;
};
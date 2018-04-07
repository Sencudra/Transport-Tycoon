#ifndef PLAYER_H
#define PLAYER_H


#include "resources.h"


class Player
{
public:
    Player();
    Player(int balance, std::string m_companyName, std::string m_playerName);
    //Player(const Player &obj);
    //Player(const Player &&obj);
    ~Player();

    void addMoney(int money){m_balance += money;}
    bool getMoney(int money);
    int getBalance(){return m_balance;}

    std::string getCompanyName(){return m_companyName;}
    std::string getPlayerName(){return m_playerName;}

    //setPlayerName(string newName):m_playerName(newName){;}
    //setCompanyName(string newName):m_companyName(newName){;}


private:

    std::string m_companyName;
    std::string m_playerName;

    int m_balance;
    rs::Color m_color;

};

#endif // PLAYER_H

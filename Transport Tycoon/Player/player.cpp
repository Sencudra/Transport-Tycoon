#include "player.h"

Player::Player()
{
    m_companyName = "New Company";
    m_playerName = "Player1";
    m_balance = 20000;
	m_color = rs::Color::GREEN;
}

Player::Player(int balance, std::string m_companyName, std::string m_playerName):
    m_balance(balance), m_playerName(m_playerName), m_companyName(m_companyName)
{

}

bool Player::getMoney(int money)
{
    if (m_balance - money > 0)
    {
        m_balance -= money;
        return true;
    }
    else
    {
		std::cout << "Player::getMoney: insufficient funds";
        return false;
    }
}

Player::~Player()
{

}

//Player::PLayer(const Player &obj)
//{
//    this->m_balance = obj.m_balance;
//    this->m_playerName = obj.m_playerName;
//    this->m_companyName = obj.m_companyName;
//}


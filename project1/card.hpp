#ifndef CARD_HPP
#define CARD_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

enum class GameState { MENU, BATTLE, REWARD, GAMEOVER, RANKING, SHOP, VIEW_DECK };
enum class CardType { ATTACK, DEFENSE, HEAL, UTILITY };
enum class MonsterIntent { ATTACK, DEFEND, BUFF, STRONG_ATTACK };
enum class RelicType { POWER_UP, DEFENSE_UP, ENERGY_UP };

struct Relic {
    std::wstring name;
    std::wstring description;
    RelicType type;
    sf::RectangleShape shape;
};

struct Card {
    std::wstring name;
    CardType type;
    int cost;
    int value;
    sf::RectangleShape shape;
};

struct RankEntry {
    int floor;
};

struct Button {
    sf::RectangleShape shape;
    sf::Text text;
    // 매개변수가 있는 생성자
    Button(const sf::Font& font) : text(font) {}
    // 기본 생성자
    Button() = default; 
};

#endif
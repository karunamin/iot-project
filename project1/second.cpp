#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <random>
#include <sstream>
#include <algorithm>
#include <iostream>

using namespace std;

enum class CardType { ATTACK, DEFENSE, HEAL };
enum class GameState { BATTLE, REWARD, GAMEOVER };

struct Card {
    string name;
    CardType type;
    int cost;
    int value;
    sf::RectangleShape shape;
};

struct Button {
    sf::RectangleShape shape;
    sf::Text text;
    Button(const sf::Font& font) : text(font) {}
};

class Game {
public:
    int playerHP = 100;
    int playerBlock = 0;
    int energy = 3;
    int monsterHP = 80;
    int monsterAttack = 10;
    int floor = 1;

    GameState state = GameState::BATTLE;
    vector<Card> hand;
    vector<Card> rewardOptions; // 보상으로 나올 카드 3장

    sf::Font font;
    Button endTurnBtn;
    mt19937 rng;

    Game() : rng(random_device{}()), font(), endTurnBtn(font) {}

    void init() {
        if (!font.openFromFile("NanumGothicCoding.ttf")) {
            cout << "폰트 로드 실패!" << endl;
        }

        // 버튼 설정
        endTurnBtn.shape.setSize({ 160.f, 60.f });
        endTurnBtn.shape.setPosition({ 800.f, 580.f });
        endTurnBtn.shape.setFillColor(sf::Color(150, 50, 50));
        endTurnBtn.text.setString("TURN END");
        endTurnBtn.text.setCharacterSize(22);
        endTurnBtn.text.setPosition({ 820.f, 595.f });

        drawHand();
    }

    Card createCardData(string name, CardType type, int cost, int value) {
        Card c;
        c.name = name; c.type = type; c.cost = cost; c.value = value;
        c.shape.setSize({ 120.f, 170.f });
        c.shape.setFillColor(sf::Color(45, 52, 54));
        c.shape.setOutlineThickness(2.f);
        c.shape.setOutlineColor(sf::Color::White);
        return c;
    }

    void drawHand() {
        hand.clear();
        for (int i = 0; i < 5; i++) {
            Card c = getRandomCardFromPool();
            c.shape.setPosition({ 80.f + i * 140.f, 400.f });
            hand.push_back(c);
        }
    }

    Card getRandomCardFromPool() {
        vector<Card> pool = {
            createCardData("Strike", CardType::ATTACK, 1, 8),
            createCardData("Bash", CardType::ATTACK, 2, 16),
            createCardData("Defend", CardType::DEFENSE, 1, 7),
            createCardData("Heal", CardType::HEAL, 2, 12)
        };
        uniform_int_distribution<int> dist(0, (int)pool.size() - 1);
        return pool[dist(rng)];
    }

    // 보상 카드 생성
    void generateRewards() {
        rewardOptions.clear();
        state = GameState::REWARD;
        for (int i = 0; i < 3; i++) {
            Card c = getRandomCardFromPool();
            // 보상 카드는 화면 중앙에 배치
            c.shape.setPosition({ 250.f + i * 180.f, 250.f });
            c.shape.setSize({ 150.f, 210.f });
            c.shape.setFillColor(sf::Color(39, 174, 96)); // 보상 카드는 초록색 계열
            rewardOptions.push_back(c);
        }
    }

    void playCard(int i) {
        if (state != GameState::BATTLE) return;
        Card& c = hand[i];
        if (energy >= c.cost) {
            energy -= c.cost;
            if (c.type == CardType::ATTACK) monsterHP -= c.value;
            else if (c.type == CardType::DEFENSE) playerBlock += c.value;
            else if (c.type == CardType::HEAL) playerHP = min(100, playerHP + c.value);
            hand.erase(hand.begin() + i);

            // 몬스터 처치 체크
            if (monsterHP <= 0) {
                generateRewards();
            }
        }
    }

    void monsterTurn() {
        if (state != GameState::BATTLE) return;
        playerHP -= max(0, monsterAttack - playerBlock);
        playerBlock = 0;
        energy = 3;
        if (playerHP <= 0) state = GameState::GAMEOVER;
        else drawHand();
    }

    void startNextFloor() {
        floor++;
        monsterHP = 80 + (floor * 15);
        monsterAttack = 10 + (floor * 2);
        energy = 3;
        playerBlock = 0;
        state = GameState::BATTLE;
        drawHand();
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode({ 1000, 700 }), "Card Game - Reward System");
    window.setFramerateLimit(60);

    Game game;
    game.init();

    sf::Text uiText(game.font);
    sf::Text cardText(game.font);

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();

            if (const auto* mouseBtn = event->getIf<sf::Event::MouseButtonPressed>()) {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

                if (game.state == GameState::BATTLE) {
                    // 전투 중 클릭 처리
                    for (int i = 0; i < (int)game.hand.size(); i++) {
                        if (game.hand[i].shape.getGlobalBounds().contains(mousePos)) {
                            game.playCard(i); break;
                        }
                    }
                    if (game.endTurnBtn.shape.getGlobalBounds().contains(mousePos)) game.monsterTurn();
                }
                else if (game.state == GameState::REWARD) {
                    // 보상 선택 처리
                    for (int i = 0; i < (int)game.rewardOptions.size(); i++) {
                        if (game.rewardOptions[i].shape.getGlobalBounds().contains(mousePos)) {
                            cout << game.rewardOptions[i].name << " 획득!" << endl;
                            game.startNextFloor();
                            break;
                        }
                    }
                }
            }
        }

        window.clear(sf::Color(30, 39, 46));

        if (game.state == GameState::BATTLE) {
            // --- 전투 화면 그리기 ---
            stringstream ss;
            ss << "FLOOR: " << game.floor << " | HP: " << game.playerHP << " | ENERGY: " << game.energy << "\nMONSTER HP: " << game.monsterHP;
            uiText.setString(ss.str());
            uiText.setPosition({ 30.f, 30.f });
            window.draw(uiText);

            for (auto& c : game.hand) {
                window.draw(c.shape);
                cardText.setString(c.name + "\nCost:" + to_string(c.cost) + "\nVal:" + to_string(c.value));
                cardText.setPosition(c.shape.getPosition() + sf::Vector2f(10.f, 15.f));
                window.draw(cardText);
            }
            window.draw(game.endTurnBtn.shape);
            window.draw(game.endTurnBtn.text);
        }
        else if (game.state == GameState::REWARD) {
            // --- 보상 화면 그리기 ---
            uiText.setString("CHOOSE A CARD REWARD");
            uiText.setPosition({ 350.f, 150.f });
            window.draw(uiText);

            for (auto& c : game.rewardOptions) {
                window.draw(c.shape);
                cardText.setString(c.name + "\nCost:" + to_string(c.cost) + "\nVal:" + to_string(c.value));
                cardText.setPosition(c.shape.getPosition() + sf::Vector2f(10.f, 20.f));
                window.draw(cardText);
            }
        }
        else if (game.state == GameState::GAMEOVER) {
            uiText.setString("GAME OVER\nFloor: " + to_string(game.floor));
            uiText.setCharacterSize(50);
            uiText.setPosition({ 350.f, 300.f });
            window.draw(uiText);
        }

        window.display();
    }
    return 0;
}
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>

using namespace std;

// ================= 카드 =================
enum CardType { ATTACK, DEFENSE, HEAL, ENERGY, DRAW, POISON, AOE };

class Card {
public:
    string name;
    CardType type;
    int cost;
    int value;

    Card(string n, CardType t, int c, int v) : name(n), type(t), cost(c), value(v) {}
};

// ================= 플레이어 =================
class Player {
public:
    int hp = 100;
    int block = 0;
    int energy = 3;
    int gold = 100;

    vector<Card> deck;
    vector<Card> hand;
    vector<Card> discard;

    mt19937 g{ random_device{}() };

    void shuffleDeck() { shuffle(deck.begin(), deck.end(), g); }

    void draw(int n = 5) {
        for (int i = 0; i < n; i++) {
            if (deck.empty()) {
                if (discard.empty()) break;
                deck = discard;
                discard.clear();
                shuffleDeck();
            }
            hand.push_back(deck.back());
            deck.pop_back();
        }
    }

    void resetTurn() { energy = 3; block = 0; }

    void endTurn() {
        discard.insert(discard.end(), hand.begin(), hand.end());
        hand.clear();
    }
};

// ================= 몬스터 =================
class Monster {
public:
    int hp = 60;
    int attack = 10;
    int poison = 0;

    void applyPoison() {
        if (poison > 0) {
            hp -= poison;
            cout << "Poison deals " << poison << " damage!\n";
            poison--;
        }
    }
};

// ================= 게임 =================
class Game {
public:
    Player player;
    Monster monster;
    int floor = 1;
    string playerName;

    void init() {
        for (int i = 0; i < 4; i++)
            player.deck.push_back(Card("Strike", ATTACK, 1, 8));

        for (int i = 0; i < 4; i++)
            player.deck.push_back(Card("Defend", DEFENSE, 1, 5));

        //  추가 카드
        player.deck.push_back(Card("Fireball", ATTACK, 2, 15));
        player.deck.push_back(Card("Heal", HEAL, 2, 10));
        player.deck.push_back(Card("Energy Boost", ENERGY, 0, 2));
        player.deck.push_back(Card("Draw", DRAW, 1, 2));
        player.deck.push_back(Card("Poison Sting", POISON, 1, 3));
        player.deck.push_back(Card("Whirlwind", AOE, 2, 10));

        player.shuffleDeck();
    }

    void showStatus() {
        cout << "\nHP: " << player.hp
            << " | Block: " << player.block
            << " | Energy: " << player.energy
            << " | Gold: " << player.gold
            << " | Floor: " << floor << endl;
        cout << "Monster HP: " << monster.hp << " (Poison: " << monster.poison << ")\n";
    }

    void showHand() {
        cout << "\n[Hand]\n";
        for (int i = 0; i < player.hand.size(); i++) {
            string typeStr;

            switch (player.hand[i].type) {
            case ATTACK: typeStr = "ATK"; break;
            case DEFENSE: typeStr = "DEF"; break;
            case HEAL: typeStr = "HEAL"; break;
            case ENERGY: typeStr = "ENG"; break;
            case DRAW: typeStr = "DRAW"; break;
            case POISON: typeStr = "POISON"; break;
            case AOE: typeStr = "AOE"; break;
            }

            cout << i << ": " << player.hand[i].name
                << " [" << typeStr << "]"
                << " (Cost:" << player.hand[i].cost
                << ", Value:" << player.hand[i].value << ")\n";
        }
        cout << "-1: End Turn\n";
    }

    void playCard(int idx) {
        if (idx < 0 || idx >= player.hand.size()) return;

        Card& c = player.hand[idx];

        if (player.energy < c.cost) {
            cout << "Not enough energy!\n";
            return;
        }

        player.energy -= c.cost;

        switch (c.type) {
        case ATTACK:
            monster.hp -= c.value;
            cout << c.name << " deals " << c.value << " damage!\n";
            break;

        case DEFENSE:
            player.block += c.value;
            cout << c.name << " gives " << c.value << " block!\n";
            break;

        case HEAL:
            player.hp = min(player.hp + c.value, 100);
            cout << c.name << " heals " << c.value << " HP!\n";
            break;

        case ENERGY:
            player.energy += c.value;
            cout << c.name << " gains " << c.value << " energy!\n";
            break;

        case DRAW:
            player.draw(c.value);
            cout << c.name << " draws " << c.value << " cards!\n";
            break;

        case POISON:
            monster.poison += c.value;
            cout << c.name << " applies " << c.value << " poison!\n";
            break;

        case AOE:
            monster.hp -= c.value;
            cout << c.name << " hits all enemies for " << c.value << "!\n";
            break;
        }

        player.discard.push_back(c);
        player.hand.erase(player.hand.begin() + idx);
    }

    Card getRandomCard() {
        vector<Card> pool = {
            Card("Strike+", ATTACK, 1, 12),
            Card("Big Defend", DEFENSE, 1, 8),
            Card("Mega Heal", HEAL, 2, 15),
            Card("Poison+", POISON, 1, 5),
            Card("Draw+", DRAW, 1, 3),
            Card("Energy+", ENERGY, 0, 3),
            Card("Explosion", AOE, 2, 14)
        };

        uniform_int_distribution<int> dist(0, pool.size() - 1);
        return pool[dist(player.g)];
    }

    void reward() {
        cout << "\nChoose a card reward:\n";

        vector<Card> choices;
        for (int i = 0; i < 3; i++) {
            Card c = getRandomCard();
            choices.push_back(c);
            cout << i << ": " << c.name << endl;
        }

        int pick;
        cin >> pick;

        if (pick >= 0 && pick < 3) {
            player.deck.push_back(choices[pick]);
            cout << choices[pick].name << " added to deck!\n";
        }
    }

    void battle() {
        monster.hp = 60 + floor * 10;
        monster.attack = 10 + floor * 2;
        monster.poison = 0;

        while (player.hp > 0 && monster.hp > 0) {
            player.resetTurn();
            player.draw(5);

            //  턴 시작 독 데미지
            monster.applyPoison();
            if (monster.hp <= 0) break;

            while (true) {
                showStatus();
                showHand();

                int choice;
                cin >> choice;

                if (choice == -1) break;

                playCard(choice);

                if (monster.hp <= 0) break;
            }

            player.endTurn();

            if (monster.hp <= 0) break;

            int damage = max(0, monster.attack - player.block);
            player.hp -= damage;
            cout << "Monster attacks for " << damage << " damage!\n";
        }

        if (player.hp > 0) {
            cout << "Battle Won!\n";
            player.gold += 50;
            reward();  //  보상
            floor++;
        }
        else {
            cout << "You were defeated...\n";
        }
    }

    void run() {
        init();
        cout << "Enter your name: ";
        cin >> playerName;

        while (player.hp > 0) {
            battle();
            if (player.hp <= 0) break;

            cout << "Proceed to next floor? (y/n): ";
            char next;
            cin >> next;
            if (next != 'y') break;
        }

        if (player.hp > 0)
            cout << "\nCongratulations! You cleared " << floor - 1 << " floors.\n";
        else
            cout << "\nGame Over at floor " << floor << ".\n";
    }
};

// ================= 실행 =================
int main() {
    Game game;
    game.run();
    return 0;
}
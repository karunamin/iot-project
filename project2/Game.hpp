#ifndef GAME_HPP
#define GAME_HPP

#include "Card.hpp"
#include <random>
#include <algorithm>
#include <fstream>
#include <filesystem>
#include <vector>
#include <iostream>

class Game {
public:
    // 플레이어 스탯
    int playerHP = 100;
    int maxPlayerHP = 100;
    int playerBlock = 0;
    int energy = 3;
    int maxEnergy = 3;

    // 몬스터 및 층 데이터
    int monsterHP = 80;
    int maxMonsterHP = 80;
    int monsterAttack = 10;
    int monsterBlock = 0;
    int floor = 0;
    bool isBossFloor = false;
    
    Button viewDeckBtn;
    GameState prevState; // 덱을 보기 전 상태를 저장 (BATTLE이나 REWARD 등)

    GameState state = GameState::MENU;
    MonsterIntent nextIntent;

    // 데이터 컨테이너
    std::vector<Card> masterDeck, drawPile, hand, discardPile, rewardOptions, rewardPool;
    std::vector<Relic> masterRelics, shopRelics, relicPool;
    std::vector<RankEntry> highScores;

    sf::Font font;
    sf::Texture cardTexture;
    Button startBtn, rankBtn, backBtn, endTurnBtn;
    std::mt19937 rng;

    Game();
    void init();
    void initRelics();
    void generateShop();
    void applyRelicEffect(Relic& r); // 유물 효과 처리 함수
    Card createCardData(std::string name, CardType type, int cost, int value);
    void setNextIntent();
    void drawCards(int count);
    void playCard(int i);
    void monsterTurn();
    void generateRewards();
    void startNextFloor();
    void saveScore();
    void loadRanking();
    void renderDeck(sf::RenderWindow& window, sf::Text& cardText); // 덱 렌더링 함수
};

#endif
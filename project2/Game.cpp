#include "Game.hpp"
#include <iostream>

Game::Game() : rng(std::random_device{}()), startBtn(font), rankBtn(font), backBtn(font), endTurnBtn(font), viewDeckBtn(font) {}

void Game::init() {
    // 1. 폰트 로드
    if (!font.openFromFile("NanumGothicCoding.ttf")) std::cout << "Font missing!" << std::endl;

    // 2. 카드 배경 이미지 로드 (중요!)
    if (!cardTexture.loadFromFile("assets/test1.png")) {
        std::cout << "Error: test1.png not found!" << std::endl;
    }

    // 3. 버튼 설정
    startBtn.shape.setSize({ 250.f, 70.f });
    startBtn.shape.setPosition({ 375.f, 280.f });
    startBtn.shape.setFillColor(sf::Color(46, 204, 113));
    startBtn.text.setString("GAME START");
    startBtn.text.setCharacterSize(28);
    startBtn.text.setPosition({ 415.f, 295.f });

    rankBtn.shape.setSize({ 250.f, 70.f });
    rankBtn.shape.setPosition({ 375.f, 370.f });
    rankBtn.shape.setFillColor(sf::Color(52, 152, 219));
    rankBtn.text.setString("RANKING");
    rankBtn.text.setCharacterSize(28);
    rankBtn.text.setPosition({ 440.f, 385.f });

    backBtn.shape.setSize({ 120.f, 50.f });
    backBtn.shape.setPosition({ 30.f, 30.f });
    backBtn.shape.setFillColor(sf::Color(149, 165, 166));
    backBtn.text.setString("BACK");
    backBtn.text.setCharacterSize(20);
    backBtn.text.setPosition({ 65.f, 42.f });

    endTurnBtn.shape.setSize({ 160.f, 60.f });
    endTurnBtn.shape.setPosition({ 800.f, 580.f });
    endTurnBtn.shape.setFillColor(sf::Color(150, 50, 50));
    endTurnBtn.text.setString("TURN END");
    endTurnBtn.text.setCharacterSize(22);
    endTurnBtn.text.setPosition({ 820.f, 595.f });

    rewardPool.push_back(createCardData("Heavy Strike", CardType::ATTACK, 2, 25));
    rewardPool.push_back(createCardData("Iron Wall", CardType::DEFENSE, 2, 20));
    rewardPool.push_back(createCardData("Quick Regen", CardType::HEAL, 1, 15));
    rewardPool.push_back(createCardData("Double Slash", CardType::ATTACK, 1, 12));
    rewardPool.push_back(createCardData("Holy Shield", CardType::DEFENSE, 1, 10));
    rewardPool.push_back(createCardData("Life Drain", CardType::HEAL, 2, 12));
    rewardPool.push_back(createCardData("Berserk", CardType::ATTACK, 3, 40));

    // 덱 보기 버튼 (우측 상단 혹은 적절한 위치)
    viewDeckBtn.shape.setSize({ 150.f, 50.f });
    viewDeckBtn.shape.setPosition({ 820.f, 480.f });
    viewDeckBtn.shape.setFillColor(sf::Color(100, 100, 100, 200));
    viewDeckBtn.text.setString("VIEW DECK");
    viewDeckBtn.text.setCharacterSize(20);
    viewDeckBtn.text.setPosition({ 840.f, 492.f });

    initRelics();
    loadRanking();

}

void Game::saveScore() {
    if (!std::filesystem::exists("save")) {
        std::filesystem::create_directory("save");
    }

    std::ofstream outFile("save/ranking.txt", std::ios::app);
    if (outFile.is_open()) {
        outFile << floor << "\n";
        outFile.close();

        // 중요: 저장 후 즉시 리스트를 새로고침합니다.
        loadRanking();
    }
}

void Game::loadRanking() {
    highScores.clear();
    std::ifstream inFile("save/ranking.txt");
    if (!inFile.is_open()) return; // 파일이 없으면 그냥 리턴

    int f;
    while (inFile >> f) {
        highScores.push_back({ f });
    }

    // 내림차순 정렬 (높은 층수가 위로)
    std::sort(highScores.begin(), highScores.end(), [](const RankEntry& a, const RankEntry& b) {
        return a.floor > b.floor;
        });

    // 상위 5개만 유지
    if (highScores.size() > 5) highScores.resize(5);

    inFile.close();
}

Card Game::createCardData(std::string name, CardType type, int cost, int value) {
    Card c; c.name = name; c.type = type; c.cost = cost; c.value = value;
    c.shape.setSize({ 120.f, 170.f });
    c.shape.setTexture(&cardTexture); // 이미지 적용
    c.shape.setFillColor(sf::Color::White);
    c.shape.setOutlineThickness(2.f);
    c.shape.setOutlineColor(sf::Color::White);
    return c;
}

void Game::playCard(int i) {
    if (state != GameState::BATTLE) return;
    Card& c = hand[i];

    if (energy >= c.cost) {
        energy -= c.cost;

        if (c.type == CardType::ATTACK) {
            int dmg = c.value; // 공격력

            // 몬스터의 방어도가 공격력보다 높거나 같을 때
            if (monsterBlock >= dmg) {
                monsterBlock -= dmg;
            }
            // 몬스터의 방어도가 공격력보다 낮을 때 (방어도를 다 깎고 남은 데미지가 HP에 적용)
            else {
                dmg -= monsterBlock; // 방어도만큼 데미지 상쇄
                monsterBlock = 0;    // 방어도는 0이 됨
                monsterHP -= dmg;    // 남은 데미지 적용
            }
        }
        else if (c.type == CardType::DEFENSE) {
            playerBlock += c.value;
        }
        else if (c.type == CardType::HEAL) {
            // playerHP = std::min(100, playerHP + c.value); // (기존: 100 고정)
            playerHP = std::min(maxPlayerHP, playerHP + c.value); // (수정: 변수 사용)
        }

        // 카드 사용 후 처리
        discardPile.push_back(hand[i]);
        hand.erase(hand.begin() + i);

        // 손패 위치 재정렬
        for (int j = 0; j < (int)hand.size(); j++)
            hand[j].shape.setPosition({ 80.f + j * 140.f, 400.f });

        // 승리 판정
        if (monsterHP <= 0) {
            monsterHP = 0;
            generateRewards();
        }
    }
}

void Game::generateRewards() {
    rewardOptions.clear();
    state = GameState::REWARD;

    // 1. 전체 보상 풀을 복사해서 섞기
    std::vector<Card> tempPool = rewardPool;
    std::shuffle(tempPool.begin(), tempPool.end(), rng);

    // 2. 섞인 리스트에서 상위 3개만 선택
    for (int i = 0; i < 3 && i < (int)tempPool.size(); i++) {
        rewardOptions.push_back(tempPool[i]);
    }

    // 3. 화면 배치 (일반 카드와 동일한 스타일 적용)
    for (int i = 0; i < (int)rewardOptions.size(); i++) {
        rewardOptions[i].shape.setSize({ 120.f, 170.f }); // 크기 조절
        rewardOptions[i].shape.setPosition({ 200.f + i * 220.f, 250.f });
        rewardOptions[i].shape.setOutlineColor(sf::Color::Yellow);
        rewardOptions[i].shape.setOutlineThickness(3.f);
    }
}

void Game::monsterTurn() {
    monsterBlock = 0;
    if (nextIntent == MonsterIntent::ATTACK) playerHP -= std::max(0, monsterAttack - playerBlock);
    else if (nextIntent == MonsterIntent::STRONG_ATTACK) playerHP -= std::max(0, (int)(monsterAttack * 1.5f) - playerBlock);
    else if (nextIntent == MonsterIntent::DEFEND) monsterBlock += (10 + floor);
    else if (nextIntent == MonsterIntent::BUFF) monsterAttack += 2;

    playerBlock = 0;
    energy = maxEnergy;
    discardPile.insert(discardPile.end(), hand.begin(), hand.end()); hand.clear();

    if (playerHP <= 0) {
        state = GameState::GAMEOVER;
        saveScore(); loadRanking();
    }
    else {
        setNextIntent(); drawCards(5);
    }
}

void Game::setNextIntent() {
    std::uniform_int_distribution<int> dist(0, 3);
    int action = dist(rng);
    if (action == 0) nextIntent = MonsterIntent::ATTACK;
    else if (action == 1) nextIntent = MonsterIntent::DEFEND;
    else if (action == 2) nextIntent = MonsterIntent::BUFF;
    else nextIntent = MonsterIntent::STRONG_ATTACK;
}

void Game::drawCards(int count) {
    for (int i = 0; i < count; i++) {
        if (drawPile.empty()) {
            drawPile = discardPile; discardPile.clear();
            std::shuffle(drawPile.begin(), drawPile.end(), rng);
            if (drawPile.empty()) break;
        }
        hand.push_back(drawPile.back()); drawPile.pop_back();
    }
    for (int i = 0; i < (int)hand.size(); i++) hand[i].shape.setPosition({ 80.f + i * 140.f, 400.f });
}

void Game::initRelics() {
    // 기존에 생성된 유물 목록을 비웁니다.
    relicPool.clear();

    // 1. 공격력 관련 유물
    Relic r1;
    r1.name = "Red Stone";
    r1.description = "Monster Atk -5";
    r1.type = RelicType::POWER_UP;
    r1.shape.setSize({ 100.f, 100.f });
    r1.shape.setFillColor(sf::Color::Red); // 빨간색 사각형으로 표시

    // 2. 최대 체력 관련 유물
    Relic r2;
    r2.name = "Health Gem";
    r2.description = "Max HP +20";
    r2.type = RelicType::DEFENSE_UP;
    r2.shape.setSize({ 100.f, 100.f });
    r2.shape.setFillColor(sf::Color::Green); // 초록색 사각형

    // 3. 에너지 관련 유물
    Relic r3;
    r3.name = "Battery";
    r3.description = "Max Energy +1";
    r3.type = RelicType::ENERGY_UP;
    r3.shape.setSize({ 100.f, 100.f });
    r3.shape.setFillColor(sf::Color::Blue); // 파란색 사각형

    // 풀(Pool)에 유물들을 담아둡니다.
    relicPool.push_back(r1);
    relicPool.push_back(r2);
    relicPool.push_back(r3);
}

void Game::generateShop() {
    state = GameState::SHOP;
    shopRelics.clear();

    std::shuffle(relicPool.begin(), relicPool.end(), rng);
    for (int i = 0; i < 2 && i < (int)relicPool.size(); i++) {
        Relic r = relicPool[i];
        r.shape.setPosition({ 300.f + i * 250.f, 300.f });
        shopRelics.push_back(r);
    }
}



void Game::startNextFloor() {
    // 게임 시작 시 덱 초기화 (0층에서 1층 갈 때)
    if (floor == 0) {
        masterDeck.clear();
        for (int i = 0; i < 5; i++) masterDeck.push_back(createCardData("Strike", CardType::ATTACK, 1, 60));
        for (int i = 0; i < 5; i++) masterDeck.push_back(createCardData("Defend", CardType::DEFENSE, 1, 5));
    }

    floor++; // 층수 증가

    // --- 1. 상점층 판정 (5, 15, 25... 층) ---
    if (floor % 5 == 0 && floor % 10 != 0) {
        generateShop(); // 상점 상태(GameState::SHOP)로 전환 및 유물 생성
        return;         // 상점으로 빠지므로 아래 전투 준비 로직은 실행하지 않음
    }

    // --- 2. 전투 준비 (일반 또는 보스) ---
    if (floor % 10 == 0) {
        isBossFloor = true;
        maxMonsterHP = 180 + (floor * 20);
        monsterAttack = 14 + (floor);
    }
    else {
        isBossFloor = false;
        maxMonsterHP = 80 + (floor * 10);
        monsterAttack = 9 + (floor);
    }

    // 전투 셋팅 초기화
    monsterHP = maxMonsterHP;
    energy = maxEnergy;
    playerBlock = 0;
    monsterBlock = 0;

    // 덱 섞기 및 핸드 드로우
    drawPile = masterDeck;
    discardPile.clear();
    hand.clear();
    std::shuffle(drawPile.begin(), drawPile.end(), rng);

    setNextIntent();    // 몬스터 의도 결정
    state = GameState::BATTLE; // 상태를 전투로 변경
    drawCards(5);       // 카드 5장 뽑기
}

void Game::applyRelicEffect(Relic& r) {
    if (r.type == RelicType::POWER_UP) {
        this->monsterAttack -= 5; // 몬스터 공격력 약화
    }
    else if (r.type == RelicType::DEFENSE_UP) {
        this->maxPlayerHP += 20;  // 최대 체력 한도 증가
        this->playerHP += 20;     // 현재 체력도 보너스로 증가
    }
    else if (r.type == RelicType::ENERGY_UP) {
        this->maxEnergy += 1;     // 최대 에너지 한도 증가
        this->energy += 1;        // 즉시 에너지 보너스
    }
}

void Game::renderDeck(sf::RenderWindow& window, sf::Text& cardText) {
    sf::RectangleShape dim({ 1000.f, 700.f });
    dim.setFillColor(sf::Color(0, 0, 0, 220)); // 배경 흐리게
    window.draw(dim);

    for (int i = 0; i < (int)masterDeck.size(); i++) {
        float x = 100.f + (i % 6) * 140.f; // 한 줄에 6장씩
        float y = 100.f + (i / 6) * 190.f;

        masterDeck[i].shape.setPosition({ x, y });
        window.draw(masterDeck[i].shape);

        cardText.setString(masterDeck[i].name + "\nCost: " + std::to_string(masterDeck[i].cost));
        cardText.setPosition({ x + 10.f, y + 15.f });
        cardText.setCharacterSize(16);
        window.draw(cardText);
    }

    // 돌아가기 안내
    sf::Text tip(font, "Click BACK button to return", 20);
    tip.setPosition({ 400.f, 20.f });
    window.draw(tip);
}
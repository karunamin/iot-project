#include "Game.hpp"
#include <sstream>

int main() {
    sf::RenderWindow window(sf::VideoMode({ 1000, 700 }), "TESTETES");
    window.setFramerateLimit(60);
    Game game;
    game.init();

    sf::Text uiText(game.font), cardText(game.font);

    while (window.isOpen()) {
        sf::Vector2f mPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();

            if (const auto* mBtn = event->getIf<sf::Event::MouseButtonPressed>()) {
                // --- 마우스 클릭 이벤트 처리 섹션 ---
                if (game.state == GameState::MENU) {
                    if (game.startBtn.shape.getGlobalBounds().contains(mPos)) game.startNextFloor();
                    if (game.rankBtn.shape.getGlobalBounds().contains(mPos)) game.state = GameState::RANKING;
                }
                else if (game.state == GameState::RANKING) {
                    if (game.backBtn.shape.getGlobalBounds().contains(mPos)) game.state = GameState::MENU;
                }
                else if (game.state == GameState::BATTLE) {
                    for (int i = 0; i < (int)game.hand.size(); i++) {
                        if (game.hand[i].shape.getGlobalBounds().contains(mPos)) {
                            game.playCard(i);
                            break;
                        }
                    }
                    if (game.endTurnBtn.shape.getGlobalBounds().contains(mPos)) game.monsterTurn();
                }
                else if (game.state == GameState::REWARD) {
                    for (int i = 0; i < (int)game.rewardOptions.size(); i++) {
                        // 클릭한 위치(mPos)가 보상 카드의 영역 안에 있는지 확인
                        if (game.rewardOptions[i].shape.getGlobalBounds().contains(mPos)) {
                            // 마스터 덱에 추가
                            game.masterDeck.push_back(game.rewardOptions[i]);
                            // 다음 층으로 이동
                            game.startNextFloor();
                            break;
                        }
                    }
                }
                else if (game.state == GameState::GAMEOVER) {
                    // 게임오버 시 아무데나 클릭하면 메뉴로 초기화하여 돌아감
                    game.state = GameState::MENU;
                    game.floor = 0;
                    game.playerHP = 100;
                }
                else if (game.state == GameState::SHOP) {
                    for (int i = 0; i < (int)game.shopRelics.size(); i++) {
                        if (game.shopRelics[i].shape.getGlobalBounds().contains(mPos)) {
                            // 1. 유물 효과 적용
                            game.applyRelicEffect(game.shopRelics[i]);

                            // 2. 마스터 유물 리스트에 추가 (보유 목록 확인용)
                            game.masterRelics.push_back(game.shopRelics[i]);

                            // 3. 다음 층으로 이동
                            game.startNextFloor();
                            break;
                        }
                    }
                }
                // 1. 덱 보기 버튼 클릭 (전투나 보상 화면에서 노출)
                if ((game.state == GameState::BATTLE || game.state == GameState::REWARD) &&
                    game.viewDeckBtn.shape.getGlobalBounds().contains(mPos)) {
                    game.prevState = game.state; // 현재 상태 저장
                    game.state = GameState::VIEW_DECK;
                }

                // 2. 덱 화면에서 돌아가기 (기존 backBtn 활용)
                else if (game.state == GameState::VIEW_DECK) {
                    if (game.backBtn.shape.getGlobalBounds().contains(mPos)) {
                        game.state = game.prevState; // 이전 상태로 복구
                    }
                }
            }
        }

        // --- 화면 그리기(Rendering) 섹션 ---
        window.clear(sf::Color(30, 39, 46));

        if (game.state == GameState::MENU) {
            sf::Text title(game.font, "TESTETES", 60);
            title.setPosition({ 380, 120 }); title.setFillColor(sf::Color::Yellow);
            window.draw(title);
            window.draw(game.startBtn.shape); window.draw(game.startBtn.text);
            window.draw(game.rankBtn.shape); window.draw(game.rankBtn.text);
        }
        else if (game.state == GameState::BATTLE) {
            // 1. 플레이어 상태 표시 (에너지 등)
            std::stringstream ss;
            ss << "PLAYER HP: " << game.playerHP << " / " << game.maxPlayerHP
                << " | BLOCK: " << game.playerBlock << "\nENERGY: " << game.energy;
            uiText.setString(ss.str());
            uiText.setPosition({ 30, 30 });
            uiText.setFillColor(sf::Color::Cyan);
            window.draw(uiText);

            // 2. 몬스터 정보 및 층수(Floor) 표시 --------------------------------------
            // 층수 텍스트 생성 (몬스터 HP 위쪽에 배치)
            sf::Text floorText(game.font, "FLOOR " + std::to_string(game.floor), 30);
            floorText.setPosition({ 650, 10 }); // 몬스터 HP(50)보다 위쪽인 10에 배치
            floorText.setFillColor(sf::Color::Yellow);
            if (game.isBossFloor) floorText.setFillColor(sf::Color(255, 100, 100)); // 보스층이면 색상 변경
            window.draw(floorText);

            // 몬스터 HP 및 방어도 정보
            sf::Text mInfo(game.font,
                "MONSTER HP: " + std::to_string(game.monsterHP) +
                " | BLOCK: " + std::to_string(game.monsterBlock), 24);
            mInfo.setPosition({ 650, 50 });
            mInfo.setFillColor(sf::Color::Red);
            window.draw(mInfo);


            for (auto& c : game.hand) {
                window.draw(c.shape);
                cardText.setString(c.name + "\nCost: " + std::to_string(c.cost) + "\nVal: " + std::to_string(c.value));
                cardText.setPosition(c.shape.getPosition() + sf::Vector2f(10, 15));
                cardText.setCharacterSize(18); cardText.setFillColor(sf::Color::White);
                window.draw(cardText);
            }
            window.draw(game.endTurnBtn.shape); window.draw(game.endTurnBtn.text);

            // 몬스터 의도(Intent) 표시
            std::string intentStr;
            sf::Color intentColor;
            switch (game.nextIntent) {
            case MonsterIntent::ATTACK: intentStr = "Next: ATTACK (" + std::to_string(game.monsterAttack) + ")"; intentColor = sf::Color::Red; break;
            case MonsterIntent::STRONG_ATTACK: intentStr = "Next: HEAVY ATK (" + std::to_string(static_cast<int>(game.monsterAttack * 1.5f)) + ")"; intentColor = sf::Color(255, 69, 0); break;
            case MonsterIntent::DEFEND: intentStr = "Next: DEFEND"; intentColor = sf::Color::Blue; break;
            case MonsterIntent::BUFF: intentStr = "Next: POWER UP"; intentColor = sf::Color::Magenta; break;
            }
            sf::Text intentT(game.font, intentStr, 22);
            intentT.setPosition({ 650.f, 90.f });
            intentT.setFillColor(intentColor);
            window.draw(intentT);
        }
        else if (game.state == GameState::REWARD) {
            sf::RectangleShape dim({ 1000.f, 700.f });
            dim.setFillColor(sf::Color(0, 0, 0, 180));
            window.draw(dim);

            sf::Text winT(game.font, "VICTORY! CHOOSE A REWARD", 40);
            winT.setPosition({ 230, 150 });
            winT.setFillColor(sf::Color::Yellow);
            window.draw(winT);

            for (auto& c : game.rewardOptions) {
                // 1. 카드 배경(이미지 포함) 그리기
                window.draw(c.shape);

                // 2. 카드 텍스트 형식을 일반 카드와 동일하게 설정
                cardText.setString(c.name + "\nCost: " + std::to_string(c.cost) + "\nVal: " + std::to_string(c.value));

                // 카드 배경 위치에 맞춰 텍스트 위치 조정
                cardText.setPosition(c.shape.getPosition() + sf::Vector2f(10, 15));
                cardText.setCharacterSize(18);
                cardText.setFillColor(sf::Color::White);
                window.draw(cardText);
            }
        }
        else if (game.state == GameState::RANKING) {
            sf::Text title(game.font, "TOP 5 RANKING", 50);
            title.setPosition({ 330.f, 80.f });
            title.setFillColor(sf::Color::Yellow);
            window.draw(title);
            window.draw(game.backBtn.shape);
            window.draw(game.backBtn.text);

            if (game.highScores.empty()) {
                sf::Text emptyT(game.font, "No Records Yet", 30);
                emptyT.setPosition({ 380.f, 300.f });
                window.draw(emptyT);
            }
            else {
                for (int i = 0; i < (int)game.highScores.size(); i++) {
                    float yPos = 200.f + (static_cast<float>(i) * 60.f);
                    sf::Text line(game.font, std::to_string(i + 1) + ".  FLOOR  " + std::to_string(game.highScores[i].floor), 35);
                    line.setPosition({ 350.f, yPos });
                    line.setFillColor(sf::Color::White);
                    window.draw(line);
                }
            }
        }
        else if (game.state == GameState::GAMEOVER) {
            // --- 게임오버 전용 렌더링 ---
            sf::RectangleShape dim({ 1000.f, 700.f });
            dim.setFillColor(sf::Color(0, 0, 0, 200));
            window.draw(dim);

            sf::Text goT(game.font, "GAME OVER", 70);
            goT.setPosition({ 320.f, 200.f });
            goT.setFillColor(sf::Color::Red);
            window.draw(goT);

            sf::Text scoreT(game.font, "You reached Floor: " + std::to_string(game.floor), 30);
            scoreT.setPosition({ 360.f, 320.f });
            scoreT.setFillColor(sf::Color::White);
            window.draw(scoreT);

            sf::Text retryT(game.font, "Click anywhere to Menu", 20);
            retryT.setPosition({ 380.f, 450.f });
            window.draw(retryT);
        }
        else if (game.state == GameState::SHOP) {
            sf::Text shopT(game.font, "MYSTERIOUS RELIC SHOP", 45);
            shopT.setPosition({ 250, 100 });
            shopT.setFillColor(sf::Color::Magenta);
            window.draw(shopT);

            for (auto& r : game.shopRelics) {
                window.draw(r.shape);

                sf::Text rText(game.font, r.name + "\n" + r.description, 18);
                rText.setPosition(r.shape.getPosition() + sf::Vector2f(0, 110));
                rText.setFillColor(sf::Color::White);
                window.draw(rText);
            }
        }

        if (game.state == GameState::BATTLE || game.state == GameState::REWARD) {
            window.draw(game.viewDeckBtn.shape);
            window.draw(game.viewDeckBtn.text);
        }

        // 덱 보기 화면 렌더링
        if (game.state == GameState::VIEW_DECK) {
            game.renderDeck(window, cardText);
            window.draw(game.backBtn.shape); // 돌아가기 버튼 재활용
            window.draw(game.backBtn.text);
        }

        window.display();
    }
    return 0;
}
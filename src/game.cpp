#include "game.h"

#include <iostream>

#include "SFML/Graphics.hpp"
#include "SFML/System/Clock.hpp"

#include "GameObjects/ball.h"
#include "GameObjects/bricks.h"
#include "GameObjects/paddle.h"

#include "collisionManager.h"
#include "lifeManager.h"

extern int offset;

namespace game
{
static int activeBricks = quantX * quantY;

void updateLife(int& life, Ball& ball, Paddle paddle, int wHeight);

void loop()
{
    Ball ball;
    Brick bricks[quantY][quantX];
    Paddle paddle;
    int life = 3;

    sf::Text lifeText;
    sf::Font font;
    if (!font.loadFromFile("res/PermanentMarker-Regular.ttf"))
        std::cout << "Failed to load font\n";
    lifeText.setFont(font);
    lifeText.setPosition(10, 10);
    lifeText.setCharacterSize(40);
    lifeText.setFillColor(sf::Color::White);

    const int windowWidth = 1024;
    const int windowHeight = 768;

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Daniela Gonzalez");
    sf::Clock clock;

    srand(time(NULL));
    initBall(ball, 800, 500);
    initBrick(bricks);
    initPaddle(paddle, windowWidth, windowHeight);

    while (window.isOpen())
    {
        sf::Time dt = clock.restart();
        updateBall(ball, dt, windowWidth, windowHeight);
        updatePaddle(paddle, windowWidth, dt);
        updateLife(life, ball, paddle, windowHeight);
        lifeText.setString("Lives: " + std::to_string(life));

        if (getBallToPaddCollision(ball, paddle))
        {
            ball.y = paddle.y - paddle.sprite.getGlobalBounds().height - offset;
            ball.dirY *= -1;
            ball.speedX = (ball.x - paddle.x) / (paddle.sprite.getGlobalBounds().width) * 5;
        }

        checkBallToBrickCollision(ball, bricks, activeBricks);

        sf::Event event;

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }


        window.clear();
        for (int i = 0; i < quantY; i++)
        {
            for (int j = 0; j < quantX; j++)
            {
                window.draw(bricks[i][j].rect);
            }
        }

        window.draw(paddle.sprite);
        window.draw(ball.sprite);
        window.draw(lifeText);
        window.display();
    }
}

void updateLife(int& life, Ball& ball, Paddle paddle, int wHeight)
{
    if (ball.y >= wHeight)
    {
        restartBall(ball, paddle.x, wHeight / 2);
        decreasePlayerLife(life);
    }
}
}
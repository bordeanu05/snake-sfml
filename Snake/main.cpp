/// Code written by Bordeanu Calin

#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <vector>
#include <windows.h>

#define BLOCKS 40
#define blockSize 16

const int WIDTH = BLOCKS * blockSize;
const int HEIGHT = BLOCKS * blockSize;

sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Snake");

bool gameOver;

void SetCoinPos(sf::RectangleShape &coin)
{
    int x = rand()%BLOCKS;
    int y = rand()%BLOCKS;
    coin.setPosition({x*blockSize, y*blockSize});
}

class Snake
{
public:
    std::vector<sf::RectangleShape> body;
    int length = 0;

    sf::Vector2f dir = {0, -1};
/*
     0, -1 -> sus
     0,  1 -> jos
     1,  0 -> dreapta
    -1,  0 -> stanga
*/

    void Move()
    {
        sf::RectangleShape &head = body[0];

        int x = head.getPosition().x/blockSize;
        int y = head.getPosition().y/blockSize;

        for(int i = body.size()-1; i>0; --i)
        {
            body[i].setPosition(body[i-1].getPosition());
        }

        if(dir.x==0 && dir.y==-1){
            head.setPosition({x*blockSize, (y-1)*blockSize});
        }
        if(dir.x==0 && dir.y==1){
            head.setPosition({x*blockSize, (y+1)*blockSize});
        }
        if(dir.x==1 && dir.y==0){
            head.setPosition({(x+1)*blockSize, y*blockSize});
        }
        if(dir.x==-1 && dir.y==0){
            head.setPosition({(x-1)*blockSize, y*blockSize});
        }
    }

    void AddTail()
    {
        sf::RectangleShape newPart;
        newPart.setFillColor(sf::Color::White);
        newPart.setSize({blockSize, blockSize});
        newPart.setOutlineThickness(1);
        newPart.setOutlineColor(sf::Color::Black);

        sf::RectangleShape lastPart = body.back();

        int x = lastPart.getPosition().x;
        int y = lastPart.getPosition().y;

        newPart.setPosition({x, y});

        body.push_back(newPart);
    }

    void CheckBounds()
    {
        sf::RectangleShape &head = body[0];

        int x = head.getPosition().x/blockSize;
        int y = head.getPosition().y/blockSize;

        if(x>BLOCKS) x = 1;
        if(x<0) x = BLOCKS;
        if(y>BLOCKS) y = 0;
        if(y<0) y = BLOCKS;

        x*=blockSize;
        y*=blockSize;

        head.setPosition({x, y});
    }

    void CheckCollision()
    {
        for(int i = 1; i<body.size(); ++i){
            int x1 = body[0].getPosition().x;
            int x2 = body[i].getPosition().x;

            int y1 = body[0].getPosition().y;
            int y2 = body[i].getPosition().y;

            if(x1==x2 && y1==y2){
                gameOver = true;
                std::cout << "Game over! Press 'R' to restart";
            }
        }
    }

    void CheckForCoin(sf::RectangleShape &coin)
    {
        if(body[0].getGlobalBounds().intersects(coin.getGlobalBounds())){
            AddTail();
            SetCoinPos(coin);
            std::cout << "Score: " << ++length << '\n';
        }
    }

    void ChangeDir()
    {
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && dir.y!=1)     dir = {0, -1};
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && dir.y!=-1)  dir = {0, 1};
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && dir.x!=1)   dir = {-1, 0};
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && dir.x!=-1) dir = {1, 0};
    }

    void Draw()
    {
        for(int i = 0; i<body.size(); ++i)
            window.draw(body[i]);
    }

    Snake()
    {
        sf::RectangleShape head;

        head.setFillColor(sf::Color(255, 100, 100));
        head.setSize({blockSize, blockSize});
        head.setPosition({BLOCKS/2*blockSize, BLOCKS/2*blockSize});
        body.push_back(head);
    }
};

int main()
{
    srand((time(0)));
    gameOver = false;
    system("cls");

    Snake snake;

    sf::Clock clock;
    float t = 0;

    sf::RectangleShape coin;
    coin.setFillColor(sf::Color::Yellow);
    coin.setSize({blockSize, blockSize});
    SetCoinPos(coin);

    while(window.isOpen())
    {
        sf::Event e;
        while(window.pollEvent(e)){
            if(e.type == sf::Event::Closed) window.close();
            if(gameOver && sf::Keyboard::isKeyPressed(sf::Keyboard::R)) main();
        }

        sf::Time time = clock.restart();
        t+=time.asSeconds();

        if(!gameOver)
        {
            snake.ChangeDir();
            if(t>0.1){
                t = 0;
                snake.Move();
                snake.CheckCollision();
                snake.CheckBounds();
                snake.CheckForCoin(coin);
            }
        }

        window.clear(sf::Color(0, 0, 0));
        snake.Draw();
        window.draw(coin);

        window.display();
    }
}

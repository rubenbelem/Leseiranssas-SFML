#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <stdlib.h>
#include <time.h>
#include <SFML/System.hpp>
#define SCREEN_HEIGHT 768.0f
#define SCREEN_WIDTH 1024.0f
#define IS_KEY_PRESSED(KEY) sf::Keyboard::isKeyPressed(sf::Keyboard::KEY)

enum class Direction {
    LEFT = -1,
    RIGHT = 1,
    UP = -1,
    DOWN = 1
};

#include <iostream>
#include <cmath>
using std::cout;

void checkLimits(sf::CircleShape& shape) {
    sf::Vector2f position = shape.getPosition();
    int d = shape.getRadius() * 2;

    if (position.x + d > SCREEN_WIDTH) {
        position.x = SCREEN_WIDTH - d;
    }
    else if (position.x < 0.0f) {
        position.x = 0.0f;
    }

    if (position.y + d > SCREEN_HEIGHT) {
        position.y = SCREEN_HEIGHT - d;
    }
    else if (position.y < 0.0f) {
        position.y = 0.0f;
    }

    shape.setPosition(position);

    //cout << shape.getPosition().x << " : " << shape.getPosition().y << '\n';
 }

 void checkLimitsEnemy(sf::CircleShape& shape, sf::Vector2f& speed) {
    sf::Vector2f position = shape.getPosition();
    int d = shape.getRadius() * 2;

    if (position.x + d > SCREEN_WIDTH) {
        position.x = SCREEN_WIDTH - d;
        speed.x *= -1;
    }
    else if (position.x < 0.0f) {
        position.x = 0.0f;
        speed.x *= -1;
    }

    if (position.y + d > SCREEN_HEIGHT) {
        position.y = SCREEN_HEIGHT - d;
        speed.y *= -1;
    }
    else if (position.y < 0.0f) {
        position.y = 0.0f;
        speed.y *= -1;
    }

    shape.setPosition(position);
 }

class Player {
    private:
        sf::Vector2<Direction> m_dir;
        const static float SPD_LIMIT;
        const static float ACCEL;

    public:
        sf::CircleShape draw;
        sf::Vector2f m_speed;

        Player(): m_dir(Direction::RIGHT, Direction::UP), draw(15) {
            draw.setFillColor(sf::Color(240, 100, 50));
            draw.setPosition(0, 0);
        }

        static float friction(float speed) {
            auto abs = std::abs(speed);
            auto signal = speed < 0? -1 : 1;

            static const auto FRICTION = ACCEL;

            abs -= FRICTION;
            if (std::trunc(abs) < 0) abs = 0.f;

            return abs * signal;
        }

        void update() {
            sf::Vector2<bool> moving(true, true);

            if (IS_KEY_PRESSED(Right)) m_dir.x = Direction::RIGHT;
            else if (IS_KEY_PRESSED(Left)) m_dir.x = Direction::LEFT;
            else moving.x = false;

            if (IS_KEY_PRESSED(Down)) m_dir.y = Direction::DOWN;
            else if (IS_KEY_PRESSED(Up)) m_dir.y = Direction::UP;
            else moving.y = false;

            if (moving.x) {
                m_speed.x += (int) m_dir.x;

                if (m_speed.x > SPD_LIMIT) m_speed.x = SPD_LIMIT;
                if (m_speed.x < -SPD_LIMIT) m_speed.x = -SPD_LIMIT;
            }
            else if (m_speed.x) {
                m_speed.x = friction(m_speed.x);
            }

            if (moving.y) {
                m_speed.y += (int) m_dir.y;

                if (m_speed.y > SPD_LIMIT) m_speed.y = SPD_LIMIT;
                if (m_speed.y < -SPD_LIMIT) m_speed.y = -SPD_LIMIT;
            }
            else if (m_speed.y) {
                m_speed.y = friction(m_speed.y);
            }

            sf::Vector2f delta = m_speed;

            //cout << (int)m_dir.x << ' ' << (int)m_dir.y << "; " << m_speed.x << "; " << m_speed.y << '\n';

            draw.move(delta);
            checkLimits(draw);
        }
};

float calcDistance(sf::Vector2f p1, sf::Vector2f p2) {
    return sqrtf(powf(p1.x - p2.x, 2.0f) + powf(p1.y - p2.y, 2.0f));
}

class Enemy {
    private:
        sf::Vector2f m_position;
        int d;
    public:
        sf::Vector2f m_speed;
        sf::CircleShape draw;
        int id;

        Enemy(int p_id) : draw(10), id(p_id) {
            m_speed.x = (rand() % 5 + 1) * (rand() % 2 == 0? 1 : -1);
            m_speed.y = (rand() % 5 + 1) * (rand() % 2 == 0? 1 : -1);
            //sf::R
            sf::Vector2f v(rand() % 500, rand() % 700);
            draw.setPosition(v);
            draw.setFillColor(sf::Color(rand() % 200 + 50, rand() % 200 + 50, rand() % 200 + 50));
            d = draw.getRadius() * 2;
            //m_speed = speed
            //cout << m_speed.x << " : " << m_speed.y << '\n';
        }
        void update() {
            draw.move(m_speed);
            checkLimitsEnemy(draw, m_speed);
        }

        bool checkCollision(Enemy* en) {
            if (calcDistance(draw.getPosition(), en->draw.getPosition()) <= d) {
                float aux;
                aux = m_speed.x;
                m_speed.x = en->m_speed.x;
                en->m_speed.x = aux;

                aux = m_speed.y;
                m_speed.y = en->m_speed.y;
                en->m_speed.y = aux;
            }
        }

        bool checkCollisionPlayer(Player& en) {
            /*if (calcDistance(draw.getPosition(), en.draw.getPosition()) <= d) {
                m_speed.x = en.m_speed.x;
                m_speed.y = en.m_speed.y;
            }*/
        }
};

const float Player::ACCEL = 0.50f;
const float Player::SPD_LIMIT = 10.0f;

int main() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(1024, 768), "Minha janelita", sf::Style::Default, settings);
    window.setFramerateLimit(60);
    srand(time(NULL));
    std::vector<Enemy*> enemies;
    Player p1;
    sf::Clock clock;
    sf::Time elapsed;
    int n = 10;


    for (int i = 0; i < n; i++) {
        enemies.push_back(new Enemy(i));
    }
    while(window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed) {
                window.close();
            }
        }

        elapsed = clock.getElapsedTime();

        /*if (elapsed.asMilliseconds() > 50) {
            enemies.push_back(new Enemy);
            cout << ++n << '\n';
            clock.restart();
        }*/

        p1.update();
        for (Enemy* en : enemies) {

            for (Enemy* en2 : enemies) {
                if (en->id != en2->id) {
                    en->checkCollision(en2);
                    en->checkCollisionPlayer(p1);
                }
            }
            en->update();
        }
        window.clear();
        for (Enemy* en : enemies) {
            //en->update();
            window.draw(en->draw);
        }
        window.draw(p1.draw);
        window.display();
    }
}

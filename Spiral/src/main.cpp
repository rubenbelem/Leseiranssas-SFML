#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <math.h>

using namespace std;

class Point {
    public:
        sf::CircleShape draw;
        sf::Vector2f position;

        Point(int x, int y) : position(x, y), draw(4) {
            draw.setPosition(position);
            draw.setFillColor(sf::Color::White);
        }
};

class Spiral {
    private:
        float m_radius;
        float m_angle;
        float m_incAngle;
        float m_incRadius;
        sf::Vector2f m_center;
        vector<Point*> m_points;
        int m_nPoints;

    public:
        Spiral(int nPoints) : m_radius(0.01), m_angle(0), m_incAngle(0.0), m_incRadius(0.4), m_center(400, 300) {
            m_nPoints = nPoints;

            for (int i = 0; i < m_nPoints; i++) {
                m_points.push_back(new Point(m_center.x + cosf(m_angle) * m_radius, m_center.y + sinf(m_angle) * m_radius));
                m_radius += m_incRadius;
                m_angle += m_incAngle;
            }
        }
        void update() {
            m_radius = 1.0;
            m_angle = 1.0;

            for (Point* point : m_points) {
                point->draw.setPosition(m_center.x + sinf(m_angle) * m_radius, m_center.y + cosf(m_angle) * m_radius);
                //std::cout << "Angulo: " << m_angle << "\n";
                m_radius += m_incRadius;

                m_angle = m_angle + m_incAngle;
            }

            m_incAngle = (m_incAngle + 0.0001);
        }



        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
            for (Point* point : m_points) {
                target.draw(point->draw, states);
            }
        }
};



int main() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;

    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML works!", sf::Style::Default, settings);
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);
    Spiral spiral(400);

    window.setFramerateLimit(60);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        spiral.update();

        window.clear();

        //spiral.draw(window);
        window.draw(spiral);
        window.display();
    }

    return 0;
}

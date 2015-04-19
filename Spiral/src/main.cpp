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

        Point(sf::Vector2f vec) : position(vec), draw(4) {
            draw.setPosition(position);
            draw.setFillColor(sf::Color::White);
        }
};

class Spiral : public sf::Drawable {
    private:
        float m_radius;
        float m_angle;
        float m_incAngle;
        float m_incRadius;
        sf::Vector2f* m_center;
        vector<Point*> m_points;
        int m_nPoints;

        sf::Vector2f pointPositionUpdate() {
            sf::Vector2f newPosition(m_center->x + log10f(m_angle) / sinf(m_angle) * m_radius, m_center->y + ((sinf(m_angle) * log10f(m_angle)) / tanf(m_angle)) * m_radius);
            return newPosition;
        }

    public:
        Spiral(int nPoints, sf::Vector2f* center) : m_radius(0.01), m_angle(0), m_incAngle(0.0), m_incRadius(0.5), m_center(center) {
            m_nPoints = nPoints;

            for (int i = 0; i < m_nPoints; i++) {
                m_points.push_back(new Point(pointPositionUpdate()));
                m_radius += m_incRadius;
                m_angle += m_incAngle;
            }

            //for (int i = 0; i < m_nPoints - 1; i++) {

            //}
        }

        void update() {
            m_radius = 1.0;
            m_angle = 1.0;

            for (Point* point : m_points) {
                point->draw.setPosition(pointPositionUpdate());
                //std::cout << "Angulo: " << m_angle << "\n";
                m_radius += m_incRadius;

                m_angle = m_angle + m_incAngle;
            }

            m_incAngle = (m_incAngle + 0.00009);
        }

        void draw(sf::RenderTarget& target, sf::RenderStates states) const override {
            for (Point* point : m_points) {
                target.draw(point->draw, states);
            }
        }
};



int main() {
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode(1268, 768), "SFML works!", sf::Style::Default, settings);
    sf::CircleShape shape(100.f);
    Spiral spiral(1500, new sf::Vector2f(600, 400));

    shape.setFillColor(sf::Color::Green);

    //spirals.push_back(Spiral(200, new sf::Vector2f(300, 300)));
    //spirals.push_back(Spiral(200, new sf::Vector2f(600, 300)));

    window.setFramerateLimit(60);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        /*for (Spiral s : spirals) {
            s.update();
        }*/

        spiral.update();

        window.clear();

        /*for (Spiral s : spirals) {
            window.draw(s);
        }*/

        window.draw(spiral);
        window.display();
    }

    return 0;
}

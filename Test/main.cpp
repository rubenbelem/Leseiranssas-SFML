//
// Created by lathe on 1/28/19.
//

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <algorithm>
#include <random>
#include <ctime>
#include <cstdlib>

#define N 50
#define HEIGHT 600
#define WIDTH 800

using namespace std;

sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "SFML works!");

struct Bar {
    sf::RectangleShape bar;

};

int mapHeightToColor(int input) {
    int output_start = 50;
    float slope = (255.0 - output_start) / (HEIGHT - 0.0);
    return output_start + slope * (input - 0.0);
}

void drawLines(vector<int> v, float lineWidth, int redIndex = 1, int greenIndex = -1) {


    for (int x = 0; x < N; ++x) {
        int height = v[x];
        sf::RectangleShape line(sf::Vector2f(lineWidth, height));
        //playSound(height);
        if (x == redIndex)
            line.setFillColor(sf::Color::Red);
        else if (x == greenIndex) line.setFillColor(sf::Color::Green);
        else line.setFillColor(sf::Color(mapHeightToColor(height), mapHeightToColor(height),  mapHeightToColor(height)));

        line.setPosition(x * (lineWidth ), HEIGHT - height);

        window.draw(line);

    }

    window.display();
    window.clear();
}

int main() {
    srand ( unsigned ( std::time(0) ) );


    vector<int> v(N);
    int lineWidth = WIDTH / (N - 1);
    bool sorted = false;

    for (int i = 0; i < N; ++i) {
        v[i] = 1 + i * (double)(HEIGHT / N);
    }

    std::random_shuffle(v.begin(), v.end());

    window.setFramerateLimit(60);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (!sorted) {
            for (int i = 0; i < N; ++i) {
                int minIndex = i;
                for (int j = i + 1; j < N; ++j) {
                    if (v[j] < v[minIndex]) {
                        minIndex = j;

                        //drawLines(v, lineWidth, j, minIndex);
                    }

                    drawLines(v, lineWidth, j, minIndex);
                }

                std::swap(v[i], v[minIndex]);

                drawLines(v, lineWidth, -1, minIndex);

                sorted = true;
            }
        }
        else {
            drawLines(v, lineWidth, -1, -1);
        }


    }

    return 0;
}
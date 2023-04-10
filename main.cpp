#include "helper.h"
#include "lines.h"

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>


const int WIN_HEIGHT = 480;
const int WIN_WIDTH = 640;
const int TILE_SIZE = 80;


int main( int argc, char** argv)
{
    struct DemoConfig demoConfig;

    if(argc == 6)
    {
        demoConfig = initConfig(argv);
    }
   
    std::vector<Point2d> pointsToPlot;
    pointsToPlot = getPoints(demoConfig.p1, demoConfig.p2);

    std::cout << "\npointsToPlot.size() is " << pointsToPlot.size() << "\n";
    printPoints(pointsToPlot);
 

    int printCount = 1;
    std::string imageName = demoConfig.title + ".jpg";


    #if 1
    sf::RenderWindow window(sf::VideoMode(WIN_WIDTH, WIN_HEIGHT), demoConfig.title);
    window.setFramerateLimit(1);

    while(window.isOpen())
    {
        sf::Event event;
        while(window.pollEvent(event))
        {
            if(event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        // clear background
        window.clear(sf::Color::Black);

        #if 1
        for(int y = 0; y < WIN_HEIGHT; y += TILE_SIZE)
        {
            for(int x = 0; x < WIN_WIDTH; x += TILE_SIZE)
            {
                sf::RectangleShape rect(sf::Vector2f(TILE_SIZE, TILE_SIZE));
                rect.setPosition(sf::Vector2f(x,y));
                rect.setFillColor(sf::Color::White);
                rect.setOutlineThickness(4.0f);
                rect.setOutlineColor(sf::Color(0,0,100));
                window.draw(rect);
            }
        }
        #endif

        #if 1
        for(unsigned int i = 0; i < pointsToPlot.size(); i++)
        {
            int x = pointsToPlot[i].x * TILE_SIZE;
            int y = pointsToPlot[i].y * TILE_SIZE;
            sf::RectangleShape rect(sf::Vector2f(TILE_SIZE-4, TILE_SIZE-4));
            rect.setPosition(sf::Vector2f(x,y));
            rect.setFillColor(sf::Color::Yellow);
            window.draw(rect);
        }
        #endif

        sf::Vertex line[] = { 
            sf::Vertex(sf::Vector2f(demoConfig.p1.x * TILE_SIZE + TILE_SIZE/2, demoConfig.p1.y * TILE_SIZE + TILE_SIZE/2), sf::Color::Red),
            sf::Vertex(sf::Vector2f(demoConfig.p2.x * TILE_SIZE + TILE_SIZE/2, demoConfig.p2.y * TILE_SIZE + TILE_SIZE/2), sf::Color::Red)
        };

        window.draw(line, 2, sf::Lines);

        if(printCount == 1)
        {
            sf::Texture texture;
            texture.create(window.getSize().x, window.getSize().y);
            texture.update(window);
            if(texture.copyToImage().saveToFile(imageName))
            {
                std::cout << "screenshot saved to " << imageName << "\n";
                ++printCount;
            }
        }

        window.display();
    } 
    #endif 

    return 0;
}
#include "helper.h"
#include <iostream>

DemoConfig initConfig(char** argv)
{
    DemoConfig config;

    config.p1.x = atoi(argv[1]);
    config.p1.y = atoi(argv[2]);
    config.p2.x = atoi(argv[3]);
    config.p2.y = atoi(argv[4]);
    config.title = argv[5];
    return config;
}


void printPoints(const std::vector<Point2d>& p)
{
    for(auto e : p)
    {
        std::cout << e.x << ", " << e.y << "\n";
    }
}
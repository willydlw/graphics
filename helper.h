#ifndef MY_HELPER_H
#define MY_HELPER_H

#include <string>
#include "lines.h"

struct DemoConfig
{
    Point2d p1;
    Point2d p2;
    std::string title;

    // Constructors
    DemoConfig(): p1({0,0}), p2({0,1}), title("Default")
    {}

    DemoConfig(int x1, int y1, int x2, int y2, std::string winTitle) :
        p1({x1,y1}), p2({x2,y2}), title(winTitle)
    {}

};


DemoConfig initConfig(char** argv);
void printPoints(const std::vector<Point2d>& p);

#endif 
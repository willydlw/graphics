#ifndef LINES_H
#define LINES_H

#include <vector>


struct Point2d{
    int x;
    int y;

    Point2d() : x(0), y(0) {}
    Point2d(int xx, int yy) : x(xx), y(yy) {}
};


// Function prototypes

std::vector<Point2d> bresenham_line_horizontal(Point2d p1, Point2d p2);
std::vector<Point2d> bresenham_line_vertical(Point2d p1, Point2d p2);

std::vector<Point2d> bresenham_line_case1(int x1, int y1, int x2, int y2);

std::vector<Point2d> getPoints(Point2d p1, Point2d p2);
void swap(int* a, int* b);




#endif 
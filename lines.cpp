#include "lines.h"

#include <cmath>
#include <cstdlib>
#include <iostream>


void swap(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}


std::vector<Point2d> getPoints(Point2d p1, Point2d p2)
{
    std::vector<Point2d> points;
    int dx = p1.x - p2.x;
    int dy = p1.y - p2.y;

    std::cout << __FUNCTION__ << ", dx: " << dx << "\n";

    if(dx == 0)
    {
        points = bresenham_line_vertical(p1, p2);
    }
    else if(dy == 0)
    {
        points = bresenham_line_horizontal(p1, p2);
    }

    std::cout << __FUNCTION__ << ", points.size() is "
            << points.size() << "\n";

    return points;
}


std::vector<Point2d> bresenham_line_vertical(Point2d p1, Point2d p2)
{
    std::vector<Point2d> points;

    if(p1.y > p2.y)
    {
        swap(&p1.x, &p2.x);
        swap(&p1.y, &p2.y);
    }

    for(int y = p1.y; y <= p2.y; y++)
    {
        std::cout << "y: " << y << "\n";
        points.push_back({p1.x,y});
    }

    return points;
}


std::vector<Point2d> bresenham_line_horizontal(Point2d p1, Point2d p2)
{
    std::vector<Point2d> points;

    if(p1.x > p2.x)
    {
        swap(&p1.x, &p2.x);
        swap(&p1.y, &p2.y);
    }

    for(int x = p1.x; x <= p2.x; x++)
    {
        std::cout << "x: " << x << "\n";
        points.push_back({x,p1.y});
    }

    return points;
}


std::vector<Point2d> bresenham_line_case1(int x1, int y1, int x2, int y2)
{
    std::vector<Point2d> points;

    if(x1 > x2)
    {
        swap(&x1, &x2);
        swap(&y1, &y2);
    }
 

    // calculate constants
    const int dx = x2 - x1;
    const int dy = y2 - y1; 

    const int twody = 2 * dy;                        // 2 dy
    const int twodydx = 2 * dy - 2 * dx;        // 2dy - 2dx

    // plot the pixel at x1, y1
    points.push_back(Point2d(x1,y1));


    // initial decision parameter: p(0) = 2 * dy - dx;
    int pk = 2 * dy - dx;       

    // for each integer x-coordinate, xk, along the line
    for(int x = x1 + 1, y = y1; x < x2; x++)
    {
        if(pk <= 0)
        {
            // plot pixel at x(k) + 1, y(k)    
            Point2d p(x,y);
            points.push_back(p);

            // p(k+1) = p(k) + 2 dy
            pk += twody;
        }
        else
        {   // plot pixel at (x(k)+1, y(k) + 1)
            y += 1;
            Point2d p(x,y);
            points.push_back(p);

            // p(k+1) = p(k) + 2dy - 2dx
            pk += twodydx;
        }
    }

    // add final point (x2, y2) to list
    points.push_back(Point2d(x2,y2));

    return points;
}


/* Bresenham's Line Generation Algorithm Case 2

    Given coordinates of two points: A(x1, y1) and B(x2, y2), find
    the intermediate points for drawing the line AB on a computer
    screen of pixels. Every pixel has integer coordinates.

    Simplifying Assumptions:
    1. Draw lines from left to right
    2. x1 < x2 and y1 < y2
    3. Slope of line is greater than 1.


*/

/*  Case 2: Slope of line is greater than one.  m > 1

    1. Line segment goes from top of screen towards bottom of screen.
        y1 <= y2
    2. Line has positive slope greater than 1.
        Vertical projection y2 - y1 is longer than the horizontal projection x2 - x1.
    3. For each row y between y1 and y2, there is exactly one column x (computed 
       by the algorithm) containing a pixel of the line.
       
       Each column between x1 and x2 may contain multiple rasterized pixels.

    Choose the integer x corresponding to the pixel center that is closest to the 
    ideal (fractional) x for the same y. On successive rows x can remain the 
    same or increase by 1.


    Line equation: x = (1/m) (y - b)            (7)
        x = (1/m) (y - b)

        m - slope
        b - y intercept

    
    Determine x,y pixel values
    --------------------------

    y(k) is the current pixel value of y
    y(k + 1) will always be y(k) incremented by 1 when m > 1. 
        Thus, y(k + 1) = y(k) + 1

    Next x is the smaller of two distances.
        x = (1/m) (y(k+1)) - b )

        Distance d1 is distance from true x to x(k) 
            d1 = x - x(k) 
            d1 = (1/m) (y(k+1) - b) - x(k)
            d1 = (1/m) (y(k) + 1 - b) - x(k)
                         
        Distance d2 is distance from true x to x(k+1)
            d2 = x(k+1) - x    
            d2 = x(k+1) - ( (1/m) (y(k+1) - b))
            d2 = x(k) + 1 - (1/m)(y(k) + 1 - b)


    Subtract d2 from d1.
        d1 - d2 = [ (1/m)(y(k) + 1 - b) - x(k)] - [x(k) + 1 - ((1/m) (y(k) + 1 - b)]

        d1 - d2 = 2/m(y(k) + 1 - b) - 2 x(k) - 1

        Substitute dy/dx for m.
        d1 - d2 = 2/(dy/dx)(y(k) + 1 - b) - 2 x(k) - 1

        Multiply both sides by dy.
        dy(d1 - d2) = 2dx (y(k) + 1 - b) - 2 dy x(k) - dy


    Find decision parameters P(k), P(k+1)
        Let P(k) = dy (d1 - d2)
        Let C = 2dx - 2dx b - dy which is a constant.

        P(k) = 2dx y(k)  - 2 dy x(k) + C                    (8)

        P(k+1) = 2 dx y(k+1) - 2dy x(k+1) + C               (9)

    
    Recurrence relation: Subtract P(k) from P(k+1).
        P(k+1) - P(k) = [2 dx y(k+1) - 2dy x(k+1) + C ] - [2dx y(k)  - 2 dy x(k) + C ]

        P(k+1) - P(k) = 2dx - 2dy (x(k+1) - x(k))           (10)

    When P(k) < 0 then d1 - d2 < 0 and d1 < d2. We choose x(k) as next y value 
    because the current x(k) distance from the intersection point x is smaller.

    When P(k) > 0 then (d1 - d2) > 0 and d1 > d2. We choose x(k) + 1 as 
    the next x value.


    P(k+1) = 2 dx when P(k) < 0.
        P(k+1) - P(k) = 2 dx - 2 dy(x(k) - x(k)) = 2 dx - 2 dy(0) = 2 dx

    P(k+1) = 2 dx - 2 dy when P(k) > 0.
        P(k+1) - P(k) = 2 dx - 2 dy(x(k) + 1 - x(k)) = 2 dx - 2 dy(1) = 2 dx - 2 dy


    -------------------------------
    Initial Decision Parameter, P(0)
    -------------------------------
    P(k) = 2 dx y(k) - 2 dy x(k) + [2dx - 2dx b - dy]

    Equation for b:
        y(k) = m x(k) + b
        b = y(k) - m x(k)

    Sub for b:
    P(k) = 2 dx y(k) - 2 dy x(k) + [2dx - 2dx (y(k) - m x(k)) - dy]

    Sub dy/dx for m:
    P(k) = 2 dx y(k) - 2 dy x(k) + [2dx - 2dx (y(k) - dy/dx x(k)) - dy]

    Simplify:
    P(k) = 2 dx y(0) - 2 dy x(k) + [2dx - 2dx (y(0) - dy/dx x(k)) - dy]

    P(k) = 2 dx - dy

    P(0) = 2 dx - dy


std::vector<Point2d> bresenham_line_case2(int x1, int y1, int x2, int y2)
{
    std::vector<Point2d> points;

    if(y1 > y2)
    {
        swap(&x1, &x2);
        swap(&y1, &y2);
    }
}
*/

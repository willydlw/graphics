#include <iostream>
#include <cmath>
#include <cstdlib>

#include <SFML/Graphics.hpp>


const int WIN_HEIGHT = 480;
const int WIN_WIDTH = 640;
const int TILE_SIZE = 80;

struct Point2d{
    int x;
    int y;

    Point2d() : x(0), y(0) {}
    Point2d(int xx, int yy) : x(xx), y(yy) {}
};

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


// Function prototypes
void printPoints(const std::vector<Point2d>& p);
std::vector<Point2d> getPoints(Point2d p1, Point2d p2);
std::vector<Point2d> bresenham_line_vertical(Point2d p1, Point2d p2);
void swap(int* a, int* b);


// Function Definitions

void swap(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}


/*
    Graphics Conventions:
    1. Top left is (0,0) such that pixel coordinates increase in the right
       and down directions.
    2. Pixel centers have integer coordinates.

    Line endpoints are pixels at (x1, y1) and (x2, y2) where x is the column
    and y is the row.
*/




/*  Case 1: Slope of line is between zero and one.  0 < m <= 1

    1. Line segment goes down and to the right.
        x1 <= x2, y1 <= y2
    2. Line has positive slope less than 1.
        Horizontal project x2 - x1 is longer than the vertical projection y2 - y1.
    3. For each column x between x1 and x2, there is exactly one row y (computed 
       by the algorithm) containing a pixel of the line.
       
       Each row between y1 and y2 may contain multiple rasterized pixels.

    Choose the integer y corresponding to the pixel center that is closest to the 
    ideal (fractional) y for the same x. On successive columns y can remain the 
    same or increase by 1.


    Line equation: y = mx + b                   (1)
        m - slope
        b - y intercept


    Slope: (change in y / change in x) or (rise / run)
        m = (y2 - y1) / (x2 - x1) = dy / dx     (2)

    
    Determine x,y pixel values
    --------------------------

    x(k) is the current pixel value of x
    x(k + 1) will always be x(k) incremented by 1 when 0 < m <= 1. 
        Thus, x(k + 1) = x(k) + 1

    Next y is the smaller of two distances.
        y = m(x(k+1)) + b

        Distance d1 is distance from true y to y(k) 
            d1 = y - y(k) 
            d1 = m x(k+1) + b - y(k)
            d1 = m(x(k) + 1) + b - y(k)
                         
        Distance d2 is distance from true y to y(k+1)
            d2 = y(k+1) - y    
            d2 = y(k+1) - (m x(k+1) + b)
            d2 = y(k) + 1 - m(x(k) + 1) - b


    Subtract d2 from d1.
        d1 - d2 = [ m(x(k) + 1) + b - y(k)] - [y(k) + 1 - (m (x(k) + 1) + b)]

        d1 - d2 = 2m (x(k) + 1) + 2b - 2y(k) - 1            (3)

        Substitute dy/dx for m.
        d1 - d2 = 2 dy/dx (x(k) + 1) - 2y(k) + 2b - 1

        Mulitply both sides by dx.
        dx(d1 - d2) = 2dy (x(k) + 1) - 2(y(k)) dx + 2b dx - dx


    Find decision parameters P(k), P(k+1)
        Let P(k) = dx (d1 - d2)
        Let C = 2dy + 2b dx - dx which is a constant.

        P(k) = 2dy x(k)  - 2 dx y(k) + C                    (4)

        P(k+1) = 2 dy x(k+1) - 2dx y(k+1) + C               (5)

    
    Recurrence relation: Subtract P(k) from P(k+1).
        P(k+1) - P(k) = 2dy (x(k) - x(k+1)) - 2 dx(y(k+1) - y(k)) + C - C

        P(k+1) - P(k) = 2dy - 2dx (y(k+1) - y(k))           (6)

    When P(k) < 0 then d1 - d2 < 0 and d1 < d2. We choose y(k) as next y value 
    because the current y(k) distance from the intersection point y is smaller.

    When P(k) > 0 then (d1 - d2) > 0 and d1 > d2. We choose y(k) + 1 as 
    the next y value.


    P(k+1) = 2 dy when P(k) < 0.
        P(k+1) - P(k) = 2 dy - 2 dx(y(k) - y(k)) = 2 dy - 2 dx(0) = 2 dy

    P(k+1) = 2 dy - 2 dx when P(k) > 0.
        P(k+1) - P(k) = 2 dy - 2 dx(y(k) + 1 - y(k)) = 2 dy - 2 dx(1) = 2 dy - 2 dx


    -------------------------------
    Initial Decision Parameter, P(0)
    -------------------------------
    P(k) = 2 dy x(k) - 2 dx y(k) + [2 dx b - dx + 2 dy]

    Equation for b:
        y(k) = m x(k) + b
        b = y(k) - m x(k)

    Sub for b:
    P(k) = 2 dy x(k) - 2 dx y(k) + 2 dx (y(k)-m x(k)) - dx + 2 dy

    Sub dy/dx for m:
    P(k) = 2 dy x(k) - 2 dx y(k) + 2 dx (y(k)- dy/dx x(k)) - dx + 2 dy

    Simplify:
    P(k) = 2 dy x(0) - 2 dx y(k) + 2 dx y(k)- 2 dy x(k)) - dx + 2 dy

    P(k) = 2 dy - dx

    P(0) = 2 dy - dx

*/


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

    printPoints(points);
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

void printPoints(const std::vector<Point2d>& p)
{
    for(auto e : p)
    {
        std::cout << e.x << ", " << e.y << "\n";
    }
}

struct DemoConfig initConfig(int argc, char** argv)
{
    struct DemoConfig config;

    config.p1.x = atoi(argv[1]);
    config.p1.y = atoi(argv[2]);
    config.p2.x = atoi(argv[3]);
    config.p2.y = atoi(argv[4]);
    config.title = argv[5];
    return config;
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

int main( int argc, char** argv)
{
    struct DemoConfig demoConfig;

    if(argc == 6)
    {
        demoConfig = initConfig(argc, argv);
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
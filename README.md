# Algorithms for primitive geometry graphics

## Graphics Conventions
    1. Top left pixel coordinates are (0,0) such that x coordinates increase in the right direction and y coordinates increase in the down direction.
    2. Pixel centers have integer coordinates.
    3. Line endpoints are pixels at (x1, y1) and (x2, y2) where x is the column and y is the row.


## Bresenham's Line Algorithm



### Case 1: Slope in range 0 < m <= 1
1. Line segment points
    1. Line segment is drawn from left to right
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

//
//  main.cpp
//  cs4820p2regular
//
//  Created by James Kizer on 6/11/14.
//  Copyright (c) 2014 JimmyTime Software. All rights reserved.
//

#include <iostream>
#include <string>
#include <cstdlib>
#include <set>
#include <stdio.h>
#include <math.h>
#include <assert.h>


#define MIN_POINT_VALUE 0
#define MAX_POINT_VALUE 9999
#define MAX_CIRCUMFERENCE 4.0*(double)MAX_POINT_VALUE

class Point;


std::ostream &operator<<(std::ostream &out, const Point &p);

class Point
{
public:
    Point(int in_x, int in_y): x(in_x), y(in_y){}
    
    
    
    void Print(std::ostream &out) const {
        
        char xStr[33];
        char yStr[33];
        
        sprintf(xStr, "%d", x);
        sprintf(yStr, "%d", y);
        
        out<<std::string(std::string("(" + std::string(xStr) + std::string(",") + std::string(yStr) + std::string(")")));
    }

public:
    const int x;
    const int y;
};



std::ostream &operator<<(std::ostream &out, const Point &p) {
    //out << std::string(S, S.get_len());
    p.Print(out);
    return out;
}

//

struct PointCompareXFirst {
    bool operator() (const Point& lhs, const Point& rhs) const
    {
        //first x
        if(lhs.x < rhs.x)
            return true;
        
        else if (lhs.x > rhs.x)
            return false;
        
        //then if they tie, y
        return (lhs.y < rhs.y);
        
    }
};

struct PointCompareYFirst {
    bool operator() (const Point& lhs, const Point& rhs) const
    {
        //first y
        if(lhs.y < rhs.y)
            return true;
        
        else if (lhs.y > rhs.y)
            return false;
        
        //then if they tie, x
        return (lhs.x < rhs.x);
        
    }
};

typedef std::set<Point,PointCompareXFirst>::iterator PointerToPoint;

struct PointerToPointCompareYFirst {
    bool operator() (const PointerToPoint& lhs, const PointerToPoint& rhs) const
    {
        PointCompareYFirst compare;
        return compare(*lhs, *rhs);
    }
};


std::set<Point,PointCompareXFirst> *setOfPoints;

double distanceBetweenPoints(const Point &p1, const Point &p2)
{
    //compute absolute distance between points in each dimension
    double dx = (double)abs(p1.x - p2.x);
    double dy = (double)abs(p1.y - p2.y);
    
    //return the hypotenuse
    return hypot(dx, dy);
}

double getcircumferenceOf3Points(const Point &p1, const Point &p2, const Point &p3)
{
    double d1 = distanceBetweenPoints(p1, p2);
    double d2 = distanceBetweenPoints(p2, p3);
    double d3 = distanceBetweenPoints(p3, p1);
    
    return d1 + d2 + d3;
}

double getMinimumcircumferenceOf3PointsInSetOfPointsSortedInSecondDimension(std::set<Point,PointCompareYFirst> *setOfPoints, int heightOfBox)
{
    //starting with the first point, find the range of points that fit in the box of height maxHeightOfBox
    double minimumcircumference = MAX_CIRCUMFERENCE;
    
    std::set<Point,PointCompareYFirst>::iterator begin = setOfPoints->begin();
    std::set<Point,PointCompareYFirst>::iterator end = setOfPoints->end();
    
    
    //boxBottom
    std::set<Point,PointCompareYFirst>::iterator currentPoint = begin;
    
    std::set<Point,PointCompareYFirst>::iterator currentInBoxBottom;
    std::set<Point,PointCompareYFirst>::iterator afterLastInBoxBottom;
    std::set<Point,PointCompareYFirst>::iterator afterLastInBox;
    std::set<Point,PointCompareYFirst>::iterator middleLoopPointer;
    std::set<Point,PointCompareYFirst>::iterator innerLoopPointer;
    
    while(currentPoint != end)
    {
        //define box
        currentInBoxBottom = currentPoint;
        afterLastInBoxBottom = setOfPoints->upper_bound(Point(MAX_POINT_VALUE+1,currentInBoxBottom->y));
        afterLastInBox = setOfPoints->upper_bound(Point(MAX_POINT_VALUE+1,currentInBoxBottom->y + heightOfBox));
        
        //for all elements in the bottom of the box (as there may be several at the same y value)
        while(currentInBoxBottom != afterLastInBoxBottom)
        {
         
            //for the remaining elements in the box
            middleLoopPointer = std::next(currentInBoxBottom);
            while(middleLoopPointer != afterLastInBox)
            {
                innerLoopPointer = std::next(middleLoopPointer);
                while(innerLoopPointer != afterLastInBox)
                {
                    double circumference = getcircumferenceOf3Points(*currentInBoxBottom, *middleLoopPointer, *innerLoopPointer);
                    if (minimumcircumference < 0)
                        minimumcircumference = circumference;
                    else
                        minimumcircumference = fmin(minimumcircumference, circumference);
                    //we just set the minimum circumference
                    //note that the height of the box only needs to be as large as the current min circumference
                    //potentially update the height of the box
                    if(ceil(minimumcircumference) < heightOfBox)
                    {
                        heightOfBox = ceil(minimumcircumference);
                        afterLastInBox = setOfPoints->upper_bound(Point(MAX_POINT_VALUE+1,currentInBoxBottom->y + heightOfBox));
                    }
                    assert(std::distance(innerLoopPointer, afterLastInBox));
                    innerLoopPointer++;
                }
                middleLoopPointer++;
            }
            currentInBoxBottom++;
        }
        currentPoint = afterLastInBoxBottom;
        
        //note that the height of the box only needs to be as large as the current min circumference
        //potentially update the height of the box
        heightOfBox = ceil(fmin(heightOfBox, minimumcircumference));
    }

    return minimumcircumference;
}

//end should be the point past the last point in the range
double getMinimumcircumferenceOf3PointsInRangeOfIteratorsInFirstDimension(const PointerToPoint begin, const PointerToPoint end, int numberOfPointsInRange, std::set<Point,PointCompareXFirst> *set)
{
    int distance = std::distance(begin, end);
    assert(distance == numberOfPointsInRange);
    if (numberOfPointsInRange < 3)
        return MAX_CIRCUMFERENCE;
    
    else if (numberOfPointsInRange == 3)
    {
        PointerToPoint b = begin;
        PointerToPoint p1 = b++;
        PointerToPoint p2 = b++;
        PointerToPoint p3 = b++;
        assert(b == end);
        return getcircumferenceOf3Points(*p1, *p2, *p3);
    }
    
    //we need to:
    
    //1)split in half (find n/2 point, include all values that share x with n/2 point in left hand side)
    PointerToPoint placeholder = begin;
    int pointsInLeftHalf = 0;
    int pointsInRightHalf = numberOfPointsInRange;
    for (; pointsInLeftHalf<(numberOfPointsInRange/2); pointsInLeftHalf++, pointsInRightHalf--)
    {
        placeholder++;
    }
    
    assert(pointsInLeftHalf + pointsInRightHalf == numberOfPointsInRange);
    
    //create "median" point that is guaranteed to come after all points with x value equal to placeholder
    //and guaranteed to come before all points with x values greater than placeholder
    Point medianPoint(placeholder->x, MAX_POINT_VALUE+1);

    //upper bound returns the first iterator after medianPoint
    PointerToPoint firstRight = set->upper_bound(medianPoint);
    PointerToPoint lastLeft = std::prev(firstRight);
    
    //update the number of points in the left side
    //note must be lastLeft >= placeholder
    
    
    pointsInLeftHalf++;
    pointsInRightHalf--;
    
    while(placeholder != lastLeft)
    {
        placeholder++;
        pointsInLeftHalf++;
        pointsInRightHalf--;
    }
    assert(pointsInLeftHalf + pointsInRightHalf == numberOfPointsInRange);
    int leftDistance = std::distance(begin, firstRight);
    int rightDistance = std::distance(firstRight, end);
    assert(leftDistance == pointsInLeftHalf);
    assert(rightDistance == pointsInRightHalf);
    
    //2)recurse for both sides and find the minimum circumference out of the pair
    assert(pointsInLeftHalf>0);
    
    double minimumcircumferenceFromRecursions;
    
    //since we make sure that all points with the same x value are grouped together,
    //it is possible that the left side makes up the entire set
    //if so, we cant recurse (boundless recursion) and we need to analize vertically
    if(pointsInLeftHalf < numberOfPointsInRange)
    {
        double minimumcircumferenceInLeftHalf = getMinimumcircumferenceOf3PointsInRangeOfIteratorsInFirstDimension(begin, (const PointerToPoint)firstRight, pointsInLeftHalf, set);
        double minimumcircumferenceInRightHalf = getMinimumcircumferenceOf3PointsInRangeOfIteratorsInFirstDimension((const PointerToPoint)firstRight, end, pointsInRightHalf, set);
        minimumcircumferenceFromRecursions = fmin(minimumcircumferenceInLeftHalf, minimumcircumferenceInRightHalf);
    }
    else
    {
        minimumcircumferenceFromRecursions = MAX_CIRCUMFERENCE;
    }

    //3)combine results
    
    PointerToPoint firstInBand;
    PointerToPoint lastInBand;
    
    //at this point, we need to get a vertical band of points around the medianPoint, total width of the circumference.
    //keep in mind, the circumference is double and we need our band to be integer
    double halfcircumference = minimumcircumferenceFromRecursions/2;
    int halfcircumferenceInt = ceil(halfcircumference);
    
    Point leftBandEdgePoint(placeholder->x - halfcircumferenceInt, MAX_POINT_VALUE+1);
    Point rightBandEdgePoint(placeholder->x + halfcircumferenceInt, MAX_POINT_VALUE+1);
    firstInBand = set->lower_bound(leftBandEdgePoint);
    lastInBand = set->upper_bound(rightBandEdgePoint);
    
    //generate set of points in vertical band sorted by y dimension first
    std::set<Point,PointCompareYFirst> *setOfPointsInVerticalBand = new std::set<Point,PointCompareYFirst>;
    
    for(PointerToPoint it = firstInBand; it != lastInBand; it++)
    {
        setOfPointsInVerticalBand->insert(*it);
    }
    
    double minimumcircumferenceFromVerticalBand = getMinimumcircumferenceOf3PointsInSetOfPointsSortedInSecondDimension(setOfPointsInVerticalBand, halfcircumferenceInt*2);
    
    delete setOfPointsInVerticalBand;
    
    return fmin(minimumcircumferenceFromRecursions, minimumcircumferenceFromVerticalBand);

}


int main(int argc, const char * argv[])
{
    
    int N;
    std::ostream &output = std::cout;

    scanf("%d", &N);
    
    
    setOfPoints = new std::set<Point,PointCompareXFirst>;
    
    char    *buffer;
    buffer = (char*)malloc(1024);
    for (int i=0; i<N; i++)
    {
        std::string line;
        int x, y;
        
        size_t  n = 1024;
        buffer = (char*)malloc(n);
        
        scanf("%d %d", &x, &y);

        Point p(x,y);
        setOfPoints->insert(p);
    }
    
//    for (auto it = setOfPoints->begin(); it != setOfPoints->end(); it++)
//    {
//        output << (*it) <<std::endl;
//    }
    
    
    double minimumcircumference = getMinimumcircumferenceOf3PointsInRangeOfIteratorsInFirstDimension(setOfPoints->begin(), setOfPoints->end(), setOfPoints->size(), setOfPoints);

    output.precision(2);
    std::fixed(output);
    output << minimumcircumference <<std::endl;

    delete(setOfPoints);
    return 0;
}


#include <stdio.h>
#include <float.h>
#include <stdlib.h>
#include <math.h>
 

// A structure to represent a Point in 1D plane
typedef struct
{
    int x;
} Point;


/* Following two functions are needed for library function qsort().
   Refer: http://www.cplusplus.com/reference/clibrary/cstdlib/qsort/ */
 
// Needed to sort array of points according to X coordinate
int compare(const void* a, const void* b)
{
    Point *p1 = (Point *)a,  *p2 = (Point *)b;
    return (p1->x - p2->x);
}


// A utility function to find the distance between two points
float dist(Point p1, Point p2)
{
    return (p2.x - p1.x);
}



// A Brute Force method to return the smallest distance between two points
// in P[] of size n
float bruteForce(Point P[], int n)
{
    float min = FLT_MAX;
    int i=0;
    int j=0;
    for (i = 0; i < n; ++i)
        for (j = i+1; j < n; ++j)
            if (dist(P[i], P[j]) < min)
                min = dist(P[i], P[j]);
    return min;
}


// A utility function to find minimum of two float values
float min(float x, float y)
{
    return (x < y)? x : y;
}


// A recursive function to find the smallest distance. The array P contains
// all points sorted according to x coordinate
float distance(Point P[], int n)
{
    // If there are 2 or 3 points, then use brute force
    if (n <= 3)
        return bruteForce(P, n);
 
    // Find the middle point
    int mid = n/2;
    Point midPoint = P[mid];
 
    // Consider the vertical line passing through the middle point
    // calculate the smallest distance dl on left of middle point and
    // dr on right side
    float dl = distance(P, mid);
    float dr = distance(P + mid, n-mid);
 
    // Find the smaller of two distances
    float d = min(dl, dr);
 
    // Find the closest points in strip.  Return the minimum of d and closest
    // distance is strip[]
    return min(d, (P[mid+1].x - P[mid].x));
}


// The main functin that finds the smallest distance
// This method mainly uses distance()
float closest(Point P[], int n)
{
    qsort(P, n, sizeof(Point), compare);
 
    // Use recursive function distance() to find the smallest distance
    return distance(P, n);
}

// Driver program to test above functions
int main()
{
    Point P[] = {2, 12, 40, 14, 30};
    int n = sizeof(P) / sizeof(P[0]);
    printf("The smallest distance is %f ", closest(P, n));
    return 0;
}

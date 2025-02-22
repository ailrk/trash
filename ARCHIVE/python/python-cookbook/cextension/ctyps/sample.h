#include <math.h>
#include <stdbool.h>

extern int gcd(int, int);
extern bool in_mandel(double x0, double y0, int n);
extern int divide(int a, int b, int *remainder);
extern double avg(double *a, int n);
typedef struct Point {
  double x, y;
} Point;

extern double distance(Point *p1, Point *p2);


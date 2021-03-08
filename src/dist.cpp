#include <stdlib.h>
#include <cmath>

double get_dist_from_center(double x, double y) {
    double cx = 0.5;
    double cy = 0.5;
    double a = std::abs(cx - x);
    double o = std::abs(cy - y);
    double theta = std::atan(o/a);
    double h = o / std::sin(theta);
    return h;
}
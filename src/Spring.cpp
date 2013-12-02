#include <iostream>
#include "Spring.h"

using namespace std;

Spring::Spring(int point1, int point2, float rest, float stiff) {
    p1 = point1;
    p2 = point2;
    rest_length = rest;
    stiffness = stiff;
}

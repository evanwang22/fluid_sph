#include "vecmath.h"
#include <vector>

using namespace std;

class Spring {

public:

    Spring(int p1, int p2, float rest_length, float stiffness);

    int p1;
    int p2;
    float rest_length;
    float stiffness;

};

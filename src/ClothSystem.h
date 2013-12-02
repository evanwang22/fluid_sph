#ifndef CLOTHSYSTEM_H
#define CLOTHSYSTEM_H

#include "extra.h"
#include <vector>
#include <map>

#include "particleSystem.h"
#include "Spring.h"

class ClothSystem: public ParticleSystem
{
///ADD MORE FUNCTION AND FIELDS HERE
public:
	ClothSystem(int rows, int columns);
	vector<Vector3f> evalF(vector<Vector3f> state);
    Vector3f springForces(int i, vector<Vector3f> state);
    int getIndex(int i, int j);
	void draw();

    int nColumns;
    int nRows;
    
    map<int, vector<Spring>> spring_map;
	
    static constexpr float mass = 1;
    static constexpr float g = -9.8;
    // drag constant
    static constexpr float drag_k = 0.5;
    // spring constant
    static constexpr float spring_k = 120;

private:

};


#endif

#ifndef PENDULUMSYSTEM_H
#define PENDULUMSYSTEM_H

#include "extra.h"
#include <vector>
#include <map>

#include "particleSystem.h"
#include "ClothSystem.h"

class PendulumSystem: public ParticleSystem
{
public:
	PendulumSystem(int numParticles);
	
	vector<Vector3f> evalF(vector<Vector3f> state);
    
    Vector3f springForces(int i, vector<Vector3f> state);
	
	void draw();

    map<int, vector<Spring>> spring_map;
	
    static constexpr float mass = 1;
    static constexpr float g = -2;
    // drag constant
    static constexpr float drag_k = 0.5;
    // spring constant
    static constexpr float spring_k = 20;
};

#endif

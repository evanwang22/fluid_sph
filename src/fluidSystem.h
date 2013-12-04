#ifndef FLUIDSYSTEM_H
#define FLUIDSYSTEM_H

#include "extra.h"
#include <vector>
#include <map>

#include "particleSystem.h"

class FluidSystem: public ParticleSystem
{
public:
	FluidSystem(int numParticles);
	
	vector<Vector3f> evalF(vector<Vector3f> state);
    
    Vector3f checkBounds(Vector3f position);
    
	void draw();

	
    static constexpr float mass = 1;
    static constexpr float g = -2;
    // drag constant
    static constexpr float drag_k = 0.5;
    // spring constant
    static constexpr float spring_k = 20;
    static constexpr float smoothing_width = 50.0f;
};

#endif

#ifndef FLUIDSYSTEM_H
#define FLUIDSYSTEM_H

#include "extra.h"
#include <vector>
#include <map>
#include <math.h>
#include "particleSystem.h"

class FluidSystem: public ParticleSystem
{
public:
	FluidSystem(int numParticles);
	
	vector<Vector3f> evalF(vector<Vector3f> state);
    
    Vector3f checkBounds(Vector3f position);
    
    float calcMassDensity(Vector3f p1_pos, vector<Vector3f>* state);
    float calculatePressureGradient(float p1_md, float p2_md, Vector3f pos1, Vector3f pos2);
    float calculateViscosity(Vector3f pos1, Vector3f pos2, Vector3f velocity1, Vector3f velocity2, float p1_md);

	void draw();
	void setMD(const vector<float>  & newState) { m_dState = newState; };

    static constexpr float mass = 1;
    static constexpr float g = -2;
    // drag constant
    static constexpr float drag_k = 0.5;
    // spring constant
    static constexpr float spring_k = 20;

    //change smoothing width as necessary, the larger the more accurate
    static constexpr float smoothing_width = 50.0f;
   

protected:
    vector<float> m_dState;

    // rest_density = starting density of particles, depends on material properties
    // units kg/m^3
    static constexpr float rest_density = 1000.0f;;

    static constexpr float buoyancy = 0.5f;

};

#endif

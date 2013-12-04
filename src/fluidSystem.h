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
    float calcColorfield(int p1_index, vector<Vector3f>* state);
    float calcNormal(int p1_index, vector<Vector3f>* state);


	void draw();
	void setMD(const vector<float>  & newState) { m_dState = newState; };

    static constexpr float mass = 1;
    static constexpr float g = -2;
    // drag constant
    static constexpr float drag_k = 0.5;
    // spring constant
    static constexpr float spring_k = 20;

    //change smoothing width as necessary, the larger the more accurate
    static constexpr float smoothing_width = 5.0f;

    // rest_density = starting density of particles, depends on material properties
    static constexpr float rest_density = 1.0f;
   
    vector<float> m_dState;

protected:

    // rest density corresponds to starting density of particles, depends on material properties
    //static constexpr float rest_density = 1.0f;

    static constexpr float buoyancy = 0.5f;

};

#endif

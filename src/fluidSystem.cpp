#include <iostream>
#include "fluidSystem.h"
#include <math.h>

#ifndef PI
#define PI 3.14159265359
#endif

FluidSystem::FluidSystem(int numParticles): ParticleSystem(numParticles)
{
    m_numParticles = numParticles;

    // initialize all these particles together somehow ??
    
    for (int i = 0; i < 20; i++) {
	    for (int j = 0; j < 20; j++) {	
		// for this system, we care about the position and the velocity
            m_vVecState.push_back(Vector3f(0.125*j,0,0.125*i));
            m_vVecState.push_back(Vector3f(0.1*j,0,0.25*pow(-1,i)));
        }
	}

}

//using guassian kernel
//h = smoothing width
//r = distance between two particles
float get_kernel(float r, float h) {

	float base = 315.0f/ (64*PI*pow(h,9));
	float smoothing;
	if ( 0.0f <= abs(r) <= h) {
		smoothing = pow( (h*h)-(r*r), 3);
	}	
	else if ( abs(r) > h) {
		smoothing = 0.0f;
	}
	else {
		cerr << "SOMETHING BAD IS HAPPENING WITH THE KERNEL" << endl;
	}
		
	return base*smoothing;
}

// for a given state, evaluate f(X,t)
vector<Vector3f> FluidSystem::evalF(vector<Vector3f> state)
{
	vector<Vector3f> f;
	float mass_density = 0;

    for (int i = 0; i < state.size(); i++) {
        if (i%2 == 0) {
            Vector3f gravity_f = Vector3f(0,mass*g,0);
            Vector3f accel = (gravity_f)/mass;

            f.push_back(state.at(i+1));
            f.push_back(accel);
        }
    }
    
    // We need to have a bounding box so put a min and max
    // on the position vectors for the particles
    
    for (int i = 0; i < state.size(); i++) {
        if (i%2 == 0) {
            
            if (state.at(i).y() < -2.0f) {
                f.at(i).y() = 0;
                f.at(i+1).y() = 0;
            }
            if (state.at(i).x() < -1.0f || state.at(i).x() > 3.0f) {
                f.at(i).x() = 0;
                f.at(i+1).x() = 0;
            }
            if (state.at(i).z() < -1.0f || state.at(i).z() > 3.0f) {
                f.at(i).z() = 0;
                f.at(i+1).z() = 0;
            }
        }
    }


	return f;
}
Vector3f FluidSystem::checkBounds(Vector3f position) {
    if (position.y() < -2.0f) {
        position.y() = -2.0f;
    }
    if (position.x() < -1.0f) {
        position.x() = -1.0f;
    }
    if (position.x() > 3.0f) {
        position.x() = 3.0f;
    }
    if (position.z() < -1.0f) {
        position.z() = -1.0f;
    }
    if (position.z() > 3.0f) {
        position.z() = 3.0f;
    }

    return position;

}

void FluidSystem::draw()
{
	for (int i = 0; i < m_numParticles; i++) {
        m_vVecState.at(i*2) = checkBounds(m_vVecState.at(i*2));
        Vector3f pos = m_vVecState.at(i*2);
		if (particlesOn) {
            glPushMatrix();
            glTranslatef(pos[0], pos[1], pos[2] );
            glutSolidSphere(0.075f,10.0f,10.0f);
            glPopMatrix();
       }

	}
    glPopMatrix();
}



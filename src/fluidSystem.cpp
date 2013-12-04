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
            m_vVecState.push_back(Vector3f(0,0,0));
        }
	}
}
// using gaussian kernel
// h = smoothing width
// r = distance between two particles
float calculateKernel(float r, float h) {

	float base = 315.0f/ (64*PI*pow(h,9));
	float smoothing;
	if ( 0.0f <= abs(r) <= h) {
		smoothing = pow( (h*h)-(r*r), 3);
	}	
	else if ( abs(r) > h) {
		smoothing = 0.0f;
	}
	else {
		cerr << "SOMETHING BAD IS HAPPENING WITH THE <<MASS DENSITY>> KERNEL" << endl;
	}
		
	return base*smoothing;
}

float FluidSystem::calcMassDensity(Vector3f p1_pos, vector<Vector3f>* state) {
    float mass_density;
	//loop through all neighboring particles to get mass_density
    for (int j = 0; j < state->size(); j++) {
        if (j%2 == 0) {
            Vector3f p2_pos = state->at(j);
            float distance = (p1_pos - p2_pos).abs();
            //mass density of particle j on particle i
            float md_i = mass * calculateKernel(distance, smoothing_width);
            mass_density += md_i;
        }
    }
    return mass_density;
}

float calculateKernelPressureGradient ( float r, float h) {
    float base = (-45.0f * r)/ (PI*pow(h,6)*abs(r));
    float smoothing;
	if ( 0.0f <= abs(r) <= h) {
		smoothing = pow(h-abs(r),2);
	}	
	else if ( abs(r) > h) {
		smoothing = 0.0f;
	}
	else {
		cerr << "SOMETHING BAD IS HAPPENING WITH THE <<PRESSURE GRADIENT>> KERNEL" << endl;
	}
		
	return base*smoothing;
}

//not the full pressure gradient, does not take sum, do that in evalF
float FluidSystem::calculatePressureGradient(float p1_md, float p2_md, Vector3f pos1, Vector3f pos2) {
	float pressure_gradient;
    //boltzmann's constant
	float k_b = pow(1.38, -23);
    float p1_pressure = k_b*(p1_md - rest_density);
    float p2_pressure = k_b*(p2_md - rest_density);

    float distance = (pos1-pos2).abs();
    
    float gradient_kernel = calculateKernelPressureGradient(distance, smoothing_width);
    
    float avg_pressure = (p1_pressure + p2_pressure)/(2.0f);
    float vol = mass/p2_pressure;
    return avg_pressure*vol*gradient_kernel;
    
}

float calculateKernalLaplacian(float r, float h) {

    float base = -945.0f/(32*PI*pow(h,9));
    float smoothing;
    if (0.0f <= abs(r) <= h) {
        smoothing = r * pow((pow(h, 2)-pow(r, 2)), 2);
    } else {
        smoothing = 0.0f;
    }

    return base*smoothing;
}

float calculateKernalLaplacian2nd(float r, float h) {

    float base = -945.0f/(32*PI*pow(h,9));
    float smoothing;
    if (0.0f <= abs(r) <= h) {
        smoothing = (pow(h, 2)-pow(r, 2))*(3*pow(h,2)-7*pow(r, 2));
    } else {
        smoothing = 0.0f;
    }

    return base*smoothing;
}

float FluidSystem::calcColorfield(int p1_index, vector<Vector3f>* state) {
    float colorfield;
    for (int j = 0; j < state->size(); j++) {
        if (j%2 == 0) {
            float distance = (state->at(p1_index)-state->at(j)).abs();
            colorfield += 1.0/m_dState.at(j/2) * calculateKernalLaplacian2nd(distance, smoothing_width);
        }
    }
    return colorfield;
}

float FluidSystem::calcNormal(int p1_index, vector<Vector3f>* state) {
    float normal;
    for (int j = 0; j < state->size(); j++) {
        if (j%2 == 0) {
            float distance = (state->at(p1_index)-state->at(j)).abs();
            normal += 1.0/m_dState.at(j/2) * calculateKernalLaplacian(distance, smoothing_width);
        }
    }
    return normal;
}


// for a given state, evaluate f(X,t)
vector<Vector3f> FluidSystem::evalF(vector<Vector3f> state)
{
	vector<Vector3f> f;
    vector<float> m_dCurrent;

    for (int i = 0; i < state.size(); i++) {
        if (i%2 == 0) {

            Vector3f p1_pos = state.at(i);
            m_dCurrent.push_back(calcMassDensity(p1_pos, &state));
        }
    }
    this->setMD(m_dCurrent);

    for (int i = 0; i < state.size(); i++) {
        if (i%2 == 0) {
            Vector3f gravity_f = Vector3f(0,m_dState.at(i/2)*g,0);
            Vector3f buoyancy_f = Vector3f(0, buoyancy*(m_dState.at(i/2)- rest_density)*g, 0);
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



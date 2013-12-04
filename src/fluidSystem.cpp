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

float calculateLaplacianKernelViscosity( float r, float h) {
    float base = (15.0f)/ (2.0*PI*pow(h,3));
    float smoothing;
	if ( 0.0f <= abs(r) <= h) {
        float term1 = pow(abs(r),3)/(2.0f*pow(h,3));
        float term2 = pow(abs(r),2)/pow(h,2);
        float term3 = h/ (2.0f* abs(r));
		smoothing = -term1+term2+term3-1.0f;
	}	
	else if ( abs(r) > h) {
		smoothing = 0.0f;
	}
	else {
		cerr << "SOMETHING BAD IS HAPPENING WITH THE <<PRESSURE GRADIENT>> KERNEL" << endl;
	}
		
	return base*smoothing;
}

float FluidSystem::calculateViscosity(Vector3f pos1, Vector3f pos2) {
    //mu = viscosity coefficient, depends on material properties
    //currently in pascal seconds
    float mu = pow(8.9f, -4); 
    
    

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
    if (pos1 == pos2) {
        return 0.0f;
    }

    return avg_pressure*vol*gradient_kernel;
    
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

            Vector3f gravity_f = Vector3f(0,mass*g,0);
            Vector3f accel = (gravity_f)/mass;

            f.push_back(state.at(i+1));
            f.push_back(accel);
        }
    }
    this->setMD(m_dCurrent);
    
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



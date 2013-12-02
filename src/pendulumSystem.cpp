#include "pendulumSystem.h"
#include <iostream>

PendulumSystem::PendulumSystem(int numParticles):ParticleSystem(numParticles)
{
	m_numParticles = numParticles;
    
	// fill in code for initializing the state based on the number of particles
	for (int i = 0; i < m_numParticles; i++) {
		
		// for this system, we care about the position and the velocity
        m_vVecState.push_back(Vector3f(0,i,0));
        m_vVecState.push_back(Vector3f(0,0,0));

	}

    for (int i = 0; i < m_numParticles-1; i++) {
        Spring s1 = Spring(i*2, i*2+2, 0.5, spring_k);
        spring_map[i*2].push_back(s1);
        Spring s2 = Spring(i*2+2, i*2, 0.5, spring_k);
        spring_map[i*2+2].push_back(s2);
    }
}


// for a given state, evaluate f(X,t)
vector<Vector3f> PendulumSystem::evalF(vector<Vector3f> state)
{
	vector<Vector3f> f;

    for (int i = 0; i < state.size(); i++) {
        if (i%2 == 0) {
            Vector3f gravity_f = Vector3f(0,mass*g,0);
            Vector3f drag_f = -1*drag_k*state.at(i+1);
            Vector3f spring_f = springForces(i, state);

            Vector3f accel = (gravity_f + drag_f + spring_f)/mass;

            f.push_back(state.at(i+1));
            f.push_back(accel);
        }
    }
    // One point has fixed velocity/position
    f[0] = Vector3f(0,0,0);
    f[1] = Vector3f(0,0,0);
    
	return f;
}

Vector3f PendulumSystem::springForces(int i, vector<Vector3f> state) {

    Vector3f spring_f = Vector3f(0,0,0);
    // For all springs point i is attached to
    for (int j = 0; j < spring_map[i].size(); j++) {
        // Get other point attached to spring
        int x_j = spring_map[i][j].p2;

        // Calculations
        Vector3f d = state.at(i) - state.at(x_j);
        spring_f += -1*spring_k * (d.abs()-spring_map[i][j].rest_length) * (d.normalized());

    }
    return spring_f;

}

// render the system (ie draw the particles)
void PendulumSystem::draw()
{
	for (int i = 0; i < m_numParticles; i++) {

        Vector3f pos = m_vVecState.at(i*2);
        if (particlesOn) {
            glPushMatrix();
            glTranslatef(pos[0], pos[1], pos[2] );
            glutSolidSphere(0.075f,10.0f,10.0f);
            glPopMatrix();
        }

        std::vector<Spring> springs = spring_map[i*2];
        for (int k = 0; k < springs.size(); ++k) {
            if (springsOn) {
                glBegin(GL_LINES);
                glVertex(pos);
                glVertex(m_vVecState[springs[k].p2]);
                glEnd();
            }
        }
	}
    glPopMatrix();
}

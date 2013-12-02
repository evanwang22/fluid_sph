#include "ClothSystem.h"
#include <math.h>

ClothSystem::ClothSystem(int numRows, int numColumns):ParticleSystem(numRows*numColumns)
{
	m_numParticles = numRows*numColumns;
    nColumns = numColumns;
    nRows = numRows;
    
	// fill in code for initializing the state based on the number of particles
	for (int i = 0; i < nRows; i++) {
	    for (int j = 0; j <nColumns; j++) {	
		// for this system, we care about the position and the velocity
            m_vVecState.push_back(Vector3f(0.5*j,0,0.5*i));
            m_vVecState.push_back(Vector3f(0,0,0));
        }
	}
    
    vector<Vector3f> spring_diffs;

    // structural springs
    spring_diffs.push_back(Vector3f(0,1,0));
    spring_diffs.push_back(Vector3f(0,-1,0));
    spring_diffs.push_back(Vector3f(1,0,0));
    spring_diffs.push_back(Vector3f(-1,0,0));

    // shear springs
    spring_diffs.push_back(Vector3f(-1,-1,0));
    spring_diffs.push_back(Vector3f(-1,1,0));
    spring_diffs.push_back(Vector3f(1,-1,0));
    spring_diffs.push_back(Vector3f(1,1,0));

    // flex springs
    spring_diffs.push_back(Vector3f(0,2,0));
    spring_diffs.push_back(Vector3f(0,-2,0));
    spring_diffs.push_back(Vector3f(2,0,0));
    spring_diffs.push_back(Vector3f(-2,0,0));

    // structural & shear springs
    for (int i = 0; i <numRows; i++) {
        for (int j = 0; j <numColumns; j++) {
            // get the index of the position
            int index = getIndex(i, j);
            // loop over all possible springs
            for (int k = 0; k < spring_diffs.size(); k++) {
                int d_x = spring_diffs[k].x();
                int d_y = spring_diffs[k].y();
                if (getIndex(i+d_x, j+d_y) != -1) {

                    // structural
                    if (k < 4) {
                        Spring s1 = Spring(index, getIndex(i+d_x, j+d_y), 0.5, spring_k);
                        spring_map[index].push_back(s1);
                    // shear
                    } else if (k < 8) {
                        Spring s1 = Spring(index, getIndex(i+d_x, j+d_y), 0.5*sqrt(2), spring_k);
                        spring_map[index].push_back(s1);
                    // flex
                    } else {
                        Spring s1 = Spring(index, getIndex(i+d_x, j+d_y), 1.0, spring_k);
                        spring_map[index].push_back(s1);
                    }
                }
            }
        }
    }
}

// get the index of a position in state from the row & column
// if there's no particle there return -1
int ClothSystem::getIndex(int i, int j) {
    if (i >= 0 && j >= 0 && i < nRows && j < nColumns) {
        return (nColumns*i+j)*2;
    } else {
        return -1;
    }
}


// for a given state, evaluate f(X,t)
vector<Vector3f> ClothSystem::evalF(vector<Vector3f> state)
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

    // Two fixed points have fixed velocity/position
    f[0] = Vector3f(0,0,0);
    f[1] = Vector3f(0,0,0);
    f[(nColumns-1)*2] = Vector3f(0,0,0);
    f[(nColumns-1)*2+1] = Vector3f(0,0,0);

	return f;
}
Vector3f ClothSystem::springForces(int i, vector<Vector3f> state) {

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
void ClothSystem::draw()
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


#include <iostream>
#include "simpleSystem.h"

using namespace std;

SimpleSystem::SimpleSystem()
{
    m_numParticles = 1;
    m_vVecState.push_back(Vector3f(1,1,1));
}

// for a given state, evaluate f(X,t)
vector<Vector3f> SimpleSystem::evalF(vector<Vector3f> state)
{
	vector<Vector3f> f;

    for (int i = 0; i < state.size(); i++) {
        Vector3f cur = state.at(i);
        Vector3f next_step = Vector3f(cur.y()*(-1), cur.x(), 0);
        f.push_back(next_step);
    }

	return f;
}

// render the system (ie draw the particles)
void SimpleSystem::draw()
{   
    if (particlesOn){
        Vector3f pos = m_vVecState.at(0);
        glPushMatrix();
        glTranslatef(pos[0], pos[1], pos[2] );
        glutSolidSphere(0.075f,10.0f,10.0f);
        glPopMatrix();
    }
}

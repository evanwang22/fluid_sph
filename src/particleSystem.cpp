#include "particleSystem.h"
ParticleSystem::ParticleSystem(int nParticles):m_numParticles(nParticles){
    particlesOn = true;
    springsOn = true;

}

void ParticleSystem::shift(int i, int j) {

    for (int m = 0; m < m_numParticles; m++) {
        m_vVecState.at(m*2) += Vector3f(i, j, 0);
    }
}

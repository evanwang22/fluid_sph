#include "TimeStepper.h"

void ForwardEuler::takeStep(ParticleSystem* particleSystem, float stepSize)
{
    vector<Vector3f> current_state = particleSystem->getState();
    vector<Vector3f> evalF_state = particleSystem->evalF(current_state);
    vector<Vector3f> new_state;
    
    for (int i = 0; i < current_state.size(); i++) {
        Vector3f cur = current_state.at(i);
        Vector3f next_step = cur + stepSize*(evalF_state.at(i));
        new_state.push_back(next_step);
    }

    particleSystem->setState(new_state);
    
}

void Trapzoidal::takeStep(ParticleSystem* particleSystem, float stepSize)
{
    vector<Vector3f> current_state = particleSystem->getState();
    vector<Vector3f> evalF_state = particleSystem->evalF(current_state);
    vector<Vector3f> euler_state;
    vector<Vector3f> new_state;
    
    // Calculate Euler's method
    for (int i = 0; i < current_state.size(); i++) {
        Vector3f cur = current_state.at(i);
        Vector3f next_step = cur + stepSize*(evalF_state.at(i));
        euler_state.push_back(next_step);
    }

    vector<Vector3f> evalF_euler = particleSystem->evalF(euler_state);
    
    // Using eulers, calculate trapezoid step
    for (int i=0; i < current_state.size(); i++) {
        Vector3f cur = current_state.at(i);
        Vector3f sum_force = evalF_state.at(i)+evalF_euler.at(i);
        Vector3f trap_step = cur + stepSize*(0.5)*sum_force;
        new_state.push_back(trap_step);
    }

    particleSystem->setState(new_state);
}

// Own implementation of RK4
void RungeKutta::takeStep(ParticleSystem* particleSystem, float stepSize) {

    vector<Vector3f> current_state = particleSystem->getState();
    vector<Vector3f> evalF_state = particleSystem->evalF(current_state);
    vector<Vector3f> new_state;

    vector<Vector3f> k1;
    vector<Vector3f> k2;
    vector<Vector3f> k3;
    vector<Vector3f> k4;

    vector<Vector3f> k2_helper;
    vector<Vector3f> k3_helper;
    vector<Vector3f> k4_helper;

    for (int i = 0; i < current_state.size(); i++) {
        Vector3f next_step = stepSize*(evalF_state.at(i));
        k1.push_back(next_step);
        Vector3f helper_step = current_state.at(i) + next_step/2;
        k2_helper.push_back(helper_step);
    }
    
    vector<Vector3f> k2_helper_eval = particleSystem->evalF(k2_helper);

    for (int i = 0; i < current_state.size(); i++) {
        Vector3f next_step = stepSize*(k2_helper_eval.at(i));
        k2.push_back(next_step);
        Vector3f helper_step = current_state.at(i) + next_step/2;
        k3_helper.push_back(helper_step);
    }

    vector<Vector3f> k3_helper_eval = particleSystem->evalF(k3_helper);

    for (int i = 0; i < current_state.size(); i++) {
        Vector3f next_step = stepSize*(k3_helper_eval.at(i));
        k3.push_back(next_step);
        Vector3f helper_step = current_state.at(i) + next_step;
        k4_helper.push_back(helper_step);
    }

    vector<Vector3f> k4_helper_eval = particleSystem->evalF(k4_helper);

    for (int i = 0; i < current_state.size(); i++) {
        Vector3f next_step = stepSize*(k4_helper_eval.at(i));
        k4.push_back(next_step);
    }

    for (int i = 0; i < current_state.size(); i++) {
        Vector3f real_step = current_state.at(i) + k1.at(i)/6 + k2.at(i)/3 + k3.at(i)/3 + k4.at(i)/6;
        new_state.push_back(real_step);
    }

    particleSystem->setState(new_state);
}

#include "ParticleFilter.h"

/**
 * @brief uses a particle filtering algorithm to run one step of 
*/
void ParticleFilter::runPF(/*a particle filtering algo?, initial states?*/){
    
    //initialize states:

    // run pf

    // get particles and propogate states based on that 

    return;
}

/**
 * @brief takes in particles and uses runge kutta to propogate states for each particle on it for timestep Dt
 * 
 * @param particles particles to propogate for time Dt
 * @param Dt time step in seconds to run 
*/
void ParticleFilter::propogate(){
    //runs RK on each particle for 
    return;
}

/**
 * @brief takes in all the current particles and current measurements to calculate current probablility for each particle
 * 
 * @param particles some sort of array or list of n particles
 * @param meas current altitude measurement 
*/
void ParticleFilter::update(){
    //loop through all the particles and update weights
    return;
}

/**
 * @brief integrates a particle's state IN PLACE for timestep Dt
 * 
*/
void ParticleFilter::RK4(Particle& p, double Dt){
    Eigen::Matrix<double,6,1> k1,k2,k3,k4;
    k1 = f_orbital(p.state);
    k2 = f_orbital(p.state+(Dt*k1/2.0));
    k3 = f_orbital(p.state+(Dt*k2/2.0));
    k4 = f_orbital(p.state+(Dt*k3));
    p.state = p.state + (Dt/6.0*(k1+2.0*k2+2.0*k3+k4));
}

/**
 * @brief returns the continuous derivative dx of the state x for orbital motion
*/
Eigen::Matrix<double,6,1> ParticleFilter::f_orbital(Eigen::Matrix<double,6,1> x){
    Eigen::Vector3d r {x[0],x[1],x[2]};
    Eigen::Vector3d dv = _mu_moon/(pow(r.norm(),3))*r;
    
    Eigen::Matrix<double,6,1> dx;
    dx << x[3],x[4],x[5],dv[0],dv[1],dv[2];
    
    return dx;
}
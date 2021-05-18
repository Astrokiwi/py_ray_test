/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   coupling.h
 * Author: davidjwilliamson
 *
 * Created on 13 April 2021, 16:25
 */


#ifndef ARRAYCOUPLER_H
#define ARRAYCOUPLER_H

#include <vector>
#include "rt_prototypes.h"
#include "coupling.h"

struct Particle {
    double Pos[3];
    double OpticalDepth;
    double smoothing;
    double opacity;
    double mass;
    int proc;
};

class ArrayParticlePositionCoupler: public ParticlePositionCoupler {
public:
    std::vector<struct Particle> particles;

    int Nlocal,Ntot;
    
    double constant_mass = 1.;

    double *position(int i) override;
    double mass(int i) override;
    double smoothing(int i) override;
    double opacity(int i) override;
    bool isDusty(int i) override;
    bool isGas(int i) override;
    bool isActive(int i) override;
    bool isAlive(int i) override;
    long long allN() override;
    long long localN() override;
    long long allGasN() override;
    
};

#endif /* ARRAYCOUPLER_H */


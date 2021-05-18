#ifndef MPITOOLS_H
#define MPITOOLS_H

#include "ArrayParticlePositionCoupler.h"
#include <memory>

std::shared_ptr<ArrayParticlePositionCoupler> distribute_data(std::vector<Particle> &allData, int Ntot);
std::shared_ptr<std::vector<Particle>> gather_data(std::shared_ptr<ArrayParticlePositionCoupler> particlePositionCoupler);
std::vector<double> gather_tau(double *thistau,int localN,int allN);

#endif
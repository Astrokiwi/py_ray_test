/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   testdata.h
 * Author: davidjwilliamson
 *
 * Created on 16 April 2021, 10:09
 */

#ifndef TESTDATA_H
#define TESTDATA_H

#include <memory>
#include "ArrayParticlePositionCoupler.h"
//
//struct TestData {
//    double Pos[3];
//    double OpticalDepth;
//};
//
//extern struct TestData *testpositions;
//
//extern int Nlocal,Ntot;

std::shared_ptr<ArrayParticlePositionCoupler> generate_test_data();
//std::shared_ptr<std::vector<Particle>> gather_data(std::shared_ptr<ArrayParticlePositionCoupler> particlePositionCoupler);
void dump_positions(std::shared_ptr<ArrayParticlePositionCoupler> particlePositionCoupler);

//std::vector<double> gather_tau(double *thistau,int localN,int allN);
std::vector<double> test_depths();

void run_test();

#endif /* TESTDATA_H */

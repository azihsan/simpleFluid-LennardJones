#include <stdio.h>
#include <math.h>
#include "vector.hpp"
#include "operation.hpp"
#include "molecule.hpp"
#include "mdfunctions.hpp"

void init_pos(molecule *atom, double density, int n_atom){

	int i=0, j=0, n=0;
    // here gap vector is distance that should go between two atom
    // pos is the position where atom will occupy in cartesian coordinate
    // region is length of square box
    // initUnitCell is total nxn unit cell in the square box
    // all of these three are from vectors class such as having the x, y direction and other methods

    vectors gap, pos, region, initUnitCell;

    region.setVector(1./sqrt(density) * n_atom, 1./sqrt(density) * n_atom);

    initUnitCell.setVector_int(n_atom, n_atom);

    gap.setVector(region.getXVector()/initUnitCell.getXVector_int(), region.getYVector()/initUnitCell.getYVector_int());
    
    // positioning process of the atoms(initial position)
    // Here atoms are arranged into 2D lattice structure.
    while (i < initUnitCell.getXVector_int()){

        while (j < initUnitCell.getYVector_int()){

            pos.setVector((double)i + 0.5, (double)j + 0.5);
            
            //  the position follow the rule -> atom_position = (position * gap) + (-0.5 * region)

            atom[n].setPositionVect(operation::vectAdd(operation::vectDot(pos, gap),operation::vectScale(region, -0.5)));

            n += 1;
            j += 1;
        }
        i += 1;
        j = 0;
    }
}

void init_acc(molecule *atom, int n_atom){
    int i = 0;
//acceleration initialization
    while (i < n_atom * n_atom){
        //set the acceleration value in x and y direction to 0
        atom[i].setAcceleration(0.,0.);
        i++;
    }
}

void init_pot(molecule *atom, int n_atom){
    int i = 0;
    while (i < n_atom * n_atom){
        //set the potential energy value of an atom to 0
        atom[i].setPotential(0.);
        i++;
    }
}
void init_kin(molecule *atom, int n_atom){
    int i = 0;
    while (i < n_atom * n_atom){
        //set the kinetic energy value of an atom to 0
        atom[i].setKinetic(0.);
        i++;
    }
}

void reset(molecule *atom, int n_atom){
    init_acc(atom, n_atom);
    init_pot(atom, n_atom);
    init_kin(atom, n_atom);
}

//  pseudorand.h
//  This pseudorandom generator number based on LCG(Linear Congruential Generator), Monte Carlo method 
//  by D.H. Lehmer(1948)
//  the equation for deriving this number is based on:
//  s[i+1] = ( a * s[i] + c ) % m
//  where a = well chosen multiplier, m = equal to/ slightly smaller than the largest integer that can be repsented in one computer word, and c = is constant
//  MolDyn

int randSeedP = 17;
double randDouble(){
    randSeedP = (randSeedP * IMUL + IADD) & MASK;
    return (randSeedP * SCALE);
}

void pseudorand(molecule *atom, double temperature, int n_atom){
    double s;
    s = 2. * PI * randDouble();
    atom->setVelocity(sqrt(2*temperature*(1.-(1./n_atom)))*cos(s), sqrt(2*temperature*(1.-(1./n_atom)))*sin(s));
}

void wrappingVect(vectors *r, double density, int n_atom)
{
    vectors region;
    region.setVector((1./sqrt(density) * n_atom), (1./sqrt(density) * n_atom));

    // later I use atom position as arguments to place vectors r.
    // this is conditional if to check whether r in x direction greater than half of region

    if (r->getXVector() >= 0.5 * region.getXVector())
    {
        r->setXVector(r->getXVector() - region.getXVector());
    }
    else if (r->getXVector() < -0.5 * region.getXVector())
    {
        r->setXVector(r->getXVector() + region.getXVector());
    }
      // this is conditional if to check whether r in y direction greater than half of region
    if (r->getYVector() >= 0.5 * region.getYVector())
    {
        r->setYVector(r->getYVector() - region.getYVector());
    }
    else if (r->getYVector() < -0.5 * region.getYVector())
    {
        r->setYVector(r->getYVector() + region.getYVector());
    }

}

void wrapping(molecule *atom, double density, int n_atom){
    //periodic boundary condition wrapping

    // length of the box. vectors data type, means region will have a value in x and y direction
    vectors region;
    region.setVector((1./sqrt(density) * n_atom), (1./sqrt(density) * n_atom));

    // later I use atom position as arguments to place vectors r.
    // this is conditional if to check whether r in x direction greater than half of region

    if (atom->getXPosition() >= 0.5 * region.getXVector())
    {
        atom->setXPosition(atom->getXPosition() - region.getXVector());
    }
    else if (atom->getXPosition() < -0.5 * region.getXVector())
    {
        atom->setXPosition(atom->getXPosition() + region.getXVector());
    }

    // this is conditional if to check whether r in y direction greater than half of region
    if (atom->getYPosition() >= 0.5 * region.getYVector())
    {
        atom->setYPosition(atom->getYPosition() - region.getYVector());
    }
    else if (atom->getYPosition() < -0.5 * region.getYVector())
    {
        atom->setYPosition(atom->getYPosition() + region.getYVector());
    }
    
}

//this function to wrap all the atom.
void wrapping_all(molecule *atom, double density, int n_atom){
    for (int i = 0; i< n_atom * n_atom; i++)
    {
        wrapping(&atom[i], density, n_atom);
    }
}


void pot_force(molecule *atom, double density, int n_atom){
   // local variable declaration for doing force calculation
    int i , j ;

    // deltaR is distance between two atoms and tempForce is temporary value of force
    vectors deltaR, tempForce;

    // rCutoff is cut-off distance which limit whether two atoms potential energy can be calculated.
    // if they are more than cut off they will not be calculated.
    // rrCutOff is square value of rCutOff
    // magDeltaRR distance square between two atoms
    // magDeltaRRi is reciprocal value of magDeltaRR
    // magDeltaRR3i is cube of magDeltaRRi
    // potVal is a temporary value that later will be assigned to molecule private member with molecule::setPotential(double a)
    // forceVal is a temporary value that later will be assigne to molecule private member with molecule::setKinetic(double a)

    double rCutOff, rrCutOff, mag_deltaRR, mag_deltaRRi, mag_deltaRR3i, potVal, forceVal;
    rCutOff  = pow(2. , 1./6);
    rrCutOff = rCutOff * rCutOff;

//force and potential initialization
    reset(atom, n_atom);
    tempForce.setVector(0., 0.);
    for ( i = 0; i < n_atom * n_atom ; i++){
        for ( j = i+1 ; j < n_atom * n_atom ; j++){
            
            // calculate distance between two molecules (delta R)
            deltaR = operation::vectSub(atom[i].getPosition(), atom[j].getPosition());
            
            // normalize the distance between two atoms
            wrappingVect(&deltaR, density, n_atom);

            // calculate the magnitude of squared distance(r^2) between two atom vectors.
            mag_deltaRR = operation::vectSquare(deltaR);
        
            // distance between two atoms(magnitude) should be less than cut-off distance(rcut); to avoid discontinuity.
            if (mag_deltaRR < rrCutOff){
               
                // this is somewhat make easier
                mag_deltaRRi = 1./mag_deltaRR;
                mag_deltaRR3i = mag_deltaRRi * mag_deltaRRi * mag_deltaRRi; //-> (magDeltaR)^(-6)
    
                // calculate potential(lennard jones 6-12) every atom
                potVal = 4. * mag_deltaRR3i * (mag_deltaRR3i - 1.) + 1. ;
                
                atom[i].setPotential(atom[i].getPotential() + potVal);
                
                // calculate force for each atom
                forceVal = 48. * mag_deltaRR3i * (mag_deltaRR3i - 0.5) * mag_deltaRRi;
                tempForce.setVector(forceVal * deltaR.getXVector(), forceVal * deltaR.getYVector());
                
                
                // get acceleration from the force, meanwhile the newton's 3rd law happens, f(ji) = -f(ij).
                
                atom[i].setAccelerationVect(operation::vectAdd(atom[i].getAcceleration(), tempForce)); //vector addition operation
                atom[j].setAccelerationVect(operation::vectSub(atom[j].getAcceleration(), tempForce)); //vector substraction operation
              
              }
        }
    }

}


void leapfrog(molecule *atom, double time_step, double density, int n_atom, int state){


    /* state == 0 here is integrate at first half step. Half step means half time step advance for velocity
     */
    if (state == 0){
        for (int i = 0 ; i < n_atom * n_atom ; i++){
        atom[i].setVelocityVect(operation::vectAdd(atom[i].getVelocity(), operation::vectScale(atom[i].getAcceleration(), time_step/2.)));
        atom[i].setPositionVect(operation::vectAdd(atom[i].getPosition(), operation::vectScale(atom[i].getVelocity(), time_step)));

         }
    }

    // then state ==1 is calculation for the rest half of integration.

    else if(state == 1){
        for (int i = 0 ; i < n_atom * n_atom ; i++)
        {
        	atom[i].setVelocityVect(operation::vectAdd(atom[i].getVelocity(), operation::vectScale(atom[i].getAcceleration(),time_step/2.)));
        }
                                }
}


// here is a calculation for total energy calculation of system, after force and potential calculation, the system
// will obtain potential energy and kinetic energy. And total energy is sum of both of them.


double totalKineticEnergyCalc(molecule *atom, int n_atom){
    // local variable declaration for temporary value such as tempEneTotal
     // velMag is a square velocity magnitude

    double tempKineticEnergyTotal = 0, velMag2;
    for(int i = 0; i < n_atom * n_atom; i++){
        velMag2 = pow(atom[i].getXVelocity(),2) + pow(atom[i].getYVelocity(),2);
        atom[i].setKinetic(0.5*velMag2); // EK = 1/2 * mass * vel^2 with mass = 1 md unit
        tempKineticEnergyTotal += (atom[i].getKinetic());
    }
    return (tempKineticEnergyTotal);
    }

double totalPotentialEnergyCalc(molecule *atom, int n_atom){
    double tempPotentialEnergyTotal = 0, velMag2;
    for(int i = 0; i < n_atom * n_atom; i++){
        tempPotentialEnergyTotal += (atom[i].getPotential());
    }
    return (tempPotentialEnergyTotal);
    }

double totalEnergyCalc(molecule *atom, int n_atom){
  // local variable declaration for temporary value such as tempEneTotal
   // velMag is a square velocity magnitude
    double tempEneTotal = 0, velMag2;

    for(int i = 0; i < n_atom * n_atom; i++){
        velMag2 = pow(atom[i].getXVelocity(),2) + pow(atom[i].getYVelocity(),2);
        atom[i].setKinetic(0.5*velMag2); // EK = 1/2 * mass * vel^2 with mass = 1 md unit
        tempEneTotal += (atom[i].getKinetic() + atom[i].getPotential());
    }
    return (tempEneTotal);
    }




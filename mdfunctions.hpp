#ifndef mdFunctions_h
#define mdFunctions_h
#include "molecule.hpp"




//for pseudo random parameter
#define PI 3.14159265359
#define IADD 453806245
#define IMUL 314159269
#define MASK 2147483647
#define SCALE 0.4656612873e-9

// initiation position of atoms
void init_pos(molecule *atom, double density, int n_atom);

// initiation acceleration of atoms, assign acceleration value to 0
void init_acc(molecule *atom, int n_atom);

// initiation of potential energy of atoms, assign a value to 0
void init_pot(molecule *atom, int n_atom);

// initiation of kinetic energy of atoms, assign a value to 0
void init_kin(molecule *atom, int n_atom);

/*  resetting the accelaration, kinetic energy, and potential energy
    basically this function takes init_acc, init pot, init pos in a set of reset function*/
void reset(molecule *atom, int n_atom);

// assigning the velocity to each atom based on pseudorandom
double randDouble();
void pseudorand(molecule *atom, double temperature, int n_atom);

/* wrappingVect is wrapping function for vector input.
 * Both wrapping and wrapping_all function are function for wrapping(periodic boundary condition).
 * Wrapping_all takes all atom to be wrapped.
 */
void wrappingVect(vectors *r, double density, int n_atom);
void wrapping(molecule *atom, double density, int n_atom);
void wrapping_all(molecule *atom, double density, int n_atom);

/* pot_force is a function to calculate potential energy and force. The reason why I combine both of them in one function
 * due to the force is a further calculation of potential energy which is negative gradient of potential energy.
 */
void pot_force(molecule *atom, double density, int n_atom);

/*leapfrog function is a function of integrator. Means, we integrate an acceleration to obtain velocity and position.
 *leapfrog function takes state as an argument, due to the algorithm of leapfrog that advance in half step then continue
 *half other step after force calculation.
 */
void leapfrog(molecule *atom, double time_step, double density, int n_atom, int state);

/* Here is the funciton to calculate total potential energy, kinetic energy and total energy of system.
 * which kinetic energy is the sum of total potential energy and total kinetic energy
 */
double totalPotentialEnergyCalc(molecule *atom, int n_atom);
double totalKineticEnergyCalc(molecule *atom, int n_atom);
double totalEnergyCalc(molecule *atom, int n_atom);



#endif /* mdFunctions_h */

//
//  molecule.hpp
//  MolDyn
//
//  Created by Ahmad Zainul Ihsan on 12/20/16.
//  Copyright © 2016 azihsan. All rights reserved.
//
// Molecule Class
// Idea behind this class is to collect all the objects called atoms that have all same properties.
// later on an object called atom having position, velocity, and acceleration with the vector data type.
// and the properties governed from those 3 vectors(position, velocity, acceleration)
// are potential and kinetic that will have double data type.
// I use this class in all calculation.

#ifndef molecule_hpp
#define molecule_hpp

#include <stdio.h>
#include <iostream>


#include "vector.hpp"




class molecule{
private:

    // I put vector pos, vel, acc, double potential, kinetic in private member due to the safety of the each member

    vectors pos; // an atom will have POSITION in x and y direction
    vectors vel; // an atom will have VELOCITY in x and y direction
    vectors acc; // an atom will have ACCELERATION in x and y direction
    double potential; //potential energy due to the interaction with other this value will be owned by an atom
    double kinetic; //kinetic energy due to the movement of an atom inside the system an atom will have a velocity

public:

    // Below I have Set and Get method for defining Position.
    void setPositionVect(vectors a); // here I can also define position that will take vector as an input
    void setPosition(double a, double b); // method to set a value for position in x and y direction
    void setXPosition(double a); // method to set a value for position in x direction
    void setYPosition(double a); // method to set a value for position in y direction
    vectors getPosition(); // method to get a value for position in x and y direction
    double getXPosition(); // method to get a value for position in x direction
    double getYPosition(); // method to get a value for position in y direction
    
    // Below I have Set and Get method for defining Velocity.
    void setVelocityVect(vectors a); // here I can also define velocity  that will take vector as an input
    void setVelocity(double a, double b); // method tp set a value for velocity in x and y direction
    void setXVelocity(double a); // method to set a value for velocity in x direction
    void setYVelocity(double a); // method to set a value for velocity in y direction
    vectors getVelocity(); // method to get a value for velocity in x and y direction
    double getXVelocity(); //method to get a value for velocity in x direction
    double getYVelocity(); //method to get a value for velocity in y direction
    
    // Below I have Set and Get method for defining Acceleration.
    void setAccelerationVect(vectors a); // here I can also define acceleration that will take vector as an input
    void setAcceleration(double a, double b);  // method tp set a value for acceleration in x and y direction
    void setXAcceleration(double a);  // method tp set a value for acceleration in x direction
    void setYAcceleration(double a);  // method tp set a value for acceleration in y direction
    vectors getAcceleration(); // method to get a value for velocity in x and y direction
    double getXAcceleration(); // method to get a value for velocity in x direction
    double getYAcceleration(); // method to get a value for velocity in y direction
    
    //Set and get for atom's potential energy
    void setPotential(double a);
    double getPotential();
    
    //Set and get for atom's kinetic energy
    void setKinetic(double a);
    double getKinetic();
    
    // Later on, these methods for helping to identify/observe the dynamic of atoms by printing atom position, velocity, and acceleration.
    void printPosition();
    void printVelocity();
    void printAcceleration();
    

    
};

#endif /* molecule_hpp */

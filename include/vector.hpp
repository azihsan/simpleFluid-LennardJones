//
//  vector.hpp
//  MolDyn
//
//  Created by Ahmad Zainul Ihsan on 12/12/16.
//  Copyright © 2016 azihsan. All rights reserved.
//
// Here, the class for declaring the structure of vector, which every vector has two value in x and y direction
// Private member for the data of x and y having double and integer value as the data type.
// Public member is specialized for methods, Set and Get value to the vector and also printing the value of a vector.

#ifndef vector_hpp
#define vector_hpp


#include <stdio.h>

class vectors{
private :
    double x; // data for vector in x directin
    double y; // data for vector in y direction
    int x_int; // integer data for vector in x direction
    int y_int;  // integer data for vector in y direction
    
public :
    void setVector(double a, double b); //method to set the vector that takes argument a as x and b as y
    void setVector_int(int a, int b); // Reason I apply the integer vector is to define the simulation box that only take integet value.
    void setXVector(double a); // method to set value in x direction
    void setYVector(double a); // method to set value in y direction
    double getXVector(); // method to get value in x direction
    double getYVector(); // method to get value in y direction
    int getXVector_int(); // method to get integer value in x direction
    int getYVector_int(); // method to get integer value in y direction
    void printVector(); // method to print vector, to be used for analysing the molecule.
};

#endif /* vector_hpp */

//
//  operation.hpp
//  MolDyn
//
//  Created by Ahmad Zainul Ihsan on 12/12/16.
//  Copyright © 2016 azihsan. All rights reserved.
//
// the idea behind this class, i collect all the operations that will be used for other class,
// thus the methods are static.
// will be found in the calculation e.g. operation::vectAdd(a,b)
// especially molecule class such that i make a magic box for vector operation.

#ifndef operation_hpp
#define operation_hpp

#include <stdio.h>
#include <iostream>
#include "vector.hpp"

class operation{
    //all methods are in public data member and using static data member due to this operation will be used for any other class without defining the object
    //this operation class is used within the mdfunctions in order to get simplicity.
public :
    static vectors vectAdd(vectors a, vectors b); //method for 2 vectors addition
    static vectors vectSub(vectors a, vectors b); //method for 2 vectors submission
    static vectors vectDot(vectors a, vectors b); //method for 2 vectors multipilication
    static double vectSquare(vectors a); //method for squaring the a vector
    static vectors vectCube(vectors a, vectors b, vectors c); // method for 3 vectors multiplication
    static vectors vectDiv(vectors a, vectors b); //method for 2 vectors division
    static vectors vectScale(vectors a, double b);//method for scaling a vector
};

#endif /* operation_hpp */

//
//  operation.cpp
//  MolDyn
//
//  Created by Ahmad Zainul Ihsan on 12/12/16.
//  Copyright © 2016 azihsan. All rights reserved.
//

#include "operation.hpp"
#include "vector.hpp"


//this method defines the vector addition, I use the Set method coming from vector class
vectors operation::vectAdd(vectors a, vectors b){
    a.setVector(a.getXVector() + b.getXVector(), a.getYVector() + b.getYVector());
    return (a);
}

//this method defines the vector submission, I use the Set method coming from vector class
vectors operation::vectSub(vectors a, vectors b){
    a.setVector(a.getXVector() - b.getXVector(), a.getYVector() - b.getYVector());
    return (a);
}

//this method defines the vector multiplication(dot), I use the Set method coming from vector class
 vectors operation::vectDot(vectors a, vectors b){
     a.setVector(a.getXVector() * b.getXVector(), a.getYVector() * b.getYVector());
     return (a);
 };

//this method defines the vector division, I use the Set method coming from vector class
vectors operation::vectDiv(vectors a, vectors b){
    a.setVector(a.getXVector() / b.getXVector(), a.getYVector() / b.getYVector());
    return (a);
}

double operation::vectSquare(vectors a){
    return(a.getXVector() * a.getXVector() + a.getYVector() * a.getYVector());
}
//this method defines the vector cube, I use the Set method coming from vector class
vectors operation::vectCube(vectors a, vectors b, vectors c){
    a.setVector(a.getXVector() * b.getXVector() * c.getXVector(), a.getYVector() * b.getYVector() * c.getYVector());
    return (a);
}
//this method defines the scaling vector, I use the Set method coming from vector class
vectors operation::vectScale(vectors a, double b){
    a.setVector(a.getXVector() * b, a.getYVector() * b);
    return (a);
}


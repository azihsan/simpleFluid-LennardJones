//
//  vector.cpp
//  MolDyn
//
//  Created by Ahmad Zainul Ihsan on 12/12/16.
//  Copyright © 2016 azihsan. All rights reserved.
//
// Here, I define what is the method in vector class(vector.hpp)
// The get and set method as bridging from private to public data member.

#include "vector.hpp"
#include <iostream>

using namespace std;

// set vector from 2 double(a and b) values. Later on a wil be assigned to x and b will be assigned to y
void vectors::setVector(double a, double b){
    x = a;
    y = b;
}
void vectors::setVector_int(int a, int b){
    x_int = a;
    y_int = b;
}
void vectors::setXVector(double a){
    x = a;
}
void vectors::setYVector(double a){
    y = a;
}
// this is method to get the value from private data vector(x and y)
double vectors::getXVector(){
    return (x);
}
double vectors::getYVector(){
    return (y);
}
int vectors::getXVector_int(){
    return (x_int);
}
int vectors::getYVector_int(){
    return (y_int);
}
// this is method for printing the vector value.
void vectors::printVector(){
    cout <<"X coordinate is "<<x<<" and Y coordinate is "<<y<<endl;}


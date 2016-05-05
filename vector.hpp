//
//  vector.hpp
//  CG_Prak1
//
//  Created by max on 28/09/15.
//  Copyright © 2015 max kohler. All rights reserved.
//

#ifndef vector_hpp
#define vector_hpp

#include <iostream>
#include <stdio.h>



class Vector
{
public:
    float X;
    float Y;
    float Z;
    
    Vector(float x, float y, float z);
    Vector();
    float dot(const Vector& v) const;
    Vector cross(const Vector& v) const;
    Vector operator+(const Vector& v) const;
    Vector operator-(const Vector& v) const;
    Vector& operator+=(const Vector& v);
    Vector operator*(float c) const;
    Vector operator-() const;
    Vector& normalize();
    float length() const;
    float lengthSquared() const;
    Vector reflection(const Vector& normal) const;
    bool triangleIntersection(const Vector& d, const Vector& a, const Vector& b, const Vector& c, float& s) const;
};

#endif /* vector_hpp */
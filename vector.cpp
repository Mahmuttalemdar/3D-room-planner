//
//  vector.cpp
//  CG_Prak1
//
//  Created by max on 28/09/15.
//  Copyright © 2015 max kohler. All rights reserved.
//

#include "vector.hpp"
#include <Math.h>

Vector::Vector()
{
    this->X = 0.0f;
    this->Y = 0.0f;
    this->Z = 0.0f;
}

Vector::Vector(float x_par, float y_par, float z_par)
{
    this->X = x_par;
    this->Y = y_par;
    this->Z = z_par;
}

float Vector::dot(const Vector& v) const
{
    return (this->X * v.X + this->Y * v.Y + this->Z * v.Z);
}

Vector Vector::cross(const Vector& v) const
{
    Vector kreuzprodukt;
    
    kreuzprodukt.X = this->Y * v.Z - this->Z * v.Y;
    kreuzprodukt.Y = this->Z * v.X - this->X * v.Z;
    kreuzprodukt.Z = this->X * v.Y - this->Y * v.X;
    
    return kreuzprodukt;
}

Vector Vector::operator+(const Vector& v) const
{
    Vector ergebnis;
    ergebnis.X = this->X + v.X;
    ergebnis.Y = this->Y + v.Y;
    ergebnis.Z = this->Z + v.Z;
    return ergebnis;
}

Vector Vector::operator-(const Vector& v) const
{
    
    Vector ergebnis;
    ergebnis.X = this->X - v.X;
    ergebnis.Y = this->Y - v.Y;
    ergebnis.Z = this->Z - v.Z;
    return ergebnis;
    
}

Vector& Vector::operator+=(const Vector& v)
{
    this->X = this->X + v.X;
    this->Y = this->Y + v.Y;
    this->Z = this->Z + v.Z;
    return *this;
}

Vector Vector::operator*(float c) const
{
    Vector ergebnis;
    ergebnis.X = this->X * c;
    ergebnis.Y = this->Y * c;
    ergebnis.Z = this->Z * c;
    return ergebnis;
}

Vector Vector::operator-() const
{
    Vector ergebnis;
    ergebnis.X = this->X;
    ergebnis.Y = this->Y;
    ergebnis.Z = this->Z;
    ergebnis = ergebnis * (-1.0f);
    return ergebnis;
}

float Vector::lengthSquared() const
{
    return (this->X*this->X + this->Y*this->Y + this->Z*this->Z);
}

float Vector::length() const
{
    return sqrtf(this->lengthSquared());
}

Vector& Vector::normalize()
{
    float laenge = this->length();
    
    *this= *this * (1.0f/laenge);
    return *this;
}

Vector Vector::reflection(const Vector& normal) const
{
    Vector einfallenNormalisiert = *this;
    einfallenNormalisiert.normalize();
    
    Vector normalNormalisiert = normal;
    normalNormalisiert.normalize();
    
    Vector ergebnis;
    ergebnis = *this - (normal*this->dot(normal))*2; //r=d−2(d⋅n)n
    // h = (r + v) / 2
    // 2h = r + v
    // 2h - v = r
    //ergebnis = normalNormalisiert*2.0f;
    //ergebnis = ergebnis - einfallenNormalisiert;
    
    //std::cout << "this " << this->X << " " << this->Y << " " << this->Z << std::endl;
    //std::cout << "ergebnis " << ergebnis.X << " "  << ergebnis.Y << " " << ergebnis.Z << std::endl;
    
    return ergebnis;
}

bool Vector::triangleIntersection(const Vector& d, const Vector& a, const Vector& b, const Vector& c, float& s) const
{
    
    
    Vector normalenVektor = ((b-a).cross(c-a)).normalize();
    
    float EbenengleichungD = a.dot(normalenVektor);
    
    s = (EbenengleichungD - (normalenVektor.dot(*this))) / normalenVektor.dot(d);
    
    Vector auftreffpunkt = (*this) + (d * s);
    
    
    if(s <= 0) //strahl zeigt in falsche richtung
    {
        return false;
    }
    
    if(normalenVektor.dot(d) == 0)  // strahl und ebene verlaufen parallel
    {
        return false;
    }
    
    Vector crossABP = (b-a).cross(auftreffpunkt-a);
    float flaecheABP = crossABP.length() / 2;
    
    Vector crossBCP = (c-b).cross(auftreffpunkt-b);
    float flaecheBCP = crossBCP.length() / 2;
    
    Vector crossCAP = (c-a).cross(auftreffpunkt-a);
    float flaecheCAP = crossCAP.length() / 2;
    
    Vector crossGesamt = (b-a).cross(c-a);
    float flaecheGesamt = crossGesamt.length() / 2;
    
    
    if((flaecheGesamt - (flaecheABP+flaecheBCP+flaecheCAP)) > -0.0001)
    {
        return true;
    }
    else    //Flächen passen nicht
    {
        return false;
    }
    
}




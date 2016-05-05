//
//  color.cpp
//  CG_Prak1
//
//  Created by max on 29/09/15.
//  Copyright © 2015 max kohler. All rights reserved.
//

#include "color.hpp"


Color::Color()
{
    this->R = 0;
    this->G = 0;
    this->B = 0;
}

Color::Color(float r_par, float g_par, float b_par)
{
    this->R = r_par;
    this->G = g_par;
    this->B = b_par;
}

Color Color::operator*(const Color& c) const
{
    Color ergebnis;
    ergebnis.R = this->R * c.R;
    ergebnis.G = this->G * c.G;
    ergebnis.B = this->B * c.B;
    
    return ergebnis;
}


Color Color::operator*(const float Factor) const
{
    Color ergebnis;
    ergebnis.R = this->R * Factor;
    ergebnis.G = this->G * Factor;
    ergebnis.B = this->B * Factor;
    
    return ergebnis;
}

Color Color::operator+(const Color& c) const
{
    Color ergebnis;
    ergebnis.R = this->R + c.R;
    ergebnis.G = this->G + c.G;
    ergebnis.B = this->B + c.B;
    
    return ergebnis;
}

Color& Color::operator+=(const Color& c)
{
    this->R += c.R;
    this->G += c.G;
    this->B += c.B;
    
    return *this;
}
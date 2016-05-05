//
//  color.hpp
//  CG_Prak1
//
//  Created by max on 29/09/15.
//  Copyright © 2015 max kohler. All rights reserved.
//

#ifndef color_hpp
#define color_hpp

#include <stdio.h>



class Color
{
public:
    float R;
    float G;
    float B;
    
    Color();
    Color(float r, float g, float b);
    Color operator*(const Color& c) const;
    Color operator*(const float Factor) const;
    Color operator+(const Color& c) const;
    Color& operator+=(const Color& c);
};

#endif /* color_hpp */
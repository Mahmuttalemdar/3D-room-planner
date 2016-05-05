//
//  Material.h
//  RealtimeRending
//
//  Created by Philipp Lensing on 27.10.14.
//  Copyright (c) 2014 Philipp Lensing. All rights reserved.
//

#ifndef __RealtimeRending__Material__
#define __RealtimeRending__Material__

#include <iostream>
#include "vector.hpp"
#include "color.hpp"
#include "texture.h"

class Material
{
public:
    Material( Vector kd, Vector ks, Vector ka, float ns, std::string name );
    Material( Color diffCol, Color specColor, Color ambCol, Texture diffTex, float specExp, std::string name );
    Material(std::string name);
    Material();
    ~Material();
    const Color& getDiffuseColor() const;
    const Color& getSpecularColor() const;
    const Color& getAmbientColor() const;
    float getSpecularExponent() const;
    const std::string& getName() const;
    
    void setDiffuseColor( const Color& color);
    void setSpecularColor( const Color& color);
    void setAmbientColor( const Color& color);
    void setSpecularExponent( const float exp);
    void setName( const std::string& name);
    void setDiffuseTexture( const char* Filename);
    Texture getDiffuseTexture(){return m_DiffuseTexture;}


    Color m_DiffuseColor;
    Color m_SpecularColor;
    Color m_AmbientColor;
    Texture m_DiffuseTexture;
    float m_SpecularExp;
    std::string m_Name;
    bool hasTexture;
    
};

#endif /* defined(__RealtimeRending__Material__) */

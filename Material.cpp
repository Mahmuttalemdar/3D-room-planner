//
//  Material.cpp
//  RealtimeRending
//
//  Created by Philipp Lensing on 27.10.14.
//  Copyright (c) 2014 Philipp Lensing. All rights reserved.
//

#include "Material.h"

Material::Material( Vector kd, Vector ks, Vector ka, float ns, std::string name )
{

}

Material::Material( Color diffCol, Color specColor, Color ambCol, Texture diffTex, float specExp, std::string name )
{
    this->m_DiffuseColor = diffCol;
    this->m_SpecularColor = specColor;
    this->m_AmbientColor = ambCol;
    this->m_DiffuseTexture = diffTex;
    this->m_SpecularExp = specExp;
    this->m_Name = name;
    this->hasTexture = false;
}

Material::Material(std::string name)
{
    this->m_Name = name;
    this->hasTexture = false;
}

Material::Material()
{
}

Material::~Material()
{
    
}

const Color& Material::getDiffuseColor() const
{
    return m_DiffuseColor;
}

const Color& Material::getSpecularColor() const
{
    return m_SpecularColor;
}

const Color& Material::getAmbientColor() const
{
    return m_AmbientColor;
}

float Material::getSpecularExponent() const
{
    return m_SpecularExp;
}

const std::string& Material::getName() const
{
    return m_Name;
}

void Material::setDiffuseColor( const Color& color)
{
    m_DiffuseColor = color;
}

void Material::setSpecularColor( const Color& color)
{
    m_SpecularColor = color;
}

void Material::setAmbientColor( const Color& color)
{
    m_AmbientColor = color;
}

void Material::setSpecularExponent( const float exp)
{
    m_SpecularExp = exp;
}

void Material::setName( const std::string& name)
{
    m_Name = name;
}

void Material::setDiffuseTexture( const char* Filename)
{
    m_DiffuseTexture.LoadFromBMP(Filename);
}




//
//  PlaneModel.h
//  RealtimeRending
//
//  Created by Philipp Lensing on 25.11.14.
//  Copyright (c) 2014 Philipp Lensing. All rights reserved.
//

#ifndef __RealtimeRending__PlaneModel__
#define __RealtimeRending__PlaneModel__

#include <iostream>
#include "texture.h"
#include "vector.hpp"



struct PlaneVertex
{
    Vector Pos;
    Vector Normal;
    float u0,v0;
    float u1,v1;
};

class PlaneModel
{
public:
    PlaneModel();
    ~PlaneModel();
    bool load(const char* tex1, const char* tex2, const char* Mixmap);
    void draw();
    Texture& getGrass();
    Texture& getSand();
    Texture& getMixmap();
protected:
    Texture m_GrassTex;
    Texture m_SandTex;
    Texture m_Mixmap;
    GLuint m_VertexBuffer;
    GLuint m_IndexBuffer;
};

#endif /* defined(__RealtimeRending__PlaneModel__) */

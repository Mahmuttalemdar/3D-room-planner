//
//  Texture.h
//  RealtimeRending
//
//  Created by Philipp Lensing on 24.10.14.
//  Copyright (c) 2014 Philipp Lensing. All rights reserved.
//

#ifndef __RealtimeRending__Texture__
#define __RealtimeRending__Texture__

//mac
#include <GLUT/GLUT.h>
#include <OpenGL/OpenGL.h>

//win
//#include <GL\glew.h>
//#include <GL\freeglut.h>

#include <iostream>



class RGBImage;

class Texture
{
public:
    Texture();
    ~Texture();
    bool LoadFromBMP( const char* Filename );
    void apply() const;
    bool isValid() const;
    const RGBImage* getRGBImage() const;
	GLuint m_TextureID;
protected:
    unsigned char* LoadBMP( const char* Filename, unsigned int& width, unsigned int& height );
    RGBImage* createImage( unsigned char* Data, unsigned int width, unsigned int height );
    
    RGBImage* m_pImage;
};

#endif /* defined(__RealtimeRending__Texture__) */

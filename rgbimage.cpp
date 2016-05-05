//
//  rgbimage.cpp
//  CG_Prak1
//
//  Created by max on 29/09/15.
//  Copyright © 2015 max kohler. All rights reserved.
//

//mac
#include <GLUT/GLUT.h>
#include <OpenGL/OpenGL.h>
#include <unistd.h>

//win
//#include <GL\glew.h>
//#include <GL\freeglut.h>

#include "rgbimage.h"
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include "Vector.hpp"


RGBImage::RGBImage()
{
    
}

RGBImage::RGBImage(unsigned int width, unsigned int height)
{
    this->m_Height = height;
    this->m_Width = width;
    
    m_Image = new Color[m_Height * m_Width];
    
}

RGBImage::~RGBImage()
{
    delete m_Image;
}

unsigned int RGBImage::width() const
{
    return this->m_Width;
}

unsigned int RGBImage::height() const
{
    return this->m_Height;
}

void RGBImage::setPixelColor(unsigned int x, unsigned int y, const Color& c)
{
    if(this->width() >= x && this->height() >= y)
    {
        this->m_Image[this->width() * y + x] = c;
    }
}

const Color& RGBImage::getPixelColor(unsigned int x, unsigned int y) const
{
    if(this->width() >= x && this->height() >= y)
    {
        return this->m_Image[this->width() * y + x];
    }
    return this->m_Image[0];
}

unsigned char RGBImage::convertColorChannel(float f)
{
    if (f < 0.0f)
    {
        f = 0.0f;
    }
    if (f > 1.0f)
    {
        f = 1.0f;
    }
    
    return f * 255;
}


bool RGBImage::saveToDisk(const char* filename)
{
    FILE *datei = fopen(filename, "wb");
    char buffer[3];
    
    int filesize = 54 + 3 * this->width() * this->height();
    
    unsigned char bmpfileheader[14] =       // DateiKopf = 14 byte
    {
        'B','M',        //ID field
        
        0,0,0,0,        //Size of the BMP file
        
        0,0,            //Application specific
        
        0,0,            //Application specific
        
        54,0,0,0        //Offset where the pixel array (bitmap data) can be found
    };
    
    unsigned char bmpinfoheader[40] =       // Informationsblock = 40 byte
    {
        40,0,0,0,       //info header groesse
        
        0,0,0,0,        //breite
        
        0,0,0,0,        //hoehe
        
        1,0,            //Number of color planes
        
        24,0,           //bits pro pixel
        
        0,0,0,0,        //keine kompression
        
        0,0,0,0,        //Size of the raw bitmap data
        
        0,0,0,0,        // horz resoluition in pixel / m
        
        0,0,0,0,        // vert resolutions
        
        0,0,0,0,        // #colors in pallete
        
        0,0,0,0         // #important colors

    };
    
    unsigned char bmppad[3] = { 0, 0, 0 };
    
    
    bmpfileheader[2] = (unsigned char)( filesize );
    bmpfileheader[3] = (unsigned char)( filesize >> 8 );
    bmpfileheader[4] = (unsigned char)( filesize >> 16 );
    bmpfileheader[5] = (unsigned char)( filesize >> 24 );
    
    
    bmpinfoheader[4] = (unsigned char)( this->width() );
    bmpinfoheader[5] = (unsigned char)( this->width() >> 8);
    bmpinfoheader[6] = (unsigned char)( this->width() >> 16  );
    bmpinfoheader[7] = (unsigned char)( this->width() >> 24);
     
    bmpinfoheader[8] = (unsigned char)( this->height() );
    bmpinfoheader[9] = (unsigned char)( this->height() >> 8 );
    bmpinfoheader[10] = (unsigned char)( this->height() >> 16 );
    bmpinfoheader[11] = (unsigned char)( this->height() >> 24 );
    
    
    fwrite( bmpfileheader, 1, 14, datei );
    fwrite( bmpinfoheader, 1, 40, datei );
    
    for(int y = this->height()-1; y >= 0  ; y-- )       // von 599 bis 0
    {
        for(int x = 0; x < this->width(); x++ )         // von 0 bis 799
        {
            buffer[0] = convertColorChannel(getPixelColor(x, y).B);
            buffer[1] = convertColorChannel(getPixelColor(x, y).G);
            buffer[2] = convertColorChannel(getPixelColor(x, y).R);
            
            fwrite( buffer, sizeof(char), sizeof(buffer), datei );
        }
        fwrite( bmppad, 1, ( 4 - ( this->width() * 3 ) % 4 ) % 4, datei );        //überflüssig weil 2400 % 4 = 0
    }
    
    
    fclose(datei);
    
    return true;
}

void RGBImage::loadHeightmap(const char* filename)
{
    int i;
    FILE* f = fopen(filename, "rb");
    if(!f)
    {
        std::cout << "image is not loaded" << std::endl;
        return;
    }
    //unsigned char info[54];
    unsigned char* info = new unsigned char[54];
    fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header
    
    // extract image height and width from header
//    this->m_Width = *(int*)&info[18];
//    this->m_Height = *(int*)&info[22];
    
    this->m_Width = *(int*)&info[18];
    this->m_Height = *(int*)&info[22];
    

    
    int size = 3 * this->m_Width * this->m_Height;
    
    unsigned char* data = new unsigned char[size]; // allocate 3 bytes per pixel
    fread(data, sizeof(unsigned char), size, f); // read the rest of the data at once
    fclose(f);
    
    this->pixeldata = new int[size];
    
    for(i = 0; i < size; i++)
    {
        this->pixeldata[i] = *(int*)&data[i];
    }

    for(i = 0; i < size; i += 3)
    {
        float tmp = this->pixeldata[i];
        this->pixeldata[i] = this->pixeldata[i+2];
        this->pixeldata[i+2] = tmp;
    }
    
    
    

}

void RGBImage::drawHeightmap()
{
    float height = 1.0f;
    float size = 0.005f;
    int p = 0;
    
    for(int x = 0; x < this->m_Width-1; x++ )
    {
        for(int y = 0; y < this->m_Height-1; y++ )
        {
            Color ca = this->getPixelColor(x, y);
            Color cb = this->getPixelColor(x+1, y);
            Color cc = this->getPixelColor(x, y+1);
            Color cd = this->getPixelColor(x+1, y+1);
            
            glBegin(GL_POINTS);
            glVertex3f(-5+x * size,     ca.R * height, -5+y * size);
//            glVertex3f(-5+x+1 * size,   cb.R * height, -5+y * size);
//            glVertex3f(-5+x * size,     cc.R * height, -5+y+1 * size);
//            glVertex3f(-5+x+1 * size,   cd.R * height, -5+y+1 * size);
            glEnd();
            p++;
        }
        
    }
    
}








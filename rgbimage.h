//
//  rgbimage.hpp
//  CG_Prak1
//
//  Created by max on 29/09/15.
//  Copyright © 2015 max kohler. All rights reserved.
//

#ifndef rgbimage_hpp
#define rgbimage_hpp

#include <stdio.h>
#include "color.hpp"
#include <cstdint>

//typedef uint8_t  CHAR;
//typedef uint16_t WORD;
//typedef uint32_t DWORD;
//
//typedef int8_t  BYTE;
//typedef int16_t SHORT;
//typedef int32_t LONG;
//
//typedef LONG INT;
//typedef INT BOOL;
//
//typedef struct tagBITMAPFILEHEADER
//{
//    WORD    bfType;        // must be 'BM'
//    DWORD   bfSize;        // size of the whole .bmp file
//    WORD    bfReserved1;   // must be 0
//    WORD    bfReserved2;   // must be 0
//    DWORD   bfOffBits;
//} BITMAPFILEHEADER;
//
//typedef struct tagBITMAPINFOHEADER
//{
//    DWORD  biSize;            // size of the structure
//    LONG   biWidth;           // image width
//    LONG   biHeight;          // image height
//    WORD   biPlanes;          // bitplanes
//    WORD   biBitCount;        // resolution
//    DWORD  biCompression;     // compression
//    DWORD  biSizeImage;       // size of the image
//    LONG   biXPelsPerMeter;   // pixels per meter X
//    LONG   biYPelsPerMeter;   // pixels per meter Y
//    DWORD  biClrUsed;         // colors used
//    DWORD  biClrImportant;    // important colors
//} BITMAPINFOHEADER;


class RGBImage
{
public:
    RGBImage();
    RGBImage(unsigned int width, unsigned int height);
    ~RGBImage();
    void setPixelColor(unsigned int x, unsigned int y, const Color& c);
    const Color& getPixelColor(unsigned int x, unsigned int y) const;
    bool saveToDisk(const char* filename);
    unsigned int width() const;
    unsigned int height() const;
    
    static unsigned char convertColorChannel(float f);
    void loadHeightmap(const char* filename);
    void drawHeightmap();
    
//protected:
    unsigned int m_Height;
    unsigned int m_Width;
    Color* m_Image;
    int* pixeldata;
    
};

#endif /* rgbimage_hpp */
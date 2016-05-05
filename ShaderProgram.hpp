//
//  ShaderProgram.hpp
//  CG_Prak6
//
//  Created by max on 09/01/16.
//  Copyright © 2016 max kohler. All rights reserved.
//

#ifndef ShaderProgram_hpp
#define ShaderProgram_hpp

//mac
#include <GLUT/GLUT.h>
#include <OpenGL/OpenGL.h>

//win
//#include <GL\glew.h>
//#include <GL\freeglut.h>
//#include <string>


#include <stdio.h>
#include <iostream>
#include "vector.hpp"
#include "Matrix.h"
#include "color.hpp"
#include <fstream>
#include <vector>
#include <algorithm>

class ShaderProgram
{
public:
    ShaderProgram();
    ~ShaderProgram();
    bool load(const char* VertexShader, const char* FragmentShader);
    bool loadVertexShader(const char* VertexShader);
    bool loadFragmentShader(const char* FragmentShader);
    bool compile(std::string* CompileErrors = NULL);
    
    GLint getParameterID(const char* ParameterName) const;
    
    void setParameter(GLint ID, float Param);
    void setParameter(GLint ID, int Param);
    void setParameter(GLint ID, const Vector& Param);
    void setParameter(GLint ID, const Color& Param);
    void setParameter(GLint ID, const Matrix& Param);
    
    void activate() const;
    void deactivate() const;
    
    std::string readFile(const char *filePath);
    
private:
    GLuint m_VertexShader;
    GLuint m_FragmentShader;
    GLuint m_ShaderProgram;
    const char *vertShaderSrc;
    const char *fragShaderSrc;
    
};

#endif /* ShaderProgram_hpp */

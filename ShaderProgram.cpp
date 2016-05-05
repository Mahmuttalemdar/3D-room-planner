//
//  ShaderProgram.cpp
//  CG_Prak6
//
//  Created by max on 09/01/16.
//  Copyright © 2016 max kohler. All rights reserved.
//

#include "ShaderProgram.hpp"



ShaderProgram::ShaderProgram()
{
    
}

ShaderProgram::~ShaderProgram()
{
    
}

bool ShaderProgram::load(const char* VertexShader, const char* FragmentShader)
{
    bool ret = false;
    if (VertexShader != NULL)
    {
        ret = loadVertexShader(VertexShader);
    }
    if(FragmentShader != NULL)
    {
        ret = loadFragmentShader(FragmentShader);
    }
    return ret;
}

bool ShaderProgram::loadVertexShader(const char* vertex_path)
{
//    this->m_VertexShader = glCreateShader(GL_VERTEX_SHADER);
//    
//    std::string vertShaderStr = readFile(vertex_path);
//    
//    this->vertShaderSrc = vertShaderStr.c_str();
    
    FILE * pFile;
    long lSize;
    char * SourceVS;
    size_t result;
    pFile = fopen(vertex_path, "rb");

    if (pFile == NULL)
    {
        fputs("File error", stderr);
        return false;
    }
    
    // obtain file size:
    fseek(pFile, 0, SEEK_END);
    lSize = ftell(pFile);
    rewind(pFile);
    if (lSize > 0) {
        SourceVS = (char*)malloc(sizeof(char)*(lSize + 1));
        if (SourceVS == NULL)
        {
            fputs("Memory error", stderr);
            return false;
        }
        
        result = fread(SourceVS, sizeof(char), lSize, pFile);
        SourceVS[lSize] = '\0';
        if (result != lSize)
        {
            fputs("Reading error", stderr);
            return false; //exit(3);
        }
    }

    fclose(pFile);
    
    m_VertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(m_VertexShader, 1, (const GLchar**)&SourceVS, NULL);
    
    std::cout << "Ende load Vertex" << std::endl;
    free(SourceVS);

    
    return true;
    
}

bool ShaderProgram::loadFragmentShader(const char* fragment_path)
{
//    this->m_FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//    
//    std::string fragShaderStr = readFile(fragment_path);
//    
//    this->fragShaderSrc = fragShaderStr.c_str();
    
    FILE * pFile;
    long lSize;
    char * SourceFS;
    size_t result;
    pFile = fopen(fragment_path, "rb");

    if (pFile == NULL)
    {
        fputs("File error", stderr);
        return false;//exit(1);
    }
    

    fseek(pFile, 0, SEEK_END);
    lSize = ftell(pFile);
    rewind(pFile);
    
    if (lSize > 0)
    {
        SourceFS = (char*)malloc(sizeof(char)*(lSize+1));
        if (SourceFS == NULL)
        {
            fputs("Memory error", stderr);
            return false;//exit(2);
        }
        
        // copy the file into the buffer:
        result = fread(SourceFS, sizeof(char), lSize, pFile);
        SourceFS[lSize] = '\0';
        if (result != lSize)
        {
            fputs("Reading error", stderr);
            return false; //exit(3);
        }
    }

    fclose(pFile);
    
    m_FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(m_FragmentShader, 1, &SourceFS, NULL);// letzer Parameter FilesizeFS/NULL
    
    std::cout << "Ende load Frag" << std::endl;
    free(SourceFS);
    return true;
}

bool ShaderProgram::compile(std::string* CompileErrors)
{
//    GLint result = GL_FALSE;
//    int logLength;
//    
//    // Compile vertex shader
//    std::cout << "Compiling vertex shader." << std::endl;
//    glShaderSource(this->m_VertexShader, 1, &vertShaderSrc, NULL);
//    glCompileShader(this->m_VertexShader);
//    
//    // Check vertex shader
//    glGetShaderiv(this->m_VertexShader, GL_COMPILE_STATUS, &result);
//    glGetShaderiv(this->m_VertexShader, GL_INFO_LOG_LENGTH, &logLength);
//    std::vector<char> vertShaderError((logLength > 1) ? logLength : 1);
//    glGetShaderInfoLog(this->m_VertexShader, logLength, NULL, &vertShaderError[0]);
//    std::cout << &vertShaderError[0] << std::endl;
//    
//    
//    // Compile fragment shader
//    std::cout << "Compiling fragment shader." << std::endl;
//    glShaderSource(this->m_FragmentShader, 1, &fragShaderSrc, NULL);
//    glCompileShader(this->m_FragmentShader);
//    
//    // Check fragment shader
//    glGetShaderiv(this->m_FragmentShader, GL_COMPILE_STATUS, &result);
//    glGetShaderiv(this->m_FragmentShader, GL_INFO_LOG_LENGTH, &logLength);
//    std::vector<char> fragShaderError((logLength > 1) ? logLength : 1);
//    glGetShaderInfoLog(this->m_FragmentShader, logLength, NULL, &fragShaderError[0]);
//    std::cout << &fragShaderError[0] << std::endl;
//    
//    std::cout << "Linking program" << std::endl;
//    GLuint program = glCreateProgram();
//    glAttachShader(program, this->m_VertexShader);
//    glAttachShader(program, this->m_FragmentShader);
//    glLinkProgram(program);
//    
//    glGetProgramiv(program, GL_LINK_STATUS, &result);
//    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
//    std::vector<char> programError( (logLength > 1) ? logLength : 1 );
//    glGetProgramInfoLog(program, logLength, NULL, &programError[0]);
//    std::cout << &programError[0] << std::endl;
//    
//    glDeleteShader(this->m_VertexShader);
//    glDeleteShader(this->m_FragmentShader);
//    
//    this->m_ShaderProgram = program;
//    
//    return true;
    
    GLint compiled, linked;
    glCompileShader(m_VertexShader);
    glCompileShader(m_FragmentShader);
    
    // Ueberpruefe, ob der Vertex-Shader compiliert wurde
    glGetShaderiv(m_VertexShader, GL_COMPILE_STATUS, &compiled);
    if (compiled == GL_FALSE)
    {
        GLint  logSize;
        glGetShaderiv(m_VertexShader, GL_INFO_LOG_LENGTH, &logSize);
        char* logMsg = new char[logSize];
        glGetShaderInfoLog(m_VertexShader, logSize, NULL, logMsg);
        std::cerr << "Failure in Vertex\n" << logMsg << std::endl;
        delete[] logMsg;
        
        glDeleteShader(m_VertexShader);
        glDeleteShader(m_FragmentShader);
        return false;
    }
    
    // Ueberpruefe, ob der Fragment-Shader compiliert wurde
    glGetShaderiv(m_FragmentShader, GL_COMPILE_STATUS, &compiled);
    if (compiled == GL_FALSE)
    {
        GLint  logSize2;
        glGetShaderiv(m_FragmentShader, GL_INFO_LOG_LENGTH, &logSize2);
        std::cout << "logsize frag:" << logSize2 << std::endl;
        char* logMsg2 = new char[logSize2];
        glGetShaderInfoLog(m_FragmentShader, logSize2, NULL, logMsg2);
        std::cerr << "Failure in Fragment\n" << logMsg2 << std::endl;
        delete[] logMsg2;
        
        glDeleteShader(m_VertexShader);
        glDeleteShader(m_FragmentShader);
        return false;//return (GLuint)NULL;
    }
    
    m_ShaderProgram = glCreateProgram();
    
    glAttachShader(m_ShaderProgram, m_VertexShader);
    glAttachShader(m_ShaderProgram, m_FragmentShader);
    
    
    // link  and error check
    glLinkProgram(m_ShaderProgram);
    
    
    glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, &linked);
    if (!linked)
    {
        std::cerr << "Shader program failed to link" << std::endl;
        GLint  logSize;
        glGetProgramiv(m_ShaderProgram, GL_INFO_LOG_LENGTH, &logSize);
        char* logMsg = new char[logSize];
        glGetProgramInfoLog(m_ShaderProgram, logSize, NULL, logMsg);
        std::cerr << logMsg << std::endl;
        delete[] logMsg;
        
        return false;
        //exit(EXIT_FAILURE);
    }
    
    return true;
}

GLint ShaderProgram::getParameterID(const char* ParameterName) const
{
    GLint loc = glGetUniformLocation(this->m_ShaderProgram, ParameterName);
    return loc;
}

void ShaderProgram::setParameter(GLint ID, float Param)
{
    glUniform1f(ID, Param);
}

void ShaderProgram::setParameter(GLint ID, int Param)
{
    glUniform1i(ID, Param);
}

void ShaderProgram::setParameter(GLint ID, const Vector& Param)
{
    glUniform3f(ID, Param.X, Param.Y, Param.Z);
}

void ShaderProgram::setParameter(GLint ID, const Color& Param)
{
    glUniform3f(ID, Param.R, Param.G, Param.B);
}

void ShaderProgram::setParameter(GLint ID, const Matrix& Param)
{
   // glUniformMatrix4fv(ID, 1, GL_FALSE, );
}

void ShaderProgram::activate() const
{
    glUseProgram(this->m_ShaderProgram);
}

void ShaderProgram::deactivate() const
{
    glUseProgram(0);
}

std::string ShaderProgram::readFile(const char *filePath)
{
    std::string content;
    std::ifstream fileStream(filePath, std::ios::in);
    
    if(!fileStream.is_open())
    {
        std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
        return "";
    }
    
    std::string line = "";
    while(!fileStream.eof()) {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }
    
    fileStream.close();
    return content;
}
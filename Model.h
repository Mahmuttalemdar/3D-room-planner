//
//  Model.h
//  RealtimeRending
//
//  Created by Philipp Lensing on 23.10.14.
//  Copyright (c) 2014 Philipp Lensing. All rights reserved.
//

#ifndef __RealtimeRending__Model__
#define __RealtimeRending__Model__

#include <iostream>
#include "vector.hpp"
#include "color.hpp"
#include <string>
#include <map>
#include <vector>
#include "material.h"
#include "ShaderProgram.hpp"



struct Vertex
{
    Vector Position;
    Vector Normal;
    Vertex();
    Vertex( const Vector p, const Vector n, float TexS, float TexT, Material p_mat, bool hasM);
    Vertex(const Vector& p) : Position(p) {};
    float  TexcoordS;
    float  TexcoordT;
    Material mat;
    bool hasMat;
};

class BoundingBox
{
public:
    BoundingBox();
    BoundingBox( const Vector& min, const Vector& max);
    
    Vector Min;
    Vector Max;
    Vertex* m_pBoundingVertices;
    Vector m_BoundingPositionsFromFile[8];
    
    unsigned int m_BoundingVertexCount;
    void createBBox(Vertex* m_pVertices, unsigned int m_VertexCount);
    void drawBBox();
};

class Model
{
public:
    Model();
    ~Model();
    
    bool load( const char* Filename, bool FitSize, const char* modellePfad);

	int loadObject(const char* filename, bool FitSize, const char* modellePfad);
	int loadObjectInArrays(const char* filename, bool FitSize, const char* modellePfad);
	int loadMaterial(const char* filename, const char* modellePfad);

    void drawLines() const;
    void drawTriangles() const;
    void drawWithBuffer(bool picked);
	void drawArrays();
    
    
    
    

	BoundingBox m_Box;
    BoundingBox& getBoundingBox() {return this->m_Box;};
    
    
    
    void calcNormals();
    void setIndices();
    void createCube();
	void genBuffer();
    
	GLuint m_VertexBufferID;
	GLuint m_VertexIndexBufferID;


	unsigned int m_VertexCount;
	unsigned int* m_VertexIndices;
	unsigned int m_VertexIndexCount;

	std::vector<Vertex*> VertexArrays;

	Vertex* m_pVertices;
    
    ShaderProgram m_ShaderProgram;
	Material* m_pMaterials;
	unsigned int m_MaterialCount;
    
};

#endif /* defined(__RealtimeRending__Model__) */

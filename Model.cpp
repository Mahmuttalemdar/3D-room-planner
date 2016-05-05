//
//  Model.cpp
//  RealtimeRending
//
//  Created by Philipp Lensing on 23.10.14.
//  Copyright (c) 2014 Philipp Lensing. All rights reserved.
//

extern float g_Time;

//mac
#include <GLUT/GLUT.h>
#include <OpenGL/OpenGL.h>

//win
//#include <GL\glew.h>
//#include <GL\freeglut.h>


#include "Model.h"
#include <vector>
#include <assert.h>
#include <math.h>
#include <map>
#include <float.h>
#include <fstream>
#define BUFFER_OFFSET(i) ((char *)NULL + (i))


Vertex::Vertex()
{
    
}

Vertex::Vertex(Vector p, Vector n, float TexS, float TexT, Material p_mat, bool hasM)
{
    this->Position = p;
    this->Normal = n;
    this->TexcoordS = TexS;
    this->TexcoordT = TexT;
    this->mat = p_mat;
    this->hasMat = hasM;
    
}

BoundingBox::BoundingBox()
{
    
}
BoundingBox::BoundingBox( const Vector& min, const Vector& max) : Min(min), Max(max)
{
    
}

void BoundingBox::createBBox(Vertex* vertices, unsigned int m_VertexCount)
{
    this->Max.X = this->Min.X = vertices[0].Position.X;
    this->Max.Y = this->Min.Y = vertices[0].Position.Y;
    this->Max.Z = this->Min.Z = vertices[0].Position.Z;
    
    for (int i = 0; i < m_VertexCount; i++)
    {
        Vector vertex = vertices[i].Position;
        
        if (this->Min.X < vertex.X) this->Min.X = vertex.X;
        if (this->Min.Y < vertex.Y) this->Min.Y = vertex.Y;
        if (this->Min.Z < vertex.Z) this->Min.Z = vertex.Z;
        
        if (this->Max.X > vertex.X) this->Max.X = vertex.X;
        if (this->Max.Y > vertex.Y) this->Max.Y = vertex.Y;
        if (this->Max.Z > vertex.Z) this->Max.Z = vertex.Z;
    }
    
    this->m_BoundingPositionsFromFile[0] = Vector(this->Min.X, this->Min.Y, this->Max.Z);
    this->m_BoundingPositionsFromFile[1] = Vector(this->Min.X, this->Max.Y, this->Max.Z);
    this->m_BoundingPositionsFromFile[2] = Vector(this->Max.X, this->Min.Y, this->Max.Z);
    this->m_BoundingPositionsFromFile[3] = Vector(this->Max.X, this->Max.Y, this->Max.Z);
    this->m_BoundingPositionsFromFile[4] = Vector(this->Max.X, this->Min.Y, this->Min.Z);
    this->m_BoundingPositionsFromFile[5] = Vector(this->Max.X, this->Max.Y, this->Min.Z);
    this->m_BoundingPositionsFromFile[6] = Vector(this->Min.X, this->Min.Y, this->Min.Z);
    this->m_BoundingPositionsFromFile[7] = Vector(this->Min.X, this->Max.Y, this->Min.Z);
    
    struct Face
    {
        unsigned int pidx[3];
    };
    Face Faces[12]
    {
        //face 1
        { 3,4,2},
        { 3,2,1},
        //face 2
        { 5,6,4},
        { 5,4,3},
        //face 3
        { 7,8,6},
        { 7,6,5},
        //face 4
        { 1,2,8},
        { 1,8,7},
        //face 5
        { 4,6,8},
        { 4,8,2},
        //face 6
        { 5,3,1},
        { 5,1,7}
    };
    
    this->m_pBoundingVertices = new Vertex[12 * 3];
    m_BoundingVertexCount = 12 * 3;
    for (int i = 0; i<12; i++)
    {
        unsigned int PosIdx0 = Faces[i].pidx[0] - 1;
        unsigned int PosIdx1 = Faces[i].pidx[1] - 1;
        unsigned int PosIdx2 = Faces[i].pidx[2] - 1;
        
        Vector a = this->m_pBoundingVertices[i * 3 + 0].Position = m_BoundingPositionsFromFile[PosIdx0];
        Vector b = this->m_pBoundingVertices[i * 3 + 1].Position = m_BoundingPositionsFromFile[PosIdx1];
        Vector c = this->m_pBoundingVertices[i * 3 + 2].Position = m_BoundingPositionsFromFile[PosIdx2];
        
        Vector normal = (b - a).cross(c - a);
        normal.normalize();
        
        this->m_pBoundingVertices[i * 3].Normal =
        this->m_pBoundingVertices[i * 3 + 1].Normal =
        this->m_pBoundingVertices[i * 3 + 2].Normal = normal;
    }
}

void BoundingBox::drawBBox()
{
    
    glDisable(GL_LIGHTING);
    
    //boundingbox
    glPointSize(5);
    glColor4f(0, 0, 1, 1);
    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 36; i += 3)
    {
        glVertex3f(this->m_pBoundingVertices[i+0].Position.X, this->m_pBoundingVertices[i+0].Position.Y, this->m_pBoundingVertices[i+0].Position.Z);
        glVertex3f(this->m_pBoundingVertices[i+1].Position.X, this->m_pBoundingVertices[i+1].Position.Y, this->m_pBoundingVertices[i+1].Position.Z);
        glVertex3f(this->m_pBoundingVertices[i+2].Position.X, this->m_pBoundingVertices[i+2].Position.Y, this->m_pBoundingVertices[i+2].Position.Z);
    }
    glEnd();
    //glFlush();
    
    glEnable(GL_LIGHTING);
}

Model::Model() : m_pVertices(NULL), m_pMaterials(NULL), m_MaterialCount(0), m_VertexCount(0)
{

}

Model::~Model()
{
    if( m_pVertices)
        delete [] m_pVertices;
    if(m_pMaterials)
        delete [] m_pMaterials;
}

bool Model::load( const char* Filename, bool FitSize, const char* modellePfad)
{
    this->loadObjectInArrays(Filename, FitSize, modellePfad);
    
    this->m_ShaderProgram.load("/Users/max/Uni/2015WS/Computergrafik/Hausarbeit/Hausarbeit_WS2015/Hausarbeit_WS2015/pickedvert.glsl",
                               "/Users/max/Uni/2015WS/Computergrafik/Hausarbeit/Hausarbeit_WS2015/Hausarbeit_WS2015/pickedfrag.glsl");
    
    this->m_ShaderProgram.compile();
    
    this->m_ShaderProgram.activate();
    this->m_ShaderProgram.setParameter(this->m_ShaderProgram.getParameterID("LightPos"), Vector( 0,15,0));
    this->m_ShaderProgram.setParameter(this->m_ShaderProgram.getParameterID("LightColor"), Vector(1.0f, 0.0f, 1.0f));
    this->m_ShaderProgram.deactivate();
    
    return true;
}



void Model::drawLines() const
{
    glColor3f(1,0,1);
    for(unsigned int i = 0; i < this->m_VertexCount ; i+= 3)
    {
        glBegin(GL_LINE_LOOP);
        glVertex3f(this->m_pVertices[i].Position.X,   this->m_pVertices[i].Position.Y,   this->m_pVertices[i].Position.Z);
        glVertex3f(this->m_pVertices[i+1].Position.X, this->m_pVertices[i+1].Position.Y, this->m_pVertices[i+1].Position.Z);
        glVertex3f(this->m_pVertices[i+2].Position.X, this->m_pVertices[i+2].Position.Y, this->m_pVertices[i+2].Position.Z);
        glEnd();
    }
    
    glEnd();
    glFlush();
}

void Model::drawTriangles() const
{
    ShaderProgram sp;
    sp = this->m_ShaderProgram;
    sp.activate();
    
        for(unsigned int i = 0; i < this->m_VertexCount ; i+=3)
        {
            if(m_pVertices[i].hasMat)
            {
                Material mat = m_pVertices[i].mat;
                
                float diff[4] = {mat.getDiffuseColor().R, mat.getDiffuseColor().G, mat.getDiffuseColor().B, 1.0};
                float amb [4] = {mat.getAmbientColor().R, mat.getAmbientColor().G, mat.getAmbientColor().B, 1.0};
                float spec[4] = {mat.getSpecularColor().R, mat.getSpecularColor().G, mat.getSpecularColor().B, 1.0};

                
                glMaterialfv(GL_FRONT, GL_DIFFUSE, diff);
                glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
                glMateriali(GL_FRONT, GL_SHININESS, mat.getSpecularExponent());
                glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
                
                

                sp.setParameter(this->m_ShaderProgram.getParameterID("DiffColor"),
                                            Vector(mat.getDiffuseColor().R,
                                                   mat.getDiffuseColor().G,
                                                   mat.getDiffuseColor().B));
                sp.setParameter(this->m_ShaderProgram.getParameterID("SpecColor"),
                                            Vector(mat.getSpecularColor().R,
                                                   mat.getSpecularColor().G,
                                                   mat.getSpecularColor().B));
                sp.setParameter(this->m_ShaderProgram.getParameterID("AmbientColor"),
                                            Vector(mat.getAmbientColor().R,
                                                   mat.getAmbientColor().G,
                                                   mat.getAmbientColor().B));
                sp.setParameter(this->m_ShaderProgram.getParameterID("SpecExp"),
                                            mat.getSpecularExponent());
        
                

                
                glActiveTexture(GL_TEXTURE0);
                glClientActiveTexture(GL_TEXTURE0);

                if(m_pVertices[i].mat.hasTexture)
                {
                    mat.getDiffuseTexture().apply();
                }


                //sp.setParameter(sp.getParameterID("DiffuseTexture"), 0);

            }
    
			glPointSize(5.0f);
			glColor4f(0, 1, 0, 1);
            glBegin(GL_POINTS);
    
            glNormal3f(this->m_pVertices[i].Normal.X,   this->m_pVertices[i].Normal.Y,      this->m_pVertices[i].Normal.Z);
            glTexCoord2f(m_pVertices[i].TexcoordS,      m_pVertices[i].TexcoordT);
            glVertex3f(this->m_pVertices[i].Position.X, this->m_pVertices[i].Position.Y,    this->m_pVertices[i].Position.Z);
    
            glNormal3f(this->m_pVertices[i+1].Normal.X,     this->m_pVertices[i+1].Normal.Y,    this->m_pVertices[i+1].Normal.Z);
            glTexCoord2f(m_pVertices[i+1].TexcoordS,        m_pVertices[i+1].TexcoordT);
            glVertex3f(this->m_pVertices[i+1].Position.X,   this->m_pVertices[i+1].Position.Y,  this->m_pVertices[i+1].Position.Z);
    
            glNormal3f(this->m_pVertices[i+2].Normal.X,     this->m_pVertices[i+2].Normal.Y,    this->m_pVertices[i+2].Normal.Z);
            glTexCoord2f(m_pVertices[i+2].TexcoordS,        m_pVertices[i+2].TexcoordT);
            glVertex3f(this->m_pVertices[i+2].Position.X,   this->m_pVertices[i+2].Position.Y,  this->m_pVertices[i+2].Position.Z);
            
            glEnd();

			if (m_pVertices[i].hasMat)
			{
				if (m_pVertices[i].mat.hasTexture)
				{
					glColor4f(1, 1, 1, 1);
					glBindTexture(GL_TEXTURE_2D, 0);
					glDisable(GL_TEXTURE_2D);
				}
			}
        }
        sp.deactivate();
        glFlush();
}

void Model::genBuffer()
{
	// create gpu buffer for vertices
	glGenBuffers(1, &(this->m_VertexBufferID));
	glBindBuffer(GL_ARRAY_BUFFER, this->m_VertexBufferID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*this->m_VertexCount, this->m_pVertices, GL_STATIC_DRAW);

	// create gpu buffer for indices
	glGenBuffers(1, &(this->m_VertexIndexBufferID));
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_VertexIndexBufferID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*this->m_VertexIndexCount, this->m_VertexIndices, GL_STATIC_DRAW);
}

void Model::drawWithBuffer(bool picked)
{
	Material mat = m_pVertices[0].mat;
	//Material matt = m_pVertices[434].mat;

	this->m_ShaderProgram.activate();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_VertexIndexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, this->m_VertexBufferID);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);


	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(0));
	glNormalPointer(GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(12));
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(24));

	this->m_ShaderProgram.setParameter(this->m_ShaderProgram.getParameterID("DiffColor"),
		Vector(mat.getDiffuseColor().R,
			mat.getDiffuseColor().G,
			mat.getDiffuseColor().B));
	this->m_ShaderProgram.setParameter(this->m_ShaderProgram.getParameterID("SpecColor"),
		Vector(mat.getSpecularColor().R,
			mat.getSpecularColor().G,
			mat.getSpecularColor().B));
	this->m_ShaderProgram.setParameter(this->m_ShaderProgram.getParameterID("AmbientColor"),
		Vector(mat.getAmbientColor().R,
			mat.getAmbientColor().G,
			mat.getAmbientColor().B));
	this->m_ShaderProgram.setParameter(this->m_ShaderProgram.getParameterID("SpecExp"),
		mat.getSpecularExponent());
    
    //tobis shaderzeug
    const float timeScale2 = 0.002f;
    GLfloat timeValue = glutGet(GLUT_ELAPSED_TIME)*timeScale2;
    GLint locPosition = this->m_ShaderProgram.getParameterID("position");
    GLint locTime = this->m_ShaderProgram.getParameterID("time");
    this->m_ShaderProgram.setParameter(locTime, glutGet(GLUT_ELAPSED_TIME) * timeScale2);
    //this->m_ShaderProgram.setParameter(locPosition, Vector(sinf(timeValue*3.43), sinf(timeValue * 3.03), sinf(timeValue*2.63)));
    
    const float timeScale = 0.004f;
    float g_timebase = g_Time;//time from last frame
    g_Time = glutGet(GLUT_ELAPSED_TIME);//actual time
    float deltaTime = g_Time - g_timebase + 1;
    float time = glutGet(GLUT_ELAPSED_TIME);
    
    GLint locDeltaTime = this->m_ShaderProgram.getParameterID("deltaTime");
    this->m_ShaderProgram.setParameter(locDeltaTime, Vector(sinf(time*timeScale), sinf(time*timeScale), picked));
    this->m_ShaderProgram.setParameter(locTime, glutGet(GLUT_ELAPSED_TIME) * timeScale2);
    //Ende tobis shaderzeug


	glActiveTexture(GL_TEXTURE0);
	glClientActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, mat.getDiffuseTexture().m_TextureID);
	GLint loc = this->m_ShaderProgram.getParameterID("DiffuseTexture");
	this->m_ShaderProgram.setParameter(loc, 0);

	//glActiveTexture(GL_TEXTURE1);
	//glClientActiveTexture(GL_TEXTURE1);
	//glEnable(GL_TEXTURE_2D);
	//glBindTexture(GL_TEXTURE_2D, m_pVertices[434].mat.getDiffuseTexture().m_TextureID);
	//GLint locc = this->m_ShaderProgram.getParameterID("DiffuseTexture");
	//this->m_ShaderProgram.setParameter(locc, 1);


	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glDrawElements(GL_TRIANGLES, this->m_VertexIndexCount, GL_UNSIGNED_INT, BUFFER_OFFSET(0));

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);

	//glBindTexture(GL_TEXTURE_2D, 1);
	//glDisable(GL_TEXTURE_2D);

	this->m_ShaderProgram.deactivate();


	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Model::drawArrays()
{
	glBindBuffer(GL_ARRAY_BUFFER, this->m_VertexBufferID);


	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer(3, GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(0));
	glNormalPointer(GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(12));


	glActiveTexture(GL_TEXTURE0);
	glClientActiveTexture(GL_TEXTURE0);
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), BUFFER_OFFSET(24));
	
	if (m_pVertices[0].mat.hasTexture)
	{
		m_pVertices[0].mat.getDiffuseTexture().apply();
	}

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glDrawArrays(GL_TRIANGLES, 0, this->m_VertexCount);

	if (m_pVertices[0].mat.hasTexture)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
	}
	
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

int Model::loadObject(const char* filename, bool fitsize, const char* modellePfad)
{
    std::vector<Vector> tmp_UVCoords;
    std::vector<Vector> tmp_VertexCoords;
	std::vector<Vector> tmp_Normals;


	std::vector<Vertex> vertices_tmp;

	this->m_VertexCount = 0;

    bool useMaterial = false;
    
    
    Material curmat("default");
    
    std::vector<std::string*> lines;

    std::ifstream in(filename);
    if(!in.is_open())
    {
        std::cout << "datei nicht geoeffnet" << std::endl;
        return -1;
    }
    char buf[256];
   
    while(!in.eof())
    {
        in.getline(buf,256);
        lines.push_back(new std::string(buf));
    }

    for(int i=0;i<lines.size();i++)
    {
        
        if((*lines[i])[0]=='m' && (*lines[i])[1]=='t' && (*lines[i])[2]=='l' && (*lines[i])[3]=='l')
        {
            char mtllib[200];
            sscanf(lines[i]->c_str(),"mtllib %s",mtllib);
            
            char fullPath[200] = {0};
            strcat(fullPath,modellePfad);
            strcat(fullPath,mtllib);
            
           this->loadMaterial(fullPath, modellePfad);
        }
        
        else if(lines[i]->c_str()[0]=='v' && lines[i]->c_str()[1]==' ')			//if vertexcoord
        {
			//this->m_VertexCount++;

            float tmpx,tmpy,tmpz;
            sscanf(lines[i]->c_str(),"v %f %f %f",&tmpx,&tmpy,&tmpz);
            tmp_VertexCoords.push_back(Vector(tmpx, tmpy, tmpz));

			//Vertex v = Vertex(Vector(tmpx, tmpy, tmpz));
			//v.Normal = Vector(0, 0, 0);

			//vertices_tmp_unique.push_back(v);
        }

        else if(lines[i]->c_str()[0]=='v' && lines[i]->c_str()[1]=='t')        //if texturcoord
        {
            float u,v;
            sscanf(lines[i]->c_str(),"vt %f %f",&u,&v);
            tmp_UVCoords.push_back(Vector(u, v, 0));
        }
        
        
        else if((*lines[i])[0]=='u' && (*lines[i])[1]=='s' && (*lines[i])[2]=='e')     //use material material_name
        {
            
            char tmp[200];
            sscanf(lines[i]->c_str(),"usemtl %s",tmp);      //read the name of the material to tmp

            for(int i=0; i < m_MaterialCount; i++)     //go through all of the materials
            {
                if(strcmp(m_pMaterials[i].m_Name.c_str(),tmp)==0)   //and compare the tmp with the name of the material
                {
                    curmat = m_pMaterials[i];       //if it's equal then set the current material to that
                    useMaterial = true;
                    break;
                }
            }
        }
        
        else if(lines[i]->c_str()[0]=='f')     //if face
        {
            int v1,v2,v3,v4;
            int uv1,uv2,uv3,uv4;

			
            
            if(count(lines[i]->begin(),lines[i]->end(),' ') == 4)     //if it is a triangle (it has 3 space in it)
            {
				//this->m_VertexIndexCount += 3;


                sscanf(lines[i]->c_str(),"f  %d/%d  %d/%d  %d/%d", &v1, &uv1, &v2, &uv2, &v3, &uv3);

				//tmp_vertexIndices.push_back(v1);
				//tmp_vertexIndices.push_back(v2);
				//tmp_vertexIndices.push_back(v3);


				//Vertex* pos1 = &(vertices_tmp_unique.at(v1 - 1));
				//Vertex* pos2 = &(vertices_tmp_unique.at(v2 - 1));
				//Vertex* pos3 = &(vertices_tmp_unique.at(v3 - 1));

                //
                Vector posi1 = tmp_VertexCoords.at(v1-1);
                Vector posi2 = tmp_VertexCoords.at(v2-1);
                Vector posi3 = tmp_VertexCoords.at(v3-1);
                //

                Vector uvposi1 = tmp_UVCoords.at(uv1-1);
                Vector uvposi2 = tmp_UVCoords.at(uv2-1);
                Vector uvposi3 = tmp_UVCoords.at(uv3-1);
                
                Vector normal = (posi2-posi1).cross(posi3-posi1);
				//Vector normal = ((pos2->Position) - (pos1->Position)).cross((pos3->Position) - (pos1->Position));
                normal.normalize();

				//Vector n1 = pos1->Normal;
				//pos1->Normal = normal + n1;
				//pos1->Normal.normalize();
				//
				//Vector n2 = pos2->Normal;
				//pos2->Normal = normal + n2;
				//pos2->Normal.normalize();

				//Vector n3 = pos3->Normal;
				//pos3->Normal = normal + n3;
				//pos3->Normal.normalize();


                vertices_tmp.push_back(Vertex(posi1, normal, uvposi1.X, 1-uvposi1.Y, curmat, useMaterial));
                vertices_tmp.push_back(Vertex(posi2, normal, uvposi2.X, 1-uvposi2.Y, curmat, useMaterial));
                vertices_tmp.push_back(Vertex(posi3, normal, uvposi3.X, 1-uvposi3.Y, curmat, useMaterial));

            }
            else if(count(lines[i]->begin(),lines[i]->end(),' ') == 5)
            {
				//this->m_VertexIndexCount += 6;

                sscanf(lines[i]->c_str(),"f %d/%d %d/%d %d/%d %d/%d", &v1, &uv1, &v2, &uv2, &v3, &uv3, &v4, &uv4);

				//tmp_vertexIndices.push_back(v1);
				//tmp_vertexIndices.push_back(v2);
				//tmp_vertexIndices.push_back(v3);

				//tmp_vertexIndices.push_back(v1);
				//tmp_vertexIndices.push_back(v3);
				//tmp_vertexIndices.push_back(v4);

				

                Vector posi1 = tmp_VertexCoords.at(v1-1);
                Vector posi2 = tmp_VertexCoords.at(v2-1);
                Vector posi3 = tmp_VertexCoords.at(v3-1);
                Vector posi4 = tmp_VertexCoords.at(v4-1);

				//Vertex* pos1 = &(vertices_tmp_unique.at(v1 - 1));
				//Vertex* pos2 = &(vertices_tmp_unique.at(v2 - 1));
				//Vertex* pos3 = &(vertices_tmp_unique.at(v3 - 1));
				//Vertex* pos4 = &(vertices_tmp_unique.at(v4 - 1));
                
                Vector uvposi1 = tmp_UVCoords.at(uv1-1);
                Vector uvposi2 = tmp_UVCoords.at(uv2-1);
                Vector uvposi3 = tmp_UVCoords.at(uv3-1);
                Vector uvposi4 = tmp_UVCoords.at(uv4-1);
                
				//Vector normal = ((pos2->Position) - (pos1->Position)).cross((pos3->Position) - (pos1->Position));
				Vector normal = (posi2 - posi1).cross(posi3 - posi1);
				normal.normalize();


				//Vector n1 = pos1->Normal;
				//pos1->Normal = normal + n1;
				//pos1->Normal.normalize();

				//Vector n2 = pos2->Normal;
				//pos2->Normal = normal + n2;
				//pos2->Normal.normalize();

				//Vector n3 = pos3->Normal;
				//pos3->Normal = normal + n3;
				//pos3->Normal.normalize();

				//Vector n4 = pos4->Normal;
				//pos4->Normal = normal + n4;
				//pos4->Normal.normalize();
                
                vertices_tmp.push_back(Vertex(posi1, normal, uvposi1.X, 1-uvposi1.Y, curmat, useMaterial));
                vertices_tmp.push_back(Vertex(posi2, normal, uvposi2.X, 1-uvposi2.Y, curmat, useMaterial));
                vertices_tmp.push_back(Vertex(posi3, normal, uvposi3.X, 1-uvposi3.Y, curmat, useMaterial));
                
                vertices_tmp.push_back(Vertex(posi1, normal, uvposi1.X, 1-uvposi1.Y, curmat, useMaterial));
                vertices_tmp.push_back(Vertex(posi3, normal, uvposi3.X, 1-uvposi3.Y, curmat, useMaterial));
                vertices_tmp.push_back(Vertex(posi4, normal, uvposi4.X, 1-uvposi4.Y, curmat, useMaterial));
            }
        }
    }
    
    
    
    //this->m_VertexCount = vertices_tmp_unique.size();
	this->m_VertexCount = vertices_tmp.size();
    this->m_pVertices = new Vertex[m_VertexCount];


	this->m_VertexIndices = new unsigned int[this->m_VertexIndexCount];

    
    for(int i = 0; i < this->m_VertexCount; i++)
    {
		//this->m_pVertices[i] = vertices_tmp_unique.at(i);
		this->m_pVertices[i] = vertices_tmp.at(i);
        if(fitsize)
        {
            this->m_pVertices[i].Position = this->m_pVertices[i].Position * 0.01;
        }
    }

	//for (int i = 0; i < this->m_VertexIndexCount; i++)
	//{
	//	this->m_VertexIndices[i] = tmp_vertexIndices.at(i);
	//}


    //m_Box = BoundingBox(Vector(min_X,min_Y,min_Z),Vector(max_X,max_Y,max_Z));

    

	this->genBuffer();

    return true;
}

int Model::loadObjectInArrays(const char* filename, bool FitSize, const char* modellePfad)
{
	std::vector<Vector> tmp_UVCoords;
	std::vector<Vector> tmp_VertexCoords;
	std::vector<Vector> tmp_Normals;

	std::vector< unsigned int > tmp_vertexIndices;


	std::vector<Vertex> vertices_tmp_unique;


	this->m_VertexCount = 0;
	this->m_VertexIndexCount = 0;

	bool useMaterial = false;


	Material curmat("default");

	std::vector<std::string*> lines;

	std::ifstream in(filename);
	if (!in.is_open())
	{
		std::cout << "datei nicht geoeffnet" << std::endl;
		return -1;
	}
	char buf[256];

	while (!in.eof())
	{
		in.getline(buf, 256);
		lines.push_back(new std::string(buf));
	}

	for (int i = 0; i<lines.size(); i++)
	{

		if ((*lines[i])[0] == 'm' && (*lines[i])[1] == 't' && (*lines[i])[2] == 'l' && (*lines[i])[3] == 'l')
		{
			char mtllib[200];
			sscanf(lines[i]->c_str(), "mtllib %s", mtllib);

			char fullPath[200] = { 0 };
			strcat(fullPath, modellePfad);
			strcat(fullPath, mtllib);

			this->loadMaterial(fullPath, modellePfad);
		}

		else if (lines[i]->c_str()[0] == 'v' && lines[i]->c_str()[1] == ' ')			//if vertexcoord
		{
			this->m_VertexCount++;

			float tmpx, tmpy, tmpz;
			sscanf(lines[i]->c_str(), "v %f %f %f", &tmpx, &tmpy, &tmpz);

			Vertex v = Vertex(Vector(tmpx, tmpy, tmpz));
			v.Normal = Vector(0, 0, 0);

			vertices_tmp_unique.push_back(v);
		}

		else if (lines[i]->c_str()[0] == 'v' && lines[i]->c_str()[1] == 't')        //if texturcoord
		{
			float u, v;
			sscanf(lines[i]->c_str(), "vt %f %f", &u, &v);
			tmp_UVCoords.push_back(Vector(u, v, 0));
		}


		else if ((*lines[i])[0] == 'u' && (*lines[i])[1] == 's' && (*lines[i])[2] == 'e')     //use material material_name
		{

			char tmp[200];
			sscanf(lines[i]->c_str(), "usemtl %s", tmp);      //read the name of the material to tmp

			for (int i = 0; i < m_MaterialCount; i++)     //go through all of the materials
			{
				if (strcmp(m_pMaterials[i].m_Name.c_str(), tmp) == 0)   //and compare the tmp with the name of the material
				{
					curmat = m_pMaterials[i];       //if it's equal then set the current material to that
					useMaterial = true;
					break;
				}
			}
		}

		else if (lines[i]->c_str()[0] == 'f')     //if face
		{
			int v1, v2, v3, v4;
			int uv1, uv2, uv3, uv4;


			if (count(lines[i]->begin(), lines[i]->end(), ' ') == 4)     //if it is a triangle (it has 3 space in it)
			{
				this->m_VertexIndexCount += 3;


				sscanf(lines[i]->c_str(), "f  %d/%d  %d/%d  %d/%d", &v1, &uv1, &v2, &uv2, &v3, &uv3);

				tmp_vertexIndices.push_back(v1);
				tmp_vertexIndices.push_back(v2);
				tmp_vertexIndices.push_back(v3);


				Vertex* pos1 = &(vertices_tmp_unique.at(v1 - 1));
				Vertex* pos2 = &(vertices_tmp_unique.at(v2 - 1));
				Vertex* pos3 = &(vertices_tmp_unique.at(v3 - 1));

				Vector uvposi1 = tmp_UVCoords.at(uv1 - 1);
				Vector uvposi2 = tmp_UVCoords.at(uv2 - 1);
				Vector uvposi3 = tmp_UVCoords.at(uv3 - 1);


				//UV setzen
				pos1->TexcoordS = uvposi1.X;
				pos1->TexcoordT = 1 - uvposi1.Y;

				pos2->TexcoordS = uvposi2.X;
				pos2->TexcoordT = 1 - uvposi2.Y;

				pos3->TexcoordS = uvposi3.X;
				pos3->TexcoordT = 1 - uvposi3.Y;

				//Normalen setzen
				Vector normal = ((pos2->Position) - (pos1->Position)).cross((pos3->Position) - (pos1->Position));
				normal.normalize();

				Vector n1 = pos1->Normal;
				pos1->Normal = normal + n1;
				pos1->Normal.normalize();
				
				Vector n2 = pos2->Normal;
				pos2->Normal = normal + n2;
				pos2->Normal.normalize();

				Vector n3 = pos3->Normal;
				pos3->Normal = normal + n3;
				pos3->Normal.normalize();

				//Material setzen
				pos1->mat = curmat;
				pos1->hasMat = useMaterial;

				pos2->mat = curmat;
				pos2->hasMat = useMaterial;

				pos3->mat = curmat;
				pos3->hasMat = useMaterial;

			}
			else if (count(lines[i]->begin(), lines[i]->end(), ' ') == 5)
			{
				this->m_VertexIndexCount += 6;

				sscanf(lines[i]->c_str(), "f %d/%d %d/%d %d/%d %d/%d", &v1, &uv1, &v2, &uv2, &v3, &uv3, &v4, &uv4);

				tmp_vertexIndices.push_back(v1);
				tmp_vertexIndices.push_back(v2);
				tmp_vertexIndices.push_back(v3);

				tmp_vertexIndices.push_back(v1);
				tmp_vertexIndices.push_back(v3);
				tmp_vertexIndices.push_back(v4);


				Vertex* pos1 = &(vertices_tmp_unique.at(v1 - 1));
				Vertex* pos2 = &(vertices_tmp_unique.at(v2 - 1));
				Vertex* pos3 = &(vertices_tmp_unique.at(v3 - 1));
				Vertex* pos4 = &(vertices_tmp_unique.at(v4 - 1));

				Vector uvposi1 = tmp_UVCoords.at(uv1 - 1);
				Vector uvposi2 = tmp_UVCoords.at(uv2 - 1);
				Vector uvposi3 = tmp_UVCoords.at(uv3 - 1);
				Vector uvposi4 = tmp_UVCoords.at(uv4 - 1);

				pos1->TexcoordS = uvposi1.X;
				pos1->TexcoordT = 1 - uvposi1.Y;

				pos2->TexcoordS = uvposi2.X;
				pos2->TexcoordT = 1 - uvposi2.Y;

				pos3->TexcoordS = uvposi3.X;
				pos3->TexcoordT = 1 - uvposi3.Y;

				pos4->TexcoordS = uvposi4.X;
				pos4->TexcoordT = 1 - uvposi4.Y;

				Vector normal = ((pos2->Position) - (pos1->Position)).cross((pos3->Position) - (pos1->Position));

				normal.normalize();


				Vector n1 = pos1->Normal;
				pos1->Normal = normal + n1;
				pos1->Normal.normalize();

				Vector n2 = pos2->Normal;
				pos2->Normal = normal + n2;
				pos2->Normal.normalize();

				Vector n3 = pos3->Normal;
				pos3->Normal = normal + n3;
				pos3->Normal.normalize();

				Vector n4 = pos4->Normal;
				pos4->Normal = normal + n4;
				pos4->Normal.normalize();

				//Material setzen
				pos1->mat = curmat;
				pos1->hasMat = useMaterial;

				pos2->mat = curmat;
				pos2->hasMat = useMaterial;

				pos3->mat = curmat;
				pos3->hasMat = useMaterial;

				pos4->mat = curmat;
				pos4->hasMat = useMaterial;

			}
		}
	}
	this->m_pVertices = new Vertex[m_VertexCount];


	this->m_VertexIndices = new unsigned int[this->m_VertexIndexCount];


	for (int i = 0; i < this->m_VertexCount; i++)
	{
		this->m_pVertices[i] = vertices_tmp_unique.at(i);

		if (FitSize)
		{
			this->m_pVertices[i].Position = this->m_pVertices[i].Position * 0.01;
		}
	}

	for (int i = 0; i < this->m_VertexIndexCount; i++)
	{
		this->m_VertexIndices[i] = tmp_vertexIndices.at(i)-1;
	}


    this->m_Box.createBBox(this->m_pVertices, this->m_VertexCount);


	this->genBuffer();

	return true;
}


int Model::loadMaterial(const char* p_matPfad, const char* modellePfad)
{
    std::vector<Material> materials;
    
    std::ifstream mtlin(p_matPfad);
    if(!mtlin.is_open())
    {
        std::cout << "connot open the material file" << std::endl;
        return -1;
    }

    std::vector<std::string> tmp;
    char c[200];
    while(!mtlin.eof())
    {
        mtlin.getline(c,200);   //read all lines to tmp
        tmp.push_back(c);
    }
    char name[200]; //name of the material
    char filename[200];     //filename of the texture

    
    Material mat;

    
    Color diffColor;
    Color specColor;
    Color ambColor;
    Texture diffTex;
    float specExp;
    std::string matname;
    

    bool ismat=false;       //do we already have a material read in to these variables?
    strcpy(filename,"\0");  //set filename to nullbyte character
    
    for(int i=0;i<tmp.size();i++) //go through all lines of the mtllib file
    {
		if (tmp[i] != "")
		{
			if (tmp[i][0] == 'n' && tmp[i][1] == 'e' && tmp[i][2] == 'w')
			{

				if (ismat)       //if we have a material
				{
					materials.push_back(mat);
				}
				ismat = false;
				sscanf(tmp[i].c_str(), "newmtl %s", name);        //read in the name
				mat.setName(name);
			}
			else if (tmp[i][1] == 'N' && tmp[i][2] == 's')      //specular exponent
			{
				//sscanf(tmp[i].c_str(),"Ns %f",&ns);
				sscanf(tmp[i].c_str(), "\t Ns %f", &specExp);
				mat.setSpecularExponent(specExp);
				ismat = true;
			}
			else if (tmp[i][1] == 'K' && tmp[i][2] == 'a')      //the ambient color
			{
				//sscanf(tmp[i].c_str(),"Ka %f %f %f",&amb[0],&amb[1],&amb[2]);
				sscanf(tmp[i].c_str(), "\t Ka %f %f %f", &ambColor.R, &ambColor.G, &ambColor.B);

				mat.setAmbientColor(ambColor);
				ismat = true;
			}
			else if (tmp[i][1] == 'K' && tmp[i][2] == 'd')      //the diffuse color
			{
				//sscanf(tmp[i].c_str(),"Kd %f %f %f",&dif[0],&dif[1],&dif[2]);
				sscanf(tmp[i].c_str(), "\t Kd %f %f %f", &diffColor.R, &diffColor.G, &diffColor.B);
				std::string test = tmp[i].c_str();
				mat.setDiffuseColor(diffColor);
				ismat = true;
			}
			else if (tmp[i][1] == 'K' && tmp[i][2] == 's')      //the specular color
			{
				//sscanf(tmp[i].c_str(),"Ks %f %f %f",&spec[0],&spec[1],&spec[2]);
				sscanf(tmp[i].c_str(), "\t Ks %f %f %f", &specColor.R, &specColor.G, &specColor.B);
				mat.setSpecularColor(specColor);
				ismat = true;
			}

			else if (tmp[i][1] == 'm' && tmp[i][2] == 'a' && tmp[i][3] == 'p' && tmp[i][4] == '_' && tmp[i][5] == 'K' && tmp[i][6] == 'd')      //and the texture
			{
				sscanf(tmp[i].c_str(), "\t map_Kd %s", filename);

				char texturPfad[200] = { 0 };
				strcat(texturPfad, modellePfad);
				strcat(texturPfad, filename);

				mat.setDiffuseTexture(texturPfad);
				mat.hasTexture = true;
				ismat = true;

			}
		}

        
    }
    if(ismat)
    {
        materials.push_back(mat);
    }
    
    m_MaterialCount = materials.size();
    m_pMaterials = new Material[m_MaterialCount];
    
    for(int i = 0; i < m_MaterialCount; i++)
    {
        m_pMaterials[i] = materials.at(i);

    }
    return 0;
}



void Model::setIndices()
{
    this->m_VertexIndexCount = this->m_VertexCount;

    this->m_VertexIndices = new unsigned int[this->m_VertexIndexCount];
    
    
    for(int i = 0; i < this->m_VertexIndexCount; i++)
    {
        this->m_VertexIndices[i] = i;

    }
    

}





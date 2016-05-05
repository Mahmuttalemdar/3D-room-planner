
//mac
#include <GLUT/GLUT.h>
#include <OpenGL/OpenGL.h>

//win
//#include <GL\glew.h>
//#include <GL\freeglut.h>
//#define _USE_MATH_DEFINES
//#include <cmath>

#include <iostream>
#include <math.h>
#include "Camera.h"
#include "model.h"
#include "matrix.h"
#include "Scene.hpp"
#include "ShaderProgram.hpp"
#include "Hud.h"

//win
//const char* modellePfad = "C:/CG/Hausarbeit/CG_Hausarbeit_mk/CG_Hausarbeit_mk/modelle/";

//mac
const char* modellePfad = "/Users/max/Uni/2015WS/Computergrafik/Hausarbeit/Hausarbeit_WS2015/Hausarbeit_WS2015/modelle/";

const unsigned int g_WindowWidth=1280;
const unsigned int g_WindowHeight=720;
const Vector g_LightPos = Vector( 0,15,0);

Camera g_Camera;
Scene s;
Model* g_Model;
Vector Mouse3D;
Vector tankDirection(1,0,0);
Hud g_Hud;
SceneNode* testNode;
float g_Time = 0.0f;
bool g_Active = false;

int g_Mode = -1;

bool ready = false;

bool keySwitch = false;

int mouseXglobal;
int mouseYglobal;

int g_MouseButton = 0;
int g_MouseState = 0;
int g_Button = 0;
int g_DrawMode = 2;

float oldTimeSinceStart = 0;

float ForwardBackward;
float LeftRight;

char szenendatei[50] = "szene_saved.txt";



Vector getMousePosition(int mouseX, int mouseY);
float getDeltaTime();
void SetupDefaultGLSettings();
void DrawScene();
void MouseCallback(int Button, int State, int x, int y);
void MouseMoveCallback(int x, int y);
void KeyboardCallback( unsigned char key, int x, int y);
void KeyboardUpCallback( unsigned char key, int x, int y);
void MousePassiveMoveCallback( int x, int y);
void SpecialKeyboardCallback( int key, int x, int y);
void SpecialKeyboardUpCallback( int key, int x, int y);

char* generateFullPath(const char* p_Path)
{
    char* fullPath = new char[200];
    strcpy(fullPath,"\0");
    strcat(fullPath,modellePfad);
    strcat(fullPath,p_Path);
    return fullPath;
}

int main(int argc, char * argv[])
{
    // initialize the glut system and create a window
    glutInitWindowSize(g_WindowWidth, g_WindowHeight);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutCreateWindow("CG Praktikum");

	//WIN
//	GLenum err = glewInit();
//	if (GLEW_OK != err)
//	{
//		/* Problem: glewInit failed, something is seriously wrong. */
//		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
//	}
//	fprintf(stdout, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
	//WIN
    
    SetupDefaultGLSettings();
    
    glutDisplayFunc(DrawScene);
    glutMouseFunc(MouseCallback);                       //mouse click
    glutKeyboardFunc(KeyboardCallback);
    glutKeyboardUpFunc(KeyboardUpCallback);
    glutMotionFunc(MouseMoveCallback);                  //mouse click and move
    glutPassiveMotionFunc(MousePassiveMoveCallback);    //mouse move, no click
    glutSpecialFunc(SpecialKeyboardCallback);
    glutSpecialUpFunc(SpecialKeyboardUpCallback);

    s.addSceneFile(generateFullPath("szene_saved.txt"), modellePfad);
    
	g_Hud.initHUD();

    glutMainLoop();
    
}


void SetupDefaultGLSettings()
{
    glClearColor(0, 0, 0, 255);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
	//glCullFace(GL_FRONT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(65,  //field of view angle, in degrees, in the y direction.
                   (double)g_WindowWidth/(double)g_WindowHeight,    //field of view in the x direction
                   0.045f,      //distance from the viewer to the near clipping plane
                   1000.0f);    //distance from the viewer to the far clipping plane
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
 
    // Setup Light Color
    GLfloat ambientLight[] = { 0.5f, 0.5f, 0.5f, 0.0f };
    GLfloat diffuseLight[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat specularLight[] = { 1.0f, 1.0f, 1.0f, 0.0f };
    
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
    
    glLightfv(GL_LIGHT1, GL_DIFFUSE, ambientLight);
    glLightfv(GL_LIGHT1, GL_AMBIENT, diffuseLight);
    glLightfv(GL_LIGHT1, GL_SPECULAR, specularLight);

    // Setup default material
    float diff[4] = {1,1,1,1};
    float amb[4]  = {0.2f,0.2f,0.2f,1};
    float spec[4] = {0.5f,0.5f,0.5f,1};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diff);
    glMaterialfv(GL_FRONT, GL_SPECULAR, spec);
    glMateriali(GL_FRONT, GL_SHININESS, 30);
    glMaterialfv(GL_FRONT, GL_AMBIENT, amb);
    
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_NORMALIZE);

}

void DrawGroundGrid()
{
    const float GridSize=10.0f;
    const unsigned int GridSegments=20;
    const float GridStep=GridSize/(float)GridSegments;
    const float GridOrigin=-GridSize*0.5f;
    
    glDisable( GL_LIGHTING);
    glBegin(GL_LINES);
        glColor3f(1.0f, 1.0f, 1.0f);
        for( unsigned int i=0; i<GridSegments+1; i++)
        {
            float itpos=GridOrigin + GridStep*(float)i;
            glVertex3f(itpos, 0, GridOrigin);
            glVertex3f(itpos, 0, GridOrigin+GridSize);
        
            glVertex3f(GridOrigin, 0, itpos);
            glVertex3f(GridOrigin+GridSize, 0, itpos);

        }
    glEnd();
    glEnable( GL_LIGHTING);

}

void MouseCallback(int Button, int State, int x, int y)
{
    g_MouseButton = Button;
    g_MouseState = State;

    if(ready)
    {
        g_Camera.mouseInput(x, y, Button, State);
        
        std::set<SceneNode*>::iterator it;
        std::set<SceneNode*> myset = s.g_SelectedSceneNodesSet;
        if(myset.size() > 0)
        {
            for (it=myset.begin(); it!=myset.end(); ++it)
            {
                (*it)->handleMouseInput(x, y, g_MouseButton, g_MouseState);
            }
        }
    }
}

void MouseMoveCallback( int x, int y)
{
    if(ready)
    {
        g_Camera.mouseInput(x, y, g_MouseButton, g_MouseState);
        
        std::set<SceneNode*>::iterator it;
        std::set<SceneNode*> myset = s.g_SelectedSceneNodesSet;
        if(myset.size() > 0)
        {
            for (it=myset.begin(); it!=myset.end(); ++it)
            {
                (*it)->handleMouseInput(x, y, g_MouseButton, g_MouseState);
            }
        }
    }
}

void MousePassiveMoveCallback( int x, int y)
{
    if(ready)
    {
        std::set<SceneNode*>::iterator it;
        std::set<SceneNode*> myset = s.g_SelectedSceneNodesSet;
        if(myset.size() > 0)
        {
            for (it=myset.begin(); it!=myset.end(); ++it)
            {
                (*it)->handlePassiveMouseInput(x, y, g_MouseButton, g_MouseState);
            }
        }
    }
    
    mouseXglobal = x;
    mouseYglobal = y;
}

void KeyboardCallback( unsigned char key, int x, int y)
{
    g_Button = key;
    switch (key)
    {
        case 27://escape
            g_Mode = 0;
            break;
        case '1':
            g_Mode = 1;
            break;
        case '2':
            g_Mode = 2;
            break;
        case '3':
            g_Mode = 3;
            break;
        case 'a':
            g_DrawMode = 1;
            break;
        case 's':
            g_DrawMode = 2;
            break;
        case 'd':
            g_DrawMode = 3;
            break;
    }
    
}

void KeyboardUpCallback( unsigned char key, int x, int y)
{
    g_Mode = 0;
    g_Button = 0;
}

void SpecialKeyboardCallback( int key, int x, int y)
{

}

void SpecialKeyboardUpCallback( int key, int x, int y)
{

}


void DrawScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    GLfloat lpos[4];
    lpos[0]=g_LightPos.X; lpos[1]=g_LightPos.Y; lpos[2]=g_LightPos.Z; lpos[3]=1;
    GLfloat lposs[4];
    lpos[0]=g_LightPos.X*-1; lpos[1]=g_LightPos.Y*-1; lpos[2]=g_LightPos.Z*-1; lpos[3]=1;
    
    glLightfv(GL_LIGHT0, GL_POSITION, lpos);
    glLightfv(GL_LIGHT1, GL_POSITION, lposs);
	
    
    g_Camera.apply();

    g_Time = getDeltaTime();
	
    s.draw(g_DrawMode);
    
    if(s.recursiveTriangleIntersectionCheck(s.getRoot()) != NULL)
    {
        g_Hud.setHoverOverModell(s.recursiveTriangleIntersectionCheck(s.getRoot())->getName());
        
        if(g_MouseButton == GLUT_LEFT_BUTTON && g_MouseState == GLUT_DOWN)
        {
            g_Hud.setGetroffenesModell(s.recursiveTriangleIntersectionCheck(s.getRoot())->getName());
            
            SceneNode* selectedSceneNode = s.recursiveTriangleIntersectionCheck(s.getRoot());

            selectedSceneNode->setIsActive(true);
            s.g_SelectedSceneNodesSet.insert(selectedSceneNode);
        }
        
        
    }
    else
    {
        g_Hud.setHoverOverModell("nix");
        if(g_MouseButton == GLUT_LEFT_BUTTON && g_MouseState == GLUT_DOWN)
        {
            std::set<SceneNode*>::iterator it;
            std::set<SceneNode*> myset = s.g_SelectedSceneNodesSet;
            if(myset.size() > 0)
            {
                for (it=myset.begin(); it!=myset.end(); ++it)
                {
                    (*it)->setIsActive(false);
                }
            }
            s.g_SelectedSceneNodesSet.clear();
            g_Hud.setGetroffenesModell("nix");
        }
    }
    
    
	g_Hud.drawHUD(g_WindowWidth, g_WindowHeight, mouseXglobal, mouseYglobal);
    g_Hud.draw_buttons(g_WindowWidth, g_WindowHeight, mouseXglobal, mouseYglobal);
    g_Hud.draw_save_button(g_WindowWidth, g_WindowHeight, mouseXglobal, mouseYglobal);
    
    ready = true;
    
    glutSwapBuffers();
    glutPostRedisplay();

}

float getDeltaTime()
{
    
    float timeSinceStart = glutGet(GLUT_ELAPSED_TIME);
    float deltaTime = timeSinceStart - oldTimeSinceStart;
    oldTimeSinceStart = timeSinceStart;
    //deltaTime = deltaTime / 1000;
    
    return deltaTime;
    
}

Vector getMousePosition(int mouseX, int mouseY)
{
    float mouseXnormalized = (2.0f * mouseX) / g_WindowWidth - 1.0f;
    float mouseYnormalized = (2.0f * mouseY) / g_WindowHeight - 1.0f;
    mouseYnormalized = -mouseYnormalized;
    

    
    Vector normalisierterBildraum(mouseXnormalized, mouseYnormalized, 1);
    
    Matrix invertedProjectionMatrix = g_Camera.getProjectionMatrix();
    invertedProjectionMatrix.invert();
    
    Vector kameraRaum = invertedProjectionMatrix * normalisierterBildraum;
    kameraRaum.normalize();
    
    Matrix invertedViewMatrix = g_Camera.getViewMatrix();
    invertedViewMatrix.invert();
    
    Vector weltRaum = invertedViewMatrix.transformVec3x3(kameraRaum);
    
    Vector camPosi = g_Camera.getPosition();
    
    float faktor = -camPosi.Y/weltRaum.Y;
    
    
    Vector schnittpunkt = camPosi + weltRaum * faktor ;
    
    
    return Vector(schnittpunkt.X, 0, schnittpunkt.Z);
}




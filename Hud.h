#ifndef __HUD_H__
#define __HUD_H__


//mac
#include <GLUT/GLUT.h>
#include <OpenGL/OpenGL.h>

//win
//#include <GL\glew.h>
//#include <GL\freeglut.h>


#include "Model.h"
#include "Texture.h"
#include "Scene.hpP"


class Hud
{
private:
    char hoverOverModel[200];
    char getroffenesModel[200];
    char saved[200];
	Model* hud_one;
	Texture hudTexture;
	Texture chair;
    Texture buttons;
    Texture save_button;
    
    bool mouseLeftDown;
    bool mouseRightDown;
    char fps[32];
    float mouseXcoords;
    float mouseYcoords;
    
public:
	Hud();
	~Hud();

	void initHUD();

	void drawHUD(unsigned int width, unsigned int height, float x, float y);
    
    void draw_save_button(unsigned int width, unsigned int height, float x, float y);
	void draw_chair(unsigned int width, unsigned int height, float x, float y);
    void draw_buttons(unsigned int width, unsigned int height, float x, float y);

	int CalculateFrameRate();
	Texture getTexture() { return hudTexture; }
    void setGetroffenesModell(std::string s) { sprintf(this->getroffenesModel, "%s", s.c_str());};
    void setHoverOverModell(std::string s) { sprintf(this->hoverOverModel, "%s", s.c_str());};
    void setSaved(std::string s) { sprintf(this->saved, "%s", s.c_str());};
};

#endif
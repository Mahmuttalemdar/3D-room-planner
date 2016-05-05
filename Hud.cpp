#include "Hud.h"
#include <iostream>
#include <sstream>

//mac
#include <mach/mach.h>
#include <mach/mach_time.h>

extern int g_MouseButton;
extern int g_MouseState;
extern int g_Button;
extern const char* modellePfad;
extern Scene s;
extern char szenendatei[200];


Hud::Hud()
{

}


Hud::~Hud()
{

}


void Hud::initHUD()
{
	this->hudTexture.LoadFromBMP("/Users/max/Uni/2015WS/Computergrafik/Hausarbeit/Hausarbeit_WS2015/Hausarbeit_WS2015/modelle/CG_HUD.bmp");
	this->chair.LoadFromBMP("/Users/max/Uni/2015WS/Computergrafik/Hausarbeit/Hausarbeit_WS2015/Hausarbeit_WS2015/modelle/chair.bmp");
    this->buttons.LoadFromBMP("/Users/max/Uni/2015WS/Computergrafik/Hausarbeit/Hausarbeit_WS2015/Hausarbeit_WS2015/modelle/buttons.bmp");
    this->save_button.LoadFromBMP("/Users/max/Uni/2015WS/Computergrafik/Hausarbeit/Hausarbeit_WS2015/Hausarbeit_WS2015/modelle/save_button.bmp");
    
    sprintf(this->getroffenesModel, "getroffen: %s", "nix");
    sprintf(this->hoverOverModel, "mouse over: %s", "nix");
    sprintf(this->saved, "%s", "nicht gespeichert");
    
    this->mouseLeftDown = false;
    this->mouseRightDown = false;
    this->mouseXcoords = 0.0f;
    this->mouseYcoords = 0.0f;

}

void Hud::draw_chair(unsigned int width, unsigned int height, float mouseX, float mouseY)
{
	// Temporary disable lighting
	glDisable(GL_LIGHTING);

	// Our HUD consists of a simple rectangle
	glMatrixMode(GL_PROJECTION);

	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0f, (float)width, 0.0f, (float)height, -1.0f, 1.0f);
	// left, right, bottom, top, near, far

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	

	this->chair.apply();

	glBegin(GL_QUADS);

	glTexCoord2f(0.0, 1.0);
	glVertex2f(0, height);

	glTexCoord2f(0.0, 0.0);
	glVertex2f(0, height - 300);

	glTexCoord2f(1.0, 0.0);
	glVertex2f(300, height - 300);

	glTexCoord2f(1.0, 1.0);
	glVertex2f(300, height);

	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);

	if (mouseX < 300 && mouseX > 0 && mouseY < 300 && mouseY > 0)
	{
		glColor3f(1.0f, 0.0f, 0.0f);
		glLineWidth(10.0f);

		glBegin(GL_LINE_LOOP);


		glVertex2f(0, height);

		glVertex2f(0, height - 300);

		glVertex2f(300, height - 300);

		glVertex2f(300, height);

		glEnd();
		glLineWidth(1.0f);
	}




	glPopMatrix();


	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	// Reenable lighting
	glEnable(GL_LIGHTING);

	///inet
}

void Hud::draw_buttons(unsigned int width, unsigned int height, float mouseX, float mouseY)
{
    glDisable(GL_LIGHTING);
    
    glMatrixMode(GL_PROJECTION);
    
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0f, (float)width, 0.0f, (float)height, 1.0f, -1.0f);
    // left, right, bottom, top, near, far
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    
    this->buttons.apply();
    
    glBegin(GL_QUADS);
    
    glTexCoord2f(0.0, 1.0);
    glVertex2f(20, 120);
    
    glTexCoord2f(0.0, 0.0);
    glVertex2f(20, 20);
    
    glTexCoord2f(1.0, 0.0);
    glVertex2f(338, 20);
    
    glTexCoord2f(1.0, 1.0);
    glVertex2f(338, 120);
    
    glEnd();
    
    
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
    
    
    if (g_Button == '1')
    {
        glColor3f(1.0f, 0.0f, 0.0f);
        glLineWidth(10.0f);
        
        glBegin(GL_LINE_LOOP);

        glVertex2f(20, 120);
        
        glVertex2f(20, 20);
        
        glVertex2f(126, 20);
        
        glVertex2f(126, 120);
        
        glEnd();
        glLineWidth(1.0f);
    }
    if (g_Button == '2')
    {
        glColor3f(1.0f, 0.0f, 0.0f);
        glLineWidth(10.0f);
        
        glBegin(GL_LINE_LOOP);
        
        glVertex2f(126, 120);
        
        glVertex2f(126, 20);
        
        glVertex2f(232, 20);
        
        glVertex2f(232, 120);
        
        glEnd();
        glLineWidth(1.0f);
    }
    if (g_Button == '3')
    {
        glColor3f(1.0f, 0.0f, 0.0f);
        glLineWidth(10.0f);
        
        glBegin(GL_LINE_LOOP);
        
        glVertex2f(232, 120);
        
        glVertex2f(232, 20);
        
        glVertex2f(338, 20);
        
        glVertex2f(338, 120);
        
        glEnd();
        glLineWidth(1.0f);
    }
    
    
    
    
    glPopMatrix();
    
    
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    
    // Reenable lighting
    glEnable(GL_LIGHTING);
    
    ///inet
}

void Hud::draw_save_button(unsigned int width, unsigned int height, float mouseX, float mouseY)
{
    glDisable(GL_LIGHTING);
    
    glMatrixMode(GL_PROJECTION);
    
    glPushMatrix();
    glLoadIdentity();
    glOrtho(0.0f, (float)width, 0.0f, (float)height, 1.0f, -1.0f);
    // left, right, bottom, top, near, far
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();
    
    
    this->save_button.apply();
    
    glBegin(GL_QUADS);
    
    glTexCoord2f(0.0, 1.0);
    glVertex2f(width-100, height);
    
    glTexCoord2f(0.0, 0.0);
    glVertex2f(width-100, height-100);
    
    glTexCoord2f(1.0, 0.0);
    glVertex2f(width, height-100);
    
    glTexCoord2f(1.0, 1.0);
    glVertex2f(width, height);
    
    glEnd();
    
    
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
    
    
    if (mouseX < width && mouseX > width-100 && mouseY < 100 && mouseY > 0 )
    {
        glColor3f(1.0f, 0.0f, 0.0f);
        glLineWidth(10.0f);
        
        glBegin(GL_LINE_LOOP);
        
        glVertex2f(width-100, height);

        glVertex2f(width-100, height-100);
        
        glVertex2f(width, height-100);
        
        glVertex2f(width, height);
        
        glEnd();
        glLineWidth(1.0f);
        
        if(g_MouseState == GLUT_DOWN)
        {
            char* fullPath = new char[200];
            strcpy(fullPath,"\0");
            strcat(fullPath,modellePfad);
            strcat(fullPath,szenendatei);
            
            s.saveSceneFile(fullPath, modellePfad);
            
            sprintf(this->saved, "%s", szenendatei);
        }
    }
    //savetext
    char selcModel[32];
    
    sprintf(selcModel, "datei: %s", this->saved);
    
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(width/2, height-30.0f, 0.0f);
    glScalef(0.2f, 0.2f, 0.2f);
    
    glColor3f(0.0f, 1.0f, 0.0f);
    
    int h = 0;
    while (*(selcModel + h++) != '\0')
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *(selcModel + h - 1));
    
    glPopMatrix();
    //savetext
    
    
    
    glPopMatrix();
    
    
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    
    // Reenable lighting
    glEnable(GL_LIGHTING);
    
    ///inet
}


void Hud::drawHUD(unsigned int width, unsigned int height, float mouseX, float mouseY)
{
	CalculateFrameRate();

	glDisable(GL_LIGHTING);

	glMatrixMode(GL_PROJECTION);

	glPushMatrix();
	glLoadIdentity();
	glOrtho(0.0f, (float) width,   0.0f,(float) height, 1.0f, -1.0f);
	// left, right, bottom, top, near, far

	glMatrixMode(GL_MODELVIEW);


    //hover
    char hovModel[32];
    
    sprintf(hovModel, "Hover over: %s", this->hoverOverModel);
    
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0.0f, 200.0f, 0.0f);
    glScalef(0.2f, 0.2f, 0.2f);
    
    glColor3f(0.0f, 1.0f, 0.0f);
    
    int hover = 0;
    while (*(hovModel + hover++) != '\0')
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *(hovModel + hover - 1));
    
    glPopMatrix();
    //hover
    
    //hit
    char selcModel[32];
    
    sprintf(selcModel, "Selected Model: %s", this->getroffenesModel);
    
    glPushMatrix();
    glLoadIdentity();
    glTranslatef(0.0f, 150.0f, 0.0f);
    glScalef(0.2f, 0.2f, 0.2f);
    
    glColor3f(0.0f, 1.0f, 0.0f);
    
    int h = 0;
    while (*(selcModel + h++) != '\0')
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *(selcModel + h - 1));
    
    glPopMatrix();
    //hit

	//x
	char xkoords[32];

	sprintf(xkoords, "MouseX: %f", mouseX);

	glPushMatrix();
	glLoadIdentity();
	glTranslatef(mouseX, -mouseY+height, 0.0f);
	glScalef(0.1f, 0.1f, 0.1f);

    glColor3f(0.0f, 1.0f, 0.0f);

	int j = 0;
	while (*(xkoords + j++) != '\0')
		glutStrokeCharacter(GLUT_STROKE_ROMAN, *(xkoords + j - 1));

	glPopMatrix();
	//x

	//y
	char ykoords[32];

	sprintf(ykoords, "MouseY: %f", mouseY);

	glPushMatrix();
	glLoadIdentity();
	glTranslatef(mouseX, -mouseY+height-15, 0.0f);
	glScalef(0.1f, 0.1f, 0.1f);


	int k = 0;
	while (*(ykoords + k++) != '\0')
		glutStrokeCharacter(GLUT_STROKE_ROMAN, *(ykoords + k - 1));

	glPopMatrix();
	//y

	//fps
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(width-50, 0.0f, 0.0f);
	glScalef(0.1f, 0.1f, 0.1f);


	int i = 0;
	while (*(this->fps + i++) != '\0')
		glutStrokeCharacter(GLUT_STROKE_ROMAN, *(this->fps + i - 1));

	glPopMatrix();
	//fps

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	// Reenable lighting
	glEnable(GL_LIGHTING);

	///inet
}


int Hud::CalculateFrameRate()
{
	static float framesPerSecond = 0.0f;       // This will store our fps
	static float lastTime = 0.0f;       // This will hold the time from the last frame
	//float currentTime = GetTickCount() * 0.001f;
    float currentTime = mach_absolute_time() * 0.001f;
	++framesPerSecond;
	if (currentTime - lastTime > 1.0f)
	{
		lastTime = currentTime;
		fprintf(stderr, "\nCurrent Frames Per Second: %d\n\n", (int)framesPerSecond);
		sprintf(this->fps, "FPS: %d", (int)framesPerSecond);
		framesPerSecond = 0;
	}

	
	return 0;
}
//
//  SceneNode.cpp
//  CG_Prak4_neu
//
//  Created by max on 26/11/15.
//  Copyright © 2015 max kohler. All rights reserved.
//
#include "Camera.h"

extern int mouseXglobal;
extern int mouseYglobal;

extern const unsigned int  g_WindowWidth;
extern const unsigned int g_WindowHeight;
extern Camera g_Camera;
extern int g_Mode;

#include "SceneNode.hpp"

SceneNode::SceneNode()
{
    this->name = "default";
    this->m_pModel = NULL;
    this->m_pParent = NULL;
    
    mTranslation.identity();
    mRotation.identity();
    mScale.identity();
    
    //m_lastScale = 1;
    
    this->m_LocalTransform.identity();
    this->m_Scaling = Vector(1,1,1);
}

SceneNode::SceneNode(const std::string& pName,
          const Vector& Translation,
          const Vector& RotationAxis,
          const float RotationAngle,
          const Vector& Scale,
          SceneNode* pParent,
          Model* pModel )
{
    this->name = pName;
    
    Matrix mRot;
    Matrix mTra;
    mRot.identity();
    mTra.identity();
    mRot.rotationAxis(RotationAxis, RotationAngle);
    mTra.translation(Translation);
    this->m_LocalTransform = mTra * mRot;
    
    this->m_Scaling = Scale;
    this->m_pParent = pParent;
    this->m_pModel = pModel;
}

std::string& SceneNode::getName()
{
    return this->name;
}

const Matrix& SceneNode::getLocalTransform() const
{
    //return m_LocalTransform;
    return mTranslation;
}

Matrix SceneNode::getGlobalTransform() const
{
    if(this->m_pParent == NULL)
    {
        return getLocalTransform();
    }
    return m_pParent->getGlobalTransform() * this->getLocalTransform();
}

Vector& SceneNode::getScaling()
{
    return this->m_Scaling;
}

const SceneNode* SceneNode::getParent() const
{
    return this->m_pParent;
}

const std::set<SceneNode*>& SceneNode::getChildren() const
{
    return this->m_Children;
}

Model* SceneNode::getModel()
{
    return this->m_pModel;
}

void SceneNode::setName(std::string& pName)
{
    this->name = pName;
}

void SceneNode::setModel(Model* pModel)
{
    this->m_pModel = pModel;
}

void SceneNode::setLocalTransform(const Vector& Translation, const Vector& RotationAxis, const float RotationAngle)
{
    //sollte nicht mehr benutzt werden
    Matrix mRot;
    Matrix mTra;
    mRot.identity();
    mTra.identity();
    mRot.rotationAxis(RotationAxis, RotationAngle);
    mTra.translation(Translation);
    this->m_LocalTransform = mTra * mRot;
    //
}

void SceneNode::setRotation(const Vector& RotationAxis, const float RotationAngle)
{
    Matrix mRot;
    mRot.identity();
    mRot.rotationAxis(RotationAxis, RotationAngle);
    this->mRotation = mRot;
}

void SceneNode::setLocalTransform(const Matrix& localTransform)
{
    //this->m_LocalTransform = localTransform;
    this->mTranslation = localTransform;
}

void SceneNode::setLocalTransform(const Vector& localTransform)
{
    Matrix mTra;
    mTra.identity();
    mTra.translation(localTransform);
    //this->m_LocalTransform = mTra;
    this->mTranslation = mTra;

}

void SceneNode::setScaling(const Vector& Scaling)
{
    //this->m_Scaling = Scaling;
    Matrix mSca;
    mSca.identity();
    mSca.scale(Scaling);
    this->mScale = mSca;
    
    //zum abspeichern in vectorform
    this->m_Scaling = Scaling;
}

void SceneNode::setScaling(const Matrix& Scaling)
{
    this->mScale = Scaling;
}

void SceneNode::setParent(SceneNode* pParent)
{
    this->m_pParent = pParent;
}

void SceneNode::addChild(SceneNode* pChild)
{
    this->m_Children.insert(pChild);
}

void SceneNode::removeChild(SceneNode* pChild)
{
    this->m_Children.erase(pChild);
}

void SceneNode::intersectionCheck()
{
    
}

Vector SceneNode::MousePicking(int x, int y)
{
    Matrix projMatrix = g_Camera.getProjectionMatrix();
    projMatrix.invert();
    
    float mouseXnorm = (2.0f * ((float)x) / g_WindowWidth) - 1.0f;
    float mouseYnorm = (2.0f * (float)y / g_WindowHeight) - 1.0f;
    m_lastMouseX = mouseXnorm;
    m_lastMouseY = mouseYnorm;
    
    mouseYnorm = -mouseYnorm;
    
    Vector ray(mouseXnorm, mouseYnorm, 1);
    ray = projMatrix * ray;
    ray.normalize();
    
    Matrix inverseViewM = g_Camera.getViewMatrix();
    inverseViewM.invert();
    
    //Code-Block von Max
    Vector directionVec = inverseViewM.transformVec3x3(ray);
    return directionVec;
}


void SceneNode::handlePassiveMouseInput(int x, int y, int Button, int State)
{
    if (g_Mode == 1)
    {
        this->placeObject(mouseXglobal, mouseYglobal);
    }
    if (g_Mode == 2)
    {
        m_fetchScale = this->middleClickScale(mouseXglobal, mouseYglobal);
        m_lastScale = m_fetchScale;
    }
    if (g_Mode == 3)
    {
        m_fetchRotation = this->rightClickRotation(mouseXglobal, mouseYglobal);
        m_lastRotation = m_fetchRotation;
    }
    
    if (g_Mode == 0)
    {
        //m_lastScale = m_fetchScale;
        //m_lastRotation = m_fetchRotation;
        bool boolhit = this->selectObject(mouseXglobal, mouseYglobal);
    }
}

void SceneNode::handleMouseInput(int x, int y, int Button, int State)
{
//    if (/*Button == GLUT_MIDDLE_BUTTON &&*/ State == GLUT_DOWN && g_Mode == 2)
//    {
//        //m_fetchScale = this->middleClickScale(mouseXglobal, mouseYglobal);
//    }
//    else
//    {
//        if (/*Button == GLUT_MIDDLE_BUTTON &&*/ State == GLUT_UP && g_Mode == 0)
//        {
//            //m_lastScale = m_fetchScale;
//        }
//    }
//    if (/*Button == GLUT_RIGHT_BUTTON &&*/ State == GLUT_DOWN && g_Mode == 3)
//    {
//        m_fetchRotation = this->rightClickRotation(mouseXglobal, mouseYglobal);
//    }
//    else
//    {
//        if (Button == GLUT_RIGHT_BUTTON && State == GLUT_UP && g_Mode == 0)
//        {
//            m_lastRotation = m_fetchRotation;
//        }
//    }
//    if (Button == GLUT_LEFT_BUTTON && State == GLUT_DOWN && g_Mode == 0)
//    {
//        if (this->getIsHitByMouseRay())
//        {
//            g_Mode = 3;
//            this->setIsActive(true);
//        }
//    }
//    else
//    {
//        if (g_Mode == 3)
//        {
//            if (Button == GLUT_LEFT_BUTTON && State == GLUT_DOWN)
//            {
//                g_Mode = 0;
//            }
//        }
//    }	
}

void SceneNode::placeObject(int x, int y)
{
    Vector directionVec = this->MousePicking(x, y);
    
    Vector camPosi = g_Camera.getPosition();
    float faktor = -camPosi.Y / directionVec.Y;
    Vector intersection = camPosi + directionVec * faktor;
    
    Vector locPosi = this->mTranslation.translation();
    
    mTranslation.translation(intersection.X, locPosi.Y, intersection.Z);
    //mTranslation.translation(intersection.X, 0.0f, intersection.Z);

    this->setLocalTransform(mTranslation);
}

float SceneNode::middleClickScale(int x, int y)
{
    float mouseYnorm = (2.0f * (float)y / g_WindowHeight) - 1.0f - m_lastMouseY;
    mouseYnorm = -mouseYnorm;
    
    //float currentScale = m_lastScale + mouseYnorm * 10.75f;
    float currentScale = y/(float) g_WindowHeight;
//    if (currentScale <= 0.05f) {
//        currentScale = 0.05f;
//    }
    mScale.scale(currentScale);
    //mScale.scale(y/(float) g_WindowHeight);
    
    //fürs abspeichern
    this->m_lastScale = currentScale;
    
    
    return (currentScale);
}

float SceneNode::rightClickRotation(int x, int y)
{
    float mouseXnorm = (2.0f * ((float)x) / g_WindowWidth) -1.0f -m_lastMouseX + m_lastRotation;
    float mouseYnorm = (2.0f * (float)y / g_WindowHeight) -1.0f -m_lastMouseY;
    
    //float currentRotation = mouseXnorm*5.0f;
    float currentRotation = x/(float)g_WindowWidth *5.0f;
    mRotation = mRotation.rotationY(currentRotation * M_PI * 2.2f); //rotationSpeed * deltaTime)
    
    this->m_lastRotation = currentRotation * M_PI * 2.2f;
    
    return currentRotation;
}

void SceneNode::setIsHitByMouseRay(bool b)
{
    this->m_isHitByMouseRay = b;
}

bool SceneNode::getIsHitByMouseRay()
{
    return this->m_isHitByMouseRay;
}

void SceneNode::setIsActive(bool b)
{
    this->m_isActive = b;
}

bool SceneNode::getIsActive()
{
    return this->m_isActive;
}

bool SceneNode::selectObject(int x, int y)
{
    if (this->selectBoundingBox(x, y))
    {
        if (this->selectModel(x, y))
        {
            this->setIsHitByMouseRay(true);
            return true;
        }
    }
    else
    {
        this->setIsHitByMouseRay(false);
        return false;
    }
    return false;
}

bool SceneNode::selectModel(int x, int y)
{
    //Aufpassen, Objekt muss vollstendig geladen sein, vor Mausabfrage.
    //sonst passiert Zugriff auf noch leeren Speicherbereich, weil Mousefunc schneller als laden

        bool isHit = false;
        float currentDistOfTria;
        float shortestDist = std::numeric_limits<float>::infinity();

        Vertex* vertices = this->getModel()->m_pVertices;
    

    
        Vector intersection = this->MousePicking(x, y);
        
        for (int i = 0; i < this->getModel()->m_VertexIndexCount; i += 3)
        {
            int vertInd0 = this->getModel()->m_VertexIndices[i];
            int vertInd1 = this->getModel()->m_VertexIndices[i+1];
            int vertInd2 = this->getModel()->m_VertexIndices[i+2];
            
            Vector a(vertices[vertInd0].Position.X, vertices[vertInd0].Position.Y, vertices[vertInd0].Position.Z);
            a =  this->getGlobalTransform()* this->mRotation * this->mScale * a;
            Vector b(vertices[vertInd1].Position.X, vertices[vertInd1].Position.Y, vertices[vertInd1].Position.Z);
            b = this->getGlobalTransform()* this->mRotation * this->mScale * b;
            Vector c(vertices[vertInd2].Position.X, vertices[vertInd2].Position.Y, vertices[vertInd2].Position.Z);
            c = this->getGlobalTransform()* this->mRotation * this->mScale * c;
            
            if (g_Camera.getPosition().triangleIntersection(intersection, a, b, c, currentDistOfTria))
            {
                if (currentDistOfTria < shortestDist)
                {
                    shortestDist = currentDistOfTria;
                    isHit = true;
                }
            }
        }
        if (isHit == true)
        {
            this->setIsHitByMouseRay(true);
            return true;
        }
        else
        {
            this->setIsHitByMouseRay(false);
            return false;
        }
}

bool SceneNode::selectBoundingBox(int x, int y)
{
    //Aufpassen, Objekt muss vollstendig geladen sein, vor Mausabfrage.
    //sonst passiert Zugriff auf noch leeren Speicherbereich, weil Mousefunc schneller als laden

        bool isHit = false;
        float currentDistOfTria;
        float shortestDist = std::numeric_limits<float>::infinity();
        Vertex* vertices = this->getModel()->getBoundingBox().m_pBoundingVertices;
        Vector intersection = this->MousePicking(x, y);
    
    
        for (int i = 0; i < this->getModel()->getBoundingBox().m_BoundingVertexCount; i += 3)
        {
            Vector a(vertices[i].Position.X, vertices[i].Position.Y, vertices[i].Position.Z);
            a =  this->getGlobalTransform()* this->mRotation * this->mScale * a;
            Vector b(vertices[i + 1].Position.X, vertices[i + 1].Position.Y, vertices[i + 1].Position.Z);
            b = this->getGlobalTransform()* this->mRotation * this->mScale * b;
            Vector c(vertices[i + 2].Position.X, vertices[i + 2].Position.Y, vertices[i + 2].Position.Z);
            c = this->getGlobalTransform()* this->mRotation * this->mScale * c;
            
            if (g_Camera.getPosition().triangleIntersection(intersection, a, b, c, currentDistOfTria))
            {
                if (currentDistOfTria < shortestDist)
                {
                    shortestDist = currentDistOfTria;
                    isHit = true;
                }
            }
        }
        if (isHit == true) {
            return true;
        }
        else {
            this->setIsHitByMouseRay(false);
            return false;
        }
}

void SceneNode::drawSceneNode(bool picked)
{
    Matrix m, ma,mb,mc;
    Matrix mpos, mrot, msca;

    m = this->getGlobalTransform() * Matrix().scale(this->getScaling());
    
    mpos = this->mTranslation;
    mrot = this->mRotation;
    msca = this->mScale;

    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    
    
    glMultMatrixf(mpos* mrot * msca);
    this->getModel()->drawWithBuffer(picked);
    glPopMatrix();
}

void SceneNode::drawSceneNodeWithBoundingBox()
{
    Matrix m, ma,mb,mc;
    Matrix mpos, mrot, msca;

    m = this->getGlobalTransform() * Matrix().scale(this->getScaling());
    
    mpos = this->mTranslation;
    mrot = this->mRotation;
    msca = this->mScale;
    
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    
    
    glMultMatrixf(mpos* mrot * msca);
    this->getModel()->drawWithBuffer(false);
    this->getModel()->getBoundingBox().drawBBox();
    glPopMatrix();
}

void SceneNode::drawSceneNodeOnlyBoundingBox()
{
    Matrix m, ma,mb,mc;
    Matrix mpos, mrot, msca;
    
    m = this->getGlobalTransform() * Matrix().scale(this->getScaling());
    
    mpos = this->mTranslation;
    mrot = this->mRotation;
    msca = this->mScale;
    
    
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    
    
    glMultMatrixf(mpos* mrot * msca);

    this->getModel()->getBoundingBox().drawBBox();
    glPopMatrix();
}

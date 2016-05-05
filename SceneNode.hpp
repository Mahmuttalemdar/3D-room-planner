//
//  SceneNode.hpp
//  CG_Prak4_neu
//
//  Created by max on 26/11/15.
//  Copyright © 2015 max kohler. All rights reserved.
//

#ifndef SceneNode_hpp
#define SceneNode_hpp

#include <stdio.h>
#include <string>
#include "vector.hpp"
#include "Matrix.h"
#include <set>
#include "Model.h"
#include <cmath>

class SceneNode
{
public:
    SceneNode();
    SceneNode(const std::string& name,
              const Vector& Translation,
              const Vector& RotationAxis,
              const float RotationAngle,
              const Vector& Scale,
              SceneNode* pParent,
              Model* pModel );
    
    std::string& getName();
    const Matrix& getLocalTransform() const;
    Matrix getGlobalTransform() const;
    Vector& getScaling();
    const SceneNode* getParent() const;
    const std::set<SceneNode*>& getChildren() const;
    Model* getModel();
    
    void setName(std::string& pName);
    void setModel(Model* pModel);
    
    void setLocalTransform(const Vector& Translation, const Vector& RotationAxis, const float RotationAngle);
    void setLocalTransform(const Matrix& localTransform);
    void setLocalTransform(const Vector& localTransform);
    void setRotation(const Vector& RotationAxis, const float RotationAngle);
    void setScaling(const Vector& Scaling);
    void setScaling(const Matrix& Scaling);
    
    void setParent(SceneNode* pParent);
    void addChild(SceneNode* pChild);
    void removeChild(SceneNode* pChild);
    
    void intersectionCheck();
    Vector MousePicking(int x, int y);
    bool shootRayOnObject(int x, int y, Vertex* vert, bool isObject);
    void handlePassiveMouseInput(int x, int y, int Button, int State);
    void handleMouseInput(int x, int y, int Button, int State);
    void placeObject(int x, int y);
    float middleClickScale(int x, int y);
    float rightClickRotation(int x, int y);
    void setIsHitByMouseRay(bool b);
    bool getIsHitByMouseRay();
    void setIsActive(bool b);
    bool getIsActive();
    bool selectObject(int x, int y);
    bool selectModel(int x, int y);
    bool selectBoundingBox(int x, int y);
    
    void drawSceneNode(bool picked);
    void drawSceneNodeWithBoundingBox();
    void drawSceneNodeOnlyBoundingBox();
    
//protected:
    std::string name;
    Model* m_pModel;
    SceneNode* m_pParent;
    std::set<SceneNode*> m_Children;
    
    Matrix m_LocalTransform;    //steht in datei
    Vector m_Scaling;           //steht in datei
    
//    Matrix aktuellePosition;
//    Matrix neuePosition;
    
    float m_lastMouseX;     //benoetigt für mauspicking
    float m_lastMouseY;     //benoetigt für mauspicking
    
    bool m_isActive;
    bool m_isHitByMouseRay;
    
    //
    Matrix mTranslation;     //gesetzt durch tobis methoden
    Matrix mRotation;       //gesetzt durch tobis methoden
    Matrix mScale;          //gesetzt durch tobis methoden
    //
    
    float m_lastRotation;
    float m_fetchRotation;
    
    float m_lastScale;
    float m_fetchScale;
    
};
#endif /* SceneNode_hpp */

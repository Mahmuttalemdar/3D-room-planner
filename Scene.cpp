//
//  Scene.cpp
//  CG_Prak4_neu
//
//  Created by max on 26/11/15.
//  Copyright © 2015 max kohler. All rights reserved.
//

#include "Scene.hpp"
#include <fstream>
#include "vector.hpp"
#include <cstring>
#include <sstream>


extern int mouseXglobal;
extern int mouseYglobal;

Scene::Scene()
{

}

Scene::~Scene()
{
    
}

bool Scene::addSceneFile(const char* Scenefile, const char* modellePfad)
{
    SceneNode* root;
    this->m_Root = root;
    
    char in_room_name[200];
    Vector in_room_size;
    char in_room_wallpaper_texture[200];
    //float in_room_wallpaper_tiling[2];
    Vector in_room_wallpaper_tiling;
    char in_room_model[200];
    
    char in_object_name[200];
    Vector in_object_translation;
    Vector in_object_rotation;
    float in_rotationAngle;
    Vector in_object_scaling;
    char in_object_model[200];
    char in_object_parent[200];
    
    
    
    std::vector<std::string*> coord;
    
    std::ifstream in(Scenefile);
    if(!in.is_open())
    {
        std::cout << "datei nicht geoeffnet" << std::endl;
        return -1;
    }
    
    char buf[256];
    
    while(!in.eof())
    {
        in.getline(buf,256);
        coord.push_back(new std::string(buf));
    }
    
    for(int i=0;i<coord.size();i++)
    {
        
        if((*coord[i])[0]=='r' && (*coord[i])[1]=='o' && (*coord[i])[2]=='o' && (*coord[i])[3]=='m')
        {
            sscanf(coord[i]->c_str(),"roomname %s { size %f %f %f wallpaper { texture %s tiling %f %f } model %s }",
                   in_room_name,
                   &in_room_size.X,
                   &in_room_size.Y,
                   &in_room_size.Z,
                   in_room_wallpaper_texture,
                   &in_room_wallpaper_tiling.X,
                   &in_room_wallpaper_tiling.Y,
                   in_room_model);
            
            SceneNode* rootNode = new SceneNode();
            rootNode->m_isActive =false;
            this->m_Root = rootNode;
            
            std::string s_in_room_name(in_room_name);
            
            
            Model* model = new Model();
            
            char fullPath[200] = {0};
            strcat(fullPath,modellePfad);
            strcat(fullPath,in_room_model);
            
            model->load(fullPath, true, modellePfad);
            this->m_Models.insert(std::pair<std::string, Model*>(in_room_name, model));
            
            this->m_Root->setScaling(in_room_size);
            this->m_Root->setModel(model);
            this->m_Root->setName(s_in_room_name);
            
            this->m_SceneNodesSet.insert(this->m_Root);
            
        }
        else
        {
            sscanf(coord[i]->c_str(),"object %s { translation %f %f %f rotation %f %f %f %f scaling %f %f %f model %s parent %s }",
                   in_object_name,
                   &in_object_translation.X, &in_object_translation.Y, &in_object_translation.Z,
                   &in_object_rotation.X, &in_object_rotation.Y, &in_object_rotation.Z,
                   &in_rotationAngle,
                   &in_object_scaling.X, &in_object_scaling.Y, &in_object_scaling.Z,
                   &in_object_model,
                   &in_object_parent);
            
            SceneNode* newNode = new SceneNode();
            newNode->m_isActive = false;
            
            Model* model = new Model();
            
            char fullPath[200] = {0};
            strcat(fullPath,modellePfad);
            strcat(fullPath,in_object_model);
            
            model->load(fullPath, true, modellePfad);
            this->m_Models.insert(std::pair<std::string, Model*>(in_object_name, model));
            
            std::string s_in_object_name(in_object_name);
            newNode->setName(s_in_object_name);
            
            std::string s_in_object_parent(in_object_parent);
            
            if(s_in_object_parent == "NULL")
            {
                newNode->setParent(this->m_Root);
                this->m_Root->addChild(newNode);
            }
            else
            {
                SceneNode* parent = findSceneNode(s_in_object_parent);
                if(parent == NULL)
                {
                    parent = m_Root;
                }
                newNode->setParent(parent);
                parent->addChild(newNode);
            }
            
            std::string s_in_model_ID(in_object_name);
            newNode->setModel(m_Models.at(s_in_model_ID));
            
            //newNode->setLocalTransform(in_object_translation, in_object_rotation, in_rotationAngle);
            
            newNode->m_lastRotation = in_rotationAngle;
            newNode->m_lastScale = in_object_scaling.X;
            
            newNode->setLocalTransform(in_object_translation);
            newNode->setRotation(in_object_rotation, in_rotationAngle);
            newNode->setScaling(in_object_scaling);
  
            this->m_SceneNodesSet.insert(newNode);
        }
    }
    return false;
}

bool Scene::saveSceneFile(const char* Scenefile, const char* modellePfad)
{
    std::ofstream saveFile;
    saveFile.open(Scenefile);
    
    
//    saveFile << "roomname " << this->getRoot()->getName() << " { size " << raumSize.X << " " << raumSize.Y << " " << raumSize.Z << " " << " wallpaper " << " { texture flower.bmp tiling 3 1 } model " << this->getRoot()->name << "}\n";
//    saveFile.close();
    
    SceneNode* s = this->findSceneNode("vase");
    SceneNode* sa = this->findSceneNode("vase2");
    SceneNode* sb = this->findSceneNode("tisch");
    SceneNode* sc = this->findSceneNode("tisch2");
    
    Vector trans = s->getLocalTransform().translation();
    Vector transa = sa->getLocalTransform().translation();
    Vector transb = sb->getLocalTransform().translation();
    Vector transc = sc->getLocalTransform().translation();
    
    float scal = s->m_lastScale;
    float scala = sa->m_lastScale;
    float scalb = sb->m_lastScale;
    float scalc = sc->m_lastScale;
    
    float rota = s->m_lastRotation;
    float rotaa = sa->m_lastRotation;
    float rotab = sb->m_lastRotation;
    float rotac = sc->m_lastRotation;
    
    
    saveFile << "roomname wohnzimmer { size 1 1 1 wallpaper { texture flower.bmp tiling 3 1 } model room.obj }\n";
    saveFile << "object vase { translation " << trans.X << " " << trans.Y << " " << trans.Z << " "<< " rotation 0 1 0 " << rota << " scaling " << scal << " " << scal << " " << scal<< " model vase.obj parent NULL }\n";

    saveFile << "object vase2 { translation " << transa.X << " " << transa.Y << " " << transa.Z << " "<< " rotation 0 1 0 " << rotaa << " scaling " << scala << " " << scala << " " << scala << " model vase2.obj parent NULL }\n";
    
    saveFile << "object tisch { translation " << transb.X << " " << transb.Y << " " << transb.Z << " "<< " rotation 0 1 0 " << rotab << " scaling " << scalb << " " << scalb << " " << scalb << " model tisch.obj parent NULL }\n";
    
    saveFile << "object tisch2 { translation " << transc.X << " " << transc.Y << " " << transc.Z << " "<< " rotation 0 1 0 " << rotac << " scaling " << scalc << " " << scalc << " " << scalc << " model tisch2.obj parent NULL }";
    saveFile.close();
    
    return true;
    
}


void Scene::draw(int mode)
{
    std::set<SceneNode*>::iterator it;
    for (it = this->m_SceneNodesSet.begin(); it != this->m_SceneNodesSet.end(); ++it)
    {
        this->draw(*it, mode);
    }
}

void Scene::draw(SceneNode* pNode, int mode)
{
    if(pNode != NULL)
    {
        if(mode == 1)
        {
            pNode->drawSceneNodeWithBoundingBox();
        }
        else if (mode == 2)
        {
            pNode->drawSceneNode(pNode->m_isActive);
        }
        else if (mode == 3)
        {
            pNode->drawSceneNodeOnlyBoundingBox();
        }
    }
}

SceneNode* Scene::findSceneNode(std::string name)
{
    std::set<SceneNode*>::iterator it;
    for (it = this->m_SceneNodesSet.begin(); it != this->m_SceneNodesSet.end(); ++it)
    {
        if((*it)->getName() == name)
        {
            return (*it);
        }
    }
    return NULL;
}




SceneNode* Scene::recursiveGetChildren(std::string name, SceneNode* startNode) const
{
    SceneNode* sn = NULL;
    std::set<SceneNode*>::iterator it;
    std::set<SceneNode*> myset = startNode->getChildren();
    if(myset.size() > 0)
    {
        for (it=myset.begin(); it!=myset.end(); ++it)
        {
            if((*it)->getName() == name)
            {
                return *it;
            }
        }
        for (it=myset.begin(); it!=myset.end(); ++it)
        {
            if(*it != NULL)
            {
                sn = recursiveGetChildren(name, *it);
            }
        }
    }
    else
    {
        return NULL;
    }
    return sn;
}

SceneNode* Scene::recursiveTriangleIntersectionCheck(SceneNode* startNode)
{
    SceneNode* sn = NULL;
    std::set<SceneNode*>::iterator it;
    std::set<SceneNode*> myset = startNode->getChildren();
    if(myset.size() > 0)
    {
        for (it=myset.begin(); it!=myset.end(); ++it)
        {
            if((*it)->selectObject(mouseXglobal, mouseYglobal) == true)
            {
                return *it;
            }

        }
        for (it=myset.begin(); it!=myset.end(); ++it)
        {
            if(*it != NULL)
            {
                sn = recursiveTriangleIntersectionCheck(*it);
            }
        }
    }
    else
    {
        return NULL;
    }
    return sn;
}

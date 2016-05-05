//
//  Scene.hpp
//  CG_Prak4_neu
//
//  Created by max on 26/11/15.
//  Copyright © 2015 max kohler. All rights reserved.
//

#ifndef Scene_hpp
#define Scene_hpp

#include <stdio.h>
#include "Scenenode.hpp"

class Scene
{
public:
    Scene();
    ~Scene();
    bool addSceneFile(const char* Scenefile, const char* modellePfad);
    bool saveSceneFile(const char* Scenefile, const char* modellePfad);
    void draw(int mode);
    SceneNode* getRoot() { return this->m_Root;};
    void checkIntersections();
    SceneNode* recursiveTriangleIntersectionCheck(SceneNode* startNode);
        std::set<SceneNode*> g_SelectedSceneNodesSet;
protected:
        SceneNode* findSceneNode(std::string name);
        void drawAllChildren(SceneNode* startNode);
        SceneNode* recursiveGetChildren(std::string name, SceneNode* startNode) const;
    void draw(SceneNode* node, int mode);
    SceneNode* m_Root;
    std::map<std::string, Model*> m_Models;
        std::set<SceneNode*> m_SceneNodesSet;
};
#endif /* Scene_hpp */


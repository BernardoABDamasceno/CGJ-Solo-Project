#include "./mglScenegraph.hpp"

namespace mgl{

    SceneGraph::SceneGraph(){}

    void SceneGraph::draw(){ this->root->draw(); }

    void SceneGraph::setCamera(glm::mat4 viewMatrix, glm::mat4 projectionMatrix){
        this->Camera->setViewMatrix(viewMatrix);
        this->Camera->setProjectionMatrix(projectionMatrix);
    }

    void SceneGraph::setCameraView(glm::mat4 viewMatrix){ this->Camera->setViewMatrix(viewMatrix); }

    void SceneGraph::setCameraProjection(glm::mat4 projectionMatrix){ this->Camera->setProjectionMatrix(projectionMatrix); }

    void SceneGraph::createCamera(GLuint UBO_BP){ this->Camera = new mgl::Camera(UBO_BP); }

    mgl::Camera* SceneGraph::getCamera(){ return this->Camera; }

    mgl::SceneNode* SceneGraph::getRoot(){ return this->root; }

}
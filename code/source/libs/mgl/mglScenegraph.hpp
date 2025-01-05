////////////////////////////////////////////////////////////////////////////////
//
// Scene Management Class
//
// Copyright (c)2022-24 by Carlos Martinho
//
////////////////////////////////////////////////////////////////////////////////

#ifndef MGL_SCENEGRAPH_HPP
#define MGL_SCENEGRAPH_HPP
#include <glm/glm.hpp>
#include "./mglSceneNode.hpp"
#include "./mglIDrawable.hpp"
#include "./mglCamera.hpp"

namespace mgl {

class SceneGraph;

////////////////////////////////////////////////////////////////////// SceneGraph

class SceneGraph: public IDrawable {
  public:
    SceneGraph();
    
    // Setters
    void setCamera(glm::mat4 viewMatrix, glm::mat4 projectionMatrix);
    void setCameraView(glm::mat4 viewMatrix);
    void setCameraProjection(glm::mat4 projectionMatrix);

    void createCamera(GLuint UBO_BP);

    // Getters
    mgl::Camera* getCamera();
    mgl::SceneNode* getRoot();

    void draw() override;

  private:
    mgl::SceneNode *root = new mgl::SceneNode();
    mgl::Camera *Camera = nullptr;
};

////////////////////////////////////////////////////////////////////////////////
}  // namespace mgl

#endif /* MGL_SCENEGRAPH_HPP */

#ifndef MGL_SCENENODE_HPP
#define MGL_SCENENODE_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

#include <list>
#include "./mglShader.hpp"
#include "./mglMesh.hpp"
#include "./mglConventions.hpp"

namespace mgl{
class SceneNode;

class SceneNode: public IDrawable{

public:
    SceneNode(SceneNode *parent = nullptr, mgl::Mesh *mesh = nullptr, mgl::ShaderProgram *shader = nullptr, glm::vec3* value = nullptr, glm::mat4 modelMatrix = glm::mat4(1.0f));

    // Setters
    void setModelMatrix(glm::mat4 modelMatrix);
    void setMesh(mgl::Mesh *mesh);
    void setShader(mgl::ShaderProgram *shader);
    void setColor(glm::vec3* value);


    void createSon(mgl::Mesh *mesh, mgl::ShaderProgram *shader, glm::vec3* value, glm::mat4 modelMatrix);
   
    // Getters
    glm::mat4 getModelMatrix();
    mgl::Mesh* getMesh();
    mgl::ShaderProgram* getShader();
    std::vector<SceneNode*> getSons();
    SceneNode getParent();


    void draw() override;

    void rotateLocal(glm::vec3 rotation, float angle);
    void rotateGlobal(glm::vec3 rotation, float angle);
    void translate(glm::vec3 translation);
    void scale(glm::vec3 scale);

private:
    glm::mat4 ModelMatrix;
    mgl::Mesh *Mesh;
    mgl::ShaderProgram *Shader;
    glm::vec3* color;
    glm::quat GlobalOrientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

    SceneNode *Parent;
    std::vector<SceneNode*> Sons;

    GLint ModelMatrixId;
    GLint ColorAttributeId;
    
    void setParent(SceneNode *parent);
};

}
#endif
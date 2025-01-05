#include "./mglSceneNode.hpp"
#include <iostream>

namespace mgl{

    SceneNode::SceneNode(SceneNode *parent, mgl::Mesh *mesh, mgl::ShaderProgram *shader, glm::vec3* value, glm::mat4 modelMatrix)
    {
        this->Parent = parent;
        this->Mesh = mesh;
        this->Shader = shader;
        this->ModelMatrix = modelMatrix; 
        this->color = value;

        if(Shader != nullptr){
            this->ModelMatrixId = this->Shader->Uniforms[mgl::MODEL_MATRIX].index;
            this->ColorAttributeId = this->Shader->Uniforms[mgl::COLOR_ATTRIBUTE].index;
        }
    }

    void SceneNode::setColor(glm::vec3* value){ this->color = value; }

    void SceneNode::setModelMatrix(glm::mat4 modelMatrix){ this->ModelMatrix = modelMatrix; }

    void SceneNode::setMesh(mgl::Mesh *mesh){ this->Mesh = mesh; }

    void SceneNode::setShader(mgl::ShaderProgram *shader){
        this->Shader = shader;
        this->ModelMatrixId = this->Shader->Uniforms[mgl::MODEL_MATRIX].index;
        this->ColorAttributeId = this->Shader->Uniforms[mgl::COLOR_ATTRIBUTE].index;
    }

    void SceneNode::createSon(mgl::Mesh *mesh, mgl::ShaderProgram *shader, glm::vec3* value, glm::mat4 modelMatrix){
        this->Sons.push_back(new SceneNode(this, mesh, shader, value, modelMatrix));
    }

    glm::mat4 SceneNode::getModelMatrix(){ return this->ModelMatrix; }

    mgl::Mesh* SceneNode::getMesh(){ return this->Mesh; }

    mgl::ShaderProgram* SceneNode::getShader(){ return this->Shader; }

    std::vector<SceneNode*> SceneNode::getSons(){ return this->Sons; }

    SceneNode SceneNode::getParent(){ return this->Parent; }

    void SceneNode::setParent(SceneNode *parent){ this->Parent = parent; }

    void SceneNode::draw(){
        if(this->Shader == nullptr || this->Mesh == nullptr){        
            for(SceneNode* i : Sons)
                i->draw();
            return;
        }

        this->Shader->bind();
        
        glUniformMatrix4fv(this->ModelMatrixId, 1, GL_FALSE, glm::value_ptr(this->ModelMatrix));
        glUniform3fv(this->ColorAttributeId, 1, glm::value_ptr(*this->color));
        
        this->Mesh->draw();

        this->Shader->unbind();

        for(SceneNode* i : Sons)
                i->draw();
    }

    void SceneNode::rotateLocal(glm::vec3 rotation, float angle){
        this->ModelMatrix = glm::rotate(this->ModelMatrix, glm::radians(angle), rotation);

        for(SceneNode* i : Sons)
            i->rotateLocal(rotation, angle);
    }

    void SceneNode::rotateGlobal(glm::vec3 rotation, float angle){
        // Create a quaternion from the rotation axis and angle
        glm::quat quaternion = glm::angleAxis(glm::radians(angle), rotation);
        
        // Update the orientation quaternion
        this->GlobalOrientation = quaternion * this->GlobalOrientation;
        
        // Create a rotation matrix from the quaternion
        glm::mat4 rotationMatrix = glm::toMat4(quaternion);
        
        // Combine the new rotation with the existing model matrix
        this->ModelMatrix = rotationMatrix * this->ModelMatrix;

        for(SceneNode* i : Sons)
            i->rotateGlobal(rotation, angle);
    }

    void SceneNode::translate(glm::vec3 translation){
        this->ModelMatrix = glm::translate(this->ModelMatrix, translation);

        for(SceneNode* i : Sons)
            i->translate(translation);
    }

    void SceneNode::scale(glm::vec3 scale){
        this->ModelMatrix = glm::scale(this->ModelMatrix, scale);

        for(SceneNode* i : Sons)
            i->scale(scale);
    }

}

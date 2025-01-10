#include "app.hpp"

#include <iostream>
#include <fstream>

///////////////////////////////////////////////////////////////////////// Save and load

void App3D::updateStylise(){
  ShaderWood->bind();
  glUniform1fv(glGetUniformLocation(ShaderWood->ProgramId, "stylised"), 1, &stylisedOn);
  ShaderWood->unbind();

  ShaderNoWood->bind();
  glUniform1fv(glGetUniformLocation(ShaderNoWood->ProgramId, "stylised"), 1, &stylisedOn);
  ShaderNoWood->unbind();
}

void App3D::save() {
  // Save the state of the application
  std::ofstream file("../../Save/save.txt");
  if( file.is_open() ) {
    file << currentCamera->cameraEye.x << " " << currentCamera->cameraEye.y << " " << currentCamera->cameraEye.z << std::endl;
    file << currentCamera->cameraCenter.x << " " << currentCamera->cameraCenter.y << " " << currentCamera->cameraCenter.z << std::endl;
    file << currentCamera->cameraUp.x << " " << currentCamera->cameraUp.y << " " << currentCamera->cameraUp.z << std::endl;
    file << currentCamera->yaw << std::endl;
    file << currentCamera->pitch << std::endl;
    file << currentCamera->radius << std::endl;
    file << stylisedOn << std::endl;
    file << lineOn << std::endl;

    file.close();
    std::cout << "Saved sucessfuly!" << std::endl;
  }
  else {
    std::cout << "Unable to open file" << std::endl;
  }
}

void App3D::load() {
  // Load the state of the application
  std::ifstream file("../../Save/save.txt");
  if( file.is_open() ) {
    file >> currentCamera->cameraEye.x >> currentCamera->cameraEye.y >> currentCamera->cameraEye.z;
    file >> currentCamera->cameraCenter.x >> currentCamera->cameraCenter.y >> currentCamera->cameraCenter.z;
    file >> currentCamera->cameraUp.x >> currentCamera->cameraUp.y >> currentCamera->cameraUp.z;
    file >> currentCamera->yaw;
    file >> currentCamera->pitch;
    file >> currentCamera->radius;
    file >> stylisedOn;
    file >> lineOn;

    Scene->setCameraView(glm::lookAt(currentCamera->cameraEye, currentCamera->cameraCenter, currentCamera->cameraUp));

    file.close();
    std::cout << "Loaded sucessfuly!" << std::endl;
  }
  else {
    std::cout << "Unable to open file" << std::endl;
  }
}

///////////////////////////////////////////////////////////////////////// MESHES

void App3D::createMeshes() {
  std::string mesh_dir = "../../assets/models/";

  std::vector<std::pair<std::string, int>> objects = {
    {mesh_dir + "SphereUVNT_Smoth.obj", 2}
  };

  for(std::pair<std::string, int> obj : objects){
    mgl::Mesh* mesh = new mgl::Mesh();
    mesh->joinIdenticalVertices();
    mesh->create(obj.first);
    Meshes.push_back(std::make_pair(mesh, obj.second));
  }

  glGenVertexArrays(1, &quadVAO);
  glGenBuffers(1, &quadVBO);
  glBindVertexArray(quadVAO);
  glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

}

///////////////////////////////////////////////////////////////////////// SCENE

void App3D::createSceneGraph(){
  Scene = new mgl::SceneGraph();

  Scene->createCamera(UBO_BP);
  Scene->setCamera(glm::lookAt(currentCamera->cameraEye, currentCamera->cameraCenter, currentCamera->cameraUp), currentCamera->projectionMatrix);
}
  
void App3D::populateSceneGraph(){
  mgl::ShaderProgram* Shaders[] = {ShaderWood, ShaderNoWood};
  for (std::pair<mgl::Mesh*, int> mesh : Meshes){
    for(int j = 0; j < mesh.second; j++)
      Scene->getRoot()->createSon(mesh.first, Shaders[j], 
                                  new glm::vec3(0.392f, 0.32f, 0.185f), 
                                  glm::translate( glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, (-1.5f)*pow(-1, j))) );
  } 
}

///////////////////////////////////////////////////////////////////////// SHADER

void App3D::createShaderPrograms() {
  // shader wood
  ShaderWood = new mgl::ShaderProgram();
  ShaderWood->addShader(GL_VERTEX_SHADER, "../../shaders/light-model-wood-vs.glsl");
  ShaderWood->addShader(GL_FRAGMENT_SHADER, "../../shaders/light-model-wood-fs.glsl");

  ShaderWood->addAttribute(mgl::POSITION_ATTRIBUTE, mgl::Mesh::POSITION);

  ShaderWood->addAttribute(mgl::NORMAL_ATTRIBUTE, mgl::Mesh::NORMAL);

  ShaderWood->addUniform(mgl::MODEL_MATRIX);
  ShaderWood->addUniformBlock(mgl::CAMERA_BLOCK, UBO_BP);
  ShaderWood->addUniform(mgl::COLOR_ATTRIBUTE);

  ShaderWood->addUniform("stylised");

  ShaderWood->create();

  // shader no wood
  ShaderNoWood = new mgl::ShaderProgram();
  ShaderNoWood->addShader(GL_VERTEX_SHADER, "../../shaders/light-model-vs.glsl");
  ShaderNoWood->addShader(GL_FRAGMENT_SHADER, "../../shaders/light-model-fs.glsl");

  ShaderNoWood->addAttribute(mgl::POSITION_ATTRIBUTE, mgl::Mesh::POSITION);

  ShaderNoWood->addAttribute(mgl::NORMAL_ATTRIBUTE, mgl::Mesh::NORMAL);

  ShaderNoWood->addUniform(mgl::MODEL_MATRIX);
  ShaderNoWood->addUniformBlock(mgl::CAMERA_BLOCK, UBO_BP);
  ShaderNoWood->addUniform(mgl::COLOR_ATTRIBUTE);

  ShaderNoWood->addUniform("stylised");

  ShaderNoWood->create();

  //set both
  updateStylise();

  // ScreenShaders
  ScreenShaders = new mgl::ShaderProgram();
  ScreenShaders->addShader(GL_VERTEX_SHADER, "../../shaders/screen-shader-vs.glsl");
  ScreenShaders->addShader(GL_FRAGMENT_SHADER, "../../shaders/screen-shader-fs.glsl");

  ScreenShaders->addUniform(mgl::NEAR_UNIFORM);
  ScreenShaders->addUniform(mgl::FAR_UNIFORM);
  ScreenShaders->addUniform("lineOn");

  ScreenShaders->create();

  // Set near and far plane for screen shader
  ScreenShaders->bind();
  glUniform1fv(glGetUniformLocation(ScreenShaders->ProgramId, "near"), 1, &currentCamera->near);
  glUniform1fv(glGetUniformLocation(ScreenShaders->ProgramId, "far"), 1, &currentCamera->far);
  glUniform1fv(glGetUniformLocation(ScreenShaders->ProgramId, "lineOn"), 1, &lineOn);  
  ScreenShaders->unbind();
}

/////////////////////////////////////////////////////////////////////////// DRAW

void App3D::createFrameBuffer(){
  // Create framebuffer
  glGenFramebuffers(1, &fbo);
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);

  // Create color texture
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, windowLength, windowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

  // Create depth texture
  glGenTextures(1, &depthTexture);
  glBindTexture(GL_TEXTURE_2D, depthTexture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, windowLength, windowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthTexture, 0);

  // Check framebuffer status
  if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
  
  glBindFramebuffer(GL_FRAMEBUFFER, 0); // Unbind framebuffer
}

void App3D::drawScene() {
  Scene->draw();
}

////////////////////////////////////////////////////////////////////// CALLBACKS

void App3D::initCallback(GLFWwindow *win) {
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  createSceneGraph();

  load();

  createMeshes();

  createFrameBuffer();

  createShaderPrograms();  // after mesh;

  populateSceneGraph();

}

void App3D::displayCallback(GLFWwindow *win, double elapsed) {  
  // first pass
  glBindFramebuffer(GL_FRAMEBUFFER, fbo);
  glEnable(GL_DEPTH_TEST);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(0.1f, 0.1f, 0.3f, 1.0f);
  
  drawScene();  

  // second pass
  glBindFramebuffer(GL_FRAMEBUFFER, 0); //back to default
  glDisable(GL_DEPTH_TEST);

  glClearColor(0.1f, 0.1f, 0.3f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
 
  ScreenShaders->bind();
  glBindVertexArray(quadVAO);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);

  GLint colorTextureLocation = glGetUniformLocation(ScreenShaders->ProgramId, "colorTexture");
  glUniform1i(colorTextureLocation, 0);

  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, depthTexture);

  GLint textureLocation = glGetUniformLocation(ScreenShaders->ProgramId, "depthTexture");
  glUniform1i(textureLocation, 1);

  glDrawArrays(GL_TRIANGLES, 0, 6);
  ScreenShaders->unbind();
  
  }

void App3D::windowSizeCallback(GLFWwindow *win, int winx, int winy) {
  glViewport(0, 0, winx, winy);

  windowLength = winx;
  windowHeight = winy;


  glDeleteRenderbuffers(1, &rbo);
  glDeleteTextures(1, &texture);
  glDeleteFramebuffers(1, &fbo);
  createFrameBuffer();

  defaultCameraSettings->projectionMatrix = glm::perspective(glm::radians(30.0f), (float)winx / winy, defaultCameraSettings->near, defaultCameraSettings->far);
  Scene->setCameraProjection(glm::perspective(glm::radians(30.0f), (float)winx / winy, currentCamera->near, currentCamera->far));
  
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////  Camera Stuff
void App3D::mouseButtonCallback(GLFWwindow *win, int button, int action, int mods) {
  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
    firstMouse = true;
    mouseIsHeld = true;
  }
  else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    mouseIsHeld = false;
}

void App3D::scrollCallback(GLFWwindow *window, double xoffset, double yoffset) {
  currentCamera->radius -= yoffset;

  currentCamera->radius = glm::clamp(currentCamera->radius , currentCamera->near, currentCamera->far);

  // Create quaternions for yaw and pitch
  glm::quat qPitch = glm::angleAxis(glm::radians(currentCamera->pitch), glm::vec3(1.0f, 0.0f, 0.0f));
  glm::quat qYaw = glm::angleAxis(glm::radians(currentCamera->yaw), glm::vec3(0.0f, 1.0f, 0.0f));

  // Combine pitch and yaw rotations
  glm::quat orientation = qYaw * qPitch;
  orientation = glm::normalize(orientation);

  // Update the camera position using the orientation quaternion
  glm::vec3 forward = orientation * glm::vec3(0.0f, 0.0f, -1.0f);
  currentCamera->cameraEye = currentCamera->cameraCenter + currentCamera->radius * forward;

  currentCamera->cameraUp = orientation * glm::vec3(0.0f, 1.0f, 0.0f);

  // Set the view matrix
  Scene->setCameraView(glm::lookAt(currentCamera->cameraEye, currentCamera->cameraCenter, currentCamera->cameraUp));
}

void App3D::cursorCallback(GLFWwindow *win, double xpos, double ypos) {
  if (firstMouse)
  {
      lastX = xpos;
      lastY = ypos;
      firstMouse = false;
  }

  if (mouseIsHeld) {
    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    float sensitivity = 0.5f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    currentCamera->yaw   -= xoffset;
    currentCamera->pitch -= yoffset;

    // Create quaternions for yaw and pitch
    glm::quat qPitch = glm::angleAxis(glm::radians(currentCamera->pitch), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::quat qYaw = glm::angleAxis(glm::radians(currentCamera->yaw), glm::vec3(0.0f, 1.0f, 0.0f));

    // Combine pitch and yaw rotations
    glm::quat orientation = qYaw * qPitch;
    orientation = glm::normalize(orientation);

    // Update the camera position using the orientation quaternion
    glm::vec3 forward = orientation * glm::vec3(0.0f, 0.0f, -1.0f);
    currentCamera->cameraEye = currentCamera->cameraCenter + currentCamera->radius * forward;

    currentCamera->cameraUp = orientation * glm::vec3(0.0f, 1.0f, 0.0f);
    
    // Set the view matrix
    Scene->setCameraView(glm::lookAt(currentCamera->cameraEye, currentCamera->cameraCenter, currentCamera->cameraUp));
  }

}

void App3D::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
  if(key == GLFW_KEY_L && action == GLFW_PRESS){
    if (lineOn == 1.0f)
      lineOn = 0.0f;
    else
      lineOn = 1.0f;
    ScreenShaders->bind();
    glUniform1fv(glGetUniformLocation(ScreenShaders->ProgramId, "lineOn"), 1, &lineOn);
    ScreenShaders->unbind();
  }
  else if(key == GLFW_KEY_S && action == GLFW_PRESS){
    if (stylisedOn == 1.0f){
      stylisedOn = 0.0f;
      lineOn = 0.0f;
    }
    else{
      stylisedOn = 1.0f;
      lineOn = 1.0f;
    }

  
    ScreenShaders->bind();
    glUniform1fv(glGetUniformLocation(ScreenShaders->ProgramId, "lineOn"), 1, &lineOn);
    ScreenShaders->unbind();

    updateStylise();
  }
  else if(key == GLFW_KEY_R && action == GLFW_PRESS){
    stylisedOn = 0.0f;
    lineOn = 0.0f;

    ScreenShaders->bind();
    glUniform1fv(glGetUniformLocation(ScreenShaders->ProgramId, "lineOn"), 1, &lineOn);
    ScreenShaders->unbind();

    updateStylise();

    currentCamera->cameraEye = defaultCameraSettings->cameraEye;
    currentCamera->cameraCenter = defaultCameraSettings->cameraCenter;
    currentCamera->cameraUp = defaultCameraSettings->cameraUp;
    currentCamera->yaw = defaultCameraSettings->yaw;
    currentCamera->pitch = defaultCameraSettings->pitch;
    currentCamera->radius = defaultCameraSettings->radius;
    Scene->setCameraView(glm::lookAt(defaultCameraSettings->cameraEye, defaultCameraSettings->cameraCenter, defaultCameraSettings->cameraUp));
  }
}

void App3D::windowCloseCallback(GLFWwindow *win) {
  std::cout << "Window is closing!" << std::endl;
  save();
}
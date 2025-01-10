#include "app.hpp"

#include <iostream>


///////////////////////////////////////////////////////////////////////// MESHES

void App3D::createMeshes() {
  std::string mesh_dir = "../../assets/models/";

  std::vector<std::pair<std::string, int>> objects = {
    {mesh_dir + "SphereUVNT_Smoth.obj", 1}
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

  int i = 0;
  for (std::pair<mgl::Mesh*, int> mesh : Meshes){
    for(int j = 0; j < mesh.second; j++, i++)
      Scene->getRoot()->createSon(mesh.first, Shaders, new glm::vec3(0.392f, 0.32f, 0.185f /*1.0f, 0.0f, 0.0f*/), glm::mat4(1.0f));
  } 
}

///////////////////////////////////////////////////////////////////////// SHADER

void App3D::createShaderPrograms() {
  Shaders = new mgl::ShaderProgram();
  Shaders->addShader(GL_VERTEX_SHADER, "../../shaders/blinn-phong-vs.glsl");
  Shaders->addShader(GL_FRAGMENT_SHADER, "../../shaders/blinn-phong-fs.glsl");

  Shaders->addAttribute(mgl::POSITION_ATTRIBUTE, mgl::Mesh::POSITION);

  Shaders->addAttribute(mgl::NORMAL_ATTRIBUTE, mgl::Mesh::NORMAL);

  //Shaders->addAttribute(mgl::TEXCOORD_ATTRIBUTE, mgl::Mesh::TEXCOORD);

  //Shaders->addAttribute(mgl::TANGENT_ATTRIBUTE, mgl::Mesh::TANGENT);

  Shaders->addUniform(mgl::MODEL_MATRIX);
  Shaders->addUniformBlock(mgl::CAMERA_BLOCK, UBO_BP);
  Shaders->addUniform(mgl::COLOR_ATTRIBUTE);

  Shaders->create();

  // ScreenShaders
  ScreenShaders = new mgl::ShaderProgram();
  ScreenShaders->addShader(GL_VERTEX_SHADER, "../../shaders/screen-shader-vs.glsl");
  ScreenShaders->addShader(GL_FRAGMENT_SHADER, "../../shaders/screen-shader-fs.glsl");

  Shaders->addAttribute(mgl::POSITION_ATTRIBUTE, mgl::Mesh::POSITION);

  Shaders->addAttribute(mgl::TEXCOORD_ATTRIBUTE, mgl::Mesh::TEXCOORD);

  Shaders->addUniform("near");
  Shaders->addUniform("far");

  ScreenShaders->create();

  ScreenShaders->bind();
  glUniform1fv(glGetUniformLocation(ScreenShaders->ProgramId, "near"), 1, &currentCamera->near);
  glUniform1fv(glGetUniformLocation(ScreenShaders->ProgramId, "far"), 1, &currentCamera->far);
  ScreenShaders->unbind();

  // ParticleShaders
  // ParticleShaders = new mgl::ShaderProgram();
  // ParticleShaders->addShader(GL_VERTEX_SHADER, "../../shaders/particle-vs.glsl");
  // ParticleShaders->addShader(GL_FRAGMENT_SHADER, "../../shaders/particle-fs.glsl");
  // ParticleShaders->addShader(GL_GEOMETRY_SHADER, "../../shaders/particle-gs.glsl");

  // // ParticleShaders->addAttribute(mgl::POSITION_ATTRIBUTE, mgl::Mesh::POSITION);

  // // ParticleShaders->addUniform("particleSize");

  // ParticleShaders->addUniformBlock(mgl::CAMERA_BLOCK, UBO_BP);

  // ParticleShaders->create();

}

/////////////////////////////////////////////////////////////////////////// DRAW
//particles
// float points[] = {
//   -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // top-left
//    0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // top-right
//    0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // bottom-right
//   -0.5f, -0.5f, 1.0f, 1.0f, 0.0f  // bottom-left
// };

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

  // Create depth texture (GL_DEPTH_COMPONENT is often used for depth textures)
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


  // for particle test
  // glGenBuffers(1, &VBO);
  // glGenVertexArrays(1, &VAO);
  // glBindVertexArray(VAO);
  // glBindBuffer(GL_ARRAY_BUFFER, VBO);
  // glBufferData(GL_ARRAY_BUFFER, sizeof(points), &points, GL_STATIC_DRAW);
  // glEnableVertexAttribArray(0);
  // glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
  // glEnableVertexAttribArray(1);
  // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
  // glBindVertexArray(0);

}

void App3D::drawScene() {
  Scene->draw();
}

////////////////////////////////////////////////////////////////////// CALLBACKS

void App3D::initCallback(GLFWwindow *win) {
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  createMeshes();

  createSceneGraph();

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

  //particles

  // ParticleShaders->bind();
  // glBindVertexArray(VAO);
  // // glUniform1f(ParticleShaders->Uniforms["particleSize"].index, 1.0f);
  // glDrawArrays(GL_POINTS, 0, 4);
  // ParticleShaders->unbind();
  
  ////////////////////////////////////

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
  Scene->setCameraProjection(glm::perspective(glm::radians(30.0f), (float)winx / winy, 1.0f, 20.0f));
  
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
  if(key == GLFW_KEY_P && action == GLFW_PRESS){} //does nothing
}

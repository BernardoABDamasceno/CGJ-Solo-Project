#ifndef APP3D_HPP
#define APP3D_HPP

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include <mgl/mgl.hpp>

////////////////////////////////////////////////////////////////////////// MYAPP

class App3D : public mgl::App {
 public:
  void initCallback(GLFWwindow *win) override;
  void displayCallback(GLFWwindow *win, double elapsed) override;
  void windowSizeCallback(GLFWwindow *win, int width, int height) override;
  void mouseButtonCallback(GLFWwindow *win, int button, int action, int mods) override;
  void cursorCallback(GLFWwindow *win, double xpos, double ypos) override;
  void scrollCallback(GLFWwindow *win, double xoffset, double yoffset) override;
  void keyCallback(GLFWwindow *window, int key, int scancode,int action, int mods) override;
  void windowCloseCallback(GLFWwindow *win) override;

 private:
  const GLuint UBO_BP = 0;

  // Framebuffer
  unsigned int fbo;
  unsigned int texture;
  unsigned int depthTexture;
  unsigned int rbo;

  // Screen mesh
  unsigned int quadVBO, quadVAO;

   float lineOn = 0.0f; 
   float stylisedOn = 0.0f;

  mgl::ShaderProgram *Shaders = nullptr;
  mgl::ShaderProgram *ScreenShaders = nullptr;
  mgl::ShaderProgram *ParticleShaders = nullptr;
  mgl::SceneGraph *Scene = nullptr;
  std::vector<std::pair<mgl::Mesh*, int>> Meshes = {};

  float quadVertices[34] = {
    // positions        // texture Coords
    -1.0f,  1.0f, 0.0f, 1.0f,
    -1.0f, -1.0f, 0.0f, 0.0f,
     1.0f, -1.0f, 1.0f, 0.0f,

    -1.0f,  1.0f, 0.0f, 1.0f,
     1.0f, -1.0f, 1.0f, 0.0f,
     1.0f,  1.0f, 1.0f, 1.0f
  };

  // For camera
  float lastX, lastY;

  struct CameraProperties {
    glm::vec3 cameraEye = glm::vec3(10.0f, 0.0f, 0.0f);
    glm::vec3 cameraCenter = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    float yaw = -90.0f;
    float pitch = 0.0f;
    float radius = 10.0f;
    float near = 1.0f;
    float far = 100.0f;
    glm::mat4 projectionMatrix = glm::perspective(glm::radians(30.0f), 640.0f / 480.0f, near, far);
  };

  CameraProperties *defaultCameraSettings = new CameraProperties();
  CameraProperties *currentCamera = new CameraProperties();
  
  bool firstMouse = false;
  bool mouseIsHeld = false;

  float windowLength = 640.0f;
  float windowHeight = 480.0f;

  void createFrameBuffer();
  void createMeshes();
  void createShaderPrograms();
  void createCamera();
  void createSceneGraph();
  void populateSceneGraph();
  void setTangram();
  void drawScene();

  void save();
  void load();
};

#endif /* APP3D_HPP */
#include "app/app.hpp"

int main(int argc, char *argv[]) {
  mgl::Engine &engine = mgl::Engine::getInstance();
  engine.setApp(new App3D());
  engine.setOpenGL(4, 6);
  engine.setWindow(640.0f, 480.0f, "Solo Project", 0, 1);
  engine.init();
  engine.run();
  exit(EXIT_SUCCESS);
}


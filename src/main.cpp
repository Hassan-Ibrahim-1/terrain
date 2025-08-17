#include "app.hpp"
#include "engine.hpp"

int main() {
  EngineInitInfo cinfo;
#ifdef __APPLE__
  cinfo.window_width = 1280;
  cinfo.window_height = 720;
#else
  cinfo.window_width = 3840;
  cinfo.window_height = 2160;
#endif
  cinfo.window_title = "App";
  cinfo.clear_color = Color(glm::vec3(0.1, 0.1, 0.1));
  cinfo.cursor_enabled = false;

  // Call init before creating an Application instance
  engine::init(cinfo);
  App app;
  engine::run(app);

  return 0;
}

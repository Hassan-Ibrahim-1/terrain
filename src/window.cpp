// TODO: fix formatting in this file

#include "debug.hpp"
#include <glad/glad.h>

#include "window.hpp"

static void default_framebuffer_size_callback(GLFWwindow *window, int width,
                                              int height);

Window::Window(uint width, uint height, const std::string &title,
               bool enable_msaa)
    : _width(width), _height(height), _title(title) {
  _window = create_window();
  glfwMakeContextCurrent(_window);
  load_opengl_functions();
  // Sets a default basic frame buffer size callback
  glfwSetFramebufferSizeCallback(_window, default_framebuffer_size_callback);
  if (enable_msaa) {
    glfwWindowHint(GLFW_SAMPLES, 4);
    glEnable(GL_MULTISAMPLE);
    LOG("MSAA enabled");
  }
#ifdef __linux__
  glViewport(0, 0, _width, _height);
#endif
  _loaded = true;
}

Window::~Window() {
  if (_loaded) {
    glfwDestroyWindow(_window);
  }
}

uint Window::width() const { return _width; }

uint Window::height() const { return _height; }

GLFWwindow *Window::data() { return _window; }

bool Window::in_focus() const {
  return glfwGetWindowAttrib(_window, GLFW_ICONIFIED) == 0;
}

bool Window::should_close() const { return glfwWindowShouldClose(_window); }

void Window::enable_cursor() {
  glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void Window::disable_cursor() {
  glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

GLFWwindow *Window::create_window() {
  GLFWwindow *window = nullptr;
#ifdef __linux__
  auto *monitor = glfwGetPrimaryMonitor();
  float xscale = 0.0;
  float yscale = 0.0;
  glfwGetMonitorContentScale(monitor, &xscale, &yscale);
  window = glfwCreateWindow(static_cast<float>(_width) * xscale,
                            static_cast<float>(_height) * yscale,
                            _title.c_str(), monitor, NULL);
#else
  window = glfwCreateWindow(_width, _height, _title.c_str(), NULL, NULL);
#endif

  ASSERT(window != nullptr, "Bad window intialization");
  return window;
}

void Window::load_opengl_functions() {
  ASSERT(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress),
         "Bad glad initialization");
}

static void default_framebuffer_size_callback(GLFWwindow *window, int width,
                                              int height) {
  glViewport(0, 0, width, height);
}

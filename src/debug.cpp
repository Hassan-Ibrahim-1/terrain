#include "debug.hpp"
#include <cstdlib>
#include <glad/glad.h>

void debug::print_stack_trace() {
#ifdef __APPLE__
  constexpr int kBacktraceDepth = 15;
  void *backtrace_addrs[kBacktraceDepth];

  int trace_size = backtrace(backtrace_addrs, kBacktraceDepth);

  for (int i = 0; i < trace_size; ++i) {
    Dl_info info;
    dladdr(backtrace_addrs[i], &info);

    std::stringstream cmd(std::ios_base::out);
    cmd << "atos -o " << info.dli_fname << " -l " << std::hex
        << reinterpret_cast<uint64_t>(info.dli_fbase) << ' '
        << reinterpret_cast<uint64_t>(backtrace_addrs[i]);

    FILE *atos = popen(cmd.str().c_str(), "r");

    constexpr int kBufferSize = 200;
    char buffer[kBufferSize];

    fgets(buffer, kBufferSize, atos);
    pclose(atos);

    std::cout << buffer;
  }
  std::cout << std::flush;
#endif
}

void debug::check_gl_error(const char *file, int line) {
  GLenum errorCode;
  bool had_error = false;
  while ((errorCode = glGetError()) != GL_NO_ERROR) {
    std::string error;
    switch (errorCode) {
    case GL_INVALID_ENUM:
      error = "INVALID_ENUM";
      break;
    case GL_INVALID_VALUE:
      error = "INVALID_VALUE";
      break;
    case GL_INVALID_OPERATION:
      error = "INVALID_OPERATION";
      break;
    case GL_STACK_OVERFLOW:
      error = "STACK_OVERFLOW";
      break;
    case GL_STACK_UNDERFLOW:
      error = "STACK_UNDERFLOW";
      break;
    case GL_OUT_OF_MEMORY:
      error = "OUT_OF_MEMORY";
      break;
    case GL_INVALID_FRAMEBUFFER_OPERATION:
      error = "INVALID_FRAMEBUFFER_OPERATION";
      break;
    }
    LOG_ERROR("GL_ERROR: %s | %s (%d)", error.c_str(), file, line);
    std::cout << error << " | " << file << " (" << line << ")";
    had_error = true;
  }
  if (had_error) {
    std::abort();
  }
  /*LOG("No gl error");*/
}

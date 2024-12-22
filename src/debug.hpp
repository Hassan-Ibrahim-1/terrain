#pragma once

#include <iostream>
#include <execinfo.h>
#include <cstdlib>
#include <cxxabi.h>
#include <dlfcn.h>
#include <sstream>

namespace debug {

void print_stack_trace();
void check_gl_error(const char* file, int line);

}

#define DEBUG_LOG
#define DEBUG_ASSERT
#define DEBUG_ERROR
#define DEBUG_OPENGL

#ifdef DEBUG_LOG
// printf("\033[33;44myellow on blue\n");

#define DEBUG_YELLOW "\033[33;33m"
#define DEBUG_RESET "\033[0m"

#define LOG(msg, ...) \
    do { \
        printf("%s[%s]%s: ", DEBUG_YELLOW, __PRETTY_FUNCTION__, DEBUG_RESET); \
        printf(msg, ##__VA_ARGS__); \
        printf("\n"); \
    } while (0)
#define LOG_ERROR(msg, ...) \
    do { \
        fprintf(stderr, "%s[ERROR -> %s]%s: ", DEBUG_RED, __PRETTY_FUNCTION__, DEBUG_RESET); \
        fprintf(stderr, msg, ##__VA_ARGS__); \
        fprintf(stderr, "\n"); \
        debug::print_stack_trace(); \
    } while (0)


#else
#undef LOG
#undef LOG_ERROR
#define LOG(msg, ...) 
#define LOG_ERROR(msg, ...)  
#endif // DEBUG_LOG

#ifdef DEBUG_ASSERT
#define DEBUG_RED     "\033[1;31m"
#define DEBUG_RESET   "\033[0m"

#define ASSERT(condition, msg, ...) \
    do { \
        if (!(condition)) { \
            fprintf(stderr, "%sAssertion failed in %s:%s %s | ", DEBUG_RED, __PRETTY_FUNCTION__, DEBUG_RESET, #condition); \
            fprintf(stderr, msg, ##__VA_ARGS__); \
            fprintf(stderr, "\n"); \
            debug::print_stack_trace(); \
            std::abort(); \
        } \
    } while (0)

/*std::cerr << DEBUG_RED << "Assertion failed in " << __PRETTY_FUNCTION__ << ": " \*/
/*<< DEBUG_RESET << #condition << ", " << msg << "\n"; \*/
/*std::abort(); \*/
#else
#define ASSERT(condition, msg, ...) 
#endif // DEBUG_ASSERT

#ifdef DEBUG_ERROR
#define DEBUG_RED     "\033[1;31m"
#define DEBUG_RESET   "\033[0m"

#define ERROR(msg, ...) \
    do { \
        fprintf(stderr, "%s[%s]%s: ", DEBUG_RED, __PRETTY_FUNCTION__, DEBUG_RESET); \
        fprintf(stderr, msg, ##__VA_ARGS__); \
        fprintf(stderr, "\n"); \
        debug::print_stack_trace(); \
        std::abort(); \
    } while (0)

#else
#define ERROR(msg, ...)  
#endif // DEBUG_ERROR

#ifdef DEBUG_OPENGL
#define CHECK_GL_ERROR() \
    do { \
        debug::check_gl_error(__FILE__, __LINE__); \
    } while (0)
#else
#undef CHECK_GL_ERROR 
#define CHECK_GL_ERROR  
#endif // DEBUG_OPENGL


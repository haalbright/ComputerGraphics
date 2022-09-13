////////////////////////////////////////////////////////////////////////////////
/// @file
/// @brief Contains main function to create a window and run engine that
///        repeatedly generates a framebuffer and displays it.
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Includes

// STL
#include <chrono>
#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <thread>
#include <fstream>
#include <typeinfo>

#include <sstream>
#include "Scene.h"
#include "Sphere.h"
#include "Object.h"
#include "Plane.h"
#include "Camera.h"
#include "Renderer.h"
#include "RayTracer.h"
// Engine
#include "GLInclude.h"

////////////////////////////////////////////////////////////////////////////////
// Global variables - avoid these

// Window

RayTracer g_rTracer;
int g_width=1360;
int g_height=768;
Scene g_scene;
// Framebuffer
std::unique_ptr<glm::vec4[]> g_frame{nullptr}; ///< Framebuffer

// Frame rate
const unsigned int FPS = 60;
float g_frameRate{0.f};
std::chrono::high_resolution_clock::time_point g_frameTime{
  std::chrono::high_resolution_clock::now()};
float g_delay{0.f};
float g_framesPerSecond{0.f};

////////////////////////////////////////////////////////////////////////////////
// Functions

////////////////////////////////////////////////////////////////////////////////
/// @brief Initialize GL settings
void
initialize(GLFWwindow* _window) {
  glClearColor(1.f, 1.f, 1.f, 1.f);

  g_frame = std::make_unique<glm::vec4[]>(g_width*g_height);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Callback for resize of window
///
/// Responsible for setting window size (viewport) and projection matrix.
void resize(GLFWwindow* window, int _w, int _h) {
  g_width = _w;
  g_height = _h;
  g_rTracer.resize(g_width,g_height);
  // Viewport
  glfwGetFramebufferSize(window, &g_width, &g_height);
  glViewport(0, 0, g_width, g_height);
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Draw function for single frame
void
draw(GLFWwindow* _window, double _currentTime) {
  //////////////////////////////////////////////////////////////////////////////
  // Clear
  glClearColor(1.0, 0.0, 0.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);
  g_rTracer.resize(g_width,g_height);
  g_rTracer.render(g_scene);
  std::unique_ptr<glm::vec4[]>& new_frame=g_rTracer.getFrame();
  std::cout<<new_frame.get()[5].x<<std::endl;
  for(int i = 0; i < g_width*g_height; ++i)
    g_frame[i] = glm::vec4(0.f, 0.4f, 0.f, 0.f);

  //////////////////////////////////////////////////////////////////////////////
  // Draw
  // if(g_width!=1360||g_height!=768){
    std::cout<<"Width: "<<g_width<<std::endl;
    std::cout<<"height: "<<g_height<<std::endl;
  // }
  // Simple static :P
  for(int i = 0; i < g_width*g_height; ++i){
    //g_frame[i] = glm::vec4(float(rand())/RAND_MAX, float(rand())/RAND_MAX,
                           //float(rand())/RAND_MAX, 1.f);
    g_frame[i]=glm::vec4(new_frame.get()[i].x,new_frame.get()[i].y,new_frame.get()[i].z, new_frame.get()[i].a);
    //std::cout<<i<<std::endl;
  }
  //g_frame=new_frame;
///////////
std::cout<<"before draw"<<std::endl;
  glm::vec4 name=new_frame.get()[g_width*g_height];
  //std::cout<<new_frame.get()[0]<<std::endl;
  std::cout<<"-----"<<std::endl;
  glDrawPixels(g_width, g_height, GL_RGBA, GL_FLOAT, g_frame.get());
  //std::cout<<"hanna is right"<<std::endl;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Main application loop
void
run(GLFWwindow* _window) {
  using namespace std::chrono;

  std::cout << "Starting main loop" << std::endl;

  while(!glfwWindowShouldClose(_window)) {
    draw(_window, glfwGetTime());
    ////////////////////////////////////////////////////////////////////////////
    // Show
    glfwSwapBuffers(_window);
    glfwPollEvents();

    ////////////////////////////////////////////////////////////////////////////
    // Record frame time
    high_resolution_clock::time_point time = high_resolution_clock::now();
    g_frameRate = duration_cast<duration<float>>(time - g_frameTime).count();
    g_frameTime = time;
    g_framesPerSecond = 1.f/(g_delay + g_frameRate);
    printf("FPS: %6.2f\n", g_framesPerSecond);

    ////////////////////////////////////////////////////////////////////////////
    // Delay to fix the frame-rate
    g_delay = std::max(0.f, 1.f/FPS - g_frameRate);
    std::this_thread::sleep_for(std::chrono::microseconds(
        static_cast<long int>(g_delay*1000)));
  }

  std::cout << "Ending main loop" << std::endl;
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Callback function for keyboard input
/// @param _window
/// @param _key Key
/// @param _scancode Platform specific keycode
/// @param _action Action, e.g., pressed or released
/// @param _mods Keyboard modifiers
void
keyCallback(GLFWwindow* _window, int _key, int _scancode,
           int _action, int _mods) {
  if(_action == GLFW_PRESS) {
    switch(_key) {
      // Escape key : quit application
      case GLFW_KEY_ESCAPE:
        std::cout << "Closing window" << std::endl;
        glfwSetWindowShouldClose(_window, GLFW_TRUE);
        break;
        // Arrow keys
      case GLFW_KEY_LEFT:
      case GLFW_KEY_RIGHT:
        break;
        // Unhandled
      default:
        std::cout << "Unhandled key: " << _key << std::endl;
        break;
    }
  }
}

////////////////////////////////////////////////////////////////////////////////
/// @brief Error handler
/// @param _code Error code
/// @param _msg Error message
void errorCallback(int _code, const char* _msg) {
  std::cerr << "Error " << _code << ": " << _msg << std::endl;
}

////////////////////////////////////////////////////////////////////////////////
// Main

////////////////////////////////////////////////////////////////////////////////
/// @brief main
/// @param _argc Count of command line arguments
/// @param _argv Command line arguments
/// @return Application success status
int
main(int _argc, char** _argv) {
  // std::string filename="example.txt";
  // Scene example;
  //
  // example.readFromFile(filename);
  // example.printFunc();//print all objects and lights in scene
  //////////////////////////////////////////////////////////////////////////////
  // Initialize
  // g_width=1360;
  // g_height=768;
  g_rTracer.resize(g_width,g_height);
  std::string filename="snowman.txt";
  //std::string filename="example.txt";
  g_scene.readFromFile(filename);

  {
    glm::vec3 origin(50,50,0);
    glm::vec3 direction(5,5,5);
    glm::vec3 pPoint(50,50,50);
    glm::vec3 p1Normal(1,2,-3);
    glm::vec3 p2Normal(1,2,3);
    glm::vec3 p3Normal(3,2,-1);
    Ray ex(origin,direction);
    glm::vec4 kD(0.2, 0.2, 0.2, 0.3);
    glm::vec4 kA(0.2, 0.2, 0.2, 0.3);
    glm::vec4 kS(0.2, 0.2, 0.2, 0.3);
    float p=.1;
    Material m;
    m.kD=kD;
    m.kS=kS;
    m.kA=kA;
    m.rho=p;
    glm::vec3 s1C(50,50,10);
    Sphere s1(s1C,5,m);
    Sphere s2(s1C,9,m);
    Sphere s3(s1C,13,m);
    s1.collide(ex);
    s2.collide(ex);
    s3.collide(ex);
    Plane p1(pPoint,p1Normal,m);
    Plane p2(pPoint,p2Normal,m);
    Plane p3(pPoint,p3Normal,m);
    p1.collide(ex);
    p2.collide(ex);
    p3.collide(ex);
    std::cout<<"done"<<std::endl;
  }
  std::cout<<"Before Raytracer"<<std::endl;
  std::cout<<"started Raytracer"<<std::endl;
  //return 0;

  std::cout<<"rendering...."<<std::endl;

  //std::unique_ptr<glm::vec4[]>&  _frame=rTracer.getFrame();
  std::cout << "Initializing GLFWWindow" << std::endl;
  // GLFW

  glfwSetErrorCallback(errorCallback);
  if(!glfwInit()) {
    std::cerr << "GLFW Cannot initialize" << std::endl;
    return 1;
  }

  GLFWwindow* window = glfwCreateWindow(
    g_width, g_height, "Spiderling: A Rudamentary Game Engine", NULL, NULL);
  if(!window) {
    std::cerr << "GLFW Cannot create window" << std::endl;
    return 1;
  }

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  glfwGetFramebufferSize(window, &g_width, &g_height);
  glViewport(0, 0, g_width, g_height);  // Initialize viewport


  // Assign callback functions
  std::cout << "Assigning Callback functions" << std::endl;
  glfwSetFramebufferSizeCallback(window, resize);
  glfwSetKeyCallback(window, keyCallback);

  // Program initialize
  std::cout << "Initializing application" << std::endl;
  //glClearColor(0.f, 0.f, 0.f, 1.f);
  initialize(window);
  //  return 0;
  //////////////////////////////////////////////////////////////////////////////
  // Main loop
  run(window);

  //////////////////////////////////////////////////////////////////////////////
  // Cleanup GLFW Window
  std::cout << "Destroying GLFWWindow" << std::endl;
  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}

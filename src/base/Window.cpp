
#include "base/Window.h"

#include <GL/glew.h>
#include <GL/glfw.h>
#include <cassert>

namespace three {
  
  static Window * currentWindow = 0;
  
  // -- Callbacks --
  
  static void mouseButton(int button, int state)
  {
    if (currentWindow)
    {
      if (state == GLFW_PRESS)
        currentWindow->mousePressed(button);
      else
        currentWindow->mouseReleased(button);
    }
  }
  
  static void mousePosition(int x, int y)
  {
    if (currentWindow)
      currentWindow->mouseMoved(x, y);
  }

  static void windowResized(int width, int height)
  {
    if (currentWindow)
      currentWindow->resize(width, height);
  }
  
  // -- Window --
  
  Window::Window(int width, int height)
  {
    assert(!currentWindow && "We may only have one window active at all times");
    currentWindow = this;
    
    glfwInit();
    
    glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 2);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 0);
    
    glfwOpenWindow(width, height, 8, 8, 8, 8, 24, 0, GLFW_WINDOW);
    
    glfwSetMouseButtonCallback(&mouseButton);
    glfwSetMousePosCallback(&mousePosition);
    glfwSetWindowSizeCallback(&windowResized);
    
    glewInit();  
  }
  
  Window::~Window()
  {
    glfwTerminate();
    currentWindow = 0;
  }
  
  void Window::run()
  {
    glfwSetTime(0.0);
    m_lastTimeStamp = 0.0;
    m_totalTime = 0.0;

    // Call resize before we start
    {
      int width, height;
      glfwGetWindowSize(&width, &height);
      resize(width, height);
    }
    
    bool running = true;
    while (running)
    {
      running = step();
    }
  }
  
  bool Window::step()
  {
    double timeStamp = glfwGetTime();
    double dt = timeStamp - m_lastTimeStamp;
    m_lastTimeStamp = timeStamp;

    m_totalTime += dt;
    
    bool continueOn = update(dt);
    
    paint();
		
		glfwSwapBuffers();
		return continueOn && glfwGetWindowParam(GLFW_OPENED);
  }
  
  void Window::setTitle(std::string const& title)
  {
    glfwSetWindowTitle(title.c_str());
  }
  
  bool Window::isKeyDown(int key) const
  {
    return glfwGetKey(key);
  }
  
}

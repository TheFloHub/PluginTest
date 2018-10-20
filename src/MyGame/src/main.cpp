#include <GL/glew.h>
#include <GL/GL.h>
#include <GLFW/glfw3.h>
#include "InputManager.h"
#include <chrono>
#include <iostream>
#include <string>
#include <thread>

using namespace MyGame;

bool initGL(int width, int height)
{
  glViewport(0, 0, width, height);
  return true;
}

void errorCallback(int /*error*/, const char * description)
{
  std::cout << "GLFW Error: " << description << std::endl;
}

void resizeCallback(GLFWwindow * /*pWindow*/, int width, int height)
{
  glViewport(0, 0, width, height);
}

int main(void)
{
  // Initialize GLFW library
  GLFWwindow * window = nullptr;
  glfwSetErrorCallback(errorCallback);
  if (glfwInit() == false)
  {
    std::cout << "GLFW initialization failed." << std::endl;
    system("pause");
    exit(EXIT_FAILURE);
  }

  // Create a windowed mode window and its OpenGL context
  int const width = 800;
  int const height = 600;
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
  if (!window)
  {
    std::cout << "GLFW couldn't create a window." << std::endl;
    glfwTerminate();
    system("pause");
    exit(EXIT_FAILURE);
  }

  glfwSetFramebufferSizeCallback(window, resizeCallback);
  InputManager::getInstance().init(window);

  // Make the window's context current
  glfwMakeContextCurrent(window);
  glfwSwapInterval(0);

  // Initialize GLEW
  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK)
  {
    std::cout << "GLEW initialization failed." << std::endl;
    glfwTerminate();
    system("pause");
    exit(EXIT_FAILURE);
  }

  // Initialize G3dLib
  if (initGL(width, height) == false)
  {
    std::cout << "G3dLib initialization failed." << std::endl;
    glfwTerminate();
    system("pause");
    exit(EXIT_FAILURE);
  }

  // Frames per second
  double fpsSum = 0;
  size_t fpsIndex = 0;
  std::vector<double> fpsVector(100, 0.0);

  // Loop until the user closes the window
  double lastTime = glfwGetTime();
  double currentTime = 0.0;
  double deltaTime = 0.0;
  while (!glfwWindowShouldClose(window))
  {
    // Poll for and process events
    glfwPollEvents();

    // update
    //pRoot->update(deltaTime);

    // render
    //pRenderer->render(pRoot.get());

    // Swap front and back buffers
    glfwSwapBuffers(window);

    // Reset inputs
    InputManager::getInstance().resetFrame();

    // Update time
    currentTime = glfwGetTime();
    deltaTime = currentTime - lastTime;
    lastTime = currentTime;

    // Frames per second
    fpsSum -= fpsVector[fpsIndex];
    fpsSum += deltaTime;
    fpsVector[fpsIndex] = deltaTime;
    ++fpsIndex;
    if (fpsIndex == fpsVector.size())
    {
      fpsIndex = 0;
    }
    // std::cout << 1.0/(fpsSum/(double)fpsVector.size()) << std::endl;
    glfwSetWindowTitle(
        window,
        (std::string("FPS: ") +
         std::to_string((int)(1.0 / (fpsSum / (double)fpsVector.size()))))
            .c_str());

    std::this_thread::sleep_for(std::chrono::milliseconds(5));
  }

  glfwTerminate();
  exit(EXIT_SUCCESS);
}
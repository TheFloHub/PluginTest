#include "InputManager.h"
#include <GLFW/glfw3.h>

#include <iostream>

const size_t MyGame::InputManager::MAX_KEY_ID = 500;

const size_t MyGame::InputManager::MAX_MOUSE_BUTTON_ID = 10;

MyGame::InputManager & MyGame::InputManager::getInstance()
{
  static InputManager instance;
  return instance;
}

void MyGame::InputManager::keyCallback(GLFWwindow * pWindow, int key,
                                       int /*scancode*/, int action,
                                       int /*mods*/)
{
  if (key < MAX_KEY_ID)
  {
    if (action == GLFW_PRESS)
    {
      getInstance().mKey[key] = 1;
      getInstance().mKeyDown[key] = 1;
    }
    else if (action == GLFW_RELEASE)
    {
      getInstance().mKey[key] = 0;
      getInstance().mKeyUp[key] = 1;
    }
  }

  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(pWindow, GLFW_TRUE);
  }
}

void MyGame::InputManager::mouseButtonCallback(GLFWwindow * /*pWindow*/,
                                               int button, int action,
                                               int /*mods*/)
{
  if (button < MAX_MOUSE_BUTTON_ID)
  {
    if (action == GLFW_PRESS)
    {
      getInstance().mMouseButton[button] = 1;
      getInstance().mMouseButtonDown[button] = 1;
    }
    else if (action == GLFW_RELEASE)
    {
      getInstance().mMouseButton[button] = 0;
      getInstance().mMouseButtonUp[button] = 1;
    }
  }
}

void MyGame::InputManager::cursorPositionCallback(GLFWwindow * /*window*/,
                                                  double xpos, double ypos)
{
  getInstance().mDiffX = xpos - getInstance().mLastX;
  getInstance().mDiffY = ypos - getInstance().mLastY;
  getInstance().mLastX = xpos;
  getInstance().mLastY = ypos;
}

MyGame::InputManager::InputManager()
    : mKey(MAX_KEY_ID, 0), mKeyDown(MAX_KEY_ID, 0), mKeyUp(MAX_KEY_ID, 0),
      mMouseButton(MAX_MOUSE_BUTTON_ID, 0),
      mMouseButtonDown(MAX_MOUSE_BUTTON_ID, 0),
      mMouseButtonUp(MAX_MOUSE_BUTTON_ID, 0), mLastX(0.0), mLastY(0.0),
      mDiffX(0.0), mDiffY(0.0)
{
}

MyGame::InputManager::~InputManager() {}

void MyGame::InputManager::init(GLFWwindow * pWindow)
{
  glfwSetKeyCallback(pWindow, keyCallback);
  glfwSetMouseButtonCallback(pWindow, mouseButtonCallback);
  glfwSetCursorPosCallback(pWindow, cursorPositionCallback);
  glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
  glfwGetCursorPos(pWindow, &mLastX, &mLastY);
}

void MyGame::InputManager::resetFrame()
{
  std::fill(mKeyDown.begin(), mKeyDown.end(), static_cast<unsigned char>(0));
  std::fill(mKeyUp.begin(), mKeyUp.end(), static_cast<unsigned char>(0));
  std::fill(mMouseButtonDown.begin(), mMouseButtonDown.end(),
            static_cast<unsigned char>(0));
  std::fill(mMouseButtonUp.begin(), mMouseButtonUp.end(),
            static_cast<unsigned char>(0));

  mDiffX = 0.0;
  mDiffY = 0.0;
}

bool MyGame::InputManager::getKey(KeyboardInput name) const
{
  return mKey[static_cast<size_t>(name)] != 0;
}

bool MyGame::InputManager::getKeyDown(KeyboardInput name) const
{
  return mKeyDown[static_cast<size_t>(name)] != 0;
}

bool MyGame::InputManager::getKeyUp(KeyboardInput name) const
{
  return mKeyUp[static_cast<size_t>(name)] != 0;
}

bool MyGame::InputManager::getMouseButton(MouseInput name) const
{
  return mMouseButton[static_cast<size_t>(name)] != 0;
}

bool MyGame::InputManager::getMouseButtonDown(MouseInput name) const
{
  return mMouseButtonDown[static_cast<size_t>(name)] != 0;
}

bool MyGame::InputManager::getMouseButtonUp(MouseInput name) const
{
  return mMouseButtonUp[static_cast<size_t>(name)] != 0;
}

double MyGame::InputManager::getMouseDeltaX() const { return mDiffX; }

double MyGame::InputManager::getMouseDeltaY() const { return mDiffY; }
/**
 * @author Florian Feuerstein
 * @date 28.07.2016
 *
 * Base class for everything attached to scene objects.
 * Components add logic and functionality to empty scene objects.
 */

#pragma once

#include "Core/CoreDll.h"

class SceneObject;

class CORE_API Component
{

public:

  Component();
  
  virtual ~Component();

  Component(Component const &) = delete;

  Component(Component &&) = delete;

  Component & operator=(Component const &) = delete;

  Component & operator=(Component &&) = delete;

  virtual void update(double deltaTime);

  virtual void render() const;
  
  SceneObject const * getSceneObject() const;

  SceneObject * getSceneObject();

  void setEnabled(bool isEnabled);

  bool isEnabled() const;

protected:
  friend class SceneObject;

  SceneObject * m_sceneObject{nullptr};

  bool m_isEnabled{true};

};

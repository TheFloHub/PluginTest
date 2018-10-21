/**
 * @author Florian Feuerstein
 * @date 28.07.2016
 *
 * Scene objects are empty containers for components.
 * Adding components to a scene object is like
 * filling the scene object with logic and
 * functionality. Scene objects can have children
 * so that they create a hierarchy. The standard
 * component of a scene object is the
 * transform component giving it a distinct
 * position, rotation and scale. These transformations
 * are also applied to all children. The scene object is
 * also the owner of all its components and children.
 */

// TODO: make all SceneObjects and Components shared?

#pragma once

#include "Core/CoreDll.h"
#include <memory>

class Component;

class SceneObject final
{
public:
  CORE_API SceneObject();

  CORE_API ~SceneObject();

  CORE_API SceneObject(SceneObject const &) = delete;

  CORE_API SceneObject & operator=(SceneObject const &) = delete;

  CORE_API SceneObject(SceneObject &&) = delete;

  CORE_API SceneObject & operator=(SceneObject &&) = delete;

  /**
   * Adds the component to the scene object and commits ownership.
   * If the component could not be added the method returns false.
   */
  CORE_API bool addComponent(Component * pComponent);

  /**
   * Removes the component from this scene object committing ownership to the
   * caller. Only call this, if you know what you're doing.
   */
  CORE_API void removeComponent(Component * pComponent);

  /** Returns the component of the specific type or nullptr. */
  // template <class TComponent> TComponent * getComponent()
  //{
  //  auto iter = m_components.find(typeid(TComponent).hash_code());
  //  if (iter != std::end(m_components))
  //  {
  //    assert(dynamic_cast<TComponent *>(iter->second));
  //    return static_cast<TComponent *>(iter->second);
  //  }
  //  return nullptr;
  //}

  /** Returns the component of the specific type or nullptr. */
  // template <class TComponent> TComponent const * getComponent() const
  //{
  //  auto iter = m_components.find(typeid(TComponent).hash_code());
  //  if (iter != std::end(m_components))
  //  {
  //    assert(dynamic_cast<TComponent *>(iter->second));
  //    return static_cast<TComponent const *>(iter->second);
  //  }
  //  return nullptr;
  //}

  /** Returns the parent scene object or nullptr. */
  CORE_API SceneObject * getParent();

  /** Returns the parent scene object or nullptr. */
  CORE_API SceneObject const * getParent() const;

  /**
   * Adds the scene object as child and commits ownership.
   * If the child could not be added the method returns false.
   */
  CORE_API bool addChild(SceneObject * pChild);

  /** Updates all components and children with their components. */
  CORE_API void update(double deltaTime);

  /** Renders all components. */
  CORE_API void render() const;

  /** Returns the number of children. */
  CORE_API size_t getNumberOfChildren() const;

  /** Returns true if the node has no children. */
  CORE_API bool isLeafNode() const;

  /** Return the child at the given index. Not boundary safe. */
  CORE_API SceneObject * getChild(size_t index);

  /** Return the child at the given index. Not boundary safe. */
  CORE_API SceneObject const * getChild(size_t index) const;

  /** Enables or disables the scene object. */
  CORE_API void setEnabled(bool isEnabled);

  /** Returns true if the scene object is enabled. */
  CORE_API bool isEnabled() const;

private:
  class Impl;
  friend class Impl;
  std::unique_ptr<Impl> m_impl;
};

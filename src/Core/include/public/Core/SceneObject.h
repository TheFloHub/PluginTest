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
#include <string>
#include <typeinfo>
#include <unordered_map>
#include <vector>

// TODO: pimpl? no std in interface

class Component;

class CORE_API SceneObject final
{
public:
  SceneObject();

  SceneObject(std::string const & name);

  ~SceneObject();

  SceneObject(SceneObject const &) = delete;

  SceneObject & operator=(SceneObject const &) = delete;

  SceneObject(SceneObject &&) = delete;

  SceneObject & operator=(SceneObject &&) = delete;

  /**
   * Adds the component to the scene object and commits ownership.
   * If the component could not be added the method returns false.
   */
  bool addComponent(Component * pComponent);

  /**
   * Removes the component from this scene object committing ownership to the
   * caller. Only call this, if you know what you're doing.
   */
  void removeComponent(Component * pComponent);

  /** Returns the component of the specific type or nullptr. */
  template <class TComponent> TComponent * getComponent()
  {
    auto iter = m_components.find(typeid(TComponent).hash_code());
    if (iter != std::end(m_components))
    {
      assert(dynamic_cast<TComponent *>(iter->second));
      return static_cast<TComponent *>(iter->second);
    }
    return nullptr;
  }

  /** Returns the component of the specific type or nullptr. */
  template <class TComponent> TComponent const * getComponent() const
  {
    auto iter = m_components.find(typeid(TComponent).hash_code());
    if (iter != std::end(m_components))
    {
      assert(dynamic_cast<TComponent *>(iter->second));
      return static_cast<TComponent const *>(iter->second);
    }
    return nullptr;
  }

  /** Returns the parent scene object or nullptr. */
  SceneObject * getParent();

  /** Returns the parent scene object or nullptr. */
  SceneObject const * getParent() const;

  /**
   * Adds the scene object as child and commits ownership.
   * If the child could not be added the method returns false.
   */
  bool addChild(SceneObject * pChild);

  /** Updates all components and children with their components. */
  void update(double deltaTime);

  /** Renders all components. */
  void render() const;

  /** Returns the number of children. */
  size_t getNumberOfChildren() const;

  /** Returns true if the node has no children. */
  bool isLeafNode() const;

  /** Return the child at the given index. Not boundary safe. */
  SceneObject * getChild(size_t index);

  /** Return the child at the given index. Not boundary safe. */
  SceneObject const * getChild(size_t index) const;

  /** Returns the first child with the given name. */
  SceneObject * getChild(std::string const & name);

  /** Returns the first child with the given name. */
  SceneObject const * getChild(std::string const & name) const;

  /** Enables or disables the scene object. */
  void setEnabled(bool isEnabled);

  /** Returns true if the scene object is enabled. */
  bool isEnabled() const;

  /** Returns the name of the scene object. */
  std::string const & getName() const;

private:
  /**
   * Removes the child from this scene object committing ownership to the
   * caller. Only call this, if you know what you're doing.
   */
  void removeChild(SceneObject * child);

  /** Checks if the given node is a descendant of this scene object. */
  bool isNodeDescendant(SceneObject const * pNode) const;

  /** The name of this scene object. */
  std::string m_name{};

  /** The parent scene object. */
  SceneObject * m_parent{nullptr};

  /** The children scene objects. */
  std::vector<SceneObject *> m_children{};

  /**
   * The map of all components. The key is a unique identifier
   * of the component class. Thus only one instance of one
   * specific component type is allowed, which is part of the concept.
   */
  std::unordered_map<size_t, Component *> m_components{};

  /**
   * If a scene object is disabled then its components as well as its children
   * are neither updated nor rendered.
   */
  bool m_isEnabled{true};
};

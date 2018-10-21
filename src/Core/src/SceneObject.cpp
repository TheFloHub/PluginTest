#include "Core/SceneObject.h"
#include "Core/Component.h"
#include <algorithm>
#include <string>
#include <typeinfo>
#include <unordered_map>
#include <vector>

/********** Impl start ************/

class SceneObject::Impl final
{
public:
  Impl(SceneObject * d);

  ~Impl();

  Impl(Impl const &) = delete;

  Impl & operator=(Impl const &) = delete;

  Impl(Impl &&) = delete;

  Impl & operator=(Impl &&) = delete;

  bool addComponent(Component * pComponent);

  void removeComponent(Component * pComponent);

  SceneObject * getParent();

  SceneObject const * getParent() const;

  bool addChild(SceneObject * pChild);

  void update(double deltaTime);

  void render() const;

  size_t getNumberOfChildren() const;

  bool isLeafNode() const;

  SceneObject * getChild(size_t index);

  SceneObject const * getChild(size_t index) const;

  void setEnabled(bool isEnabled);

  bool isEnabled() const;

private:
  /**
   * Removes the child from this scene object committing ownership to the
   * caller. Only call this, if you know what you're doing.
   */
  void removeChild(SceneObject * child);

  /** Checks if the given node is a descendant of this scene object. */
  bool isNodeDescendant(SceneObject const * pNode) const;

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

  SceneObject * m_d;
};

SceneObject::Impl::Impl(SceneObject * d) : m_d(d) {}

SceneObject::Impl::~Impl()
{
  if (m_parent != nullptr)
  {
    m_parent->m_impl->removeChild(m_d);
  }

  for (auto & component : m_components)
  {
    component.second->m_sceneObject = nullptr; // detach component
    delete component.second;                   // delete component
  }
  m_components.clear();

  for (auto & child : m_children)
  {
    child->m_impl->m_parent = nullptr; // detach node
    delete child;                      // delete node
  }
  m_children.clear();
}

bool SceneObject::Impl::addComponent(Component * pComponent)
{
  // don't add if null or if there already exists an owner
  if (pComponent == nullptr || pComponent->m_sceneObject != nullptr)
    return false;
  // insert the component if there isn't a component of the same type yet.
  auto infoPair = m_components.insert(std::pair<size_t, Component *>(
      typeid(*pComponent).hash_code(), pComponent));
  // if insertion was successfull set this as the owner
  if (infoPair.second)
  {
    infoPair.first->second->m_sceneObject = m_d;
  }
  return infoPair.second;
}

void SceneObject::Impl::removeComponent(Component * pComponent)
{
  auto id = typeid(*pComponent).hash_code();
  auto cIter = m_components.find(id);
  if (cIter != std::end(m_components) && cIter->second == pComponent)
  {
    m_components.erase(cIter);
  }
}

SceneObject * SceneObject::Impl::getParent() { return m_parent; }

SceneObject const * SceneObject::Impl::getParent() const { return m_parent; }

bool SceneObject::Impl::addChild(SceneObject * pChild)
{
  if (pChild == nullptr || pChild == m_d || pChild->m_impl->m_parent == m_d)
    return false;
  // if there already exists a parent for the new child
  if (pChild->m_impl->m_parent != nullptr)
  {
    // and it's not valid to add the child return false.
    if (pChild->m_impl->isNodeDescendant(m_d))
    {
      return false;
    }
    // otherwise detach the child
    else
    {
      pChild->m_impl->m_parent->m_impl->removeChild(pChild);
    }
    return false;
  }
  pChild->m_impl->m_parent = m_d;
  m_children.push_back(pChild);
  return true;
}

void SceneObject::Impl::update(double deltaTime)
{
  // HINT: no need to update transform
  for (auto iter = m_components.begin(); iter != m_components.end(); ++iter)
  {
    if (iter->second->isEnabled())
    {
      iter->second->update(deltaTime);
    }
  }
  for (auto iter = m_children.begin(); iter != m_children.end(); ++iter)
  {
    if ((*iter)->isEnabled())
    {
      (*iter)->update(deltaTime);
    }
  }
}

void SceneObject::Impl::render() const
{
  // HINT: no need to render transform
  for (auto iter = m_components.cbegin(); iter != m_components.cend(); ++iter)
  {
    if (iter->second->isEnabled())
    {
      iter->second->render();
    }
  }
}

size_t SceneObject::Impl::getNumberOfChildren() const
{
  return m_children.size();
}

bool SceneObject::Impl::isLeafNode() const { return m_children.empty(); }

SceneObject * SceneObject::Impl::getChild(size_t index)
{
  return m_children[index];
}

SceneObject const * SceneObject::Impl::getChild(size_t index) const
{
  return m_children[index];
}

void SceneObject::Impl::setEnabled(bool isEnabled) { m_isEnabled = isEnabled; }

bool SceneObject::Impl::isEnabled() const { return m_isEnabled; }

void SceneObject::Impl::removeChild(SceneObject * child)
{

  auto iter = m_children.begin();
  auto last = m_children.end();
  while (iter != last && (*iter) != child)
  {
    ++iter;
  }
  if (iter != last)
  {
    m_children.erase(iter);
    child->m_impl->m_parent = nullptr;
  }
}

bool SceneObject::Impl::isNodeDescendant(SceneObject const * pNode) const
{
  if (pNode == nullptr)
    return false;
  SceneObject const * pCurrentNode = pNode;
  while (pCurrentNode != nullptr && pCurrentNode != m_d)
  {
    pCurrentNode = pCurrentNode->m_impl->m_parent;
  }
  return pCurrentNode == m_d;
}

/******************** Impl end *************************************/

SceneObject::SceneObject() : m_impl(new Impl(this)) {}

SceneObject::~SceneObject() = default;

bool SceneObject::addComponent(Component * pComponent)
{
  return m_impl->addComponent(pComponent);
}

void SceneObject::removeComponent(Component * pComponent)
{
  return m_impl->removeComponent(pComponent);
}

SceneObject * SceneObject::getParent() { return m_impl->getParent(); }

SceneObject const * SceneObject::getParent() const
{
  return m_impl->getParent();
}

bool SceneObject::addChild(SceneObject * pChild)
{
  return m_impl->addChild(pChild);
}

void SceneObject::update(double deltaTime) { m_impl->update(deltaTime); }

void SceneObject::render() const { m_impl->render(); }

size_t SceneObject::getNumberOfChildren() const
{
  return m_impl->getNumberOfChildren();
}

bool SceneObject::isLeafNode() const { return m_impl->isLeafNode(); }

SceneObject * SceneObject::getChild(size_t index)
{
  return m_impl->getChild(index);
}

SceneObject const * SceneObject::getChild(size_t index) const
{
  return m_impl->getChild(index);
}

void SceneObject::setEnabled(bool isEnabled) { m_impl->setEnabled(isEnabled); }

bool SceneObject::isEnabled() const { return m_impl->isEnabled(); }

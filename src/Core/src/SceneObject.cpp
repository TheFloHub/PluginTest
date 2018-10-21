#include "Core/SceneObject.h"
#include "Core/Component.h"
#include <algorithm>

SceneObject::SceneObject() = default;

SceneObject::SceneObject(std::string const & name)
    : m_name(name)
{

}

SceneObject::~SceneObject()
{
  if (m_parent != nullptr)
  {
    m_parent->removeChild(this);
  }

  for (auto & component : m_components)
  {
    component.second->m_sceneObject = nullptr; // detach component
    delete component.second;                   // delete component
  }
  m_components.clear();

  for (auto & child : m_children)
  {
    child->m_parent = nullptr; // detach node
    delete child;              // delete node
  }
  m_children.clear();
}


bool SceneObject::addComponent(Component * pComponent)
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
    infoPair.first->second->m_sceneObject = this;
  }
  return infoPair.second;
}

void SceneObject::removeComponent(Component * pComponent)
{
  auto id = typeid(*pComponent).hash_code();
  auto cIter = m_components.find(id);
  if (cIter != std::end(m_components) && cIter->second == pComponent)
  {
    m_components.erase(cIter);
  }
}

bool SceneObject::addChild(SceneObject * pChild)
{
  if (pChild == nullptr || pChild == this || pChild->m_parent == this)
    return false;
  // if there already exists a parent for the new child
  if (pChild->m_parent != nullptr)
  {
    // and it's not valid to add the child return false.
    if (pChild->isNodeDescendant(this))
    {
      return false;
    }
    // otherwise detach the child
    else
    {
      pChild->m_parent->removeChild(pChild);
    }
    return false;
  }
  pChild->m_parent = this;
  m_children.push_back(pChild);
  return true;
}


void SceneObject::removeChild(SceneObject * child)
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
    child->m_parent = nullptr;
  }
}

void SceneObject::update(double deltaTime)
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

void SceneObject::render() const
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

bool SceneObject::isNodeDescendant(SceneObject const * pNode) const
{
  if (pNode == nullptr)
    return false;
  SceneObject const * pCurrentNode = pNode;
  while (pCurrentNode != nullptr && pCurrentNode != this)
    pCurrentNode = pCurrentNode->m_parent;
  return pCurrentNode == this;
}

size_t SceneObject::getNumberOfChildren() const
{
  return m_children.size();
}

SceneObject * SceneObject::getParent() { return m_parent; }

SceneObject const * SceneObject::getParent() const { return m_parent; }

bool SceneObject::isLeafNode() const { return m_children.empty(); }

SceneObject * SceneObject::getChild(size_t index)
{
  return m_children[index];
}

SceneObject const * SceneObject::getChild(size_t index) const
{
  return m_children[index];
}

SceneObject * SceneObject::getChild(std::string const & name)
{
  auto iter = m_children.begin();
  auto end = m_children.end();
  while (iter != end && (*iter)->m_name != name)
  {
    ++iter;
  }
  if (iter != end)
  {
    return *iter;
  }
  return nullptr;
}

SceneObject const * SceneObject::getChild(std::string const & name) const
{
  auto iter = m_children.cbegin();
  auto end = m_children.cend();
  while (iter != end && (*iter)->m_name != name)
  {
    ++iter;
  }
  if (iter != end)
  {
    return *iter;
  }
  return nullptr;
}

void SceneObject::setEnabled(bool isEnabled) { m_isEnabled = isEnabled; }

bool SceneObject::isEnabled() const { return m_isEnabled; }

std::string const & SceneObject::getName() const { return m_name; }
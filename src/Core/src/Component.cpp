#include "Core/Component.h"
#include "Core/SceneObject.h"

Component::Component() = default;

Component::~Component()
{
  if (m_sceneObject != nullptr)
  {
    m_sceneObject->removeComponent(this);
  }
}

void Component::update(double) {}

void Component::render() const {}

SceneObject const * Component::getSceneObject() const { return m_sceneObject; }

SceneObject * Component::getSceneObject() { return m_sceneObject; }

void Component::setEnabled(bool isEnabled) { m_isEnabled = isEnabled; }

bool Component::isEnabled() const { return m_isEnabled; }
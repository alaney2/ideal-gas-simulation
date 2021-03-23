#pragma once

#include "cinder/gl/gl.h"

namespace idealgas {

/**
 * A gas particle to put in the gas container.
 */
class Particle {
 public:
  Particle(const glm::vec2& position, const glm::vec2& velocity, int mass,
           int radius, const ci::Color& color);

  double GetSpeed();
  glm::vec2 GetPosition() const;
  glm::vec2 GetVelocity() const;
  double GetMass() const;
  int GetRadius() const;
  ci::Color GetColor() const;

  void SetPosition(const glm::vec2& position);
  void SetVelocity(const glm::vec2& velocity);

 private:
  glm::vec2 position_;
  glm::vec2 velocity_;
  double mass_;
  int radius_;
  ci::Color color_;
};

}  // namespace idealgas
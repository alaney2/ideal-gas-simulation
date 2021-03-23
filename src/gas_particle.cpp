#include "gas_particle.h"

namespace idealgas {

Particle::Particle(const glm::vec2& position, const glm::vec2& velocity,
                   int mass, int radius, const ci::Color& color)
    : position_(position),
      velocity_(velocity),
      mass_(mass),
      radius_(radius),
      color_(color) {
}

double Particle::GetSpeed() const {
  return glm::length(velocity_);
}

glm::vec2 Particle::GetPosition() const {
  return position_;
}

glm::vec2 Particle::GetVelocity() const {
  return velocity_;
}

double Particle::GetMass() const {
  return mass_;
}

int Particle::GetRadius() const {
  return radius_;
}

ci::Color Particle::GetColor() const {
  return color_;
}

void Particle::SetPosition(const glm::vec2& position) {
  position_ = position;
}
void Particle::SetVelocity(const glm::vec2& velocity) {
  velocity_ = velocity;
}

}  // namespace idealgas
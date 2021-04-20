#include "physics_engine.h"

using glm::vec2;
using std::vector;

namespace idealgas {

PhysicsEngine::PhysicsEngine() { }

void PhysicsEngine::ParticleWallCollision(const size_t window_length,
                                          const size_t margin, Particle& particle) {
  double lower_bound = margin + particle.GetRadius();
  double upper_bound = window_length - margin - particle.GetRadius();

  double x_pos = particle.GetPosition().x;
  double y_pos = particle.GetPosition().y;

  if (x_pos <= lower_bound || x_pos >= upper_bound) {
    particle.SetVelocity(
        vec2(-particle.GetVelocity().x, particle.GetVelocity().y));
  }

  if (y_pos <= lower_bound || y_pos >= upper_bound) {
    particle.SetVelocity(
        vec2(particle.GetVelocity().x, -particle.GetVelocity().y));
  }
}

bool PhysicsEngine::DetectCollision(const Particle& p1, const Particle& p2) {
  vec2 velocity_diff = p1.GetVelocity() - p2.GetVelocity();
  vec2 position_diff = p1.GetPosition() - p2.GetPosition();

  bool is_touching = glm::distance(p1.GetPosition(), p2.GetPosition()) <=
                     p1.GetRadius() + p2.GetRadius();
  bool is_moving_closer = glm::dot(velocity_diff, position_diff) < 0;

  return is_touching && is_moving_closer;
}

void PhysicsEngine::AdjustVelocitiesOnCollision(vector<idealgas::Particle> &particles) {
  for (size_t i = 0; i < particles.size(); ++i) {
    for (size_t j = i + 1; j < particles.size(); ++j) {
      if (DetectCollision(particles[i], particles[j])) {
        vec2 velocity_1 =
            GetVelocityAfterCollision(particles[i], particles[j]);
        vec2 velocity_2 =
            GetVelocityAfterCollision(particles[j], particles[i]);

        particles[i].SetVelocity(velocity_1);
        particles[j].SetVelocity(velocity_2);
      }
    }
  }
}

vec2 PhysicsEngine::GetVelocityAfterCollision(const Particle& p1,
                                              const Particle& p2) {
  vec2 velocity_diff = p1.GetVelocity() - p2.GetVelocity();
  vec2 position_diff = p1.GetPosition() - p2.GetPosition();

  double mass_ratio = 2 * p2.GetMass() / (p1.GetMass() + p2.GetMass());
  double constant = glm::dot(velocity_diff, position_diff) /
                    pow(glm::length(position_diff), 2);

  vec2 new_position = vec2(mass_ratio * constant * position_diff.x,
                           mass_ratio * constant * position_diff.y);

  return p1.GetVelocity() - new_position;
}
} // namespace idealgas

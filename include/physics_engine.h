#pragma once

#include "cinder/gl/gl.h"
#include "gas_particle.h"

using glm::vec2;
using std::vector;

namespace idealgas {

/**
 *
 */
class PhysicsEngine {
 public:
  /**
   *
   */
  PhysicsEngine();
  /**
   * Sets new velocity after hitting wall.
   */
  static void NegateVelocityOnWallCollision(const int window_size, const int margin, Particle &particle);

  /**
   * Detects if there is a collision between two particles.
   * @param p1 first particle
   * @param p2 second particle
   * @return if there is a collision
   */
  static bool DetectCollision(const Particle &p1, const Particle &p2);

  /**
   * Sets new velocities of particles that have collided.
   */
  static void AdjustVelocityOnCollision(vector<idealgas::Particle> &particles);

  /**
   * Gets the new velocity after a collision.
   * @param p1 first particle
   * @param p2 second particle
   * @return new velocity vec2 after collision
   */
  static vec2 GetVelocityAfterCollision(const Particle &p1, const Particle &p2);
};

}  // namespace idealgas
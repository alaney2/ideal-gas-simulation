#pragma once

#include "cinder/gl/gl.h"
#include "gas_particle.h"

namespace idealgas {

/**
 * The engine which runs all the calculations for the particles in the gas container.
 */
class PhysicsEngine {
 public:
  PhysicsEngine();

  /**
   * Sets new velocity after hitting wall.
   */
  static void ParticleWallCollision(const size_t window_length,
                                    const size_t margin,
                                            Particle &particle);

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
  static void AdjustVelocitiesOnCollision(std::vector<idealgas::Particle> &particles);

  /**
   * Gets the new velocity after a collision.
   * @param p1 first particle
   * @param p2 second particle
   * @return new velocity vec2 after collision
   */
  static glm::vec2 GetVelocityAfterCollision(const Particle &p1, const Particle &p2);
};

}  // namespace idealgas
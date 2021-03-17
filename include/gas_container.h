#pragma once

#include "cinder/gl/gl.h"
#include "gas_particle.h"

using glm::vec2;

namespace idealgas {

/**
 * The container in which all of the gas particles_ are contained. This class
 * stores all of the particles_ and updates them on each frame of the simulation.
 */
class GasContainer {
 public:
  /**
   * The gas container to hold all the gas particles.
   */
  GasContainer(int kWindowSize, int kMargin,
               const ci::Color& kBorderColor);

  /**
   * Displays the container walls and the current positions of the particles_.
   */
  void Display() const;

  /**
   * Updates the positions and velocities of all particles_ (based on the rules
   * described in the assignment documentation).
   */
  void AdvanceOneFrame();

  /**
   * Generates a given amount of particles of a given particle to a vector.
   * @param particles vector of particles
   * @param particle to generate
   * @param particle_amount number of particles to generate
   */
  void GenerateParticles(std::vector<idealgas::Particle> &particles,
                         Particle &particle, size_t particle_amount);

  /**
   * Sets new velocity after hitting wall.
   */
  void NegateVelocityOnWallCollision(Particle &particle);

  /**
   * Detects if there is a collision between two particles.
   * @param p1 first particle
   * @param p2 second particle
   * @return if there is a collision
   */
  static bool DetectCollision(Particle &p1, Particle &p2);

  /**
   * Sets new velocities of particles that have collided.
   */
  void AdjustVelocityOnCollision();

  /**
   * Gets the new velocity after a collision.
   * @param p1 first particle
   * @param p2 second particle
   * @return new velocity vec2 after collision
   */
  vec2 GetVelocityAfterCollision(Particle &p1, Particle &p2);

 private:
  const int kWindowSize_; // size of application window
  const int kMargin_; // size of margin surrounding container
  const ci::Color kBorderColor_; // color of gas container border
  std::vector<idealgas::Particle> particles_; // vector of particles in container
};

}  // namespace idealgas

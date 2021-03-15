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
   * TODO: Add more parameters to this constructor, and add documentation.
   */
  GasContainer(const int kWindowSize, const int kMargin,
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

  void GenerateParticles(std::vector<idealgas::Particle> &particles,
                         Particle particle, size_t particle_amount);

 private:
  /**
   * This variable is just for the purposes of demonstrating how to make a shape move
   * across a screen. Please remove it once you start working on your code.
   */
  int dummy_variable_ = 0;
  const int kWindowSize_;
  const int kMargin_;
  const ci::Color kBorderColor_;
  std::vector<idealgas::Particle> particles_;
};

}  // namespace idealgas

#pragma once

#include "cinder/gl/gl.h"
#include "gas_particle.h"

namespace idealgas {

/**
 * The container in which all of the gas particles_ are contained. This class
 * stores all of the particles_ and updates them on each frame of the
 * simulation.
 */
class GasContainer {
 public:
  /**
   * The gas container used to hold the gas particles.
   */
  GasContainer(const int kWindowLength, const int kWindowWidth,
               const int kMargin, const ci::Color &kBorderColor);

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
   * Draws the outlines for the histograms
   */
  void DrawHistogramBoxes() const;

  /**
   * Updates three maps of different particles
   */
  void UpdateHistograms();

  /**
   * Draws histogram bins
   * @param top_left_corner of histogram
   * @param bottom_right_corner of histogram
   * @param color of particles and histogram bins
   * @param speeds map of how many particles are in each bin
   */
  void DisplayHistogram(const glm::vec2 &top_left_corner,
                        const glm::vec2 &bottom_right_corner,
                        const ci::Color &color,
                        std::map<int, int> speeds) const;

  /**
   * sets all values of keys in histogram maps to 0
   */
  void ResetHistograms();

  /**
   * @return speed of fastest particle
   */
  int MaxParticleSpeed() const;

  /**
   * Slows down all particles
   */
  void SlowDownParticles();

  /**
   * Speeds up all particles
   */
  void SpeedUpParticles();

 private:
  int frames = 0;
  const int kWindowLength_;          // length of the application window
  const int kWindowWidth_;           // width of the application window
  const int kMargin_;                // size of margin surrounding container
  const ci::Color kBorderColor_;     // color of gas container border
  std::vector<idealgas::Particle> particles_;
                                     // vector of particles in container
  std::map<int, int> slow_speeds_;   // map of how many particles are in each bin for the slow particles
  std::map<int, int> medium_speeds_; // medium particles
  std::map<int, int> fast_speeds_;   // fast particles
  const size_t num_bins_ = 8;        // number of bins in each histogram
};

}  // namespace idealgas

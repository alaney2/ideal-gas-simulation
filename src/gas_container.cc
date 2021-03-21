#include "gas_container.h"
#include "physics_engine.h"

namespace idealgas {

using glm::vec2;

GasContainer::GasContainer(const int kWindowSize, const int kMargin,
                           const ci::Color &kBorderColor)
    : kWindowSize_(kWindowSize),
      kMargin_(kMargin),
      kBorderColor_(kBorderColor) {
  Particle green_particle(vec2(), vec2(4, 4), 5, 10, ci::Color("green"));
  GenerateParticles(particles_, green_particle, 60);
}

void GasContainer::Display() const {
  for (const auto &particle : particles_) {
    ci::gl::color(particle.GetColor());
    ci::gl::drawSolidCircle(particle.GetPosition(),
                            static_cast<float>(particle.GetRadius()));
  }
  ci::gl::color(kBorderColor_);
  ci::gl::drawStrokedRect(
      ci::Rectf(vec2(kMargin_, kMargin_),
                vec2(kWindowSize_ - kMargin_, kWindowSize_ - kMargin_)));
}

void GasContainer::AdvanceOneFrame() {
  PhysicsEngine::AdjustVelocityOnCollision(particles_);

  for (auto &particle : particles_) {
    PhysicsEngine::NegateVelocityOnWallCollision(kWindowSize_, kMargin_, particle);
    particle.SetPosition(particle.GetPosition() += particle.GetVelocity());
  }
}

void GasContainer::GenerateParticles(std::vector<idealgas::Particle> &particles,
                                     Particle &particle,
                                     size_t particle_amount) {
  size_t max_particles =
      pow((kWindowSize_ - 2 * kMargin_) / (2 * particle.GetRadius()), 2);
  if (particle_amount > max_particles) {
    particle_amount = max_particles;
  }

  size_t lower_bound = kMargin_ + particle.GetRadius();
  size_t upper_bound = kWindowSize_ - 2 * (kMargin_ + particle.GetRadius());

  for (size_t i = 0; i < particle_amount; ++i) {
    size_t rand_x_pos = (rand() % (upper_bound + 1)) + lower_bound;
    size_t rand_y_pos = (rand() % (upper_bound + 1)) + lower_bound;

    particle.SetPosition(vec2(rand_x_pos, rand_y_pos));
    particles.push_back(particle);
  }
}

}  // namespace idealgas

#include "gas_container.h"
#include "physics_engine.h"

namespace idealgas {

using glm::vec2;

GasContainer::GasContainer(const int kWindowLength, const int kWindowWidth,
                           const int kMargin,
                           const ci::Color &kBorderColor)
    : kWindowLength_(kWindowLength),
      kWindowWidth_(kWindowWidth),
      kMargin_(kMargin),
      kBorderColor_(kBorderColor) {
  Particle green_particle(vec2(), vec2(4, 4), 15, 15, ci::Color("green"));
  Particle red_particle(vec2(), vec2(4, 4), 10, 10, ci::Color("red"));
  Particle orange_particle(vec2(), vec2(4, 4), 6, 6, ci::Color("orange"));

  GenerateParticles(particles_, green_particle, 30);
  GenerateParticles(particles_, red_particle, 30);
  GenerateParticles(particles_, orange_particle, 30);
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
                vec2(kWindowLength_ - kMargin_, kWindowLength_ - kMargin_)));
  GasContainer::DrawHistogramBoxes();
}

void GasContainer::AdvanceOneFrame() {
  PhysicsEngine::AdjustVelocitiesOnCollision(particles_);

  for (auto &particle : particles_) {
    PhysicsEngine::ParticleWallCollision(kWindowLength_, kMargin_, particle);
    particle.SetPosition(particle.GetPosition() += particle.GetVelocity());
  }
}

void GasContainer::GenerateParticles(std::vector<idealgas::Particle> &particles,
                                     Particle &particle,
                                     size_t particle_amount) {
  size_t max_particles =
      pow((kWindowLength_ - 2 * kMargin_) / (2 * particle.GetRadius()), 2);
  if (particle_amount > max_particles) {
    particle_amount = max_particles;
  }

  size_t lower_bound = kMargin_ + particle.GetRadius();
  size_t upper_bound = kWindowLength_ - 2 * (kMargin_ + particle.GetRadius());

  for (size_t i = 0; i < particle_amount; ++i) {
    size_t rand_x_pos = (rand() % (upper_bound + 1)) + lower_bound;
    size_t rand_y_pos = (rand() % (upper_bound + 1)) + lower_bound;

    particle.SetPosition(vec2(rand_x_pos, rand_y_pos));
    particles.push_back(particle);
  }
}
void GasContainer::DrawHistogramBoxes() const {
  ci::gl::drawStrokedRect(
      ci::Rectf(vec2(kWindowLength_, kMargin_/2),
                vec2(kWindowWidth_ - kMargin_, (kWindowLength_ - 2*kMargin_)/3 + kMargin_/2)));
  ci::gl::drawStrokedRect(
      ci::Rectf(vec2(kWindowLength_, (kWindowLength_ - 2*kMargin_)/3 + kMargin_),
                vec2(kWindowWidth_ - kMargin_, 2*(kWindowLength_ - 2*kMargin_)/3 + kMargin_)));
  ci::gl::drawStrokedRect(
      ci::Rectf(vec2(kWindowLength_, 2*(kWindowLength_ - 2*kMargin_)/3 + 3*kMargin_/2),
                vec2(kWindowWidth_ - kMargin_, kWindowLength_ - kMargin_/2)));
}

}  // namespace idealgas

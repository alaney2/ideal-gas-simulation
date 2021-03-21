#include "gas_container.h"

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
  AdjustVelocityOnCollision();

  for (auto &particle : particles_) {
    NegateVelocityOnWallCollision(particle);
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

void GasContainer::NegateVelocityOnWallCollision(Particle &particle) {
  double lower_bound = kMargin_ + particle.GetRadius();
  double upper_bound = kWindowSize_ - kMargin_ - particle.GetRadius();

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

void GasContainer::AdjustVelocityOnCollision() {
  for (size_t i = 0; i < particles_.size(); ++i) {
    for (size_t j = i + 1; j < particles_.size(); ++j) {
      if (DetectCollision(particles_[i], particles_[j])) {
        vec2 velocity_1 =
            GetVelocityAfterCollision(particles_[i], particles_[j]);
        vec2 velocity_2 =
            GetVelocityAfterCollision(particles_[j], particles_[i]);

        particles_[i].SetVelocity(velocity_1);
        particles_[j].SetVelocity(velocity_2);
      }
    }
  }
}

bool GasContainer::DetectCollision(const Particle &p1, const Particle &p2) {
  vec2 velocity_diff = p1.GetVelocity() - p2.GetVelocity();
  vec2 position_diff = p1.GetPosition() - p2.GetPosition();

  bool is_touching = glm::distance(p1.GetPosition(), p2.GetPosition()) <=
                     p1.GetRadius() + p2.GetRadius();
  bool is_moving_closer = glm::dot(velocity_diff, position_diff) < 0;

  return is_touching && is_moving_closer;
}

vec2 GasContainer::GetVelocityAfterCollision(const Particle &p1,
                                             const Particle &p2) {
  vec2 velocity_diff = p1.GetVelocity() - p2.GetVelocity();
  vec2 position_diff = p1.GetPosition() - p2.GetPosition();

  double mass_ratio = 2 * p2.GetMass() / (p1.GetMass() + p2.GetMass());
  double constant = glm::dot(velocity_diff, position_diff) /
                    pow(glm::length(position_diff), 2);

  vec2 new_position = vec2(mass_ratio * constant * position_diff.x,
                           mass_ratio * constant * position_diff.y);

  return p1.GetVelocity() - new_position;
}

}  // namespace idealgas

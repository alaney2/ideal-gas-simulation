#include "gas_container.h"

namespace idealgas {

using glm::vec2;

GasContainer::GasContainer(const int kWindowSize, const int kMargin,
                           const ci::Color &kBorderColor) : kWindowSize_(kWindowSize),
                            kMargin_(kMargin), kBorderColor_(kBorderColor) {
  Particle orange_particle(vec2(), vec2(1,1),5,100,ci::Color("orange"));
  GenerateParticles(particles_, orange_particle, 20);
}

void GasContainer::Display() const {
  // This function has a lot of magic numbers; be sure to design your code in a way that avoids this.
//  ci::gl::color(ci::Color("orange"));
//  ci::gl::drawSolidCircle(vec2(dummy_variable_, 200), 10);
  for (size_t i = 0; i < particles_.size(); i++) {
    ci::gl::color(particles_[i].GetColor());
    ci::gl::drawSolidCircle(particles_[i].GetPosition(), particles_[i].GetRadius());
  }
  ci::gl::color(kBorderColor_);
  ci::gl::drawStrokedRect(ci::Rectf(vec2(kMargin_, kMargin_), vec2(kWindowSize_-kMargin_,
                                                                                kWindowSize_-kMargin_)));
}

void GasContainer::AdvanceOneFrame() {
  NegateVelocityOnWallCollision();
  AdjustVelocityOnCollision();

  for (size_t i = 0; i < particles_.size(); ++i) {
    particles_[i].SetPosition(particles_[i].GetPosition() += particles_[i].GetVelocity());
  }

}

void GasContainer::GenerateParticles(std::vector<idealgas::Particle> &particles,
                                     Particle &particle, size_t particle_amount) {
  std::cout << particle_amount * pow(particle.GetRadius(), 2) << std::endl;
  std::cout << pow(kWindowSize_ - 2*(kMargin_ + particle.GetRadius()), 2) << std::endl;
  while (particle_amount * pow(particle.GetRadius(), 2)
         > pow(kWindowSize_ - 2*(kMargin_ + particle.GetRadius()), 2)) {
    particle_amount -= 1;
    std::cout << particle_amount << std::endl;
  }

  size_t lower_bound = kMargin_ + particle.GetRadius();
  size_t upper_bound = kWindowSize_ - 2*(kMargin_ + particle.GetRadius());

  for (size_t i = 0; i < particle_amount; ++i) {
    size_t rand_x_pos = (rand() % (upper_bound)) + lower_bound;
    size_t rand_y_pos = (rand() % (upper_bound)) + lower_bound;

    particle.SetPosition(vec2(rand_x_pos, rand_y_pos));
    particles.push_back(particle);
  }
}

void GasContainer::NegateVelocityOnWallCollision() {
  for (size_t i = 0; i < particles_.size(); ++i) {
    size_t lower_bound = kMargin_ + particles_[i].GetRadius();
    size_t upper_bound = kWindowSize_ - kMargin_ - particles_[i].GetRadius();

    size_t x_pos = particles_[i].GetPosition().x;
    size_t y_pos = particles_[i].GetPosition().y;

    if (x_pos <= lower_bound || x_pos >= upper_bound) {
      particles_[i].SetVelocity(vec2(-particles_[i].GetVelocity().x, particles_[i].GetVelocity().y));
    }

    if (y_pos <= lower_bound || y_pos >= upper_bound) {
      particles_[i].SetVelocity(vec2(particles_[i].GetVelocity().x, - particles_[i].GetVelocity().y));
    }
  }
}

void GasContainer::AdjustVelocityOnCollision() {
  for (size_t i = 0; i < particles_.size(); ++i) {
    for (size_t j = i + 1; j < particles_.size(); ++j) {
      if (DetectCollision(particles_[i], particles_[j])) {
        vec2 velocity_1 = GetVelocityAfterCollision(particles_[i], particles_[j]);
        vec2 velocity_2 = GetVelocityAfterCollision(particles_[j], particles_[i]);

        particles_[i].SetVelocity(velocity_1);
        particles_[j].SetVelocity(velocity_2);
      }
    }
  }
}

bool GasContainer::DetectCollision(Particle &p1, Particle &p2) const {
  vec2 velocity_diff = p1.GetVelocity() - p2.GetVelocity();
  vec2 position_diff = p1.GetPosition() - p2.GetPosition();

  bool is_touching = glm::distance(p1.GetPosition(), p2.GetPosition())
                     < p1.GetRadius() + p2.GetRadius();
  bool is_moving_closer = glm::dot(velocity_diff, position_diff) < 0;

  return is_touching && is_moving_closer;
}
vec2 GasContainer::GetVelocityAfterCollision(Particle &p1, Particle &p2) {
  vec2 velocity_diff = p1.GetVelocity() - p2.GetVelocity();
  vec2 position_diff = p1.GetPosition() - p2.GetPosition();

  double mass_ratio = 2.0 * p2.GetMass() / (p1.GetMass() + p2.GetMass());
  double constant = glm::dot(velocity_diff, position_diff)
                    / pow(glm::length(position_diff), 2);

  vec2 new_position = vec2(mass_ratio * constant * position_diff.x,
                           mass_ratio * constant * position_diff.y);

  return p1.GetVelocity() - new_position;
}

}  // namespace idealgas

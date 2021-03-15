#include "gas_container.h"

namespace idealgas {

using glm::vec2;

GasContainer::GasContainer(const int kWindowSize, const int kMargin,
                           const ci::Color &kBorderColor) : kWindowSize_(kWindowSize),
                            kMargin_(kMargin), kBorderColor_(kBorderColor) {
  Particle orange_particle(vec2(), vec2(1,1),5,20,ci::Color("orange"));
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

  for (size_t i = 0; i < particles_.size(); ++i) {
    particles_[i].SetPosition(particles_[i].GetPosition() += particles_[i].GetVelocity());
  }

}

void GasContainer::GenerateParticles(std::vector<idealgas::Particle> &particles,
                                     Particle particle, size_t particle_amount) {
  size_t lower_bound = kMargin_ + particle.GetRadius();
  size_t upper_bound = kWindowSize_ - 2*(kMargin_ + particle.GetRadius());

  for (size_t i = 0; i < particle_amount; ++i) {
    size_t rand_x_pos = (rand() % (upper_bound)) + lower_bound;
    size_t rand_y_pos = (rand() % (upper_bound)) + lower_bound;

    particle.SetPosition(vec2(rand_x_pos, rand_y_pos));
    particles.push_back(particle);
  }
}


}  // namespace idealgas

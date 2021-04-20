#include "gas_container.h"

namespace idealgas {

using std::vector;
using glm::vec2;

GasContainer::GasContainer(const size_t kWindowLength,
                           const size_t kWindowWidth, const size_t kMargin,
                           const ci::Color &kBorderColor)
    : kWindowLength_(kWindowLength),
      kWindowWidth_(kWindowWidth),
      kMargin_(kMargin),
      kBorderColor_(kBorderColor) {
  Particle orange_particle(vec2(), vec2(4, 4), 6, 6, fast_color_);
  Particle red_particle(vec2(), vec2(3, 2), 12, 12, medium_color_);
  Particle green_particle(vec2(), vec2(2, 2), 18, 18, slow_color_);

  GenerateParticles(particles_, green_particle, 33);
  GenerateParticles(particles_, red_particle, 33);
  GenerateParticles(particles_, orange_particle, 33);
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
                vec2(kWindowLength_ - kMargin_, kWindowLength_ - kMargin_)), 4);
  DisplayHistogram(vec2(kWindowLength_, kMargin_/2),
                   vec2(kWindowWidth_ - kMargin_, (kWindowLength_ - 2*kMargin_)/3 + kMargin_/2),
                  ci::Color("orange"), fast_speeds_);
  DisplayHistogram(vec2(kWindowLength_, (kWindowLength_ - 2*kMargin_)/3 + kMargin_),
                   vec2(kWindowWidth_ - kMargin_, 2*(kWindowLength_ - 2*kMargin_)/3 + kMargin_),
                  ci::Color("red"), medium_speeds_);
  DisplayHistogram(vec2(kWindowLength_, 2*(kWindowLength_ - 2*kMargin_)/3 + 3*kMargin_/2),
                   vec2(kWindowWidth_ - kMargin_, kWindowLength_ - kMargin_/2),
                  ci::Color("green"), slow_speeds_);

  GasContainer::DrawHistogramBoxes();
}

void GasContainer::AdvanceOneFrame() {
  ++frames;
  PhysicsEngine::AdjustVelocitiesOnCollision(particles_);

  for (auto &particle : particles_) {
    PhysicsEngine::ParticleWallCollision(kWindowLength_, kMargin_, particle);
    particle.SetPosition(particle.GetPosition() += particle.GetVelocity());
  }
  // Update every two frames
  if (frames % 2 == 0) {
    UpdateHistograms();
  }
}

void GasContainer::GenerateParticles(vector<idealgas::Particle> &particles,
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
  ci::gl::drawStringCentered("Speed", vec2((kWindowLength_ + kWindowWidth_ - kMargin_)/2, kMargin_/4));
  ci::gl::drawStringCentered("1 / λ", vec2(kWindowWidth_ - kMargin_*0.67, kWindowLength_/2));
  ci::gl::color(kBorderColor_);
  ci::gl::drawStrokedRect(
      ci::Rectf(vec2(kWindowLength_, kMargin_/2),
                vec2(kWindowWidth_ - kMargin_, (kWindowLength_ - 2*kMargin_)/3 + kMargin_/2)), 2);
  ci::gl::drawStrokedRect(
      ci::Rectf(vec2(kWindowLength_, (kWindowLength_ - 2*kMargin_)/3 + kMargin_),
                vec2(kWindowWidth_ - kMargin_, 2*(kWindowLength_ - 2*kMargin_)/3 + kMargin_)), 2);
  ci::gl::drawStrokedRect(
      ci::Rectf(vec2(kWindowLength_, 2*(kWindowLength_ - 2*kMargin_)/3 + 3*kMargin_/2),
                vec2(kWindowWidth_ - kMargin_, kWindowLength_ - kMargin_/2)), 2);
}

void GasContainer::UpdateHistograms() {
  ResetHistograms();
  int max_speed = MaxParticleSpeed();
  for (auto & particle : particles_) {
    for (size_t bin = 0; bin < num_bins_; ++bin) {
      if (particle.GetSpeed() <= max_speed * (static_cast<double>((bin + 1.0) / num_bins_))) {
        if (particle.GetColor() == fast_color_) {
          fast_speeds_[bin] += 1;
        } else if (particle.GetColor() == medium_color_) {
          medium_speeds_[bin] += 1;
        } else if (particle.GetColor() == slow_color_) {
          slow_speeds_[bin] += 1;
        }
        break;
      }
    }
  }

  CalculateMaxHeight();
}

void GasContainer::DisplayHistogram(const glm::vec2 &top_left_corner,
                                    const glm::vec2 &bottom_right_corner,
                                    const ci::Color &color,
                                    std::map<int, int> speeds) const {

  float bin_width = (bottom_right_corner.x - top_left_corner.x) / static_cast<float>(num_bins_);
  for (size_t bin = 0; bin < num_bins_; ++bin) {
    float bin_height_ratio = static_cast<float>(static_cast<float>(speeds[bin])/(max_height_ * 1.0));
    ci::gl::color(color);
    ci::gl::drawStrokedRect(
        ci::Rectf(vec2(top_left_corner.x + bin*bin_width,
                       bottom_right_corner.y - ((bottom_right_corner.y - top_left_corner.y) * bin_height_ratio * 0.95)),
                  vec2(top_left_corner.x + (bin + 1.0) * bin_width, bottom_right_corner.y)), 2);
  }
}

void GasContainer::ResetHistograms() {
  for (size_t bin = 0; bin < num_bins_; ++bin) {
    slow_speeds_[bin] = 0;
    medium_speeds_[bin] = 0;
    fast_speeds_[bin] = 0;
  }
}

int GasContainer::MaxParticleSpeed() const {
  double max_speed = 0;
  for (auto & particle : particles_) {
    double particle_speed = particle.GetSpeed();
    if (particle_speed > max_speed) {
      max_speed = particle_speed;
    }
  }

  return static_cast<int>(max_speed);
}

void GasContainer::SlowDownParticles() {
  for (auto & particle : particles_) {
    particle.SetVelocity(particle.GetVelocity() * glm::vec2(0.5, 0.5));
  }
}

void GasContainer::SpeedUpParticles() {
  for (auto & particle : particles_) {
    particle.SetVelocity(particle.GetVelocity() * glm::vec2(2, 2));
  }
}

std::map<int, int> GasContainer::GetMap(const ci::Color& color) const {
  if (color == fast_color_) {
    return fast_speeds_;
  } else if (color == medium_color_) {
    return medium_speeds_;
  } else {
    return slow_speeds_;
  }
}
void GasContainer::CalculateMaxHeight() {
  int max_height = 0;
  for (auto const& speed : fast_speeds_) {
    if (speed.second > max_height) {
      max_height = speed.second;
    }
  }

  for (auto const& speed : medium_speeds_) {
    if (speed.second > max_height) {
      max_height = speed.second;
    }
  }

  for (auto const& speed : slow_speeds_) {
    if (speed.second > max_height) {
      max_height = speed.second;
    }
  }

  max_height_ = max_height;
}

}  // namespace idealgas

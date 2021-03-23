#include "gas_container.h"
#include "physics_engine.h"

namespace idealgas {

using std::vector;
using glm::vec2;

GasContainer::GasContainer(const int kWindowLength, const int kWindowWidth,
                           const int kMargin,
                           const ci::Color &kBorderColor)
    : kWindowLength_(kWindowLength),
      kWindowWidth_(kWindowWidth),
      kMargin_(kMargin),
      kBorderColor_(kBorderColor) {
  Particle orange_particle(vec2(), vec2(4, 4), 6, 6, ci::Color("orange"));
  Particle red_particle(vec2(), vec2(3, 2), 10, 10, ci::Color("red"));
  Particle green_particle(vec2(), vec2(2, 2), 15, 15, ci::Color("green"));

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
                vec2(kWindowLength_ - kMargin_, kWindowLength_ - kMargin_)), 4);
  GasContainer::DrawHistogramBoxes();

  DisplayHistogram(vec2(kWindowLength_, kMargin_/2),
                   vec2(kWindowWidth_ - kMargin_, (kWindowLength_ - 2*kMargin_)/3 + kMargin_/2),
                  ci::Color("orange"), fast_speeds_);
  DisplayHistogram(vec2(kWindowLength_, (kWindowLength_ - 2*kMargin_)/3 + kMargin_),
                   vec2(kWindowWidth_ - kMargin_, 2*(kWindowLength_ - 2*kMargin_)/3 + kMargin_),
                  ci::Color("red"), medium_speeds_);
  DisplayHistogram(vec2(kWindowLength_, 2*(kWindowLength_ - 2*kMargin_)/3 + 3*kMargin_/2),
                   vec2(kWindowWidth_ - kMargin_, kWindowLength_ - kMargin_/2),
                  ci::Color("green"), slow_speeds_);
}

void GasContainer::AdvanceOneFrame() {
  PhysicsEngine::AdjustVelocitiesOnCollision(particles_);

  for (auto &particle : particles_) {
    PhysicsEngine::ParticleWallCollision(kWindowLength_, kMargin_, particle);
    particle.SetPosition(particle.GetPosition() += particle.GetVelocity());
  }

  UpdateHistograms();
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
  for (size_t bin = 0; bin < num_bins_; ++bin) {
    slow_speeds_[bin] = 0;
    medium_speeds_[bin] = 0;
    fast_speeds_[bin] = 0;
  }

  double max_speed = 0;
  for (size_t i = 0; i < particles_.size(); ++i) {
    double particle_speed = particles_[i].GetSpeed();
    if (particle_speed > max_speed) {
      max_speed = particle_speed;
    }
  }
  max_speed_ = static_cast<int>(max_speed);
  for (size_t i = 0; i < particles_.size(); ++i) {
    for (size_t bin = 0; bin < num_bins_; ++bin) {
      if (particles_[i].GetSpeed() <= max_speed_ * (static_cast<double>((bin + 1.0) / num_bins_))) {
        if (particles_[i].GetColor() == ci::Color("orange")) {
          fast_speeds_[bin] += 1;
        } else if (particles_[i].GetColor() == ci::Color("red")) {
          medium_speeds_[bin] += 1;
        } else if (particles_[i].GetColor() == ci::Color("green")) {
          slow_speeds_[bin] += 1;
        }
        break;
      }
    }
  }
//  double speed_diff = max_speed - min_speed;
//  if (speed_diff == 0) {
//    num_bins = 1;
//  } else if (speed_diff <= 0.8){
//    num_bins = static_cast<int>(speed_diff * 10);
//  } else {
//    num_bins = 8;
//  }
//
//  float bin_width = (bottom_right_corner.x - top_left_corner.x) / static_cast<float>(num_bins);
//  ci::gl::color(color);
//  for (size_t i = 0; i < num_bins; ++i) {
//    ci::gl::drawSolidRect(
//        ci::Rectf(vec2(top_left_corner.x + i*bin_width, top_left_corner.y),
//                  vec2(bottom_right_corner.x + i*bin_width, bottom_right_corner.y)));
//  }
}

void GasContainer::DisplayHistogram(const glm::vec2 &top_left_corner,
                                    const glm::vec2 &bottom_right_corner,
                                    const ci::Color &color,
                                    std::map<int, int> speeds) const {
  int max_height = 0;
//  for (size_t bin = 0; bin < speeds.size(); ++bin) {
//    if (speeds[bin] > max_height) {
//      max_height = speeds[bin];
//    }
//  }
  for (auto const& speed : speeds) {
    if (speed.second > max_height) {
      max_height = speed.second;
    }
  }
  float bin_width = (bottom_right_corner.x - top_left_corner.x) / static_cast<float>(num_bins_);
  for (size_t bin = 0; bin < num_bins_; ++bin) {
    float bin_height_ratio = static_cast<float>(static_cast<float>(speeds[bin])/(max_height * 1.0));
    ci::gl::color(color);
    ci::gl::drawStrokedRect(
        ci::Rectf(vec2(top_left_corner.x + bin*bin_width,
                       bottom_right_corner.y - ((bottom_right_corner.y - top_left_corner.y) * bin_height_ratio)),
                  vec2(top_left_corner.x + (bin + 1.0) * bin_width, bottom_right_corner.y)));
  }
}

}  // namespace idealgas

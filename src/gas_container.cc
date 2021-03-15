#include "gas_container.h"

namespace idealgas {

using glm::vec2;

GasContainer::GasContainer(const int kWindowSize, const int kMargin,
                           const ci::Color kBorderColor) : kWindowSize_(kWindowSize),
                            kMargin_(kMargin), kBorderColor_(kBorderColor) {
    GenerateParticles(particles_);
}

void GasContainer::Display() const {
  // This function has a lot of magic numbers; be sure to design your code in a way that avoids this.
  ci::gl::color(ci::Color("orange"));
  ci::gl::drawSolidCircle(vec2(dummy_variable_, 200), 10);
  ci::gl::color(kBorderColor_);
  ci::gl::drawStrokedRect(ci::Rectf(vec2(kMargin_, kMargin_), vec2(kWindowSize_-kMargin_,
                                                                                kWindowSize_-kMargin_)));
}

void GasContainer::AdvanceOneFrame() {
  ++dummy_variable_;
}

void GasContainer::GenerateParticles(std::vector<idealgas::Particle> &particles,
                                     const Particle &particle, int num) {

}


}  // namespace idealgas

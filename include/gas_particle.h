#pragma once

#include "cinder/gl/gl.h"

namespace idealgas {

/**
 * A gas particle to put in the gas container.
 */
class Particle {
 public:
    Particle(glm::vec2 position, glm::vec2 velocity, int mass, int radius, ci::Color color);

    glm::vec2 GetPosition() const;
    glm::vec2 GetVelocity() const;
    int GetMass() const;
    int GetRadius() const;
    ci::Color GetColor() const;

    void SetPosition(glm::vec2 position);
    void SetVelocity(glm::vec2 velocity);

  private:
    glm::vec2 position_;
    glm::vec2 velocity_;
    int mass_;
    int radius_;
    ci::Color color_;
};

}
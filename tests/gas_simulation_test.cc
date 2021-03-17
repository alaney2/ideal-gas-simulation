#include <catch2/catch.hpp>

#include <gas_container.h>

using idealgas::GasContainer;
using idealgas::Particle;
using glm::vec2;

std::vector<idealgas::Particle> particles_;
const int kWindowSize = 200;
const int kMargin = 0;

TEST_CASE("Particle-particle collisions") {
//  GasContainer container(750,100,"white");
//  container.AdvanceOneFrame();

  Particle particle_moving_right(vec2(100,100), vec2(1,0), 1, 1, "cyan");
  Particle particle_moving_left(vec2(100,100), vec2(-1,0), 1, 1, "cyan");
  Particle particle_moving_up(vec2(100,100), vec2(0,1), 1, 1, "cyan");
  Particle particle_moving_down(vec2(100,100), vec2(0,-1), 1, 1, "cyan");
  Particle particle_moving_diagonally(vec2(100,100), vec2(1,1), 1, 1, "cyan");
  Particle particle_moving_antidiagonally(vec2(100,100), vec2(-1,-1), 1, 1, "cyan");

  SECTION("Collision from right") {
    Particle target_particle(vec2(101,100), vec2(-1,0),1,1,"cyan");
    REQUIRE(GasContainer::DetectCollision(particle_moving_right, target_particle));
  }

}

TEST_CASE("Particles not colliding when moving in different directions") {
  Particle middle_particle(vec2(100,100), vec2(0,0), 1,1,"cyan");

  SECTION("Does not bounce from right") {
    Particle target_particle(vec2(101,100), vec2(1,0), 1, 1, "cyan");

    REQUIRE_FALSE(GasContainer::DetectCollision(middle_particle, target_particle));
  }

}

TEST_CASE("Container wall boundaries") {
  SECTION("Particle does not collide with left wall") {
    Particle particle(vec2(101.1,200), vec2(1, 0), 1, 1, "cyan");
    size_t lower_bound = kMargin + particle.GetRadius();
    size_t upper_bound = kWindowSize - kMargin - particle.GetRadius();
    size_t x_pos = particle.GetPosition().x;

    bool out_of_bounds = x_pos <= lower_bound || x_pos >= upper_bound;
    REQUIRE_FALSE(out_of_bounds);
  }
}

TEST_CASE("Particle colliding with container wall") {
  SECTION("Right wall collision") {
    Particle particle(vec2(199,100), vec2(1,0), 1, 1, "cyan");
  }
}

TEST_CASE("Velocity update after collision") {
  SECTION("Collision from right") {
    Particle particle(vec2(199,100), vec2(-1,0), 1, 1, "cyan");
    size_t lower_bound = kMargin + particle.GetRadius();
    size_t upper_bound = kWindowSize - kMargin - particle.GetRadius();
    size_t x_pos = particle.GetPosition().x;

    bool out_of_bounds = x_pos <= lower_bound || x_pos >= upper_bound;
  }
}

TEST_CASE("Position update after collision") {
  SECTION("Collision with top wall") {
    Particle particle(vec2(100, 0.9), vec2(0,-1), 1, 1, "cyan");
  }
}



/*

You can (and should) create more test files; this project is too big
for all tests to be in the same file. Remember that, for each file (foo.cc)
containing non-trivial code, you should have a corresponding test file
(foo_test.cc)

Make sure to add any files that you create to CMakeLists.txt.

TODO Delete this comment and the placeholder test before submitting your code.
*/

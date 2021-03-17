#include <catch2/catch.hpp>

#include <gas_container.h>

using idealgas::GasContainer;
using idealgas::Particle;
using glm::vec2;

std::vector<idealgas::Particle> particles_;
const int kWindowSize = 200;
const int kMargin = 0;
GasContainer container(kWindowSize, kMargin, "white");

TEST_CASE("Particle-particle collisions") {
  Particle particle_moving_right(vec2(100,100), vec2(1,0), 1, 1, "cyan");
  Particle particle_moving_left(vec2(100,100), vec2(-1,0), 1, 1, "cyan");
  Particle particle_moving_up(vec2(100,100), vec2(0,1), 1, 1, "cyan");
  Particle particle_moving_down(vec2(100,100), vec2(0,-1), 1, 1, "cyan");
  Particle particle_moving_diagonally(vec2(100,100), vec2(1,1), 1, 1, "cyan");
  Particle particle_moving_antidiagonally(vec2(100,100), vec2(-1,-1), 1, 1, "cyan");

  SECTION("Collision from right") {
    Particle target_particle(vec2(101,100), vec2(-1,0),1,1,"cyan");
    REQUIRE(GasContainer::DetectCollision(particle_moving_right, target_particle));

    particle_moving_right.SetVelocity(container.GetVelocityAfterCollision(particle_moving_right, target_particle));
    target_particle.SetVelocity(container.GetVelocityAfterCollision(target_particle, particle_moving_right));

    particle_moving_right.SetPosition(particle_moving_right.GetPosition() + particle_moving_right.GetVelocity());
    target_particle.SetPosition(target_particle.GetPosition() + target_particle.GetVelocity());

    REQUIRE(particle_moving_right.GetPosition().x == 99.0f);
    REQUIRE(particle_moving_right.GetPosition().y == 100.0f);
    REQUIRE(target_particle.GetPosition().x == 100.0f);
    REQUIRE(target_particle.GetPosition().y == 100.0f);
  }

  SECTION("Collision from left") {
    Particle target_particle(vec2(99,100), vec2(1,0),1,1,"cyan");
    REQUIRE(GasContainer::DetectCollision(particle_moving_left, target_particle));

    particle_moving_left.SetVelocity(container.GetVelocityAfterCollision(particle_moving_left, target_particle));
    target_particle.SetVelocity(container.GetVelocityAfterCollision(target_particle, particle_moving_left));

    particle_moving_left.SetPosition(particle_moving_left.GetPosition() + particle_moving_left.GetVelocity());
    target_particle.SetPosition(target_particle.GetPosition() + target_particle.GetVelocity());

    REQUIRE(particle_moving_left.GetPosition().x == 101.0f);
    REQUIRE(particle_moving_left.GetPosition().y == 100.0f);
    REQUIRE(target_particle.GetPosition().x == 100.0f);
    REQUIRE(target_particle.GetPosition().y == 100.0f);
  }

  SECTION("Collision from top") {
    Particle target_particle(vec2(100,101), vec2(0,-1),1,1,"cyan");
    REQUIRE(GasContainer::DetectCollision(particle_moving_up, target_particle));

    particle_moving_up.SetVelocity(container.GetVelocityAfterCollision(particle_moving_up, target_particle));
    target_particle.SetVelocity(container.GetVelocityAfterCollision(target_particle, particle_moving_up));

    particle_moving_up.SetPosition(particle_moving_up.GetPosition() + particle_moving_up.GetVelocity());
    target_particle.SetPosition(target_particle.GetPosition() + target_particle.GetVelocity());

    REQUIRE(particle_moving_up.GetPosition().x == 100.0f);
    REQUIRE(particle_moving_up.GetPosition().y == 99.0f);
    REQUIRE(target_particle.GetPosition().x == 100.0f);
    REQUIRE(target_particle.GetPosition().y == 100.0f);
  }

  SECTION("Collision from bottom") {
    Particle target_particle(vec2(100,99), vec2(0,1),1,1,"cyan");
    REQUIRE(GasContainer::DetectCollision(particle_moving_down, target_particle));

    particle_moving_down.SetVelocity(container.GetVelocityAfterCollision(particle_moving_down, target_particle));
    target_particle.SetVelocity(container.GetVelocityAfterCollision(target_particle, particle_moving_down));

    particle_moving_down.SetPosition(particle_moving_down.GetPosition() + particle_moving_down.GetVelocity());
    target_particle.SetPosition(target_particle.GetPosition() + target_particle.GetVelocity());

    REQUIRE(particle_moving_down.GetPosition().x == 100.0f);
    REQUIRE(particle_moving_down.GetPosition().y == 101.0f);
    REQUIRE(target_particle.GetPosition().x == 100.0f);
    REQUIRE(target_particle.GetPosition().y == 100.0f);
  }

  SECTION("Diagonal collision") {
    Particle target_particle(vec2(101,101), vec2(-1,-1),1,1,"cyan");
    REQUIRE(GasContainer::DetectCollision(particle_moving_diagonally, target_particle));

    particle_moving_diagonally.SetVelocity(container.GetVelocityAfterCollision(particle_moving_diagonally, target_particle));
    target_particle.SetVelocity(container.GetVelocityAfterCollision(target_particle, particle_moving_diagonally));

    particle_moving_diagonally.SetPosition(particle_moving_diagonally.GetPosition() + particle_moving_diagonally.GetVelocity());
    target_particle.SetPosition(target_particle.GetPosition() + target_particle.GetVelocity());

    REQUIRE(particle_moving_diagonally.GetPosition().x == 99.0f);
    REQUIRE(particle_moving_diagonally.GetPosition().y == 99.0f);
    REQUIRE(target_particle.GetPosition().x == 100.0f);
    REQUIRE(target_particle.GetPosition().y == 100.0f);
  }

  SECTION("Antidiagonal collision") {
    Particle target_particle(vec2(99,99), vec2(1,1),1,1,"cyan");
    REQUIRE(GasContainer::DetectCollision(particle_moving_antidiagonally, target_particle));

    particle_moving_antidiagonally.SetVelocity(container.GetVelocityAfterCollision(particle_moving_antidiagonally, target_particle));
    target_particle.SetVelocity(container.GetVelocityAfterCollision(target_particle, particle_moving_antidiagonally));

    particle_moving_antidiagonally.SetPosition(particle_moving_antidiagonally.GetPosition() + particle_moving_antidiagonally.GetVelocity());
    target_particle.SetPosition(target_particle.GetPosition() + target_particle.GetVelocity());

    REQUIRE(particle_moving_antidiagonally.GetPosition().x == 101.0f);
    REQUIRE(particle_moving_antidiagonally.GetPosition().y == 101.0f);
    REQUIRE(target_particle.GetPosition().x == 100.0f);
    REQUIRE(target_particle.GetPosition().y == 100.0f);
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

TEST_CASE("Velocity update after particle collision") {
  SECTION("Collision from right particle") {
    Particle particle_moving_right(vec2(199,100), vec2(-1,0), 1, 1, "cyan");
    Particle target_particle(vec2(101,100), vec2(-1,0), 1, 1, "cyan");
    particle_moving_right.SetVelocity(container.GetVelocityAfterCollision(particle_moving_right, target_particle));
    target_particle.SetVelocity(container.GetVelocityAfterCollision(target_particle, particle_moving_right));

    REQUIRE(particle_moving_right.GetVelocity().x == -1.0);
    REQUIRE(particle_moving_right.GetVelocity().y == 0.0);
    REQUIRE(target_particle.GetVelocity().x == -1.0);
    REQUIRE(target_particle.GetVelocity().y == 0.0);
  }

  SECTION("Collision from left particle") {
    Particle particle_moving_left(vec2(100,100), vec2(-1,0), 1, 1, "cyan");
    Particle target_particle(vec2(99,100), vec2(1,0), 1, 1, "cyan");
    particle_moving_left.SetVelocity(container.GetVelocityAfterCollision(particle_moving_left, target_particle));
    target_particle.SetVelocity(container.GetVelocityAfterCollision(target_particle, particle_moving_left));

    REQUIRE(particle_moving_left.GetVelocity().x == 1.0);
    REQUIRE(particle_moving_left.GetVelocity().y == 0.0);
    REQUIRE(target_particle.GetVelocity().x == 1.0);
    REQUIRE(target_particle.GetVelocity().y == 0.0);
  }

  SECTION("Collision from top particle") {
    Particle particle_moving_up(vec2(100,100), vec2(0,1), 1, 1, "cyan");
    Particle target_particle(vec2(100, 101), vec2(0,-1), 1, 1, "cyan");
    particle_moving_up.SetVelocity(container.GetVelocityAfterCollision(particle_moving_up, target_particle));
    target_particle.SetVelocity(container.GetVelocityAfterCollision(target_particle, particle_moving_up));

    REQUIRE(particle_moving_up.GetVelocity().x == 0.0);
    REQUIRE(particle_moving_up.GetVelocity().y == -1.0);
    REQUIRE(target_particle.GetVelocity().x == 0.0);
    REQUIRE(target_particle.GetVelocity().y == -1.0);
  }

  SECTION("Collision from bottom particle") {
    Particle particle_moving_down(vec2(100,100), vec2(0,-1), 1, 1, "cyan");
    Particle target_particle(vec2(100, 99), vec2(0,1), 1, 1, "cyan");
    particle_moving_down.SetVelocity(container.GetVelocityAfterCollision(particle_moving_down, target_particle));
    target_particle.SetVelocity(container.GetVelocityAfterCollision(target_particle, particle_moving_down));

    REQUIRE(particle_moving_down.GetVelocity().x == 0.0);
    REQUIRE(particle_moving_down.GetVelocity().y == 1.0);
    REQUIRE(target_particle.GetVelocity().x == 0.0);
    REQUIRE(target_particle.GetVelocity().y == 1.0);
  }

  SECTION("Diagonal collision") {
    Particle particle_moving_diagonally(vec2(100,100), vec2(1,1), 1, 1, "cyan");
    Particle target_particle(vec2(101, 101), vec2(-1,-1), 1, 1, "cyan");
    particle_moving_diagonally.SetVelocity(container.GetVelocityAfterCollision(particle_moving_diagonally, target_particle));
    target_particle.SetVelocity(container.GetVelocityAfterCollision(target_particle, particle_moving_diagonally));

    REQUIRE(particle_moving_diagonally.GetVelocity().x == -1.0);
    REQUIRE(particle_moving_diagonally.GetVelocity().y == -1.0);
    REQUIRE(target_particle.GetVelocity().x == -1.0);
    REQUIRE(target_particle.GetVelocity().y == -1.0);
  }

  SECTION("Antidiagonal collision") {
    Particle particle_moving_antidiagonally(vec2(100,100), vec2(-1,-1), 1, 1, "cyan");
    Particle target_particle(vec2(99, 99), vec2(1,1), 1, 1, "cyan");
    particle_moving_antidiagonally.SetVelocity(container.GetVelocityAfterCollision(particle_moving_antidiagonally, target_particle));
    target_particle.SetVelocity(container.GetVelocityAfterCollision(target_particle, particle_moving_antidiagonally));

    REQUIRE(particle_moving_antidiagonally.GetVelocity().x == 1.0);
    REQUIRE(particle_moving_antidiagonally.GetVelocity().y == 1.0);
    REQUIRE(target_particle.GetVelocity().x == 1.0);
    REQUIRE(target_particle.GetVelocity().y == 1.0);
  }
}

TEST_CASE("Position update after wall collision") {
  SECTION("Collision with top wall") {
    Particle particle(vec2(100, 0.9), vec2(0,-1), 1, 1, "cyan");
    container.NegateVelocityOnWallCollision(particle);
    particle.SetPosition(particle.GetPosition() + particle.GetVelocity());

    REQUIRE(particle.GetPosition().x == 100.0f);
    REQUIRE(particle.GetPosition().y == 1.9f);
  }

  SECTION("Collision with bottom wall") {
    Particle particle(vec2(100, 199.1), vec2(0,1), 1, 1, "cyan");
    container.NegateVelocityOnWallCollision(particle);
    particle.SetPosition(particle.GetPosition() + particle.GetVelocity());

    REQUIRE(particle.GetPosition().x == 100.0f);
    REQUIRE(particle.GetPosition().y == 198.1f);
  }

  SECTION("Collision with right wall") {
    Particle particle(vec2(199, 100), vec2(1,0), 1, 1, "cyan");
    container.NegateVelocityOnWallCollision(particle);
    particle.SetPosition(particle.GetPosition() + particle.GetVelocity());

    REQUIRE(particle.GetPosition().x == 198.0f);
    REQUIRE(particle.GetPosition().y == 100.0f);
  }

  SECTION("Collision with left wall") {
    Particle particle(vec2(1, 100), vec2(-1,0), 1, 1, "cyan");
    container.NegateVelocityOnWallCollision(particle);
    particle.SetPosition(particle.GetPosition() + particle.GetVelocity());

    REQUIRE(particle.GetPosition().x == 2.0f);
    REQUIRE(particle.GetPosition().y == 100.0f);
  }

  SECTION("Collision with top left corner") {
    Particle particle(vec2(0, 0), vec2(-1,-1), 1, 1, "cyan");
    container.NegateVelocityOnWallCollision(particle);
    particle.SetPosition(particle.GetPosition() + particle.GetVelocity());

    REQUIRE(particle.GetPosition().x == 1.0f);
    REQUIRE(particle.GetPosition().y == 1.0f);
  }

  SECTION("Collision with top right corner") {
    Particle particle(vec2(200, 0), vec2(1,-1), 1, 1, "cyan");
    container.NegateVelocityOnWallCollision(particle);
    particle.SetPosition(particle.GetPosition() + particle.GetVelocity());

    REQUIRE(particle.GetPosition().x == 199.0f);
    REQUIRE(particle.GetPosition().y == 1.0f);
  }

  SECTION("Collision with bottom left corner") {
    Particle particle(vec2(0, 200), vec2(-1,1), 1, 1, "cyan");
    container.NegateVelocityOnWallCollision(particle);
    particle.SetPosition(particle.GetPosition() + particle.GetVelocity());

    REQUIRE(particle.GetPosition().x == 1.0f);
    REQUIRE(particle.GetPosition().y == 199.0f);
  }

  SECTION("Collision with bottom right corner") {
    Particle particle(vec2(200, 200), vec2(1,1), 1, 1, "cyan");
    container.NegateVelocityOnWallCollision(particle);
    particle.SetPosition(particle.GetPosition() + particle.GetVelocity());

    REQUIRE(particle.GetPosition().x == 199.0f);
    REQUIRE(particle.GetPosition().y == 199.0f);
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

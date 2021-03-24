#include <catch2/catch.hpp>

#include "physics_engine.h"

using idealgas::PhysicsEngine;
using idealgas::Particle;
using glm::vec2;

const int kWindowSize = 200;
const int kMargin = 0;
PhysicsEngine engine;

TEST_CASE("Particle collisions with different mass") {
  SECTION("New velocity after heavy particle collides with light particle") {
    Particle heavy_particle(vec2(100, 100), vec2(1, 0), 5, 5, "cyan");
    Particle light_particle(vec2(102, 100), vec2(-1, 0), 1, 1, "cyan");
    REQUIRE(PhysicsEngine::DetectCollision(heavy_particle, light_particle));

    heavy_particle.SetVelocity(engine.GetVelocityAfterCollision(heavy_particle, light_particle));
    light_particle.SetVelocity(engine.GetVelocityAfterCollision(light_particle, heavy_particle));

    REQUIRE(heavy_particle.GetVelocity().x == Approx(0.33333f));
    REQUIRE(heavy_particle.GetVelocity().y == 0.0f);
    REQUIRE(light_particle.GetVelocity().x == Approx(1.22222f));
    REQUIRE(light_particle.GetVelocity().y == 0);
  }

  SECTION("New position after heavy particle collides with light particle") {
    Particle heavy_particle(vec2(100, 100), vec2(1, 0), 5, 5, "cyan");
    Particle light_particle(vec2(102, 100), vec2(-1, 0), 1, 1, "cyan");
    REQUIRE(PhysicsEngine::DetectCollision(heavy_particle, light_particle));

    heavy_particle.SetVelocity(engine.GetVelocityAfterCollision(heavy_particle, light_particle));
    light_particle.SetVelocity(engine.GetVelocityAfterCollision(light_particle, heavy_particle));

    heavy_particle.SetPosition(heavy_particle.GetPosition() + heavy_particle.GetVelocity());
    light_particle.SetPosition(light_particle.GetPosition() + light_particle.GetVelocity());

    REQUIRE(heavy_particle.GetPosition().x == Approx(100.33334f));
    REQUIRE(heavy_particle.GetPosition().y == 100);
    REQUIRE(light_particle.GetPosition().x == Approx(103.22222f));
    REQUIRE(light_particle.GetPosition().y == 100);
  }
}

TEST_CASE("Particle-particle collisions") {
  Particle particle_moving_right(vec2(100,100), vec2(1,0), 1, 1, "cyan");
  Particle particle_moving_left(vec2(100,100), vec2(-1,0), 1, 1, "cyan");
  Particle particle_moving_up(vec2(100,100), vec2(0,1), 1, 1, "cyan");
  Particle particle_moving_down(vec2(100,100), vec2(0,-1), 1, 1, "cyan");
  Particle particle_moving_diagonally(vec2(100,100), vec2(1,1), 1, 1, "cyan");
  Particle particle_moving_antidiagonally(vec2(100,100), vec2(-1,-1), 1, 1, "cyan");

  SECTION("Collision from right") {
    Particle target_particle(vec2(101,100), vec2(-1,0),1,1,"cyan");
    REQUIRE(PhysicsEngine::DetectCollision(particle_moving_right, target_particle));

    particle_moving_right.SetVelocity(engine.GetVelocityAfterCollision(particle_moving_right, target_particle));
    target_particle.SetVelocity(engine.GetVelocityAfterCollision(target_particle, particle_moving_right));

    particle_moving_right.SetPosition(particle_moving_right.GetPosition() + particle_moving_right.GetVelocity());
    target_particle.SetPosition(target_particle.GetPosition() + target_particle.GetVelocity());

    REQUIRE(particle_moving_right.GetPosition().x == 99.0f);
    REQUIRE(particle_moving_right.GetPosition().y == 100.0f);
    REQUIRE(target_particle.GetPosition().x == 100.0f);
    REQUIRE(target_particle.GetPosition().y == 100.0f);
  }

  SECTION("Collision from left") {
    Particle target_particle(vec2(99,100), vec2(1,0),1,1,"cyan");
    REQUIRE(PhysicsEngine::DetectCollision(particle_moving_left, target_particle));

    particle_moving_left.SetVelocity(engine.GetVelocityAfterCollision(particle_moving_left, target_particle));
    target_particle.SetVelocity(engine.GetVelocityAfterCollision(target_particle, particle_moving_left));

    particle_moving_left.SetPosition(particle_moving_left.GetPosition() + particle_moving_left.GetVelocity());
    target_particle.SetPosition(target_particle.GetPosition() + target_particle.GetVelocity());

    REQUIRE(particle_moving_left.GetPosition().x == 101.0f);
    REQUIRE(particle_moving_left.GetPosition().y == 100.0f);
    REQUIRE(target_particle.GetPosition().x == 100.0f);
    REQUIRE(target_particle.GetPosition().y == 100.0f);
  }

  SECTION("Collision from top") {
    Particle target_particle(vec2(100,101), vec2(0,-1),1,1,"cyan");
    REQUIRE(PhysicsEngine::DetectCollision(particle_moving_up, target_particle));

    particle_moving_up.SetVelocity(engine.GetVelocityAfterCollision(particle_moving_up, target_particle));
    target_particle.SetVelocity(engine.GetVelocityAfterCollision(target_particle, particle_moving_up));

    particle_moving_up.SetPosition(particle_moving_up.GetPosition() + particle_moving_up.GetVelocity());
    target_particle.SetPosition(target_particle.GetPosition() + target_particle.GetVelocity());

    REQUIRE(particle_moving_up.GetPosition().x == 100.0f);
    REQUIRE(particle_moving_up.GetPosition().y == 99.0f);
    REQUIRE(target_particle.GetPosition().x == 100.0f);
    REQUIRE(target_particle.GetPosition().y == 100.0f);
  }

  SECTION("Collision from bottom") {
    Particle target_particle(vec2(100,99), vec2(0,1),1,1,"cyan");
    REQUIRE(PhysicsEngine::DetectCollision(particle_moving_down, target_particle));

    particle_moving_down.SetVelocity(engine.GetVelocityAfterCollision(particle_moving_down, target_particle));
    target_particle.SetVelocity(engine.GetVelocityAfterCollision(target_particle, particle_moving_down));

    particle_moving_down.SetPosition(particle_moving_down.GetPosition() + particle_moving_down.GetVelocity());
    target_particle.SetPosition(target_particle.GetPosition() + target_particle.GetVelocity());

    REQUIRE(particle_moving_down.GetPosition().x == 100.0f);
    REQUIRE(particle_moving_down.GetPosition().y == 101.0f);
    REQUIRE(target_particle.GetPosition().x == 100.0f);
    REQUIRE(target_particle.GetPosition().y == 100.0f);
  }

  SECTION("Diagonal collision") {
    Particle target_particle(vec2(101,101), vec2(-1,-1),1,1,"cyan");
    REQUIRE(PhysicsEngine::DetectCollision(particle_moving_diagonally, target_particle));

    particle_moving_diagonally.SetVelocity(engine.GetVelocityAfterCollision(particle_moving_diagonally, target_particle));
    target_particle.SetVelocity(engine.GetVelocityAfterCollision(target_particle, particle_moving_diagonally));

    particle_moving_diagonally.SetPosition(particle_moving_diagonally.GetPosition() + particle_moving_diagonally.GetVelocity());
    target_particle.SetPosition(target_particle.GetPosition() + target_particle.GetVelocity());

    REQUIRE(particle_moving_diagonally.GetPosition().x == 99.0f);
    REQUIRE(particle_moving_diagonally.GetPosition().y == 99.0f);
    REQUIRE(target_particle.GetPosition().x == 100.0f);
    REQUIRE(target_particle.GetPosition().y == 100.0f);
  }

  SECTION("Antidiagonal collision") {
    Particle target_particle(vec2(99,99), vec2(1,1),1,1,"cyan");
    REQUIRE(PhysicsEngine::DetectCollision(particle_moving_antidiagonally, target_particle));

    particle_moving_antidiagonally.SetVelocity(engine.GetVelocityAfterCollision(particle_moving_antidiagonally, target_particle));
    target_particle.SetVelocity(engine.GetVelocityAfterCollision(target_particle, particle_moving_antidiagonally));

    particle_moving_antidiagonally.SetPosition(particle_moving_antidiagonally.GetPosition() + particle_moving_antidiagonally.GetVelocity());
    target_particle.SetPosition(target_particle.GetPosition() + target_particle.GetVelocity());

    REQUIRE(particle_moving_antidiagonally.GetPosition().x == 101.0f);
    REQUIRE(particle_moving_antidiagonally.GetPosition().y == 101.0f);
    REQUIRE(target_particle.GetPosition().x == 100.0f);
    REQUIRE(target_particle.GetPosition().y == 100.0f);
  }
}

TEST_CASE("Particles not colliding when not moving towards each other") {
  // Particle to compare with other particles.
  Particle particle(vec2(100,100), vec2(0,0), 1,1,"cyan");

  SECTION("Does not bounce from right") {
    Particle target_particle(vec2(101,100), vec2(1,0), 1, 1, "cyan");

    REQUIRE(PhysicsEngine::DetectCollision(particle, target_particle) == false);
  }

  SECTION("Does not bounce from left") {
    Particle target_particle(vec2(99,100), vec2(-1,0), 1, 1, "cyan");

    REQUIRE(PhysicsEngine::DetectCollision(particle, target_particle) == false);
  }

  SECTION("Does not bounce from top") {
    Particle target_particle(vec2(100,99), vec2(0,-1), 1, 1, "cyan");

    REQUIRE(PhysicsEngine::DetectCollision(particle, target_particle) == false);
  }

  SECTION("Does not bounce from bottom") {
    Particle target_particle(vec2(100,101), vec2(0,1), 1, 1, "cyan");

    REQUIRE(PhysicsEngine::DetectCollision(particle, target_particle) == false);
  }

  SECTION("Does not bounce diagonally") {
    Particle target_particle(vec2(101,99), vec2(1,-1), 1, 1, "cyan");

    REQUIRE(PhysicsEngine::DetectCollision(particle, target_particle) == false);
  }

  SECTION("Does not bounce antidiagonally") {
    Particle target_particle(vec2(99,101), vec2(-1,1), 1, 1, "cyan");

    REQUIRE(PhysicsEngine::DetectCollision(particle, target_particle) == false);
  }

  SECTION("Does not when still") {
    Particle target_particle(vec2(100,101), vec2(0,0), 1, 1, "cyan");

    REQUIRE(PhysicsEngine::DetectCollision(particle, target_particle) == false);
  }
}

TEST_CASE("Container wall boundaries") {
  SECTION("Particle does not collide with right wall") {
    Particle particle(vec2(198.9,100), vec2(0, 0), 1, 1, "cyan");
    size_t lower_bound = kMargin + particle.GetRadius();
    size_t upper_bound = kWindowSize - kMargin - particle.GetRadius();
    double x_pos = particle.GetPosition().x;

    bool out_of_bounds = x_pos <= lower_bound || x_pos >= upper_bound;
    REQUIRE(out_of_bounds == false);
  }

  SECTION("Particle does not collide with left wall") {
    Particle particle(vec2(1.1,100), vec2(0, 0), 1, 1, "cyan");
    std::cout << particle.GetPosition() << std::endl;
    size_t lower_bound = kMargin + particle.GetRadius();
    size_t upper_bound = kWindowSize - kMargin - particle.GetRadius();
    double x_pos = particle.GetPosition().x;

    bool out_of_bounds = x_pos <= lower_bound || x_pos >= upper_bound;
    REQUIRE(out_of_bounds == false);
  }

  SECTION("Particle does not collide with top wall") {
    Particle particle(vec2(100,198.9), vec2(0, 0), 1, 1, "cyan");
    size_t lower_bound = kMargin + particle.GetRadius();
    size_t upper_bound = kWindowSize - kMargin - particle.GetRadius();
    double y_pos = particle.GetPosition().y;

    bool out_of_bounds = y_pos <= lower_bound || y_pos >= upper_bound;
    REQUIRE(out_of_bounds == false);
  }

  SECTION("Particle does not collide with bottom wall") {
    Particle particle(vec2(100,198.9), vec2(0, 0), 1, 1, "cyan");
    size_t lower_bound = kMargin + particle.GetRadius();
    size_t upper_bound = kWindowSize - kMargin - particle.GetRadius();
    double y_pos = particle.GetPosition().y;

    bool out_of_bounds = y_pos <= lower_bound || y_pos >= upper_bound;
    REQUIRE(out_of_bounds == false);
  }
}

TEST_CASE("Particle colliding with container wall") {
  SECTION("Right wall collision") {
    Particle particle(vec2(199,100), vec2(0,0), 1, 1, "cyan");
    size_t lower_bound = kMargin + particle.GetRadius();
    size_t upper_bound = kWindowSize - kMargin - particle.GetRadius();
    double x_pos = particle.GetPosition().x;

    bool out_of_bounds = x_pos <= lower_bound || x_pos >= upper_bound;
    REQUIRE(out_of_bounds);
  }

  SECTION("Left wall collision") {
    Particle particle(vec2(1,100), vec2(0, 0), 1, 1, "cyan");
    size_t lower_bound = kMargin + particle.GetRadius();
    size_t upper_bound = kWindowSize - kMargin - particle.GetRadius();
    double x_pos = particle.GetPosition().x;

    bool out_of_bounds = x_pos <= lower_bound || x_pos >= upper_bound;
    REQUIRE(out_of_bounds);
  }

  SECTION("Top wall collision") {
    Particle particle(vec2(100,199), vec2(0, 0), 1, 1, "cyan");
    size_t lower_bound = kMargin + particle.GetRadius();
    size_t upper_bound = kWindowSize - kMargin - particle.GetRadius();
    double y_pos = particle.GetPosition().y;

    bool out_of_bounds = y_pos <= lower_bound || y_pos >= upper_bound;
    REQUIRE(out_of_bounds);
  }

  SECTION("Bottom wall collision") {
    Particle particle(vec2(100,1), vec2(0, 0), 1, 1, "cyan");
    size_t lower_bound = kMargin + particle.GetRadius();
    size_t upper_bound = kWindowSize - kMargin - particle.GetRadius();
    double y_pos = particle.GetPosition().y;

    bool out_of_bounds = y_pos <= lower_bound || y_pos >= upper_bound;
    REQUIRE(out_of_bounds);
  }
}

TEST_CASE("Velocity update after particle collision") {
  SECTION("No collision") {
    Particle particle_moving_right(vec2(199,100), vec2(-1,0), 1, 1, "cyan");
    Particle target_particle(vec2(101,100), vec2(-1,0), 1, 1, "cyan");
    particle_moving_right.SetVelocity(engine.GetVelocityAfterCollision(particle_moving_right, target_particle));
    target_particle.SetVelocity(engine.GetVelocityAfterCollision(target_particle, particle_moving_right));

    REQUIRE(particle_moving_right.GetVelocity().x == -1.0);
    REQUIRE(particle_moving_right.GetVelocity().y == 0.0);
    REQUIRE(target_particle.GetVelocity().x == -1.0);
    REQUIRE(target_particle.GetVelocity().y == 0.0);
  }

  SECTION("Collision from left particle") {
    Particle particle_moving_left(vec2(100,100), vec2(-1,0), 1, 1, "cyan");
    Particle target_particle(vec2(99,100), vec2(1,0), 1, 1, "cyan");
    particle_moving_left.SetVelocity(engine.GetVelocityAfterCollision(particle_moving_left, target_particle));
    target_particle.SetVelocity(engine.GetVelocityAfterCollision(target_particle, particle_moving_left));

    REQUIRE(particle_moving_left.GetVelocity().x == 1.0);
    REQUIRE(particle_moving_left.GetVelocity().y == 0.0);
    REQUIRE(target_particle.GetVelocity().x == 1.0);
    REQUIRE(target_particle.GetVelocity().y == 0.0);
  }

  SECTION("Collision from top particle") {
    Particle particle_moving_up(vec2(100,100), vec2(0,1), 1, 1, "cyan");
    Particle target_particle(vec2(100, 101), vec2(0,-1), 1, 1, "cyan");
    particle_moving_up.SetVelocity(engine.GetVelocityAfterCollision(particle_moving_up, target_particle));
    target_particle.SetVelocity(engine.GetVelocityAfterCollision(target_particle, particle_moving_up));

    REQUIRE(particle_moving_up.GetVelocity().x == 0.0);
    REQUIRE(particle_moving_up.GetVelocity().y == -1.0);
    REQUIRE(target_particle.GetVelocity().x == 0.0);
    REQUIRE(target_particle.GetVelocity().y == -1.0);
  }

  SECTION("Collision from bottom particle") {
    Particle particle_moving_down(vec2(100,100), vec2(0,-1), 1, 1, "cyan");
    Particle target_particle(vec2(100, 99), vec2(0,1), 1, 1, "cyan");
    particle_moving_down.SetVelocity(engine.GetVelocityAfterCollision(particle_moving_down, target_particle));
    target_particle.SetVelocity(engine.GetVelocityAfterCollision(target_particle, particle_moving_down));

    REQUIRE(particle_moving_down.GetVelocity().x == 0.0);
    REQUIRE(particle_moving_down.GetVelocity().y == 1.0);
    REQUIRE(target_particle.GetVelocity().x == 0.0);
    REQUIRE(target_particle.GetVelocity().y == 1.0);
  }

  SECTION("Diagonal collision") {
    Particle particle_moving_diagonally(vec2(100,100), vec2(1,1), 1, 1, "cyan");
    Particle target_particle(vec2(101, 101), vec2(-1,-1), 1, 1, "cyan");
    particle_moving_diagonally.SetVelocity(engine.GetVelocityAfterCollision(particle_moving_diagonally, target_particle));
    target_particle.SetVelocity(engine.GetVelocityAfterCollision(target_particle, particle_moving_diagonally));

    REQUIRE(particle_moving_diagonally.GetVelocity().x == -1.0);
    REQUIRE(particle_moving_diagonally.GetVelocity().y == -1.0);
    REQUIRE(target_particle.GetVelocity().x == -1.0);
    REQUIRE(target_particle.GetVelocity().y == -1.0);
  }

  SECTION("Antidiagonal collision") {
    Particle particle_moving_antidiagonally(vec2(100,100), vec2(-1,-1), 1, 1, "cyan");
    Particle target_particle(vec2(99, 99), vec2(1,1), 1, 1, "cyan");
    particle_moving_antidiagonally.SetVelocity(engine.GetVelocityAfterCollision(particle_moving_antidiagonally, target_particle));
    target_particle.SetVelocity(engine.GetVelocityAfterCollision(target_particle, particle_moving_antidiagonally));

    REQUIRE(particle_moving_antidiagonally.GetVelocity().x == 1.0);
    REQUIRE(particle_moving_antidiagonally.GetVelocity().y == 1.0);
    REQUIRE(target_particle.GetVelocity().x == 1.0);
    REQUIRE(target_particle.GetVelocity().y == 1.0);
  }
}

TEST_CASE("Position update after wall collision") {
  SECTION("Collision with top wall") {
    Particle particle(vec2(100, 0.9), vec2(0,-1), 1, 1, "cyan");
    engine.ParticleWallCollision(kWindowSize, kMargin, particle);
    particle.SetPosition(particle.GetPosition() + particle.GetVelocity());

    REQUIRE(particle.GetPosition().x == 100.0f);
    REQUIRE(particle.GetPosition().y == 1.9f);
  }

  SECTION("Collision with bottom wall") {
    Particle particle(vec2(100, 199.1), vec2(0,1), 1, 1, "cyan");
    engine.ParticleWallCollision(kWindowSize, kMargin, particle);
    particle.SetPosition(particle.GetPosition() + particle.GetVelocity());

    REQUIRE(particle.GetPosition().x == 100.0f);
    REQUIRE(particle.GetPosition().y == 198.1f);
  }

  SECTION("Collision with right wall") {
    Particle particle(vec2(199, 100), vec2(1,0), 1, 1, "cyan");
    engine.ParticleWallCollision(kWindowSize, kMargin, particle);
    particle.SetPosition(particle.GetPosition() + particle.GetVelocity());

    REQUIRE(particle.GetPosition().x == 198.0f);
    REQUIRE(particle.GetPosition().y == 100.0f);
  }

  SECTION("Collision with left wall") {
    Particle particle(vec2(1, 100), vec2(-1,0), 1, 1, "cyan");
    engine.ParticleWallCollision(kWindowSize, kMargin, particle);
    particle.SetPosition(particle.GetPosition() + particle.GetVelocity());

    REQUIRE(particle.GetPosition().x == 2.0f);
    REQUIRE(particle.GetPosition().y == 100.0f);
  }

  SECTION("Collision with top left corner") {
    Particle particle(vec2(0, 0), vec2(-1,-1), 1, 1, "cyan");
    engine.ParticleWallCollision(kWindowSize, kMargin, particle);
    particle.SetPosition(particle.GetPosition() + particle.GetVelocity());

    REQUIRE(particle.GetPosition().x == 1.0f);
    REQUIRE(particle.GetPosition().y == 1.0f);
  }

  SECTION("Collision with top right corner") {
    Particle particle(vec2(200, 0), vec2(1,-1), 1, 1, "cyan");
    engine.ParticleWallCollision(kWindowSize, kMargin, particle);
    particle.SetPosition(particle.GetPosition() + particle.GetVelocity());

    REQUIRE(particle.GetPosition().x == 199.0f);
    REQUIRE(particle.GetPosition().y == 1.0f);
  }

  SECTION("Collision with bottom left corner") {
    Particle particle(vec2(0, 200), vec2(-1,1), 1, 1, "cyan");
    engine.ParticleWallCollision(kWindowSize, kMargin, particle);
    particle.SetPosition(particle.GetPosition() + particle.GetVelocity());

    REQUIRE(particle.GetPosition().x == 1.0f);
    REQUIRE(particle.GetPosition().y == 199.0f);
  }

  SECTION("Collision with bottom right corner") {
    Particle particle(vec2(200, 200), vec2(1,1), 1, 1, "cyan");
    engine.ParticleWallCollision(kWindowSize, kMargin, particle);
    particle.SetPosition(particle.GetPosition() + particle.GetVelocity());

    REQUIRE(particle.GetPosition().x == 199.0f);
    REQUIRE(particle.GetPosition().y == 199.0f);
  }
}


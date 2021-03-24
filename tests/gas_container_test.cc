#include <gas_container.h>

#include <catch2/catch.hpp>

#include "gas_particle.h"

namespace idealgas {

TEST_CASE("Map size update") {
  GasContainer container(1000,1000,200,"white");
  std::vector<Particle> particles_;

  Particle particle(glm::vec2{20,20}, glm::vec2{2,2}, 5, 5, "orange");
  Particle particle_two(glm::vec2{20,20}, glm::vec2{2,2}, 5, 5, "orange");

  SECTION("Single orange particle") {
    particles_.push_back(particle);
    container.UpdateHistograms();
    std::map<int, int> histogram_map_ = container.GetMap("orange");
    REQUIRE(histogram_map_.size() == 12);
    for (auto const& i : histogram_map_) {
      if (i.first == 12) {
        REQUIRE(i.second == 1);
      } else {
        REQUIRE(i.second == 0);
      }
    }
  }

  SECTION("Multiple orange particles") {
    particles_.clear();
    for (size_t i = 0; i < 12; ++i) {
      particles_.push_back(particle);
    }
    container.UpdateHistograms();
    std::map<int, int> histogram_map_ = container.GetMap("orange");
    for (auto const& i : histogram_map_) {
      if (i.first == 12) {
        REQUIRE(i.second == 8);
      } else {
        REQUIRE(i.second == 0);
      }
    }
  }
}
}
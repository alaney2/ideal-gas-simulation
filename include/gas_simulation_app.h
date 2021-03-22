#pragma once

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "gas_container.h"

namespace idealgas {

/**
 * An app for visualizing the behavior of an ideal gas.
 */
class IdealGasApp : public ci::app::App {
 public:
  IdealGasApp();

  void draw() override;
  void update() override;

  const int kWindowLength = 750;
  const int kWindowWidth = static_cast<int>(1.5 * kWindowLength);
  const int kMargin = 75;
  const ci::Color kBorderColor = ci::Color("white");

 private:
  GasContainer container_; // The gas container for the particles to move in.
};

}  // namespace idealgas

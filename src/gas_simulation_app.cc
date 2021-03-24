#include "gas_simulation_app.h"

namespace idealgas {


IdealGasApp::IdealGasApp() : container_(kWindowLength, kWindowWidth,
                 kMargin, kBorderColor) {
    ci::app::setWindowSize(kWindowWidth, kWindowLength);
}

void IdealGasApp::draw() {
  ci::Color background_color("black");
  ci::gl::clear(background_color);

  container_.Display();
}

void IdealGasApp::update() {
  container_.AdvanceOneFrame();
}

void IdealGasApp::keyDown(cinder::app::KeyEvent event) {
  if (event.getCode() == cinder::app::KeyEvent::KEY_UP) {
    container_.SpeedUpParticles();
  }
  if (event.getCode() == cinder::app::KeyEvent::KEY_DOWN) {
    container_.SlowDownParticles();
  }
}

}  // namespace idealgas

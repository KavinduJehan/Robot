#include <webots/Robot.hpp>
#include <webots/Motor.hpp>
#include <webots/Camera.hpp>

#define TIME_STEP 32
#define SPEED 3.0

using namespace webots;

int main() {
  Robot robot;
  Camera *camera = robot.getCamera("camera");
  Motor *leftMotor = robot.getMotor("left_motor");
  Motor *rightMotor = robot.getMotor("right_motor");

  camera->enable(TIME_STEP);
  leftMotor->setPosition(INFINITY);
  rightMotor->setPosition(INFINITY);

  while (robot.step(TIME_STEP) != -1) {
    const unsigned char *image = camera->getImage();

    int redSum = 0;
    int blackCount = 0;
    int whiteCount = 0;
    
    for (int x = 0; x < camera->getWidth(); ++x) {
      int r = Camera::imageGetRed(image, camera->getWidth(), x, camera->getHeight()/2);
      int g = Camera::imageGetGreen(image, camera->getWidth(), x, camera->getHeight()/2);
      int b = Camera::imageGetBlue(image, camera->getWidth(), x, camera->getHeight()/2);

      if (r > 150 && g < 100 && b < 100) redSum++;
      else if (r < 50 && g < 50 && b < 50) blackCount++;
      else whiteCount++;
    }

    // Stop at red line
    if (redSum > camera->getWidth() / 4) {
      leftMotor->setVelocity(0);
      rightMotor->setVelocity(0);
      break;
    }

    // Obstacle avoidance (simplified)
    if (blackCount > camera->getWidth() * 0.6) {
      // turn right
      leftMotor->setVelocity(SPEED);
      rightMotor->setVelocity(0.5 * SPEED);
    } else {
      // follow path
      leftMotor->setVelocity(SPEED);
      rightMotor->setVelocity(SPEED);
    }
  }

  return 0;
}

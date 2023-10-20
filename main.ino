#include "cube.h"
#include "animations.h"

animation *animations[4];

int animation_index = 0;

int lastState;
uint last_update = 0;

void setup() {

  // initialize all animations
  animations[0] = new wave_animation(0.2f, 4.0f, 3);
  animations[1] = new strobe_animation(100);
  animations[2] = new sphere_animation(3, 3, 3);
  animations[3] = new rainbow_animation();

  setup_cube();

  // configure button
  pinMode(0, INPUT);
  lastState = digitalRead(0);
}


void loop() {

  // update current animation
  animations[animation_index]->tick();

  // on button pressed
  if (digitalRead(0) != lastState && !digitalRead(0)) {
    animation_index++;

    if (animation_index == 4)
      animation_index = 0;
  }
  lastState = digitalRead(0);

  render_cube();
}

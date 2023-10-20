#pragma once

// super class for animation objects
class animation {
protected:
  float step_speed = 0.01;

public:

  animation() {}

  virtual void tick() {}

  void increment_speed(float val) {
    step_speed += val;
  }
};


class strobe_animation : public animation {
private:
  int step = 0, start_del = 0, dir = 1;

public:
  strobe_animation(int delay_s) {
    this->step_speed = delay_s;
  }

  void tick() {

    if (step == DIMENSIONS - 1)
      dir = -1;
    else if (step <= 0)
      dir = 1;

    // delay
    if (millis() > start_del + step_speed) {

      clear();

      // fill whole plane
      for (int z = 0; z < DIMENSIONS; z++)
        for (int i = 0; i < DIMENSIONS; i++) {
          set_pixel(i, step, z, CRGB(125, 125, 125));
        }

      step += dir;

      start_del = millis();
    }
  }
};


class wave_animation : public animation {
private:

  float step = 0;
  float amplitude = 2;
  int D = 0;

public:

  wave_animation(float speed, float amplitude, int D) {
    this->step_speed = speed;
    this->amplitude = amplitude;
    this->D = D;
  }

  void tick() {

    if (step >= 1000)
      step = 0;

    clear();

    for (int x = 0; x < DIMENSIONS; x++) {
      for (int z = 0; z < DIMENSIONS; z++) {

        int y = sin(sqrtf((float)((x - 3) * (x - 3) + (z - 3) * (z - 3))) * 0.8 + step) * amplitude + D;
        set_pixel(x, y, z, CRGB(255 * (DIMENSIONS * 1 / (float)x), 255 * (DIMENSIONS * 1 / (float)y), 255 * (DIMENSIONS * 1 / (float)z)));
      }
    }

    step += step_speed;
  }
};


class sphere_animation : public animation {
private:

  int center[3];
  float radius = 0;

  void draw_sphere(float radius, char r, char g, char b) {

    for (int i = 0; i < radius; i++)
      for (int j = 0; j < radius; j++)
        for (int k = 0; k < radius; k++) {

          int x = i + center[0] - radius, y = j + center[1] - radius, z = k + center[2] - radius;
          float distance = sqrtf(pow(center[0] - x, 2) + pow(center[1] - y, 2) + pow(center[2] - z, 2));

          if (x < DIMENSIONS && z < DIMENSIONS && y < DIMENSIONS && x >= 0 && y >= 0 && z >= 0) {
            if (distance <= radius) {

              float inv_distance = (radius - distance);

              // intensity based on how close to center
              char red = inv_distance * ((float)r / radius);
              char green = inv_distance * ((float)g / radius);
              char blue = inv_distance * ((float)b / radius);

              CRGB color(red, green, blue);

              add_color_pixel(x, y, z, color);
              add_color_pixel(radius * 2 - x - 1, y, z, color);
              add_color_pixel(x, y, radius * 2 - z - 1, color);
              add_color_pixel(radius * 2 - x - 1, y, radius * 2 - z - 1, color);

              add_color_pixel(x, radius * 2 - y - 1, z, color);
              add_color_pixel(radius * 2 - x - 1, y, z, color);
              add_color_pixel(x, radius * 2 - y - 1, radius * 2 - z - 1, color);
              add_color_pixel(radius * 2 - x - 1, radius * 2 - y - 1, radius * 2 - z - 1, color); /* */
            }
          }
        }
  }

public:

  sphere_animation(char x, char y, char z) {
    this->center[0] = x;
    this->center[1] = y;
    this->center[2] = z;
  }

  void tick() {

    clear();

    draw_sphere(abs(sin(radius)) * DIMENSIONS / 2, 255, 0, 0);
    draw_sphere(abs(sin(radius + 3.14 / 3)) * DIMENSIONS / 2, 0, 255, 0);
    draw_sphere(abs(sin(radius + 3.14 / 3 * 2)) * DIMENSIONS / 2, 0, 0, 255);

    radius += step_speed;
  }
};


class rainbow_animation : public animation {
private:

  float const_r_point[3] = { -DIMENSIONS / 2.0f, -DIMENSIONS / 2.0f, -DIMENSIONS / 2.0f };
  float const_g_point[3] = { 0, DIMENSIONS / 2.0f, DIMENSIONS / 2.0f };
  float const_b_point[3] = { DIMENSIONS / 2.0f, -DIMENSIONS / 2.0f, -DIMENSIONS / 2.0f };

  float r_point[3] = { 0.0f, 0.0f, 0.0f };
  float g_point[3] = { 0.0f, 0.0f, 0.0f };
  float b_point[3] = { 0.0f, 0.0f, 0.0f };

  float theta = 0;

  void rotate_at_y_axis(float *vector, float *transformed_vec) {

    transformed_vec[1] = vector[1] + (DIMENSIONS / 2.0f);
    transformed_vec[0] = cos(theta) * vector[0] + vector[2] * sin(theta) + (DIMENSIONS / 2.0f);
    transformed_vec[2] = sin(theta) * -vector[0] + vector[2] * cos(theta) + (DIMENSIONS / 2.0f);
  }


  void rotate_at_x_axis(float *vector, float *transformed_vec) {

    transformed_vec[0] = vector[0] + (DIMENSIONS / 2.0f);
    transformed_vec[1] = cos(theta) * vector[1] - vector[2] * sin(theta) + (DIMENSIONS / 2.0f);
    transformed_vec[2] = sin(theta) * vector[1] + vector[2] * cos(theta) + (DIMENSIONS / 2.0f);
  }

public:

  rainbow_animation() {}

  void tick() {

    rotate_at_y_axis(const_r_point, r_point);
    rotate_at_y_axis(const_g_point, g_point);
    rotate_at_y_axis(const_b_point, b_point);

    rotate_at_x_axis(const_r_point, r_point);
    rotate_at_x_axis(const_g_point, g_point);
    rotate_at_x_axis(const_b_point, b_point);

    /* */
    for (int i = 0; i < DIMENSIONS; i++)
      for (int j = 0; j < DIMENSIONS; j++)
        for (int k = 0; k < DIMENSIONS; k++) {

          const float scale = 255.0f / pow(DIMENSIONS,2) ;
          
          CRGB color(scale * (abs(r_point[0] - i) + abs(r_point[1] - j) + abs(r_point[2] - k)),
                     scale * (abs(g_point[0] - i) + abs(g_point[1] - j) + abs(g_point[2] - k)),
                     scale * (abs(b_point[0] - i) + abs(b_point[1] - j) + abs(b_point[2] - k)));

          set_pixel(i, j, k, color);
        }

    theta += step_speed;
  }
};

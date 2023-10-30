#pragma once
#define MAX_POINTS 4

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

    if (step == DIMENSION - 1)
      dir = -1;
    else if (step <= 0)
      dir = 1;

    // delay
    if (millis() > start_del + step_speed) {

      clear(CRGB(0,0,0));

      // fill whole plane
      for (int z = 0; z < DIMENSION; z++)
        for (int i = 0; i < DIMENSION; i++) {
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

    clear(CRGB(0,0,0));

    for (int x = 0; x < DIMENSION; x++) {
      for (int z = 0; z < DIMENSION; z++) {
        
        int y = sin(sqrtf((float)((x - 3) * (x - 3) + (z - 3) * (z - 3))) * 0.8 + step) * amplitude + D;
        set_pixel(x, y, z, CRGB(255 * (DIMENSION * 1 / (float)x), 255 * (DIMENSION * 1 / (float)y), 255 * (DIMENSION * 1 / (float)z)));
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

          if (x < DIMENSION && z < DIMENSION && y < DIMENSION && x >= 0 && y >= 0 && z >= 0) {
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

    clear(CRGB(0,0,0);

    draw_sphere(abs(sin(radius)) * DIMENSION / 2, 255, 0, 0);
    draw_sphere(abs(sin(radius + 3.14 / 3)) * DIMENSION / 2, 0, 255, 0);
    draw_sphere(abs(sin(radius + 3.14 / 3 * 2)) * DIMENSION / 2, 0, 0, 255);

    radius += step_speed;
  }
};


class rainbow_animation : public animation {

private:

  struct point {
    float x, y, z;
    float trans_x, trans_y, trans_z;
    float r, g, b;
  };

  char point_len = 0;
  float dropoff;

  point points[MAX_POINTS] = { { -DIMENSION/2, -DIMENSION/2, -DIMENSION/2, 1, 0, 0 },
                               { DIMENSION/2, -DIMENSION/2, -DIMENSION/2, 0, 1, 0 },
                               { DIMENSION/2 , -DIMENSION/2, DIMENSION/2, 0, 0, 1 },
                               { -DIMENSION/2, -DIMENSION/2, DIMENSION/2, 0.5, 0, 0.5 } };

  float scale = 255.0f / (DIMENSION * 3);
  float theta = 0;

  void rotate_y_axis(point &vec) {

    // rotate and translate vector
    vec.trans_x = cos(theta) * vec.x + vec.z * sin(theta) + (DIMENSION / 2.0f);
    vec.trans_y = vec.y + (DIMENSION / 2.0f);
    vec.trans_z = sin(theta) * -vec.x + vec.z * cos(theta) + (DIMENSION / 2.0f);
  }

  void rotate_x_axis(point &vec) {
    // rotate and translate vector
    vec.trans_x = vec.x + (DIMENSION / 2.0f);
    vec.trans_y = cos(theta) * vec.y + vec.z * sin(theta) + (DIMENSION / 2.0f);
    vec.trans_z = sin(theta) * -vec.y + vec.z * cos(theta) + (DIMENSION / 2.0f);
  }

public:

  RainbowEffect(float speed,float dropoff) {
    this->animation_speed = speed;
    this->dropoff = dropoff;
  }

  void add_point() {
    if (point_len > MAX_POINTS)
      point_len = 0;

    points[point_len].r = random() / (float)RAND_MAX;
    points[point_len].g = random() / (float)RAND_MAX;
    points[point_len].b = random() / (float)RAND_MAX;

    points[point_len].x = random() / (float)RAND_MAX*DIMENSION - DIMENSION / 2.0f;
    points[point_len].y = random() / (float)RAND_MAX*DIMENSION - DIMENSION / 2.0f;
    points[point_len].z = random() / (float)RAND_MAX*DIMENSION - DIMENSION / 2.0f;

    point_len++;
  }

  void tick() {

    theta += animation_speed;

    for (int i = 0; i < point_len; i++) {
        rotate_y_axis(points[i]);
    }

    for (int z = 0; z < DIMENSION; z++) {
      for (int y = 0; y < DIMENSION; y++) {
        for (int x = 0; x < DIMENSION; x++) {

          char r = 0, g = 0, b = 0;

          for (int i = 0; i < point_len; i++) {

            float inter_dist = scale * (DIMENSION + dropoff - (abs(points[i].trans_x - x) + abs(points[i].trans_y - y) + abs(points[i].trans_z - z)));
            r += inter_dist * points[i].r;
            g += inter_dist * points[i].g;
            b += inter_dist * points[i].b;
          }

          set_pixel(x, y, z, CRGB(r, g, b));
        }
      }
    }
  }

};

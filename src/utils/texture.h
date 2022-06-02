#ifndef RTCAMP2022_SRC_UTILS_TEXTURE_H_
#define RTCAMP2022_SRC_UTILS_TEXTURE_H_

#include "util_funcs.h"
#include "perlin.h"

class texture {
 public:
  virtual color value(double u, double v, const point3 &p) const = 0;
};

class solid_color : public texture {
 public:
  solid_color() {}
  solid_color(color c) : color_value(c) {}

  solid_color(double red, double green, double blue) : solid_color(color(red, green, blue)) {}

  color value(double u, double v, const vec3 &p) const override {
    return color_value;
  }

 private:
  color color_value;
};

class checker_texture : public texture {
 public:
  checker_texture() {}

  checker_texture(shared_ptr<texture> color_a, shared_ptr<texture> color_b)
      : even(color_a), odd(color_b) {}

  checker_texture(color c1, color c2)
      : even(make_shared<solid_color>(c1)), odd(make_shared<solid_color>(c2)) {}

  color value(double u, double v, const point3 &p) const override {
    auto sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
    if (sines < 0) {
      return odd->value(u, v, p);
    } else {
      return even->value(u, v, p);
    }
  }

 public:
  shared_ptr<texture> even;
  shared_ptr<texture> odd;
};

class noise_texture : public texture {
 public:
  noise_texture() {}
  noise_texture(color c) : texture_color(c) {}
  noise_texture(color c, double sc) : texture_color(c), scale(sc) {}

  color value(double u, double v, const point3 &p) const override {
    // noise : [0,1]
    return texture_color * 0.5 * (1 + sin(scale * p.z() + 10 * noise.turb(p)));
  }

 public:
  perlin noise;
  color texture_color{WHITE};
  double scale{1};
};

#endif //RTCAMP2022_SRC_UTILS_TEXTURE_H_

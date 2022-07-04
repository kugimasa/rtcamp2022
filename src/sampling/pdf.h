#ifndef RTCAMP2022_SRC_SAMPLING_PDF_H_
#define RTCAMP2022_SRC_SAMPLING_PDF_H_

#include "../utils/vec3.h"
#include "../utils/util_funcs.h"
#include "../utils/onb.h"
class pdf {
 public:
  virtual ~pdf() {}

  virtual double value(const vec3 &direction) const = 0;
  virtual vec3 generate() const = 0;
};

inline vec3 random_cosine_direction() {
  auto r1 = random_double();
  auto r2 = random_double();
  auto z = sqrt(1 - r2);

  auto phi = 2 * PI * r1;
  auto x = cos(phi) * sqrt(r2);
  auto y = sin(phi) * sqrt(r2);

  return vec3(x, y, z);
}

class cosine_pdf : public pdf {
 public:
  cosine_pdf(const vec3 &w) { uvw.build_from_w(w); }

  double value(const vec3 &direction) const override {
    auto cosine = dot(unit_vector(direction), uvw.w());
    return (cosine <= 0) ? 0 : cosine / PI;
  }

  vec3 generate() const override {
    return uvw.local(random_cosine_direction());
  }

 public:
  onb uvw;
};

class hittable_pdf : public pdf {
 public:
  hittable_pdf(shared_ptr<hittable> p, const point3 &origin) : ptr(p), o(origin) {}

  double value(const vec3 &direction) const override {
    return ptr->pdf_value(o, direction);
  }

  vec3 generate() const override {
    return ptr->random(o);
  }
 public:
  point3 o;
  shared_ptr<hittable> ptr;
};

#endif //RTCAMP2022_SRC_SAMPLING_PDF_H_

// Using the code of Ray Tracing in One Weekend
// https://raytracing.github.io/books/RayTracingInOneWeekend.html

#ifndef RAY_CAMERA_CAMERA_H_
#define RAY_CAMERA_CAMERA_H_

#include "../utils/util_funcs.h"

class camera {
 public:
  /// t0 から t1で移動するカメラ
  camera(vec3 lookfrom, vec3 lookat, vec3 vup, double vfov,
         double aspect, double aperture, double focus_dist,
         double t0, double t1)
      : camera(lookfrom, lookat, vup, vfov, aspect, aperture, focus_dist) {
    time0 = t0;
    time1 = t1;
  }
  /// Defocus Blur
  camera(vec3 lookfrom, vec3 lookat, vec3 vup, double vfov, double aspect, double aperture, double focus_dist) {
    lens_radius = aperture / 2;
    double theta = vfov * M_PI / 180;
    double half_height = tan(theta / 2);
    double half_width = aspect * half_height;
    origin = lookfrom;
    w = unit_vector(lookfrom - lookat);
    u = unit_vector(cross(vup, w));
    v = cross(w, u);
    lower_left_corner = origin - half_width * focus_dist * u - half_height * focus_dist * v - focus_dist * w;
    horizontal = 2 * half_width * focus_dist * u;
    vertical = 2 * half_height * focus_dist * v;
  }
  camera(vec3 lookfrom, vec3 lookat, vec3 vup, double vfov, double aspect) {
    double theta = vfov * M_PI / 180;
    double half_height = tan(theta / 2);
    double half_width = aspect * half_height;
    origin = lookfrom;
    w = unit_vector(lookfrom - lookat);
    u = unit_vector(cross(vup, w));
    v = cross(w, u);
    lower_left_corner = origin - half_width * u - half_height * v - w;
    horizontal = 2 * half_width * u;
    vertical = 2 * half_height * v;
  }
  camera() {
    lower_left_corner = vec3(-2.0, -1.5, -1.0);
    horizontal = vec3(4.0, 0.0, 0.0);
    vertical = vec3(0.0, 3.0, 0.0);
    origin = vec3(0.0, 0.0, 0.0);
  }
  //ray get_ray(double u, double v) {return ray(origin, lower_left_corner + u*horizontal + v*vertical - origin);}
  ray get_ray(double s, double t) {
    vec3 rd = lens_radius * random_in_unit_disk();
    vec3 offset = u * rd.x() + v * rd.y();
    double time = time0 + drand48() * (time1 - time0);
    return ray(origin + offset, lower_left_corner + s * horizontal + t * vertical - origin - offset, time);
  }

  vec3 origin;
  vec3 lower_left_corner;
  vec3 horizontal;
  vec3 vertical;
  vec3 u, v, w;
  double time0{0.0};
  double time1{0.0};
  double lens_radius;
};
#endif //RAY_CAMERA_CAMERA_H_

#ifndef RTCAMP2022_SRC_UTILS_PERLIN_H_
#define RTCAMP2022_SRC_UTILS_PERLIN_H_

#include "util_funcs.h"

class perlin {
 public:
  perlin() {
    ranvec = new vec3[point_count];
    for (int i = 0; i < point_count; ++i) {
      ranvec[i] = unit_vector(vec3::random(-1, 1));
    }
    perm_x = perlin_generate_perm();
    perm_y = perlin_generate_perm();
    perm_z = perlin_generate_perm();
  }

  ~perlin() {
    delete[] ranvec;
    delete[] perm_x;
    delete[] perm_y;
    delete[] perm_z;
  }

  double noise(const point3 &p) const {
    auto u = p.x() - floor(p.x());
    auto v = p.y() - floor(p.y());
    auto w = p.z() - floor(p.z());
    auto i = static_cast<int>(floor(p.x()));
    auto j = static_cast<int>(floor(p.y()));
    auto k = static_cast<int>(floor(p.z()));
    vec3 c[2][2][2];

    for (int di = 0; di < 2; di++) {
      for (int dj = 0; dj < 2; dj++) {
        for (int dk = 0; dk < 2; dk++) {
          c[di][dj][dk] = ranvec[
              perm_x[(i + di) & 255] ^
                  perm_y[(j + dj) & 255] ^
                  perm_z[(k + dk) & 255]
          ];
        }
      }
    }

    return perlin_interp(c, u, v, w);
  }

 private:

  static double perlin_interp(vec3 c[2][2][2], double u, double v, double w) {
    // Hermitian Smoothing
    u = u * u * (3 - 2 * u);
    v = v * v * (3 - 2 * v);
    w = w * w * (3 - 2 * w);
    auto accum = 0.0;

    for (int i = 0; i < 2; i++) {
      for (int j = 0; j < 2; j++) {
        for (int k = 0; k < 2; k++) {
          // 長さ[0,1]のベクトル
          vec3 wegiht_v(u - i, v - j, w - k);
          accum += (i * u + (1 - i) * (1 - u))
              * (j * v + (1 - j) * (1 - v))
              * (k * w + (1 - k) * (1 - w))
              * dot(c[i][j][k], wegiht_v);
        }
      }
    }
    return accum;
  }

  static const int point_count = 256;
  vec3 *ranvec;
  int *perm_x;
  int *perm_y;
  int *perm_z;

  static int *perlin_generate_perm() {
    auto p = new int[point_count];
    for (int i = 0; i < perlin::point_count; ++i) {
      p[i] = i;
    }
    permute(p, point_count);
    return p;
  }

  static void permute(int *p, int n) {
    for (int i = n - 1; i < 0; ++i) {
      int target = random_int(0, i);
      int tmp = p[i];
      p[i] = p[target];
      p[target] = tmp;
    }
  }
};

#endif //RTCAMP2022_SRC_UTILS_PERLIN_H_
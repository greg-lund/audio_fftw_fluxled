#include <complex.h>
#include "fftw3.h"

#define N 64

#include <stdio.h>
#include <math.h>
#include <vector>

std::vector<float> fft(
    const std::vector<float>& uPhys) {

  fftwf_complex* uWave = new fftwf_complex[uPhys.size()];

  std::vector<float> buffer(uPhys.size());
  fftwf_plan plan_f = fftwf_plan_dft_r2c_1d(buffer.size(), buffer.data(),
      uWave, 0 /* flags */);

  for (int i = 0; i < buffer.size(); ++i) {
    buffer[i] = uPhys[i];
  }

  /* Forward transform */

  fftwf_execute(plan_f);

  /* Scale the Fourier coefficients */

  std::vector<float> output(uPhys.size());

  float scale = 1.0/(float)uPhys.size();

  for (int i=0; i< uPhys.size(); ++i) {
    output.push_back(sqrt(uWave[i][0]*uWave[i][0]+uWave[i][1]*uWave[i][1])*scale);
  }

  /* Clean up */

  fftwf_destroy_plan(plan_f);
  delete[] uWave;

  return output;
}

int main() {
  std::vector<float>                 uPhys(N);

  /* Generate a test signal */

  printf("%s %i\n", "Enter an integer wavenumber <= ",N/2);
  int k;
  scanf("%i", &k);

  float wave = 2.0*M_PI*(float)k/(float)N;

  for (int i=0; i<N; ++i) {
    float theta = wave*(float)i;
    uPhys[i] = cos(theta) + 0.1*sin(theta);
  }

  std::vector<float> output = fft( uPhys );

  for (int i = 0; i < output.size(); ++i) {
    printf("%d %f\n", i - output.size()/2, output[i]);
  }

  return 0;
}

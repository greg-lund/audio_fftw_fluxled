#include "port_audio_stream.h"
#include <memory>
#include <fftw3.h>
#include <cmath>
#include <chrono>
#include <unistd.h>

int main() {

  /*---- FFT & STREAM SETUP ----*/

  size_t size = 512;

  std::vector<float> output;
  std::vector<float> input_tmp;
  std::vector<float> data;

  data.resize(size);
  output.resize(size/2);
  input_tmp.resize(size);

  fftwf_complex out[size];
  fftwf_plan plan = fftwf_plan_dft_r2c_1d(size, input_tmp.data(), out, 0 /* flags */);

  std::unique_ptr<AudioStream> audio_stream = std::make_unique<PortAudioStream>();
  audio_stream->StartStream();

  /*---- END SETUP ----*/

  while(true) {
    data = audio_stream->GetBuffer();
    input_tmp = data;
    fftwf_execute(plan);

    float amp = 0;
    for(int i = 0; i < size/2; i++) {
      output[i] = sqrt(out[i][0]*out[i][0] + out[i][1]*out[i][1])/(size);
      amp+=output[i];
      i++;
    }
    printf("%f\n",amp);
  }

  /*---- CLEAN UP ----*/
  fftwf_destroy_plan(plan);
  audio_stream->EndStream();
  return 0;
}


#include "port_audio_stream.h"
#include <memory>
#include <fftw3.h>
#include <cmath>
#include <chrono>
#include <unistd.h>


std::vector<float> do_fft(std::vector<float> input);
float get_amplitude(std::vector<float> input);

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

  float max = 0;
  float min = 1;
  while(true) {
    data = audio_stream->GetBuffer();
    input_tmp = data;
    fftwf_execute(plan);

    float amp = 0;
    for(int i = 0; i < size/2; i++) {
      output[i] = sqrt(out[i][0]*out[i][0] + out[i][1]*out[i][1])/(size);
      //printf("%d: %f\n",i,output[i]);
      amp+=output[i];
      i++;
    }
    if(amp > max) max = amp;
    if(amp < min) min = amp;
    printf("%f\n",amp);
  }

  printf("max: %f\nmin: %f\n",max,min);
  /*---- CLEAN UP ----*/
  fftwf_destroy_plan(plan);
  audio_stream->EndStream();
  return 0;
}

std::vector<float> do_fft(std::vector<float> input) {

  size_t size = input.size();
  std::vector<float> output;
  std::vector<float> input_tmp;
  output.resize(size);
  input_tmp.resize(size);

  fftwf_complex out[size];
  fftwf_plan plan = fftwf_plan_dft_r2c_1d(size, input_tmp.data(), out, 0 /* flags */);
  input_tmp = input;
  fftwf_execute(plan);

  int i = 0;
  for(const auto& o : out) {
    output[i] = sqrt(o[0]*o[0] + o[1]*o[1])/(size*size);
    i++;
  }

  fftwf_destroy_plan(plan);
  return output;

}


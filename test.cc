#include "portaudio.h"
#include <cstdio>
#include <array>

constexpr int SAMPLE_RATE = 44100;
constexpr int FRAMES_PER_BUFFER = 256;

void get_device_info() {
  int numDevices = Pa_GetDeviceCount();
  if( numDevices < 0 ) {
    printf("Error: Pa_CountDevices returned %d\n", numDevices);
    return;
  }

  const PaDeviceInfo *deviceInfo;
  for(int i=0; i < numDevices; i++) {
    deviceInfo = Pa_GetDeviceInfo(i);
    printf("%s: num input: %d, num output: %d, default sample rate: %f\n", deviceInfo->name, deviceInfo->maxInputChannels, deviceInfo->maxOutputChannels, deviceInfo->defaultSampleRate);
  }
}

int main() {
  PaError err = Pa_Initialize();
  get_device_info();
  if (err != paNoError) {
    printf("BAD\n");
    return 1;
  }

  //setup input and output
  PaStream *stream;
  err = Pa_OpenDefaultStream(&stream, 2, 0, paFloat32, SAMPLE_RATE, 256, NULL, NULL);
  if (err != paNoError) {
    printf("BAD\n");
    return 1;
  }

  //start stream
  err = Pa_StartStream(stream);
  if(err != paNoError) {
    printf("Couldn't open stream\n");
    return 1;
  }

  std::array<float,256> block;
  err = Pa_WriteStream(stream, block.data(), FRAMES_PER_BUFFER);

  if(err != paNoError) {
    printf("Gotcha\n");
    return 1;
  }
  for(float f : block) {
    printf("%f\n",f);
  }

  //stop stream
  printf("before stop\n");
  err = Pa_StopStream(stream);
  if(err != paNoError) {
    printf("Didn't stop correctly\n");
    return 1;
  }
  printf("before close\n");
  err = Pa_CloseStream(stream);
  if(err != paNoError) {
    printf("Didn't close correctly\n");
    return 1;
  }
  printf("Before\n");
  err = Pa_Terminate();
  printf("Here\n");
}

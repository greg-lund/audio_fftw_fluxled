#include "port_audio_stream.h"

namespace {
  constexpr size_t kDefaultSampleRate = 44100;
  constexpr size_t kDefaultBufferSize = 512;
}

PortAudioStream::PortAudioStream() {
  Options options;
  options.buffer_size = kDefaultBufferSize;
  options.sample_rate = kDefaultSampleRate;
  SetOptions(options);
}

PortAudioStream::~PortAudioStream() = default;

void PortAudioStream::StartStream() {

  PaStreamParameters inputParameters;
  PaError err;
  const PaDeviceInfo* inputInfo;

  err = Pa_Initialize();

  inputParameters.device = Pa_GetDefaultInputDevice();

  inputInfo = Pa_GetDeviceInfo(inputParameters.device);

  inputParameters.channelCount = 1;
  inputParameters.sampleFormat = paFloat32;
  inputParameters.suggestedLatency = inputInfo->defaultHighInputLatency;
  inputParameters.hostApiSpecificStreamInfo = NULL;

  /* Now we can open the stream */
  err = Pa_OpenStream(&stream, &inputParameters, 0, sample_rate_, buffer_size_, paClipOff, NULL, NULL);

  err = Pa_StartStream(stream);
  stream_is_running_ = true;
  /* The first buffer is garbage */
  GetBuffer();

}
void PortAudioStream::EndStream() {

  Pa_StopStream(stream);
  Pa_Terminate();
}


std::vector<float> PortAudioStream::GetBuffer() {

  if(!stream_is_running_) {
    return std::vector<float>();
  }
  std::vector<float> buffer;
  buffer.reserve(buffer_size_);
  buffer.resize(buffer_size_);
  Pa_ReadStream(stream,buffer.data(),buffer_size_);

  return buffer;
}

bool PortAudioStream::SetOptions(const Options& options) {
  if(stream_is_running_) {
    return false;
  }
  if(options.buffer_size.has_value()) {
    buffer_size_ = options.buffer_size.value();
  }
  if(options.sample_rate.has_value()) {
    sample_rate_ = options.sample_rate.value();
  }
  return true;
}





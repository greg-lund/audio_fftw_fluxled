#ifndef PORT_AUDIO_STREAM_H_
#define PORT_AUDIO_STREAM_H_

#include "audio_stream.h"

#include <portaudio.h>

class PortAudioStream : public AudioStream {
  public:
    PortAudioStream();
    ~PortAudioStream() override;

    void StartStream() override;
    void EndStream() override;
    std::vector<float> GetBuffer() override;
    bool SetOptions(const Options& options) override;

  private:
    PaStream *stream = nullptr;
    bool stream_is_running_ = false;
    size_t sample_rate_;
    size_t buffer_size_;


};


#endif  // PORT_AUDIO_STREAM_H_



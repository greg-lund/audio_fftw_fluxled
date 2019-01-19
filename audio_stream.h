#ifndef AUDIO_STREAM_H_
#define AUDIO_STREAM_H_

#include <optional>
#include <vector>

class AudioStream {
  public:
    struct Options {
      std::optional<size_t> sample_rate;
      std::optional<size_t> buffer_size;
    };

    virtual ~AudioStream() = default;
    virtual void StartStream() = 0;
    virtual void EndStream() = 0;
    virtual std::vector<float> GetBuffer() = 0;
    virtual bool SetOptions(const Options& options) = 0;
};

#endif  // AUDIO_STREAM_H_

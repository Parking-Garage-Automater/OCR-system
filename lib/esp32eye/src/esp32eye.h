#ifndef ESP32EYE_H
#define ESP32EYE_H

#include "internal/config.hpp"
#include "internal/mjpeg.hpp"
#include <memory>

namespace esp32eye
{

  class CameraClass
  {
  public:
    bool begin(const Config &config);

    bool end();

    ResolutionList listResolutions() const;

    inline bool changeResolution(const Resolution &resolution, int sleepFor = 500);

    std::unique_ptr<Frame> capture();

    int streamMjpeg(Client &client, const MjpegConfig &cfg = MjpegConfig());

    struct [[deprecated("use esp32eye::MjpegConfig instead")]] StreamMjpegConfig : MjpegConfig
    {
    };
  };

  extern CameraClass Camera;

  inline std::unique_ptr<Frame> capture()
  {
    return Camera.capture();
  }

} // namespace esp32eye

#endif // ESP32EYE_H

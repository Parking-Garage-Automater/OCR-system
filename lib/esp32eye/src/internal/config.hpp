#ifndef ESP32EYE_CONFIG_HPP
#define ESP32EYE_CONFIG_HPP

#include "pins.hpp"
#include "resolution.hpp"
#include <algorithm>
#include <cstring>
#include <esp_camera.h>

namespace esp32eye
{
  namespace detail
  {

    int
    convertJpegQuality(int quality);

  }

  class Config
  {
  public:
    Config();

    ~Config();

    Config &setPins(const Pins &pins);

    Config &setResolution(const Resolution &resolution);

    Config &setBufferCount(int n);

    camera_config_t *get() const { return m_cfg; }

    inline Config &setRgb();

    inline Config &setYuv();

    Config &setGrayscale();

    Config &setJpeg(int quality);

  private:
    struct CameraConfigT : public camera_config_t
    {
      CameraConfigT()
      {
        xclk_freq_hz = 20000000;
        ledc_timer = LEDC_TIMER_0;
        ledc_channel = LEDC_CHANNEL_0;
        pixel_format = PIXFORMAT_RGB565;
        frame_size = FRAMESIZE_QQVGA;
        fb_count = 2;
        grab_mode = CAMERA_GRAB_LATEST;

        pin_pwdn = pin_reset = pin_xclk = pin_sccb_sda = pin_sccb_scl = -1;
        pin_d7 = pin_d6 = pin_d5 = pin_d4 = pin_d3 = pin_d2 = pin_d1 = pin_d0 = -1;
        pin_vsync = pin_href = pin_pclk = -1;
      }
    };
    CameraConfigT *m_cfg;

    friend class CameraClass;
  };

} // namespace esp32eye

#endif // ESP32EYE_CONFIG_HPP

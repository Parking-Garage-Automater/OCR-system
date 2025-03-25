#include "config.hpp"
#include <algorithm>

namespace esp32eye
{
  namespace detail
  {
    int convertJpegQuality(int quality)
    {
      return std::min(63, std::max(0, 100 - quality));
    }
  } // namespace detail

  Config::Config()
      : m_cfg(new CameraConfigT{}) {}

  Config::~Config()
  {
    delete m_cfg;
  }

  Config &Config::setPins(const Pins &pins)
  {
    m_cfg->pin_pwdn = pins.PWDN;
    m_cfg->pin_reset = pins.RESET;
    m_cfg->pin_xclk = pins.XCLK;
    m_cfg->pin_sccb_sda = pins.SDA;
    m_cfg->pin_sccb_scl = pins.SCL;
    m_cfg->pin_d7 = pins.D7;
    m_cfg->pin_d6 = pins.D6;
    m_cfg->pin_d5 = pins.D5;
    m_cfg->pin_d4 = pins.D4;
    m_cfg->pin_d3 = pins.D3;
    m_cfg->pin_d2 = pins.D2;
    m_cfg->pin_d1 = pins.D1;
    m_cfg->pin_d0 = pins.D0;
    m_cfg->pin_vsync = pins.VSYNC;
    m_cfg->pin_href = pins.HREF;
    m_cfg->pin_pclk = pins.PCLK;
    return *this;
  }

  Config &Config::setResolution(const Resolution &res)
  {
    m_cfg->frame_size = res.as<framesize_t>();
    return *this;
  }

  Config &Config::setBufferCount(int n)
  {
    m_cfg->fb_count = std::max(1, n);
    return *this;
  }

  Config &Config::setRgb()
  {
    m_cfg->pixel_format = PIXFORMAT_RGB565;
    return *this;
  }

  Config &Config::setYuv()
  {
    m_cfg->pixel_format = PIXFORMAT_YUV422;
    return *this;
  }

  Config &Config::setGrayscale()
  {
    m_cfg->pixel_format = PIXFORMAT_GRAYSCALE;
    return *this;
  }

  Config &Config::setJpeg(int quality)
  {
    m_cfg->pixel_format = PIXFORMAT_JPEG;
    m_cfg->jpeg_quality = detail::convertJpegQuality(quality);
    return *this;
  }

} // namespace esp32eye

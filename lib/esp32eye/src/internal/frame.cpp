#include "frame.hpp"
#include "config.hpp"

#include <Arduino.h>
#include <esp_camera.h>

namespace esp32eye
{

  static const int PIXFORMAT_BMP = -101;

  Frame::Frame() = default;

  Frame::Frame(void *fb)
      : m_fb(static_cast<camera_fb_t *>(fb)),
        m_data(m_fb->buf),
        m_size(m_fb->len),
        m_width(m_fb->width),
        m_height(m_fb->height),
        m_pixFormat(m_fb->format)
  {
  }

  Frame::~Frame()
  {
    releaseFb();

    // Free buffer only if manually allocated (e.g. by toJpeg/toBmp)
    if (m_data != nullptr && m_fb == nullptr)
    {
      free(m_data);
      m_data = nullptr;
    }
  }

  void Frame::releaseFb()
  {
    if (m_fb != nullptr)
    {
      esp_camera_fb_return(m_fb);
      m_fb = nullptr;
      m_data = nullptr;
    }
  }

  bool Frame::writeTo(Print &os, int timeout)
  {
    return writeToImpl(os, timeout, nullptr);
  }

  bool Frame::writeTo(Client &os, int timeout)
  {
    return writeToImpl(os, timeout, &os);
  }

  bool Frame::writeToImpl(Print &os, int timeout, Client *client)
  {
    auto startTime = millis();
    for (size_t i = 0; i < m_size; i += os.write(&m_data[i], m_size - i))
    {
      if (millis() - startTime > static_cast<unsigned long>(timeout) ||
          (client != nullptr && !client->connected()))
      {
        return false;
      }
      yield();
    }
    return true;
  }

  bool Frame::isJpeg() const
  {
    return m_pixFormat == PIXFORMAT_JPEG;
  }

  bool Frame::toJpeg(int quality)
  {
    uint8_t *data = nullptr;
    size_t size = 0;

    bool ok = fmt2jpg(
        m_data, m_size, m_width, m_height,
        static_cast<pixformat_t>(m_pixFormat),
        detail::convertJpegQuality(quality),
        &data, &size);

    if (!ok)
    {
      return false;
    }

    releaseFb();

    if (m_data != nullptr && m_fb == nullptr)
    {
      free(m_data);
    }

    m_data = data;
    m_size = size;
    m_pixFormat = PIXFORMAT_JPEG;
    return true;
  }

  bool Frame::isBmp() const
  {
    return m_pixFormat == PIXFORMAT_BMP;
  }

  bool Frame::toBmp()
  {
    uint8_t *data = nullptr;
    size_t size = 0;

    bool ok = fmt2bmp(
        m_data, m_size, m_width, m_height,
        static_cast<pixformat_t>(m_pixFormat),
        &data, &size);

    if (!ok)
    {
      return false;
    }

    releaseFb();

    if (m_data != nullptr && m_fb == nullptr)
    {
      free(m_data);
    }

    m_data = data;
    m_size = size;
    m_pixFormat = PIXFORMAT_BMP;
    return true;
  }

} // namespace esp32eye

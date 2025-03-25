#ifndef ESP32EYE_FRAME_HPP
#define ESP32EYE_FRAME_HPP

#include <cstddef>
#include <cstdint>
#include <esp_camera.h>

class Client;
class Print;

namespace esp32eye
{

  class Frame
  {
  public:
    Frame();
    explicit Frame(void *fb);
    ~Frame();

    Frame(const Frame &) = delete;
    Frame &operator=(const Frame &) = delete;

    Frame(Frame &&) noexcept = default;
    Frame &operator=(Frame &&) noexcept = default;

    uint8_t *data() const { return m_data; }
    size_t size() const { return m_size; }
    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }

    bool writeTo(Print &os, int timeout = 10000);
    bool writeTo(Client &os, int timeout = 10000);

    bool isJpeg() const;
    bool toJpeg(int quality);

    bool isBmp() const;
    bool toBmp();

    bool isValid() const { return m_data != nullptr && m_size > 0; }

  private:
    void releaseFb();
    bool writeToImpl(Print &os, int timeout, Client *client);

  private:
    camera_fb_t *m_fb = nullptr;
    uint8_t *m_data = nullptr;
    size_t m_size = 0;
    int m_width = -1;
    int m_height = -1;
    int m_pixFormat = -1;

    friend class CameraClass;
  };

} // namespace esp32eye

#endif // ESP32EYE_FRAME_HPP

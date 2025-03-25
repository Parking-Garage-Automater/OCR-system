#ifndef ESP32EYE_MJPEG_HPP
#define ESP32EYE_MJPEG_HPP

#include "frame.hpp"
#include <memory>

namespace esp32eye
{

  struct MjpegConfig
  {
    int minInterval = 0; // Minimum ms between frames
    int maxFrames = -1;  // -1 = stream indefinitely
    int frameTimeout = 10000;
  };

  namespace detail
  {

    class MjpegController
    {
    public:
      explicit MjpegController(MjpegConfig cfg);

      const MjpegConfig &getConfig() const { return m_cfg; }
      int countSentFrames() const { return m_count; }

      enum class Action : int
      {
        CAPTURE = -1,
        RETURN = -2,
        SEND = -3,
        STOP = -4
      };

      int decideAction();
      void notifyCapture();
      void notifyReturn(std::unique_ptr<Frame> frame);
      Frame *getFrame() const { return m_frame.get(); }
      void notifySent(bool ok);
      void notifyFail();

      void reset();

    private:
      MjpegConfig m_cfg;
      std::unique_ptr<Frame> m_frame;
      unsigned long m_nextCaptureTime;
      Action m_nextAction = Action::CAPTURE;
      int m_count = 0;
    };

    class MjpegHeader
    {
    public:
      void prepareResponseHeaders();
      void prepareResponseContentType();
      void preparePartHeader(size_t contentLength);
      void preparePartTrailer();
      size_t writeTo(Print &os);

    private:
      size_t size = 0;
      char buf[160];
      friend class MjpegController; // if direct access is needed
    };

    constexpr const char *MJPEG_BOUNDARY = "e8b8c539-047d-4777-a985-fbba6edff11e";

  } // namespace detail
} // namespace esp32eye

#endif // ESP32EYE_MJPEG_HPP

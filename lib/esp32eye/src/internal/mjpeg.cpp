#include "mjpeg.hpp"
#include <Arduino.h>

namespace esp32eye
{
  namespace detail
  {

    MjpegController::MjpegController(MjpegConfig cfg)
        : m_cfg(cfg), m_nextCaptureTime(millis())
    {
    }

    int MjpegController::decideAction()
    {
      if (m_nextAction == Action::CAPTURE)
      {
        int timeLeft = static_cast<int>(m_nextCaptureTime - millis());
        if (timeLeft > 0)
        {
          return timeLeft;
        }
        return static_cast<int>(Action::CAPTURE);
      }

      return static_cast<int>(m_nextAction);
    }

    void MjpegController::notifyCapture()
    {
      m_nextAction = Action::RETURN;
      m_nextCaptureTime = millis() + static_cast<unsigned long>(m_cfg.minInterval);
    }

    void MjpegController::notifyReturn(std::unique_ptr<Frame> frame)
    {
      if (!frame)
      {
        notifyFail();
        return;
      }

      m_frame = std::move(frame);
      m_nextAction = Action::SEND;
    }

    void MjpegController::notifySent(bool ok)
    {
      ++m_count;

      if (!ok)
      {
        notifyFail();
        return;
      }

      m_frame.reset();

      if (m_cfg.maxFrames < 0 || m_count < m_cfg.maxFrames)
      {
        m_nextAction = Action::CAPTURE;
      }
      else
      {
        m_nextAction = Action::STOP;
      }
    }

    void MjpegController::notifyFail()
    {
      m_frame.reset();
      m_nextAction = Action::STOP;
    }

    void MjpegController::reset()
    {
      m_frame.reset();
      m_nextAction = Action::CAPTURE;
      m_count = 0;
      m_nextCaptureTime = millis();
    }

    void MjpegHeader::prepareResponseHeaders()
    {
      size = snprintf(buf, sizeof(buf),
                      "HTTP/1.1 200 OK\r\n"
                      "Content-Type: multipart/x-mixed-replace;boundary=%s\r\n"
                      "\r\n",
                      MJPEG_BOUNDARY);
    }

    void MjpegHeader::prepareResponseContentType()
    {
      size = snprintf(buf, sizeof(buf),
                      "multipart/x-mixed-replace;boundary=%s", MJPEG_BOUNDARY);
    }

    void MjpegHeader::preparePartHeader(size_t contentLength)
    {
      size = snprintf(buf, sizeof(buf),
                      "Content-Type: image/jpeg\r\n"
                      "Content-Length: %zu\r\n\r\n",
                      contentLength);
    }

    void MjpegHeader::preparePartTrailer()
    {
      size = snprintf(buf, sizeof(buf), "\r\n--%s\r\n", MJPEG_BOUNDARY);
    }

    size_t MjpegHeader::writeTo(Print &os)
    {
      return os.write(buf, size);
    }

  } // namespace detail
} // namespace esp32eye

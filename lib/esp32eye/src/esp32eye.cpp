#include "esp32eye.h"

#include <Arduino.h>
#include <esp_camera.h>

namespace esp32eye
{

  CameraClass Camera;

  // Optional: cache the sensor pointer after init
  static sensor_t *getSensor()
  {
    return esp_camera_sensor_get();
  }

  bool CameraClass::begin(const Config &config)
  {
    return esp_camera_init(reinterpret_cast<const camera_config_t *>(config.m_cfg)) == ESP_OK;
  }

  bool CameraClass::end()
  {
    return esp_camera_deinit() == ESP_OK;
  }

  ResolutionList CameraClass::listResolutions() const
  {
    sensor_t *sensor = getSensor();
    if (sensor == nullptr)
    {
      return ResolutionList();
    }

    camera_sensor_info_t *info = esp_camera_sensor_get_info(&sensor->id);
    if (info == nullptr)
    {
      return ResolutionList();
    }

    return ResolutionList(info->max_size + 1);
  }

  bool CameraClass::changeResolution(const Resolution &resolution, int sleepFor)
  {
    sensor_t *sensor = getSensor();
    if (sensor == nullptr)
    {
      return false;
    }

    framesize_t frameSize = resolution.as<framesize_t>();
    if (sensor->status.framesize == frameSize)
    {
      return true;
    }

    if (sensor->set_framesize(sensor, frameSize) != 0)
    {
      return false;
    }

    if (sleepFor > 0)
    {
      delay(sleepFor);
    }

    return true;
  }

  std::unique_ptr<Frame> CameraClass::capture()
  {
    camera_fb_t *fb = esp_camera_fb_get();
    if (fb == nullptr)
    {
      return nullptr;
    }

    // C++11-compatible version:
    return std::unique_ptr<Frame>(new Frame(fb));
  }

  int CameraClass::streamMjpeg(Client &client, const MjpegConfig &cfg)
  {
    detail::MjpegHeader hdr;
    hdr.prepareResponseHeaders();
    hdr.writeTo(client);

    using Controller = detail::MjpegController;
    using Action = Controller::Action;

    Controller ctrl(cfg);

    while (true)
    {
      Action act = static_cast<Action>(ctrl.decideAction());

      switch (act)
      {
      case Action::CAPTURE:
      {
        ctrl.notifyCapture();
        break;
      }
      case Action::RETURN:
      {
        ctrl.notifyReturn(capture());
        break;
      }
      case Action::SEND:
      {
        auto *frame = ctrl.getFrame();

        if (!frame || !frame->isJpeg())
        {
          ctrl.notifySent(false);
          break;
        }

        hdr.preparePartHeader(frame->size());
        hdr.writeTo(client);

        ctrl.notifySent(frame->writeTo(client, cfg.frameTimeout));

        hdr.preparePartTrailer();
        hdr.writeTo(client);

#ifdef DEBUG
        Serial.printf("[MJPEG] Frame sent (%d bytes)\n", static_cast<int>(frame->size()));
#endif

        break;
      }
      case Action::STOP:
      {
        client.stop();
        return ctrl.countSentFrames();
      }
      default:
      {
        delay(static_cast<int>(act));
        break;
      }
      }
    }
  }

} // namespace esp32eye

#include <WiFi.h>
#include <esp32eye.h>
#include <SPI.h>
#include <Wire.h>
#include <HTTPClient.h>

const char *WIFI_SSID = "Rokas_wifi";
const char *WIFI_PASS = "internetas2001";

const char *API_ENDPOINT = "http://172.20.10.4:7002/process-label";

static auto hiRes = esp32eye::Resolution::find(800, 600);

void connectToWiFi()
{
  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Connecting to WiFi");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConnected to WiFi!");
  Serial.println(WiFi.localIP());
}

void captureAndSendImage()
{
  auto frame = esp32eye::capture();
  if (frame == nullptr)
  {
    Serial.println("Failed to capture image");
    return;
  }

  HTTPClient http;
  http.begin(API_ENDPOINT);
  http.addHeader("Content-Type", "image/jpeg");

  int httpResponseCode = http.POST(frame->data(), frame->size());

  if (httpResponseCode > 0)
  {
    Serial.printf("Image sent! HTTP response code: %d\n", httpResponseCode);
  }
  else
  {
    Serial.printf("Failed to send image. Error: %s\n", http.errorToString(httpResponseCode).c_str());
  }

  http.end();
}

void setup()
{
  Serial.begin(115200);

  using namespace esp32eye;
  Config cfg;
  cfg.setPins(pins::ESP32EYE);
  cfg.setResolution(hiRes);
  cfg.setBufferCount(2);
  cfg.setJpeg(80);

  bool camera_ok = Camera.begin(cfg);
  Serial.println(camera_ok ? "Camera initialized successfully" : "Camera failed to initialize");

  connectToWiFi();

  delay(3000);
  captureAndSendImage();
}
void loop()
{
  delay(5000);
  captureAndSendImage();
}
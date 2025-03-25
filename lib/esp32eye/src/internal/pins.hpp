#ifndef ESP32EYE_PINS_HPP
#define ESP32EYE_PINS_HPP

namespace esp32eye
{

  struct Pins
  {
    int D0, D1, D2, D3, D4, D5, D6, D7;
    int XCLK, PCLK, VSYNC, HREF;
    int SDA, SCL;
    int RESET, PWDN;

    Pins(int d0, int d1, int d2, int d3, int d4, int d5, int d6, int d7,
         int xclk, int pclk, int vsync, int href,
         int sda, int scl, int reset, int pwdn)
        : D0(d0), D1(d1), D2(d2), D3(d3), D4(d4), D5(d5), D6(d6), D7(d7),
          XCLK(xclk), PCLK(pclk), VSYNC(vsync), HREF(href),
          SDA(sda), SCL(scl), RESET(reset), PWDN(pwdn) {}
  };

  namespace pins
  {

    // Default pin mapping for ESP32-EYE
    const Pins ESP32EYE(
        34, 13, 14, 35, 39, 38, 37, 36, // D0–D7
        4, 25, 5, 27,                   // XCLK, PCLK, VSYNC, HREF
        18, 23,                         // SDA, SCL
        -1, -1                          // RESET, PWDN
    );

  } // namespace pins
} // namespace esp32eye

#endif // ESP32EYE_PINS_HPP

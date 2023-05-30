# UncannyEyesESP32
This is a ESP32 compatible redo of Adafruit https://github.com/adafruit/Uncanny_Eyes  for  ESP32 using default Adafruit_GFX library. 


https://github.com/jujojujo2003/UncannyEyesESP32/assets/1708406/8bf5ff73-4136-44c4-a586-412c8c9cfc50

## Components used
* 2 x [128x128 RGB565 16-bit SSD1351 OLED display](https://www.amazon.com/1-5inch-Interface-Raspberry-Examples-Provided/dp/B07D9NVJPZ)
* 1 x [ESP32  38 pin dev board](https://www.amazon.com/KeeYees-Development-Bluetooth-Microcontroller-ESP-WROOM-32/dp/B07QCP2451/)
* **[Optional]** Use some kind of [plano convex lens](https://www.amazon.com/gp/product/B00V54BM2A/) in front of this to make it more like an eye than a display.

## Connections
![ESP32-38 PIN-DEVBOARD](https://github.com/jujojujo2003/UncannyEyesESP32/assets/1708406/2afbed5b-3aef-428f-9cb9-6e508460cc97)

ESP32 has 2 SPI buses called VSPI and HSPI. Connect the 2  displays to them. (DIN on display <-> MOSI on ESP32, others should be self explanatory) 

|   | DIN | CLK  | CS | DC | RST  |
| ------------- | ------------- | ------------- | ------------- | ------------- | ------------- |
| Left Display | 13 | 14  | 27 | 26  | 25|
| Right Display | 23 | 18  | 0 | 17  | 16|

# Compiling software
* Get arduino  IDE
* Install the following libraries
  * [Adafruit-GFX-Library](https://github.com/adafruit/Adafruit-GFX-Library)
  * [Adafruit-SSD1351-library](https://github.com/adafruit/Adafruit-SSD1351-library)
* Open AppaEyes.ino , compile and upload to your ESP32

## Notes
* I was able to drive the display  at SPI_FREQUENCY 30000000 and  gave consistent 30FPS driving 2 displays with 1 ESP32 or at 60 FPS for 1 display on 1 ESP32 (you can also try assiging 1 core each to the displays on dual core ESP32 for higher performance) . Try lowering this frequency if the display looks corrupted
* You can change the asset data.h to any RGB565 image. Just make sure you maintain the dimentions of the image. Refer data.h on tools to convert 

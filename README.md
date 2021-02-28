# JLC_CODE
This project is developed using Tuya SDK, which enables you to quickly develop branded apps connecting and controlling smart scenarios of many devices.For more information, please check Tuya Developer Website.

Introduce
get the temperature and the humidity information from the SHT30 sensor on board
show the information on the 0.96" OLED screen
upload the information to the tuya cloud that I can see the temperature and the humidity in tuya APP.
Video
In my oshw project

Hardware
WBR3D WiFi module

SHT30 temperature and humidity sensor

SSD1306 0.96"OLED screen

STM32G431CBT6 microcontroller

PCB Link

Software
MCU SDK by Tuya

STM32 CubeMX with HAL

Source Code is in the "source" folder

Todo
This is the first board to use the Tuya module. And it actually expose some problems.

the LDO is too hot to influence the temperature sensor, maybe 2 or 3 degrees.
The MCU is a little expensive. Although I buy it(STM32G031CBT6) for only about 10RMB, it is inappropriate to use a high performance MCU to do such a little thing.
The fixing way of the OLED, using female header and male pin, is unreliable.
The USB2TTL chip CH340 and the flash chip may unused.

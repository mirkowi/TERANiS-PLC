//
// Created by mirko on 07.03.2020.
//

#include "PlcIoMraa.h"

#include <mraa.h>
#include <iostream>
#include <cstdint>

#include "teranis.h"

mraa_gpio_context di0 = NULL;
mraa_gpio_context di1 = NULL;
mraa_gpio_context di2 = NULL;
mraa_gpio_context di3 = NULL;
mraa_gpio_context di4 = NULL;
mraa_gpio_context di5 = NULL;
mraa_gpio_context di6 = NULL;
mraa_gpio_context di7 = NULL;
mraa_gpio_context di8 = NULL;
mraa_gpio_context di9 = NULL;
mraa_gpio_context led = NULL;

mraa_gpio_context initPin(int pin, mraa_gpio_dir_t dir)
{
  mraa_gpio_context context = mraa_gpio_init(pin);
  if (!context) {
    std::cerr << "Failed to initialize pin " << pin << std::endl;
  }
  else
  {
    if (mraa_gpio_dir(context, dir)!=MRAA_SUCCESS) {
      std::cerr << "Failed to set direction of pin " << pin << std::endl;
    }
  }
  return context;
}

void TPlcIoMraa::begin() {
  led = initPin(13,MRAA_GPIO_OUT);
  di0 = initPin(4,MRAA_GPIO_IN);
  di1 = initPin(5,MRAA_GPIO_IN);
  di2 = initPin(6,MRAA_GPIO_IN);
  di3 = initPin(7,MRAA_GPIO_IN);
  di4 = initPin(8,MRAA_GPIO_IN);
  di5 = initPin(9,MRAA_GPIO_IN);
  di6 = initPin(10,MRAA_GPIO_IN);
  di7 = initPin(11,MRAA_GPIO_IN);
  di8 = initPin(12,MRAA_GPIO_IN);
  //di9 = initPin(13,MRAA_GPIO_IN);
}

void TPlcIoMraa::read() {
  IX(0,0) = di0 && mraa_gpio_read(di0);
  IX(0,1) = di1 && mraa_gpio_read(di1);
  IX(0,2) = di2 && mraa_gpio_read(di2);
  IX(0,3) = di3 && mraa_gpio_read(di3);
  IX(0,4) = di4 && mraa_gpio_read(di4);
  IX(0,5) = di5 && mraa_gpio_read(di5);
  IX(0,6) = di6 && mraa_gpio_read(di6);
  IX(0,7) = di7 && mraa_gpio_read(di7);
  IX(1,0) = di8 && mraa_gpio_read(di8);
  IX(1,1) = di9 && mraa_gpio_read(di9);
}

void TPlcIoMraa::write() {
  mraa_gpio_write(led, I(uint16_t,0)!=0);//QX(0,0));
}

void TPlcIoMraa::end() {
  mraa_gpio_close(di0);
  mraa_gpio_close(di1);
  mraa_gpio_close(di2);
  mraa_gpio_close(di3);
  mraa_gpio_close(di4);
  mraa_gpio_close(di5);
  mraa_gpio_close(di6);
  mraa_gpio_close(di7);
  mraa_gpio_close(di8);
  mraa_gpio_close(di9);
  mraa_gpio_close(led);
  mraa_deinit();
}


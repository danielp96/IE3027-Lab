
#include <string.h>

#include <Arduino.h>

#include "config.h"


void handle1(AdafruitIO_Data *data);
void handle2(AdafruitIO_Data *data);


unsigned int data = 0;
int msg = 0x30; // '0' in ascii

AdafruitIO_Feed *data_feed = io.feed("Data");
AdafruitIO_Feed *led1_feed = io.feed("Pilot1");
AdafruitIO_Feed *led2_feed = io.feed("Pilot2");

void setup()
{

    // start the serial connection
    Serial.begin(9600);

    // wait for serial monitor to open
    while(! Serial);
  
    // connect to io.adafruit.com
    io.connect();

    led1_feed->onMessage(handle1);
    led2_feed->onMessage(handle2);
  
    // wait for a connection
    while(io.status() < AIO_CONNECTED)
    {
        delay(500);
    }

    while(io.mqttStatus() < AIO_CONNECTED)
    {
        delay(500);
    }

}

void loop()
{
    io.run();

    if (Serial.available())
    {
        data = Serial.read();

        data_feed->save(data);
    }

    delay(3000);

}

void handle1(AdafruitIO_Data *data)
{
    String pin_value = data->toString();

    if (pin_value.compareTo("ON") == 0)
    {
        msg |= 0x01; // set first bit
    } 
    else if (pin_value.compareTo("OFF") == 0)
    {
        msg &= 0xFE; //clear first bit
    } 

    Serial.write(msg);

}


void handle2(AdafruitIO_Data *data)
{
    String pin_value = data->toString();

    if (pin_value.compareTo("ON") == 0)
    {
        msg |= 0x02; // set second bit
    } 
    else if (pin_value.compareTo("OFF") == 0)
    {
        msg &= 0xFD; //clear second bit
    } 

    Serial.write(msg);

}
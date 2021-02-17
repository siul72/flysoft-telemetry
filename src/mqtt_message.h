#ifndef MQTT_MESSAGE_H
#define MQTT_MESSAGE_H

#include <Arduino.h>

class MqttMessage {

public:
    String topic = "default";
    String payload;

};

#endif // MQTT_MESSAGE_H

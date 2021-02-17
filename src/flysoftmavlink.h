#ifndef FLYSOFTMAVLINK_H
#define FLYSOFTMAVLINK_H

#include <Arduino.h>
#include <common/mavlink.h>
#include <checksum.h>
#include <mavlink_types.h>
#include <protocol.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include "queue.h"
#include "utils.h"
#include "mqtt_message.h"

class FlysoftMavlink
{
public:
    FlysoftMavlink(uint16_t rx_port, uint16_t tx_port, uint16_t baudrate=57600);
    ~FlysoftMavlink();
    bool begin();
    void getStream();
    void getMessages(std::vector<MqttMessage*> *list);
private:
    SoftwareSerial* _myserial;
    char log_msg[1024];
    Queue<MqttMessage*> myMavlinkQueue;
    void parseMavlinkMessage(mavlink_message_t msg, MqttMessage *f);
    mavlink_message_t my_mavlink_msg;
    mavlink_status_t status1;
    double MILLIG_TO_MS2;
    uint16_t baudrate;
    uint8_t system_id;
    uint8_t component_id;
    uint8_t type;
    uint8_t autopilot;
    uint8_t received_sysid;
    uint8_t received_compid;

};

#endif // FLYSOFTMAVLINK_H

#include "flysoftmavlink.h"

FlysoftMavlink::FlysoftMavlink(uint16_t rx_port, uint16_t tx_port, uint16_t baudrate){
    this->_myserial = new SoftwareSerial(rx_port, tx_port);
    this->baudrate = baudrate;
    this->MILLIG_TO_MS2 = 9.80665 / 1000.0;
    this->system_id = 1; // Your i.e. Arduino sysid
    this->component_id = 158; // Your i.e. Arduino compid
    this->type = MAV_TYPE_QUADROTOR;
    this->autopilot = MAV_AUTOPILOT_INVALID;

}

FlysoftMavlink::~FlysoftMavlink(){
    delete _myserial;
}

bool FlysoftMavlink::begin(){
    //this->_myserial->setTimeout(20); //good for 9600 baud
    //this->_myserial->setTimeout(160);
    this->_myserial->setTimeout(40);
    this->_myserial->begin(this->baudrate);

    return true;
}

void FlysoftMavlink::getStream(){

    uint size = MAVLINK_MAX_PACKET_LEN/2; //goof for 9600 baud
    //uint size = MAVLINK_MAX_PACKET_LEN;
    char buffer[size];
    //mavlink_message_t my_mavlink_msg;
    //String debug;
    String* frame = NULL;

    while (_myserial->available() > 0){
        //Serial.println("readBytes");
        size_t bytes_received = _myserial->readBytes(buffer, size);
        //sprintf(log_msg, "received %d bytes", (uint32_t)bytes_received);
        //Serial.println(log_msg);
        for(uint32_t i = 0; i < bytes_received; i++){

//            if (buffer[i] == (char)MAVLINK_STX){
//                Serial.println("GOT STX");
//            }
            uint8_t ret = mavlink_frame_char(MAVLINK_COMM_0,buffer[i], &my_mavlink_msg, &status1);
            //sprintf(log_msg, "return %d", ret);
            //Serial.println(log_msg);
            switch((mavlink_framing_t)ret){
                case MAVLINK_FRAMING_INCOMPLETE:
                    continue;
                case MAVLINK_FRAMING_OK:
                    //Serial.println("Message Parsing Done with OK!");
                    frame = new String();
                    parseMavlinkMessage(my_mavlink_msg, frame);
                    myMavlinkQueue.push(frame);
                break;

                case MAVLINK_FRAMING_BAD_CRC:
                    Serial.println("Message Parsing Done with bad CRC!");
                break;

                default:
                    Serial.println("Message Parsing Done with default!");

            }

        }

    }
    //Serial.println(debug);
    Serial.println("Exit without decode!");

}

void FlysoftMavlink::getMessages(std::vector<String*> *list){

    int x = myMavlinkQueue.count();
    sprintf(log_msg, "pop=%d messages", x);
    Serial.println(log_msg);
    while(myMavlinkQueue.count() > 0 ){
        String *frame = myMavlinkQueue.pop();
        //sprintf(log_msg, "pop=%s", frame->c_str());
        //Serial.println(log_msg);
        list->push_back(frame);
    }

}

void FlysoftMavlink::parseMavlinkMessage(mavlink_message_t msg, String *f){


    char str[18] = "";
    StaticJsonDocument<200> doc;
    Utils::timeToString(str, sizeof(str));
    doc["timestamp"]=str;
    doc["msg_id"]=msg.msgid;
    JsonObject obj;

    //Get String from xml msg.msgid common.xml
    switch (msg.msgid) {
        //<message id="0" name="HEARTBEAT">
        case MAVLINK_MSG_ID_HEARTBEAT:
            mavlink_heartbeat_t heartbeat;
            mavlink_msg_heartbeat_decode(&msg, &heartbeat);
            obj = doc.createNestedObject("heartbeat");
            obj["mavlink_version"] = heartbeat.mavlink_version;
            obj["type"] = heartbeat.type;
            obj["autopilot"] = heartbeat.autopilot;
            obj["base_mode"] = heartbeat.base_mode;
            obj["custom_mode"] = heartbeat.custom_mode;
            obj["system_status"] = heartbeat.system_status;
        break;
        //<message id="1" name="SYS_STATUS">
        case MAVLINK_MSG_ID_SYS_STATUS:
            mavlink_sys_status_t sys_status;
            mavlink_msg_sys_status_decode(&msg, &sys_status);
            obj = doc.createNestedObject("sys_status");
            obj["battery_remainings"] = sys_status.battery_remaining;
            obj["voltage_battery"] = sys_status.voltage_battery;
            obj["current_battery"] = sys_status.current_battery;
            obj["load"] = sys_status.load;
            obj["errors_comm"] = sys_status.errors_comm;
            obj["drop_rate_comm"] = sys_status.drop_rate_comm;
            obj["onboard_control_sensors_health"] = sys_status.onboard_control_sensors_health;
            obj["onboard_control_sensors_present"] = sys_status.onboard_control_sensors_present;
            obj["onboard_control_sensors_enabled"] = sys_status.onboard_control_sensors_enabled;
        break;
        //<message id="29" name="SCALED_PRESSURE">
        case MAVLINK_MSG_ID_SCALED_PRESSURE:
            mavlink_scaled_pressure_t scaled_pressure;
            mavlink_msg_scaled_pressure_decode(&msg, &scaled_pressure);
            obj = doc.createNestedObject("scaled_pressure");
            obj["time_boot_ms"] = scaled_pressure.time_boot_ms;
            obj["temperature"] = scaled_pressure.temperature;
            obj["press_abs"] = scaled_pressure.press_abs;
            obj["press_diff"] = scaled_pressure.press_diff;
        break;
        //message id=30
        case MAVLINK_MSG_ID_ATTITUDE:
            mavlink_attitude_t attitude;
            mavlink_msg_attitude_decode(&msg, &attitude);
            obj = doc.createNestedObject("attitude");
            obj["time_boot_ms"] = attitude.time_boot_ms;
            obj["roll"] = attitude.roll;
            obj["pitch"] = attitude.pitch;
            obj["yaw"] = attitude.yaw;
            obj["rollspeed"] = attitude.rollspeed;
            obj["pitchspeed"] = attitude.pitchspeed;
            obj["yawspeed"] = attitude.yawspeed;
        break;
        //<message id="35" name="RC_CHANNELS_RAW">
        case MAVLINK_MSG_ID_RC_CHANNELS_RAW:
            mavlink_rc_channels_raw_t rc_channels_raw;
            mavlink_msg_rc_channels_raw_decode(&msg, &rc_channels_raw);
            obj = doc.createNestedObject("rc_channels_raw");
            obj["time_boot_ms"] = rc_channels_raw.time_boot_ms;
            obj["port"] = rc_channels_raw.port;
            obj["chan1_raw"] = rc_channels_raw.chan1_raw;
            obj["chan2_raw"] = rc_channels_raw.chan2_raw;
            obj["chan3_raw"] = rc_channels_raw.chan3_raw;
            obj["chan4_raw"] = rc_channels_raw.chan4_raw;
            obj["chan5_raw"] = rc_channels_raw.chan5_raw;
            obj["chan6_raw"] = rc_channels_raw.chan6_raw;
            obj["chan7_raw"] = rc_channels_raw.chan7_raw;
            obj["chan8_raw"] = rc_channels_raw.chan8_raw;
        break;
        //<message id="74" name="VFR_HUD">
        case MAVLINK_MSG_ID_VFR_HUD:
            mavlink_vfr_hud_t vfr_hud;
            mavlink_msg_vfr_hud_decode(&msg, &vfr_hud);
            obj = doc.createNestedObject("vfr_hud");
            obj["groundspeed"] = vfr_hud.groundspeed;
            obj["throttle"] = vfr_hud.throttle;
            obj["alt"] = vfr_hud.alt;
            obj["climb"] = vfr_hud.climb;
            obj["heading"] = vfr_hud.heading;
            obj["airspeed"] = vfr_hud.airspeed;
        break;
        //<message id="147" name="BATTERY_STATUS">
        case MAVLINK_MSG_ID_BATTERY_STATUS:
            mavlink_battery_status_t battery_status;
            obj = doc.createNestedObject("battery_status");
            mavlink_msg_battery_status_decode(&msg, &battery_status);
            obj["id"] = battery_status.id;
            obj["type"] = battery_status.type;
            //obj["voltages"] = battery_status.voltages;
            obj["vCell1"] = battery_status.voltages[0];
            obj["vCell2"] = battery_status.voltages[1];
            obj["vCell3"] = battery_status.voltages[2];
            obj["vCell4"] = battery_status.voltages[3];
            obj["temperature"] = battery_status.temperature;
            obj["charge_state"] = battery_status.charge_state;
            obj["time_remaining"] = battery_status.time_remaining;
            obj["current_battery"] = battery_status.current_battery;
            obj["energy_consumed"] = battery_status.energy_consumed;
            obj["battery_function"] = battery_status.battery_function;
            obj["current_consumed"] = battery_status.current_consumed;
            obj["battery_remaining"] = battery_status.battery_remaining;
        break;
        //<message id="253" name="STATUSTEXT">
        case MAVLINK_MSG_ID_STATUSTEXT:
            mavlink_statustext_t statustext;
            mavlink_msg_statustext_decode(&msg, &statustext);
            obj = doc.createNestedObject("statustext");
            obj["text"] = statustext.text;
            obj["severity"] = statustext.severity;
        break;

    }

    serializeJson(doc, *f);



}

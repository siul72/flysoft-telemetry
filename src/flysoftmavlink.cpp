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
    this->_myserial->setTimeout(160);
    this->_myserial->begin(this->baudrate);

    return true;
}

void FlysoftMavlink::getStream(){

    //uint size = MAVLINK_MAX_PACKET_LEN/2; //goof for 9600 baud
    uint size = MAVLINK_MAX_PACKET_LEN;
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

    //Get String from xml msg.msgid common.xml
    switch (msg.msgid) {
        //message id=30
        case MAVLINK_MSG_ID_ATTITUDE:
            mavlink_attitude_t attitude;
            mavlink_msg_attitude_decode(&msg, &attitude);
            JsonObject obj = doc.createNestedObject("attitude");
            obj["time_boot_ms"] = attitude.time_boot_ms;
            obj["roll"] = attitude.roll;
            obj["pitch"] = attitude.pitch;
            obj["yaw"] = attitude.yaw;
            obj["rollspeed"] = attitude.rollspeed;
            obj["pitchspeed"] = attitude.pitchspeed;
            obj["yawspeed"] = attitude.yawspeed;
        break;

    }

    serializeJson(doc, *f);

//    message id=147<frame timestamp="0000:00:01:43.172">message id=147</frame>
//    message id=29<frame timestamp="0000:00:01:43.172">message id=29</frame>
//    message id=253<frame timestamp="0000:00:01:44.098">message id=253</frame>
//    message id=35<frame timestamp="0000:00:01:44.098">message id=35</frame>
//    message id=1<frame timestamp="0000:00:01:44.100">message id=1</frame>
//    message id=74<frame timestamp="0000:00:01:44.100">message id=74</frame>
//    message id=0<frame timestamp="0000:00:01:44.100">message id=0</frame>
//    message id=35<frame timestamp="0000:00:01:44.101">message id=35</frame>
//    message id=35<frame timestamp="0000:00:01:44.037">message id=35</frame>
//   message id=35<frame timestamp="0000:00:01:44.219">message id=35</frame>
//     message id=74<frame timestamp="0000:00:01:44.113">message id=74</frame>
//    message id=0<frame timestamp="0000:00:01:44.113">message id=0</frame>
//    message id=147<frame timestamp="0000:00:01:45.025">message id=147</frame>
//    message id=29<frame timestamp="0000:00:01:45.026">message id=29</frame>
//    message id=253<frame timestamp="0000:00:01:45.026">message id=253</frame>
//     message id=35<frame timestamp="0000:00:01:45.185">message id=35</frame>
//    message id=1<frame timestamp="0000:00:01:45.111">message id=1</frame>
//    message id=74<frame timestamp="0000:00:01:45.112">message id=74</frame>
//    message id=0<frame timestamp="0000:00:01:45.112">message id=0</frame>
//    message id=35<frame timestamp="0000:00:01:45.178">message id=35</frame>
//     message id=35<frame timestamp="0000:00:01:45.137">message id=35</frame>
//    message id=30<frame timestamp="0000:00:01:45.137">message id=30</frame>
//     message id=1<frame timestamp="0000:00:01:45.160">message id=1</frame>
//    message id=35<frame timestamp="0000:00:01:45.161">message id=35</frame>
//    message id=74<frame timestamp="0000:00:01:45.161">message id=74</frame>
//    message id=0<frame timestamp="0000:00:01:45.162">message id=0</frame>
//    message id=30<frame timestamp="0000:00:01:46.074">message id=30</frame>
//    message id=147<frame timestamp="0000:00:01:46.074">message id=147</frame>
//    message id=29<frame timestamp="0000:00:01:46.075">message id=29</frame>
//    message id=253<frame timestamp="0000:00:01:46.075">message id=253</frame>


}

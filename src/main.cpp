#include <Arduino.h>
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <TaskScheduler.h>
#include <ESP8266Ping.h>
#include <AsyncMqttClient.h>
#include <SoftwareSerial.h>
#include "utils.h"

#define  LED_WATCHDOG 2
#define  MAVLINK_RX_PIN 4 //D2 on WeMos board
#define  MAVLINK_TX_PIN 16 //D0 on WeMos board

void saveParamCallback();
void consoleThread();
void parseConsoleRxFrame();
void wifiConnect();
void connectToMqtt();
void toggleLed();
void mavlinkThread();

Task readConsoleTask(1000, TASK_FOREVER, &consoleThread);
Task parseConsoleRxTask(40, TASK_FOREVER, &parseConsoleRxFrame);
Task wifiConnectTask(1000, TASK_ONCE, &wifiConnect);
Task mqttReconnectTask(2000, TASK_ONCE, &connectToMqtt);
Task heartbeatTask(1000, TASK_FOREVER, &toggleLed);
Task readMavlinkPortTask(40, TASK_FOREVER, &mavlinkThread);

SoftwareSerial mavlinkSerial(MAVLINK_RX_PIN, MAVLINK_TX_PIN, true);
WiFiManager wm;
WiFiManagerParameter mqtt_port_param; // global param ( for non blocking w params )
WiFiManagerParameter mqtt_address_param; // global param ( for non blocking w params )
Scheduler runner;
std::vector<String> myQueue;
AsyncMqttClient mqttClient;
uint16 mqtt_port;

char msg[1028];

void testPing(String host){

  if(Ping.ping(host.c_str())) {
    Serial.println("Ping to mqtt server ok!!");
  } else {
    Serial.println("Ping to mqtt server nok!!");
  }
}
/* MQTT Callbacks*/
void onMqttConnect(bool sessionPresent) {
  mqttReconnectTask.disable();
  runner.deleteTask(mqttReconnectTask);
  heartbeatTask.enable();
  digitalWrite(LED_WATCHDOG, !digitalRead(LED_WATCHDOG));
  Serial.println("Connected to MQTT.");
  Serial.println("Session present: ");
  Serial.println(sessionPresent);

  runner.addTask(readMavlinkPortTask);
  readMavlinkPortTask.enable();
  readMavlinkPortTask.setIterations(TASK_FOREVER);
  Serial.println("added and enable readMavlinkPortTask");


}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  digitalWrite(LED_WATCHDOG, !digitalRead(LED_WATCHDOG));
  Serial.printf("Disconnected from MQTT. %d\n", (int)reason);
  heartbeatTask.disable();

  if (WiFi.isConnected()) {

    runner.addTask(mqttReconnectTask);
    mqttReconnectTask.setIterations(TASK_ONCE);
    mqttReconnectTask.enable();
    Serial.println("mqttReconnectTask Start");
  }

  if(readMavlinkPortTask.isEnabled()){
    readMavlinkPortTask.disable();
    runner.deleteTask(readMavlinkPortTask);
    Serial.println("remove readMavlinkPortTask");
  }

}

void onMqttSubscribe(uint16_t packetId, uint8_t qos) {
  Serial.println("Subscribe acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
  Serial.print("  qos: ");
  Serial.println(qos);
}

void onMqttUnsubscribe(uint16_t packetId) {
  Serial.println("Unsubscribe acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
}

void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
  Serial.println("Publish received.");
  Serial.print("  topic: ");
  Serial.println(topic);
  Serial.print("  qos: ");
  Serial.println(properties.qos);
  Serial.print("  dup: ");
  Serial.println(properties.dup);
  Serial.print("  retain: ");
  Serial.println(properties.retain);
  Serial.print("  len: ");
  Serial.println(len);
  Serial.print("  index: ");
  Serial.println(index);
  Serial.print("  total: ");
  Serial.println(total);
}

void onMqttPublish(uint16_t packetId) {
  Serial.println("Publish acknowledged.");
  Serial.print("  packetId: ");
  Serial.println(packetId);
}

void mqttPublish(String payload) {

  if(!mqttClient.connected()){
    return;
  }

  mqttClient.publish("flysoft/telemetry/mavlink", 0, true, payload.c_str());

}
/*****************************************************************
*  Task callback definition
*/
void toggleLed(){
  digitalWrite(LED_WATCHDOG, !digitalRead(LED_WATCHDOG));
  String payload = "HB";
  mqttPublish(payload);
}

void connectToMqtt() {
  Serial.println("Connecting to MQTT...");
  digitalWrite(LED_WATCHDOG, !digitalRead(LED_WATCHDOG));
  testPing(mqtt_address_param.getValue());
  digitalWrite(LED_WATCHDOG, !digitalRead(LED_WATCHDOG));
  mqttClient.connect();
  digitalWrite(LED_WATCHDOG, !digitalRead(LED_WATCHDOG));
}

void consoleThread() {

  while(Serial.available()>0){
    // FRAME = KBUS;<Time Stamp>;<Sequence ID>;<Function Enum>;<Number of Elements>;<Element>;ENDK;
    String s=Serial.readString();

    char str[18] = "";
    Serial.println("new frame on queue");
    String frame;
    frame.concat(FRAME_HEADER);
    frame.concat(";");
    Utils::timeToString(str, sizeof(str));
    frame.concat(str);
    frame.concat(";");
    frame.concat(FRAME_FUNCTION_SEND_SINGLE_POT_VALUE);
    s.trim();
    frame.concat(s);
    frame.concat(FRAME_END);

    myQueue.push_back(frame);
    sprintf(msg, "push:%s", frame.c_str());
    Serial.println(msg);
    parseConsoleRxTask.enableIfNot();

  }

}

void mavlinkThread() {

  bool rcv_done = false;

  String frame = "";
  while(mavlinkSerial.available()>0){
    if (!rcv_done){
      char str[18] = "";
      Utils::timeToString(str, sizeof(str));
      //Serial.print(str);
      frame = frame +"<frame timestamp=\""+str+"\">";
    }
//    byte b=mavlinkSerial.read();
//    //delay(1);
//    char tmp[4];
//    sprintf(tmp, "%02x", b);
//    frame = frame + tmp + ':';
    char c = mavlinkSerial.read();
    frame = frame + c;
    rcv_done = true;
  }

  if (rcv_done) {
    Serial.println("new frame on queue");
    frame = frame +"</frame>";
    //rxQueue.push(frame);
    Serial.println(frame);
    mqttPublish(frame);
    digitalWrite(LED_WATCHDOG, !digitalRead(LED_WATCHDOG));

  }

}

void parseConsoleRxFrame(){
    Serial.println(">parseReceivedFrame");
    parseConsoleRxTask.disable();
    bool x = myQueue.size();
    sprintf(msg, "pop=%d", x);
    Serial.println(msg);
    while(!myQueue.empty() ){
      String frame = myQueue.back();
      myQueue.pop_back();
      sprintf(msg, "pop=%s", frame.c_str());
      Serial.println(msg);
      std::vector<String> myFunctionList = Utils::stringSplit(frame, ';');


      if (myFunctionList.size() < 7){
        Serial.println("WRONG_FRAME_SIZE");

        continue;
      }

      if (strcmp(myFunctionList[0].c_str(), FRAME_HEADER) != 0){
        sprintf(msg, "WRONG FRAME START expect=%s got=%s", FRAME_HEADER, myFunctionList[0].c_str());
        Serial.println(msg);

        continue;
      }

      if (strcmp(myFunctionList[6].c_str(), FRAME_END) != 0){
        sprintf(msg, "WRONG FRAME END expect=%s got=%s", FRAME_END, myFunctionList[6].c_str());
  	  Serial.println(msg);
        continue;
      }

  	//function
  	sprintf(msg, "FUNCTION %s", myFunctionList[3].c_str());
  	Serial.println(msg);
  	int function = atoi(myFunctionList[3].c_str());

  	switch(function){
  			case 1:

  			break;
  			case 2:

  			break;
  			case 3:

  			break;
        case 99:
          sprintf(msg, "FUNCTION Reset Wifi Settings called");
          Serial.println(msg);
          wm.resetSettings();
          wifiConnectTask.enable();

  			break;

  			default:
  				sprintf(msg, "FUNCTION %d unsuported", function);
  				Serial.println(msg);
  				continue;
  	}
  }
}

String getParam(String name){
   //read parameter from server, for customhmtl input
   String value;
   if(wm.server->hasArg(name)) {
     value = wm.server->arg(name);
   }
   return value;
}

void wifiConnect() {
    digitalWrite(LED_WATCHDOG, !digitalRead(LED_WATCHDOG));
    wifiConnectTask.disable();
    runner.deleteTask(wifiConnectTask);
    WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
    //reset settings - wipe credentials for testing
    //wm.resetSettings();
    // set dark theme
    wm.setClass("invert");
    // add a custom input field
    int customFieldLength = 40;
    new (&mqtt_address_param) WiFiManagerParameter("mqtt_address_id", "MQTT Address", "192.168.1.200", customFieldLength,"placeholder=\"MQTT address Placeholder\"");
    wm.addParameter(&mqtt_address_param);
    new (&mqtt_port_param) WiFiManagerParameter("mqtt_port_id", "MQTT Port", "1883", customFieldLength,"placeholder=\"MQTT port Placeholder\"");
    wm.addParameter(&mqtt_port_param);
    wm.setSaveParamsCallback(saveParamCallback);
    // auto generated AP name from chipid with password
    bool ret;
    Serial.println("Try to wifi connect...");
    ret = wm.autoConnect("password");
    if (ret){
      digitalWrite(LED_WATCHDOG, !digitalRead(LED_WATCHDOG));
      Serial.println("... connected!");

      //start MQTT task
      String mqtt_address = String(mqtt_address_param.getValue());
      //Serial.println("connected1");
      IPAddress ip;
      ip.fromString(mqtt_address_param.getValue());
      Serial.println("connected 2");
      //String mqtt_port_string = getParam("mqtt_port_id");
      mqtt_port = atoi(mqtt_port_param.getValue());
      Serial.println("connected 3");
      sprintf(msg, "Server address %s:%d", mqtt_address.c_str(), mqtt_port);
      Serial.println(msg);
      mqttClient.setServer(ip, mqtt_port);
      mqttReconnectTask.enable();

    } else {
        Serial.println("... wifi fail to auto-connect!!!!!");
    }

 }

void saveParamCallback(){
  Serial.println("[CALLBACK] saveParamCallback fired");
  Serial.println("PARAM mqtt_address_id = " + getParam("mqtt_address_id"));
  Serial.println("PARAM mqtt_port_id = " + getParam("mqtt_port_id"));
}

 void setup() {
       pinMode(LED_WATCHDOG, OUTPUT);
       pinMode(MAVLINK_RX_PIN, INPUT);
       pinMode(MAVLINK_TX_PIN, OUTPUT);
       digitalWrite(LED_WATCHDOG, !digitalRead(LED_WATCHDOG));
       Serial.begin(115200);
       mavlinkSerial.begin(57600);
       Serial.println("start logging");
       mqttClient.onConnect(onMqttConnect);
       mqttClient.onDisconnect(onMqttDisconnect);
       mqttClient.onSubscribe(onMqttSubscribe);
       mqttClient.onUnsubscribe(onMqttUnsubscribe);
       mqttClient.onMessage(onMqttMessage);
       mqttClient.onPublish(onMqttPublish);
       Serial.println("Initialized mqtt");

       runner.init();
       Serial.println("Initialized scheduler");
       runner.addTask(wifiConnectTask);
       wifiConnectTask.enable();
       Serial.println("added and enable wifiConnectTask");
       runner.addTask(readConsoleTask);
       readConsoleTask.enable();
       runner.addTask(parseConsoleRxTask);
       runner.addTask(mqttReconnectTask);
       runner.addTask(heartbeatTask);

       Serial.println("FlySoft telemetry started");

  }

void loop() {
      runner.execute();

}

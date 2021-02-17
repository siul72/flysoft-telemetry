
win32 {
    HOMEDIR += $$(USERPROFILE)
}
else {
    HOMEDIR += $$(HOME)
}

INCLUDEPATH += "$${HOMEDIR}/workspace/flysoft-telemetry/include"
INCLUDEPATH += "$${HOMEDIR}/workspace/flysoft-telemetry/src"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduinoespressif8266/libraries/SoftwareSerial/src"
INCLUDEPATH += "$${HOMEDIR}/workspace/flysoft-telemetry/.pio/libdeps/d1_mini/ArduinoJson/src"
INCLUDEPATH += "$${HOMEDIR}/workspace/flysoft-telemetry/.pio/libdeps/d1_mini/MAVLink v2 C library"
INCLUDEPATH += "$${HOMEDIR}/workspace/flysoft-telemetry/.pio/libdeps/d1_mini/AsyncMqttClient/src"
INCLUDEPATH += "$${HOMEDIR}/workspace/flysoft-telemetry/.pio/libdeps/d1_mini/ESPAsyncTCP/src"
INCLUDEPATH += "$${HOMEDIR}/workspace/flysoft-telemetry/.pio/libdeps/d1_mini/ESP8266Ping/src"
INCLUDEPATH += "$${HOMEDIR}/workspace/flysoft-telemetry/.pio/libdeps/d1_mini/Queue/src"
INCLUDEPATH += "$${HOMEDIR}/workspace/flysoft-telemetry/.pio/libdeps/d1_mini/TaskScheduler/src"
INCLUDEPATH += "$${HOMEDIR}/workspace/flysoft-telemetry/.pio/libdeps/d1_mini/WiFiManager"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduinoespressif8266/libraries/ESP8266mDNS/src"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduinoespressif8266/libraries/ESP8266WebServer/src"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduinoespressif8266/libraries/DNSServer/src"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduinoespressif8266/libraries/ESP8266WiFi/src"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduinoespressif8266/tools/sdk/include"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduinoespressif8266/tools/sdk/libc/xtensa-lx106-elf/include"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduinoespressif8266/cores/esp8266"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduinoespressif8266/tools/sdk/lwip2/include"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduinoespressif8266/variants/d1_mini"
INCLUDEPATH += "$${HOMEDIR}/workspace/flysoft-telemetry/.pio/libdeps/d1_mini/PixhawkArduinoMAVLink/src"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduinoespressif8266/libraries/ArduinoOTA"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduinoespressif8266/libraries/EEPROM"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduinoespressif8266/libraries/ESP8266AVRISP/src"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduinoespressif8266/libraries/ESP8266HTTPClient/src"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduinoespressif8266/libraries/ESP8266HTTPUpdateServer/src"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduinoespressif8266/libraries/ESP8266LLMNR"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduinoespressif8266/libraries/ESP8266NetBIOS"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduinoespressif8266/libraries/ESP8266SSDP"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduinoespressif8266/libraries/ESP8266SdFat/src"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduinoespressif8266/libraries/ESP8266WiFiMesh/src"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduinoespressif8266/libraries/ESP8266httpUpdate/src"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduinoespressif8266/libraries/Ethernet/src"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduinoespressif8266/libraries/GDBStub/src"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduinoespressif8266/libraries/Hash/src"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduinoespressif8266/libraries/LittleFS/src"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduinoespressif8266/libraries/SD/src"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduinoespressif8266/libraries/SDFS/src"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduinoespressif8266/libraries/SPI"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduinoespressif8266/libraries/SPISlave/src"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduinoespressif8266/libraries/Servo/src"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduinoespressif8266/libraries/TFT_Touch_Shield_V2"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduinoespressif8266/libraries/Ticker/src"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduinoespressif8266/libraries/Wire"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/framework-arduinoespressif8266/libraries/esp8266/src"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/toolchain-xtensa/xtensa-lx106-elf/include/c++/4.8.2"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/toolchain-xtensa/xtensa-lx106-elf/include/c++/4.8.2/xtensa-lx106-elf"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/toolchain-xtensa/lib/gcc/xtensa-lx106-elf/4.8.2/include-fixed"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/toolchain-xtensa/lib/gcc/xtensa-lx106-elf/4.8.2/include"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/toolchain-xtensa/xtensa-lx106-elf/include"
INCLUDEPATH += "$${HOMEDIR}/.platformio/packages/tool-unity"

DEFINES += "PLATFORMIO=50100"
DEFINES += "ESP8266"
DEFINES += "ARDUINO_ARCH_ESP8266"
DEFINES += "ARDUINO_ESP8266_WEMOS_D1MINI"
DEFINES += "F_CPU=80000000L"
DEFINES += "__ets__"
DEFINES += "ICACHE_FLASH"
DEFINES += "ARDUINO=10805"
DEFINES += "ARDUINO_BOARD=&quot;PLATFORMIO_D1_MINI&quot;"
DEFINES += "FLASHMODE_DIO"
DEFINES += "LWIP_OPEN_SRC"
DEFINES += "NONOSDK22x_190703=1"
DEFINES += "TCP_MSS=536"
DEFINES += "LWIP_FEATURES=1"
DEFINES += "LWIP_IPV6=0"
DEFINES += "VTABLES_IN_FLASH"

OTHER_FILES += platformio.ini

HEADERS += src/flysoftmavlink.h \
    src/mqtt_message.h
SOURCES += src/utils.cpp
HEADERS += src/utils.h
SOURCES += src/flysoftmavlink.cpp
HEADERS += src/queue.h
SOURCES += src/main.cpp

CONFIG += c++11

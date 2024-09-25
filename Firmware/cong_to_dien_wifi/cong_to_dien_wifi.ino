

// Enable debug console
// Set CORE_DEBUG_LEVEL = 3 first
#define ERA_DEBUG

#define DEFAULT_MQTT_HOST "mqtt1.eoh.io"

// You should get Auth Token in the ERa App or ERa Dashboard
#define ERA_AUTH_TOKEN "563f9ab3-7e9c-4a0b-9973-455c6fb9db6e"

#include <Arduino.h>
#include <ERa.hpp>
#include <ERa/ERaTimer.hpp>
#include <PZEM004Tv30.h>

#if defined(ESP32)
PZEM004Tv30 pzem(Serial2, 16, 17);
#else
PZEM004Tv30 pzem(Serial2);
#endif

float unitPrice = 0;
const char ssid[] = "ChienNguyen";
const char pass[] = "0826009929";

ERaTimer timer;

// Nhận dữ liệu từ server gửi xuống dùng để reset lại số điện hằng tháng và đơn giá tiền điện
ERA_WRITE(V0)
{
  int value = param.getInt();
  if (value == 1){
     pzem.resetEnergy();
     ERa.virtualWrite(V0, value);
  }
}

ERA_WRITE(V7)
{
  unitPrice = param.getInt(); 
}

/* This function print uptime every second */
void timerEvent() {
   
  ERa.virtualWrite(V1,  pzem.voltage());
  ERa.virtualWrite(V2,  pzem.current());
  ERa.virtualWrite(V3,  pzem.power());
  ERa.virtualWrite(V4,  pzem.energy());
  ERa.virtualWrite(V5,  pzem.frequency());
  ERa.virtualWrite(V6,  pzem.pf());
  ERa.virtualWrite(V8, unitPrice * (pzem.energy()));
    
  ERA_LOG("Timer", "Uptime: %d", ERaMillis() / 1000L);

}

void setup() {
    /* Setup debug console */
    Serial.begin(115200);

    pinMode(2,OUTPUT);

    ERa.begin(ssid, pass);
    

    /* Setup timer called function every second */
    timer.setInterval(1000L, timerEvent);
}

void loop() {
    ERa.run();
    timer.run();
}


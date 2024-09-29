#include "Arduino.h"
#include <ESP32Servo.h>
#include <ArduinoJson.h>

// 馬達_宣告區
Servo Left_Tire;
Servo Right_Tire;

// -----------
// json_控制_宣告區
DeserializationError json_control_error;
StaticJsonDocument<256> json_control_data;
String json_control_msg = "";

// ----------------
void setup()
{
  Serial.begin(115200);
  Drive_Init();
}

void loop()
{
  json_control_read();
}

// json_控制_工作區
void json_control_read() {
  if (Serial.available() > 0) {
    char data = Serial.read();
    if (data == '\n') {
      json_control_error = deserializeJson(json_control_data, json_control_msg);
      if (!json_control_error) {
        // Read the target_vel array from the JSON
        JsonArray target_vel = json_control_data["target_vel"];
        if (target_vel.size() == 2) {
          json_control_set_left_speed(target_vel[0]);
          json_control_set_right_speed(target_vel[1]);
        }
      }
      json_control_msg = "";
    } else {
      json_control_msg += data;
    }
  }
}

void json_control_set_left_speed(int data) {
  int speed = data;
  if (1000 >= abs(speed)) {
    speed = map(speed, -1000, 1000, 1000, 2000);
    Left_Tire.writeMicroseconds(speed);
  }
}

void json_control_set_right_speed(int data) {
  int speed = data;
  if (1000 >= abs(speed)) {
    speed = map(speed, -1000, 1000, 1000, 2000);
    Right_Tire.writeMicroseconds(speed);
  }
}

// 馬達控制_函式區
void Drive_Init() {
  // 伺服設定 ( 角位 , 最低頻率 , 最高頻率 )
  Left_Tire.attach(26, 1000, 2000);
  Right_Tire.attach(25, 1000, 2000);
}

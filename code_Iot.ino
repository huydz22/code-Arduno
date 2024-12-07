#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL68ea6o6_z"
#define BLYNK_TEMPLATE_NAME "Ngohuy ASM"
#define BLYNK_AUTH_TOKEN "EJX2WsetCoQH0qiHLY14jOiN3eKDr3yM"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

#include <Servo.h>
/* ********************************************************************** */
#define     ENABLE_A      D5
#define     ENABLE_B      D5
#define     IN_DC_1       D4
#define     IN_DC_2       D3
#define     IN_DC_3       D2
#define     IN_DC_4       D1

#define     SERVOPIN      D8

#define     TRIG          D6
#define     ECHO          D7

#define     DIS_STOP      35
/* ********************************************************************** */
Servo myservo;
unsigned char speed_rate = 0;
unsigned char dis_right, dis_left, distance;
unsigned char pos;
unsigned char mode_run = 0, mode_hendel = 0;
unsigned char buffer_blynk[5];
char ssid[] = "Huy2.4G";
char pass[] = "99999999y";


void setup() {
  serial_Setup();
  servor_Setup();
  HCSR_Setup();
  motor_Setup();
  set_speed_rate(100);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop() {
  if(buffer_blynk[0] == 0){
    Blynk.virtualWrite(V5, 0);
    mode_hendel_();
  }
  else{
    blynk_run();
    mode_hendel = 0;
  }
  Blynk.run();
}
/* ********************************************************************** */
void mode_hendel_(void) {
  switch (mode_hendel) {
    case 0:
      distance = dis_servo();
      Serial.print("distance:  " + (String)distance + "\r\n");
      if (distance < DIS_STOP) {
        Blynk.virtualWrite(V5, 1);
        Stop();
        delay(2000);
        sevor_pos(0);
        dis_right = dis_servo();
        Serial.print("dis_right:  " + (String)dis_right + "\r\n");
        delay(2000);

        sevor_pos(180);
        dis_left = dis_servo();
        Serial.print("dis_left:  " + (String)dis_left + "\r\n");
        delay(2000);
        sevor_pos(90);
        if (dis_right < DIS_STOP && dis_left < DIS_STOP) mode_hendel = 4;
        else {
          if (dis_right > dis_left) {
            mode_hendel = 2;
          }
          else {
            mode_hendel = 3;
          }
        }
      }
      else {
        mode_hendel = 1;
      }
      break;
    case 1:
      forward();
      mode_hendel = 0;
      break;
    case 2:
      left();
      delay(800);
      mode_hendel = 0;
      break;
    case 3:
      right();
      delay(800);
      mode_hendel = 0;
      break;
    case 4:
      right();
      delay(1500);
      mode_hendel = 0;
      break;

  }
}
/* ********************************************************************** */
void blynk_run(void){
  if(buffer_blynk[1]) forward();
  else if(buffer_blynk[2]) back();
  else if(buffer_blynk[3]) left();
  else if(buffer_blynk[4]) right();
  else Stop();
}
/* ********************************************************************** */
void serial_Setup(void) {
  Serial.begin(115200);
  Serial.print("\r\n");
  Serial.print("SETUP_UART \r\n");
}
/* ********************************************************************** */
void servor_Setup(void) {
  myservo.attach(SERVOPIN);
  Serial.print("SEVOR SETUP \r\n");
}
/* ********************************************************************** */
void sevor_pos(unsigned char pos) {
  myservo.write(pos);
  Serial.print("SEVRO:  " + (String)pos + "\r\n");
}
/* ********************************************************************** */
void HCSR_Setup(void) {
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  Serial.print("HCSR04 SETUP \r\n");
}
/* ********************************************************************** */
unsigned char dis_servo(void) {
  unsigned long duration;
  unsigned char dis;
  digitalWrite(TRIG, 0);
  delayMicroseconds(2);
  digitalWrite(TRIG, 1);
  delayMicroseconds(5);
  digitalWrite(TRIG, 0);

  duration = pulseIn(ECHO, HIGH);
  dis = int(duration / 2 / 29.412);

  Serial.print("DIS:  " + (String)dis + "\r\n");
  return dis;
}
/* ********************************************************************** */
void motor_Setup(void) {
  pinMode(ENABLE_A, OUTPUT);
  pinMode(ENABLE_B, OUTPUT);
  pinMode(IN_DC_1, OUTPUT);
  pinMode(IN_DC_2, OUTPUT);
  pinMode(IN_DC_3, OUTPUT);
  pinMode(IN_DC_4, OUTPUT);

  digitalWrite(IN_DC_1, LOW);
  digitalWrite(IN_DC_2, LOW);
  digitalWrite(IN_DC_3, LOW);
  digitalWrite(IN_DC_4, LOW);

  analogWrite(ENABLE_A, speed_rate);
  analogWrite(ENABLE_B, speed_rate);
  Serial.print("MOTOR SETUP \r\n");
}
/* ********************************************************************** */
void forward(void) {
  digitalWrite(IN_DC_1, LOW);
  digitalWrite(IN_DC_2, HIGH);
  digitalWrite(IN_DC_3, LOW);
  digitalWrite(IN_DC_4, HIGH);
  Serial.print("FORWARD \r\n");
}
/* ********************************************************************** */
void back(void) {
  digitalWrite(IN_DC_1, HIGH);
  digitalWrite(IN_DC_2, LOW);
  digitalWrite(IN_DC_3, HIGH);
  digitalWrite(IN_DC_4, LOW);
  Serial.print("BACK \r\n");
}
/* ********************************************************************** */
void left(void) {
  digitalWrite(IN_DC_1, LOW);
  digitalWrite(IN_DC_2, HIGH);
  digitalWrite(IN_DC_3, HIGH);
  digitalWrite(IN_DC_4, LOW);
  Serial.print("LEFT \r\n");
}
/* ********************************************************************** */
void right(void) {
  digitalWrite(IN_DC_1, HIGH);
  digitalWrite(IN_DC_2, LOW);
  digitalWrite(IN_DC_3, LOW);
  digitalWrite(IN_DC_4, HIGH);
  Serial.print("RIGHT \r\n");
}
/* ********************************************************************** */
void Stop(void) {
  digitalWrite(IN_DC_1, LOW);
  digitalWrite(IN_DC_2, LOW);
  digitalWrite(IN_DC_3, LOW);
  digitalWrite(IN_DC_4, LOW);
  Serial.print("STOP \r\n");
}
/* ********************************************************************** */
void set_speed_rate(unsigned char new_speed_rate) {
  speed_rate = new_speed_rate;
  Serial.print("SETUP:  " + (String)speed_rate + "\r\n");
  analogWrite(ENABLE_A, speed_rate);
  analogWrite(ENABLE_B, speed_rate);
}
/* ********************************************************************** */

BLYNK_WRITE(V0) {
  buffer_blynk[0] = param.asInt();
  Serial.println("Biến MODE:" + (String)buffer_blynk[0]);
}
BLYNK_WRITE(V1) {
  buffer_blynk[1] = param.asInt();
  Serial.println("Biến tiến:" + (String)buffer_blynk[1]);
}
BLYNK_WRITE(V2) {
  buffer_blynk[2] = param.asInt();
  Serial.println("Biến lùi:" + (String)buffer_blynk[2]);
}
BLYNK_WRITE(V3) {
  buffer_blynk[3] = param.asInt();
  Serial.println("Biến trái:" + (String)buffer_blynk[3]);
}
BLYNK_WRITE(V4) {
  buffer_blynk[4] = param.asInt();
  Serial.println("Biến phải:" + (String)buffer_blynk[4]);
}

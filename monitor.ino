//เหลือเพิ่ม ข้อความ oled
//เพิ่มline token
//เพิ่มultrasonoic  อีก1 ตัว

#define BLYNK_TEMPLATE_ID "TMPL6HY6dJQVu" //กำหนดaddress id จาก blynk
#define BLYNK_TEMPLATE_NAME "Taobin"  // กำหนดaddress ชื่อ จาก blynk
#define BLYNK_AUTH_TOKEN "NDrnDZKpGs0lV5M_mBFBukws5j8Ipvko" //กำหนด adress token จาก blynk
#define LINE_TOKEN "line id มึงอ่ะ ไปก๊อปมา"

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define OLED_RESET -1
Adafruit_SSD1306 OLED(OLED_RESET);
#include <WiFi.h> //เรียกใช้libary wifi esp 32
#include <BlynkSimpleEsp32.h>  // เรียกใช้ blynk esp32
#include <TridentID_LineNotify.h>

char auth[] = BLYNK_AUTH_TOKEN; // นำadress blynk token เข้าไปเก็บในอาเรย์ชื่อ auth
char ssid[] = "MAKOTO"; // นำ ชื่อwifi adress ที่เชื่อมเข้ากับอาเรย์ชื่อ
char pass[] = "Suphawit2410"; //นำ  wifi password ที่เชื่อมเข้ากับอาเรย์ชื่อ

// กำหนด pin triger และ echo ที่เชื่อมกับบอร์ด esp32

#define trig_pin1 13
#define echo_pin1 12 

#define trig_pin2 14
#define echo_pin2 27

#define trig_pin3 26
#define echo_pin3 25

#define trig_pin4 26
#define echo_pin4 25
//  กำหนด pin ที่เชื่อมกับ relay
#define relay_soda 19
#define relay_milk 18
#define relay_redwater 23
#define relay_bluewater 5
// กำหนด ขาจำลอง blynk
#define Virtual_redsoda V1
#define Virtual_pinkmilk V2
#define Virtual_bluewater V3
#define VIRSUAL_WATER_LEVEL1 V4
#define VIRSUAL_WATER_LEVEL2 V5
#define VIRSUAL_WATER_LEVEL3 V6
#define VIRSUAL_WATER_LEVEL4 V7

#define Virtual_level V0

int time = 0;

const int tank_height = 22;
int water_level_percent1 = 0;
int water_level_percent2 = 0;
int water_level_percent3 = 0;
int water_level_percent4 = 0;
bool hasNotified = false;
bool isRelayRunning = false ;

void checkConnection(){
  if (!WiFi.isConnected()){
    Wifi.begin(ssid,password);
    while(Wifi.statues() ! = WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    } 
    Serial.println("Connect to WiFi");
  }
  if(!Blynk.connected()) {
    Blynk.connect();
  }
}



void bluesoda(){
  isRelayRunning = true;
  digitalWrite(relay_bluewater, LOW);
  delay(time);
  digitalWrite(relay_bluewater, HIGH);

delay(2000);

  digitalWrite(relay_soda,LOW);
  delay(12000-time);
  digitalWrite(relay_soda,HIGH);
  isRelayRunning = false;
}
void redsoda(){
  isRelayRunning = true;
  digitalWrite(relay_redwater, LOW);
  delay(time);
  digitalWrite(relay_redwater, HIGH);

delay(2000);

  digitalWrite(relay_soda,LOW);
  delay(12000-time);
  digitalWrite(relay_soda,HIGH);
  isRelayRunning = false;
}

void pinkmilk(){
 isRelayRunning = true;
 digitalWrite(relay_redwater, LOW);
 delay(time);
 digitalWrite(relay_redwater, HIGH);
 
 delay(2000);
 
 digitalWrite(relay_milk, LOW);
 delay(12000-time);
 digitalWrite(relay_milk, HIGH);
   isRelayRunning = false;

}

BLYNK_WRITE(VIRTUAL_redsoda){
  int relayState = param.asInt();
  if (relayState == 1 && !isRelayRunning) {
    redsoda();
  }
}

BLYNK_WRITE(VIRTUAL_pinkmilk) {
  int relayState = param.asInt();
  if (relayState == 1 && ! isRelayRunning) {
    pinkmilk();
  }
}

BLYNK_WRITE(VIRTUAL_bluewater) {
  int relayState = param.asInt();
  if (relayState == 1 && ! isRelayRunning) {
    bluesoda();
  }
}

BLYNK_WRITE(Virtual_level) {
  int relayState = param,asInt();
  if(relayState == 1){
  time = 6000;
  }
  else if(relayState == 2){
    time = 8000;
  }
else(relaystate == 3){
  time = 10000;
}
}

void setup(){ 
  OLED_begin(SSD1306_SWITCHCAPVCC,0x3c);

  pinMode(trig_PIN1,OUTPUT);
  pinMode(trig_PIN2,OUTPUT);
  pinMode(trig_PIN3,OUTPUT);
  pinMode(echo_PIN1,INPUT);
  pinMode(echo_PIN2,INPUT);
  pinMode(echo_PIN3,INPUT);
  pinMode(relay_soda,OUTPUT);
  pinMode(relay_milk,OUTPUT);
  pinMode(relay_redwater,OUTPUT);
  pinMode(relay_purplewater,OUTPUT);
  digitalWrite(relay_soda,HIGH);
  digitalWrite(relay_milk,HIGH);
  digitalWrite(relay_redwater,HIGH);
  digitalWrite(relay_purplewater,HIGH);

  Blynk.begin(auth,ssid,pass);// รับค่า และ เริ่มต้น auth ssid pass ที่ blynk 
  LINE.setToken(LINE_TOKEN);


}

void loop(){
  checkConnection();
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  digitalWrite(trig_PIN1, LOW);
  delayMicroseconds(5);
  digitalWrite(trig_PIN1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_PIN1, LOW);

  long duration1 = pulawIn(echo_PIN1,HIGH);
  int distrance1 = duration1/ 58;
  water_level_percent1 = max(0,min(100,(tank_height - distrance1)*100/tank_height));
  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    digitalWrite(trig_PIN2, LOW);
  delayMicroseconds(5);
  digitalWrite(trig_PIN2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_PIN2, LOW);


  long duration2 = pulawIn(echo_PIN1,HIGH);
  int distrance2 = duration2/ 58;
  water_level_percent2 = max(0,min(100,(tank_height - distrance2)*100/tank_height));
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  digitalWrite(trig_PIN3, LOW);
  delayMicroseconds(5);
  digitalWrite(trig_PIN3, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_PIN3, LOW);


  long duration3 = pulawIn(echo_PIN1,HIGH);
  int distrance3 = duration3/ 58;
  water_level_percent3 = max(0,min(100,(tank_height - distrance3)*100/tank_height));
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  digitalWrite(trig_PIN4, LOW);
  delayMicroseconds(5);
  digitalWrite(trig_PIN4, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_PIN4, LOW);


  long duration4 = pulawIn(echo_PIN4,HIGH);
  int distrance4 = duration4/ 58;
  water_level_percent4 = max(0,min(100,(tank_height - distrance4)*100/tank_height));
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  if (water_level_percent1 <= 50 && water_lecel_percent1 >= 40 && !hasNotified){
    LINE.notify("ระดับน้ำ : " + String(water_level_percent1)+"%");
    hasNotified = true;
  }else if (water_level_percent1 > 50) {
    hasNotifield = false;
  }

  if (water_level_percent2 <= 50 && water_lecel_percent2 >= 40 && !hasNotified){
    LINE.notify("ระดับน้ำ : " + String(water_level_percent2)+"%");
    hasNotified = true;
  }else if (water_level_percent2 > 50) {
    hasNotifield = false;
  }

  if (water_level_percent3 <= 50 && water_lecel_percent3 >= 40 && !hasNotified){
    LINE.notify("ระดับน้ำ : " + String(water_level_percent3)+"%");
    hasNotified = true;
  }else if (water_level_percent3 > 50) {
    hasNotifield = false;
  }

  if (water_level_percent4 <= 50 && water_lecel_percent4 >= 40 && !hasNotified){
    LINE.notify("ระดับน้ำ : " + String(water_level_percent4)+"%");
    hasNotified = true;
  }else if (water_level_percent4 > 50) {
    hasNotifield = false;
  }

  Blynk.VirtualWrite(VIRTUAL_WATER_LEVEL1,water_level_percent1);
  Blynk.VirtualWrite(VIRTUAL_WATER_LEVEL2,water_level_percent2);
  Blynk.VirtualWrite(VIRTUAL_WATER_LEVEL3,water_level_percent3);
  Blynk.VirtualWrite(VIRTUAL_WATER_LEVEL4,water_level_percent4);

  
  Blynk.run();//blynk เริ่มทำงาน

  delay(4000);
}
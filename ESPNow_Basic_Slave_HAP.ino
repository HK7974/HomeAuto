
#include <esp_now.h>
#include <WiFi.h>


#define CHANNEL 1
#define CHANNELSTA 2
const char* ssidsta     = "AC-130";
const char* passwordsta = "up above";
uint8_t dataRecieved;
int led1 = 19;
int led2 = 21;
int led3 = 17;
int led4 = 16;
bool led1s=1;bool led2s =1;
bool led3s =1;bool led4s=1;

void setup() {
  pinMode(led1, OUTPUT);
  pinMode(led2,OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  digitalWrite(led1,led1s);
  digitalWrite(led2,led2s);
  digitalWrite(led3,led3s);
  digitalWrite(led4,led4s);
  Serial.begin(115200);
  //WiFi.mode(WIFI_AP);
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP("HAP_1","HAP_1_PASSWORD",CHANNEL,0);
  esp_now_init();
  esp_now_register_recv_cb(OnDataRecv);
}

void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
 // Serial.print("Last Packet Recv from: "); Serial.println(macStr);
  //Serial.print("Last Packet Recv Data: "); Serial.println(*data);
  //Serial.println("");
  memcpy(&dataRecieved,data,sizeof(dataRecieved));
}

void loop() {
  if (dataRecieved){
  Serial.println(dataRecieved);
  delay(100);
  }

if (dataRecieved == 1){
  led1s = !led1s;
  digitalWrite(led1,led1s);
}

if (dataRecieved == 2){
  led2s = !led2s;
  digitalWrite(led2,led2s);
}
if (dataRecieved == 3){
  led3s = !led3s;
  digitalWrite(led3,led3s);
}
if (dataRecieved == 4){
  led4s = !led4s;
  digitalWrite(led4,led4s);
}
dataRecieved=0;

}


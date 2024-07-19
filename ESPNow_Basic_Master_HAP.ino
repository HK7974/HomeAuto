
#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h> 

esp_now_peer_info_t slave;
#define CHANNEL 1

uint8_t data;
uint8_t pinNo = 0; 
int pin1 = 21, pin2 = 19, pin3 = 17, pin4 = 16;

void setup() {
  pinMode(pin1,INPUT);
  pinMode(pin2,INPUT);
  pinMode(pin3,INPUT);
  pinMode(pin4,INPUT);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  esp_now_init();
  esp_now_register_send_cb(OnDataSent);
  ScanForSlave();
  esp_now_add_peer(&slave);
  
  
}

void loop() {
  pinRead(&data,pin1,pin2,pin3,pin4);
  
  delay(500);
}

void ScanForSlave() {
  int8_t scanResults = WiFi.scanNetworks(false, false, false, 300, CHANNEL);
  bool slaveFound = 0;
  memset(&slave, 0, sizeof(slave));

  Serial.println("");
  if (scanResults == 0) {
    Serial.println("No WiFi devices in AP Mode found");
  } else {
    Serial.print("Found "); Serial.print(scanResults); Serial.println(" devices ");
    for (int i = 0; i < scanResults; ++i) {
      String SSID = WiFi.SSID(i);
      String BSSIDstr = WiFi.BSSIDstr(i);

      delay(10);
      if (SSID.indexOf("HAP") == 0) {
        Serial.println("Found a Slave.");
        int mac[6];
        if ( 6 == sscanf(BSSIDstr.c_str(), "%x:%x:%x:%x:%x:%x",  &mac[0], &mac[1], &mac[2], &mac[3], &mac[4], &mac[5] ) ) {
          for (int ii = 0; ii < 6; ++ii ) {
            slave.peer_addr[ii] = (uint8_t) mac[ii];
          }
        }

        slave.channel = CHANNEL; 
        slave.encrypt = 0; 

        slaveFound = 1;
        break;
      }
    }
  }

  if (slaveFound) {
    Serial.println("Slave Found, processing..");
  } else {
    Serial.println("Slave Not Found, trying again.");
  }
  WiFi.scanDelete();
}


void pinRead (uint8_t *datatosend,int pin1,int pin2,int pin3,int pin4){
  if(bool state1 = digitalRead(pin1)){*datatosend = 1;esp_now_send(slave.peer_addr,&data,sizeof(data));}
  //delay(10);
  if(bool state2 = digitalRead(pin2)){*datatosend = 2;esp_now_send(slave.peer_addr,&data,sizeof(data));}
  if(bool state2 = digitalRead(pin3)){*datatosend = 3;esp_now_send(slave.peer_addr,&data,sizeof(data));}
  if(bool state2 = digitalRead(pin4)){*datatosend = 4;esp_now_send(slave.peer_addr,&data,sizeof(data));}
}

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print("Last Packet Sent to: "); Serial.println(macStr);
  Serial.print("Last Packet Send Status: "); Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}



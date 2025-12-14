#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <rdm6300.h>

#include <WebSerial.h>
#include <ESPAsyncTCP.h>
#include <AsyncElegantOTA.h> //web ota lib
#include <ESP8266HTTPClient.h>
#include <ESPAsyncWebServer.h>
#include <ESPAsyncWiFiManager.h>         //https://github.com/tzapu/WiFiManager

#define RDM6300_1_RX_PIN 5 //D1 Pin in
#define RDM6300_2_RX_PIN 4 //D2 Pin out

AsyncWebServer server(80);
DNSServer dns;

#define relay_up  13 //d7
#define relay_down 12 //D6
#define relay_stop 15 //D8
#define buzzer A0 //
#define ir  14 //D5

Rdm6300 rdm_in;
Rdm6300 rdm_out;

int IR, flag = 0 ;
String read_data = "", read_data2 = "", dat = "", prev_data = "", prev_data2 = "", Chipid = "", payload = "";
bool card_status1 = true, card_status2 = true, shutterInSystem, shutterOutSystem, rdm1_read, rdm2_read;
unsigned long lastTime1 = 0, lastTime2 = 0, ShutterInTime, ShutterOutTime;

// config.h (do not push to GitHub)

const char* SERVER_URL = "http://yourserver.com/room_access_control.php";
String API_KEY = "ApIkey";



void setup() {
  Serial.begin(115200);

  rdm_in.begin(RDM6300_1_RX_PIN, 1);
  rdm_out.begin(RDM6300_2_RX_PIN, 2);

  pinMode(ir, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(relay_up, OUTPUT);
  pinMode(relay_down, OUTPUT);
  pinMode(relay_stop, OUTPUT);

  shutterInSystem = false;
  shutterOutSystem = false;
  rdm1_read = false;
  rdm2_read = false;

  AsyncWiFiManager wifiManager(&server, &dns);
  WiFi.mode(WIFI_STA);
  bool res = wifiManager.autoConnect("Parking System");

  if (!res) {
    Serial.println(F("Failed to connect"));
    ESP.restart();
  }
  else {
    //if you get here you have connected to the WiFi
    Serial.println(F("Device connected... :)"));
  }

  WebSerial.begin(&server);
  //  WebSerial.msgCallback(recvMsg);
  AsyncElegantOTA.begin(&server, "ABC", "12345");    // Start ElegantOTA
  server.begin();
  code_up_dt();
  //  chipID();
  Serial.println(WiFi.localIP());
  //  Chipid =  String(ESP.getChipId(), DEC);
  Chipid = "2490368";
  Serial.print("Chip ID: "); Serial.println(Chipid);
  digitalWrite(relay_down, 1);
  digitalWrite(relay_up, 1);
  digitalWrite(relay_stop, 1);
}

void loop() {
  RDM_WORK();
  IR = digitalRead(ir);
  check_milis();
}

void RDM1() {
  if (rdm_in.get_new_tag_id()) {
    String lengthCheck = String(rdm_in.get_tag_id(), DEC);
    rdm1_read = true;
    readDataLength(lengthCheck.length());
    read_data = dat;
    read_data += lengthCheck;
    Serial.print(F("RDM1: "));
    Serial.println(read_data);
    digitalWrite(buzzer, 1);
    delay(10);
    digitalWrite(buzzer, 0);
    if (((millis() - lastTime1) > 5000) || (read_data != prev_data)) {
      prev_data = read_data;
      card_status1 = true;
      lastTime1 = millis();
      if (card_status1 == true) {
        Serial.println(F("Sending to DB"));
        user_check_IN();
        card_status1 = false;
        rdm1_read = false;
        Serial.println("card_status1 false");
      }
    }
    else {
      digitalWrite(LED_BUILTIN, HIGH);
      rdm1_read = false;
    }
  }
}

void RDM2() {
  if (rdm_out.get_new_tag_id()) {
    String lengthCheck1 = String(rdm_out.get_tag_id(), DEC);
    rdm2_read = true;
    readDataLength(lengthCheck1.length());
    read_data2 = dat;
    read_data2 += lengthCheck1;
    digitalWrite(buzzer, 1);
    delay(10);
    digitalWrite(buzzer, 0);
    Serial.print(F("RDM2: "));
    Serial.println(read_data2);
    if (((millis() - lastTime2) > 5000) || (read_data2 != prev_data2)) {
      prev_data2 = read_data2;
      card_status2 = true;
      lastTime2 = millis();
      if (card_status2 == true) {
        Serial.println(F("Sending to DB"));
        user_check_out();
        card_status2 = false;
        Serial.println("card_status2 false");
      }
    }
    else {
      rdm2_read = false;
      digitalWrite(LED_BUILTIN, HIGH);
    }
  }
}

void user_check_IN() {
  Serial.println(F("User Check"));
  WebSerial.println(F("User Check"));
  WiFiClient client;
  HTTPClient http;
  http.begin(client, serverName2);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  String httpRequestData =  "api_key=" + apiKeyValue +
                            "&card_number=" + String(read_data) +
                            "&device_id=" + String(Chipid) +
                            "";
  Serial.print(F("httpRequestData: "));
  Serial.println(httpRequestData);
  int httpResponseCode = http.POST(httpRequestData);
  payload = http.getString();                  //Get the response payload
  if (httpResponseCode > 0) {
    WebSerial.print(F("HTTP: "));
    WebSerial.println(httpResponseCode);
    WebSerial.println(F("Payload"));
    WebSerial.println(payload);
    if (payload == Chipid) {
      WebSerial.println("Employee Founded");
      //      DoorAuto_in();
      shutterAuto_in();
      WebSerial.println("Sutter Auto in");
    } else {
      WebSerial.println("Wrong Card");
      //      mysql_log(0, "IN", read_data, "wrong card");
      card_status1 = false;
      card_status2 = false;
      digitalWrite(LED_BUILTIN, 0);
    }
  } else {
    rdm_in.end();
    rdm_in.begin(RDM6300_1_RX_PIN, 1);
  }
  http.end();
}

void user_check_out() {
  WebSerial.println(F("User Check"));
  WiFiClient client;
  HTTPClient http;
  http.begin(client, serverName2);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  String httpRequestData =  "api_key=" + apiKeyValue +
                            "&card_number=" + String(read_data2) +
                            "&device_id=" + String(Chipid) +
                            "";
  WebSerial.print(F("httpRequestData: "));
  WebSerial.println(httpRequestData);
  int httpResponseCode = http.POST(httpRequestData);
  payload = http.getString();                  //Get the response payload
  if (httpResponseCode > 0) {
    WebSerial.print(F("HTTP: "));
    WebSerial.println(httpResponseCode);
    WebSerial.println(F("Payload"));
    WebSerial.println(payload);    //Print request response payload
    if (payload == Chipid) {
      WebSerial.println("Employee Founded");
      shutterAuto_out();
      WebSerial.println("Sutter Auto out");
      rdm2_read = false;
    } else {
      digitalWrite(LED_BUILTIN, 0);
      card_status1 = false;
      card_status2 = false;
      rdm2_read = false;
      WebSerial.println("Wrong Card");
    }
  } else {
    rdm_out.end();
    rdm_out.begin(RDM6300_2_RX_PIN, 2);
  }
  http.end();
}

void readDataLength(int dataLength) {
  if (dataLength == 2) {
    dat = "00000000";
  } else if (dataLength == 3) {
    dat = "0000000";
  } else if (dataLength == 4) {
    dat = "000000";
  } else if (dataLength == 5) {
    dat = "00000";
  } else if (dataLength == 6) {
    dat = "0000";
  } else if (dataLength == 7) {
    dat = "000";
  } else if (dataLength == 8) {
    dat = "00";
  } else if (dataLength == 9) {
    dat = "0";
  } else if (dataLength == 10) {
    dat = "";
  }
}

void code_up_dt() {
  Serial.print(F(__FILE__));                          // Always display sketch name and date info
  Serial.print("\t");
  Serial.print(F(__DATE__));
  Serial.print("\t");
  Serial.println(F(__TIME__));
}

void shutter_closed() {
  digitalWrite(relay_down, 1);
  delay(50);
  digitalWrite(relay_down, 0);
}

void shutter_open() {
  digitalWrite(relay_up, 1);
  delay(50);
  digitalWrite(relay_up, 0);
}

void shutterAuto_in() {
  shutter_open();
  Serial.println("Door Open relay on");
  shutterInSystem = true;
  ShutterInTime = millis();
}

void shutterAuto_out() {
  shutter_open();
  Serial.println("Door Open relay on");
  shutterOutSystem = true;
  ShutterOutTime = millis();
}

void check_milis() {
  if (shutterInSystem)
    if (millis() - ShutterInTime > 10000) {
      if (IR == 1) {
        shutter_closed();
        Serial.println("Shutter Colsed normally");
        shutterInSystem = false;
        rdm1_read = false;
        flag = 0;
      }
    } else if (IR == 0) {
      flag = 1;
      Serial.println("flag value is 1");
    } else if (flag == 1) {
      if (IR == 1) {
        shutter_closed();
        Serial.println("Shutter Colsed bike passing completed!");
        shutterInSystem = false;
        rdm1_read = false;
        flag = 0;
      }
    }

  if (shutterOutSystem)
    if (millis() - ShutterOutTime > 10000) {
      if (IR == 1) {
        shutter_closed();
        Serial.println("Shutter Colsed normally");
        shutterOutSystem = false;
        rdm2_read = false;
        flag = 0;
      }
    } else if (IR == 0) {
      flag = 1;
      Serial.println("flag value is 1");
    } else if (flag == 1) {
      if (IR == 1) {
        shutter_closed();
        Serial.println("Shutter Colsed bike passing completed!");
        shutterOutSystem = false;
        rdm2_read = false;
        flag = 0;
      }
    }
}

void RDM_WORK() {
  if (rdm1_read == 0 && rdm2_read == 0 && IR == 1) {
    RDM1();//welcome
    RDM2();
  }
}

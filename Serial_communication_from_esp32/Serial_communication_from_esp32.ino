#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <common.h>
#include <Firebase.h>
#include <FirebaseESP32.h>
#include <FirebaseFS.h>
#include <Utils.h>

//#include "FirebaseESP8266.h"  // Install Firebase ESP8266 library
//#include <ESP8266WiFi.h>


#define FIREBASE_HOST "wifitrackernodemcu-default-rtdb.asia-southeast1.firebasedatabase.app/" //Without http:// or https://
#define FIREBASE_AUTH "DNSWFUivtm839z6CqUNsgy6VvfMvmYvrc0Ty6ShZ"
//#define WIFI_SSID "HATH420"
//#define WIFI_PASSWORD "hath@420"

FirebaseData firebaseData;
FirebaseData swStates;

FirebaseJson json;


String readString; //main captured String
String serialMsg = "";
String prevMsg = "q";
int msgSent = 0;
int serialMsgUpdated = 0;
int device1;
int device2;
int intruder;
int pir1;
int pir2;
int mq2;
int door;
int swState[4];
int prevSwState[4];
int change[4];

int lockState; //lock state

int ind1; // , locations
int ind2;
int ind3;
int ind4;
int ind5;
int ind6;
int ind7;
int ind8;
int ind9;
int ind10;

void setup() {
  Serial.begin(9600);


  WiFi.mode(WIFI_STA);
  WiFiManager wm;
  bool res;
  res = wm.autoConnect("AutoConnectAP", "password"); // password protected ap
  if (!res) {
    Serial.println("Failed to connect");
    // ESP.restart();
  }
  else {
    //if you get here you have connected to the WiFi
    Serial.println("connected...yeey :)");
  }

  //  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  //  Serial.print("Connecting to Wi-Fi");
  //  while (WiFi.status() != WL_CONNECTED)
  //  {
  //    Serial.print(".");
  //    delay(300);
  //  }
  //  Serial.println();
  //  Serial.print("Connected with IP: ");
  //  Serial.println(WiFi.localIP());
  //  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
}

void loop() {

  while (Serial.available()) {
    readString = Serial.readString();

    ind1 = readString.indexOf(',');  //finds location of first ,
    device1 = readString.substring(0, ind1).toInt();   //captures first data String
    ind2 = readString.indexOf(',', ind1 + 1 ); //finds location of second ,
    device2 = readString.substring(ind1 + 1, ind2 + 1).toInt();
    ind3 = readString.indexOf(',', ind2 + 1 );
    intruder = readString.substring(ind2 + 1, ind3 + 1).toInt();
    ind4 = readString.indexOf(',', ind3 + 1 );
    pir1 = readString.substring(ind3 + 1, ind4 + 1).toInt();
    ind5 = readString.indexOf(',', ind4 + 1 );
    pir2 = readString.substring(ind4 + 1, ind5 + 1).toInt();
    ind6 = readString.indexOf(',', ind5 + 1);
    mq2 = readString.substring(ind5 + 1, ind6 + 1).toInt();
    ind7 = readString.indexOf(',', ind6 + 1);
    //    swState[0] = readString.substring(ind6 + 1, ind7 + 1).toInt();
    //    ind8 = readString.indexOf(',', ind7 + 1);
    //    swState[1] = readString.substring(ind7 + 1, ind8 + 1).toInt();
    //    ind9 = readString.indexOf(',', ind8 + 1);
    //    swState[2] = readString.substring(ind8 + 1, ind9 + 1).toInt();
    //    ind10 = readString.indexOf(',', ind9 + 1);
    //    swState[3] = readString.substring(ind9 + 1, ind10 + 1).toInt();
    door = readString.substring(ind6 + 1).toInt(); /////ind10 to ind6

    readString = ""; //clears variable for new input

    Firebase.setInt(firebaseData, "device1", device1);
    Firebase.setInt(firebaseData, "device2", device2);
    Firebase.setInt(firebaseData, "intruder", intruder);
    Firebase.setInt(firebaseData, "pir1", pir1);
    Firebase.setInt(firebaseData, "pir2", pir2);
    Firebase.setInt(firebaseData, "mq2", mq2);
    //    Firebase.setInt(firebaseData, "/Devices/Device1", swState[0]);
    //    Firebase.setInt(firebaseData, "/Devices/Device2", swState[1]);
    //    Firebase.setInt(firebaseData, "/Devices/Device3", swState[2]);
    //    Firebase.setInt(firebaseData, "/Devices/Device4", swState[3]);
    Firebase.setInt(firebaseData, "door", door);


    Firebase.getString(swStates, "/Devices/Device1");
    String temp = swStates.stringData();
    swState[0] = temp.toInt();

    Firebase.getString(swStates, "/Devices/Device2");
    temp = swStates.stringData();
    swState[1] = temp.toInt();

    Firebase.getString(swStates, "/Devices/Device3");
    temp = swStates.stringData();
    swState[2] = temp.toInt();

    Firebase.getString(swStates, "/Devices/Device4");
    temp = swStates.stringData();
    swState[3] = temp.toInt();

    for (int i = 0; i < 4; i++) {
      if (swState[i] != prevSwState[i]) {
        change[i] = 1;
      }
      else {
        change[i] = 0;
      }
      prevSwState[i] = swState[i];
      serialMsg += change[i];
    }

    Firebase.getString(swStates, "/lock");
    serialMsg += swStates.stringData();
    Serial.println(serialMsg);
    serialMsg = "";
  }



  //  //expect a string like 90,low,15.6,125*
  //  //or 130,hi,7.2,389*
  //  while (Serial.available())  {
  //    readString = Serial.readString();
  //    //Serial.println("Recieved message from Spresense");
  //    //do stuff
  //    //      Serial.println();
  //    //      Serial.print("captured String is : ");
  //    //      Serial.println(readString); //prints string to serial port out
  //    if ((serialMsg != prevMsg) || 1) { //serial print only if data has changed ///////////////////////////////////debug
  //      Serial.println(serialMsg);
  //      //Serial.println(readString);
  //      prevMsg = serialMsg;
  //      serialMsg = "";
  //    }
  //
  //    ind1 = readString.indexOf(',');  //finds location of first ,
  //    device1 = readString.substring(0, ind1).toInt();   //captures first data String
  //    ind2 = readString.indexOf(',', ind1 + 1 ); //finds location of second ,
  //    device2 = readString.substring(ind1 + 1, ind2 + 1).toInt();
  //    ind3 = readString.indexOf(',', ind2 + 1 );
  //    intruder = readString.substring(ind2 + 1, ind3 + 1).toInt();
  //    ind4 = readString.indexOf(',', ind3 + 1 );
  //    pir1 = readString.substring(ind3 + 1, ind4 + 1).toInt();
  //    ind5 = readString.indexOf(',', ind4 + 1 );
  //    pir2 = readString.substring(ind4 + 1, ind5 + 1).toInt();
  //    ind6 = readString.indexOf(',', ind5 + 1);
  //    mq2 = readString.substring(ind5 + 1, ind6 + 1).toInt();
  //    ind7 = readString.indexOf(',', ind6 + 1);
  //    swState[0] = readString.substring(ind6 + 1, ind7 + 1).toInt();
  //    ind8 = readString.indexOf(',', ind7 + 1);
  //    swState[1] = readString.substring(ind7 + 1, ind8 + 1).toInt();
  //    ind9 = readString.indexOf(',', ind8 + 1);
  //    swState[2] = readString.substring(ind8 + 1, ind9 + 1).toInt();
  //    ind10 = readString.indexOf(',', ind9 + 1);
  //    swState[3] = readString.substring(ind9 + 1, ind10 + 1).toInt();
  //    door = readString.substring(ind10 + 1).toInt();
  //
  //    readString = ""; //clears variable for new input
  //  }


}

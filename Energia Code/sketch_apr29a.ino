#define trigPin 11
#define echoPin 12
#define LED RED_LED

#ifndef __CC3200R1M1RGC__
#include <SPI.h>
#endif
#include <WiFi.h>

int oneCount = 0;
int zeroCount = 0;

// your network name also called SSID
char ssid[] = "ukyedu";
// your network password
//char password[] = "";

// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
//IPAddress server(50,62,217,1);  // numeric IP for Google (no DNS)
char server[] = "10.20.65.220";    // name address for Google (using DNS)

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
WiFiClient client;

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(115200);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(LED, OUTPUT); 

  // attempt to connect to Wifi network:
  Serial.print("Attempting to connect to Network named: ");
  // print the network name (SSID);
  Serial.println(ssid); 
  // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
  WiFi.begin(ssid);
  while ( WiFi.status() != WL_CONNECTED) {
    // print dots while we wait to connect
    Serial.print(".");
    delay(300);
  }
  
  Serial.println("\nYou're connected to the network");
  Serial.println("Waiting for an ip address");
  
  while (WiFi.localIP() == INADDR_NONE) {
    // print dots while we wait for an ip addresss
    Serial.print(".");
    delay(300);
  }

  Serial.println("\nIP Address obtained");
  printWifiStatus();
}

void loop() {
  int duration;
  float distance;
  int conversion;

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(1000);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  
  distance = (duration/2) / 74.07;
  
  if (distance >= 20 || distance <= 1){
    zeroCount++;
    oneCount--;
    if (oneCount < 1) {
      oneCount = 1;
    }
    if (zeroCount == 500) {
      oneCount = 0;
      digitalWrite(LED, LOW);
    }
    if (zeroCount > 1000) {
      zeroCount = 1000;
    }
  }
  
  else{
    oneCount++;
    zeroCount--;
    if (zeroCount < 1) {
      zeroCount = 1;
    }
    if (oneCount == 500) {
      zeroCount = 0;
      digitalWrite(LED, HIGH);
      Serial.println("POST");
      postServer();
    }
    if (oneCount > 1000) {
      oneCount = 1000;
    }
  }
}

void postServer() {
  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  if (client.connect(server, 80)) {
    Serial.println("connected to server");
    // Make a HTTP request:
    client.println("POST /api/alerts HTTP/1.1");
    client.println("Host: 10.20.65.220");
    client.println("Connection: close");
    client.println();
  }

  client.stop();
}


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}







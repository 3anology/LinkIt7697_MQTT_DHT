#include <LWiFi.h>
#include <PubSubClient.h>
//DHT
#include "DHT.h"
#define DHTPIN A0     // what pin we're connected to
#define DHTTYPE DHT11   // DHT 22  (AM2302)

DHT dht(DHTPIN, DHTTYPE);


char ssid[] = "SSID";
char password[] = "Password";
char mqtt_server[] = "MQTT Broker IP address";
char sub_topic[] = "7697 subscribe topic";
char pub_topic[] = "7697 publish topic";
char client_Id[] = "7697client-01-";
char temp_topic[] = "7697-Temp";
char hum_topic[] = "7697-Hum";
 
int status = WL_IDLE_STATUS;

WiFiClient mtclient;      //從WiFiClient 產生 mtclient的物件
PubSubClient client(mtclient);
long lastMsg = 0;
char msg[50];
int value = 0;



void setup() {
    //Initialize serial and wait for port to open:
    Serial.begin(9600);
    //while (!Serial) {
         // wait for serial port to connect. Needed for native USB port only
    //}
    setup_wifi();
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);
//DHT
    Serial.println("DHTxx test!");
    dht.begin();
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
//DHT
    int h = dht.readHumidity();
    int t = dht.readTemperature();
    char temp[75];
    sprintf(temp, "%d", t);
    char hum[75];
    sprintf(hum, "%d", h);
    
  
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;

    Serial.print("Publish message ");
    Serial.println(msg);
   
//DHT
    Serial.print("Publish message: ");
    Serial.println(msg);
    //client.publish("t4", "hello world");
    client.publish(temp_topic,temp);
    client.publish(hum_topic,hum);
    Serial.print(temp_topic);
    Serial.print(":");
    Serial.println(temp);
    Serial.print(hum_topic);
    Serial.print(":");
    Serial.println(hum);
}
}
void setup_wifi() {
   // attempt to connect to Wifi network:
   Serial.print("Attempting to connect to SSID: ");
   Serial.println(ssid);
   WiFi.begin(ssid);
   while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.print(".");
    }
    randomSeed(micros());
    Serial.println("Connected to wifi");
    printWifiStatus();
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Input Message arrived [");
  Serial.print(sub_topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void reconnect() {  //重新連線MQTT
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = client_Id;
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(pub_topic, client_Id);
      // ... and resubscribe
      client.subscribe(sub_topic);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void printWifiStatus() {                     //顯示Wifi狀態
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


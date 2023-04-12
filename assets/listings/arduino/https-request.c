#include <WiFiNINA.h>

char ssid[] = "<SSID>";       // Network SSID
char pass[] = "<PASSWORD>";   // Network Password
int status = WL_IDLE_STATUS;  // Network Status

char server[] = "www.example.com";  // Server Address
WiFiSSLClient client;               // Client Object

void setup() {
  // Setup and wait for serial connection
  Serial.begin(115200);
  while (!Serial)
    ;

  // Connect to WiFI
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    delay(1000);
  }

  // Print WiFi status
  Serial.println("Connected to WiFi");
  printWiFiStatus();

  // Connect to server and send request
  Serial.println();
  Serial.println("Starting connection to server...");
  if (client.connect(server, 443)) {
    Serial.println("Connected to server.");
    client.println("GET / HTTP/1.1");
    client.println("Host: www.example.com");
    client.println("Connection: close");
    client.println();
  }
}

void loop() {
  // Read server response
  while (client.available()) {
    char c = client.read();
    Serial.write(c);
  }

  // Check connection status
  if (!client.connected()) {
    Serial.println();
    Serial.println("Disconnecting from server.");
    client.stop();
    while (true)
      ;  // Halt
  }
}

void printWiFiStatus() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  long rssi = WiFi.RSSI();
  Serial.print("Signal Strenght (RSSI): ");
  Serial.print(rssi);
  Serial.println(" dBm");
}
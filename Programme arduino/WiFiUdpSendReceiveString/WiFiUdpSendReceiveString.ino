#include <SPI.h>
#include <WiFi.h>
#include <WiFiUdp.h>

// Initialisation des variables

int switchState = 0;
bool stateA = 0;
bool stateB = 0;
bool stateC = 0;
bool stateD = 0;
char rep = 'A';
int cpt = 0;
int posLed = 0;

////////////////////////////////////

int status = WL_IDLE_STATUS;
char ssid[] = "UniversityElectronicBox"; //  your network SSID (name)
char pass[] = "UEB-PTUT";    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)

unsigned int localPort = 2390;      // local port to listen on

char packetBuffer[255]; //buffer to hold incoming packet
char  ReplyBuffer[] = "";       // a string to send back

WiFiUDP Udp;

void setup() {

  // On prépare les broches en spécifiant si elles sont en entrée ou en sortie
  pinMode(9, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(3, INPUT);
  pinMode(2, INPUT);
  pinMode(1, INPUT);
  pinMode(0, INPUT);
  
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv != "1.1.0") {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  Serial.println("Connected to wifi");
  printWifiStatus();

  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  Udp.begin(localPort);
  digitalWrite(9, HIGH);
  
}

void loop() {

  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  
  if (packetSize) {
    int len = Udp.read(packetBuffer, 255);
    if (len > 0) {
      packetBuffer[len] = 0;
    }
    String test = packetBuffer;
    Serial.println(test);
    cpt = test.toInt()*4;
    
    while (cpt > 0) {
      // Bouton A
      switchState = digitalRead(3); // On enregistre l'état de la broche
      if (switchState == HIGH) { // Si il y a changement de broche, alors
        if(posLed == 0) {
          rep = 'A';
          digitalWrite(9, HIGH);
          digitalWrite(8, LOW);
          digitalWrite(6, LOW);
          digitalWrite(5, LOW);
          ++posLed;
        }
        else if(posLed == 1) {
          rep = 'B';
          digitalWrite(9, LOW);
          digitalWrite(8, HIGH);
          digitalWrite(6, LOW);
          digitalWrite(5, LOW);
          ++posLed;
        }
        else if(posLed == 2) {
          rep = 'C';
          digitalWrite(9, LOW);
          digitalWrite(8, LOW);
          digitalWrite(6, HIGH);
          digitalWrite(5, LOW);
          ++posLed;
        }
        else if(posLed == 3) {
          rep = 'D';
          digitalWrite(9, LOW);
          digitalWrite(8, LOW);
          digitalWrite(6, LOW);
          digitalWrite(5, HIGH);
          posLed = 0;
        }
      }
      
      

      --cpt;
      delay(240);
    }

    Serial.println(rep);
    Udp.beginPacket("192.168.1.100", 11000);
    Udp.write(rep);
    Udp.endPacket();
    
    /*Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remoteIp = Udp.remoteIP();
    Serial.print(remoteIp);
    Serial.print(", port ");
    Serial.println(Udp.remotePort());

    // read the packet into packetBuffer
    int len = Udp.read(packetBuffer, 255);
    if (len > 0) {
      packetBuffer[len] = 0;
    }
    Serial.println("Contents:");
    Serial.println(packetBuffer);

    // send a reply, to the IP address and port that sent us the packet we received
    Udp.beginPacket(Udp.remoteIP(), 11000);
    Udp.write(ReplyBuffer);
    Udp.endPacket();*/
  }
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





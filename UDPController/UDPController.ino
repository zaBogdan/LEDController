#include <WiFiUdp.h>
#include <ESP8266WiFi.h> 
#include <base64.h>

/**
 * Constant values
 */
//Internet credentials
const char* ssid     = "demoWifiNetwork"; //Router SSID         
const char* password = "demoPassword"; //Router password 

//RGB pins
const int REDPIN = 4;
const int GREENPIN = 16;
const int BLUEPIN = 5;

//Maximum trashold for newtork connection
const int MaximumWaitingTime = 60;

/**
 * Configuration stuff
 */

//Accept change in LED
bool isServerOn = false;

//UDP intial setup
WiFiUDP Udp;
unsigned int localUdpPort = 1337;
char incPkt[256];
char replyPacket[] = "0xTrue"; 

/**
 * Functions
 */
void RGB(int r, int g,int b);
void testRGB();
void makeConnections();
void handleUDPPayload(char incPkt[],int &r,int &g,int &b);

//Setup code
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  delay(100);
  Serial.println();
  Serial.println();
  Serial.println("Initialization started.");
  pinMode(REDPIN, OUTPUT);
  pinMode(GREENPIN, OUTPUT);
  pinMode(BLUEPIN, OUTPUT);
  makeConnections();
  testRGB();
  Serial.println("Server now accepts packets");
  isServerOn = true;
}

//Loop code
void loop() {
  //format: r:xxx:g:xxx:b:xxx:cmd:xxxxxxx
  
  int packetSize = Udp.parsePacket();
  if(packetSize){
    Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int len = Udp.read(incPkt, 255);
    if(len>0){
      incPkt[len]='\0';
    }
    Serial.printf("The packet is: `%s` \n", incPkt);
    int r=0,g=0,b=0;

    //handling the packets
    handleUDPPayload(incPkt, r,g,b);
    
    Serial.printf("R: %d , G: %d B: %d \n\n", r,g,b);
    if(isServerOn)
      RGB(r,g,b);
    //Send a response back to the server
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Serial.printf("Sending a packate to ");Serial.print(Udp.remoteIP());Serial.print(" using port ");Serial.print(Udp.remotePort());Serial.println();
    Udp.write(replyPacket);
    Udp.endPacket();
  }

}

//Write with this functions the values
void RGB(int r, int g,int b){
  //make sure they are only values from 0-1023
  r = r>1023 ? 1023:r, r = r<0 ? 0:r;
  g = g>1023 ? 1023:g, g = g<0 ? 0:g;
  b = b>1023 ? 1023:b, b = b<0 ? 0:b;

  //write them
  analogWrite(REDPIN, r);
  analogWrite(GREENPIN, g);
  analogWrite(BLUEPIN, b);
}

//Connect to the local network
void makeConnections(){
  WiFi.begin(ssid, password);
  int i = 0;
  while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
    delay(1000);
    Serial.print(++i); Serial.print("...");
    if(i>MaximumWaitingTime)
      break;
  }
  if(WiFi.status() != WL_CONNECTED)
    Serial.print("Failed to connect to the internal network");
  else{
    Serial.print("The local ip is ");Serial.print(WiFi.localIP());Serial.println();
    Udp.begin(localUdpPort);
    Serial.print("UDP server started at port ");Serial.print(localUdpPort);Serial.println(); 
  }
}

// The actual logic behind 
void handleUDPPayload(char incPkt[],int &r,int &g,int &b){
  char *p, v[10][10];
  p = strtok(incPkt, ":");
  int i=0;
  while(p || i>9){
    strcpy(v[i++], p);
    p = strtok(NULL, ":");
  }
  for(int i=0;i<8;i+=2){
    if(v[i] && v[i][0]=='r')
      r = atoi(v[i+1])*4;
    if(v[i] && v[i][0]=='g')
      g = atoi(v[i+1])*4;
    if(v[i] && v[i][0]=='b')
      b = atoi(v[i+1])*4;
    if(v[i] && !strcmp(v[i], "cmd")){
      if(v[i+1] && !strcmp(v[i+1], "shutdown")){
        isServerOn = false;
        strcpy(replyPacket, "OxFalse");
        RGB(0,0,0);
      }
      if(v[i+1] && !strcmp(v[i+1], "start")){
        isServerOn = true;
        strcpy(replyPacket, "0xTrue");
      }
    }
  }
}

//Test if the band works
void testRGB(){
  Serial.println("Testing the RGB strip (all white)");
  RGB(1023,1023,1023);
  delay(5000);
  RGB(0,0,0);
  Serial.println("Test finished. If it didn t work please check your connections!");
}

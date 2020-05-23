#include <math.h>
#include <SPI.h>
/////////////////////LoRa Radio/////////////////////////
#include <RH_RF95.h>
#define RFM95_CS 10
#define RFM95_RST 9
#define RFM95_INT 2
#define RF95_FREQ 433.0
// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);
long lastsendtime = 0;
int inter = 2000;
/////////////////////SD Card///////////////////////////
#include <SD.h>
unsigned long lastlogtime = 0;
int pause = 2000;
const int chipSelect = 4;
///////////////////////////////////////////////////////


int sum = 0;
String st1 = "Voltage: ";            //String Variables
String st2 = " Current: ";
String st3 = " Efficiency ";
float current    = 15;
float voltage    = 22;
int time_now = 0;
int period = 1000;
///////////////////////////////////////////SETUP////////////////////////////////////////////////////////////////


void setup() { 
  Serial.begin(9600);

 /*/ Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
  
  }
  Serial.println("card initialized.");
  digitalWrite(chipSelect,HIGH);
/*/

  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);              //Reset
  while (!Serial);
  Serial.begin(9600);

  digitalWrite(RFM95_RST, LOW);               //Reset
  delay(10);                                  //debug
  digitalWrite(RFM95_RST, HIGH);
  delay(10);
  while (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    while (1);
  }
  Serial.println("LoRa radio init OK!");


  
  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);
  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then
  // you can set transmitter powers from 5 to 23 dBm:
  rf95.setTxPower(23, false);

}
/////////////////////////////////////////////////////MAIN LOOP/////////////////////////////////////////////////
void loop() {

  //Serial.println("Counts: " + String(counts));
  // voltage divider calibration (ratio is 2:1 therefore multiply by 3)
  // Send a message to rf95_server
  
  if (sum >= 23){
     current = 14; 
  }
  else{
     current = 15;
  }
  if(millis() >= time_now + period){
    time_now += period;
    sum = random(5)+voltage;

  }

  
  if (millis() >= lastsendtime + inter) {

    lastsendtime += inter;

    String sensorData = String(st1)+String(sum)+String(st2)+String(current);
    int sensorData_len = sensorData.length() +1;
    char radiopacket[sensorData_len];                               // Creating a list/array to send each letter as an ASCII state
    sensorData.toCharArray(radiopacket, sensorData_len);
    Serial.print("Sending "); Serial.println(radiopacket);
 
    Serial.println("Sending...");
    rf95.send((uint8_t *)radiopacket, 29);
    Serial.println(sensorData);
    Serial.println("Waiting for packet to complete..."); 
    rf95.waitPacketSent();
    digitalWrite(RFM95_CS, HIGH);
  }
  
  /*/
  if (millis() >= lastlogtime + pause)
  {
    lastlogtime += pause;
    String dataString = String(sum)+String(",")+String(current)+String(",")+String(recall);
    
    // open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another.
    File dataFile = SD.open("datalog.txt", FILE_WRITE);

    // if the file is available, write to it:
    if (dataFile) {
      dataFile.println(dataString);
      dataFile.close();
      // print to the serial port too:
      Serial.println(dataString);
    }
    // if the file isn't open, pop up an error:
    else {
      Serial.println("error opening datalog.txt");
    }
    
  }
  /*/

}



  

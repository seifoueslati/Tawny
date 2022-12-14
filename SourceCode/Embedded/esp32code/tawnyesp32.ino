#include <BluetoothSerial.h>
#include <Arduino_JSON.h>
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif
BluetoothSerial SerialBT;
float gx, gy, gz, ax, ay, az, mx, my, mz;
#define BT_DISCOVER_TIME  5000
static bool btScanAsync = true;
static bool btScanSync = true;
String car[1];
int contactSensorPin = 19;
int vibration_Sensor =15;
int present_condition = 0;
int previous_condition = 0;
int nbr=0;
void btAdvertisedDeviceFound(BTAdvertisedDevice* pDevice) {
  Serial.printf("Found a device asynchronously: %s\n", pDevice->toString().c_str());
}
void setup() {
  pinMode(contactSensorPin, INPUT_PULLUP);
  pinMode(vibration_Sensor, INPUT);
  Serial.begin(115200);
  SerialBT.begin("TAWNY");
    if (btScanAsync) {
    if (SerialBT.discoverAsync(btAdvertisedDeviceFound)) {
      delay(1000);
      SerialBT.discoverAsyncStop();
    } else {
  
    }
  }
}
void loop() {
     if (nbr==200 and btScanSync) {
    BTScanResults *pResults = SerialBT.discover(BT_DISCOVER_TIME);
    nbr=0;
    if (pResults)
      pResults->dump(&Serial);
    else
     Serial.println("Error on BT Scan, no result!");
  }
  JSONVar doc;
  nbr++;
  long measurement =vibration();
  delay(50);
  present_condition=digitalRead(contactSensorPin);
  doc["vibration"] = measurement;
  doc["backdoor"] = String(present_condition == HIGH);
  doc["speedometerValue"] = 0;
  doc["tachometerValue"] = 0;
  doc["gasValue"] = 0;
  doc["mileage"] = random(170);
  doc["coolant"] = random(30,60);
  doc["engineload"] = random(0,60);
  doc["throttle"] = random(100);
  doc["voltage"] = random(170);
  doc["fuel"] = random(170);
  doc["boost"] = random(-20,26);
  doc["revs"] = random(1000,5000);
  doc["turnSignals"]["left"] = false;
  doc["turnSignals"]["right"] = false;
  doc["iconStates"]["dippedBeam"] = 0;  
  doc["iconStates"]["brake"] = 0;
  doc["iconStates"]["drift"] = 0;  
  doc["iconStates"]["highBeam"] = 0;
  doc["iconStates"]["lock"] = 1;  
  doc["iconStates"]["seatBelt"] = 0;
  doc["iconStates"]["engineTemp"] = 0;  
  doc["iconStates"]["stab"] = 0;
  doc["iconStates"]["abs"] = 0;  
  doc["iconStates"]["gas"] = 0;
  doc["iconStates"]["trunk"] = 0;  
  doc["iconStates"]["bonnet"] = 0;
  doc["iconStates"]["doors"] = 0;  
  doc["iconStates"]["battery"] = 1;
  doc["iconStates"]["oil"] = 0;  
  doc["iconStates"]["engineFail"] = 0;
  String tmp = JSON.stringify(doc);
  Serial.println(tmp); 
  delay(100);
}
long vibration(){
  long measurement=pulseIn (vibration_Sensor, HIGH);
  return measurement;
}

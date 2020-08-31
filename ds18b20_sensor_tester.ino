#include <OneWire.h>
#include <DallasTemperature.h>

//Pin 2
#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

struct error{
  int number;
  unsigned char* address;
  float temperature;
  unsigned long timeFromStart;
};

int currentError = 0;
float realTemp;
int numberOfDevices; // Number of temperature devices found
DeviceAddress tempDeviceAddress; // We'll use this variable to store a found device address

void setup() {
  // Start serial port
  Serial.begin(9600);
  // Start up the library
  sensors.begin();
  numberOfDevices = sensors.getDeviceCount();
  Serial.print("Locating devices...");
  Serial.print(" Found ");
  Serial.print(numberOfDevices);
  Serial.println(" devices.");
  Serial.print("Enter the ambient temperature: ");
  while (Serial.available() == 0)
    realTemp = Serial.parseFloat(); 
  if (Serial.available() > 0)
    Serial.println(realTemp);
}

void loop () {
  numberOfDevices = sensors.getDeviceCount();
  while (Serial.available() > 0){
  for(int i = 0; i < numberOfDevices; i++) {
    // Search the wire for address
    if(!sensors.getAddress(tempDeviceAddress, i)) {
      Serial.print("Found ghost device at ");
      Serial.print(i+1);
      Serial.print(" but could not detect address. Check power and cabling.");
    }
   }
   if (numberOfDevices > 0){
    Serial.println("Readings:");
    Serial.println("Sensor |      Address      | Temperature");
   }
   else
    Serial.print("Sensor cannot be checked.");
  
  sensors.requestTemperatures(); // Send the command to get temperatures
  float tempC;
  float tempe[numberOfDevices];
  unsigned char* addr[numberOfDevices];
  unsigned long time = millis();
  struct error errors[100];
  
  // Loop through each device
  for ( int i = 0; i < numberOfDevices; i++ ) {
    // Search the wire for address
    if(sensors.getAddress(tempDeviceAddress, i)){    
      tempC = sensors.getTempC(tempDeviceAddress);   
      tempe[i] = tempC;
      addr[i] = tempDeviceAddress;
      Serial.print("  ");
      Serial.print(i+1);
      Serial.print("      ");
      printAddress(addr[i]);
      Serial.print("      ");
      Serial.println(tempe[i]);
    }
  }
  Serial.println("______________________________________");
  Serial.println("Errors:");
  Serial.print("Sensor |      Address      | Temperature | Time[min]");
  Serial.println();
  for (int j = 0; j < numberOfDevices; j++){
    if(sensors.getAddress(tempDeviceAddress, j) && abs(realTemp - tempe[j]) > 2){
      currentError++;
      errors[currentError].number = j;
      errors[currentError].address = addr[j];
      errors[currentError].temperature= tempe[j];
      errors[currentError].timeFromStart = (time/60000);
    }
    for (int l = 0; l < currentError; l++){ 
      if(j == errors[l+1].number){
        Serial.print("  ");
        Serial.print(j+1);
        Serial.print("      ");
        printAddress(errors[l+1].address);
        Serial.print("      ");
        Serial.print(errors[l+1].temperature);
        Serial.print("          ");
        Serial.println(errors[l+1].timeFromStart);
      }
    }
  }
  Serial.println();  
  delay(5000);
  }
}

// function to print a device address
void printAddress(DeviceAddress deviceAddress) {
  for (uint8_t i = 0; i < 8; i++) {
    if (deviceAddress[i] < 16) Serial.print("0");
      Serial.print(deviceAddress[i], HEX);
  }
}

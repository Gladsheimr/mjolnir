
#include <PDM.h>

//#include <arduinoFFT.h>

#include <ArduinoBLE.h>

//arduinoFFT FFT;


#define SAMPLE_RATE          16000
#define BUFFER_SIZE          256

#define R_LED_PIN            22
#define G_LED_PIN            23
#define B_LED_PIN            24

// BLE Service
BLEService imuService("67aa4562-ba79-46a3-a86b-91675642a359"); // Custom UUID

// BLE Characteristic
//BLECharacteristic imuCharacteristic("917649A1-D98E-11E5-9EEC-0002A5D5C51B", BLERead | BLENotify, 12);
//BLECharacteristic resCharacteristic("19B1", BLERead | BLENotify, value, 256);

// Global variables ---------------------------------------------------

short sampleBuffer[BUFFER_SIZE];
volatile int samplesRead;



BLEByteCharacteristic sampleCharacteristic("917649A1-D98E-11E5-9EEC-0002A5D5C51B", BLERead | BLENotify);
BLEShortCharacteristic samplesReadCharacteristic("917649A1-D98E-11E5-9EEC-0002A5D5C512", BLERead | BLENotify);
BLEShortCharacteristic resCharacteristic("917649A1-D98E-11E5-9EEC-0002A5D5C511", BLERead | BLENotify);

// Function definitions -----------------------------------------------

void onPDMdata(void);

// Main ---------------------------------------------------------------

void setup()
{
  Serial.begin(115200);
  while (!Serial);
   Serial.println("AUDIO BLE DATA COLLECTION EXAMPLE");
  pinMode(R_LED_PIN, OUTPUT);
  pinMode(G_LED_PIN, OUTPUT);
  pinMode(B_LED_PIN, OUTPUT);
  

  PDM.onReceive(onPDMdata);
  

  if (!PDM.begin(1, SAMPLE_RATE))
  {
    Serial.println("Failed to start PDM!");
    while (1);
  }

  PDM.setGain(30);

  analogWrite(R_LED_PIN, UINT8_MAX);
  analogWrite(G_LED_PIN, UINT8_MAX);
  analogWrite(B_LED_PIN, UINT8_MAX);

  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");
    while (1);
  }
  

  // Setup bluetooth
  BLE.setLocalName("Sounder");
  BLE.setAdvertisedService(imuService);
  //imuService.addCharacteristic(imuCharacteristic);
  imuService.addCharacteristic(resCharacteristic);
  imuService.addCharacteristic(sampleCharacteristic);
  BLE.addService(imuService);

  // start advertising
  BLE.advertise();
}

void loop()
{
  if (samplesRead)
  {
    BLEDevice central = BLE.central();
     if (central) {
       while (central.connected()) {
         Serial.println("CONNECTED");
     
         BLE.poll();

         sampleCharacteristic.setValue(-1);
//
          for(int sampleIdx = 0; sampleIdx < BUFFER_SIZE; sampleIdx++) {
              Serial.println(sampleBuffer[sampleIdx]); // Print to serial (Collection example)
            resCharacteristic.writeValue(sampleBuffer[sampleIdx]);
          
          }

         //samplesReadCharacteristic.writeValue(samplesRead);

         
         sampleCharacteristic.writeValue(1);

     
       }
       Serial.println("DISCONNECTED");
     }
       
    samplesRead = 0;
  }
}

void onPDMdata()
{
  int bytesAvailable = PDM.available();
  PDM.read(sampleBuffer, bytesAvailable);
  samplesRead = bytesAvailable / sizeof(short);


}


#ifndef _MJOLNIR_H_
#define _MJOLNIR_H_

#include <ArduinoBLE.h>

namespace mjolnir
{

BLEService mjolnirService("67aa4562-ba79-46a3-a86b-91675642a359");

BLEShortCharacteristic heartbeatCharacteristic("917649A1-D98E-11E5-9EEC-0002A5D5C51B", BLERead | BLENotify);



class Mjolnir {
    private:
      short pong = 1;
      BLEDevice central;
      bool connected;

      void blePeripheralConnectHandler(BLEDevice central) {
        // central connected event handler
        this->connected = true;
        Serial.print("Connected event, central: ");
        Serial.println(central.address());
      }

      void blePeripheralDisconnectHandler(BLEDevice central) {
        // central disconnected event handler
        this->connected = false;
        Serial.print("Disconnected event, central: ");
        Serial.println(central.address());
      }

    public:
      tflite::ErrorReporter *error_reporter;

      Mjolnir(tflite::ErrorReporter *error_reporter)
      {
        this->error_reporter = error_reporter;
      }

      void setup(const char* device_name) {
         if (!BLE.begin()) {
          this->error_reporter->Report("starting BLE failed!");
          while (1);
        }

        //BLE.setEventHandler(BLEConnected, this->blePeripheralConnectHandler);
        //BLE.setEventHandler(BLEDisconnected, this->blePeripheralDisconnectHandler);


        this->error_reporter->Report("Starting Mjolnir ...");
        BLE.setLocalName("Mjolnir");
        BLE.setDeviceName(device_name);
        BLE.setAdvertisedService(mjolnirService);

        mjolnirService.addCharacteristic(heartbeatCharacteristic);
        BLE.addService(mjolnirService);

        BLE.advertise();
        this->error_reporter->Report("Started Mjolnir ...");
      }

      void connect() {
         if (!this->central) {
           this->central = BLE.central();
         } 
      }
      

      void heartbeat() {

        
        if (this->central && this->central.connected()) {
          BLE.poll();
          this->error_reporter->Report("Connected to:");
          this->error_reporter->Report(this->central.address());
          if (this->pong > 0) {
            this->pong = 0;
          }
          else {
            this->pong = 1;
          }
          heartbeatCharacteristic.writeValue(this->pong);
        }
      }
    };

} // namespace mjolnir

#endif


#ifndef _MJOLNIR_H_
#define _MJOLNIR_H_
#include <map>
#include <ArduinoBLE.h>

namespace mjolnir
{

BLEService mjolnirService("67aa4562-ba79-46a3-a86b-91675642a359");

BLEShortCharacteristic heartbeatCharacteristic("917649A1-D98E-11E5-9EEC-0002A5D5C51B", BLERead | BLENotify);

bool mjl_connected;

class Mjolnir {
    private:
      short pong = 1;
      BLEDevice central;
      bool connected;

      std::map<const char* ,BLECharacteristic> characteristics;
      std::map<const char* ,BLECharacteristic>::iterator characteristics_it;
      static void blePeripheralConnectHandler(BLEDevice central) {
        // central connected event handler
        mjl_connected = true;
        Serial.print("Connected event, central: ");
        Serial.println(central.address());
      }

      static void blePeripheralDisconnectHandler(BLEDevice central) {
        // central disconnected event handler
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

        BLE.setEventHandler(BLEConnected, this->blePeripheralConnectHandler);
        BLE.setEventHandler(BLEDisconnected, this->blePeripheralDisconnectHandler);


        this->error_reporter->Report("Starting Mjolnir ...");
        BLE.setLocalName("Mjolnir");
        BLE.setDeviceName(device_name);
        BLE.setAdvertisedService(mjolnirService);

        mjolnirService.addCharacteristic(heartbeatCharacteristic);

        
        this->error_reporter->Report("Started Mjolnir ...");
      }

      void add_characteristic(const char* id) {
        BLEByteCharacteristic characteristic(id, BLERead | BLENotify);

        mjolnirService.addCharacteristic(characteristic);

        this->error_reporter->Report("CC %d", mjolnirService.characteristicCount());


        this->error_reporter->Report("Added characteristic");

        this->error_reporter->Report(id);

        this->characteristics.insert(std::make_pair(id, characteristic));

        this->error_reporter->Report("CC %d", mjolnirService.characteristicCount());

      }

      void write_to_characteristic(const char*  id, const char* value) {
        this->characteristics_it = this->characteristics.find(id);

        if (this->characteristics_it != this->characteristics.end()) {
          BLECharacteristic characteristic = this->characteristics_it->second;

          //this->error_reporter->Report("Found characteristic");

          characteristic.writeValue(value);
          //this->characteristics_it->erase();
        }
        
      }


      void start() {

        BLE.addService(mjolnirService);
        BLE.advertise();
      }

      void connect() {
         if (!this->central) {
           this->central = BLE.central();
         } else if (this->central && this->central.connected()) {
           this->heartbeat();
         }
      }
      

      void heartbeat() {
          const short poll_freq = 10;
          if (this->pong == poll_freq) {
            // Poll only once in a while 
            BLE.poll();
          }
          if (this->pong > poll_freq) {
            this->pong = 0;
          }
          else {
            this->pong++;
          }
          heartbeatCharacteristic.writeValue(this->pong);
        
      }
    };

} // namespace mjolnir

#endif

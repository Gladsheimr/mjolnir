
import logging
import time
import uuid

import Adafruit_BluefruitLE

from scipy.io.wavfile import write
import numpy as np


# BASED ON EXAMPLE FROM Adafruit_BluefruitLE https://github.com/adafruit/Adafruit_Python_BluefruitLE/blob/master/examples/low_level.py

SERVICE_ID = uuid.UUID('67aa4562-ba79-46a3-a86b-91675642a359')
PDM_CHAR_ID = uuid.UUID('917649A1-D98E-11E5-9EEC-0002A5D5C511')
SHORT_SAMPLE_CAPTURED_ID = uuid.UUID('917649A1-D98E-11E5-9EEC-0002A5D5C51B')


ble = Adafruit_BluefruitLE.get_provider()

PDM_COLLECTED = bytearray()

samples = 0


def writeWav(data, ssa):
    print(data)
    write("samples/example_{}.wav".format(ssa), 16000, data)




def main():
    ble.clear_cached_data()

    adapter = ble.get_default_adapter()
    adapter.power_on()
    print('Using adapter: {}'.format(adapter.name))

    ble.disconnect_devices([SERVICE_ID])

    print('Searching for UART device...')
    try:
        adapter.start_scan()
        device = ble.find_device(service_uuids=[SERVICE_ID])
        if device is None:
            raise RuntimeError('Failed to find UART device!')
    finally:
        adapter.stop_scan()

    print('Connecting to device...')
    device.connect()  
    try:
        print('Discovering services...')
        device.discover([SERVICE_ID], [SHORT_SAMPLE_CAPTURED_ID, PDM_CHAR_ID])

        uart = device.find_service(SERVICE_ID)
        rx = uart.find_characteristic(PDM_CHAR_ID)
        tx = uart.find_characteristic(SHORT_SAMPLE_CAPTURED_ID)



        def received(data):
            global PDM_COLLECTED
            
            dataBA = bytearray(data)

            PDM_COLLECTED.append(dataBA[0])
            PDM_COLLECTED.append(dataBA[1])
            print("APPENDING TO ARRAY {} to {}".format(data, len(PDM_COLLECTED)))
         

        def receivedTx(data):
            global PDM_COLLECTED
            global samples
            
            newSample = int.from_bytes(data, byteorder='big') == 1
            if newSample:
                
                if len(PDM_COLLECTED) > 0:

                    writeWav(np.frombuffer(PDM_COLLECTED), samples)

                    samples += 1

        tx.start_notify(receivedTx)

        print('Subscribing to RX characteristic changes...')
        rx.start_notify(received)

        print('Waiting 60 seconds to receive data from the device...')
        time.sleep(60)
    finally:
        device.disconnect()


ble.initialize()

ble.run_mainloop_with(main)

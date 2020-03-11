from __future__ import print_function
import asyncio
import platform
import sys

from reprint import output

from bleak import BleakClient

import itertools

import binascii


result_chr = "6D7A5BC6-D196-4A6A-95F0-B1115FD36BB4"

def process_hb(s, d):
    hb = int.from_bytes(d, "little")

def set_result(s, d):
    result = d

def print_flush(t):
    sys.stdout.write(t)

    sys.stdout.write("\n")
    sys.stdout.flush()


async def print_services(mac_addr: str, loop: asyncio.AbstractEventLoop):
    async with BleakClient(mac_addr, loop=loop) as client:
        svcs = await client.get_services()

        

        with output(initial_len=2, interval=0) as output_lines:
            hb = itertools.cycle(["💖", "💓", "💞", "💕", "💝"])
            if output_lines[0] is None and output_lines[0]:
                output_lines[0] = "device_heartbeat:    💔"
                output_lines[1] = "svc:model_res:   "
            def set_output(value, idx):
                output_lines[idx] = value
            await client.start_notify("917649A1-D98E-11E5-9EEC-0002A5D5C51B", lambda x, y: set_output("device_heartbeat:    {}".format(next(hb)), 0) )
            await client.start_notify("6D7A5BC6-D196-4A6A-95F0-B1115FD36BB4", lambda x, y: set_output("svc:model_res:       {}".format(y), 1) )

        #await asyncio.gather(get_heartbeat(client), get_model_result(client))
        while True:
            await asyncio.sleep(1)

#./mjolnir --config micro_speech.anv --output stdout --show-heartbeat true --services svc:model_result

mac_addr = "756538B3-F262-494F-9C2C-A5872CA61D12"



if __name__ == '__main__':
        
    loop = asyncio.get_event_loop()
    
    try:
        asyncio.ensure_future(print_services(mac_addr, loop))
        loop.run_forever()
    except:
        pass
    finally:
        loop.close()
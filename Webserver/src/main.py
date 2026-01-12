import asyncio
import mqtt_server
import camera_server


async def run():
    mqtt_server.start_server()
    while True:
        pass

if __name__ == "__main__":
    asyncio.run(run())

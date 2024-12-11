import asyncio
from bleak import BleakClient

# Replace with your device's address
ADDRESS = "74D5AAB8-8114-9478-0242-8EA0B1281B2F"

# This will store the notification handler
def notification_handler(sender, data):
    # data is a bytes object, you might need to decode it depending on your device
    print(f"Received: {data}")

async def main():
    async with BleakClient(ADDRESS) as client:
        print(f"Connected: {client.is_connected}")
        
        # Get services
        services = await client.get_services()
        
        # Find the characteristic you want to read from
        # You'll need to know the UUID of the characteristic you want to monitor
        # Example UUID - replace with your actual characteristic UUID:
        # characteristic_uuid = "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"
        
        # Start notifications
        # await client.start_notify(characteristic_uuid, notification_handler)
        
        # Keep the connection alive
        await asyncio.sleep(float('inf'))

try:
    asyncio.run(main())
except KeyboardInterrupt:
    print("Stopping...")
# 74D5AAB8-8114-9478-0242-8EA0B1281B2F
# ESPee
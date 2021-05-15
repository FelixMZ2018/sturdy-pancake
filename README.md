# sturdy-pancake

# Hardware 
- ESP2866 / Upgraded to ESP32 due to additional GPIO Pins
- MCP3008 Multiplexer
- Capacitive Soil Moisture Sensor

## Update

Most capactivie soil moisture sensors now seem to rely on the NE555 Chip, this causes an issue as this means they work on 5V rather than 3.3V
to remedy this the next iteration of the design is using 2 VCC lines, one VIN (5V) and one 3.3V for logic purposes.
Switching them will require a voltage divider between the AO of each sensor and the MCP3008 input
A custom PCB will have connectors for ESP based dev boards as well as Raspberry Pi Zero boards. 

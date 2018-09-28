## brazil-wireless-counter
Software for the Citizen Science project - Data collection in Sete Barras, Brazil

### Setting the Raspberry Pi as a Wi-Fi Access Point

Follow: http://www.raspberryconnect.com/network/item/333-raspberry-pi-hotspot-access-point-dhcpcd-method

The tests have been done configuring the 192.168.4.0 network, assigning 192.168.4.1 to the Pi and the range de 192.168.4.2 - .20 to the connecting devices.

### Running a web server to control de counter

Python as a web server, using a HTML template and jQuery

Go into python_server and run 'python server.py'
Open web browser and head to 'http://localhost:5002'

### Using Lora to transmit the counter

Based in the arduino code 

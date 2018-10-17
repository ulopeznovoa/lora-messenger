/*  
 *  LoRa 868 / 915MHz SX1272 LoRa module
 *  
 *  Copyright (C) Libelium Comunicaciones Distribuidas S.L. 
 *  http://www.libelium.com 
 *  
 *  This program is free software: you can redistribute it and/or modify 
 *  it under the terms of the GNU General Public License as published by 
 *  the Free Software Foundation, either version 3 of the License, or 
 *  (at your option) any later version. 
 *  
 *  This program is distributed in the hope that it will be useful, 
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License 
 *  along with this program.  If not, see http://www.gnu.org/licenses/. 
 *  
 *  Version:           1.2
 *  Design:            David Gascón 
 *  Implementation:    Covadonga Albiñana, Victor Boria, Ruben Martin
 */
 
// Include the SX1272 and SPI library: 
#include "arduPiLoRa.h"

#include <iostream>
#include <fstream>
using namespace std;

int e;

char sendFlagFile [] = "../../../../sendflag.dat"; //Path to file with counter value
char sendBuffer [] = "../../../../send.dat"; //Path to file with counter value
char receiveBuffer [] = "../../../../receive.dat"; //Path to file with counter value

void setup()
{
  // Print a start message
  printf("SX1272 module and Raspberry Pi: send & receive packets with ACK\n");
  
  // Power ON the module
  e = sx1272.ON();
  printf("Setting power ON: state %d\n", e);
  
  // Set transmission mode
  e |= sx1272.setMode(4);
  printf("Setting Mode: state %d\n", e);
  
  // Set header
  e |= sx1272.setHeaderON();
  printf("Setting Header ON: state %d\n", e);
  
  // Select frequency channel
  e |= sx1272.setChannel(CH_10_868);
  printf("Setting Channel: state %d\n", e);
  
  // Set CRC
  e |= sx1272.setCRC_ON();
  printf("Setting CRC ON: state %d\n", e);
  
  // Select output power (Max, High or Low)
  e |= sx1272.setPower('H');
  printf("Setting Power: state %d\n", e);
  
  // Set the node address
  e |= sx1272.setNodeAddress(3);
  
  // Print a success message
  if (e == 0)
    printf("SX1272 successfully configured\n");
  else
    printf("SX1272 initialization failed\n");

  delay(1000);
}

void readFile(void)
{
    ifstream file;
    file.open(counterFilePath);
    
   if (file.is_open()) 
   {
        file >> value;
	printf("\nRead value from file: %s",value);
   }
   else
   {
	printf("\nError opening file to read");
   }

   file.close();
}

void writeFile(void)
{
   ofstream file;
   file.open(counterFilePath);

   if (file.good())
   {
	file << value;
   }
   else
   {
	printf("\nError opening file to write");
   }

   file.close(); 
}

void transmit()
{
    e = sx1272.sendPacketTimeoutACK(0, value);
    printf("\nPacket sent, state %d\n",e);
 
    delay(4000);
}

void receive()// Receive message
{ 
  e = sx1272.receivePacketTimeoutACK(10000);
  if ( e == 0 )
  {
    printf("Receive packet with ACK, state %d\n",e);

    for (unsigned int i = 0; i < sx1272.packet_received.length; i++)
    {
      value[i] = (char)sx1272.packet_received.data[i];
    }
    printf("Message: %s\n", value);

  }
  else {
    printf("Receive packet with ACK, state %d\n",e);
  }
}

//This funcion returns: 1 for transmission, 0 for reception, -1 if error
int readSendFlag()
{
    ifstream file;
    file.open(sendFlagFile);
   
   char value;

   if (file.is_open()) 
   {
        file >> value;
	printf("\nRead value from sendFlag file: %s",value);
   }
   else
   {
	printf("\nError opening file to read");
   }

   file.close();

   return int(value);
}

void loop()
{
	char buffer[140];

	while(true)
	{
		int send = readSendFlag();

		if(send == 1)//Transmit buffer
		{
			//read sendBuffer
			//transmit buffer
			//clear sendBuffer
			//sendFlag = 0

		}
		else(send == 0) //Receive buffer
		{
			//receive Lora
			//if (message)
			//	write ReceiveBuffer
		}
		else // Error
		{
			return -1;
		}
	}
}

int main(int argc, char *argv[]){
	
	setup(); //Configure Lora module

	loop();

	return 0;
}

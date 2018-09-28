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

int tx_rx_error;

int action; //Stores whether the program will transmit (1) or receive (2)
char counterFilePath [] = "../../../../counterFile.dat"; //Path to file with counter value
char value[16]; //To hold counter value between file read/wile and transmit/receive

void setup()
{
  // Print a start message
  printf("SX1272 module and Raspberry Pi: send packets with ACK\n");
  
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
  if(action == 1)
  	e |= sx1272.setNodeAddress(3);
  else if(action == 2)
  	e |= sx1272.setNodeAddress(3);
  printf("Setting Node address: state %d\n", e);
  
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

//Return: 0 if error, 1 to transmit, 2 to receive
int parseargs(int argc, char *argv[])
{
	if(argc != 2)
	{
		printf("\nError: Argcount not valid - Use 't' to transmit, 'r' to receive \n");
		return 0;
	}
	else
	{
		if(argv[1][0] == 't')
			return 1;
		else if (argv[1][0] == 'r')
			return 2;
		else
		{
			printf("\nError: invalid argument - Use 't' to transmit, 'r' to receive \n");
			return 0;
		}
	}
}

int main(int argc, char *argv[]){
	
	action = parseargs(argc, argv);

	if(action == 0) //Exit if error
		return 0;

	setup(); //Configure Lora module

	if(action == 1) // Transmit
	{
		readFile();
		transmit();
	}
	else if(action == 2) //Receive
	{
		receive();
		//Add error handling if value is not received
		writeFile();
	}

	return tx_rx_error;
}

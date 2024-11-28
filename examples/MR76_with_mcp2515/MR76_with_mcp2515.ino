// CAN Receive Example
//

#include <mcp_can.h>
#include <mr76_radar.h>


long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];
char msgString[128];                        // Array to store serial string

#define CAN0_INT 32                            
MCP_CAN CAN0(5);                               
MR76 radar;

void setup()
{
  Serial.begin(115200);
  
  // Initialize MCP2515 running at 16MHz with a baudrate of 500kb/s and the masks and filters disabled.
  if(CAN0.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) == CAN_OK)
    Serial.println("MCP2515 Initialized Successfully!");
  else
    Serial.println("Error Initializing MCP2515...");
  
  CAN0.setMode(MCP_NORMAL);                     // Set operation mode to normal so the MCP2515 sends acks to received data.

  pinMode(CAN0_INT, INPUT);                            // Configuring pin for /INT input
  radar.skip_cycle = 1;                         // Skipping 1 frame
  Serial.println("MCP2515 Library Receive Example...");
}

void loop()
{
  if(!digitalRead(CAN0_INT))                         // If CAN0_INT pin is low, read receive buffer
  {
    
    CAN0.readMsgBuf(&rxId, &len, rxBuf);      
    radar.parse_data(rxId, len, rxBuf);
    // Serial.print(radar._total_objects);
    // Serial.print(radar.cycles);
    // Serial.println(rxId,HEX);
    if(radar.isready()){
      sprintf(msgString, "Found %d objects, %d", radar.object_detected, radar.is_object_complete);
      Serial.println(msgString);
      for (int iter =0; iter < radar.object_detected; iter++){
        sprintf(msgString, "id ; %d, distance long : %f, distance lat : %f", radar.object[iter].id, radar.object[iter].distance_long, radar.object[iter].distance_lat);
        Serial.println(msgString);
      }
    }

  }
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/

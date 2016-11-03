#include "esp3.h"
#include "fifo.h"
#include <stdint.h>
#include <stdio.h>
static fifoHandler fhandler;


//Loopback for send/receive
 bool hal_UartGetByte(uint8_t* byte)
{
  if(fifo_isEmpty(fhandler))
    return false;
  else
    fifo_getByte(fhandler,byte);

  return true;
}

 bool hal_UartSendByte(uint8_t byte)
{
  if(fifo_isFull(fhandler))
  {
    return false;
  }

  fifo_addByte(fhandler,byte);
  return true;
}

int main()
{
	int n;
    uint8_t output[255];
	static uint8_t u8Buffer[255];
    fhandler= fifo_create(u8Buffer, 255);
    //const uint8_t u8PacketPart1[25] = {0x00,0x00,0x00,0x55, 0x00, 0x07, 0x00, 0x05, 0x0D, 0x01, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0xF0};
    const uint8_t u8PacketPart1[25] = {0x55,0x00,0x01,0x00,0x05,0x70,0x03,0x09};
    //const uint8_t u8PacketPart1[25] = {0x55,0x00,0x01,0x55,0x55,0x55,0x00,0x01,0x00,0x05,0x70,0x03,0x09};
    //const uint8_t u8PacketPart1[25] = {0x55,0x00,0x01,0x00,0x05,0x71,0x03,0x09};  // wrong CRCH
    //const uint8_t u8PacketPart1[25] = {0x55,0x00,0x01,0x00,0x05,0x70,0x03,0x08};  // wrong CRCD
    fifo_addBuffer(fhandler,u8PacketPart1,25);
    
    uint8_t u8BufferReceived[130];
    ESP3_TELEGRAM_t telReceived;
    telReceived.u8DataBuffer=u8BufferReceived;
    telReceived.u16BufferLength=130;

    uint8_t u8BufferResponse[34];
    ESP3_TELEGRAM_t pResponse;
    pResponse.u8DataBuffer = u8BufferResponse;
    pResponse.u16BufferLength = 34;
    pResponse.type = PACKET_RESPONSE;
    pResponse.u8DataBuffer[0] = RET_NOT_SUPPORTED;
    pResponse.u16DataLength = 0x01;
    pResponse.u8OptionLength = 0;
  
	ESP3_RETURN_VALUES_t esp3Status = esp3_ReceivePacket(&telReceived);
	printf("return value = %d\n",esp3Status);

	if(esp3Status==ESP3_OK)
	{
		switch(telReceived.type)
		{
			case COMMON_COMMAND:
				switch(telReceived.u8DataBuffer[0])
				{
					case CO_RD_VERSION:
					{
						pResponse.u8DataBuffer[0] = RET_OK;
                       	//… //
                       	pResponse.u16DataLength = 1 + 16 + 16;
                        pResponse.u8OptionLength = 0;
						break;
                    }
					default: break;
				}
            	break;    
		}
		esp3_SendPacket(&pResponse);
			
	}
//	esp3_ReceivePacket(&telReceived);
	n = fifo_getNrBytes(fhandler);
	printf("fifo after getting a packet with number of element = %d\n",n);
	fifo_getBuffer(fhandler,output,n);
	for(int i = 0; i < n; i++)
	{
		printf("0x%02x \n", output[i]);
	}	

}

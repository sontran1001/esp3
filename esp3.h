#ifndef ESP3_H
#define ESP3_H

#include <stdint.h>
#include <stdbool.h>

/**
 * ESP3 packet structure through the serial port.
 *
 * Sync  is 0x55
 * CRC8H is CRC8 for ‘Header’
 * CRC8D is CRC8 for ‘Datas’ (Data and Optional Data)
 *
 *                   bytes
 *                     ▼
 * +---------------+ -----
 * |   Sync Byte   |   1
 * +---------------+ - + - ----------------------+
 * |    Header     |   2    Data length          |
 * |               |   +                         |
 * |               |   1    Optional Data length |
 * |               |   +                         |
 * |               |   1    Packet type          |
 * +---------------+ - + - ----------------------+
 * |  CRC8 Header  |   1
 * +---------------+ - + -
 * |     Data      |
 * |               |   1
 * |               |   +
 * |               |   X
 * |               |
 * +---------------+ - + -
 * | Optional Data |
 * |               |   0
 * |               |   +
 * |               |   Y
 * |               |
 * +---------------+ - + -
 * |   CRC8 Data   |   1
 * +---------------+ -----
 *
 */
typedef enum
{
	ESP3_OK, //!< The execution of the function was a success ( A full packet was received/ packet was send)
	ESP3_ONGOING_TELEGRAM, //!< A partial telegram has been received, but no new rx byte are currently available
	ESP3_BUFFER_TO_SHORT, //!< received telegram did not fit into the buffer
	ESP3_INVALID_PARAMETER, //!Parameter of the function was invalid
	ESP3_INVALID_PACKET, //! Received packet was invalid
	ESP3_INVALID_CRC, //! Checksum calculation failed
	ESP3_NO_RX_BYTE		//!< No partial telegram is currently in the buffer, and now new rx byte were received.
} ESP3_RETURN_VALUES_t;

typedef enum
{	
	//reserved
	RADIO_ERP1 	= 0x01,
	PACKET_RESPONSE = 0x02,
	RADIO_SUB_TEL	= 0x03,
	EVENT			= 0x04,
	COMMON_COMMAND	= 0x05,
	SMART_ACK_COMMAND = 0x06,
	REMOTE_MAN_COMMAND	= 0x07,
	//reserved
	RADIO_MESSAGE = 0x09,
	RADIO_ERP2 = 0x0A,
	//reserved
	RADIO_802_15_4 = 0x10,
	COMMAND_2_4	= 0x11,
	//reserved
	
}PACKET_t;

typedef enum
{	
	RET_OK,	//OK ... command is understood and triggered
	RET_ERROR,
	RET_NOT_SUPPORTED,
	RET_WRONG_PARAM,
	RET_OPERATION_DENIED,
	RET_LOCK_SET,
	RET_BUFFER_TO_SMALL,
	RET_NO_FREE_BUFFER
	//reserved
	
}RESPONSE_t;

typedef enum
{
	CO_WR_SLEEP		= 1,
	CO_WR_RESET		= 2,	
	CO_RD_VERSION   = 3
}COMMON_COMMAND_CODES_t;

typedef struct
{
	uint16_t u16DataLength;	//! Amount of raw	bytes to be received. The most significant byte is sent/received first
	uint8_t u8OptionLength; //! Amount of optional bytes to be received
	uint8_t type; 					//! Received packet type
	uint8_t *u8DataBuffer; 	//! Internal data buffer, representing the packet
	uint16_t u16BufferLength; //! Length of the internal data buffer
} ESP3_TELEGRAM_t;

bool hal_UartGetByte(uint8_t* byte); //Read 1 byte from uart

bool hal_UartSendByte(uint8_t byte); //Send 1 byte via uart, return false if a problem occured

/**
* Receive an ESP3 packet, parse and test to get actual data
* @param a pointer of an ESP3 telegram
* @return ESP3_RETURN_VALUES_t - status of the receiving process
**/
ESP3_RETURN_VALUES_t esp3_ReceivePacket(ESP3_TELEGRAM_t *telegram);
/**
* Send an ESP3 packet with parket the actual data, header and CRC
* @param a pointer of an ESP3 telegram
* @return ESP3_RETURN_VALUES_t - status of the sending process
**/
ESP3_RETURN_VALUES_t esp3_SendPacket(const ESP3_TELEGRAM_t* telegramToSend);

#endif

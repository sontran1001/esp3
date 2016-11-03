/** FIFO implementation for c **/
#ifndef FIFO_H
#define FIFO_H
#include <stdint.h>
#include <stdbool.h>
#define MAX_NUMBER_OF_FIFO_HANDLERS 1
/**
*Abstract handler for the fifa
**/
typedef struct fifoHandler_s * fifoHandler;
/**
* Create an instance of the fifo system
* @param buffer buffer to use (!=NULL)
* @param buffsize size of the buffer
* @return NULL if no resources available or handler
**/
fifoHandler fifo_create(uint8_t *pu8Buffer, const uint8_t ku8Buffsize);
/**
* releases the handler
* @param handler handle to free, can not be used afterwards!
**/
void fifo_release(fifoHandler handler);
/**
* Get one byte from the fifo
* @param handler instance of the fifo!=null
* @param byte read byte
* @return true if byte exist or false
**/
bool fifo_getByte(fifoHandler handler,uint8_t * const ku8Byte);
/**
* Get one byte from the fifo
* @param handler instance of the fifo!=null
* @param byte the  byte to add
* @return false if fifo error occured or true (check the flags)
**/
bool fifo_addByte(fifoHandler handler,uint8_t const ku8Byte);
/**
* Read max x Bytes from the fifo
* @param handler instance of the fifo!=null
* @param dest The destination buffer
* @param max number of bytes to read from the fifo
* @return Number of bytes read from fifo
**/
uint8_t fifo_getBuffer(fifoHandler handler,uint8_t * pu8Dest, uint8_t u8NumberOfBytesToRead);
/**
* Add  x Bytes from the fifo
* @param handler instance of the fifo!=null
* @param src The Source buffer
* @param max number of bytes to add to the fifo
* @return Number of bytes add to the fifo
**/
uint8_t fifo_addBuffer(fifoHandler handler,const uint8_t *src, uint8_t u8NumberOfBytesToRead);
/***
* Count the number of bytes available to read in the fifo
* @param handler the fifo handler
* @return number of bytes in the fifo available to read
**/
uint8_t fifo_getNrBytes(fifoHandler handler);
/**
* Check if the fifo is empty
* @param handler the fifo handler
* @return true or false
*/
bool  fifo_isEmpty(fifoHandler handler);
/**
* Check if the fifo is full
* @param handler the fifo handler
* @return true or false
*/
bool  fifo_isFull(fifoHandler handler);
/**
* Check if the fifo has Overflown 
* @param handler the fifo handler
* @return true or false
*/
bool  fifo_isOvf(fifoHandler handler);
/**
* Check if the fifo has raised and underflow error
* @param handler the fifo handler
* @return true or false
*/
bool  fifo_isUdf(fifoHandler handler);
/**
* The maximal bytes available for the fifo
* @param handler the fifo handler
* @return size of the fifo
*/
uint8_t fifo_getSize(fifoHandler handler);
/**
* Clear the overflow flag
* @param handler the fifo handler
*/
void fifo_clearOvf(fifoHandler handler);
/**
* Clear the underflow flag
* @param handler the fifo handler
*/
void fifo_clearUdf(fifoHandler handler);
#endif

#include "fifo.h"
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <assert.h>
struct fifoHandler_s
{
    uint8_t * pu8StartPosition;
    uint8_t * pu8EndPosition;
    uint8_t * pu8WritePosition;
    uint8_t * u8ReadPosition;
    bool bFifoEmpty ;          // Buffer is empty at the beggining
    bool bFifoFull; 
    bool bFifoUnderFlow; 
    bool bFifoOverflow ; 
    bool bUsed;
} fifos[MAX_NUMBER_OF_FIFO_HANDLERS] = {{0}};

struct fifoHandler_s * fifo_create(uint8_t *buffer, const uint8_t buffsize)
{
    struct fifoHandler_s * handler=NULL;
    for(uint8_t i=0;i<MAX_NUMBER_OF_FIFO_HANDLERS && handler==NULL;i++)
    {
        if(fifos[i].bUsed==false)
            handler=&fifos[i];
    }
    if(handler==NULL)
        return NULL;
    handler->pu8StartPosition              = buffer;
    handler->pu8EndPosition                = &buffer[buffsize-1];
    handler->pu8WritePosition              = buffer;
    handler->u8ReadPosition               = buffer;
    handler->bFifoEmpty      = 1;          // Buffer is empty at the beggining
    handler->bFifoFull       = 0; 
    handler->bFifoUnderFlow        = 0; 
    handler->bFifoOverflow        = 0;    
    return handler;
}

void fifo_release(struct fifoHandler_s * handler)
{
    //assert(handler != NULL);
    for(uint8_t i=0;i<MAX_NUMBER_OF_FIFO_HANDLERS;i++)
    {
        if(handler == &fifos[i])
        {
            handler->pu8StartPosition              = NULL;
            handler->pu8EndPosition                = NULL;
            handler->pu8WritePosition              = NULL;
            handler->u8ReadPosition               = NULL;
            handler->bUsed=false;
            break;
        }
    }
}

bool fifo_getByte(struct fifoHandler_s * handler,uint8_t * const byte)
{
    //assert(handler != NULL);
    //If the fifo would be full the writeptr can not be modified 
    //and this is a safe method
    if (handler->bFifoEmpty)
    {
        handler->bFifoUnderFlow = 1; 
        return false;
    }
    
    *byte = *(handler->u8ReadPosition);
    handler->u8ReadPosition++;
    
    handler->bFifoFull = 0;

    // Move the indicator for the oldest byte one position.
    if (handler->u8ReadPosition > handler->pu8EndPosition)
        handler->u8ReadPosition = handler->pu8StartPosition;

      // Decrease the amount of bytes in the buffer
      //Critical section start
     if (handler->u8ReadPosition == handler->pu8WritePosition)
        handler->bFifoEmpty = 1;
     if(handler->u8ReadPosition != handler->pu8WritePosition)
        handler->bFifoEmpty = 0;   
       //Critical section end
    return true;
}

bool fifo_addByte(struct fifoHandler_s * handler,uint8_t const in)
{
    //assert(handler != NULL);
    if (handler->bFifoFull)
    {
        handler->bFifoOverflow = 1;
        return false;                       
    }
    // Buffer is not empty now. Add the new byte to the buffer
    handler->bFifoEmpty = 0;
    *(handler->pu8WritePosition) = in;

    // Check if the buffer is full
    handler->pu8WritePosition++;
    if (handler->pu8WritePosition > handler->pu8EndPosition)
        handler->pu8WritePosition = handler->pu8StartPosition;
    //Critical section start
    //If an ISR routine would call get Byte after checking the if, it would be 
    //executed and the fifo would not be full afterwards, that is why it is checked
    //if the fifo is full
    if(handler->pu8WritePosition == handler->u8ReadPosition)
        handler->bFifoFull = 1;
    if(handler->pu8WritePosition != handler->u8ReadPosition)
        handler->bFifoFull = 0;    
    //Critical section end

    return true;                          
}

uint8_t fifo_getBuffer(struct fifoHandler_s * handler,uint8_t * src, uint8_t numbytes)
{
    //assert(handler != NULL);
    uint8_t number = 0;
    
    while((number < numbytes)  && fifo_getByte(handler,src))
    {
      number++;
      src++;
    }
    return number;
}

uint8_t fifo_addBuffer(struct fifoHandler_s * handler,const uint8_t *src, uint8_t numbytes)
{
    //assert(handler != NULL);
  uint8_t number = 0;
  while((number < numbytes)  &&  fifo_addByte(handler,*src))
  {
    number++;
    src++;
  }
  return number;
}

uint8_t fifo_getNrBytes(struct fifoHandler_s * handler)      
{ 
    //assert(handler != NULL);
   //temporary save of pt
  uint8_t *curWritePos=handler->pu8WritePosition;
  uint8_t *curReadPos=handler->u8ReadPosition; 
  if(curReadPos>curWritePos)
     return curReadPos-curWritePos;
  else if(curReadPos<curWritePos)
     return curWritePos-curReadPos;
  else if(handler->bFifoFull)
    return  fifo_getSize(handler);
  
  return 0;
}  
bool  fifo_isEmpty(struct fifoHandler_s * handler)
{
    //assert(handler != NULL);
    return handler->bFifoEmpty == 1;
}   
 
bool  fifo_isFull(struct fifoHandler_s * handler)
{
    //assert(handler != NULL);
    return  handler->bFifoFull == 1;}  


bool  fifo_isOvf(struct fifoHandler_s * handler)
{
    //assert(handler != NULL);
    return  handler->bFifoOverflow == 1;}    


bool  fifo_isUdf(struct fifoHandler_s * handler)
{
    //assert(handler != NULL);
    return  handler->bFifoUnderFlow == 1;} 

uint8_t fifo_getSize(struct fifoHandler_s * handler)
{
    //assert(handler != NULL);
    return  handler->pu8EndPosition - handler->pu8StartPosition+1;
}


void fifo_clearOvf(struct fifoHandler_s * handler)
{
    //assert(handler != NULL);
     handler->bFifoOverflow = 0;
}

void fifo_clearUdf(struct fifoHandler_s * handler)
{
    //assert(handler != NULL);
     handler->bFifoUnderFlow = 0;
}

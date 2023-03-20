/*
 * RBuf.h
 *
 *      Author: Graunter
 */

#ifndef RING_BUF_H_
#define RING_BUF_H_

#include <stdlib.h>
#include <stdbool.h>


typedef char* pMem_t;

struct RingBuf_s {
	volatile    pMem_t	Head;   // Place for new data
	volatile    pMem_t	Tail;	// used as empty flag when set to zero
	            pMem_t	B;      // Storage it self
    unsigned            L;      // Size of the storage
};

typedef struct RingBuf_s RingBuf_t;


/// Return min of two numbers. Commonly used but never defined as part of standard headers
//#ifndef MIN
//#define MIN( n1, n2 )   ((n1) > (n2) ? (n2) : (n1))
//#endif

/// Return max of two numbers. Commonly used but never defined as part of standard headers
//#ifndef MAX
//#define MAX( n1, n2 )   ((n1) > (n2) ? (n1) : (n2))
//#endif


//------------------------------------------------------------------------------
/**
 * Initialization of internal structures
 * @param Buf: class item
 * @param Mem: Pointer to allocated memory
 * @param Len: Items to be stored
 */
void RingBuf_Init(RingBuf_t* Buf, pMem_t Mem, unsigned Len);
//------------------------------------------------------------------------------
/**
 * Return the used (and an available) space (bytes) in this class item
 * @param rb: class item
 */
unsigned RingBuf_UsedSpace(RingBuf_t *rb);
unsigned RingBuf_FreeSpace(RingBuf_t *rb);
//------------------------------------------------------------------------------
/**
 * Total size (bytes) of the internal buffer, mostly for internal use
 * @param rb
 */
unsigned RingBuf_Capacity(RingBuf_t *rb);
//------------------------------------------------------------------------------
/**
 * Fast check without size calculation
 * @param rb
 * @return
 */
bool RingBuf_IsEmpty(RingBuf_t *rb);
bool RingBuf_IsFull(RingBuf_t *rb);
//------------------------------------------------------------------------------
/**
 * Clear all saved data
 * Really just index cleared
 * @param rb
 */
void RingBuf_FastClear(RingBuf_t *rb);
//------------------------------------------------------------------------------
/**
 * Clear all saved data
 * @param rb
 */
void RingBuf_Clear(RingBuf_t *rb);
//------------------------------------------------------------------------------
/**
 * Store some data. 
 * If there is not enough space then only the leader part will be saved
 * @param rb
 * @param InBufPtr: pointer to the incoming buffer
 * @param Cnt: size (byte) on the input
 * @return: the real size of data stored
 */
unsigned RingBuf_Put(RingBuf_t *rb, pMem_t InBufPtr, unsigned Cnt);
//------------------------------------------------------------------------------
/**
 * Store some data. 
 * If there is not enough space then no data will be saved
 * @param rb
 * @param InBufPtr: pointer to the incoming buffer
 * @param Cnt: size (byte) on the input
 * @return: the real size of data stored
 */
unsigned RingBuf_PutAll(RingBuf_t *rb, pMem_t InBufPtr, unsigned Cnt);
//------------------------------------------------------------------------------
/**
 * Extract data from this object.
 * @param rb
 * @param OutBufPtr: pointer to the output buffer
 * @param Cnt: size (byte) on the output
 * @return: the real size of data stored
 */
unsigned RingBuf_Get(RingBuf_t *rb, pMem_t OutBufPtr, unsigned Cnt);
//------------------------------------------------------------------------------
/**
 * The same as before but for one byte only
 * @param rb
 * @param Dat
 * @return: TRUE if success
 */
bool RingBuf_PutOneChar(RingBuf_t *rb, char Dat);
bool RingBuf_GetOneChar(RingBuf_t *rb, char * Dat);
//------------------------------------------------------------------------------
// TODO: must be implemented together with iterators
bool RingBuf_Read(RingBuf_t *rb, pMem_t Dat) ;
//------------------------------------------------------------------------------



#endif /* RING_BUF_H_ */

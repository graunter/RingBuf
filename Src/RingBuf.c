/*
 * RingBuf.c
 *
 *      Author: Graunter
 */

#include "RingBuf.h"

#define GENERIC_MAX(x, y) ((x) > (y) ? (x) : (y))
#define GENERIC_MIN(x, y) ((x) < (y) ? (x) : (y))

#define ENSURE_int(i)       _Generic( (i), int:      (i) )
#define ENSURE_unsigned(u)  _Generic( (u), unsigned: (u) )
#define ENSURE_float(f)     _Generic( (f), float:    (f) )

#define MAX(type, x, y) (type)GENERIC_MAX(ENSURE_##type(x), ENSURE_##type(y))
#define MIN(type, x, y) (type)GENERIC_MIN(ENSURE_##type(x), ENSURE_##type(y))


void RingBuf_Init(RingBuf_t* rb, char* Mem, unsigned Len){
	rb->B = Mem;
	rb->L = Len;
	RingBuf_FastClear(rb);
};

unsigned RingBuf_UsedSpace(RingBuf_t *rb)
{
	pMem_t TmpHead = rb->Head;
	pMem_t TmpTail = rb->Tail;
	unsigned Space = 0;

	if(rb->Tail == 0){
		return 0;
	}

	if ( TmpTail <= TmpHead)
	{
		unsigned Distance = TmpHead - TmpTail;
		Space = Distance;
	}else{
		unsigned Distance = TmpTail - TmpHead;
		Space = rb->L - Distance;
	};

	return Space;
};

unsigned RingBuf_FreeSpace(RingBuf_t *rb)
{
	unsigned Space = rb->L - RingBuf_UsedSpace(rb);
    return Space;
};

unsigned RingBuf_Capacity(RingBuf_t *rb)
{
	return rb->L;
};

/*
Free space == L-1:
*   -    -       +    -       -
0   B[0] B[1] .. B[j] B[i] .. B[L-1]
                 ^    ^        
                 T    H

Free space == L:
*   -    -       -    -       -
0   B[0] B[1] .. B[j] B[i] .. B[L-1]
^                     ^        
T                     H

*/
bool RingBuf_IsEmpty(RingBuf_t *rb)
{
	bool Ret = false;
	if( 0 == rb->Tail) {
		Ret = true;
	};
	return Ret;
}

/*
Free space == 1:
*   +    +       -    +       +
0   B[0] B[1] .. B[j] B[i] .. B[L-1]
                 ^    ^        
                 H    T

Free space == L:
*   +    +       +    +       +
0   B[0] B[1] .. B[j] B[i] .. B[L-1]
                      ^ ^       
                      T H 

*/

bool RingBuf_IsFull(RingBuf_t *rb)
{
	bool Ret = false;
	if( rb->Tail == rb->Head ){
		Ret = true;
	};
	return Ret;
}

void RingBuf_FastClear(RingBuf_t *rb)
{
	rb->Head = rb->B;
	rb->Tail = 0;
}


unsigned RingBuf_Put(RingBuf_t *rb, pMem_t InBufPtr, unsigned Cnt)
{
	unsigned FreeSpace = RingBuf_FreeSpace(rb);

	unsigned TotalCnt = MIN(unsigned, Cnt, FreeSpace);
	unsigned Ret = TotalCnt;

	pMem_t TmpHead = rb->Head;
	pMem_t pEnd = &(rb->B[rb->L-1]);
	pMem_t pBeg = &(rb->B[0]);
	pMem_t pTmpIn = InBufPtr;

	while( TotalCnt-- ){
		*TmpHead = *pTmpIn;
		if(TmpHead == pEnd){
			TmpHead = pBeg;
		}else{
            ++TmpHead;
        }
		++pTmpIn;
	};

    if( 0 == rb->Tail)          // Was empty
    {
        rb->Tail = rb->Head;    // so start from the last cell
    }
    rb->Head = TmpHead;

	return Ret;
}

unsigned RingBuf_Get(RingBuf_t *rb, pMem_t pOutBuf, unsigned Cnt)
{
    if( RingBuf_IsEmpty(rb) ){
        return 0;
    }

	unsigned TotalCnt = 0;
	unsigned UsedCnt = RingBuf_UsedSpace(rb);

	TotalCnt = MIN(unsigned, Cnt, UsedCnt);
	unsigned Ret = TotalCnt;

	//pMem_t TmpHead = rb->Head;
	pMem_t TmpTail = rb->Tail;
	pMem_t End = &(rb->B[rb->L-1]);
	pMem_t Beg = &(rb->B[0]);

	pMem_t TmpOutPtr = pOutBuf;

	while( TotalCnt-- )
    {
		*TmpOutPtr = *TmpTail;
		++TmpOutPtr;

		if(TmpTail == End){
			TmpTail = Beg;
		}else{
            ++TmpTail;
        }
	}

    if( TmpTail == rb->Head ){
        rb->Tail = 0;
    }else{
        rb->Tail = TmpTail;
    }

	return Ret;
}


bool RingBuf_PutOneChar(RingBuf_t *rb, char Dat)
{
	bool Ret = RingBuf_Put(rb, &Dat, 1);
	return Ret;
}

bool RingBuf_GetOneChar(RingBuf_t *rb, char * Dat)
{
	bool Ret = RingBuf_Get(rb, Dat, 1);
	return Ret;
}


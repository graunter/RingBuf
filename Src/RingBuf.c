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
	unsigned Space = 0;
    unsigned TmpWrCnt = rb->WrCnt;
    unsigned TmpRdCnt = rb->RdCnt;

    if( TmpRdCnt <= TmpWrCnt)
	{
		Space = TmpWrCnt - TmpRdCnt;
	}else{
		unsigned Distance = TmpRdCnt - TmpWrCnt;
		Space = 
            (rb->L + 1) -       // Storage size is zero-based
            Distance +1;        // Counter by mod(L+1)
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
RdCnt != WrCnt
-    -       +    -       -
B[0] B[1] .. B[j] B[i] .. B[L-1]
             ^    ^        
             T    H

Free space == L:
RdCnt == WrCnt
-    -       -    -       -
B[0] B[1] .. B[j] B[i] .. B[L-1]
                  ^ ^        
                  T H

Free space == 0:
RdCnt != WrCnt
-    -       -    -       -
B[0] B[1] .. B[j] B[i] .. B[L-1]
                  ^ ^        
                  T H
*/
bool RingBuf_IsEmpty(RingBuf_t *rb)
{
	bool Ret = false;
	if( (rb->RdCnt == rb->WrCnt) & (rb->Tail == rb->Head) ) {
		Ret = true;
	};
	return Ret;
}

bool RingBuf_IsFull(RingBuf_t *rb)
{
	bool Ret = false;
	if( (rb->RdCnt != rb->WrCnt) & (rb->Tail == rb->Head) ) {
		Ret = true;
	};
	return Ret;
}

void RingBuf_FastClear(RingBuf_t *rb)
{
	rb->Head = rb->B;
	rb->Tail = rb->B;
    rb->RdCnt = 0;
    rb->WrCnt = 0;
}

void RingBuf_Clear(RingBuf_t *rb)
{
	// TODO: there is no reason for the real data copy
	// just rd ptp and cnt can be changed
	char TmpCh = 0;
	while ( RingBuf_GetOneChar( rb, &TmpCh) ) {};
}

unsigned RingBuf_Put(RingBuf_t *rb, pMem_t InBufPtr, unsigned Cnt)
{
	if( 0 == InBufPtr ){
		return 0;
	}
	
	unsigned FreeSpace = RingBuf_FreeSpace(rb);

	unsigned TotalCnt = MIN(unsigned, Cnt, FreeSpace);
	unsigned Ret = TotalCnt;

	pMem_t TmpHead = rb->Head;
	pMem_t pEnd = &(rb->B[rb->L-1]);
	pMem_t pBeg = &(rb->B[0]);
    unsigned TmpWrCnt = rb->WrCnt;
    //unsigned TmpRdCnt = rb->RdCnt;

	pMem_t pTmpIn = InBufPtr;

	while( TotalCnt-- ){
		*TmpHead = *pTmpIn;
		if(TmpHead == pEnd){
			TmpHead = pBeg;
		}else{
            ++TmpHead;
        }
        // An extra tick is used to separate an empty and full states
        if( TmpWrCnt == rb->L+1 ){  
            TmpWrCnt = 0;
        }else{
            ++TmpWrCnt;
        }
		++pTmpIn;
	};

    rb->Head = TmpHead;
    rb->WrCnt = TmpWrCnt;

	return Ret;
}

unsigned RingBuf_Get(RingBuf_t *rb, pMem_t pOutBuf, unsigned Cnt)
{
    // Optimisation - usually we read faster than new data comes in
    if( RingBuf_IsEmpty(rb) ){
        return 0;
    }

	if( 0 == pOutBuf){
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
    unsigned TmpRdCnt = rb->RdCnt;

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
        // An extra tick is used to separate an empty and full states
        if( TmpRdCnt == rb->L+1 ){  
            TmpRdCnt = 0;
        }else{
            ++TmpRdCnt;
        }
	}

    rb->Tail = TmpTail;
    rb->RdCnt = TmpRdCnt;

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


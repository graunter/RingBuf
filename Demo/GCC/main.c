#include "RingBuf.h"
#include <stdio.h>
#include <string.h>


#define OK_GREEN "\x1B[32mOK\033[0m"
#define ERR_RED "\x1B[31mERR\033[0m"
#define START_W "\x1B[37mTest\033[0m"

#define CHECK_EQ( Expr, Exp, ErrMsg ) \
    if( Expr != Exp) \
    {\
        printf( ERR_RED": %s: line=%d, Fn=%s \n", ErrMsg, __LINE__, __func__ );\
    }

#define CHECK_NE( Expr, Exp, ErrMsg ) \
    if( Expr == Exp) \
    {\
        printf( ERR_RED": %s: line=%d, Fn=%s \n", ErrMsg, __LINE__, __func__ );\
    }

enum {eTestMemSize = 16};
static char MemBuf[eTestMemSize];

void TestFn_1(void)
{
    printf(START_W ": Initialization\n");

    char* BufPtr = MemBuf;
    memset(BufPtr, 0, eTestMemSize);

    RingBuf_t TstRB;
    bool Ret = false;

    RingBuf_Init( &TstRB, BufPtr, eTestMemSize );

    CHECK_EQ( RingBuf_IsEmpty( &TstRB ), true, "Wrong storage space after init");
    CHECK_EQ( RingBuf_IsFull( &TstRB ), false, "Wrong storage space after init");
    CHECK_EQ( RingBuf_FreeSpace( &TstRB ), eTestMemSize, "Wrong storage space after init");
    CHECK_EQ( RingBuf_UsedSpace( &TstRB ), 0, "Wrong storage space after init");    
    CHECK_EQ( RingBuf_Capacity( &TstRB ), eTestMemSize, "Wrong storage space after init");  

    char OneCh = '1';

    Ret = RingBuf_PutOneChar( &TstRB, OneCh );
    CHECK_EQ( Ret, true, "can't put one char into empty buffer");
    CHECK_EQ( RingBuf_IsEmpty( &TstRB ), false, "Wrong storage space after one char");
    CHECK_EQ( RingBuf_IsFull( &TstRB ), false, "Wrong storage space after one char");
    CHECK_EQ( RingBuf_FreeSpace( &TstRB ), eTestMemSize-1, "Wrong storage space after one char");
    CHECK_EQ( RingBuf_UsedSpace( &TstRB ), 1, "Wrong storage space after one char");    
    CHECK_EQ( RingBuf_Capacity( &TstRB ), eTestMemSize, "Wrong storage space after one char");  

    char SecCh = '2';

    Ret = RingBuf_PutOneChar( &TstRB, SecCh );
    CHECK_EQ( Ret, true, "can't put one char into non empty buffer");
    CHECK_EQ( RingBuf_IsEmpty( &TstRB ), false, "Wrong storage space after second char");
    CHECK_EQ( RingBuf_IsFull( &TstRB ), false, "Wrong storage space after second char");
    CHECK_EQ( RingBuf_FreeSpace( &TstRB ), eTestMemSize-2, "Wrong storage space after second char");
    CHECK_EQ( RingBuf_UsedSpace( &TstRB ), 2, "Wrong storage space after second char");    
    CHECK_EQ( RingBuf_Capacity( &TstRB ), eTestMemSize, "Wrong storage space after second char");  

    char TstFirstCh = 0;

    Ret = RingBuf_GetOneChar( &TstRB, &TstFirstCh );
    CHECK_EQ( Ret, true, "can't take one char from non empty buffer");
    CHECK_EQ( OneCh, TstFirstCh, "can't extracted char incorrekt");    
    CHECK_EQ( RingBuf_IsEmpty( &TstRB ), false, "Wrong storage space after take one char from non empty buffer");
    CHECK_EQ( RingBuf_IsFull( &TstRB ), false, "Wrong storage space after take one char from non empty buffer");
    CHECK_EQ( RingBuf_FreeSpace( &TstRB ), eTestMemSize-1, "Wrong storage space take one char from non empty buffer");
    CHECK_EQ( RingBuf_UsedSpace( &TstRB ), 1, "Wrong storage space after take one char from non empty buffer");    
    CHECK_EQ( RingBuf_Capacity( &TstRB ), eTestMemSize, "Wrong storage space after take one char from non empty buffer");      

    char TstSecCh = 0;

    Ret = RingBuf_GetOneChar( &TstRB, &TstSecCh );
    CHECK_EQ( Ret, true, "can't take last char from non empty buffer");
    CHECK_EQ( OneCh, TstFirstCh, "can't extract last char - incorrekt");    
    CHECK_EQ( RingBuf_IsEmpty( &TstRB ), true, "Wrong storage space after take last char from non empty buffer");
    CHECK_EQ( RingBuf_IsFull( &TstRB ), false, "Wrong storage space after take last char from non empty buffer");
    CHECK_EQ( RingBuf_FreeSpace( &TstRB ), eTestMemSize, "Wrong storage space take last char from non empty buffer");
    CHECK_EQ( RingBuf_UsedSpace( &TstRB ), 0, "Wrong storage space after take last char from non empty buffer");    
    CHECK_EQ( RingBuf_Capacity( &TstRB ), eTestMemSize, "Wrong storage space after take last char from non empty buffer");      

    char TstExtraCh = 0;

    Ret = RingBuf_GetOneChar( &TstRB, &TstExtraCh );
    CHECK_EQ( Ret, false, "wrong read from empty buffer");
    CHECK_EQ( 0, TstExtraCh, "mem was corrupt after read from empty buffer");    
    CHECK_EQ( RingBuf_IsEmpty( &TstRB ), true, "Wrong storage space after take last char from empty buffer");
    CHECK_EQ( RingBuf_IsFull( &TstRB ), false, "Wrong storage space after take last char from empty buffer");
    CHECK_EQ( RingBuf_FreeSpace( &TstRB ), eTestMemSize, "Wrong storage space take last char from empty buffer");
    CHECK_EQ( RingBuf_UsedSpace( &TstRB ), 0, "Wrong storage space after take last char from empty buffer");    
    CHECK_EQ( RingBuf_Capacity( &TstRB ), eTestMemSize, "Wrong storage space after take last char from empty buffer");      


}

int main(void)
{
    TestFn_1();
}
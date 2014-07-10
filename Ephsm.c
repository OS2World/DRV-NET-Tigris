 /*
**      ephsm.c - EuphPhyter implementation of class "Hsm"
**
**      $Archive:   /tmp_mnt/n/crabapple/nsclib/proj/euphyter/sw/lib/vcs/ephsm.c_v  $
**      $Author: tkelley $
**      $Revision: 1.4 $
**
**      $Log: ephsm.c,v $
 * Revision 1.4  1998/09/03  21:12:07  tkelley
 * updated 'elEEload' function
 *
 * Revision 1.3  1998/08/04  00:20:40  tkelley
 * commented out MibPRST, BISE & TxRTCNT_shift due to errors
 *
 * Revision 1.2  1998/08/03  22:13:18  tkelley
 * modified header files define statements
 *
**
**         Rev 1.14   03/18/97 12:31:12   lenglish
**      changed 3 instances of "//" comments. wle.
**      .
**
**         Rev 1.13   12/05/96 16:50:06   rdunlap
**      use PCI_SIS_VENDOR_ID instead of PCI_NSC_VENDOR_ID (2 places);
**
**         Rev 1.12   12/04/96 15:55:46   rdunlap
**      add SIS corrections and comment lines;
**      add & delete elDebug() calls to see why HsmInitialize() returns
**        HsmHWFailure;
**      add & delete elDebug() calls to see why adapter always inits. at
**        100 Mbps full duplex;
**      ifdef DEBUG and MSDOS and __WATCOMC__ then define HSM_DEBUG and
**        HSMDEBUG so that elDebug() can work;
**      delete checking for NSC OUI in PermMacAddr completely;
**      in elParseDevice(), if the device ID is not recognized, return
**        HsmHWFailure;
**      in elEEload(), do a sanity check on mediaSelection and set it to
**        AUTO_CONFIG if it is invalid;
**      delete trailing spaces at ends of all lines (helps with DIFF);
**      add SIS_7006_SVB in elParseDeviceID();
**      in various places, change "1" to ((uint32)1) for left shift (<<) and
**        store into a uint32 value [SIS];
**      correct and add some comments;
**      in elEEwritew(), set invalue to (uint32) value & use invalue when
**        calling elEEput() [SIS];
**      change "1" to ((uint32)1) when calling HsmSetMediaType();
**      in HsmMulticastLoad(), coerce to (uint32) when using left shift (<<);
**      multiple corrections in HsmTransmit() for non-NSM_DOUBLE_BUFFER:
**        coerce dsize rvalue to uint16; coerce min() arg to int;
**        don't ++frag (there is only 1 frag per descriptor);
**
**         Rev 1.11   11/11/96 11:46:42   rdunlap
**      change constant 4 to CRC_SIZE;
**      add NDIS3 DEBUG support;
**      add #include "ndis.h" file for NDIS3;
**      add NDIS3 pragmas for code sections;
**      in HsmRxCopyPkt(), HsmService(), and HsmTransmit(), coerce data types
**         Rev 1.7   05/23/96 14:23:50   rdunlap
**      in HsmMulticastLoad(), use NSM_MEMZERO() so that a call to a lib.
**        function (STOSB) won't be used;
**
**         Rev 1.6   05/23/96 12:45:28   rdunlap
**      delete unused variables "l" in elEEget() and "control" in
**        elPMDreadMode();
**
**         Rev 1.5   05/23/96 12:19:34   rdunlap
**      move elSetReceiveFilter() to the res_code area for ODI16;
**
**         Rev 1.4   05/23/96 10:39:38   rdunlap
**      for NDIS3, set HSM_DEBUG iff DEBUG is defined;
**      for NDIS3, set NSM_DOUBLE_BUFFER;
**
**         Rev 1.3   05/23/96 10:11:40   rdunlap
**      in HsmTransmit(), #ifdef NSM_DOUBLE_BUFFER, if (cmdsts & MORE) then
**        must set txpd to txpd->lLink;
**
**         Rev 1.2   05/21/96 17:39:18   rdunlap
**      add "static" to elMIIget() to match its prototype;
**
**         Rev 1.1   11/21/95 15:35:34   croussel
**      completed the implementations of MII and EEPROM support.
**
**         Rev 1.0   11/17/95 15:10:20   croussel
**      Initial revision.
**
**
*/

/* conditionally define HSM_DEBUG & HSMDEBUG for HSMMON DOS program
** (compiled by Watcom C) */
#ifdef DEBUG
#ifdef MSDOS
#ifdef __WATCOMC__
#define HSM_DEBUG
#define HSMDEBUG
#include <stdio.h>
#endif
#endif
#endif


#ifdef  __cplusplus
extern "C" {
#endif

#ifndef  _nsctypes_h_
#include "nsctypes.h"
#endif

#ifndef  _nsm_h_
#include "nsm.h"
#endif

#ifndef  _hsm_h_
#include "hsm.h"
#endif

#ifndef  _cdi_h_
#include "cdi.h"
#endif

#ifndef  _ieeemii_h_
#include "ieeemii.h"
#endif

#ifndef  _infomovr_h_
#include "infomovr.h"
#endif

#ifndef  _euphyter_h_
#include "euphyter.h"
#endif

#ifndef  _ephsm_h_
#include "ephsm.h"
#endif


//#define NSM_IO_WRITE32
//#define NsmIOwrite32(_p,_v)             \
//{					\
//	uint32  __v = _v;		\
//	uint16	__p = _p;		\
//	_asm 	_emit 	0x66		\
//	_asm 	_emit 	0x50		\
//	_asm 	_emit	0x66		\
//	_asm	mov	ax, word ptr __v	\
//	_asm 	mov	dx, __p		\
//	_asm 	_emit	0x66		\
//	_asm	out	dx, ax		\
//	_asm 	_emit 	0x66		\
//	_asm 	_emit 	0x58		\
//}


/* make NDIS3 "DEBUG" cause HSM_DEBUG here */
#ifdef _MSC_VER
#ifdef _NSMSFT_
#ifdef NDIS_WIN
#ifdef DEBUG
#define HSM_DEBUG
#endif
#endif
#endif
#endif


/* check HSM_CONTEXT_SIZE ... */
#ifdef __BORLANDC__
#if (HSM_CONTEXT_SIZE > (sizeof(EuphPhyterHsmContext) + 32))
#error  "HSM_CONTEXT_SIZE is too large"
#endif
#if (HSM_CONTEXT_SIZE < sizeof(EuphPhyterHsmContext))
#error  "HSM_CONTEXT_SIZE is too small"
#endif
#endif


/* make NDIS3 "DEBUG" cause HSM_DEBUG here */
#ifdef DEBUG
#ifdef NDIS_NT          /* WinNT */
#define HSM_DEBUG
#endif
#ifdef CHICAGO          /* Win95 */
#define HSM_DEBUG
#endif
#endif

/* for NDIS_NT or CHICAGO compiles, use <ndis.h> */
#ifdef NDIS_NT
#include <ndis.h>
#endif
#ifdef CHICAGO
#include <ndis.h>
#endif


/* conditionally compile in debug messages */
#ifdef HSM_DEBUG
#define epDebug(x)      \
                { if( hsm->nsmOptions & DO_DEBUG ) NsmDbugMessage(hsm,x); }
#else
#define epDebug(x)
#endif

/* pre-declare local functions ... */

/* init_decl_begin */
static void     epReadMACAddress( HsmContext * );
static void     epScanPMDs( HsmContext * );
static int      epParseDeviceID( HsmContext * );
static int      epFailure( HsmContext * );
       void     epResetPMD( HsmContext * );
static uint16   epAutoNegotiate( HsmContext *, PMD * );
static int      epEEread( HsmContext *, uint16, Puint16 , uint16 );
static int      epEEload( HsmContext * );
static void     epEEput( HsmContext *hsm, uint32 value, int nbits );
static uint16   epEEget( HsmContext *hsm, int nbits );
static uint16   epEEreadw( HsmContext *hsm, int offset );
static void     HSM_WAIT_US( HsmContext *hsm, uint16 microSecond );
#if 0
static void     epEEwritew( HsmContext *hsm, int offset, uint16 value );
static int      epEEwrite( HsmContext *, uint16, Puint16, uint16 );
static int      epEEupdateChecksum( HsmContext * );
static int      epEEwriteEnable( HsmContext *, bool );
#endif
/* init_decl_end */

/* res_decl_begin */
#ifndef  NECWARP
static void     epRxInit( HsmContext * );
#else
void            epRxInit( HsmContext * );
#endif
static void     epTxInit( HsmContext * );
//static void     epRxReset( HsmContext * );
static void     epFreeBuffers( HsmContext * );
//static void     epWaitTime( int );
//
//static void     epSetReceiveFilter( HsmContext * );

static void     epPollPMDs( HsmContext * );
static void     epMIIreset( HsmContext * );
static void     epMIIput( HsmContext *, uint32, int );
static uint32   epMIIget( HsmContext *, int );
static uint16   epMIIread( HsmContext *, int, int );
static void     epMIIwrite( HsmContext *, int, int, uint16 );
static bool     epMIIpollBit(
                        HsmContext *, int, int, uint16, bool, Puint16 );
static void     epSetMediaType( HsmContext *, int, int );
static void     epPMDreadMode( HsmContext *, Puint16, Puint16 );
static void     epIsolateOthers( HsmContext *, PMD * );
static uint16   epLookForLink( HsmContext * );
static bool     isPlexus( PMD * );

#ifdef  NSM_SUPPLIES_RX_BUFFERS
extern void     NsmRxReset( HsmContext * );
extern int      NsmGetRxBuffer( HsmContext *, pHsmPktDesc );
extern void     NsmReturnRxBuffer( HsmContext *, pHsmPktDesc );
extern void     NsmReceive( HsmContext *, pHsmPktDesc , uint16 );
#endif
/* res_decl_end */

/* assembler parts in ephsma.asm */
#ifdef  NECWARP
extern bool     HsmServiceTransmit( HsmContext * );
extern bool     HsmServiceReceive( HsmContext * );
//extern int      epComputeHashTableIndex( Puchar );
#endif


/* make code Locked/Resident for Win95 */
#ifdef NDIS_WIN
  #pragma LCODE
#endif


/* res_code_begin */
/*
**      HsmForceInterrupt( )    force an interrupt (if supported)
*/

int HsmForceInterrupt(
    HsmContext *hsm             /* pointer to hsm's context */
) {
    IOW32( cr, SWI );
    return( HsmOK );
}


/*      HsmTimerEvent( )        called from Nsm when timer expires */
void HsmTimerEvent(
    HsmContext *hsm             /* pointer to hsm's context */
) {
    epPollPMDs( hsm );  /* go and poll PMDs for status */
    NsmStartTimer( hsm, PMD_POLL_INTERVAL );
}


/*      HsmReset( )             Put the NIC in a known state, ready to
**                              receive and transmit.
*/
int HsmReset(
    HsmContext *hsm             /* pointer to hsm's context */
) {
   EuphPhyterHsmContext        *ep = (EuphPhyterHsmContext *) hsm;

   /* reset the chip, while we initialize data structures  */
   IOW32( ier, 0 );
   IOW32( cr, RxRESET | TxRESET | RESET );

   ep->isrValue = 0;
   ep->imrValue = 0;

   while ( IOR32( cr ) & RESET ) {
   }

   IOW32( cfg, PESEL | STRP_DUP | STRP_SPD | STRP_AN );

   /* clear all MIB counters to 0 */
   IOW32( mibc, mibACLR );

   /* set the current MAC address */
   HsmSetMacAddr( hsm, &hsm->CurrMacAddr );

   /* reset the multicast table */
   if( hsm->MulticastTableSize )
       HsmMulticastLoad( hsm );

   epTxInit( hsm );                    /* reset transmit stuff */
   epRxInit( hsm );            /* reset receive stuff */

   /* restore everything to it's last set state */
   HsmSetRxMode( hsm, hsm->rxMode );

   hsm->hsmState = HSM_INITIALIZED_STATE;
   NsmHsmEvent( hsm, HsmStateChange );

   return( HsmOK );
}


/*      HsmOpen( )              prepares a NIC for data transfer.
**                              options must already be set in HsmContext.
*/
int HsmOpen(
    HsmContext *hsm             /* pointer to hsm's context */
) {
    EuphPhyterHsmContext        *ep = (EuphPhyterHsmContext *) hsm;

    if( hsm->LinkState == 0xff )        /* no media type selected yet */
//        HsmSetMediaType( hsm, ((uint32)1) << HSM_MEDIA_AUTO_CONFIG );
        HsmSetMediaType( hsm, HSM_MEDIA_AUTO_CONFIG );

    /* we always have this many */
    ep->imrValue =
        MIBINT | SWINT |
        RxOK | RxERR | RxORN | RxSOVR |
        TxOK | TxERR | TxURN;

    /* are early interrupts configured? */
    if( hsm->nsmOptions & DO_RX_PIPELINING ) {
        ep->imrValue |= RxEARLY;
    }

    IOW32( imr, ep->imrValue );
    IOW32( ier, IE );
    IOW32( cr, RxENA );         /* unreset and start rx */

    hsm->hsmState = HSM_OPEN_STATE;
    NsmHsmEvent( hsm, HsmStateChange );

    return( HsmOK );
}


/*      HsmClose( )             shutdown a NIC.  No more data transfer
**                              after calling HsmClose.
*/
int HsmClose(
    HsmContext *hsm             /* pointer to hsm's context */
) {
    EuphPhyterHsmContext        *ep = (EuphPhyterHsmContext *) hsm;

    IOW32( ier, 0 );
    IOW32( cr, RxDIS | TxDIS );

    ep->imrValue = 0;
    IOW32( imr, 0 );            /* disable ints */

    hsm->hsmState = HSM_CLOSED_STATE;
    NsmHsmEvent( hsm, HsmStateChange );

    return( HsmOK );
}


/*
**      HsmShutdown( )          shutdown the NIC
*/
int HsmShutdown(
    HsmContext *hsm             /* pointer to hsm's context */
) {
    IOW32( ier, 0 );
    IOW32( cr, RxRESET | TxRESET | RESET );

   while ( IOR32( cr ) & RESET ) {
   }

    epFreeBuffers( hsm );
    return( HsmOK );
}



/*      HsmUpdateStatistics( )  Update the statistics portion of the
**                              HsmContext
*/
void HsmUpdateStatistics(
    HsmContext *hsm             /* pointer to hsm's context */
) {
    /* stop background updates */
    NsmEnterCriticalSection( hsm, HSM_ALL_CRITICAL_SECTION );

    /* add the mib counters to their current values */
    hsm->RxPktErrors    += IOR32( mibs + rxErroredPkts );
    hsm->RxFCSErrors    += IOR32( mibs + rxFCSerrors );
    hsm->RxMissedPkts   += IOR32( mibs + rxMissedPkts );
    hsm->RxFAEErrors    += IOR32( mibs + rxFAEerrors );
    hsm->RxSymbolErrors += IOR32( mibs + rxSymbolErrors );
    hsm->RxFramesTooLong += IOR32( mibs + rxFramesTooLong );
    hsm->TxSQE          += IOR32( mibs + txSQEerrors );

    /* turn it loose */
    NsmLeaveCriticalSection( hsm, HSM_ALL_CRITICAL_SECTION );
}


/*      HsmZeroStatistics( )    Zero the statistics portion of the
**                              HsmContext
*/
void HsmZeroStatistics(
    HsmContext *hsm             /* pointer to hsm's context */
) {
    /* stop background updates */
    NsmEnterCriticalSection( hsm, HSM_ALL_CRITICAL_SECTION );

    NSM_MEMZERO( &hsm->RxOctetsOK,
        (int)(&hsm->TxCarrierSenseLost - &hsm->RxOctetsOK) );
    IOW32( mibc, mibACLR );

    /* turn it loose */
    NsmLeaveCriticalSection( hsm, HSM_ALL_CRITICAL_SECTION );
}


/*      HsmFreezeStatistics( )  Set the operation mode of the
**                              mib statistic counters
*/
void HsmFreezeStatistics(
    HsmContext *hsm,            /* pointer to hsm's context */
    int val
) {
        IOW32( mibc, mibFRZ );
}


/*      HsmStrobeStatistics( )  Increment all physical mib counters
*/
void HsmStrobeStatistics(
    HsmContext *hsm,            /* pointer to hsm's context */
    int val
) {
        IOW32( mibc, mibSTR );
}


/*      HsmResetStatistics( )   Reset hsm and physical mib counters
*/
void HsmResetStatistics(
    HsmContext *hsm,             /* pointer to hsm's context */
    int val
) {
   /* stop background updates */
   NsmEnterCriticalSection( hsm, HSM_ALL_CRITICAL_SECTION );

   NSM_MEMZERO( &hsm->RxOctetsOK,
       (int)(&hsm->TxCarrierSenseLost - &hsm->RxOctetsOK) );
   IOW32( mibc, mibACLR );

   /* turn it loose */
   NsmLeaveCriticalSection( hsm, HSM_ALL_CRITICAL_SECTION );
}



/*      HsmGetMibStatus( )  Determine if the Mib has started or stopped
*/
int HsmGetMibStatus(
    HsmContext *hsm             /* pointer to hsm's context */
) {
	int val;

	val=IOR32(mibc) &  2;
	return(val);
}


/*
**      HsmRxFreePkt( )         release receive descriptors and buffers
**                              that were held in place.  This call is only
**                              invoked when doing receive copy avoidance.
**                              (nsmOption.DO_RX_PKTDESC &
**                                      hsmOptions.CAN_DO_RX_PKTDESC)
**                              The buffer was retained when a previous upcall
**                              to NsmRxLookahead returned "NsmRxPktHold".
*/
#ifndef  NECWARP
int HsmRxFreePkt(
    HsmContext *hsm,
    void *handle                /* handle previously passed to NsmRxLookahead */
) {
    return( HsmNotImplemented );
}
#endif

/*
**      HsmRxFlowControl( )     start/stop the upcall of NsmRxLookahead
**                              by some hardware dependent method.
**                              (This could be done by masking off the
**                              hardware receive interrupt).
*/
int HsmRxFlowControl(
    HsmContext *hsm,
    bool stopFlow               /* set to stop, clr to start */
) {
    EuphPhyterHsmContext        *ep = (EuphPhyterHsmContext *) hsm;

    if( stopFlow ) {
//#if 0
        ep->imrValue &= ~(RxOK | RxERR | RxORN | RxSOVR | RxEARLY );
        IOW32( imr, ep->imrValue );
    } else {
        ep->imrValue |= (RxOK | RxERR | RxORN | RxSOVR );
        if( hsm->nsmOptions & DO_RX_PIPELINING ) {
           ep->imrValue |= RxEARLY;
        }
        IOW32( imr, ep->imrValue );
    }
//#endif
#if 0
       IOW32( ier, 0 );
    } else {
       IOW32( ier, 1 );
    }
#endif
    return( HsmOK );
}


/*      HsmSetRxLookahead( )    Set the size of the receive lookahead space.
*/

int HsmSetRxLookahead(
    HsmContext *hsm,            /* pointer to hsm's context */
    int size                    /* mininimum size that Hsm should have */
                                /* available before calling NsmRxLookahead */
) {
#if 0  /* do nothing, since Lookahead is never used */
    if ( hsm->rxLookaheadSize < size ) {
        hsm->rxLookaheadSize = size;
    }
    if(( hsm->nsmOptions & DO_RX_PIPELINING )&& size ) {

        EuphPhyterHsmContext *ep = (EuphPhyterHsmContext *)hsm;

        /* we need to align the drain threshold with the early threshold
        ** don't set it to less than 20, or we could get PREJ packets
        ** if between 20 and 64, then set it to the next highest valid value
        ** if between 64 and 128, then set it to half
        ** EuphPhyter will always try to keep 4 bytes in the FIFO, so we need
        ** to bump by 4 as well */
        if( hsm->rxLookaheadSize < 20 ) {
           ep->rxDrainThreshold = (20+11) >> 2;
        } else if( hsm->rxLookaheadSize < 64 ) {
           ep->rxDrainThreshold = (size+11) >> 2;
        } else if( hsm->rxLookaheadSize < 128 ) {
           ep->rxDrainThreshold = (size+19) >> 3;
        } else {
           ep->rxDrainThreshold = 64 >> 2;
        }
        /* EuphPhytery ignores the LSB of the drain threshold setting, so
        ** clear it anyway */
        ep->rxDrainThreshold &= ~1;
        IOW32( rxcfg, (IOR32( rxcfg ) & ~RxDRNT) | ep->rxDrainThreshold );
    }
#endif
    return( HsmOK );
}

#ifndef NSM_DATA_XFER           /* allows NSM's to optimize data handling */
#ifndef NSM_SUPPLIES_RX_BUFFERS

/*      HsmRxCopyPkt( )         Copy the packet referred to by "handle" to
**                              the buffers described int pktDesc.  Skip over
**                              the number of bytes specified by "offset", and
**                              copy only as many bytes as specified in "count".
*/
int HsmRxCopyPkt(
    HsmContext  *hsm,           /* pointer to hsm's context */
    void        *handle,        /* packet's handle (passed to NsmRxLookahead) */
    int         offset,         /* offset into packet to copy from */
    int         count,          /* size to copy */
    HsmPktDesc  *pktDesc,       /* HsmPktDesc containing data buffers */
    int         *sts            /* final packet status (if lookahead) */
) {
    int                 i;
    uint32              cmdsts;
    Puchar              pf;
    EuphPhyterHsmContext        *ep = (EuphPhyterHsmContext *)hsm;
    pEuphPhyterDesc     rxd = ep->rxHead;
    HsmFrag             *frag;
    int                 cnt;
    uint16              dsize;

    cmdsts = rxd->cmdsts;
    if( ep->rxLookState ) {
        /* we probably don't have the packet yet so we can only copy it,
        ** if he wants less than rxLookahead size, we're OK, or if it
        ** came in while he was off getting buffers */
        if( count > hsm->rxLookaheadSize ) {
            if( !(cmdsts & OWN)  ) {
                /* if it's not mine, can't do it now */
                return( HsmPacketNotReady );
            }
        }
        ep->rxLookState = RX_LOOK_COPIED;       /* show that we got it */
    }

    /* copy the data from the buffer into the system RCB's format ... */
    frag = &pktDesc->frags[0];
    pf = rxd->buf + offset;

    dsize = (uint16) (cmdsts & DSIZE);
    dsize -= CRC_SIZE;
    cnt = min( count, dsize - offset );
    count -= cnt;
    while( cnt ) {           /* while data and frags    */
        i = min( cnt, (int)(frag->cnt) );
        NSM_MEMCPY( (Puchar) frag->fptr, pf, i );
        pf += i;
        cnt -= i;
        ++frag;
    }
    if( count ) {
        pktDesc->descByteCnt -= count;
    }
    return( HsmOK );
}
#endif

/*      HsmDisableNicInts( )    disable interrupts from this NIC.       */

#ifndef  NECWARP
int HsmDisableNicInts(
    HsmContext *hsm             /* pointer to hsm's context */
) {
    EuphPhyterHsmContext        *ep = (EuphPhyterHsmContext *) hsm;
    uint32              val;
    int    rc = 0;

    IOW32( ier, 0 );            /* disable all EuphPhyter interrupts */

    val = IOR32( isr );
    ep->isrValue |= val;

    if( ep->isrValue & ep->imrValue )
       rc |= 1;
    if( ( val & ( RxOK | RxERR | RxORN | RxSOVR )) &&
        !( ep->imrValue & ( RxOK | RxERR | RxORN | RxSOVR )) )
       rc |= 2;

    return( rc );

//    return( (ep->isrValue & ep->imrValue) != 0 );
}
#endif


/*      HsmEnableNicInts( )     enable interrupts from this NIC.        */

#ifndef  NECWARP
void HsmEnableNicInts(
    HsmContext *hsm             /* pointer to hsm's context */
) {
    IOW32( ier, IE );           /* enable all currently unmasked ints */
}
#endif

/*      HsmService( )           Process any current events that might be
**                              causing an interrupt on the NIC.
**                              Return a code if there are no events to service.*/
#ifndef  NECWARP
int HsmService(
    HsmContext *hsm             /* pointer to hsm's context */
) {
    EuphPhyterHsmContext *ep = (EuphPhyterHsmContext *)hsm;
    register    pEuphPhyterDesc txd, txdLast;
    pHsmPktDesc  txpd;
    register    pEuphPhyterDesc rxd;
    uint32      sts, cmdsts;
    uint16      txsts, rxsts, dsize;
    uint32      txHandle;
    bool        txFirst = TRUE;         /* true if next descriptor is first
                                        ** descriptor of a packet */
    bool        traffic;
    uint16      loopCount = 0;

    sts = ep->isrValue & ep->imrValue;
    ep->isrValue &= !ep->imrValue;
    while ( sts ) {

       /* process all other low priority stuff */
       if( sts &( MIBINT | TXRCMP | RXRCMP | RxSOVR | SWINT ) ) {
           if( sts & MIBINT ) {
               HsmUpdateStatistics( hsm );
           }

           /* receiver has been reset, re-enable him */
           if(( sts & RXRCMP )&& ep->rxResetPending ) {
               epDebug("(RXRCMP)");
               ep->rxResetPending = FALSE;
               ep->imrValue &= ~RXRCMP;
               ep->imrValue |= (RxOK | RxERR | RxORN | RxSOVR);
               if( hsm->nsmOptions & DO_RX_PIPELINING ) {
                   ep->imrValue |= RxEARLY;
               }
               IOW32( imr, ep->imrValue );
               epRxInit( hsm );
               IOW32( cr, RxENA );
           }

           /* software interrupt */
           if( sts & SWINT ) {
               ;       /* do nothing */
           }
       }

       do {
          traffic = FALSE;
          loopCount++;

          if ( sts & ( TxOK | TxERR | TxURN ) ) {
      #ifdef  NECWARP
             while( HsmServiceTransmit( hsm ));
//             traffic = HsmServiceTransmit( hsm );
      #else
              /* process transmit events */
              if ( ( txd = ep->txHead ) && !( ( cmdsts = txd->cmdsts ) & OWN ) ) {
                  traffic = TRUE;
  #ifdef NSM_DOUBLE_BUFFER
                  NsmEnterCriticalSection( hsm, HSM_TX_CRITICAL_SECTION );
                  ep->txHead = (pEuphPhyterDesc) txd->lLink;
  //#ifdef NSM_DOUBLE_BUFFER
                  txpd = ( pHsmPktDesc )txd->handle;
                  txHandle = txpd->handle;
                  do {
                     if( !ep->txDescHead ) {
                         ep->txDescHead = txpd;
                     } else {
                         ep->txDescTail->lLink = (uint32) txpd;
                     }
                     ep->txDescTail = txpd;
                     txpd = ( pHsmPktDesc )txpd->lLink;
                     ++hsm->txQFree;
                  } while ( txpd );
                  txd->lLink = 0;
                  /* maintain a queue of free descriptors */
                  if( !ep->txFreeHead ) {         /* free list is empty */
                      ep->txFreeHead = txd;
                  } else {    /* not empty */
                      txdLast = ep->txFreeTail;
                      txdLast->lLink = (uint32) txd;
                      txdLast->pLink = txd->physAddr;
                  }
                  ep->txFreeTail = txd;
  //#else
  //#endif
                  NsmLeaveCriticalSection( hsm, HSM_TX_CRITICAL_SECTION );

                  if( !( cmdsts & MORE ) ) {
                      txsts = (uint16) ((cmdsts >> 16) & 0x7ff);
                      NsmTransmitComplete( hsm, txHandle, (int)txsts );
                      txFirst = TRUE;
                  }
  #else
                  txdLast = txd;
                  while ( cmdsts & MORE ) {   /* find last fragment */
                     if ( !( txdLast = (pEuphPhyterDesc) txdLast->lLink ) || 
                          (( cmdsts = txdLast->cmdsts ) & OWN ) ) {
                        traffic = FALSE;
                        break;
                     }
                  }
                  if ( traffic ) {
                     NsmEnterCriticalSection( hsm, HSM_TX_CRITICAL_SECTION );

                     txpd = (pHsmPktDesc) txdLast->handle;
                     txHandle = txpd->handle;
                     do {
                        if ( !ep->txDescHead ) {
                           ep->txDescHead = txpd;
                        } else {
                           ep->txDescTail->lLink = ( uint32 ) txpd;
                        }
                        ep->txDescTail = txpd;
                        txpd = ( pHsmPktDesc ) txpd->lLink;
                        ++hsm->txQFree;
                     } while ( txpd );

//                     if (ep->txHead = (pEuphPhyterDesc) txdLast->lLink) 
//                        IOW32 (txdp, ep->txHead->physAddr);
                     ep->txHead = (pEuphPhyterDesc) txdLast->lLink;

                     txdLast->lLink = 0;
                     if ( !ep->txFreeHead) {
                        ep->txFreeHead = txd;
                     } else {
                        ep->txFreeTail->lLink = ( uint32 ) txd;
                        ep->txFreeTail->pLink = txd->physAddr;
                     }
                     ep->txFreeTail = (pEuphPhyterDesc) txdLast;

                     NsmLeaveCriticalSection( hsm, HSM_TX_CRITICAL_SECTION );
                     txsts = ( uint16 ) ((cmdsts >> 16) & 0x7ff);
                     NsmTransmitComplete( hsm, txHandle, (int) txsts );
                     txFirst = TRUE;
                  }
  #endif
              } /* end while */
      #endif  /* NECWARP */
          } /* end transmit events */


      #ifdef NECWARP
          if ( sts & ( RxOK | RxERR | RxORN | SWINT ) ) {         /* any receive events? */
             while( HsmServiceReceive( hsm ) );
      #else
          if ( sts & ( RxOK | RxERR | RxORN ) ) {         /* any receive events? */
             /* process receive events */
             rxd = ep->rxHead;
             NSM_GET32( cmdsts, rxd->cmdsts );
             if( cmdsts & OWN ) {
                traffic = TRUE;
#ifndef NSM_SUPPLIES_RX_BUFFERS
               /*********** for DRIVERS (not HSMMON) ***************/
               if( ep->rxLookState ) {
                   /* NsmRxLookahead is already been called.  We need
                   ** to call NsmRxComplete instead */
                   dsize = (uint16)(cmdsts & DSIZE);
                   dsize -= CRC_SIZE;
                   if( ep->rxLookState == RX_LOOK_PENDING ) {
                       ep->rxLookState = RX_LOOK_IDLE;
                       if( cmdsts & OK ) {
                           rxsts = 0;
                       } else {
                           rxsts = (uint16) ((cmdsts >> 18) & 0x1f);
                           if( !rxsts )        /* no error indicated? */
                               rxsts = HSM_RX_CRC_ERROR; /* then force one */
                       }
                       NsmRxComplete( hsm, (void *)rxd, dsize, rxsts );
                   } else {    /* rxLookState != RX_LOOK_PENDING */
                       ep->rxLookState = RX_LOOK_IDLE;
                   }
               } else {
                   dsize = (uint16)(cmdsts & DSIZE);
                   dsize -= CRC_SIZE;
                   if( cmdsts & OK ) {
                       /* packet is good, show it to the upper layers */
                       NsmRxLookahead( hsm, (void *)rxd, rxd->buf,
                           hsm->rxLookaheadSize, dsize,
#ifdef SIMULATE
                           /* in simulation, we want to check the status bits */
                           (cmdsts >> 18),
#else
                           /* for real drivers, just show no errors */
                           0,
#endif
                           NULL );
                   } else if(( hsm->rxMode & ACCEPT_ALL_ERRORS ) &&
                         ( cmdsts &(RUNT|TOOLONG|RXISERR|CRCERR|FAERR))) {
                       rxsts = (uint16) ((cmdsts >> 18)
#ifdef SIMULATE
                           /* in simulation, we want to check the status bits */
                           & 0x1f);
#else
                           );
#endif
                       if( !rxsts )            /* no error indicated? */
                           rxsts = HSM_RX_CRC_ERROR;   /* then force one */
                       NsmRxLookahead( hsm, (void *)rxd, rxd->buf,
                           hsm->rxLookaheadSize, dsize, rxsts, NULL );
                   }
               }

               /* give the descriptor (and buffer) back to the chip */
               rxd->cmdsts = RX_MAX_PACKET_SIZE | INCCRC;
               NsmEnterCriticalSection( hsm, HSM_RX_CRITICAL_SECTION );

               /* advance to the next descriptor */
               ep->rxHead = (pEuphPhyterDesc) rxd->lLink;
               rxd->lLink = 0;
               rxd->pLink = 0;
               if( !ep->rxHead ) {
                   ep->rxHead = rxd;
                   IOW32 (rxdp, ep->rxHead->physAddr );
               } else {
                   ep->rxTail->lLink = (uint32) rxd;
                   /* make this an autonomous 32-bit operation,
                   ** even on 16-bit compilers */
                   NSM_SET32( ep->rxTail->pLink, rxd->physAddr );
               }
               ep->rxTail = rxd;
               IOW32( cr, RxENA );

               NsmLeaveCriticalSection( hsm, HSM_RX_CRITICAL_SECTION );
#else
               /************* for HSMMON (not DRIVERS) ******************/
               rxd->cmdsts -= CRC_SIZE;
               if( cmdsts & OK ) {
                   /* packet is good, show it to the upper layers */
                   NsmReceive( hsm, (HsmPktDesc _FAR_ *)rxd, 0 );
               } else if(( hsm->rxMode & ACCEPT_ALL_ERRORS ) &&
                         ( cmdsts &(RUNT|TOOLONG|CRCERR|FAERR|OVERRUN))) {
                   rxsts = (cmdsts >> 18) & 0x1f;
                   if( !rxsts )
                       rxsts = HSM_RX_CRC_ERROR;
                   NsmReceive( hsm, (HsmPktDesc _FAR_ *)rxd, rxsts );
               } else {
                   NsmReturnRxBuffer( hsm, (HsmPktDesc _FAR_ *)rxd );
               }
               NsmGetRxBuffer( hsm, (HsmPktDesc _FAR_ *) rxd );
               rxd->cmdsts |= INCCRC;

               /* give the descriptor (and buffer) back to the chip */
               NsmEnterCriticalSection( hsm, HSM_RX_CRITICAL_SECTION );

               /* advance to the next descriptor */
               ep->rxHead = (pEuphPhyterDesc) rxd->lLink;
               rxd->lLink = 0;
               rxd->pLink = 0;
               if( !ep->rxHead ) {
                   ep->rxHead = rxd;
               } else {
                   ep->rxTail->lLink = (uint32) rxd;
                   /* make this an autonomous 32-bit operation,
                   ** even on 16-bit compilers */
                   NSM_SET32( ep->rxTail->pLink, rxd->physAddr );
               }
               ep->rxTail = rxd;
               IOW32( cr, RxENA );

               NsmLeaveCriticalSection( hsm, HSM_RX_CRITICAL_SECTION );
#endif
               /*************** for ALL *********************/
             }
      #endif  /* NECWARP */
  #ifndef NSM_SUPPLIES_RX_BUFFERS
          /***************** for DRIVERS (not HSMMON) *********************/
          } else if(( sts & RxEARLY )&&( rxd = ep->rxHead )) {
              ep->imrValue &= ~RxEARLY;
#if 0   /* disable Early RxLookAhead */
              /* process early receive events only if there were no other
              ** receive events */
              cmdsts = rxd->cmdsts;

              /* call it up as a lookahead, -1 for status and size */
              /* set a flag, so we can check in HsmRxCopyPkt, and return
              ** that it's not here yet */
              ep->rxLookState = RX_LOOK_PENDING;
              if( NsmRxLookahead( hsm, (void *)rxd, rxd->buf,
                  hsm->rxLookaheadSize, -1, 0, NULL ) == NsmRxPktDiscard )
                  ep->rxLookState = RX_LOOK_DISCARD;
                  /* Next Rx desc. handling will discard it & check its status. */
#endif
  #endif
          /********************* for ALL ***************************/
          }

       } while ( traffic && loopCount < 10 );
;       ep->isrValue |= IOR32( isr );
;       sts = ep->isrValue & ep->imrValue;
;       ep->isrValue &= !ep->imrValue;
       sts = 0;  /* inhibit loop */
    }
    return( HsmOK );
}
#endif

/*
**      HsmGetTxDescList( )     returns a list of available transmit
**                              descriptors or NULL if not enough are
**                              available.
**                              Transmit packet descriptors must be queued
**                              to HsmTransmit in the order in which they
**                              were acquired.
*/
#ifndef NECWARP
pHsmPktDesc  HsmGetTxDescList(
    HsmContext *hsm,            /* pointer to hsm's context */
    int         nfrags          /* number of fragments needed */
) {
    pHsmPktDesc      txpd = NULL;
    pHsmPktDesc      txpdLast;
    EuphPhyterHsmContext        *ep = (EuphPhyterHsmContext *) hsm;
    uint16 ndesc = ( nfrags + HSM_MAX_TX_FRAGS - 1 ) / HSM_MAX_TX_FRAGS;

    NsmEnterCriticalSection( hsm, HSM_TX_CRITICAL_SECTION );
    if( ndesc <= hsm->txQFree ) {
        hsm->txQFree -= ndesc;
        txpd = ep->txDescHead;
        txpdLast = txpd;
        while( --ndesc ) {
            txpdLast->descByteCnt = 0;
            txpdLast = (pHsmPktDesc ) txpdLast->lLink;
        }
        ep->txDescHead = (pHsmPktDesc ) txpdLast->lLink;
        txpdLast->lLink = 0;
        txpdLast->descByteCnt = 0;
        txpdLast->pLink = 0;
    }
    NsmLeaveCriticalSection( hsm, HSM_TX_CRITICAL_SECTION );
    return( txpd );
}
#endif


/*      HsmTransmit( )          Queue the completed HsmPkt transmit descriptor
**                              for transmit.
*/
#ifndef NECWARP
int HsmTransmit(
    HsmContext *hsm,            /* pointer to hsm's context */
    pHsmPktDesc txpd      /* pointer to structure containing packet */
                                /* fragments */
) {
    EuphPhyterHsmContext        *ep = (EuphPhyterHsmContext *) hsm;
    pEuphPhyterDesc     txd;
    pEuphPhyterDesc     txdFirst;       /* save pointer to first */
    pHsmPktDesc         txpdSave = txpd;        /* save txpd initial value */
    pHsmFrag            frag;
    uint32              cmdsts;
    uint16              n, dsize;

    uint16              i=0;

    uint16              pktSize = 0;
    Puchar              pbuf;

    NsmEnterCriticalSection( hsm, HSM_TX_CRITICAL_SECTION );
#ifdef NSM_DOUBLE_BUFFER
    txd = ep->txFreeHead;
    txdFirst = txd;
    ep->txFreeHead = (pEuphPhyterDesc) txd->lLink;

    /* we will put all of the packet into the first descriptor,
    ** and then free up subsequent descriptors */
    txd->handle = (uint32) txpdSave;
    pbuf = txd->buf;

    while( 1 ) {
        cmdsts = txpd->descByteCnt;
        /* NSM wants to double buffer, and has supplied "logical"
        ** addresses of fragments, copy into physical buffer */
        dsize = (uint16) (cmdsts & DSIZE);
        frag = (pHsmFrag ) &txpd->frags[0];
        pktSize += dsize;
        while( dsize ) {
            n = min( dsize, (int)(frag->cnt) );
            NSM_MEMCPY( pbuf, (Puchar ) frag->fptr, n );
            dsize -= n;
            pbuf += n;
            ++frag;
        }

        if( !( cmdsts & MORE ))         /* last one? */
            break;                      /* yes, vamoose */
        txpd = (pHsmPktDesc )txpd->lLink;
    }

    txd->cmdsts = OWN | pktSize;
#else
   /*  NEED_TX_PHYSADDRS   make TxDesc Link, dont't copy data. */
    txdFirst = ep->txFreeHead;

    do {
       pktSize = (uint16) txpd->descByteCnt;
       frag = (pHsmFrag ) &txpd->frags[0];
//       for (n=0; ((n < HSM_MAX_TX_FRAGS) && (pktSize)); n++ ) {
       while( pktSize ) {
//          if (cmdsts = txpd->frags[n].cnt & DSIZE) {
          if (cmdsts = frag->cnt & DSIZE) {
//             i++;
             txd = ep->txFreeHead;
             ep->txFreeHead = (pEuphPhyterDesc) txd->lLink;

             txd->handle = (uint32) txpdSave;
//             txd->bufPhys = txpd->frags[n].fptr;
             txd->bufPhys = frag->fptr;
             pktSize -= (uint16) cmdsts;
             txd->cmdsts = cmdsts | OWN | MORE;
             ++frag;
          }
       }
    } while ( txpd = (pHsmPktDesc) txpd->lLink );

//   if ( i ) {
       txd->cmdsts &= ~MORE;
#endif
    txd->lLink = 0;             /* last one will terminate list */
    txd->pLink = 0;

    /* add descriptor list to the transmit list */
    if( !ep->txHead ) {         /* is list empty? */
        ep->txHead = txdFirst;  /* yes, start a new one */
        ep->txTail = txd;                   /* it's now the last */
        IOW32( txdp, txdFirst->physAddr );
        IOW32( cr, TxENA );             /* fire up transmit */
    } else {
        NSM_SET32( ep->txTail->pLink, txdFirst->physAddr );
        ep->txTail->lLink = (uint32) txdFirst;
        ep->txTail = txd;                   /* it's now the last */
        IOW32( cr, TxENA );             /* fire up transmit */
    }

#ifndef NSM_DOUBLE_BUFFER
//   }
#endif
    NsmLeaveCriticalSection( hsm, HSM_TX_CRITICAL_SECTION );
    return( HsmOK );
}
#endif    /* NECWARP */
#endif                  /* NSM_DATA_XFER */



/*      epSetMediaType( )       Set speed and duplex settings on the MAC
*/
static void epSetMediaType(
    HsmContext  *hsm,
    int         speed,
    int         duplex
) {
//    EuphPhyterHsmContext        *ep = (EuphPhyterHsmContext *) hsm;
    uint32      txCfgOn = TxATP | TxECRETRY, txCfgOff = 0x3f;
    uint32      rxCfgOn = 0, rxCfgOff = 0;
#ifdef HSM_DEBUG
    char        prbuf [80];
#endif

    hsm->MediaSpeed = speed;
    hsm->FullDuplexCapable = duplex;

    /* set speed dependent parameters */
    if( speed == HW_SPEED_100_MBPS ) {
        txCfgOn |= (TxDRNT_100 | TxHBI);     /* or in new drain and HBI */
    } else {
        txCfgOn |= TxDRNT_10;
    }

//    rxCfgOn  |= (ep->rxDrainThreshold | (uint32)MAX_DMA);
    rxCfgOn  |= ( ((64/8)<<1) | (uint32)MAX_DMA);
    rxCfgOff |= (RxMXDMA | RxDRNT);

    /* set duplex dependent parameters */
    if( duplex == FDX_CAPABLE_FULL_SELECTED ) {
        /* ignore carrier sense and heartbeat errors */
        txCfgOn |= (TxCSI | TxHBI);
        rxCfgOn |= RxATP;               /* accept transmit packets */
    } else {
        txCfgOff |= (TxCSI | TxHBI);
        rxCfgOff |= RxATP;
    }

    IOW32( txcfg, (IOR32( txcfg ) & ~txCfgOff) | txCfgOn );
    IOW32( rxcfg, (IOR32( rxcfg ) & ~rxCfgOff) | rxCfgOn );
    NsmHsmEvent( hsm, HsmMediaTypeChange );
}


/*
**      epTxInit( )             reset and reinitialize the transmitter
*/
static void epTxInit(
    HsmContext *hsm
) {
    EuphPhyterHsmContext        *ep = (EuphPhyterHsmContext *) hsm;
    pEuphPhyterDesc     txd;
    pHsmPktDesc         txpd;
    register int        i;
    int                 numDesc;
    int                 bufsLeft;
    uint32              txdPhys;
#ifdef  NSM_DOUBLE_BUFFER
    Puchar              buf;
    uint32              bufPhys;
#endif
    uint32              siliconRev;
    uint32              dword;

    /* initialize transmit descriptor ring */
    i = 0;
    bufsLeft = 0;
    txd = ep->txBase;
    txdPhys = ep->txBasePhys + FieldOffset(EuphPhyterDesc,pLink);

    hsm->txQFree = hsm->txQSize;

#ifdef NSM_DOUBLE_BUFFER
    numDesc = hsm->txQSize;             /* 1 frag to 1 descriptor correspondence for euphyter */
#else
    numDesc = hsm->txQSize * HSM_MAX_TX_FRAGS; /* 1 frag to multiple desc. */
#endif
    while( numDesc-- ) {
        txd->lLink = 0;
        txd->pLink = 0;
        txd->physAddr = txdPhys;
        txd->cmdsts = 0;
        txdPhys += sizeof(EuphPhyterDesc);

#ifdef NSM_DOUBLE_BUFFER
        if( !bufsLeft ) {
            buf = ep->txBufs[i];
            bufPhys = ep->txBufsPhys[i++];
            bufsLeft = ep->txBufsPerPage;
        }
        txd->buf = buf;
        txd->bufPhys = bufPhys;
        buf += TX_BUFSIZE;
        bufPhys += TX_BUFSIZE;
        --bufsLeft;
#endif
        if( !ep->txFreeHead ) {
            ep->txFreeHead = txd;               /* free list gets them all */
        } else {
            ep->txFreeTail->lLink = (uint32) txd;
            ep->txFreeTail->pLink = txd->physAddr;
        }
        ep->txFreeTail = txd;
        ++txd;
    }

    numDesc = hsm->txQSize;             /* 1 frag to 1 descriptor correspondence for euphyter */
//#ifndef NSM_DOUBLE_BUFFER
//    hsm->txQSize = hsm->txQSize * HSM_MAX_TX_FRAGS;
//#endif
    txpd = ep->txDescBase;
    while( numDesc-- ) {
        txpd->lLink = 0;
        if( !ep->txDescHead ) {
            ep->txDescHead = txpd;
        } else {
            ep->txDescTail->lLink = (uint32) txpd;
        }
        ep->txDescTail = txpd;
        ++txpd;
    }

    ep->txHead = NULL;          /* nothing on transmit list */
    ep->txTail = NULL;

    /* set transmit for auto transmit padding, 64 byte drain threshold
    ** 32 byte fill threshold, transmit retry count */
    IOW32( txcfg, TxATP | TX_FILL | TxDRNT_100 | MAX_DMA );

    epResetPMD ( hsm );
#if 0
    siliconRev = IOR32( srr );
    if ( siliconRev == 0x0200 ) {
       IOW32( pagesel, 0x0001 );
       IOW32( pmdcsr, 0x0802 );
       IOW32( fcoctl, 0x0010 );
       IOW32( sdcfg, 0x0333 );
       IOW32( pgmcgm, 0x0860 );
       IOW32( tmr, 0x2100 );
       IOW32( cdctl2, 0x4f48 );
       IOW32( pagesel, 0 );
       dword = IOR32( tenbt );
       IOW32( tenbt, dword | 0x04 );
    } else if ( ( siliconRev & 0xff00 ) == 0x0300 ) {
#endif
#if 0
       IOW32( pagesel, 0x0001 );
       IOW32( pmdcsr, 0x189c );
       IOW32( tdata, 0x0000 );
       IOW32( dspcfg, 0x5040 );
       IOW32( sdcfg, 0x008c );
       IOW32( pagesel, 0 );
#endif
#if 0
    }
#endif
}

/*
**      epRxInit( )             reset and reinitialize the receiver
*/
#ifndef  NECWARP
static void epRxInit(
#else
void     epRxInit(
#endif
    HsmContext *hsm
) {
    EuphPhyterHsmContext        *ep = (EuphPhyterHsmContext *)hsm;
    pEuphPhyterDesc             rxd;
    pEuphPhyterDesc             rxdLast;
    register int        i;
    int                 numDesc, j;
    int                 bufsLeft;
    Puchar              buf;
    uint32              bufPhys, rxdPhys;

    /* ==== Receive Initialization ==== */
    /* set receive, 64 byte drain threshold
    ** 1 fragment per descriptor */
    ep->rxLookState = RX_LOOK_IDLE;

//    IOW32( rxcfg, ep->rxDrainThreshold |
//        ((uint32)MAX_DMA ) );

    /* -- initialize receive descriptor ring -- */
    numDesc = hsm->rxQSize;
    i = 0;
    j = 0;
    bufsLeft = 0;
    rxdLast = NULL;
    rxd = ep->rxBase;
    rxdPhys = ep->rxBasePhys + FieldOffset(EuphPhyterDesc,pLink);

    while( numDesc-- ) {
        if( !bufsLeft ) {
            buf = ep->rxBufs[i];
            bufPhys = ep->rxBufsPhys[i++];
            bufsLeft = ep->rxBufsPerPage;
        }
        rxd->lLink = 0;
        rxd->pLink = 0;
        rxd->physAddr = rxdPhys;
        rxd->cmdsts = RX_MAX_PACKET_SIZE | INCCRC;
        rxd->handle = (uint32) j;
#ifdef NSM_SUPPLIES_RX_BUFFERS
        NsmGetRxBuffer( hsm, (HsmPktDesc _FAR_ *)rxd );
        rxd->cmdsts |= INCCRC;
#else
        rxd->buf = buf;
        rxd->bufPhys = bufPhys;
#endif
        if( rxdLast ) {
            rxdLast->lLink = (uint32) rxd;
            rxdLast->pLink = rxd->physAddr;
        } else {
            ep->rxHead = rxd;
        }
        rxdLast = rxd;
        ++rxd;
        rxdPhys += sizeof(EuphPhyterDesc);
        buf += RX_BUFSIZE;
        bufPhys += RX_BUFSIZE;
        ++j;
        --bufsLeft;
    }
    ep->rxTail = --rxd;

    /* tell the chip about the start of the list,
    ** and start the receiver */
    IOW32( rxdp, ep->rxHead->physAddr );
}
//
//
///* ---- receive filter access ---- */
//
//static void epSetReceiveFilter(
//    HsmContext  *hsm            /* pointer to the HsmContext */
//) {
//
//    IOW32( rfcr, RFEN |
//         ( (uint32)(hsm->rxMode&(ACCEPT_ALL_MCASTS|ACCEPT_ALL_BCASTS|ACCEPT_ALL_PHYS))
//           << RFAA_shift ));
//
//}
//
//
///*
//**      epRxReset( )            reset the receiver
//**
//**      To keep from resetting during a bus transaction, we must
//**      first disable the receiver and wait for the receive to go idle.
//*/
//static void epRxReset(
//    HsmContext *hsm
//) {
//    EuphPhyterHsmContext        *ep = (EuphPhyterHsmContext *) hsm;
//    int                 i = 0;
//
//    ep->rxHead = NULL;  /* don't process anymore rx pkts */
//
//    /* we can't set an RxIDLE interrupt for this, at least not
//    ** in revA, so we'll need to poll it */
//    if( ep->rxResetPending )
//        return;
//
//    ep->rxResetPending = TRUE;
//
//    if( IOR32( cr ) & RxENA ) { /* is it enabled? */
//        IOW32( cr, RxDIS );
//        while( IOR32( cr ) & RxENA ) {
//            epWaitTime( 1 );
//            if( ++i == 50 )
//                break;
//        }
//    }
//
//    ep->imrValue |= RXRCMP;
//    ep->imrValue &= ~(RxOK | RxERR | RxORN | RxSOVR );
//    IOW32( imr, ep->imrValue );
//    IOW32( cr, RxRESET );
//
//
//}

/*
**      epFreeBuffers( )        free buffers previously allocated
*/
static void epFreeBuffers(
    HsmContext *hsm             /* pointer to hsm's context */
) {
    int                 i;
    EuphPhyterHsmContext        *ep = (EuphPhyterHsmContext *)hsm;

    /* free descriptors */
    NsmFreePhys( hsm, ep->txBase );
    ep->txBase = NULL;
    NsmFreePhys( hsm, ep->rxBase );
    ep->rxBase = NULL;
    NsmFree( hsm, ep->txDescBase );
    ep->txDescBase = NULL;

    /* and then buffers */
    for( i=0; i<MAX_TX_QSIZE; i++ ) {
        if( ep->txBufs[i] ) {
            NsmFreePhys( hsm, ep->txBufs[i] );
            ep->txBufs[i] = NULL;
        } else
            break;
    }

    for( i=0; i<MAX_RX_QSIZE; i++ ) {
        if( ep->rxBufs[i] ) {
            NsmFreePhys( hsm, ep->rxBufs[i] );
            ep->rxBufs[i] = NULL;
        } else
            break;
    }
}


/*
**      epComputeHashTableIndex( uchar _FAR_ *addr )
**              - compute which bit to set in the hash table
*/
#ifndef NECWARP  /* MS-C 6.0 fails to compute CRC32 */
static int epComputeHashTableIndex( Puchar  addr )
{
#define POLYNOMIAL 0x04C11DB6L
    uint32      crc = 0xffffffff, msb;
    int         i, j;
    uchar       byte;

    for( i=0; i<6; i++ ) {
        byte = *addr++;
        for( j=0; j<8; j++ ) {
            msb = crc >> 31;
            crc <<= 1;
            if( msb ^ ( byte & 1 )) {
                crc ^= POLYNOMIAL;
                crc |= 1;
            }
            byte >>= 1;
        }
    }
    return( (int)(crc >> 23) );
}
#endif

/*      HsmMulticastLoad( )     load a list of multicast addresses
*/
#ifndef NECWARP
int HsmMulticastLoad(
    HsmContext *hsm                     /* pointer to hsm's context */
) {
    FPHsmMulticastTableEntry  entry = hsm->MulticastTable;
    int                 nAddresses = hsm->MulticastTableSize;
    int                 i, bitNum;
    uint32              rfcrSave;
    EuphPhyterHsmContext        *ep = (EuphPhyterHsmContext *)hsm;

    NsmEnterCriticalSection( hsm, HSM_ALL_CRITICAL_SECTION );

    rfcrSave = IOR32( rfcr );
    for ( i = 0; i < 32; i++ ) {
       ep->hashTable[ i ] = 0;
    }

    for( i=0; i<nAddresses; i++ ) {
        if( entry->useCount ) {
            bitNum = epComputeHashTableIndex( entry->macAddr.maddr.bytes );
            ep->hashTable[bitNum>>4] |= (1 << (bitNum & 0x0f));
        }
        ++entry;
    }
    for( i=0; i<32; i++ ) {
        IOW32( rfcr, 0x200 + (i * 2) );  /* select hash table entry */
        IOW32( rfdr, (uint32)ep->hashTable[i] );         /* load new values */
    }
    IOW32( rfcr, rfcrSave );

    NsmLeaveCriticalSection( hsm, HSM_ALL_CRITICAL_SECTION );

    return( HsmOK );
}
#endif

/*      HsmSetMacAddr( )        set the NIC to receive packets addressed to
**                              a particular physical MAC address
*/
int HsmSetMacAddr(
    HsmContext *hsm,            /* pointer to hsm's context */
    MacAddr *addr               /* new address to use */
) {
    int         i;
    uint32      rfcrSave;
    uint16      w;

    NsmEnterCriticalSection( hsm, HSM_ALL_CRITICAL_SECTION );
    MAC_ADDR_COPY( &hsm->CurrMacAddr, addr );
    rfcrSave = IOR32( rfcr );
    IOW32( rfcr, rfcrSave & ~RFEN );    /* disable the receive filter */
    for( i=0; i<3; i++ ) {
        IOW32( rfcr, ((uint32) i)<<1 );
        w = addr->maddr.words[i];
        IOW32( rfdr, (uint32) w );
    }
    IOW32( rfcr, rfcrSave );
    NsmLeaveCriticalSection( hsm, HSM_ALL_CRITICAL_SECTION );

    return( HsmOK );
}



/*      HsmSetRxMode( )         set the receive mode which enables/disables
**                              the reception of all-broadcast, all-multicast,
**                              or all-unicast, error, or RX-filtered packets
*/
int HsmSetRxMode(
    HsmContext *hsm,            /* pointer to hsm's context */
    int mode                    /* set to new receive mode, as specified */
                                /* in HsmContext.rcvMode. */
) {
    uint32      rxCfgOn = 0, rxCfgOff = 0;
    uint32      txCfgOn = 0, txCfgOff = 0;
    uint32      ReceiveFilter;

    hsm->rxMode = mode;

    /* Placed in for NDIS2, substitutes for epSetReceiveFilter */
    ReceiveFilter = RFEN;
    if( mode & ACCEPT_ALL_PHYS )
        ReceiveFilter = ReceiveFilter | RFAAU;
    if( mode & ACCEPT_ALL_MCASTS )
        ReceiveFilter = ReceiveFilter | RFAAM;
    if( mode & ACCEPT_ALL_BCASTS )
        ReceiveFilter = ReceiveFilter | RFAAB;
    if( mode & ACCEPT_CAM_QUALIFIED )
        ReceiveFilter = ReceiveFilter | MHEN | APM;
//        ReceiveFilter = ReceiveFilter | MHEN | APM | RFAAM;
    IOW32( rfcr, ReceiveFilter ); /* Writing to the Receive Filter
                                     Control Register */
#if 0
    if( mode & ACCEPT_ALL_ERRORS ) {
        rxCfgOn = RxAEP | RxARP | RxALP;
    } else {
        rxCfgOff = RxAEP | RxARP | RxALP;
    }
#endif
#if 0
    if( mode & MAC_LOOPBACK ) {
        rxCfgOn |= RxATP;
        txCfgOn |= TxMLB;
    } else {
        rxCfgOff |= RxATP;
        txCfgOff |= TxMLB;
    }
#endif
//    IOW32( rxcfg, (IOR32( rxcfg ) | rxCfgOn) & ~rxCfgOff );
//    IOW32( txcfg, (IOR32( txcfg ) | txCfgOn) & ~txCfgOff );


    return( HsmOK );
}


/*      epPollPMDs( )
*/
static void epPollPMDs(
    HsmContext *hsm
) {
    PMD                 *pmd;

    int                 speed, duplex;
    uint16              status;
    EuphPhyterHsmContext        *ep = (EuphPhyterHsmContext *) hsm;

    pmd = &ep->pmds[ep->currentPmd];

    if( hsm->LinkState == LINK_DOWN ) {
LookForLink:
        if( ep->mediaType == HSM_MEDIA_AUTO_CONFIG )
            status = epLookForLink( hsm );
        else
            status = epMIIread( hsm, pmd->address, MII_STATUS );

        if( status & MIISTAT_LINK ) {
            /* see if mode has changed */
            epPMDreadMode( hsm, &speed, &duplex );
            if( ( speed != hsm->MediaSpeed )||
                ( duplex != hsm->FullDuplexCapable ) ) {
                epSetMediaType( hsm, speed, duplex );
            }

            hsm->LinkState = LINK_UP;
            NsmHsmEvent( hsm, HsmCarrierPresent );
        }
    } else {
        status = epMIIread( hsm, pmd->address, MII_STATUS );

        if( !( status & MIISTAT_LINK ) ) {              /* no link */
            if( hsm->LinkState == LINK_UP ) {           /* is this news? */
                hsm->LinkState = LINK_DOWN;             /* yes! */
                NsmHsmEvent( hsm, HsmCarrierLoss );
                goto LookForLink;
            }
        }
    }
    hsm->MIIstatusReg = status; /* nobody will ever see this */
}
//
///*
//**      epWaitTime( )           waste some time ...
//*/
//static void epWaitTime( int n )
//{
//    int i;
//    while( n-- )
//        for( i=0; i<2000; i++ )
//            (void) NsmIOread8( 0x61 );
//}

/****** MII management functions *******/


/*      epMIIidle( )    drive a clock with the MII bus idle */

static void epMIIidle( HsmContext *hsm )
{
    IOW32( mear, MDIO | MDDIR );
    IOW32( mear, MDIO | MDDIR | MDC );
}


/*      epMIIput( value, nbits )        write out the given number of bits */

static void epMIIput( HsmContext *hsm, uint32 value, int nbits )
{
    uint32      mask = ((uint32)1) << (nbits-1);
    uint32      l;

    while( nbits-- ) {
        /* assert chip select, and setup data in */
        l = ( value & mask ) ? MDDIR | MDIO : MDDIR;
        IOW32( mear, l );
        IOW32( mear, l | MDC );
        mask >>= 1;
    }
}

/*      epMIIget( nbits )               read in the given number of bits */

static uint32 epMIIget( HsmContext *hsm, int nbits )
{
    uint32      mask = ((uint32)1) << (nbits-1);
    uint32      value = 0;

    while( nbits-- ) {
        /* assert chip select, and clock in data */
        IOW32( mear, 0 );
        IOW32( mear, MDC );
        if( IOR32( mear ) & MDIO )
                value |= mask;
        mask >>= 1;
    }
    return( value );
}


/*      epMIIwrite(offset,value)        write out a value to a location */

static void epMIIwrite( HsmContext *hsm, int pmd, int offset, uint16 value )
{
    uint32      l;

    epMIIidle( hsm );
    l = MIIwrite | (pmd<<MIIpmdShift) | (offset<<MIIregShift);
    l <<= MIIcmdShift;
    epMIIput( hsm, l | value, MIIwrLen );
    epMIIidle( hsm );
}

/*      epMIIread(offset)               read in a value from a given offset */

static uint16 epMIIread( HsmContext *hsm, int pmd, int offset )
{
    uint32      l;

    epMIIidle( hsm );
    l = MIIread | (pmd<<MIIpmdShift) | (offset<<MIIregShift);
    l >>= 2;                            /* remove turnaround bits */
    epMIIput( hsm, l, MIIcmdLen - 2 );
    (void) epMIIget( hsm, 1 );          /* turnaround */
    l = (uint16) epMIIget( hsm, 16 );
    (void) epMIIget( hsm, 1 );          /* turnaround */
    return( l );
}

/*      epMIIreset( )           reset the MII management bus */

static void epMIIreset( HsmContext *hsm )
{
    epMIIput( hsm, 0xffffffff, 32 );
}


/*
**      epMIIpollBit( ) wait for a bit to come true (or false)
*/
static bool epMIIpollBit(
    HsmContext  *hsm,           /* ptr to driver context */
    int         pmdaddress,     /* MII pmd address to read */
    int         offset,         /* register offset in PMD */
    uint16      mask,           /* bit(s) to wait for */
    bool        polarity,       /* true if waiting for 1, else 0 */
    Puint16     value    /* where to put value */
) {
    uint32      i;

    i = 0;
    while( 1 ) {
        *value = epMIIread( hsm, pmdaddress, offset );
        if( polarity ) {
            if( mask & *value )
                return( TRUE );
        } else {
            if( mask & ~(*value) )
                return( TRUE );
        }
        if( ++i == 120000 )
            break;
    }
    return( FALSE );
}


/*      epPMDreadMode( )        read the current speed and duplex settings
**                              from the currently selected PMD
*/
static void epPMDreadMode(
    HsmContext          *hsm,   /* ptr to driver context */
    Puint16             speed, /* where to put speed setting */
    Puint16             duplex /* where to put duplex setting */
) {
    EuphPhyterHsmContext        *ep = (EuphPhyterHsmContext *)hsm;
    uint16              status;
    PMD                 *pmd = &ep->pmds[ep->currentPmd];

    *speed = HW_SPEED_10_MBPS;
    *duplex = FDX_CAPABLE_HALF_SELECTED;

    status = epMIIread( hsm, pmd->address, MII_ANLPAR );
    status &= epMIIread( hsm, pmd->address, MII_ANAR );

#if 0
    if( !( status &
        ( MII_NWAY_T|MII_NWAY_T_FDX|MII_NWAY_TX|MII_NWAY_TX_FDX )) ) {
        if( isPlexus( pmd ) ) {
            /* plexus rev C should report this is in the NWAY regs but the
            ** metal fix could only put it here */
            status = epMIIread(hsm, pmd->address, 25);
            if( status & 0x40 )
                status = MII_NWAY_T;   /* set is 10 Mb */
            else
                status = MII_NWAY_TX;  /* clear is 100 Mb */
        }
    }
#endif
    /* The PHY will pick the highest capability that is
    ** returned in the ANLPAR */
#if 0
    if( status &( MII_NWAY_TX_FDX | MII_NWAY_T_FDX )) {
        *duplex = FDX_CAPABLE_FULL_SELECTED;
    }
    if( status &( MII_NWAY_TX_FDX | MII_NWAY_TX )) {
        *speed = HW_SPEED_100_MBPS;
    }
#endif
    if( status &( MII_NWAY_TX_FDX | MII_NWAY_TX )) {
        *speed = HW_SPEED_100_MBPS;
        if( status & MII_NWAY_TX_FDX )
            *duplex = FDX_CAPABLE_FULL_SELECTED;
    } else {
        if( status & MII_NWAY_T_FDX )
            *duplex = FDX_CAPABLE_FULL_SELECTED;
    }
}

/*
**      epLookForLink( )        Scan all PMDs until we find one with
**                              LINK or we hit the last one.  Return
**                              his status.
*/
static uint16 epLookForLink(
    HsmContext *hsm
) {
    EuphPhyterHsmContext        *ep = (EuphPhyterHsmContext *) hsm;
    PMD                 *pmd = ep->pmds;
    int                 i;
    uint16              status;

    /* we need to AUTO across whatever PMDs this card has attached
    ** scan until we find a link, or just give up after one pass */
    for( i=0; i<ep->numPmds; i++, pmd++ ) {
        if( !pmd->dead &&( pmd->mediaTypes & HSM_MEDIA_AUTO_CONFIG )) {
            ep->currentPmd = i;

            status = epMIIread( hsm, pmd->address, MII_STATUS );
            if( status & MIISTAT_LINK ) {
                epIsolateOthers( hsm, pmd );
                break;
            } else if( pmd->address == 31 ) {
                /* take the 10Mb out of isolation,
                ** this effectively defaults to 10Mb (when we have it) */
                epMIIwrite( hsm, pmd->address, MII_CONTROL, MIICNTL_AUTO );
            }
        }
    }
    return( status );
}

/*
**      epIsolateOthers( )      Isolate other PMDs
*/
static void epIsolateOthers( HsmContext *hsm, PMD *pmdSelected )
{
    int         i;
    PMD         *pmd;
    EuphPhyterHsmContext *ep = (EuphPhyterHsmContext *)hsm;
    bool        extMIIpresent = FALSE;

    /* then, isolate all other PMDs */
    for( i=0, pmd = ep->pmds; i<ep->numPmds; i++, pmd++ ) {
        if( pmd->address == 0 && !pmd->dead ) {
            extMIIpresent = TRUE;
        }
        /* isolate him, but don't ISOLATE plexus if there is no
        ** external MII present (don't need to, and it screws up
        ** the detection of LINK status */
        if( pmd != pmdSelected &&
          !( pmd->address == 1 && !extMIIpresent )) {
            epMIIwrite( hsm, pmd->address, MII_CONTROL,
                MIICNTL_AUTO | MIICNTL_ISOLATE );
        }
    }
}


/*      isRevBPlexus    Return TRUE is pmd is a rev B plexus
*/
static bool isPlexus( PMD *pmd )
{
    return( ( pmd->id0 == MII_ID0_NSC )&&
            ( pmd->id1 == ( MII_ID1_NSC | MII_DP83840 ) ) );
}

/* res_code_end */


/* make code Init/removable for Win95 */
#ifdef NDIS_WIN
  #pragma ICODE
#endif


/* init_code_begin */

/*      HsmInitContext( )       Initialize an Hsm Context structure     */

int HsmInitContext(
    HsmContext *hsm             /* pointer to hsm's context */
) {
    EuphPhyterHsmContext        *ep = (EuphPhyterHsmContext *) hsm;

    hsm->hsmIFversion   = HSM_IF_VERSION;
    hsm->hsmOptions     =
#ifndef NSM_DOUBLE_BUFFER
        NEED_TX_PHYSADDRS |     /* want phys address in tx descriptors */
#endif
        DOES_PROMISCUOUS |      /* can do promiscuous mode */
        DOES_RX_PIPELINING |    /* can do rx pipelining */
        DOES_RX_MULTICOPIES |   /* can copy rx pkts multiple times */
        CAN_SHARE_IRQ |
        DOES_BUS_MASTERING |    /* i'm a bus master */
        NEED_TIMER;             /* need a timer */
    hsm->txQSize        = DEF_TX_QSIZE;
    hsm->rxQSize        = DEF_RX_QSIZE;
    hsm->hsmState       = HSM_UNINITIALIZED_STATE;
    hsm->maxPacketSize  = MAX_PACKET_SIZE;
    hsm->rxLookaheadSize = MAX_PACKET_SIZE;
    hsm->pnpId[0]       = ((uint32)PCI_NSC_VENDOR_ID << 16) +
                                (PCI_EUPHYTER_DEVICE_ID);

    /* for now, show all the possibilities */
    hsm->mediaTypes     = ( HSM_MEDIA_AUTO_CONFIG |
                            HSM_MEDIA_10BASET | HSM_MEDIA_10BASETfx |
                            HSM_MEDIA_100BASEX | HSM_MEDIA_100BASEXfx |
                            HSM_MEDIA_EXT_MII | HSM_MEDIA_AUTO_TP );

    hsm->FullDuplexCapable = FDX_CAPABLE_DUPLEX_UNKNOWN;
    hsm->MediaSpeed     = HW_SPEED_10_MBPS;
    hsm->MediaType      = HW_CABLE_UTP_CAT3;
    hsm->IOaddrSize     = 256;          /* 256 bytes IO space */
    hsm->ROMaddrSize    = 0;            /* no ROM space */
    hsm->MEMaddrSize    = 4096;         /* 4096 bytes MEMORY space */
//    hsm->Irq            = 0xff;
    hsm->LinkState      = 0;

    /* zero out all of my private data */
    NSM_MEMZERO( &ep->txHead,
        sizeof(EuphPhyterHsmContext) - sizeof(HsmContext) );

    return( HsmOK );
}


/*      HsmValidateContext( )   validate runtime configurable parameters */

int HsmValidateContext(
    HsmContext *hsm             /* pointer to hsm's context */
) {
    EuphPhyterHsmContext        *ep = (EuphPhyterHsmContext *) hsm;
    Puchar              p;
    int                 i, j, nBufs;
    uint16              maxMallocSize;

    /* validate queue sizes */
    if(( hsm->reqTxQSize > MAX_TX_QSIZE )||
       ( hsm->reqTxQSize &&( hsm->reqTxQSize < MIN_TX_QSIZE ))||
       ( hsm->reqRxQSize > MAX_RX_QSIZE )||
       ( hsm->reqRxQSize &&( hsm->reqRxQSize < MIN_RX_QSIZE )))
        return( HsmInvalidParameter );

    if( hsm->reqTxQSize )
        hsm->txQSize = hsm->reqTxQSize;

    if( hsm->reqRxQSize )
        hsm->rxQSize = hsm->reqRxQSize;

    /* figure how much we can malloc at a time */
#ifdef  OS2
    maxMallocSize = ( hsm->pageSize ) ? hsm->pageSize : -1;
#else
    maxMallocSize = ( hsm->pageSize ) ? hsm->pageSize : (32 * 1024);
#endif

    /* if smaller than our descriptor/buffer size, then forget it */
    if( ( maxMallocSize < sizeof(EuphPhyterDesc) ) ||
        ( maxMallocSize < TX_BUFSIZE ) )
        return( HsmOutOfMemory );

    /* allocate transmit descriptors */
#ifdef  NSM_DOUBLE_BUFFER
    i = hsm->txQSize;
#else
    i = hsm->txQSize * HSM_MAX_TX_FRAGS;
#endif
    ep->txBase = (pEuphPhyterDesc)
        NsmMallocPhys( hsm, i * sizeof(EuphPhyterDesc),
        &ep->txBasePhys );
    if( !ep->txBase )
        return( HsmOutOfMemory );

    /* allocate logical transmit descriptors */
//    ep->txDescBase = (pHsmPktDesc )
//       NsmMalloc( hsm, hsm->txQSize * sizeof(HsmPktDesc) );
    i= hsm->txQSize;
    ep->txDescBase = (pHsmPktDesc )
        NsmMalloc( hsm, i * sizeof(HsmPktDesc) );
    if( !ep->txDescBase ) {
        epFreeBuffers( hsm );
        return( HsmOutOfMemory );
    }

    /* allocate receive descriptors */
    ep->rxBase = (pEuphPhyterDesc)
        NsmMallocPhys( hsm, hsm->rxQSize * sizeof(EuphPhyterDesc),
        &ep->rxBasePhys );
    if( !ep->rxBase ) {
        epFreeBuffers( hsm );
        return( HsmOutOfMemory );
    }

    /* allocate transmit buffers */
#ifdef  NSM_DOUBLE_BUFFER
    i = 0;
    nBufs = hsm->txQSize;
    ep->txBufsPerPage = maxMallocSize / TX_BUFSIZE;
    while( nBufs ) {
        j = min( nBufs, ep->txBufsPerPage );
        p = (Puchar )
            NsmMallocPhys( hsm, j * TX_BUFSIZE, &ep->txBufsPhys[i] );
        if( !p ) {
            epFreeBuffers( hsm );
            return( HsmOutOfMemory );
        }
        ep->txBufs[i] = p;
        nBufs -= j;
        ++i;
    }
#endif

    /* allocate receive buffers */
#ifndef NSM_SUPPLIES_RX_BUFFERS
    i = 0;
    nBufs = hsm->rxQSize;
    ep->rxBufsPerPage = maxMallocSize / RX_BUFSIZE;
    while( nBufs ) {
        j = min( nBufs, ep->rxBufsPerPage );
        p = (Puchar )
            NsmMallocPhys( hsm, j * RX_BUFSIZE,
                &ep->rxBufsPhys[i] );
        if( !p ) {
            epFreeBuffers( hsm );
            return( HsmOutOfMemory );
        }
        ep->rxBufs[i] = p;
        nBufs -= j;
        ++i;
    }
#endif

    return( HsmOK );
}

/*      HsmFindAdapter( )       returns the configuration of the specified
**                              NIC, including what (if any) options are
**                              supported.  The config is returned in the
**                              appropriate fields of the HsmContext described
**                              above. This may return a value that indicates
**                              that more configuration is needed (e.g.
**                              nonPnP system, and no IOaddress is specified).
*/
int HsmFindAdapter(
    HsmContext *hsm             /* pointer to hsm's context     */
) {
    return( HsmNotFound );
}


/*
**      HsmCheckNic( )          verify that a NIC is available given
**                              the current information (IOaddr, slot, etc.)
**                              in the HsmContext.  This is called after
**                              the Nsm locates the appropriate adapter
**                              but before calling HsmInitialize.
*/

int HsmCheckNic(
    HsmContext *hsm             /* pointer to hsm's context */
) {
    uint32              v;

    if (!hsm->IOaddr)           /* if IOaddr is not set */
        return (HsmNotFound);

    /* read SRR and verify its value */
     v = IOR32( srr );
   /* if( v != 0x00000100 )
        return( HsmNotFound ); */

    return( HsmOK );
}

/*      HsmInitialize( )        prepare a NIC for use   */

int HsmInitialize(
    HsmContext *hsm
) {
    EuphPhyterHsmContext        *ep = (EuphPhyterHsmContext *) hsm;
    register int        i;
    uint32              status;

    //
    // Acknowledge any outstanding PME Status and disable PME.
    //
    IOW32( ccsr, PMESTS );

    /* reset the chip, while we initialize data structures */
    IOW32( ier, 0 );
    IOW32( cr, RxRESET | TxRESET | RESET );

    /* wait for reset complete */
    while ( ( IOR32( cr ) & RESET ) && i++ < 30000 ) {
    }

    if( i == 30000 )
        return( epFailure( hsm ) );

    ep->rxDrainThreshold = 8 << 1; /* 64 bytes is the default */

    /* disable SERR assertion on data perr's */
    /* disable use of MemoryWriteAndInvalidate PCI cmd */
    IOW32( cfg, PESEL );

#ifndef SIMULATE

    /* copy config data out of the EEPROM */

    if( epEEload( hsm ) )
        return( epFailure( hsm ) );

    /* parse into PMD array */
    if( epParseDeviceID( hsm ) )
        return( epFailure( hsm ) );

    /* scan PMDs and build array */
    epScanPMDs( hsm );
#endif

    //
    // Read the Permanent MAC Address from the EEPROM.
    //
    epReadMACAddress( hsm );

    return( HsmOK );
}


/* make code Locked/Resident for Win95 */
#ifdef NDIS_WIN
  #pragma LCODE
#endif


/*      HsmSetMediaType( )      set the Hsm's media type selection */

int HsmSetMediaType(
    HsmContext *hsm,            /* pointer to hsm's context */
    uint32 mediaType            /* new media type to select, this must */
                                /* be set to a single bit as specified in */
                                /* HsmContext.medias. */
) {
    EuphPhyterHsmContext        *ep = (EuphPhyterHsmContext *)hsm;
    PMD                 *pmd, *pmdFound = NULL;
    int                 i;
    int                 rc = HsmOK;
    int                 speed, duplex;
    uint16              cmd, status;
#ifdef HSM_DEBUG
    char                prbuf [80];
#endif

    /* is this valid for us? */
    if( !( mediaType & hsm->mediaTypes ) )
        return( HsmInvConfig );

    speed = HW_SPEED_10_MBPS;
    duplex = FDX_CAPABLE_HALF_SELECTED;
    pmd = ep->pmds;
    ep->mediaType = mediaType;  /* save for epPollPMDs */

    if( mediaType == HSM_MEDIA_AUTO_CONFIG ) {
        /* we need to AUTO across whatever PMDs this card has attached
        ** scan until we find a link, or just give up after one pass */
        for( i=0; i<ep->numPmds; i++, pmd++ ) {
            if( !pmd->dead &&( pmd->mediaTypes & HSM_MEDIA_AUTO_CONFIG )) {
                ep->currentPmd = i;
                pmdFound = pmd;
                epIsolateOthers( hsm, pmd );
                status = epAutoNegotiate( hsm, pmd );
                if( status & MIISTAT_LINK )
                    break;
            }
        }
    } else {
        /* search for a PMD that supports this media type */
        for( i=0; i<ep->numPmds; i++, pmd++ ) {
            /* if not dead, and media type is supported */
            if( !pmd->dead &&( pmd->mediaTypes & mediaType )) {
                ep->currentPmd = i;
                pmdFound = pmd;
                break;
            }
        }
        /* if we found one ... */
        if( pmdFound ) {

            /* do a MII reset to make sure everyone is listening */
            epMIIreset( hsm );

            /* then, isolate all other PMDs */
            epIsolateOthers( hsm, pmdFound );

            pmd = pmdFound;

            /* set speed and duplex in target PMD */

            switch( mediaType ) {

            case HSM_MEDIA_EXT_MII:
            case HSM_MEDIA_AUTO_TP:
                status = epAutoNegotiate( hsm, pmd );
                break;

            case HSM_MEDIA_10AUI:
            case HSM_MEDIA_10BASE2:
                /* is it a National PHY? */
                if(( pmd->id0 == MII_ID0_NSC )&&
                      (( pmd->id1 & MII_ID1_OUI_LO ) == MII_ID1_NSC )) {

                    /* what kind of National PHY? */
                    switch( pmd->id1 & MII_ID1_MODEL ) {

                    default:
                        rc = HsmInvConfig;
                        break;
                    }
                } else {
                    rc = HsmInvConfig;
                }
                break;
            case HSM_MEDIA_10BASET:
            case HSM_MEDIA_10BASET_ALT:
                cmd = 0;
                break;
            case HSM_MEDIA_10BASETfx:
            case HSM_MEDIA_10BASETfx_ALT:
                cmd = MIICNTL_FDX;
                duplex = FDX_CAPABLE_FULL_SELECTED;
                break;
            case HSM_MEDIA_100BASEX:
                cmd = MIICNTL_SPEED;
                speed = HW_SPEED_100_MBPS;
                break;
            case HSM_MEDIA_100BASEXfx:
                cmd = MIICNTL_SPEED | MIICNTL_FDX;
                speed = HW_SPEED_100_MBPS;
                duplex = FDX_CAPABLE_FULL_SELECTED;
                break;
            default:
                rc = HsmInvConfig;
                break;
            }
        } else {
            /* didn't find a match */
            rc = HsmInvConfig;
        }
    }

    /* if no errors so far, then enforce this media type */
    if( rc == HsmOK ) {
        if( !( mediaType &
             (HSM_MEDIA_AUTO_CONFIG|HSM_MEDIA_EXT_MII|HSM_MEDIA_AUTO_TP)))
        {
            epMIIwrite( hsm, pmd->address, MII_CONTROL, cmd );
            epSetMediaType( hsm, speed, duplex );

            /* wait for link to complete */
            epMIIpollBit( hsm, pmd->address, MII_STATUS,
                MIISTAT_LINK, TRUE, &status );
        }

        NsmStartTimer( hsm, PMD_POLL_INTERVAL );

        if( status & MIISTAT_LINK ) {
            hsm->LinkState = LINK_UP;           /* link present */
            NsmHsmEvent( hsm, HsmCarrierPresent );
        } else {
            hsm->LinkState = LINK_DOWN;         /* no link */
            NsmHsmEvent( hsm, HsmCarrierLoss );
        }
    }
    return( rc );
}


/* make code Init/removable for Win95 */
#ifdef NDIS_WIN
  #pragma ICODE
#endif


/*      HsmBcastAddEntry( )     Add an entry to the Hsm's broadcast
**                              filter table.
*/
int HsmBcastAddEntry(
    HsmContext *hsm,            /* pointer to hsm's context */
    int type,                   /* frame format type */
    uint32 pattern              /* protocol ID pattern */
) {
    return( HsmNotImplemented );
}


/*      HsmBcastDelEntry( )     remove an entry from the broadcast filter table
*/

int HsmBcastDelEntry(
    HsmContext *hsm,            /* pointer to hsm's context */
    int type,                   /* frame format type    */
    uint32 pattern              /* protocol ID pattern  */
) {
    return( HsmNotImplemented );
}

/*      epFailure( )            some hardware failure has occurred
**                              change to Dead State and tell upper
**                              layer
*/
static int epFailure( HsmContext *hsm )
{
    hsm->hsmState = HSM_DEAD_STATE;
    NsmHsmEvent( hsm, HsmStateChange );

    return( HsmHWFailure );
}

static void epReadMACAddress( HsmContext *hsm )
{
   uint32 i;
   uint16 mask;
   uint16 word1 = 0;
   uint16 word2 = 0;
   uint16 word3 = 0;
   uint16 word4 = 0;
   uint16 nicAddress[ 3 ];

   //
   // Read 16 bit words 6 - 9 from the EEProm.  They contain the hardwares MAC
   // address in a rather cryptic format.
   //
   word1 = epEEreadw( hsm,
                      0x06 );
   word2 = epEEreadw( hsm,
                      0x07 );
   word3 = epEEreadw( hsm,
                      0x08 );
   word4 = epEEreadw( hsm,
                      0x09 );

   //
   // Decode the cryptic format into what we can use a word at a time.
   //
   nicAddress[ 0 ] = word1 & 1;
   nicAddress[ 1 ] = word2 & 1;
   nicAddress[ 2 ] = word3 & 1;

   i = 15;
   mask = 0x2;
   while ( i-- ) {
      if ( word2 & 0x8000 ) {
         nicAddress[ 0 ] |= mask;
      }
      word2 = word2 << 1;
      mask = mask << 1;
   }

   i = 15;
   mask = 0x2;
   while ( i-- ) {
      if ( word3 & 0x8000 ) {
         nicAddress[ 1 ] |= mask;
      }
      word3 = word3 << 1;
      mask = mask << 1;
   }

   i = 15;
   mask = 0x2;
   while ( i-- ) {
      if ( word4 & 0x8000 ) {
         nicAddress[ 2 ] |= mask;
      }
      word4 = word4 << 1;
      mask = mask << 1;
   }

   //
   // Copy the hardware MAC address to the adapter data area.
   //
   MAC_ADDR_COPY( &hsm->PermMacAddr, ( MacAddr * )&nicAddress );

   //
   // If the Current MAC Address has not been set, copy in our hardware address
   //
   if ( MAC_ADDR_IS_NULL( &hsm->CurrMacAddr ) ) {
      MAC_ADDR_COPY( &hsm->CurrMacAddr, &hsm->PermMacAddr );
   }
}

static void HSM_WAIT_US( HsmContext *hsm, uint16 microSecond )
{
   uint32 wake;
   wake = IOR32( wcsr );
   wake &= ( uint32 )microSecond;
   IOW32( wcsr, wake );
}

/*      epEEput( hsm, value, nbits )
**              - write out the given number of bits
*/
static void epEEput( HsmContext *hsm, uint32 value, int nbits )
{
    uint32      mask = ((uint32)1) << (nbits-1);
    uint32      l;

    while( nbits-- ) {
        /* assert chip select, and setup data in */
        l = ( value & mask ) ? EECS | EEDI : EECS;
        IOW32( mear, l );
        HSM_WAIT_US( hsm, 2 );              //RDG
        IOW32( mear, l | EECLK );
        HSM_WAIT_US( hsm, 2 );              //RDG
        mask >>= 1;
    }
}

/*      epEEget( hsm, nbits )
**              - read in the given number of bits
*/
static uint16 epEEget( HsmContext *hsm, int nbits )
{
    uint16      mask = 1 << (nbits-1);
    uint16      value = 0;

    while( nbits-- ) {
        /* assert chip select, and clock in data */
        IOW32( mear, EECS );
        HSM_WAIT_US( hsm, 2 );              //RDG
        IOW32( mear, EECS | EECLK );
        HSM_WAIT_US( hsm, 2 );              //RDG
        if( IOR32( mear ) & EEDO )
                value |= mask;
        mask >>= 1;
    }
    return( value );
}

#if 0
/*      epEEwritew( hsm, offset, value )
**              - write out a value to a location
*/
static void epEEwritew( HsmContext *hsm, int offset, uint16 value )
{
    int i;
    uint32 invalue = (uint32) value;

    IOW32( mear, 0 );           /* clock out CS low */
    HSM_WAIT_US( hsm, 2 );              //RDG
    IOW32( mear, EECLK );
    HSM_WAIT_US( hsm, 2 );              //RDG
    epEEput( hsm, ((uint32)( EEwrite|offset) << EEcmdShift ) | invalue, 25 );
    IOW32( mear, 0 );           /* terminate write  */
    HSM_WAIT_US( hsm, 2 );              //RDG

    /* wait for operation to complete */
    IOW32( mear, EECS );        /* assert CS */
    HSM_WAIT_US( hsm, 2 );              //RDG
    IOW32( mear, EECS | EECLK );
    HSM_WAIT_US( hsm, 2 );              //RDG
    for( i = 0; i < 10; i++ ) {
    NsmWaitTime( hsm, 1 );
      /*  NsmWaitTime( hsm, 1 ); */
        if( IOR32( mear ) & EEDO )
            break;
    }
    IOW32( mear, 0 );           /* clock out CS low */
    HSM_WAIT_US( hsm, 2 );              //RDG
    IOW32( mear, EECLK );
    HSM_WAIT_US( hsm, 2 );              //RDG
}
#endif

/*      epEEreadw( hsm, offset )
**              - read in a value from a given offset
*/
static uint16 epEEreadw( HsmContext *hsm, int offset )
{
    uint16      value;

    IOW32( mear, 0 );           /* clock out CS low */
    HSM_WAIT_US( hsm, 2 );              //RDG
    IOW32( mear, EECLK );
    HSM_WAIT_US( hsm, 2 );              //RDG
    epEEput( hsm, (uint32)(EEread|offset), 9 );
    value = epEEget( hsm, 16 );
    IOW32( mear, 0 );           /* clock out CS low */
    HSM_WAIT_US( hsm, 2 );              //RDG
    IOW32( mear, EECLK );
    HSM_WAIT_US( hsm, 2 );              //RDG
    return( value );
}


static int epEEread(
    HsmContext  *hsm,           /* pointer to the HsmContext */
    uint16      offset,         /* offset from start of EEPROM */
    Puint16     value,   /* where to put the value[s] read */
    uint16      len             /* how many uint16's to read */
) {
    while( len-- ) {
        *value++ = epEEreadw( hsm, offset++ );
    }
    return( HsmOK );
}


/* ---- write some number of 16-bite values to the EEPROM ---- */
#if 0
static int epEEwrite(
    HsmContext  *hsm,           /* pointer to the HsmContext */
    uint16      offset,         /* offset from start of EEPROM */
    uint16      _FAR_ *value,   /* where to get the value[s] to write */
    uint16      len             /* how many uint16's to write */
) {
    while( len-- ) {
        epEEwritew( hsm, offset++, *value++ );
    }
    return( HsmOK );
}

/* ---- update the eeprom checksums ---- */

static int epEEupdateChecksum(
    HsmContext  *hsm            /* pointer to the HsmContext */
) {
    int         i;
    uint32      value;

        /* INCOMPLETE */
    return( HsmOK );
}

/* ---- enable EEPROM write access ---- */

static int epEEwriteEnable(
    HsmContext  *hsm,           /* pointer to the HsmContext */
    bool        enable          /* TRUE to enable write access */
) {
    IOW32( mear, 0 );           /* clock out CS low */
    HSM_WAIT_US( hsm, 2 );              //RDG
    IOW32( mear, EECLK );
    HSM_WAIT_US( hsm, 2 );              //RDG
    epEEput( hsm, enable ? EEwriteEnable : EEwriteDisable, 9 );  /* SIS FIX */
    IOW32( mear, 0 );           /* clock out CS low */
    HSM_WAIT_US( hsm, 2 );              //RDG
    IOW32( mear, EECLK );
    HSM_WAIT_US( hsm, 2 );              //RDG
    return( HsmOK );
}
#endif

/* ---- load up EEPROM variables ---- */
static int epEEload(
    HsmContext  *hsm            /* pointer to the HsmContext */
) {
    uint16      temp=0;
    uint16	addr[3];
    uint32      mediaType;
    int		i, x;
    EuphMacAddr	transfer;

#ifdef HSM_DEBUG
    char        prbuf [80];
#endif

    EuphPhyterHsmContext *ep = (EuphPhyterHsmContext *)hsm;

//    epEEread( hsm, EuphPhyterEEPmatch0, &hsm->PermMacAddr.maddr.words[0],
//        sizeof(MacAddr) >> 1 );
//
//    MAC_ADDR_COPY( &hsm->CurrMacAddr, &hsm->PermMacAddr );
//
//    epEEread(hsm, EuphPhyterEECfgsID0, &ep->vendorID, 1);
//
//    epEEread(hsm, EuphPhyterEECfgsID1, &ep->deviceID, 1);


    ep->vendorID = 0x100b;       /* Quoc, for NDIS2 */
    ep->deviceID = 0x0020;

    return( HsmOK );
}

/*      epParseDeviceID( )      builds the PMD array based on the board's
**                              configuration (implied by the deviceID).
**                              The order in the pmds array is significant,
**                              since we'll search from the beginning for
**                              a specific media type
*/
static int epParseDeviceID(
    HsmContext  *hsm            /* pointer to the HsmContext */
) {
    EuphPhyterHsmContext *ep = (EuphPhyterHsmContext *)hsm;
    PMD         *pmd = ep->pmds;


    if( ep->vendorID != PCI_NSC_VENDOR_ID )
        return( HsmHWFailure );

    if( ep->deviceID != EUPHYTER )
        return( HsmHWFailure );

#if 0
    switch( ep->deviceID ) {
    case EUPHYTER:
#endif
        pmd->address = 0x1f;
        pmd->mediaTypes =
            HSM_MEDIA_AUTO_CONFIG |
            HSM_MEDIA_10BASET | HSM_MEDIA_10BASETfx |
            HSM_MEDIA_100BASEX | HSM_MEDIA_100BASEXfx;
        ep->numPmds = 1;
#if 0
        break;

    case SIS_7006_SVB:          /* RJ-45 10/100 Mb only, uses external Plexus */
        pmd->address = 1;       /* Plexus */
        pmd->mediaTypes =
            HSM_MEDIA_AUTO_CONFIG | HSM_MEDIA_AUTO_TP |
            HSM_MEDIA_10BASET | HSM_MEDIA_10BASETfx |
            HSM_MEDIA_100BASEX | HSM_MEDIA_100BASEXfx;
        ep->numPmds = 1;
        break;

    case NI7100_SVB:
        pmd->address = 0;       /* MII MAU */
        pmd->mediaTypes =
            HSM_MEDIA_AUTO_CONFIG | HSM_MEDIA_EXT_MII;
        ++pmd;
        pmd->address = 1;       /* Plexus */
        pmd->mediaTypes =
            HSM_MEDIA_AUTO_CONFIG | HSM_MEDIA_AUTO_TP |
            HSM_MEDIA_10BASET | HSM_MEDIA_10BASETfx |
            HSM_MEDIA_100BASEX | HSM_MEDIA_100BASEXfx;
        ++pmd;
        pmd->address = 0x1f;    /* Etempl */
        pmd->mediaTypes =
            HSM_MEDIA_AUTO_CONFIG | HSM_MEDIA_10BASET | HSM_MEDIA_10BASETfx |
            HSM_MEDIA_10BASET_ALT | HSM_MEDIA_10BASETfx_ALT | HSM_MEDIA_10AUI;
        ep->numPmds = 3;
        break;

    case NI7101_T:              /* 10BaseT only */
        pmd->address = 0x1f;
        pmd->mediaTypes =
            HSM_MEDIA_AUTO_CONFIG | HSM_MEDIA_AUTO_TP |
            HSM_MEDIA_10BASET | HSM_MEDIA_10BASETfx;
        ep->numPmds = 1;
        break;

    case NI7101_CTA:            /* 10BaseT, Coax, AUI */
        pmd->address = 0x1f;
        pmd->mediaTypes =
            HSM_MEDIA_AUTO_CONFIG | HSM_MEDIA_AUTO_TP |
            HSM_MEDIA_10BASET | HSM_MEDIA_10BASETfx |
            HSM_MEDIA_10BASE2 | HSM_MEDIA_10AUI;
        ep->numPmds = 1;
        break;

    case NI7101_TM:             /* 10BaseT, External MII */
        pmd->address = 0;       /* MII MAU */
        pmd->mediaTypes =
            HSM_MEDIA_AUTO_CONFIG | HSM_MEDIA_EXT_MII;
        ++pmd;
        pmd->address = 0x1f;
        pmd->mediaTypes =
            HSM_MEDIA_AUTO_CONFIG | HSM_MEDIA_AUTO_TP |
            HSM_MEDIA_AUTO_TP | HSM_MEDIA_10BASET | HSM_MEDIA_10BASETfx;
        ep->numPmds = 2;
        break;

    case NI7100_XA:             /* 10/100BaseT, AUI */
        pmd->address = 1;       /* Plexus */
        pmd->mediaTypes =
            HSM_MEDIA_AUTO_CONFIG | HSM_MEDIA_AUTO_TP |
            HSM_MEDIA_10BASET | HSM_MEDIA_10BASETfx |
            HSM_MEDIA_100BASEX | HSM_MEDIA_100BASEXfx;
        ++pmd;
        pmd->address = 0x1f;    /* Interal 10M Phy */
        pmd->mediaTypes =
            HSM_MEDIA_AUTO_CONFIG | HSM_MEDIA_10AUI;
        ep->numPmds = 2;
        break;

    case NI7100_X:              /* 10/100BaseT */
        pmd->address = 1;       /* Plexus */
        pmd->mediaTypes =
            HSM_MEDIA_AUTO_CONFIG | HSM_MEDIA_AUTO_TP |
            HSM_MEDIA_10BASET | HSM_MEDIA_10BASETfx |
            HSM_MEDIA_100BASEX | HSM_MEDIA_100BASEXfx;
        ++pmd;
        pmd->address = 0x1f;
        pmd->mediaTypes = 0;
        ep->numPmds = 2;
        break;

    case NI7100_XM:                     /* 10/100BaseT, MII */
        pmd->address = 0;       /* MII MAU */
        pmd->mediaTypes =
            HSM_MEDIA_AUTO_CONFIG | HSM_MEDIA_EXT_MII;
        ++pmd;
        pmd->address = 1;       /* Plexus */
        pmd->mediaTypes =
            HSM_MEDIA_AUTO_CONFIG | HSM_MEDIA_AUTO_TP |
            HSM_MEDIA_10BASET | HSM_MEDIA_10BASETfx |
            HSM_MEDIA_100BASEX | HSM_MEDIA_100BASEXfx;
        ++pmd;
        pmd->address = 0x1f;
        pmd->mediaTypes = 0;
        ep->numPmds = 3;
        break;

    case NI7100_CXA:                    /* 10/100BaseT, 10Base2, AUI */
        pmd->address = 1;       /* Plexus has 10/100BaseT */
        pmd->mediaTypes =
            HSM_MEDIA_AUTO_CONFIG | HSM_MEDIA_AUTO_TP |
            HSM_MEDIA_10BASET | HSM_MEDIA_10BASETfx |
            HSM_MEDIA_100BASEX | HSM_MEDIA_100BASEXfx;
        ++pmd;
        pmd->address = 0x1f;    /* Etempl has 10Base2 and AUI */
        pmd->mediaTypes =
            HSM_MEDIA_AUTO_CONFIG | HSM_MEDIA_10BASE2 | HSM_MEDIA_10AUI;
        ep->numPmds = 2;
        break;

    default:                    /* unknown device ID */
        return( HsmHWFailure );
    }
#endif
    return( HsmOK );
}

/*
**      epScanPMDs              scan for PMDs on the MII bus
*/
static void epScanPMDs( HsmContext *hsm )
{
    EuphPhyterHsmContext        *ep = (EuphPhyterHsmContext *) hsm;
    PMD                 *pmd;
    int                 i;

    /* scan PMD information */
    pmd = ep->pmds;
    for( i=0; i<ep->numPmds; i++, pmd++ ) {
        /* send out the reset stream */
        epMIIreset( hsm );


        /* read the ID0 to see if he is alive */
        /* treat all 0's or all 1's as a dead PMD */
        pmd->id0 = epMIIread( hsm, pmd->address, MII_PHY_ID0 );
        if( !pmd->id0 || pmd->id0 == 0xffff ) {
            pmd->dead = 1;
            continue;
        }
#if 0
        /* reset the PMD */
        epMIIwrite( hsm, pmd->address, MII_CONTROL, MIICNTL_RESET );

        /* wait for reset to complete */
        if( !epMIIpollBit( hsm, pmd->address,
                MII_CONTROL, MIICNTL_RESET, FALSE, &pmd->ctl ))
            continue;
#endif
        /* read PMD id register and see if PMD is even there */
        pmd->id0 = epMIIread( hsm, pmd->address, MII_PHY_ID0 );
        pmd->id1 = epMIIread( hsm, pmd->address, MII_PHY_ID1 );
        pmd->ctl = epMIIread( hsm, pmd->address, MII_STATUS );
        pmd->sts = epMIIread( hsm, pmd->address, MII_STATUS );
        pmd->anar = epMIIread( hsm, pmd->address, MII_ANAR );

        /* if this is PM 0 (external MII), then build up the media types */
        if( !pmd->address ) {
           if( pmd->anar & MII_NWAY_T4 )
                pmd->mediaTypes |= HSM_MEDIA_100BASETT4;
           if( pmd->anar & MII_NWAY_TX_FDX )
                pmd->mediaTypes |= HSM_MEDIA_100BASEXfx;
           if( pmd->anar & MII_NWAY_TX )
                pmd->mediaTypes |= HSM_MEDIA_100BASEX;
           if( pmd->anar & MII_NWAY_T_FDX )
                pmd->mediaTypes |= HSM_MEDIA_10BASETfx;
           if( pmd->anar & MII_NWAY_T )
                pmd->mediaTypes |= HSM_MEDIA_10BASET;
        }

    }
}

/*
**  epResetPMD()  reset internal PHY
*/
void epResetPMD( HsmContext *hsm )
{
    IOW32( 0x80, 0x8000 ); /* Reset */
    NsmWaitTime( hsm, 2048 ); /* 1500ms + a */

    switch ( ((uint16)IOR32( srr )) >> 8 ) {
    case 0x02:
       IOW32( 0xCC, 0x0001 );
       IOW32( 0xF4, 0x8002 );
       IOW32( 0xD0, 0x0010 );
       IOW32( 0xF8, 0x0333 );
       IOW32( 0xE8, 0x0860 );
       IOW32( 0xD4, 0x2100 );
       IOW32( 0xE0, 0x4F48 );
       IOW32( 0xCC, 0x0000 );
       IOW32( 0xE8, (IOR32( 0xE8 ) | 0x0004 ));
       break;
    case 0x03:
       IOW32( 0xCC, 0x0001 );
       IOW32( 0xE4, 0x189c );
       IOW32( 0xFC, 0x0000 );
       IOW32( 0xF4, 0x5040 );
       IOW32( 0xF8, 0x008c );
       IOW32( 0xCC, 0x0000 );
       break;
    case 0x04:
    case 0x05:
       IOW32( 0xCC, 0x0001 );
       IOW32( 0xE4, 0x189c );
       IOW32( 0xCC, 0x0000 );
       break;
    default:
       break;
    }
       IOW32( 0x90, IOR32( 0x90 ) | 0x01E0 ); /* anar */
}

/*
**      epAutoNegotiate( )      Perform MII autonegotiation,
**                              return MII status following auto negotiation
*/
static uint16 epAutoNegotiate( HsmContext *hsm, PMD *pmd )
{
    uint16      status;
    int         speed, duplex;

    /* (re)start auto-negotiation */
    epMIIreset( hsm );
    epMIIwrite( hsm, pmd->address, MII_CONTROL, 0 );
    epMIIwrite( hsm, pmd->address, MII_CONTROL,
        MIICNTL_AUTO|MIICNTL_RST_AUTO );

    /* wait for auto-negotiation to start */
    epMIIpollBit( hsm, pmd->address, MII_CONTROL,
        MIICNTL_RST_AUTO, FALSE, &status );

    /* wait for auto negotiation to complete */
    epMIIpollBit( hsm, pmd->address, MII_STATUS,
        MIISTAT_AUTO_DONE, TRUE, &status );

    /* wait for link */
    epMIIpollBit( hsm, pmd->address, MII_STATUS,
        MIISTAT_LINK, TRUE, &status );

    if( status & MIISTAT_LINK ) {
        epPMDreadMode( hsm, &speed, &duplex );
        epSetMediaType( hsm, speed, duplex );
    }
    return( status );
}
/* init_code_end */
#ifdef  __cplusplus
}
#endif

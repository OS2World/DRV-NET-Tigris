/*
**      ephsm.h - EuphPhyter implementation of "Hsm"
**
**      $Archive:   /tmp_mnt/n/crabapple/nsclib/proj/euphyter/sw/include/vcs/elhsm.h_v  $
**      $Author: tkelley $
**      $Revision: 1.2 $
**
**      $Log: ephsm.h,v $
 * Revision 1.2  1998/08/03  22:20:20  tkelley
 * modified header file define statements
 *
**
*/

//#ifndef  _elhsm_h_
//#define  _elhsm_h_      1
#ifndef _ephsm_h_
#define _ephsm_h_

#ifndef  _nsctypes_h_
#include "nsctypes.h"
#endif

#ifndef  _euphyter_h_
#include "euphyter.h"
#endif

#ifndef  _hsm_h_
#include "hsm.h"
#endif

#ifndef  _nsm_h_
#include "nsm.h"
#endif

#ifndef  _cdi_h_
#include "cdi.h"
#endif

/*      defaults and limits     */
#define MAX_PACKET_SIZE         1514
#define CRC_SIZE                4

#define TX_BUFSIZE              (MAX_PACKET_SIZE+2)
#define RX_BUFSIZE              (MAX_PACKET_SIZE+2+CRC_SIZE)
#define RX_MAX_PACKET_SIZE      (MAX_PACKET_SIZE+CRC_SIZE)

#define MIN_TX_QSIZE            1
#define DEF_TX_QSIZE            3
//#define MAX_TX_QSIZE            8
#define MAX_TX_QSIZE            20

#define MIN_RX_QSIZE            2
#define DEF_RX_QSIZE            8
//#define MAX_RX_QSIZE            16
#define MAX_RX_QSIZE            40

//
// The Transmit Fill Threshold must always be larger than the Max DMA!
// Since we are using a Max DMA of 256 bytes, the Tx Fill Threshold is
// set to 288 bytes.  Selected a fill threshold equal to or less than
// the Max DMA can cause erroneous transmit underrun conditions.
//
//#define MAX_DMA                 0          /* 000b 512bytes */
#define MAX_DMA                 ( 7L << 20)   /* 111b 256bytes */
#define TX_FILL                 ( ( 288 / 32 ) << 8 )
//#define TX_FILL                 ( ( 544 / 32 ) << 8 )
#define MAX_PMDS                2
//#define DEF_TX_FILLT            ( 544 / 32 )
//#define TxDRNT_10               ( 512 / 32 )
#define TxDRNT_10               ( 1024 / 32 )
#define TxFIFO_SIZE             2048
//#define TxMAX_DMA               0       /* unlimited */
//#define RxMAX_DMA               0       /* unlimited */

/*      never make the transmit drain threshold higher than
**      FIFO size the transmit fill threshold */
#define TxDRNT_100              ( 1536 / 32 )
//#define TxDRNT_100              ( 1408 / 32 )
#define PMD_POLL_INTERVAL       2000

/*      The chip doesn't know anything about logical addresses,
**      so we're going to define our own descriptor structures
**      that provide some fields for the physical address of this
**      descriptor and the logical address of the next descriptor
**      in the ring.
*/
typedef struct EuphPhyterDesc {
    uint32      lLink;          /* logical ptr to next on list */
    uint32      handle;         /* optional "handle" for Nsm */
    Puchar      buf;           /* logical address of buffer */
    uint32      physAddr;       /* "physical" address of this desc */

    /* the hardware "sees" this part of the descriptor */
    uint32      pLink;          /* for use by hsm */
    uint32      cmdsts;         /* packet size in bytes */
    uint32      bufPhys;        /* physical address of buffer */
} EuphPhyterDesc, far *pEuphPhyterDesc;

/* typedef EuphPhyterDesc _FAR_ *pEuphPhyterDesc; */


/* The EuphPhyter version of HsmContext */

typedef struct PMD {
    uint16      dead;           /* not currently responding */
    uint16      address;        /* pmd address on MII mgmt bus */
    uint32      mediaTypes;     /* which media types this PHY supports */
    uint16      ctl;            /* current cmd register contents */
    uint16      sts;            /* current sts register contents */
    uint16      id0;            /* PHY ID 0 */
    uint16      id1;            /* PHY ID 1 */
    uint16      anar;           /* Auto negotiation abilities */
} PMD;

typedef struct EuphPhyterHsmContext {
    HsmContext  hsm;

    pEuphPhyterDesc txHead;       /* head of transmit descriptor list */
    pEuphPhyterDesc txTail;       /* tail of transmit descriptor list */
    pEuphPhyterDesc txFreeHead;   /* head of transmit descriptor free list */
    pEuphPhyterDesc txFreeTail; /* tail of transmit descriptor free list */
    pEuphPhyterDesc rxHead;       /* head of receive descriptor list */
    pEuphPhyterDesc rxTail;       /* tail of receive descriptor list */
    pEuphPhyterDesc rxBusyHead;   /* head of receive descriptor queued list */
    pEuphPhyterDesc rxBusyTail;   /* tail of receive descriptor queued list */

    pHsmPktDesc  txDescHead; /* head of "logical" decriptor list */
    pHsmPktDesc  txDescTail; /* tail of "logical" descriptor list */

    /* descriptors are kept separate from buffers */
    pEuphPhyterDesc txBase;     /* base of memory allocated for tx descs */
    uint32        txBasePhys;   /* phys addr of txBase */
    pEuphPhyterDesc rxBase;     /* base of memory allocated for rx descs */
    uint32        rxBasePhys;   /* phys addr of txBase */
    pHsmPktDesc   txDescBase; /* head of "logical" decriptor list */

    /* during HsmValidateContext(), we allocate memory store addresses here */
    /* during HsmReset(), we then configure this memory into descriptor lists */
//    Puchar      txBufs[MAX_TX_QSIZE];
//    uint32      txBufsPhys[MAX_TX_QSIZE];
//    Puchar      rxBufs[MAX_RX_QSIZE];
//    uint32      rxBufsPhys[MAX_RX_QSIZE];
    Puchar      txBufs[2];
    uint32      txBufsPhys[2];
    Puchar      rxBufs[2];
    uint32      rxBufsPhys[2];

    PMD         pmds[MAX_PMDS]; /* could be as many as 3 pmd's */

    uint32      isrValue;       /* last ISR value read */
    uint32      imrValue;       /* current IMR value */
    uint32      mediaType;      /* current media type selection */
    uint16      vendorID;       /* subsystem vendor ID */
    uint16      deviceID;       /* subsystem device ID */
    uchar       plexusRev;      /* plexus revision (from EEPROM) */
    uchar       mediaSelection; /* INSTALL's media selection */
    uchar       currentPmd;     /* current PMD in use */
    uchar       rxResetPending; /* we have a rx reset pending */
    uchar       rxLookState;    /* track lookahead operations */
#define RX_LOOK_IDLE    0       /* nothing happening */
#define RX_LOOK_PENDING 1       /* have a rx descriptor from NSM,
                                ** waiting for packet */
#define RX_LOOK_DISCARD 2       /* NSM doesn't want it */
#define RX_LOOK_COPIED  3       /* NSM copied it! */
    uchar       rxDrainThreshold; /* size of the rx drain theshold */
    uchar       numPmds;        /* how many are valid */
    uchar       rxBufsPerPage;  /* rcv buffers per page */
    uchar       txBufsPerPage;  /* xmit buffers per page */
    uchar       dummy;          /* for MASM 6.0 - member offset failure. */
    uint16      hashTable[ 32 ];
} EuphPhyterHsmContext;


/* An array structure which will hold the EEPROM MAC address;
   the MAC address does not fall on even words--i.e. the
   4 words have to be read to get the address instead of 3 */
typedef struct EuphMacAddr
{
    uchar       bytes[8];
    uint16      words[4];
}EuphMacAddr;


/* Macros for doing IO reads and writes */
#define IOR32( r )              NsmIOread32( (uint16)(hsm->IOaddr + r) )
#define IOW32( r, v )           NsmIOwrite32( (uint16)(hsm->IOaddr + r), v )
#ifdef  NSM_IOR32
#define EUPH_IOR32( v, r )      NSM_IOR32( v, (r+hsm->IOaddr) )
#else
#define EUPH_IOR32( v, r )      v = NsmIOread32( (uint16)(hsm->IOaddr + r) )
#endif

#endif

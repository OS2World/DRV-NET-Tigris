/*
**      macphyter.h      - MacPhyter Hardware Structures and Definitions
**
**
**      $Archive:   /tmp_mnt/n/nsclib/proj/macphyter/sw/include/vcs/euphphyter.h_v  $
**      $Author: tkelley $
**      $Revision: 1.7 $
**
**      $Log: euphyter.h,v $
 * Revision 1.7  1998/09/03  21:12:42  tkelley
 * updated the 'EuphPhyterEEOffsets'
 *
 * Revision 1.6  1998/08/04  00:20:15  tkelley
 * changed EEPROM bit definitions
 *
 * Revision 1.5  1998/08/03  23:31:59  tkelley
 * changed names of bit descriptions to match references in ephsm.c
 *
 * Revision 1.4  1998/08/03  22:41:22  tkelley
 * removed the 3 from #define 3COMRCR & #define 3COMRSR
 *
 * Revision 1.3  1998/08/03  22:38:13  tkelley
 * ISR,IMR-->#define SWI changed to #define SWINT
 *
 * Revision 1.2  1998/08/03  22:16:57  tkelley
 * modified macphy3v register set and bit definitions
 *
**
*/
#ifndef  _euphyter_h_
#define  _euphyter_h_   1

#ifndef  _nsctypes_h_
#include "nsctypes.h"
#endif

/* Configuration Registers - PCI BIOS */
#define CFGID           0x00
#define CFGCS           0x04
#define CFGRID          0x08
#define CFGLAT          0x0C
#define CFGIOA          0x10
#define CFGMA           0x14
// reserved 0x18 - 0x28
#define CFGSID          0x2C
#define CFGROM          0x30
#define CAPPTR          0x34
#define CFGINT          0x3C
#define PMCAP           0x40
#define PMCS            0x44

typedef struct EuphPhyterRegs {
    uint32      cr;             /* 0x00 command register */
    uint32      cfg;            /* 0x04 configuration register */
    uint32      mear;           /* 0x08 MII/EEPROM access register */
    uint32      ptscr;          /* 0x0C PCI test control register */
    uint32      isr;            /* 0x10 interrupt status register */
    uint32      imr;            /* 0x14 interrupt mask register */
    uint32      ier;            /* 0x18 interrupt enable register */
    uint32      _1[1];
    uint32      txdp;           /* 0x20 transmit descriptor pointer */
    uint32      txcfg;          /* 0x24 transmit configuration */
    uint32      _2[2];
    uint32      rxdp;           /* 0x30 receive descriptor pointer */
    uint32      rxcfg;          /* 0x34 receive configuration */
    uint32      _3[1];
    uint32      ccsr;           /* 0x3c clock run control and status register */
    uint32      wcsr;           /* 0x40 wake on lan/status control register */
    uint32      pcsr;           /* 0x44 pause control/status */
    uint32      rfcr;           /* 0x48 receive filter control */
    uint32      rfdr;           /* 0x4C receive filter data */
    uint32      brar;           /* 0x50 boot rom address */
    uint32      brdr;           /* 0x54 boot rom data */
    uint32      srr;            /* 0x58 silicon revision */
    uint32      mibc;           /* 0x5C mib control */
    uint32      mibs;            /* 0x60 start of MIB statistics */
    // MIB statistics--> 0x60-0x78
    //uint32      rxErroredPkts;  /* 0x60 receive errored packets */
    //uint32      rxFCSerrors;    /* 0x64 packets receive with FCS errors */
    //uint32      rxMissedPkts;      /* 0x68 packets missed (FIFO overruns */
    //uint32      rxFAEerrors;       /* 0x6C packets receive with FAE errors */
    //uint32      rxSymbolErrors;    /* 0x70 packets receive with invalid symbols */
    //uint32      rxFramesTooLong;   /* 0x74 packets received > 1518 bytes in length */
    //uint32      txSQEerrors;       /* 0x78 packet sent with SQE detected */
    uint32      _4[7];
    uint32      bmcr;           /* 0x80 basic mode control */
    uint32      bmsr;           /* 0x84 basic mode status */
    uint32      phyid1;        /* 0x88 phy identifier */
    uint32      phyid2;        /* 0x8C phy identifier 2 */
    uint32      anar;           /* 0x90 auto negotiation advertisement */
    uint32      anlpar;         /* 0x94 auto negotiation link partner ability (base) */
    uint32      aner;           /* 0x98 auto negotiate expansion */
    uint32      annptr;         /* 0x9C auto negotiation next page transmit */
    uint32      _5[8];
    uint32      physts;         /* 0xC0 phy status */
    uint32      micr;           /* 0xC4 MII interrupt control */
    uint32      misr;           /* 0xC8 MII interrupt status & misc control */
    uint32      pagesel;        /* 0xCC page select */
    // Extended Registers-->Page 0
    uint32      fcscr;          /* 0xD0 false carrier sense counter */
    uint32      recr;           /* 0xD4 receiver error counter */
    uint32      pcs;            /* 0xD8 PCS sub layer config & status */
    uint32      rbr;            /* 0xDC RMII & bypass */
    uint32      _6[1];
    uint32      phyctl;        /* 0xE4 phy control */
    uint32      tenbt;       /* 0xE8 10BaseT status/control */
    uint32      cdctl1;        /* 0xEC CD test control 1 */
    uint32      _7[4];
    // Test Registers-->Page 1
    uint32      fcoctl;        /* 0xD0 FCO control */
    uint32      tmr;            /* 0xD4 test mode */
    uint32      bgr;            /* 0xD8 band gap reference */
    uint32      crm;            /* 0xDC CRM */
    uint32      cdctl2;        /* 0xE0 CD test control 2 */
    uint32      pmdcsr;         /* 0xE4 PMD control/status */
    uint32      pgmcgm;         /* 0xE8 PGM/CGM control */
    uint32      dsptst;        /* 0xEC DSP test */
    uint32      extcfg;         /* 0xF0 extended config */
    uint32      dspcfg;         /* 0xF4 DSP config */
    uint32      sdcfg;          /* 0xF8 signal detect config. */
    uint32      tdata;       /* 0xFC test data */
} EuphPhyterRegs;

/*      when IO mapped, use this enum to get register offsets
*/
enum EuphPhyterRegOffsets {
    cr          = 0x00,         /* command register */
    cfg         = 0x04,         /* configuration register */
    mear        = 0x08,         /* MII/EEPROM access register */
    ptscr       = 0x0C,         /* PCI test control register */
    isr         = 0x10,         /* interrupt status register */
    imr         = 0x14,         /* interrupt mask register */
    ier         = 0x18,         /* interrupt enable register */
    txdp        = 0x20,         /* transmit descriptor pointer */
    txcfg       = 0x24,         /* transmit configuration */
    rxdp        = 0x30,         /* receive descriptor pointer */
    rxcfg       = 0x34,         /* receive configuration */
    ccsr        = 0x3c,         /* clock run control and status register */
    wcsr        = 0x40,         /* wake on lan/status control */
    pcsr        = 0x44,         /* pause control/status */
    rfcr        = 0x48,         /* receive filter control */
    rfdr        = 0x4C,         /* receive filter data */
    brar        = 0x50,         /* boot rom address */
    brdr        = 0x54,         /* boot rom data */
    srr         = 0x58,         /* silicon revision */
    mibc        = 0x5C,         /* mib control */
    mibs         = 0x60,         /* start of MIB statistics */
    // MIB statistics--> 0x60-0x78
    //rxErroredPkts       = 0x60, /* receive errored packets */
    //rxFCSerrors         = 0x64, /* packets receive with FCS errors */
    //rxMissedPkts        = 0x68, /* packets missed (FIFO overruns */
    //rxFAEerrors         = 0x6C, /* packets receive with FAE errors */
    //rxSymbolErrors      = 0x70, /* packets receive with invalid symbols */
    //rxFramesTooLong     = 0x74, /* packets received > 1518 bytes in length */
    //txSQEerrors         = 0x78, /* packet sent with SQE detected */
    bmcr        = 0x80,         /* basic mode control */
    bmsr        = 0x84,         /* basic mode status */
    phyid1     = 0x88,         /* phy identifier */
    phyid2     = 0x8C,         /* phy identifier 2 */
    anar        = 0x90,         /* auto negotiation advertisement */
    anlpar      = 0x94,         /* auto negotiation link partner ability (base) */
    aner        = 0x98,         /* auto negotiate expansion */
    annptr      = 0x9C,         /* auto negotiation next page transmit */
    physts      = 0xC0,         /* phy status */
    micr        = 0xC4,         /* MII interrupt control */
    misr        = 0xC8,         /* MII interrupt status & misc control */
    pagesel     = 0xCC,         /* page select */
    // Extended Registers-->Page 0
    fcscr       = 0xD0,         /* false carrier sense counter */
    recr        = 0xD4,         /* receiver error counter */
    pcs         = 0xD8,         /* PCS sub layer config & status */
    rbr         = 0xDC,         /* RMII & bypass */
    phyctl     = 0xE4,         /* phy control */
    tenbt    = 0xE8,         /* 10BaseT status/control */
    cdctl1     = 0xEC,         /* CD test control 1 */
    // Test Registers-->Page 1
    fcoctl     = 0xD0,         /* FCO control */
    tmr         = 0xD4,         /* test mode */
    bgr         = 0xD8,         /* band gap reference */
    crm         = 0xDC,         /* CRM */
    cdctl2     = 0xE0,         /* CD test control 2 */
    pmdcsr      = 0xE4,         /* PMD control/status */
    pgmcgm      = 0xE8,         /* PGM/CGM control */
    dsptst     = 0xEC,         /* DSP test */
    extcfg      = 0xF0,         /* extended config */
    dspcfg      = 0xF4,         /* DSP config */
    sdcfg       = 0xF8,         /* signal detect config. */
    tdata    = 0xFC          /* test data */
};

/*  CR  - command register bit definitions */
#define RESET           0x00000100      /* soft reset */
#define SWI             0x00000080      /* software interrupt */
#define RxRESET         0x00000020      /* receiver reset */
#define TxRESET         0x00000010      /* transmitter reset */
#define RxDIS           0x00000008      /* receiver disable */
#define RxENA           0x00000004      /* receiver enable */
#define TxDIS           0x00000002      /* transmitter disable */
#define TxENA           0x00000001      /* transmitter enable */

/*  CFG  - global configuration register bits */
#define LNKSTS          0x80000000      /* link status */
#define SPEED100        0x40000000      /* speed 100Mb */
#define FDUP            0x20000000      /* full duplex */
#define POL             0x10000000      /* 10Mb polarity indication */
#define ANEG_DN         0x08000000      /* autoneg done */
#define PAUSE_AD        0x00010000      /* strap for pause capable */
#define STRP_DUP        0x00008000      /* strap for full duplex */
#define STRP_SPD        0x00004000      /* strap phy for 100Mb capable */
#define STRP_AN         0x00002000      /* strap for auto negotiate */
#define EXT_PHY         0x00001000      /* support for external phy */

#if 0
//reserved
#define EXT_MAC         0x00000800      /* support for external mac */
#endif 

#define PHY_RST         0x00000400      /* reset internal phy */
#define PHY_DIS         0x00000200      /* disable interal phy */
#define EUPHCOM         0x00000100      /* Euphrates compatibility */
#define REQALG          0x00000080      /* PCI bus request algorithm */
#define SB              0x00000040      /* Single Backoff */
#define POW             0x00000020      /* program out-of-window */
#define EXD             0x00000010      /* disable excessive deferral errs */
#define PESEL           0x00000008      /* assert SERR on DPERR detection */
#define BROM_DIS        0x00000004      /* disable boot rom interface (unused) */
#define BEM             0x00000001      /* big endian mode */

/* MEAR - MII/EEPROM access register */
#define MDC             0x00000040      /* MII management data clock */
#define MDDIR           0x00000020      /* MII management data direction */
#define MDIO            0x00000010      /* MII management data input/output */
#define EECS            0x00000008      /* EEPROM chip select */
#define EECLK           0x00000004      /* EEPROM serial clock */
#define EEDO            0x00000002      /* EEPROM data out (our data in) */
#define EEDI            0x00000001      /* EEPROM data in (out data out) */

/* PTSCR - PCI Test Control Register */
#define BMTM_EN         0x00001000      /* bus master test mode enable */
#define BIST_RST        0x00000400      /* SRAM BIST reset */
#define BIST_CLKD       0x00000200      /* SRAM BIST clock data */
#define BIST_EN         0x00000080      /* SRAM BIST enable */
#define BIST_ACT        0x00000040      /* SRAM BIST complete (RO) */
#define RXFAIL          0x00000020      /* SRAM BIST rx ram fail (RO) */
#define TXFAIL          0x00000010      /* SRAM BIST tx ram fail (RO) */
#define RX_FLT_FAIL     0x00000008      /* SRAM BIST rx filter fail (RO) */
#define EELOAD          0x00000004      /* enable EEPROM load */
#define EEBIST_EN       0x00000002      /* enable EEPROM BIST */
#define EEBIST_FAIL     0x00000001      /* EE BIST fail indication */

/* ISR, IMR - interrupt bit definitions */
#define TXRCMP          0x02000000      /* transmit reset complete */
#define RXRCMP          0x01000000      /* receive reset complete */
#define DPERR           0x00800000      /* detected parity error */
#define SSERR           0x00400000      /* signalled system error */
#define RMABT           0x00200000      /* received system abort */
#define RTABT           0x00100000      /* received target abort */
#define RxSOVR          0x00010000      /* rx status fifo overrun */
#define HIBERR          0x00008000      /* hi bit error */
#define PHY             0x00004000      /* phy interrupt */
#define PME             0x00002000      /* power management event */
#define SWINT           0x00001000      /* software interrupt */
#define MIBINT          0x00000800      /* MIB service */
#define TxURN           0x00000400      /* tx underrun */
#define TxIDLE          0x00000200      /* tx idle */
#define TxERR           0x00000100      /* tx packet error */
#define TxDESC          0x00000080      /* tx descriptor interrupt */
#define TxOK            0x00000040      /* tx OK interrupt */
#define RxORN           0x00000020      /* rx overrun */
#define RxIDLE          0x00000010      /* rx idle */
#define RxEARLY         0x00000008      /* rx early threshold */
#define RxERR           0x00000004      /* rx packet error */
#define RxDESC          0x00000002      /* rx descriptor interrupt */
#define RxOK            0x00000001      /* rx OK interrupt */

/* IER  - interrupt enable register bit definitions */
#define IE              0x00000001      /* interrupt enable */

/* TXDP - transmit descriptor pointer register bit definitions */
#define TXDP            0xFFFFFFFC      /* (&) transmit descriptor pointer--bits 31-2 */

/* TXCFG - transmit configuration register bit definitions */
#define TxCSI           0x80000000      /* carrier sense ignore */
#define TxHBI           0x40000000      /* heartbeat ignore */
#define TxMLB           0x20000000      /* MAC loopback */
#define TxATP           0x10000000      /* automatic transmit padding */
#define TxIFG           0x0C000000      /* (&) interframe gap time */
#define TxECRETRY       0x00800000      /* excessive collision retry */
#define TxMXDMA         0x00700000      /* (256 bytes) tx max cycles per DMA burst */
//#define TxMXDMA_shift   20              /* .. as a shift value */
/* IP */
// uh? Spec calls for that FLTH must be greater
// than  MXDMA, which is 256 byte, then, how is 
// a single 32 byte greater than 256 byte??
//#define TxFLTH          0x00000100      /* (32 bytes) tx fill threshold */
// THEREFORE, SETTING TxFLTH to 512 byte
#define TxFLTH          0x00001000      /* (512 bytes) tx fill threshold; IP */
//#define TxFLTH_shift    8               /* .. as a shift value */
//#define TxDRNT          0x00000002      /* (64 bytes) tx drain threshold--bits 5-0 */
#define TxDRNT          0x00000030      /* (1536 bytes) tx drain threshold--bits 5-0; IP */

/* RXDP - receive descriptor register bit definitions */
#define RXDP            0xFFFFFFFC      /* (&) receive descriptor pointer--bits 31-2 */

/* RXCFG - receive configuration register bit definitions */
#define RxAEP           0x80000000      /* rx accept errored packets */
#define RxARP           0x40000000      /* rx accept runt packets */
#define RxATP           0x10000000      /* rx accept xmit packets (fdx) */
#define RxALP           0x08000000      /* rx accept long packets */
#define RxMXDMA         0x00700000      /* (&) rx max cycles per DMA burst */
#define RxMXDMA_shift   20              /* .. as a shift value */
#define RxDRNT          0x0000003E      /* (&) rx drain threshold--bits 5-1 */

/* CCSR - clock run constrol/status register */
#define PMESTS          0x00008000      /* a wake event has been received */
#define PMEEN           0x00000100      /* enable PME pin */
#define CLKRUN_EN       0x00000001      /* enable clock run function */

/* WCSR - wake on lan control/status register */
#define MPR             0x80000000      /* magic pkt received */
#define PATM3           0x40000000      /* pattern 3 match */
#define PATM2           0x20000000      /* pattern 2 match */
#define PATM1           0x10000000      /* pattern 1 match */
#define PATM0           0x08000000      /* pattern 0 match */
#define ARPR            0x04000000      /* ARP received */
#define BCASTR          0x02000000      /* broadcast received */
#define MCASTH          0x01000000      /* mulitcast hash match */
#define DAMATCH         0x00800000      /* DA match */
#define PHYINT          0x00400000      /* phy interrupt */
#define SOHACK          0x00200000      /* secure on hack attempt */
#define MPSOE           0x00000400      /* magic pkt secureOn enable */
#define WKMAG           0x00000200      /* wake on magic pkt */
#define WKPAT3          0x00000100      /* wake on pattern 3 match */
#define WKPAT2          0x00000080      /* wake on pattern 2 match */
#define WKPAT1          0x00000040      /* wake on pattern 1 match */
#define WKPAT0          0x00000020      /* wake on pattern 0 match */
#define WKARP           0x00000010      /* wake on ARP */
#define WKBCP           0x00000008      /* wake on broadcast */
#define WKMCP           0x00000004      /* wake on multicast match */
#define WKUCP            0x00000002      /* wake on unicast */
#define WPHY            0x00000001      /* wake on phy interrupt */

/* PCSR - pause control/status register */
#define PSEN            0x80000000      /* pause enable */
#define PS_MCAST        0x40000000      /* pause on multicast */
#define PS_DA           0x20000000      /* pause on DA */
#define PS_ACT          0x00800000      /* pause active */
#define PS_RCVD         0x00400000      /* pause frame received */
#define PSNEG           0x00200000      /* pause negotiated */
#define MLD_EN          0x00010000      /* manual load enable */
#define PAUSE_CNT       0x0000FFFF      /* (&) pause counter value */

/* RFCR - receive filter control register */
#define RFAA_shift      28
#define RFEP_shift      16
#define RFEN            0x80000000      /* RF enable */
#define RFAAB           0x40000000      /* RF accept all broadcasts */
#define RFAAM           0x20000000      /* RF accept all multicasts */
#define RFAAU           0x10000000      /* RF accept all unicast */
#define APM             0x08000000      /* accept on perfect match */
#define APAT            0x07800000      /* (&) accept on pattern match--bits 26-23 */
#define AARP            0x00400000      /* accept ARP packets */
#define MHEN            0x00200000      /* mulitcast hash enable */
#define UHEN            0x00100000      /* unicast hash enable */
#define ULM             0x00080000      /* U/L bit mask */
// The following are values for the RFADDR bits 8-0
#define OCTET10         0x00000000      /* perfect match register octets 1-0 */
#define OCTET32         0x00000002      /* perfect match register octets 3-2 */
#define OCTET54         0x00000004      /* perfect match register octets 5-4 */
#define PATTERN10       0x00000006      /* pattern 1 count, pattern 0 count */
#define PATTERN32       0x00000008      /* pattern 3 count, pattern 2 count */
#define SECURE10        0x0000000A      /* secureOn password octets 1-0 */
#define SECURE32        0x0000000C      /* secureOn password octets 3-2 */
#define SECURE54        0x0000000E      /* secureOn password octets 5-4 */
// #define RXFILTER values 200h-3FEh

/* RFDR- receive filter data register */
#define RFDATA  0x0000FFFF     /* (&) RF Data--bits 15-0 */
#define BMASK   0x00030000     /* (&) byte mask--bits 17-16 */

/* BRAR - boot rom address register */
#define AUTOINC         0x80000000      /* auto increment */
#define BOOTADDR        0x0000FFFF      /* (&) mask for boot rom address */

/* BRDR - boot rom data register */
#define BOOTDATA        0xFFFFFFFF      /* (&) mask for boot rom data */

/* SRR - silicon revision register */
#define MAJ     0x0000FF00      /* (&) major revision level--read only */
#define MIN     0x000000FF      /* (&) minor revision level--read only */

/* MIBC - MIB control register */
#define mibSTR          0x00000008      /* mib counter strobe */
#define mibACLR         0x00000004      /* clear all counters */
#define mibFRZ          0x00000002      /* freeze all counters */
#define mibWRN          0x00000001      /* warning test indicator */


typedef struct EuphPhyterDescriptor {
    uint32      link;           /* link to the next descriptor */
    uint32      cmdsts;         /* command and status bits (below) */
    uint32      bufptr;            /* physical pointer to data */
} EuphPhyterDescriptor;

/* EuphPhyterDescriptor.cmdsts bit definitions (common to both recv and xmit) */
#define OWN             0x80000000      /* set to 1 to give ownership from */
                                        /* data producer to data consumer */
                                        /* set to 0 by data consumer to  */
                                        /* release resources back to data */
                                        /* producer */

#define MORE            0x40000000      /* set to 1 to indicate that this */
                                        /* is not the last descriptor in */
                                        /* a packet */

#define INTR            0x20000000      /* set to 1 by driver to ask for */
                                        /* an interrupt when descriptor */
                                        /* is completed */

#define CRC             0x10000000      /* For transmit descriptors, indicates that CRC
                                        /* should not be appended by the MAC; for receive
                                        /* descriptors, this bit is always set--include CRC */

#define OK              0x08000000      /* set to 1 by EuphPhyter to indicate */
                                        /* the packet was sent or received  */
                                        /* without errors */

#define DSIZE           0x00000FFF      /* (&) mask for descriptor size field--bits 11-0 */

/* cmdsts bits in a transmit descriptor--cmdsts bits 26-16 */
#define SUPCRC          0x10000000      /* suppress CRC generation--cmdsts bit 28 */
#define TXA             0x04000000      /* transmit aborted */
#define TFU             0x02000000      /* transmit FIFO underrun */
#define CRS             0x01000000      /* carrier lost during xmit */
#define TD              0x00800000      /* transmit deferred */
#define ED              0x00400000      /* excessive deferral */
#define OWC             0x00200000      /* out of window collision */
#define EC              0x00100000      /* excessive collisions */
#define CCNT            0x000F0000      /* (&) collision count--bits 19-16 */

/* cmdsts bits in a receive descriptor--cmdsts bits 26-16 */
#define INCCRC          0x10000000      /* include CRC in packet size--cmdsts bit 28 */
#define RXA             0x04000000      /* receive aborted */
#define OVERRUN         0x02000000      /* rx fifo overrun */
#define DEST            0x01800000      /* (&) packet matched perfect match reg--bits 24-23 */
#define TOOLONG         0x00400000      /* too long packet received */
#define RUNT            0x00200000      /* is a runt packet */
#define RXISERR         0x00100000      /* invalid symbol error */
#define CRCERR          0x00080000      /* CRC error */
#define FAERR           0x00040000      /* frame alignment error */
#define LOOPBK          0x00020000      /* loopback packet received */
#define RXCOL           0x00010000      /* collision activity during */
                                        /* packet reception */
#define RXSTS_shift     18 /*(?)*/


/* offsets into the EEPROM */
enum EuphPhyterEEOffsets {
    CFGSID0_15          =       0x00,
    CFGSID16_31         =       0x01,
    CFGINT24_31         =       0x02,
    combo               =       0x03,
    sopas1_16           =       0x04,
    sopas17_32          =       0x05,
    sopas_pmatch        =       0x06,
    pmatch1_16          =       0x07,
    pmatch17_32         =       0x08,
    pmatch_WCSR         =       0x09,
    WCSR_RFCR           =       0x0A,
    checksum            =       0x0B
    //EuphPhyterEEMACAddr         = 0x00,         /* ethernet address */
    //EuphPhyterEEVendorID                = 0x06,         /* subsystem vendor ID address */
    //EuphPhyterEEDeviceID                = 0x07,         /* subsystem device ID address */
    //EuphPhyterEECardTypeRev     = 0x08,         /* card type and revision */
    //EuphPhyterEEPlexusRev               = 0x09,         /* plexus revision */
    //EuphPhyterEEChecksum                = 0x0F          /* Checksum word */
};

/* EEPROM NM93C46AL constants see pg 1-108*/
#define EEread          0x0180          /* .. read */
#define EEwrite         0x0140          /* .. write */
#define EEerase         0x01C0          /* .. erase */
#define EEwriteEnable   0x0130          /* .. erase / write enable */
#define EEwriteDisable  0x0100          /* .. erase / write disable */
#define EEeraseAll      0x0120          /* .. erase */
#define EEwriteAll      0x0110          /* .. erase */
#define EEaddrMask      0x013F          /* .. address mask */
#define EEcmdShift      16              /* .. address shift count */


enum EuphPhyterMibOffsets {
    rxErroredPkts       = 0,    /* 0x60 receive errored packets */
    rxFCSerrors         = 4,    /* 0x64 packets receive with FCS errors */
    rxMissedPkts        = 8,    /* 0x68 packets missed (FIFO overruns */
    rxFAEerrors         = 12,   /* 0x6C packets receive with FAE errors */
    rxSymbolErrors      = 16,   /* 0x70 packets receive with invalid symbols */
    rxFramesTooLong     = 20,   /* 0x74 packets received > 1518 bytes in length */
    txSQEerrors         = 24    /* 0x78 packet sent with SQE detected */
};

/* MII management bus constants */
#define MIIread         0x6000
#define MIIwrite        0x5002
#define MIIpmdMask      0x0F80
#define MIIpmdShift     7
#define MIIregMask      0x007C
#define MIIregShift     2
#define MIIturnaroundBits       2
#define MIIcmdLen       16 /*(?)*/
#define MIIcmdShift     16 /*(?)*/
#define MIIreset        0xffffffff
#define MIIwrLen        32 /*(?)*/

/* COMCORE1 REGISTER OFFSETS (physical layer device) */
#define BMCR            0x00    /* basic mode control */
#define BMSR            0x01    /* basic mode status */
#define PHYIDR1         0x02    /* phy identifier */
#define PHYIDR2         0x03    /* phy identifier 2 */
#define ANAR            0x04    /* auto negotiation advertisement */
#define ANLPAR          0x05    /* auto negotiation link partner ability (base) */
#define ANER            0x06    /* auto negotiate expansion */
#define ANNPTR          0x07    /* auto negotiation next page transmit */
#define PHYSTS          0x10    /* phy status */
#define MICR            0x11    /* MII interrupt control */
#define MISR            0x12    /* MII interrupt status & misc control */
#define PAGESEL         0x13    /* page select */

/* Extended Registers-->Page 0 */
#define FCSCR           0x14    /* false carrier sense counter */
#define RECR            0x15    /* receiver error counter */
#define PCSR            0x16    /* PCS sub layer config & status */
#define RBR             0x17    /* RMII & bypass */
// reserved     0x18
#define PHYCTRL         0x19    /* phy control */
#define TENBTSCR        0x1A    /* 10BaseT status/control */
#define CDCTRL1         0x1B    /* CD test control 1 */

/* Test Registers-->Page 1 */
#define FCOCTRL         0x14    /* FCO control */
#define TMR             0x15    /* test mode */
#define BGR             0x16    /* band gap reference */
#define CRM             0x17    /* CRM */
#define CDCTRL2         0x18    /* CD test control 2 */
#define PMDCSR          0x19    /* PMD control/status */
#define PGM_CGM         0x1A    /* PGM/CGM control */
#define DSPTEST         0x1B    /* DSP test */
#define EXTCFG          0x1C    /* extended config */
#define DSPCFG          0x1D    /* DSP config */
#define SDCFG           0x1E    /* signal detect config. */
#define TESTDATA        0x1F    /* test data */


#endif




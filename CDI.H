/* 
**      tech/include/cdi.h 
** 
**      Definitions and Functions declarations for 
**      harward independent NIC access, control and management 
**       
**      $Archive:   /tmp_mnt/n/crabapple/nsclib/tech/include/vcs/cdi.h_v  $ 
**      $Author: mikeh $ 
**      $Revision: 1.2 $ 
** 
**      $Log: cdi.h,v $
 * Revision 1.2  1998/09/04  00:21:59  mikeh
 * *** empty log message ***
 *
 * Revision 1.1  1998/09/03  23:31:19  mikeh
 * Initial revision
 * 
**	
**	   Rev 1.90   11/25/96 16:30:24   rdunlap
**	add HSM_MEDIA_TYPE_MAX for sanity checking of media type;
**	
**	   Rev 1.89   11/21/96 14:55:06   rdunlap
**	add _FAR_ comment/doc. on NsmFree();
**	
**	   Rev 1.88   10/15/96 13:11:22   rdunlap
**	add HSM_DOES_RX_MULTICOPIES for compile-time testing instead of
**	  run-time checking (see comments with it);
**	
**	   Rev 1.87   05/15/96 16:32:36   rdunlap
**	correct comment from int to void on HsmUpdateStatistics();
**	
**	   Rev 1.86   22 Aug 1995 15:42:12   croussel
**	
**	added definition of KW_NO32BITIO and
**	HsmContext.nsmOptions.DONT_USE_32BIT_IO
**	
**	   Rev 1.85   15 Aug 1995 16:20:34   croussel
**	remove HsmRxFreePkt, never implemented
**	remove NsmLogMessage, and Add NsmStdMessage
**	
**	   Rev 1.84   25 Jul 1995 14:50:20   croussel
**	change return type of NsmRxComplete, and comments for what
**	this function should return.  Add spelling correction for
**	HSM_RX_CRC_ERROR, but allow older spelling to continue.
**	
**	   Rev 1.83   10 Jul 1995 18:30:40   vikas
**	
**	supplied a missing #endif.
**	
**	   Rev 1.82   10 Jul 1995 17:00:10   vikas
**	
**	os/2 odi: 2nd argument to NsmLogMessage() is a message index as
**	opposed to a message pointer.
**	
**	   Rev 1.81   06/28/95 14:40:30   rdunlap
**	change comment on and semantics of HsmContext.MEMaddrSize;
**	
**	   Rev 1.80   06/27/95 12:50:02   rdunlap
**	add LINK_UNKNOWN value for LinkState;
**	
**	   Rev 1.79   06/21/95 11:51:56   rdunlap
**	change Slot comment to include its use for EISA & PCMCIA;
**	change HsmContext.pad1 to <BusNumber> & <FunctionNumber>;
**	
**	   Rev 1.78   15 Jun 1995 11:25:12   croussel
**	added KW_SOFTCRC, hsmOptions.DOES_SOFT_CRC and nsmOptions.DO_SOFT_CRC
**	enhanced comment for HsmReset()
**	
**	   Rev 1.77   08 Jun 1995 12:36:38   croussel
**	add HSM_MEDIA_AUTO_TP
**	
**	   Rev 1.76   05/23/95 10:25:32   croussel
**	fixed comment for logicalMemAddr
**	
**	   Rev 1.75   16 May 1995 12:28:22   croussel
**	allow the ability to redefine NsmWaitTime, NsmGetTime, and
**	NsmLogicalToPhysical
**	
**	   Rev 1.74   04/20/95 15:18:24   croussel
**	correct values for HsmContext.BusWidth
**	
**	   Rev 1.73   03 Apr 1995 13:19:48   croussel
**	
**	NsmMallocPhys now returns physical address
**	
**	   Rev 1.72   03/24/95 15:55:36   croussel
**	pass uint32 "handle" as second arg of NsmTransmitComplete()
**	
**	   Rev 1.71   03/14/95 14:25:32   croussel
**	added HSM_CAN_SHARE_INTS
**	
**	   Rev 1.70   08 Mar 1995 10:11:48   croussel
**	
**	added NSM_DOUBLE_BUFFER, see comments
**	added HSM_MEDIA_100BASETT4, we'll need it when T4
**	PHY is attached to external MII
**	added transmit status values, which were never defined
**	rewrote comment to NsmRxLookahead to clarify return
**	code requirements
**	
**	   Rev 1.69   02 Mar 1995 10:53:54   croussel
**	
**	add HSM_MEDIA_10BASETfx_ALT
**	
**	   Rev 1.68   02 Mar 1995 10:47:48   croussel
**	add HSM_MEDIA_10BASET_ALT
**	
**	   Rev 1.67   03/01/95 15:50:04   rdunlap
**	comments/doc. changes to make comment block of public entry points
**	and the actual "extern" functions consistent in names, parameters,
**	parameter types, etc.;
**	add some functions in the front/beginning comment block of public
**	  functions that were already added as extern functions (e.g.,
**	  HsmForceInterrupt, HsmShutdown, HsmTimerEvent, HsmValidateContext,
**	  NsmRxComplete, NsmEnter/LeaveCriticalSection, Nsm{TimerFunctions});
**	all changes are intended to help new users of CDI be less confused
**	  and are 99% comments changes;
**	other changes are purely for consistency (add "extern" to one function,
**	  e.g.);
**	
**	   Rev 1.66   02/28/95 12:33:18   rdunlap
**	change parameter #2 of HsmSetMediaType() to a uint32;
**	
**	   Rev 1.65   02/24/95 15:54:20   croussel
**	added HSM_MEDIA_EXT_MII
**	
**	   Rev 1.64   23 Feb 1995 11:08:26   P_CRINION
**	made function a uchar and *handle a _FAR_ in NsmCallCss()
**	
**	   Rev 1.63   02/22/95 09:50:00   croussel
**	restore previous TAB formatting, align to fullword boundary
**	after CardExists
**	
**	   Rev 1.61   07 Feb 1995 14:32:50   croussel
**	
**	add function definition for HsmValidateContext()
**	
**	   Rev 1.60   01/24/95 10:05:12   croussel
**	replace pad2 with LinkState, per NSSNAIS v0.19
**	
**	   Rev 1.59   23 Jan 1995 11:46:30   croussel
**	add additional values for FullDuplexCapable per Dave Milne's request
**	FullDuplexCapable must now be set by HSM, not NSM
**	
**	   Rev 1.58   01/02/95 16:37:16   croussel
**	GNU C doesn't like the extra start comment
**	
**	   Rev 1.57   12/21/94 10:58:34   croussel
**	NSSNAIS MediaSpeed is in MBps, not enum values
**	
**	   Rev 1.56   12/21/94 10:55:00   croussel
**	ANSIId should be 32 bytes, not 20
**	
**	   Rev 1.55   12/14/94 10:24:10   croussel
**	add comment for HSM_HEAP_SIZE, add nsmOption.DO_MAX_PERFORMANCE
**	
**	   Rev 1.54   12/12/94 17:34:24   croussel
**	change the form of the ifndef macros so they don't conflict with
**	the function name.  These are used to turn NSM functions into 
**	inline macros
**	
**	   Rev 1.53   12/09/94 14:23:06   rdunlap
**	change "far" to "_FAR_" in prototype for NsmCallCss()
**	
**	   Rev 1.52   12/09/94 14:08:56   rdunlap
**	move Card Services data above the NSSNAIS data in HsmContext;
**	change "bool" to "uchar" to save memory and make it a fixed size
**	  in any compiler/target environment (else bool == int == 16 or 32 bits)
**	
**	   Rev 1.51   12/02/94 15:44:50   croussel
**	add comment to maxpacketsize
**	
**	   Rev 1.50   30 Nov 1994 15:30:22   P_CRINION
**	
**	added NsmCallCss function
**	
**	   Rev 1.49   30 Nov 1994 11:35:00   P_CRINION
**	added CardServPres & ClientHandle fields for PCMCIA
**	 
**	   Rev 1.48   11/22/94 13:16:20   croussel 
**	new function prototype for HsmShutdown 
**	removing the "permanent" parameter 
**	 
**	   Rev 1.47   11/17/94 14:57:04   rdunlap 
**	updated #define values for DriverIFType field. 
**	 
**	   Rev 1.46   11/16/94 16:31:56   croussel 
**	allow preprocessor overloading of NsmDbugMessage 
**	 
**	   Rev 1.45   09 Nov 1994 15:51:40   croussel 
**	 
**	allow preprocessor override of Nsm[Enter|Leave]CriticalSection 
**	 
**	   Rev 1.44   11/08/94 14:57:20   croussel 
**	added comments describing HSM_NEED_?X_PHYS_ADDRS 
**	added comment describing the "no nesting" requirement for  
**	NsmEnterCriticalSection 
**	 
**	   Rev 1.43   11/01/94 13:49:14   croussel 
**	arg to NsmLogicalToPhysical should be a _FAR_ 
**	 
**	   Rev 1.42   10/27/94 09:50:12   rdunlap 
**	add HsmContext *Context parameter to all remaining NSM functions that 
**	  did not have it (except for NsmIO... functions) 
**	 
**	   Rev 1.41   10/26/94 10:22:02   rdunlap 
**	updated enumerated define values (to match NSSNAIS) for MediaType 
**	  and BusWidth. 
**	 
**	   Rev 1.40   10/25/94 13:40:40   rdunlap 
**	add int *status to HsmRxCopyPkt for updated packet status value 
**	 
**	   Rev 1.39   10/24/94 14:21:14   rdunlap 
**	correct Topology define spellings 
**	 
**	   Rev 1.38   10/24/94 14:15:00   rdunlap 
**	add #define values for FullDuplexCapable and Topology 
**	 
**	   Rev 1.37   10/18/94 17:28:04   rdunlap 
**	changed comment (description) of ROMrevision 
**	 
**	   Rev 1.36   10/18/94 13:47:24   croussel 
**	added HsmContext argument to Nsm[Enter|Leave]CriticalSection 
**	 
**	   Rev 1.35   10/11/94 12:37:58   croussel 
**	add HsmZeroStatistics 
**	 
**	   Rev 1.34   10/10/94 16:40:14   croussel 
**	update to NSSNAIS 0.16 
**	 
**	   Rev 1.33   10/10/94 11:38:36   croussel 
**	added comments for NSM_MAX_LOOKAHEAD_SIZE and NSM_MAX_TX_FRAGS 
**	 
**	   Rev 1.32   07 Oct 1994 11:34:06   croussel 
**	 
**	define MAX_TX_FRAGS to 8 when no HSM_MAX_TX_FRAGS defined, so that 
**	source which doesn't even care about descriptors can compile without 
**	errors 
**	 
**	   Rev 1.31   10/06/94 14:24:32   croussel 
**	added logicalMemAddr 
**	 
**	   Rev 1.30   05 Oct 1994 16:00:36   croussel 
**	allow macro overrides of NsmIO functions to facilitate the 
**	use of MSVC intrinsics on DOS real mode 
**	 
**	   Rev 1.29   10/05/94 14:05:44   croussel 
**	sized HsmPktDesc using MAX_TX_FRAGS 
**	set MAX_TX_FRAGS to min(NSM_MAX_TX_FRAGS,HSM_MAX_TX_FRAGS) 
**	 
**	   Rev 1.28   10/04/94 17:10:14   croussel 
**	added MulticastTable, and MulticastTableSize to HsmContext, 
**	removed them as parameters to HsmMulticastLoad. 
**	Add comment that requires NsmLogicalToPhysical to return a flat 32-bit 
**	address, not a segment:offset (or selector:offset). 
**	 
**	   Rev 1.27   09/29/94 15:25:16   croussel 
**	added typedef for pHsmPktDesc, that _FAR_ thing is ugly. 
**	 
**	   Rev 1.26   09/27/94 14:39:14   rdunlap 
**	add HsmPacketNotReady (enum list) for HsmRxCopyPkt() return code 
**	 
**	   Rev 1.25   09/23/94 15:17:20   rdunlap 
**	HsmGetTxDescList() returns a _FAR_ ptr 
**	HsmTransmit() takes a _FAR_ *pktDesc; 
**	NsmTransmitComplete() takes a _FAR_ *pktDesc. 
**	 
**	   Rev 1.24   09/20/94 12:44:08   rdunlap 
**	add HsmCardChange() 
**	add DOES_RX_BURST for HSM, DO_RX_BURST for NSM 
**	comment that NsmLogMessage is normally only available during driver init. 
**	 
**	   Rev 1.23   12 Sep 1994 17:44:28   F_DIMAMBRO 
**	Added dummy variable names to prototype declarations. 
**	(Required for H2INC, to work correctly.) 
**	 
**	   Rev 1.22   08/30/94 12:51:38   rdunlap 
**	added comments on the convention of not calling HsmSetRxMode, 
**	  HsmSetMediaType, and HsmSetMacAddr before calling HsmInitialize(). 
**	 
**	   Rev 1.21   08/26/94 13:55:12   croussel 
**	swapped FCSErrors and MsdPktErrors in NSSNAIS MIB structure 
**	 
**	   Rev 1.20   08/23/94 16:18:12   rdunlap 
**	add more description of HSM_NUM_CAM_ENTRIES 
**	 
**	   Rev 1.19   08/23/94 12:50:38   rdunlap 
**	changed spelling of "far" to "_FAR_" 
**	added _FAR_ to the lookahead data ptr in NsmRxLookahead 
**	 
**	   Rev 1.18   08/23/94 10:18:22   croussel 
**	put mediaTypes back in, added MAC_LOOPBACK to rxMode bits 
**	 
**	   Rev 1.17   08/17/94 12:01:32   rdunlap 
**	clean up dangling comment; 
**	change DRVDEBUG to NICDEBUG on KW_HSM_DEBUG. 
**	 
**	   Rev 1.16   08/16/94 14:56:00   croussel 
**	moved kwords_allowed, busTypes, and medias information to  
**	hsm.h preprocessor constants 
**	 
**	   Rev 1.15   08/04/94 16:38:48   rdunlap 
**	change HsmFrag.ptr to HsmFrag.fptr 
**	change NsmIOread/write formal parameter names from "addr" to "adr" 
**	  and from "ptr" to "dptr" (for h2inc/masm compatibility) 
**	change HsmSetMacAddr parameter "addr" to "maddr" (h2inc/masm) 
**	 
**	   Rev 1.14   08/03/94 14:41:08   croussel 
**	fix KW_RX_QsIZE (how did that get in there?) 
**	 
**	   Rev 1.13   08/03/94 10:41:28   croussel 
**	pktSize now called descByteCnt in HsmPktDesc 
**	 
**	   Rev 1.12   08/02/94 14:48:22   croussel 
**	expand pnpId to an array of four entries. 
**	remove maxFrags as it is already a preprocessor constant. 
**	remove maxMulticast, and maxBcast and make them preprocessor constants 
**	change name of HSM_HEAP_SIZE to HSM_CONTEXT_SIZE. 
**	 
**	   Rev 1.11   07/27/94 12:43:48   rdunlap 
**	Changed second NsmFree to NsmFreePhys. 
**	 
**	   Rev 1.10   07/27/94 10:13:04   khazzah 
**	Added function prototype for HsmShutdown 
**       
**         Rev 1.9   07/26/94 14:57:00   croussel 
**      fixed bug in comment 
**       
**         Rev 1.8   07/26/94 14:55:48   croussel 
**      lot's of changes as a result of 7-26 meeting 
**       
**         Rev 1.7   07/22/94 13:24:24   croussel 
**      HsmTimerEvent is now void, fixed some error codes 
**       
**         Rev 1.6   07/21/94 10:54:00   croussel 
**      added changes requested by Eric.   
**       
**         Rev 1.5   07/18/94 17:30:32   croussel 
**      miscellaneous little things ... 
**       
**         Rev 1.4   15 Jul 1994 15:47:06   croussel 
**      use 16-bit io addresses 
**       
**         Rev 1.3   15 Jul 1994 15:04:46   croussel 
**      added "far" to NsmIOxxxxmul declarations 
**       
**         Rev 1.2   07/15/94 14:56:58   croussel 
**      add some options bits at randy's reqest. 
**      added sendRetries 
**       
**         Rev 1.1   07/14/94 12:13:18   croussel 
**      freezing changes on july 14, changes to numerous to list 
**       
**         Rev 1.0   06/30/94 17:13:12   croussel 
**      Initial revision. 
** 
**      NIC's must provide (or stub out) the following functions, 
**      whose calling parameters and return codes are defined below: 
** 
**      int HsmInitContext( HsmContext * ) 
** 
**      int HsmBcastAddEntry( HsmContext *, int, uint32 ) 
** 
**      int HsmBcastDelEntry( HsmContext *, int, uint32 ) 
** 
**	int HsmCardChange (HsmContext *, int) 
** 
**      int HsmCheckNic( HsmContext * ) 
** 
**      int HsmClose( HsmContext * ) 
** 
**      int HsmDisableNicInts( HsmContext * ) 
** 
**      void  HsmEnableNicInts( HsmContext * ) 
** 
**      int HsmFindAdapter( HsmContext * ) 
** 
**	int HsmForceInterrupt( HsmContext * ); 
**
**      HsmPktDesc _FAR_ *HsmGetTxDescList( HsmContext *, int ) 
** 
**      int HsmInitialize( HsmContext * ) 
** 
**      int HsmMulticastLoad( HsmContext * ) 
** 
**      int HsmOpen( HsmContext * ) 
** 
**      int HsmReset( HsmContext * ) 
** 
**      int HsmRxCopyPkt( HsmContext *, void *, int, int, HsmPktDesc *, int * ) 
** 
**	int HsmRxFlowControl( HsmContext *, int ); 
**
**      int HsmService( HsmContext * ) 
** 
**      int HsmSetMacAddr( HsmContext *, MacAddr * ) 
** 
**      int HsmSetMediaType( HsmContext *, uint32 ) 
** 
**      int HsmSetRxLookahead( HsmContext *, int ) 
** 
**      int HsmSetRxMode( HsmContext *, int ) 
** 
**	int HsmShutdown(  HsmContext * ); 
**
**	void HsmTimerEvent( HsmContext * ); 
**
**      int HsmTransmit( HsmContext *, HsmPktDesc _FAR_ * ) 
** 
**      void HsmUpdateStatistics( HsmContext * ) 
** 
**      int HsmValidateContext( HsmContext * );
**
**      void HsmResetStatistics( HsmContext *, int );
**
**      void HsmStrobeStatistics( HsmContext *, int );
** 
**      void HsmFreezeStatistics( HsmContext *, int );
**
**	int HsmGetMibStatus( HsmContext * );
**
**      int HsmZeroStatistics( HsmContext * ) 
** 
** -------------------------------------------------------------------
**
**      The NIC specific code will expect that the 
**      environment will have provided the following functions ... 
** 
**      void NsmFree( HsmContext *, void _FAR_ * );
** 
**	void NsmFreePhys( HsmContext *, void _FAR_ *p ); 
** 
**      void NsmHsmEvent( HsmContext *, int ) 
** 
**      uint32 NsmLogicalToPhysical( HsmContext *, void _FAR_ * ) 
** 
**      void NsmLogMessage( HsmContext *, char * ) 
** 
**      void _FAR_ *NsmMalloc( HsmContext *, int ) 
** 
**      void _FAR_ *NsmMallocPhys( HsmContext *, int, uint32 _FAR_ * ) 
** 
**	int NsmRxComplete( HsmContext *, void *, int, int ); 
**
**      int NsmRxLookahead( HsmContext *, void *, uchar _FAR_ *, int, int, int,
**              HsmPktDesc * ); 
** 
**      void NsmTransmitComplete( HsmContext *, uint32, int ) 
** 
**      uint32 NsmIOread32( uint16 ) 
** 
**      uint16 NsmIOread16( uint16 ) 
** 
**      uchar NsmIOread8( uint16 ) 
** 
**      void NsmIOread32mul( uint16, uint32 _FAR_ *, int ) 
** 
**      void NsmIOread16mul( uint16, uint16 _FAR_ *, int ) 
** 
**      void NsmIOread8mul( uint16, uchar _FAR_ *, int ) 
** 
**      void NsmIOwrite32( uint16, uint32 ) 
** 
**      void NsmIOwrite16( uint16, uint16 ) 
** 
**      void NsmIOwrite8( uint16, uchar ) 
**       
**      void NsmIOwrite32mul( uint16, uint32 _FAR_ *, int ) 
** 
**      void NsmIOwrite16mul( uint16, uint16 _FAR_ *, int ) 
** 
**      void NsmIOwrite8mul( uint16, uchar _FAR_ *, int ) 
**       
**	void NsmEnterCriticalSection( HsmContext *, int );
**
**	void NsmLeaveCriticalSection( HsmContext *, int ); 
**
**	void NsmStdMessage( HsmContext *, int ); 
**
**	void NsmStartTimer( HsmContext *, int ); 
**
**	void NsmStopTimer( HsmContext *); 
**
**	void NsmWaitTime( HsmContext *, int ); 
**
**	uint32 NsmGetTime( HsmContext *Context ); 
**
**	void NsmDbugMessage( HsmContext *hsm, char *msg ); 
**       
**      int NsmCallCss( uchar , uint16 _FAR_ *, uchar *, int, void _FAR_ * );
**
*/ 
#ifndef  _cdi_h_ 
#define  _cdi_h_        1 
 
#ifndef  _nsctypes_h_ 
#include "nsctypes.h" 
#endif 
 
#ifndef  _macaddr_h_ 
#include "macaddr.h" 
#endif 
 
/* 
**	Nsm's expect the following to be defined in hsm.h! 
** 
**	Hsm developers should define these C preprocessor variables  
**	to the appropriate values for their Hsms,  Nsm developers  
**	will expect these constants to be set! 
*/ 
 
/*	HSM_CONTEXT_SIZE	set by the HSM in hsm.h to the size of  
**				HsmContext plus the size of any additional 
**				variables added to the end for the  
**				exclusive use of the HSM. 
*/ 
/* #define HSM_CONTEXT_SIZE   (sizeof( HsmContext )) */ 
 
/*	HSM_HEAP_SIZE		set by the HSM in hsm.h to the max size of  
**				heap that might be needed by the HSM.  The
**				actual heap size that is used will depend
**				upon driver configuration (i.e. speed, 
**				queue sizes, etc.  NSMs that support
**				dynamic memory allocation can ignore this.
**				This field is mainly for the use of NSMs that
**				must preallocate a heap (i.e. odi16).
*/ 
/* #define HSM_HEAP_SIZE   (2048) */ 
 
/* 
**	HSM_MAX_TX_FRAGS	set by the HSM in hsm.h to the maximum  
**				number of fragments in the descriptors 
**				returned by HsmGetTxDescList(). 
*/ 
/* #define HSM_MAX_TX_FRAGS        8 */ 
 
/* 
**	HSM_FAMILY_NAME		set by the HSM in hsm.h to the name of 
**				the product family.  This string is only 
**				useful for displayed messages. 
**				This size of this string should be less 
**				than or equal to 16 characters. 
*/ 
/* #define HSM_FAMILY_NAME	"InfoMover" */ 
 
/* 
**	HSM_HSM_NAME		set by the HSM in hsm.h to the name of 
**				the adapter which the HSM applies to. 
**				This string is only useful for displayed 
**				messages. 
**				This size of this string should be less 
**				than or equal to 16 characters. 
*/ 
/* #define HSM_HSM_NAME		"10/100 XXXX" */ 
 
/* 
**	HSM_SHORT_NAME		set by the HSM in hsm.h to the "short" name  
**				of the adapter which the HSM applies to. 
**				This string is useful for the visible driver 
**				name (as in protocol.ini, or net.cfg). 
**				This size of this string should be less 
**				than or equal to 6 characters. 
*/ 
/* #define HSM_SHORT_NAME	"NEXXXX" */ 
 
 
/* 
**	HSM_NUM_CAM_ENTRIES 	set by the HSM in hsm.h to the number of  
**				perfectly filtered multicast CAM entries. 
**				HSM's that do not have an on-board CAM 
**				(i.e. NE2000) will set this variable to 0. 
**				NSMs can use HSM_NUM_CAM_ENTRIES to 
**				determine the size of their multicast table, 
**				although it may be 0, so they should use  
**				a default value. 
**				This HSM constant (if non-zero) should 
**				already be reduced by one to allow for 
**				the node's physical network address in 
**				the NIC's CAM. 
*/ 
/* #define HSM_NUM_CAM_ENTRIES	0 */ 
 
/* 
**	HSM_NUM_BCAST_TBL_ENTRIES 	 
**				set by the HSM in hsm.h to the number of  
**				broadcast filter table entries. 
**				HSM's that do not have an on-board broad- 
**				cast filter table (i.e. NE2000) will set  
**				this variable to 0. 
*/ 
/* #define HSM_NUM_BCAST_TBL_ENTRIES	0 */ 
 
/* 
**	HSM_NEED_TX_PHYSADDRS	set by the HSM when it requires the NSM 
**				to specify physical (as opposed to logical) 
**				fragment addresses in the HsmPktDesc 
**				structure that is passed to HsmTransmit. 
**				This is used by bus mastering adapters 
**				which can transmit directly from upper layer 
**				buffers. 
*/ 
/* #define HSM_NEED_TX_PHYSADDRS	0 */ 
 
/* 
**	HSM_NEED_RX_PHYSADDRS	set by the HSM when it requires the NSM 
**				to specify physical (as opposed to logical) 
**				fragment addresses in the HsmPktDesc 
**				structure that is passed to HsmTransmit. 
**				This is used by bus mastering adapters 
**				which can receive directly into upper layer 
**				buffers. 
*/ 
/* #define HSM_NEED_RX_PHYSADDRS	0 */ 
 
/* 
**	HSM_KEYWORDS		set by the HSM in hsm.h to a bit mask  
**				contructed of the following bits.  This 
**				bit mask specifies to the NSM at compile 
**				time which keywords are supported by this 
**				Hsm. 
** 
** 	The actual text for the keyword is specified in the comment below 
**	as follows: 
** 
**	"KEYWORD" 	or if they are predefined for ODI and NDIS2 
** 
**	"ODI_KEYWORD"/"NDIS2_KEYWORD" 
*/ 
/* #define HSM_KEYWORDS	0 */ 
					 
#define KW_IO_ADDR        	0x00000001      /* "PORT"/"IOBASE"  
						** specifies an IO address */ 
 
#define KW_RAM_ADDR       	0x00000002      /* "MEM"/"RAMADDRESS"  
						** specifies a memory address 
						** for shared RAM adapters */ 
 
#define KW_IRQ            	0x00000004      /* "INT"/"INTERRUPT"  
						** specifies an IRQ line */ 
 
#define KW_SLOT           	0x00000008      /* "SLOT"  
						** specifies a slot location  
						** for EISA, MC, PCMCIA, and  
						** PCI busses */ 
 
#define KW_NET_ADDR_OVR   	0x00000010      /* "NODE ADDRESS"/"NETADDR"  
						** overrides the unique MAC 
						** address of the adapter */ 
 
#define KW_SERIAL  	  	0x00000020      /* "SERIAL"  
						** specifies a uniqueId for 
						** use in adapter location */ 
 
#define KW_USE_CSS        	0x00000040      /* "PCMCIA" 
						** use card & socket services */ 
 
#define KW_BUS_SIZE       	0x00000080      /* "BUSSIZE"  
						** specifies the width of  
						** the data bus to the card */ 
 
#define KW_TX_QSIZE       	0x00000100	/* "TXQUEUE" 
						** specifies the requested  
						** depth of the transmit q */ 
 
#define KW_RX_QSIZE       	0x00000200	/* "RXQUEUE" 
						** specifies the requested  
						** depth of the receive q */ 
 
 
#define	KW_TX_BURST		0x00000400	/* "TXBURSTCOUNT" & 
						** "TXBURSTTIMEOUT"  
						** specify parameters for  
						** transmit interrupt  
						** hysteresis */ 
 
#define	KW_RX_BURST		0x00000800	/* "RXBURSTCOUNT" & 
						** "RXBURSTTIMEOUT"  
						** specify parameters for  
						** receive interrupt  
						** hysteresis */ 
 
#define	KW_RX_EARLY		0x00001000	/* "RXEARLYSIZE" 
						** specify number of bytes 
						** for early interrput  
						** generation */ 
 
#define KW_MEDIA_TYPE     	0x00002000	/* "MEDIATYPE" 
						** support keyword selection 
						** of media types */ 
 
#define KW_BCAST_FILTER   	0x00004000	/* "BROADCASTFILTER type=pattern 
						** specify a broadcast filter */ 
 
#define KW_NSM_DEBUG          	0x00008000	/* "DRVDEBUG" 
						** enable output of NSM debug 
						** messages */ 
 
#define KW_HSM_DEBUG          	0x00010000	/* "NICDEBUG" 
						** enable output of HSM debug 
						** messages */ 
 
#define	KW_SOFTCRC		0x00020000	/* "SOFTCRC"
						** force CRC checking in SW */

#define	KW_NO32BITIO		0x00040000	/* "NO32BITIO"
						** force HSM to use 16-bit IO */

/* these bit positions are reseved for NSM specific applications */ 
#define KW_NSM_RESERVED0  	0x10000000	 
#define KW_NSM_RESERVED1  	0x20000000 
#define KW_NSM_RESERVED2  	0x40000000 
#define KW_NSM_RESERVED3  	0x80000000 
 
/* 
**      HSM_BUS_TYPES           set by the HSM in hsm.h to specify which 
**                              busses this adapter will function in. 
**                              Constructed from a logical-OR of the following 
**                              bit definitions: 
*/ 
/* #define HSM_BUS_TYPES	0 */ 
#define HSM_ISA     0x01        /* adapter works in an ISA bus */ 
#define HSM_EISA    0x02        /* adapter works in an EISA bus */ 
#define HSM_MC      0x04        /* adapter works in a MC bus */ 
#define HSM_PCMCIA  0x08        /* adapter works in a PCMCIA bus */ 
#define HSM_PCI     0x10        /* adapter works in a PCI bus */ 
 

/*	HSM_CAN_SHARE_INTS	set by the HSM to indicate to the NSM
**				that the adapter supported by the HSM
**				is capable of sharing interrupts.
*/
/* #define HSM_CAN_SHARE_INTS	1 */


/*	HSM_DOES_RX_MULTICOPIES	set by the HSM to indicate to the NSM
**				that the HSM supports multiple calls to 
**				HsmRxCopyPkt
**				(same as <hsmOptions & DOES_RX_MULTICOPIES>
**				but can be used at compile-time
**				instead of run-time)
*/ 
/* #define	HSM_DOES_RX_MULTICOPIES	1 */
 

/* 
**	Hsm's expect the following to be defined in nsm.h! 
** 
**	Nsm developers should define these C preprocessor variables  
**	to the appropriate values for their Nsms,  Hsm developers  
**	will expect these constants to be set! 
*/ 

/*	NSM_DOUBLE_BUFFER	set by the NSM to instruct a bus-mastering
**				HSM to double buffer (copy data in and out
**				of a preallocated local buffer) before 
**				transfers.  NSM_DOUBLE_BUFFER will override
**				the HSM requests for physical address in 
**				descriptors (HSM_NEED_?X_PHYSADDRS).
**				NSM's that set NSM_DOUBLE_BUFFER can safely
**				ignore HSM_NEED_?X_PHYSADDRS.
*/
/* #define NSM_DOUBLE_BUFFER    0 */ 
 
/*	NSM_MAX_LOOKAHEAD_SIZE	set by the NSM to the largest receive 
**				lookahead size that will ever be set by 
**				the NSM using HsmSetRxLookahead(). 
*/ 
/* #define NSM_MAX_LOOKAHEAD_SIZE 1514 */ 
 
/* 
**	NSM_MAX_TX_FRAGS	set by the NSM in nsm.h to the maximum  
**				number of fragments ever used by the NSM 
**				in the descriptors returned by  
**				HsmGetTxDescList(). 
*/ 
/* #define NSM_MAX_TX_FRAGS        8 */ 
 
/* 
**      An entry in the multicast table (pointed to in the context) 
*/ 
 
typedef struct HsmMulticastTableEntry { 
    MacAddr     macAddr; 
    uint16      useCount;       /* only load this entry if != 0 */ 
} HsmMulticastTableEntry, far *FPHsmMulticastTableEntry; 
 
 
/* 
**      All NICs must maintain the following structure. 
**      The NSSNAIS statistics structure required for using 
**      NSC's SNMP node agent is embedded within.   
*/ 
 
typedef struct HsmContext {             /* (field owner is in parens) */ 
    uint16      hsmIFversion;           /* (hsm) hsm IF version */ 
#define HSM_IF_VERSION  0x0100          /* current HSM interface version */ 
    uint16      nsmIFversion;           /* (nsm) nsm IF version */ 
#define NSM_IF_VERSION  0x0100          /* current NSM interface version */ 
    uint32      nsmContext;             /* (nsm) ptr to Nsm data, not touched by
					** Hsm, not required */ 
    uint32      hsmOptions;             /* (hsm) hsm IF options */ 
#define NEED_TX_PHYSADDRS       0x0001  /* need Phys addresses in fragment  
					** pointers of Tx Descriptors */ 
#define NEED_RX_PHYSADDRS       0x0002  /* need Phys addresses in fragment 
					** pointers of Rx Descriptors */ 
#define DOES_PROMISCUOUS        0x0004  /* capable of doing promiscuous */ 
#define DOES_BCAST_FILTERING    0x0008  /* has broadcast filtering support */ 
#define DOES_MCAST_CAM          0x0010  /* has an onboard CAM for multicast */ 
#define NEED_TIMER              0x0020  /* Hsm needs a timer */ 
#define DOES_RX_PIPELINING      0x0040  /* Hsm supports receive pipelining */ 
#define DOES_RX_MULTICOPIES     0x0080  /* Hsm supports multiple calls to 
					** HsmRxCopyPkt */ 
#define DOES_BUS_MASTERING      0x0100  /* Hsm does Bus Mastering */ 
#define CAN_SHARE_IRQ           0x0200  /* Hsm can share his interrupt */ 
#define CAN_DO_RX_PKTDESC       0x0400  /* Hsm can pass HsmPktDesc to 
					** NsmRxLookahead */ 
#define	DOES_RX_BURST		0x0800	/* Hsm supports Rx Burst mode */ 
#define	DOES_SOFT_CRC		0x1000	/* Hsm supports soft crc checking */
 
    uint32      nsmOptions;     /* (nsm) nsm IF options */ 
#define DO_DEBUG                0x0001  /* Nsm tells Hsm to do debug msgs */ 
#define	DONT_USE_32BIT_IO	0x0002	/* don't use 32-bit ISA IO */
#define DO_RX_PIPELINING        0x0040  /* Nsm supports receive pipelining */ 
#define DO_MAX_PERFORMANCE	0x0080	/* Nsm would like the Hsm to maximize
					** performance at the expense of CPU
					** utilization */
#define DO_RX_PKTDESC           0x0400  /* Nsm tells Hsm to pass HsmPktDesc 
					** to NsmRxLookahead */ 
#define DO_RX_BURST		0x0800	/* Nsm tells Hsm that Rx Burst is ok */ 
#define	DO_SOFT_CRC		0x1000	/* Nsm tells Hsm to perform crc chk of
					** incoming packets in software */
 
    uchar       reqTxQSize;     /* (nsm) requested tx Queue Size */ 
				/* set to 0 for default */ 
 
    uchar       txQSize;        /* (hsm) actual tx Queue Size */ 
 
    uchar       reqRxQSize;     /* (nsm) requested rx Queue Size */ 
				/* set to 0 for default */ 
 
    uchar       rxQSize;        /* (hsm) actual rx Queue Size */ 
 
    uchar       txQFree;        /* (hsm) num of Tx HsmPktDesc  
				** currently available */ 
 
    uchar       hsmState;       /* (hsm) current HsmState */ 
#define HSM_UNINITIALIZED_STATE 0       /* Hsm not yet initialized */ 
#define HSM_INITIALIZED_STATE   1       /* initialized but not open */ 
#define HSM_RESET_STATE         2       /* hw being reset */ 
#define HSM_OPEN_STATE          3       /* open and ready for IO */ 
#define HSM_CLOSED_STATE        4       /* closed, no IO, rdy for open */ 
#define HSM_DEAD_STATE          5       /* hardware failure */ 
 
    uint16      maxPacketSize;  /* (hsm) max packet size (excluding any CRC) */ 
 
    uint16      pageSize;       /* (nsm) OS page size in bytes, 
				** 0 = hsm doesn't need to worry about 
				** page boundaries */ 
 
    uint16      rxLookaheadSize;/* current min rx lookahead size */ 
 
    uint32      pnpId[4];       /* bus type ID (for Plug and Play discovery) */ 
				/* non-PnP Hsm's must set to 0 */ 
 
    uint32	mediaTypes;	/* specifies which media are supported by 
				** this adapter. This may not be valid until 
				** after HsmInitialize */ 
#define HSM_MEDIA_AUTO_CONFIG   0x01    /* HSM supports Auto Media Selection */ 
#define HSM_MEDIA_10AUI         0x02    /* HSM supports AUI connections 0 */ 
#define HSM_MEDIA_10BASE2       0x04    /* HSM supports 10base2 connection */ 
#define HSM_MEDIA_10BASET       0x08    /* HSM supports 10baseT connection */ 
#define HSM_MEDIA_10BASETfx     0x10    /* HSM supports 10baseT full duplex */ 
#define HSM_MEDIA_100BASEX      0x20    /* HSM supports 100baseX connection */ 
#define HSM_MEDIA_100BASEXfx    0x40    /* HSM supports 100baseX full duplex */ 
#define HSM_MEDIA_100BASETT4	0x80	/* HSM supports 100baseT-T4 */
#define HSM_MEDIA_EXT_MII    	0x100   /* Use external MII AUTO */ 
#define HSM_MEDIA_AUTO_TP    	0x200   /* HSM supports AUTO on TP (RJ45) */

/* Euphrates SVB card has 2 UTP ports ... */
#define HSM_MEDIA_10BASET_ALT  	0x0800	/* Use alternate 10BASET port */
#define HSM_MEDIA_10BASETfx_ALT 0x1000	/* Use alternate 10BASET port (fdx) */
#define	HSM_MEDIA_TYPE_MAX	0x1000	/* update to max HSM_MEDIA_type value */
  
    uint32	logicalMemAddr;	/* logical address of the shared memory  
				** used only by shared memory adapters */
 
    uint16      rxMode;         /* (hsm) current receive Mode (bitmask) ... */ 
#define ACCEPT_ALL_PHYS         0x01    /* accept all physical addresses */ 
#define ACCEPT_ALL_MCASTS       0x02    /* accept all multicast addresses  */ 
#define ACCEPT_ALL_BCASTS       0x04    /* accept all broadcast packets */ 
#define ACCEPT_ALL_ERRORS       0x08    /* accept errored rx pkts */ 
#define ACCEPT_CAM_QUALIFIED    0x10    /* accept my unicast packets  */ 
#define MAC_LOOPBACK    	0x20    /* set MAC in loopback  
					** for testing purposes only */  
 
    uint16      sendRetries;    /* (nsm) how many time to retry on 
				** send errors */ 
 
    uint32	uniqueId;	/* (nsm) used for specific card matching */ 
    uint16	rxEarlySize;	/* (hsm) received byte size needed before 
				** an early Rx interrupt is generated */ 
    uint16	rxBurstCnt;	/* (hsm) number of consecutive packets 
				** successfully received before an RxOK 
				** interrupt is generated */ 
    uint16	rxBurstTimeout;	/* (hsm) number of microseconds of idle time  
				** to wait before generating an RxOK  
				** interrupt */ 
    uint16	txBurstCnt;	/* (hsm) number of successful consecutive 
				** transmits before a TxOK interrupt 
				** is generated */ 
    uint16	txBurstTimeout;	/* (hsm) number of microseconds of idle time 
				** to wait before generating a TxOK interrupt */ 
 
    uint16      ClientHandle;   /* handle used by Pcmcia client for 
				** communication with CSS */
    uchar       CardServPres;   /* indicates presence or absence of CSS
				** Pcmcia client driver */
    uchar       CardExists;     /* indicates whether or not our Pcmcia 
				** card is inserted  */
    uchar	pad0[2];		/* for 4-byte alignment */
 
    /* stats required by NSC's SNMP/DMI agent (set by hsm/read by nsm) 
    ** any changes must be negotiated with SNMP agent team      */ 
    uchar       Interfacename[8];       /* (nsm) "SNMPSTK$" */ 
    uchar	StatBufMajVer;		/* (nsm) stat buffer major ver */ 
    uchar	StatBufMinVer;		/* (nsm) stat buffer major ver */ 
    uint16	StatBufSize;		/* (nsm) sizeof stat buffer */ 
    uchar       DriverName[32];         /* (nsm) */ 
    uchar       DriverVersion[4];       /* (nsm) */ 
    uint32      DriverSize;             /* (nsm) */ 
    uint16	DriverIFType;		/* (nsm) */ 
#define		IF_TYPE_OTHER		1 
#define		IF_TYPE_IPX		2 
#define		IF_TYPE_ODI		3 
#define		IF_TYPE_NDIS		4 
#define		IF_TYPE_PKTDRVR		5 
#define		IF_TYPE_LANTASTIC	6 
#define		IF_TYPE_UNIX		7 
#define		IF_TYPE_APPLETALK	8 
#define		IF_TYPE_IBMLSP		9 
#define		IF_TYPE_LLC		10 
#define		IF_TYPE_NETBIOS		11 
    uchar	DriverIFTypeSpec[6];	/* (nsm) version of DriverIFType */ 
    MacAddr     PermMacAddr;            /* (hsm) */ 
    MacAddr     CurrMacAddr;            /* (hsm) */ 
    uchar	BusNumber;		/* (nsm) PCI (or other) bus number */
    uchar	FunctionNumber;		/* (nsm) PCMCIA, EISA, or PCI
					** function number or channel */
    uint16	MulticastTableSize;	/* (nsm) the size of the multicast address table */ 
    FPHsmMulticastTableEntry MulticastTable;			 
					/* (nsm) multicast address table */ 
    uchar	FullDuplexCapable;	/* (hsm) */ 
#define		HALF_DUPLEX			1 
#define		FDX_CAPABLE_DUPLEX_UNKNOWN	2 
#define		FDX_CAPABLE_HALF_SELECTED	3 
#define		FDX_CAPABLE_FULL_SELECTED	4

    uchar	Topology;		/* (nsm) from mediaTypes */ 
#define		TOPO_OTHER		1 
#define		TOPO_10MBPS_ETHERNET	2 
#define		TOPO_100MBPS_ETHERNET	3 
#define		TOPO_10_100MBPS		4 
#define		TOPO_100MBPS_VG_ANYLAN	5 
#define		TOPO_4MBPS_TOKEN_RING	6 
#define		TOPO_16MBPS_TOKEN_RING	7 
#define		TOPO_4_16MBPS_TOKEN_RING 8 
#define		TOPO_2MBPS_ARCNET	9 
#define		TOPO_20MBPS_ARCNET	10 
#define		TOPO_FDDI		11 
#define		TOPO_ATM		12 
#define		TOPO_APPLETALK		13 

    uchar       MediaSpeed;             /* (hsm) */ 
#define         HW_SPEED_UNCONFIG       0 
#define         HW_SPEED_10_MBPS        10 
#define         HW_SPEED_100_MBPS       100 
#define         HW_SPEED_DEFAULT        (HW_SPEED_10_MBPS) 
 
    uchar       MediaType;              /* (hsm) */ 
#define         HW_CABLE_UNKNOWN        1 
#define         HW_CABLE_AUI            2 
#define         HW_CABLE_UTP_CAT3       3 	
#define         HW_CABLE_UTP_CAT4       4 
#define         HW_CABLE_UTP_CAT5       5 
#define         HW_CABLE_BNC            6 
#define         HW_CABLE_STP_RJ45       7 
#define         HW_CABLE_STP_DB9        8 
#define         HW_CABLE_FIBER_MIC      9 
#define         HW_CABLE_APPLE_AUI      10 
 
    uchar       SerialNumber[16];       /* (hsm) */ 
 
    /* the following hardware resource settings must be set by Nsm on 
    ** non-PnP systems, and are set by Hsm on PnP systems. */ 
    uint16      IOaddr;         /* (nsm) base of IO address range */ 
				/* (-1 == no default) */ 
 
    uint16      IOaddrSize;     /* (hsm) size of IO address range */ 
 
    uchar       Irq;            /* (nsm) current IRQ number assigned */ 
				/* (-1 == no default) */ 
 
    uchar       DMAChannel;     /* (nsm) (-1 == not assigned) */ 
				/* (-1 == no default) */ 
 
    uchar       BusWidth;       /* (hsm) */ 
#define HW_BUS_WIDTH_OTHER      1 
#define HW_BUS_WIDTH_UNKNOWN    2 
#define HW_BUS_WIDTH_8          3 
#define HW_BUS_WIDTH_16         4 
#define HW_BUS_WIDTH_32         5 
#define HW_BUS_WIDTH_64         6 
 
 
    uchar       Slot;           /* (nsm) for MC, EISA, PCMCIA and PCI busses,
				** the SLOT keyword
				** is used to map a config to an adapter */ 
 
    uint32      ROMaddr;        /* (hsm) base of ROM address range */ 
 
    uint32      ROMaddrSize;    /* (hsm) size of ROM address range */ 
 
    uchar       ROMrevision[4]; /* (hsm) ASCII representation of ROM revision */ 
				/* if ROM is present */ 
 
    uint32      MEMaddr;        /* (nsm) base of shared memory addr range */ 
				/* (-1 == no default) */ 
 
    uint32      MEMaddrSize;    /* (hsm) size of adapter's on-board memory */ 
				/* This is not necessarily Shared Memory. */
    uint16      OffsetUpdStats; /* (nsm) offset of update statistics */  
    uint16      SegmentUpdStats;/* (nsm) segment of update statistics */  
 
    uint16	MIIcontrolReg;	/* (hsm) contents of MII control register */ 
    uint16	MIIstatusReg;	/* (hsm) contents of MII status register */ 
 
    uchar	ANSIid[32];	/* (hsm) ANSI ID contents */ 
    uchar	LogicalDeviceId[7];	/* (hsm) logical device ID */ 
    uchar	pad2; 
    uchar	PnpVersion[3];	/* (hsm) PNP version number */ 
    uchar	LinkState; 	/* (hsm) current Link State */
#define LINK_DOWN	0	/* link is known to be down */
#define LINK_UP		1	/* link is known to be up */
#define	LINK_UNKNOWN	2	/* link state is unknown */

    uint32	PnpSerialNumber; /* PNP serial number */ 
    uint32	PnpVendorID; 	/* PNP vendor ID */ 
 
    /* the next group of statistics are handled by the MIB block 
    ** in Tigris and Euphrates, other NICs would have to do their 
    ** best in software */ 
    uint32      RxOctetsOK;     /* all counters maintained by hsm */ 
    uint32      RxPktsOK; 
    uint32      RxBroadcastPkts; 
    uint32      RxMulticastPkts; 
    uint32      RxPktErrors; 
    uint32      RxFCSErrors; 
    uint32      RxMissedPkts; 
    uint32      RxFAEErrors; 
    uint32	RxSymbolErrors; 
    uint32      RxFramesTooLong; 
    uint32      TxOctetsOK; 
    uint32      TxPktsOK; 
    uint32      TxDeferred; 
    uint32      TxBroadcastPkts; 
    uint32      TxMulticastPkts; 
    uint32      TxSingleCollision; 
    uint32      TxMultipleCollision; 
    uint32      TxPktErrors; 
    uint32      TxExcessiveCollision; 
    uint32      TxExcessiveDeferrals; 
    uint32      TxOWC; 
    uint32      TxSQE; 
    uint32      TxCarrierSenseLost; 
 
    uint32      TxNoResource; 
    uint32      TxPktQueue; 
 
    /* NIC specific private data placed here ... */ 
} HsmContext; 
 
 
 
/* 
**      calculate MAX_TX_FRAGS 
*/ 
#ifdef          HSM_MAX_TX_FRAGS 
#  ifdef        NSM_MAX_TX_FRAGS 
#    if         (HSM_MAX_TX_FRAGS<NSM_MAX_TX_FRAGS) 
#      define   MAX_TX_FRAGS    HSM_MAX_TX_FRAGS 
#    else 
#      define   MAX_TX_FRAGS    NSM_MAX_TX_FRAGS 
#    endif 
#  else 
#    define     MAX_TX_FRAGS    HSM_MAX_TX_FRAGS   
#  endif 
#else 
#  define	MAX_TX_FRAGS	8 
#endif 
 
/* 
**      A fragment, portion of a fragment contiguous 
**      in memory 
*/ 
 
typedef struct HsmFrag { 
    uint32      cnt;            /* byte count of fragment */ 
    uint32      fptr;           /* byte offset of fragment */ 
} HsmFrag, far *pHsmFrag; 
 
 
/* 
**      A packet, simply a collection of fragments. 
*/ 
typedef struct HsmPktDesc { 
    uint32      lLink;          /* logical ptr to next on list */ 
    uint32      handle;         /* optional "handle" for Nsm */ 
    uint32      pLink;          /* for use by hsm */ 
    uint32      descByteCnt;    /* descriptor size in bytes */ 
#define HSM_MORE        0x40000000      /* or in the MORE bit in all */ 
					/* but the last desc in a pkt */ 
//    HsmFrag     frags[MAX_TX_FRAGS];     
#ifndef  HSM_MAX_TX_FRAGS   /* define in hsm.h */
#define HSM_MAX_TX_FRAGS  4
#endif
    HsmFrag     frags[HSM_MAX_TX_FRAGS];     
} HsmPktDesc, far *pHsmPktDesc; 
 
/*
typedef HsmPktDesc _FAR_ *pHsmPktDesc; 
*/ 
 
 
 
 
/* 
** ------------------------------------------------------------------- 
** 
**      Hsm functions which are called from the driver specific module 
** 
*/ 
 
/* 
**      Valid Return Codes for HSM functions
*/ 
enum HsmReturnCodes { 
    HsmOK               = 0,    /* no error */ 

    HsmNotFound         = 1,    /* No NIC found matching requirements */ 

    HsmMultipleHsmsFound = 2,   /* no hsm specification, and multiple */ 
				/* NICs found */ 
 
    HsmInvConfig        = 3,    /* an invalid config option was specified to */ 
				/* HsmFindAdapters, or HsmInitialize */ 

    HsmCAMOverflow      = 4,    /* HsmMulticastLoad called with too many */ 

    HsmOutOfMemory      = 5,    /* call to NsmMalloc failed */ 

    HsmRxPktOverflow    = 6,    /* the HsmPktDesc passed to HsmRxCopyPkt */ 
				/* was not large enough */ 

    HsmHWFailure        = 7,    /* NIC HW looks dead */ 

    HsmBcastEntryNotFound = 8,  /* bcast filter table entry not found */ 

    HsmPacketNotReady	= 9,	/* early Rx packet is not all available */ 
				/* for HsmRxCopyPkt() [driver bug]	*/ 

    HsmInvalidParameter	= 10,	/* an invalid runtime configuration 
				** paramter was detected by 
				** HsmValidateContext */

    HsmNotImplemented   = -1    /* function called is not implemented */ 
}; 
 
/*
**	Valid return codes for NSM functions
*/
enum NsmReturnCodes { 
    NsmOK               = 0,    /* no error, for NsmRxLookahead, this 
				** implies that the packet was copied */ 

    NsmRxPktDiscard     = 1,    /* NsmRxLookahead says discard packet */ 

    NsmRxNotNow         = 2,    /* NsmRxLookahead can't accept packet now */ 

    NsmNotImplemented   = -1    /* function called is not implemented */ 
}; 
 
/* 
**      HsmInitContext( )       Initialize a Hsm Context structure 
*/ 
 
extern int HsmInitContext( HsmContext * ); 
 
 
/* 
**      HsmBcastAddEntry( )     Add an entry to the Hsm's broadcast 
**                              filter table. 
*/ 
 
extern int HsmBcastAddEntry(  
    HsmContext *Context,	/* pointer to hsm's context */ 
    int type,                   /* frame format type */ 
    uint32 pattern              /* protocol ID pattern */ 
); 
 
 
/* 
**      HsmBcastDelEntry( )     remove an entry from the broadcast filter table 
*/ 
 
extern int HsmBcastDelEntry(  
    HsmContext *Context,	/* pointer to hsm's context */ 
    int type,                   /* frame format type */ 
    uint32 pattern              /* protocol ID pattern */ 
); 
 
 
/* 
**      HsmCardChange( )        informs a PCMCIA HSM that the card 
**                              has just been reported as removed or 
**                              inserted. 
**				The actions that the HSM takes are 
**				not specified. 
**				HSMs that are not for PCMCIA should not 
**				implement this function. 
*/ 
 
extern int HsmCardChange( 
    HsmContext *Context,	/* pointer to hsm's context */ 
    int eventType		/* what happened to the card */ 
); 
 
enum CardEvents { 
    CardRemoved,		/* the PCMCIA card has been removed */ 
    CardInserted		/* the PCMCIA card has been inserted */ 
}; 
 
 
/* 
**      HsmCheckNic( )          verify that a NIC is available given 
**                              the current information (IOaddr, slot, etc.) 
**                              in the HsmContext.  This is called after  
**                              the Nsm locates the appropriate adapter 
**                              but before calling HsmInitialize. 
*/ 
 
extern int HsmCheckNic( 
    HsmContext *Context        /* pointer to hsm's context */ 
); 
 
 
/* 
**      HsmClose( )             shutdown a NIC.  No more data transfer  
**                              after calling HsmClose. 
*/ 
 
extern int HsmClose(  
    HsmContext *Context        /* pointer to hsm's context */ 
); 
 
 
/* 
**      HsmDisableNicInts( )    disable interrupts from this NIC. 
**                              return TRUE if NIC has interrupts 
**                              pending. 
*/ 
 
extern int HsmDisableNicInts(  
    HsmContext *Context        /* pointer to hsm's context */ 
); 
 
 
/* 
**      HsmEnableNicInts( )     enable interrupts from this NIC. 
*/ 
 
extern void HsmEnableNicInts(  
    HsmContext *Context        /* pointer to hsm's context */ 
); 
 
 
 
/* 
**      HsmFindAdapter( )       returns the configuration of the specified  
**                              NIC, including what (if any) options are  
**                              supported.  The config is returned in the 
**                              appropriate fields of the HsmContext described  
**                              above. This may return a value that indicates  
**                              that more configuration is needed (e.g.  
**                              nonPnP system, and no IOaddress is specified). 
*/ 
 
extern int HsmFindAdapter(  
    HsmContext *Context        /* pointer to hsm's context */ 
); 
 
 
 
/* 
**      HsmForceInterrupt( )    force an interrupt (if supported) 
*/ 
 
extern int HsmForceInterrupt(  
    HsmContext *Context        /* pointer to hsm's context */ 
); 
 
 
/* 
**      HsmGetTxDesc( )         returns a list of available transmit packet 
**                              descriptors with enough entries to store the 
**                              number of fragments passed, or NULL if not 
**                              enough are available.   
*/ 
 
/* extern HsmPktDesc _FAR_ *HsmGetTxDescList(  */
extern pHsmPktDesc HsmGetTxDescList(  
    HsmContext *Context,	/* pointer to hsm's context */ 
    int nfrags                  /* how many fragments are needed */ 
); 
 
 
/* 
**      HsmInitialize( )        initialize the hardware NIC.   
**				This function must NOT use NsmMalloc.
**				Configurable parameters must have already
**				been validated using HsmValidateContext.
*/ 
 
extern int HsmInitialize(  
    HsmContext *Context 
); 
 
 
/* 
**      HsmMulticastLoad( )     load the multicast address table  
*/ 
 
extern int HsmMulticastLoad(  
    HsmContext *Context		/* pointer to hsm's context */ 
); 
 
 
/* 
**      HsmOpen( )              prepares a NIC for data transfer.   
**                              options must already be set in HsmContext. 
*/ 
 
extern int HsmOpen(  
    HsmContext *Context        /* pointer to hsm's context */ 
); 
 
 
/* 
**      HsmReset( )             Put the NIC in a known state, ready to  
**                              receive and transmit.  CAM stays in tact. 
**                              State goes to "reset".  Must be called 
**				at Initialization time.
*/ 
 
extern int HsmReset(  
    HsmContext *Context        /* pointer to hsm's context */ 
); 
 
 
/* 
**      HsmRxCopyPkt( )         Copy the packet referred to by "handle" to  
**                              the buffers described in pktDesc.  Skip over  
**                              the number of bytes specified by "offset", and  
**                              copy only as many bytes as specified in "count".
**                              If the Hsm has set  
**                              HsmContext.nicOptions.doesRxMultiCopies  
**                              then this function may be called more than once  
**                              for each packet. 
**                              May return a code that indicates that not  
**                              enough data is currently available to satisfy  
**                              this copy. 
*/ 
 
extern int HsmRxCopyPkt(  
    HsmContext *Context ,	/* pointer to hsm's context */ 
    void *handle,               /* packet's handle (passed to NsmRxLookahead) */ 
    int offset,                 /* offset into packet to copy from */ 
    int count,                  /* size to copy */ 
    HsmPktDesc *pktDesc,        /* HsmPktDesc containing data buffers */ 
    int *status                 /* ptr to updated packet status */ 
); 
 
 
/* 
**      HsmRxFlowControl( )     start/stop the upcall of NsmRxLookahead  
**                              by some hardware dependent method.   
**                              (This could be done by masking off the 
**                              hardware receive interrupt). 
*/ 
extern int HsmRxFlowControl(  
    HsmContext *Context, 
    int stopFlow               /* set to stop, clr to start */ 
); 
 
 
/*   HsmRxFreePkt( )    ReceiveRelease. Receive buffer queued by protocol 
**                      is returned to MAC. 
**                      This is not for ReceiveLookahead.
*/
extern int HsmRxFreePkt(
    HsmContext *Context,
    int ReceiveHandle
);



/* 
**      HsmService( )           Process any current events that might be  
**                              causing an interrupt on the NIC.  Must expect  
**                              to be called with interrupts enabled.   
**                              Nsm must keep track of events and whether  
**                              or not any post-processing needs to happen 
**                              after the return of HsmService. 
*/ 
 
extern int HsmService(  
    HsmContext *Context        /* pointer to hsm's context */ 
); 
 
 
 
/* 
**      HsmSetMacAddr( )        set the NIC to receive packets addressed to  
**                              a particular physical MAC address 
**				This function should not be called before 
**				calling HsmInitialize(). 
*/ 
 
extern int HsmSetMacAddr(  
    HsmContext *Context,	/* pointer to hsm's context */ 
    MacAddr *maddr              /* new address to use */ 
); 
 
 
/* 
**      HsmSetMediaType( )      set the Hsm's media type selection 
**				This function should not be called before 
**				calling HsmInitialize(). 
*/ 
 
extern int HsmSetMediaType(  
    HsmContext *Context,	/* pointer to hsm's context */ 
    uint32 mediaType            /* new media type to select, this must */ 
				/* be set to a single bit as specified in  */ 
				/* HsmContext.mediaTypes. */ 
); 
 
 
/* 
**      HsmSetRxLookahead( )    Set the size of the receive lookahead space. 
**                              Nsm's that don't support lookahead may set 
**                              a lookahead size of 0. 
*/
 
extern int HsmSetRxLookahead(  
    HsmContext *Context,	/* pointer to hsm's context */ 
    int size                    /* minimum size that Hsm should have */ 
				/* available before calling NsmRxLookahead */ 
); 
 
 
/* 
**      HsmSetRxMode( )         set the receive mode which enables/disables  
**                              the reception of all-broadcast, all-multicast,  
**                              all-unicast packets, error pkts, or CAM  
**                              filtered packets 
**				This function should not be called before 
**				calling HsmInitialize(). 
*/ 
 
extern int HsmSetRxMode(  
    HsmContext *Context,	/* pointer to hsm's context */ 
    int mode                    /* set to new receive mode, as specified */ 
				/* in HsmContext.rxMode. */ 
); 
 
/* 
**      HsmShutdown( )          shutdown the NIC, and release memory  
*/ 
extern int HsmShutdown(  
    HsmContext *Context		/* pointer to hsm's context */ 
); 
 
/* 
**      HsmTransmit( )          Queue the completed HsmPkt transmit descriptor  
**                              for transmit. 
*/ 
 
extern int HsmTransmit(  
    HsmContext *Context,	/* pointer to hsm's context */ 
    pHsmPktDesc PktDesc 	/* pointer to structure containing packet */ 
				/* fragments */ 
); 
 
 
/* 
**      HsmTimerEvent( )        A timer previously set has expired.  Hsm's 
**                              should be aware that this may be called from 
**                              an interrupt context. 
*/ 
 
extern void HsmTimerEvent(  
    HsmContext *Context	       /* pointer to hsm's context */ 
); 
 
 
/* 
**      HsmUpdateStatistics( )  Update the statistics portion of the  
**                              HsmContext 
*/ 
 
extern void HsmUpdateStatistics(  
    HsmContext *Context	       /* pointer to hsm's context */ 
); 
 
/* 
**      HsmValidateContext( )	Validate configurable paramters in the
**				HsmContext.  The NSM calls this function
**				after parsing any command line or config
**				file options.  The HSM must validate those
**				values, and return an error code if they
**				are invalid.  This function MUST be called
**				by NSMs.  This is the last opportunity for
**				HSMs to allocate memory using NsmMalloc()
**				(and NsmMallocPhys()).
*/ 
extern int HsmValidateContext(  
    HsmContext *Context	       /* pointer to hsm's context */ 
); 
 
/*
**	HsmResetStatistics( )	Reset mib counters and HsmContext mib counters
*/
extern void HsmResetStatistics(
    HsmContext *Context,        /* pointer to hsm's context */
    int val
);

/*
**	HsmStrobeStatistics( )	Forces increment of mib counters
*/
extern void HsmStrobeStatistics(
    HsmContext *Context,        /* pointer to hsm's context */
    int val
);

/*
**	HsmFreezeStatistics( )	Enables or disables mib counters
*/
extern void HsmFreezeStatistics(
    HsmContext *Context,        /* pointer to hsm's context */
    int val
);

/*
**      HsmGetMibStatus( )  	Determines whether mib is started or stopped
*/
extern int HsmGetMibStatus(
    HsmContext *Context        /* pointer to hsm's context */
);

/* 
**      HsmZeroStatistics( )	Zero the statistics portion of the  
**                              HsmContext 
*/ 
extern void HsmZeroStatistics(  
    HsmContext *Context	       /* pointer to hsm's context */ 
); 
 
 
/* 
** ------------------------------------------------------------------- 
** 
**      Nsm functions which are called from the Hsm specific module 
** 
*/ 
 
/* 
**      NsmLogicalToPhysical( ) convert a logical address to a physical address.
**				The physical address returned should be 
**				a flat address (not segment:offset). 
*/ 
#ifndef NSM_LOGICAL_TO_PHYSICAL
/* extern uint32 NsmLogicalToPhysical( HsmContext *Context, void _FAR_ *logical ); */
extern uint32 NsmLogicalToPhysical( HsmContext *Context, PVOID logical ); 
#endif
 
/* 
**      NsmMalloc( )            allocate memory on behalf of the Hsm 
**                              module.  Can only be called during  
**                              HsmInitialize( ).  This memory MUST be  
**                              free'd if HsmShutdown is called.  Hsm's  
**                              should only use this for memory requirements  
**                              that may vary as a result of keyword option  
**                              settings (i.e. TxQSize, RxQSize). 
**                              Returns pointer to memory, or NULL if 
**                              no memory is available, memory returned by  
**                              NsmMalloc( ) must be 32-bit aligned. 
*/ 
 
/* extern  void _FAR_ *NsmMalloc( HsmContext *Context, int size );  */
extern  uint32 NsmMalloc( HsmContext *Context, int size ); 
 
 
/* 
**      NsmMallocPhys( )        similar to NsmMalloc, but requires the 
**                              Nsm to page align and lock memory into 
**                              place (prevent virtual memory swaps). 
**				Nsm's must also provide a "physical address"
**				for the start of the buffer.
** 
**                              Can only be called during  
**                              HsmInitialize( ).  This memory MUST be  
**                              free'd if HsmShutdown is called.  Hsm's  
**                              should only use this for memory requirements  
**                              that may vary as a result of keyword option  
**                              settings (i.e. TxQSize, RxQSize). 
**                              Returns pointer to memory, or NULL if 
**                              no memory is available, memory returned by  
**                              NsmMallocPhys( ) must be 32-bit aligned. 
*/ 
 
extern uint32 NsmMallocPhys( 
/*	HsmContext *Context, int size, uint32 _FAR_ *physAddr ); */
	HsmContext *Context, int size, PVOID physAddr ); 
 
/* 
**      NsmFree( )              free memory previously allocated using 
**                              NsmMalloc( ).  This should only be called 
**                              during HsmShutdown( ). 
*/ 
 
extern void NsmFree( HsmContext *Context, PVOID p ); 
 
/* 
**      NsmFreePhys( )          similar to NsmFree( ) but also unlocks 
**                              the region of memory.  Non virtual memory 
**                              Nsm's can safely implement this call by doing 
**                              a "NsmFree( p )". 
**                              This should only be called during  
**                              HsmShutdown( ).   
*/ 
 
extern void NsmFreePhys( HsmContext *Context, PVOID p ); 
 
/* 
**      NsmHsmEvent( )          Called from Hsm to notify Nsm of a NIC state 
**                              change.  This could be called from several 
**                              Hsm routines (i.e. HsmInitialize,  
**                              HsmOpen, HsmClose), or it may result from 
**                              an interrupt and be called from HsmService. 
**                              If the Hsm enters the HsmDead State, the Nsm 
**                              may wish take the appropriate action to  
**                              revive the hardware (i.e. invoke HsmReset, 
**                              HsmOpen, etc.). 
*/ 
extern void NsmHsmEvent(  
    HsmContext *Context,	/* pointer to hsm context */ 
    int event 
); 
 
enum HsmEvents { 
    HsmStateChange,             /* the Hsm has changed state, Nsm should 
				** read HsmContext.hsmState */ 
    HsmCarrierLoss,             /* the Hsm has lost transmit carrier */ 
    HsmCarrierPresent,          /* the Hsm has regained transmit carrier */ 
    HsmSpeedChange,             /* the Hsm has sensed a speed change */ 
				/* Nsm should read HsmContext.MediaSpeed */ 
    HsmMediaTypeChange          /* the Hsm has sensed a change in mediaType */ 
				/* Nsm should read HsmContext.MediaType */ 
}; 
 
 
/*
**      NsmCallCss( )           called by a PCMCIA HSM to interface
**                              with Card & Socket Services. The Nsm
**                              will make a call to CSS using the 
**                              parameters that the HSM passes in
**                              this function.
**                              HSMs that are not for PCMCIA should not
**                              ever call this function.
*/

extern int NsmCallCss(
        uchar    function,
        Puint16  handle, 
        uchar  *ptr, 
        int    arglength, 
        PVOID  argptr
);

 
/* 
**      NsmRxLookahead( )       Notify the driver of the arrival of a  
**                              received packet.  "data" points to the  
**                              start of the data, "len" is the size 
**                              of the lookahead area, "pktSize" is the  
**                              total size of the packet in bytes, if known.   
**                              For pipelined adapters this is set to -1.   
**                              "Status" is the error status of the packet,  
**                              if known.  If upper layer wants the packet,
**				Nsm should call HsmRxCopyPkt from this 
**				function to copy out contents of packet and
**				return NsmOK.  If upper layer doesn't want
**				the packet NsmRxLookahead should return
**				NsmRxPktDiscard to indicate that the 
**                              packet should be discarded.  Hsm should accept 
**				a call to HsmRxCopyData regardless of whether 
**				or not we are doing pipelining. 
*/ 
extern int NsmRxLookahead(  
    HsmContext *Context,	/* pointer to hsm context */ 
    void *handle,               /* handle to use to copy data */ 
    Puchar data,          /* pointer to start of lookahead data */ 
    int len,                    /* length of lookahead data */ 
    int pktSize,                /* size of packet  */ 
				/* if doing RxPipelining, set size to -1 */ 
    int status,                 /* error status of reception */ 
				/* if doing RxPipelining, set status to 0 */ 
#define HSM_RX_FAE_ERROR        0x01 
#define HSM_RX_CRC_ERROR       	0x02     
#define HSM_RX_CRC_EERROR       HSM_RX_CRC_ERROR	/* preserve typo */
#define HSM_RX_INVSYM           0x04 
#define HSM_RX_TOO_ESHORT       0x08 
#define HSM_RX_TOO_ELONG        0x10     
    HsmPktDesc *PktDesc        /* when hsm CAN_PASS_RX_PKTDESC and nsm says 
				** DO_PASS_RX_PKTDESC, then hsm will fill out 
				** an HsmPktDesc structure for receive packets 
				** and place here.  Otherwise, this field will 
				** be set to NULL */ 
); 
 
 
/* 
**      NsmRxComplete( )        Only called when doing receive pipelining 
**                              (see HsmContext.hsmOptions/nsmOptions). 
**                              NsmRxLookahead would have been called earlier 
**                              with the size set to -1. 
**                              This upcall from the driver indicates that 
**                              the receive packet is now available for  
**                              transfer. 
**				Returns NsmOK if the packet was copied
**				(HsmRxCopyPkt was called), else returns
**				NsmRxPktDiscard to toss the packet.
*/ 
extern int NsmRxComplete(  
    HsmContext *Context,	/* pointer to hsm context */ 
    void *handle,               /* handle to use to copy data */ 
    int pktSize,                /* final size of packet  */ 
    int status                  /* error status of reception */ 
); 
 
 
/*   NsmReceiveChain( )   Notify the protocol of the arrival of 
**                        a receive frame. Return NsmOK if the frame was
**                        added in protocol queued. HsmRxFreePkt will be
**                        called on this case.
**                        On the other cases, return NsmRxPktDiscard.
**                        (include SUCCESS copied.)
*/
extern int NsmReceiveChain(
    HsmContext *Context,
    int pktHandle,
    int pktSize,
    pHsmPktDesc pktDesc   /* this is receive data virtual address (^^;; */
);
 
 
/* 
**      NsmTransmitComplete( )  Notify the driver module of the completion  
**                              of a packet transmit.  Status indicates  
**                              success or failure.  Could be called from  
**                              HsmTransmit() or HsmService().  Nsm could  
**                              pass himself a "handle" in HsmPktDesc.handle. 
*/ 
 
extern void NsmTransmitComplete( 
    HsmContext *Context,	/* pointer to hsm context */ 
    uint32 handle,		/* from HsmPktDesc.handle of FIRST */ 
				/* pkt desc in packet */
    int status                  /* error status of transmission */ 
#define HSM_TX_ABORT		0x400	/* transmit abort */
#define HSM_TX_UNDERRUN		0x200	/* transmit underrun */
#define HSM_TX_NO_CAR        	0x100	/* carrier sense lost */ 
#define HSM_TX_DEF       	0x080 	/* transmit deferred */
#define HSM_TX_EXC_DEF          0x040 	/* excessive deferrals */
#define HSM_TX_OWC       	0x020	/* out of window collision */
#define HSM_TX_EXC_COLS	 	0x010 	/* excessive collisions */
#define HSM_TX_COLCNT		0x00f	/* collision count */
); 
 
 
/* 
**      NsmEnterCriticalSection( ) 
**                              Enter a critical section of code.  Mode  
**                              specifies what kind of critical section  
**                              you wish to protect.  This could be one 
**                              of three values: transmit, receive, all. 
** 
**				Calls to NsmEnterCriticalSection cannot 
**				be nested.  Calling NsmEnter.. a second 
**				time without calling NsmLeave... is almost 
**				guaranteed to lock you up. 
*/ 
#ifndef NSM_ENTER_CRITICAL_SECTION 
extern void NsmEnterCriticalSection(  
    HsmContext *Context,	/* pointer to hsm context */ 
    int kind  
#define HSM_TX_CRITICAL_SECTION         0 
#define HSM_RX_CRITICAL_SECTION         1 
#define HSM_ALL_CRITICAL_SECTION        2 
); 
#endif 
 
 
/* 
**      NsmLeaveCriticalSection( ) 
**                              Leave a critical section of code. 
*/ 
 
#ifndef NSM_LEAVE_CRITICAL_SECTION 
extern void NsmLeaveCriticalSection(  
    HsmContext *Context,	/* pointer to hsm context */ 
    int kind  
); 
#endif 
 
 
/* 
**      NsmStartTimer( )        Start the callback timer.  "Interval"  
**                              is expressed in ms.  If timer expires  
**                              without a call to NsmStopTimer( ), the 
**                              Nsm will call HsmTimerEvent( ).   
**                              HsmTimerEvent will only be called once. 
**                              For a repetitive timer callback, the Hsm 
**                              must restart the timer by calling  
**                              NsmStartTimer( ) again.  A second call 
**                              to NsmStartTimer( ) before timer expiration 
**                              restarts the timer. 
**                               
*/ 
extern void NsmStartTimer(  
    HsmContext *Context, 
    int interval  
); 
 
 
/* 
**      NsmStopTimer( )         Stop the callback timer. 
*/ 
 
extern void NsmStopTimer(  
    HsmContext *Context 
); 
 
 
/* 
**      NsmWaitTime( )		Pause execution for the specified number 
**				of milliseconds.  NsmWaitTime is preferred 
**				over any other method when performing  
**				fixed time delays from within Hsm's as 
**				this gives the Nsm the opportunity to  
**				share the CPU with the rest of the operating 
**				system. 
*/ 
#ifndef NSM_WAIT_TIME
extern void NsmWaitTime(  
    HsmContext *Context, 
    int interval  
); 
#endif
 
/* 
**      NsmGetTime( )           Return the current timer tick value 
**                              in ms. 
*/ 
#ifndef NSM_GET_TIME
extern uint32 NsmGetTime( HsmContext *Context ); 
#endif
 
/* 
**      NsmStdMessage( )        Write a standard error message to whatever  
**                              error logging mechanism the Nsm supports.   
**				Message numbers are defined in cdimsgs.h.
*/ 
 
extern void NsmStdMessage(  
    HsmContext *Context, 
    int		msgNum
); 
 
/* 
**      Return the 32-bit result of an IO read at "addr". 
*/ 
 
#ifndef NSM_IO_READ32 
extern uint32 NsmIOread32(  
    uint16 adr                 /* IO address to read from */ 
); 
#endif 
 
 
/* 
**      Return the 16-bit result of an IO read at "addr". 
*/ 
 
#ifndef NSM_IO_READ16 
extern uint16 NsmIOread16(  
    uint16 adr                 /* IO address to read from */ 
); 
#endif 
 
/* 
**      Return the 8-bit result of an IO read at "addr". 
*/ 
#ifndef NSM_IO_READ8 
extern uchar NsmIOread8(  
    uint16 adr                 /* IO address to read from */ 
); 
#endif 
 
 
/* 
**      Read multiple 82-bit values from an IO address 
*/ 
 
#ifndef NSM_IO_READ32MUL 
extern void NsmIOread32mul(  
    uint16 adr,                /* IO address to read from */ 
    Puint32 dptr,        /* pointer to buffer for read data */ 
    int cnt                    /* number of 32-bit values to read */ 
); 
#endif 
 
/* 
**      Read multiple 16-bit values from an IO address 
*/ 
 
#ifndef NSM_IO_READ16MUL 
extern void NsmIOread16mul(  
    uint16 adr,                /* IO address to read from */ 
    Puint16 dptr,        /* pointer to buffer for read data */ 
    int cnt                    /* number of 16-bit values to read */ 
); 
#endif 
 
/* 
**      Read multiple 8-bit values from an IO address
*/ 
 
#ifndef NSM_IO_READ8MUL 
extern void NsmIOread8mul(  
    uint16 adr,                /* IO address to read from */ 
    Puchar dptr,         /* pointer to buffer for read data */ 
    int cnt                    /* number of 8-bit values to read */ 
); 
#endif 
 
/* 
**      Write a 32-bit value to an IO address. 
*/ 
#ifndef NSM_IO_WRITE32 
extern void NsmIOwrite32(  
    uint16 adr,                 /* address to read from */ 
    uint32 value                /* value to write */ 
); 
#endif 
 
/* 
**      Write a 16-bit value to an IO address. 
*/ 
 
#ifndef NSM_IO_WRITE16 
extern void NsmIOwrite16(  
    uint16 adr,                 /* address to read from */ 
    uint16 value                /* value to write */ 
); 
#endif 
 
 
/* 
**      Write an 8-bit value to an IO address. 
*/ 
 
#ifndef NSM_IO_WRITE8 
extern void NsmIOwrite8(  
    uint16 adr,                 /* address to read from */ 
    uchar value                 /* value to write */ 
); 
#endif 
 
 
/* 
**      Write multiple 82-bit values to an IO address 
*/ 
 
#ifndef NSM_IO_WRITE32MUL 
extern void NsmIOwrite32mul(  
    uint16 adr,                /* IO address to write to */ 
    Puint32 dptr,        /* pointer to buffer for write data */ 
    int cnt                    /* number of 32-bit values to write */ 
); 
#endif 
 
 
/* 
**      Write multiple 16-bit values to an IO address 
*/ 
 
#ifndef NSM_IO_WRITE16MUL 
extern void NsmIOwrite16mul(  
    uint16 adr,                /* IO address to write to */ 
    Puint16 dptr,        /* pointer to buffer for write data */ 
    int cnt                    /* number of 16-bit values to write */ 
); 
#endif 
 
 
/* 
**      Write multiple 8-bit values to an IO address 
*/ 
 
#ifndef NSM_IO_WRITE8MUL 
extern void NsmIOwrite8mul(  
    uint16 adr,                /* IO address to write to */ 
    Puchar dptr,         /* pointer to buffer for write data */ 
    int cnt                    /* number of 8-bit values to write */ 
); 
#endif 
 
 
/* 
**	NsmDbugMessage( )	Write a debug message to whatever debug  
**				logging mechanism the Nsm supports.   
**				Hsm should use "\r\n" for line formatting. 
**				Hsm's must bracket any debug code with  
**				#ifdef HSM_DEBUG 
**				... 
**				#endif 
*/ 
#ifndef NSM_DBUG_MESSAGE 
extern void NsmDbugMessage(  
    HsmContext *Context, 
    char *msg 
); 
#endif 
  
#endif 

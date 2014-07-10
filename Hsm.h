/*
**	hsm.h - EuphPhyter implementation of "hsm.h"
**	
**	$Archive:   /tmp_mnt/n/crabapple/nsclib/proj/euphyter/sw/include/vcs/hsm.h_v  $
**	$Author: dkoch $
**	$Revision: 1.1 $
**
**	$Log: hsm.h,v $
 * Revision 1.1  1998/07/24  20:58:17  dkoch
 * Initial revision
 *
**	
**	   Rev 1.4   09/09/96 17:57:14   dkoch
**	change HSM_MAX_TX_FRAGS from 16 to 1
**	
**	   Rev 1.3   05/23/96 12:15:48   rdunlap
**	reduce HSM_CONTEXT_SIZE to conserve memory & satisfy Borland compiler;
**	
**	   Rev 1.2   05/21/96 16:46:20   rdunlap
**	change name strings from NI710X to NI720X (2 = EuphPhyter);
**	
**	   Rev 1.1   05/21/96 15:13:16   rdunlap
**	change OEM_BANNER from NI700X to NI710X;
**	
**	   Rev 1.0   11/17/95 15:08:32   croussel
**	Initial revision.
**	
*/
#ifndef	 _hsm_h_
#define  _hsm_h_        1

//#define HSM_CONTEXT_SIZE        ( 788 )   /* size of our context /700 */
//#define HSM_CONTEXT_SIZE        ( 788 + ((20-8)+(40-16))*4*2 )
#define HSM_CONTEXT_SIZE        ( 788 + ((2-8)+(2-16))*4*2 + 2*4)
#define HSM_HEAP_SIZE           2048      /* just descriptors */

//
// The smallest of HSM_MAX_TX_FRAGS and NSM_MAX_TX_FRAGS is used to
// determine MAX_TX_FRAGS which is used to specify the number of fragments
// allocated in a HsmPktDesc.  The HsmPktDesc is used to pass a packet
// description from the NSM to the HSM.  As the HsmPktDesc is not used
// to pass the description from the HSM to the hardware, it will benefit
// from the use of multiple fragments per descriptor even though the
// descriptors used by MacPhyter3v support only 1 fragment per descriptor.
// Hence, HSM_MAX_TX_FRAGS has been restored to 16 to match up with
// NSM_MAX_TX_FRAGS.
//
// change again from 16 to 1. Confuse how to match the fragments count
// between logical descriptor and physical one.
//#define HSM_MAX_TX_FRAGS        16
#define HSM_MAX_TX_FRAGS        4

#define HSM_MAX_RX_FRAGS        8

#define NSC_PNP_DEVICE          0x0070

#define OEM_BANNER              "DP83815 10/100 MacPhyter3v PCI Adapter"
#define OEM_HSM_NAME            "PCI"
#define OEM_SHORT_NAME          "TIGRIS"

#define HSM_NUM_CAM_ENTRIES     0
#define HSM_NUM_BCAST_TBL_ENTRIES 0

/* Infomover product names */
#define HSM_FAMILY_NAME         "MacPhyter3v"
#define HSM_HSM_NAME            "PCI"
#define HSM_SHORT_NAME          "DP83815"


#define	HSM_KEYWORDS	( KW_NET_ADDR_OVR + KW_TX_QSIZE + KW_RX_QSIZE \
	+ KW_SLOT + KW_MEDIA_TYPE )


#define HSM_BUS_TYPES		HSM_PCI

//#define HSM_NEED_TX_PHYSADDRS	1

#define HSM_CAN_SHARE_INTS	1
#endif

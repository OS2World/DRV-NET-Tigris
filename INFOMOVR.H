/*
**	infomovr.h - Infomover product line constants
**	
**	$Archive:   /tmp_mnt/n/crabapple/nsclib/tech/include/vcs/infomovr.h_v  $
**	$Author:   rdunlap  $
**	$Revision:   1.8  $
**
**	$Log:   /tmp_mnt/n/crabapple/nsclib/tech/include/vcs/infomovr.h_v  $
**	
**	   Rev 1.8   12/05/96 15:58:56   rdunlap
**	add PCI_SIS_VENDOR_ID	so that PCI_NSC_VENDOR_ID is still valid
**	  (allows building EUPHMON and ELMON without source changes here);
**	
**	   Rev 1.7   12/03/96 17:08:38   rdunlap
**	add SIS SVB device ID;
**	
**	   Rev 1.6   08/23/96 11:26:02   rdunlap
**	clarify National (NSC) and NEC PCI device IDs;
**	
**	   Rev 1.5   08/22/96 17:27:40   rdunlap
**	add EuphLite PCI device ID;
**	
**	   Rev 1.4   12/08/95 14:17:58   dkoch
**	add congo subsystem ID for SVB
**	
**	   Rev 1.3   11/21/95 09:16:16   dkoch
**	add IDs for congo
**	
**	   Rev 1.2   06/09/95 07:58:34   croussel
**	swap NI7101 and NI7100 in definitions of subsystem IDs
**	
**	   Rev 1.1   02/24/95 15:54:50   croussel
**	added NI7100_SVB
**	
**	   Rev 1.0   02/14/95 16:57:58   croussel
**	Initial revision.
**	
*/	
#ifndef	 _infomovr_h_
#define	 _infomovr_h_	1

/* PCI constants */

#define	PCI_NSC_VENDOR_ID	0x100B	/* NSC's PCI vendor ID */
#define	PCI_SIS_VENDOR_ID	0x1039	/* SIS's PCI vendor ID */
#define PCI_EUPH_DEVICE_ID      0x0001  /* PCI device ID assigned to euph */
/* wle//#define PCI_EUPHLITE_DEVICE_ID	0x000B	*/ /* NSC EuphLite PCI device ID */
#define PCI_EUPHLITE_DEVICE_ID	0x7006	/* SIS: EuphLite PCI device ID */
#define PCI_EUPHYTER_DEVICE_ID  0x0020  /* EuphPhyter device ID */
#define PCI_NSC_CONGO_DEVICE_ID	0x0014	/* PCI device ID assigned to congo */
#define CONGO_SVB		0x0015	/* Congo's subsystem SVB ID (internal use) */

#define	PCI_NEC_VENDOR_ID	0x1033	/* NEC's PCI vendor ID */
#define PCI_NEC_CONGO_DEVICE_ID	0x002B	/* PCI device ID assigned to congo */
#define PHYTER_SVB_DEVICE_ID	0x0016	/* phyter svb's subsystem id - wle */

/* 
**	PCI subsystem vendor ID assignments ...
**   	In the following, the suffixes mean ...
**		T = 10M twisted pair
**		X = 10/100M twisted pair
**		C = 10M coax
**		A = AUI (or mini)
**		M = 10/100M MII
*/
#define EUPHYTER        0x0020          /* Euphyter Demo board */
#define NI7100_SVB	0x0001		/* Euphrates SVB (internal use) */
#define SIS_7006_SVB	0x7006		/* SIS EuphLite SVB (internal use) */

#define NI7101_T	0x0003		/* 10 TP subsystem ID */
#define NI7101_CTA	0x0004		/* 10 TP/Coax/AUI subsystem ID */
#define NI7101_TM	0x0005		/* 10 TP/MII subsystem ID */
#define NI7100_XA	0x0006		/* 10/100 TP/AUI subsystem ID */
#define NI7100_X	0x0007		/* 10/100 TP subsystem ID */
#define NI7100_XM	0x0008		/* 10/100 TP/MII subsystem ID */
#define NI7100_CXA	0x0009		/* 10/100 TP/Coax/AUI subsystem ID */
#define PHYTER_NODE_SVB		0x000a		/* phyter node svb */
#define PHYTER_NODE_SVB_FIBER	0x000b		/* phyter node svb */
#define PHYTER_NODE_SVB_AUI	0x000c		/* phyter node svb */

#endif

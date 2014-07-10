/* 
**      tech/include/ieeemii.h 
** 
**      Definitions and Functions declarations for access to 
**	IEEE 802.3u/d4 MII compliant PHY devices
**      $Archive:   /nsclib/tech/include/vcs/ieeemii.h_v  $ 
**      $Author:   croussel  $ 
**      $Revision:   1.2  $ 
** 
**      $Log:   /nsclib/tech/include/vcs/ieeemii.h_v  $ 
**	
**	   Rev 1.2   02/28/95 09:29:32   croussel
**	extended definitions of ID registers
**	
**	   Rev 1.1   02/27/95 09:18:06   croussel
**	added NSC ID definitions
**	
**	   Rev 1.0   02/09/95 17:11:44   croussel
**	Initial revision.
*/
#ifndef	_ieeemii_h_
#define _ieeemii_h_ 1

/* MII register offsets */
#define		MII_CONTROL		0x0000
#define		MII_STATUS		0x0001
#define		MII_PHY_ID0		0x0002
#define		MII_PHY_ID1		0x0003
#define		MII_ANAR		0x0004
#define		MII_ANLPAR		0x0005
#define		MII_ANER		0x0006

/* MII Control register bit definitions. */
#define		MIICNTL_FDX		0x0100
#define		MIICNTL_RST_AUTO	0x0200
#define		MIICNTL_ISOLATE		0x0400
#define		MIICNTL_PWRDWN		0x0800
#define		MIICNTL_AUTO		0x1000
#define		MIICNTL_SPEED		0x2000
#define		MIICNTL_LPBK		0x4000
#define		MIICNTL_RESET		0x8000

/* MII Status register bit significance. */
#define		MIISTAT_EXT		0x0001
#define		MIISTAT_JAB		0x0002
#define		MIISTAT_LINK		0x0004
#define		MIISTAT_CAN_AUTO	0x0008
#define		MIISTAT_FAULT		0x0010
#define		MIISTAT_AUTO_DONE	0x0020
#define		MIISTAT_CAN_T		0x0800
#define		MIISTAT_CAN_T_FDX	0x1000
#define		MIISTAT_CAN_TX		0x2000
#define		MIISTAT_CAN_TX_FDX	0x4000
#define		MIISTAT_CAN_T4		0x8000

/* MII ID1 register bits */
#define		MII_ID1_OUI_LO		0xFC00	/* low bits of OUI mask */
#define		MII_ID1_MODEL		0x03F0	/* model number */
#define		MII_ID1_REV		0x000F	/* model number */

/* NSC's MII definitions */
#define		MII_ID0_NSC		0x2000	/* 08-00-17(21:6) */
#define		MII_ID1_NSC		0x5C00	/* 08-00-17(5:0) */
#define		MII_DP83840		0x0000	/* plexus model number */
#define		MII_DP83810		0x0010	/* euphrates model number */

/* MII NWAY Register Bits ...
** valid for the ANAR (Auto-Negotiation Advertisement) and
** ANLPAR (Auto-Negotiation Link Partner) registers */
#define		MII_NWAY_NODE_SEL	0x001f
#define		MII_NWAY_CSMA_CD	0x0001
#define		MII_NWAY_T		0x0020
#define		MII_NWAY_T_FDX		0x0040
#define		MII_NWAY_TX		0x0080
#define		MII_NWAY_TX_FDX		0x0100
#define		MII_NWAY_T4		0x0200
#define		MII_NWAY_RF		0x2000
#define		MII_NWAY_ACK		0x4000
#define		MII_NWAY_NP		0x8000

/* MII Auto-Negotiation Expansion Register Bits */
#define		MII_ANER_PDF		0x0010
#define		MII_ANER_LP_NP_ABLE	0x0008
#define		MII_ANER_NP_ABLE	0x0004
#define		MII_ANER_RX_PAGE	0x0002
#define		MII_ANER_LP_AN_ABLE	0x0001

#endif

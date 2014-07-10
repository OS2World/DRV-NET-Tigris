/*
**	macaddr.h	- Local Standard Data Types
**	
**	$Archive:   /nsclib/tech/include/vcs/macaddr.h_v  $ 	
**	$Author:   croussel  $
**	$Revision:   1.8  $	$Date:   11/07/94 15:35:24  $
**
**	$Log:   /nsclib/tech/include/vcs/macaddr.h_v  $
**	
**	   Rev 1.8   11/07/94 15:35:24   croussel
**	make dest first arg to MAC_ADDR_COPY
**	
**	   Rev 1.7   10/06/94 15:19:40   croussel
**	added MACADDR_IS_BCAST
**	
**	   Rev 1.6   08/29/94 16:43:40   croussel
**	added MacAddrIsNull, MAC_ADDR_IS_NULL
**	
**	   Rev 1.5   08/24/94 08:44:14   croussel
**	added macro versions of MAC_ADDR_EQ and MAC_ADDR_COPY (thanks robert)
**	
**	   Rev 1.4   08/04/94 16:38:24   rdunlap
**	change union name from addr to maddr (for h2inc/masm compatibility)
**	
**	   Rev 1.3   06/16/94 13:14:34   croussel
**	fixed definition of "words" (how embarassing)
**	
**	   Rev 1.2   06/13/94 16:49:14   croussel
**	MacAddrCopy should be a void
**	
**	   Rev 1.1   06/13/94 16:35:44   croussel
**	added template for MacAddrCopy( )
**	
**	   Rev 1.0   05/26/94 16:48:08   croussel
**	Initial revision.
**	
*/
#ifndef	 _macaddr_h_
#define	 _macaddr_h_	1

#ifndef	 _nsctypes_h_
#include "nsctypes.h"
#endif

/* ---- 48 bit MAC addresses ---- */
typedef struct MacAddr {
    union {
	uchar	bytes[6];
        uint16  words[3];
    } maddr;
} MacAddr;

extern bool MacAddrCompare( MacAddr *, MacAddr * );
extern bool MacAddrIsNull( MacAddr * );
extern void MacAddrCopy( MacAddr *, MacAddr * );

/* macro versions */

#define MAC_ADDR_EQ(m0, m1) \
   ((m0)->maddr.words[0] != (m1)->maddr.words[0] ? FALSE : \
    (m0)->maddr.words[1] != (m1)->maddr.words[1] ? FALSE : \
    (m0)->maddr.words[2] != (m1)->maddr.words[2] ? FALSE : TRUE)

#define MAC_ADDR_IS_NULL(m) \
   ( !(m)->maddr.words[0] && !(m)->maddr.words[1] && !(m)->maddr.words[2] )

#define MAC_ADDR_COPY(dm, sm) \
   {(dm)->maddr.words[0] = (sm)->maddr.words[0]; \
    (dm)->maddr.words[1] = (sm)->maddr.words[1]; \
    (dm)->maddr.words[2] = (sm)->maddr.words[2];}

#define MAC_ADDR_IS_BCAST(m) \
   ((m)->maddr.words[0] == 0xffff && \
    (m)->maddr.words[1] == 0xffff && \
    (m)->maddr.words[2] == 0xffff)

#endif

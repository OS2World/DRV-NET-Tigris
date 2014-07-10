/*
******************************************************************************
*									     *
*		National Semiconductor Company Confidential		     *
*									     *
*			   National Semiconductor			     *
*			NDIS 2.0.1 MAC device driver			     *
*	    Include file for National Semiconductor's CDI driver             *
*									     *
*      Source Name:    NSM.H						     *
*      Authors: 							     *
*			Frank Dimambro					     *
*									     *
*      $Log:   /home/crabapple/nsclib/tech/ndis2/include/vcs/nsm.h_v  $								     *
//*	
//*	   Rev 1.6   09/26/95 11:51:08   frd
//*	Added NSM_GET32 macro.
//*	
//*	   Rev 1.5   09/14/95 17:15:30   frd
//*	Made modification to Euphrastes
//*	
//*	   Rev 1.4   24 May 1995 09:28:02   FRD
//*
//*	
//*	   Rev 1.0   24 May 1995 09:16:20   FRD
//*	
//*	
//*	   Rev 1.3   16 Apr 1995 12:37:42   FRD
//*	Added NSM transmit drain threshold constant
//*	
//*	   Rev 1.2   12 Apr 1995 14:01:30   FRD
//*	
*									     *
******************************************************************************
*/
#ifndef	_nsm_h_
#define _nsm_h_	1

#ifndef  _nsctypes_h_
#include "nsctypes.h"
#endif

//#define NSM_DOUBLE_BUFFER       1
//#define NSM_MAX_LOOKAHEAD_SIZE  128
#define NSM_MAX_LOOKAHEAD_SIZE  256
#define NSM_TX_DRAIN_THRESHOLD	20
#define NSM_MAX_RX_FRAGS	16
//#define NSM_MAX_TX_FRAGS        16
#define NSM_MAX_TX_FRAGS        4

#define	NSM_MAJOR_VERSION	1
#define	NSM_MINOR_VERSION	0

#define  NSM_TXQUEUE_MIN	 1     // Minimum number of element in the
				       // transmit queue.
//#define  NSM_TXQUEUE_MAX         16    // Maximum number of element in the
//				       // transmit queue.
#define  NSM_TXQUEUE_MAX         40
#define  ASM_CRC32 1

#define NSM_SET32(_a_,_b_)			\
    {						\
	uint32 far *_p_ = (uint32 far *)&_a_;	\
	uint32 _v_ = _b_;			\
	_asm	push	es			\
	_asm	push	di			\
	_asm	_emit	0x66			\
	_asm	_emit	0x50			\
	_asm	les	di, _p_			\
	_asm	_emit	0x66			\
	_asm	mov	ax, word ptr _v_	\
	_asm	_emit	0x66			\
	_asm	mov	word ptr es:[di], ax	\
	_asm	_emit	0x66			\
	_asm	_emit	0x58			\
	_asm	pop	di			\
	_asm	pop	es			\
    }

#define NSM_GET32(_b_,_a_)			\
    {						\
	uint32 far *_p_ = (uint32 far *)&_a_;	\
	_asm	push	es			\
	_asm	push	di			\
	_asm	_emit	0x66			\
	_asm	_emit	0x50			\
	_asm	les	di, _p_			\
	_asm	_emit	0x66			\
	_asm	mov	ax, word ptr es:[di]	\
	_asm	_emit	0x66			\
	_asm	mov	word ptr _b_, ax	\
	_asm	_emit	0x66			\
	_asm	_emit	0x58			\
	_asm	pop	di			\
	_asm	pop	es			\
    }
#endif


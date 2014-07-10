;*****************************************************************************
;*              National Semiconductor Company Confidential                  *
;*                                                                           *
;*                          National Semiconductor                           *
;*                       NDIS 2.0.1 MAC device driver                        *
;*           Code for National Semiconductor's AT/LANTIC Board               *
;*                                                                           *
;*	Source Name:	NSMSTDIO.inc					     *
;*	Authors:							     *
;*                       Frank Dimambro                                      *
;*									     *
;*	$Log:   /home/crabapple/nsclib/tech/ndis2/include/vcs/nsmstdio.inv  $								     *
;	
;	   Rev 1.0   09/14/95 17:26:54   frd
;	Initial revision.
;*									     *
;*****************************************************************************

;*****************************************************************************
; Far-pointer, word, and double-word layout.
;*****************************************************************************
farptr	struct
 ofs	 dw	 ?		 ;Offset.
 sgm	 dw	 ?		 ;Segment or selector.
farptr  ends

dblbyte struct
 lb	 db	 ?		 ;Low byte of word.
 hb	 db	 ?		 ;High byte of word.
dblbyte ends

dblword struct
 lw	 dw	 ?		 ;Low word of doubleword.
 hw	 dw	 ?		 ;High word of doubleword.
dblword ends
;-----------------------------------------------------------------------------

IFNDEF NSMDEF_INC_
;*****************************************************************************
; Stack Structure for NSM calls.
;*****************************************************************************
Near_Params struct
 Ret_Addr    dw	    ?
 bp_ptr	     dw	    ?
Near_Params ends
;-----------------------------------------------------------------------------
ENDIF

;*****************************************************************************
; NsmIOread32 function stack paramaters Structure.
;*****************************************************************************
NsmIOread32_Params struct
 Near_Params	<>
 Near_Address	dw  ?
NsmIOread32_Params ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; NsmIOread16 function stack paramaters Structure.
;*****************************************************************************
NsmIOread16_Params struct
 Near_Params	<>
 Near_Address	dw  ?
NsmIOread16_Params ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; NsmIOread8 function stack paramaters Structure.
;*****************************************************************************
NsmIOread8_Params struct
 Near_Params	<>
 Near_Address	dw  ?
NsmIOread8_Params ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; NsmIOread32mul function stack paramaters Structure.
;*****************************************************************************
NsmIOread32mul_Params struct
 Near_Params	<>
 Near_Address	     dw		  ?
 Destination_Address farptr	 <>
 Dword_Count	     dw		  ?
NsmIOread32mul_Params ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; NsmIOread16mul function stack paramaters Structure.
;*****************************************************************************
NsmIOread16mul_Params struct
 Near_Params	<>
 Near_Address	     dw		  ?
 Destination_Address farptr	 <>
 Word_Count	     dw		  ?
NsmIOread16mul_Params ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; NsmIOread8mul function stack paramaters Structure.
;*****************************************************************************
NsmIOread8mul_Params struct
 Near_Params	<>
 Near_Address	     dw		  ?
 Destination_Address farptr	 <>
 Byte_Count	     dw		  ?
NsmIOread8mul_Params ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; NsmIOwrite32 function stack paramaters Structure.
;*****************************************************************************
NsmIOwrite32_Params struct
 Near_Params	<>
 Near_Address	dw  ?
 Value		dd  ?
NsmIOwrite32_Params ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; NsmIOwrite16 function stack paramaters Structure.
;*****************************************************************************
NsmIOwrite16_Params struct
 Near_Params	<>
 Near_Address	dw  ?
 Value		dw  ?
NsmIOwrite16_Params ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; NsmIOwrite8 function stack paramaters Structure.
;*****************************************************************************
NsmIOwrite8_Params struct
 Near_Params	<>
 Near_Address	dw  ?
 Value		db  ?
NsmIOwrite8_Params ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; NsmIOwrite32mul function stack paramaters Structure.
;*****************************************************************************
NsmIOwrite32mul_Params struct
 Near_Params	<>
 Near_Address	dw  ?
 Source_Address farptr	 <>
 Dword_Count	dw  ?
NsmIOwrite32mul_Params ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; NsmIOwrite16mul function stack paramaters Structure.
;*****************************************************************************
NsmIOwrite16mul_Params struct
 Near_Params	<>
 Near_Address	dw  ?
 Source_Address farptr	 <>
 Word_Count	dw  ?
NsmIOwrite16mul_Params ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; NsmIOwrite8mul function stack paramaters Structure.
;*****************************************************************************
NsmIOwrite8mul_Params struct
 Near_Params	<>
 Near_Address	dw  ?
 Source_Address farptr	 <>
 Byte_Count	dw  ?
NsmIOwrite8mul_Params ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; NSM_MEMZERO function stack paramaters Structure.
;*****************************************************************************
NSM_MEMZERO_Params struct
 Near_Params	<>
 Near_Address	dw  ?
 Block_Size	dw  ?
NSM_MEMZERO_Params ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; NSM_MEMCPY function stack paramaters Structure.
;*****************************************************************************
NSM_MEMCPY_Params struct
 Near_Params <>
 Destination_Address farptr	 <>
 Source_Address	     farptr	 <>
 Block_Size	     dw		  ?
NSM_MEMCPY_Params ends
;-----------------------------------------------------------------------------

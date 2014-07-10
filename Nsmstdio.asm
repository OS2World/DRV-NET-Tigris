;*****************************************************************************
;*              National Semiconductor Company Confidential                  *
;*                                                                           *
;*                          National Semiconductor                           *
;*                       NDIS 2.0.1 MAC device driver                        *
;*       Code for National Semiconductor's CDI resident driver portion.      *
;*                                                                           *
;*	Source Name:	NSMSTDIO.ASM					     *
;*      Authors:                                                             *
;*			 Frank DiMambro					     *
;*									     *
;*	$Log:   /home/crabapple/nsclib/tech/ndis2/nsmstdio/vcs/nsmstdio.asv  $								     *
;	
;	   Rev 1.0   09/14/95 17:34:34   frd
;	Initial revision.
;	
;	   Rev 1.3   16 Apr 1995 12:35:46   FRD
;	 
;	
;	   Rev 1.2   12 Apr 1995 13:49:00   FRD
;	 
;	
;	   Rev 1.1   12 Apr 1995 13:39:56   FRD
;	 
;*									     *
;*****************************************************************************
.seq
.386p
INCLUDE NSMSTDIO.inc

_TEXT	segment public use16 'CODE'	;DOS Driver contained in one segment.
		assume	cs:_TEXT
;*****************************************************************************
;
; "NsmIOread32"
;
;  uint32 NsmIOread32(uint32 addr1)
;
;     Read a 32-bit value from a 32 bit IO base Address. The return value
;     is stored in DX:AX, DX = High word, AX = Low word.
;
;*****************************************************************************
PUBLIC _NsmIOread32
_NsmIOread32:
           push bp
           mov  bp,sp
           mov  dx,[bp].NsmIOread32_Params.Near_Address
;RDG       in   ax,dx
;RDG       push ax
;RDG       add  dx,2
;RDG       in   ax,dx
;RDG       mov  dx,ax
;RDG       pop  ax
           in   eax, dx                 ;RDG
           mov  edx, eax                ;RDG
           shr  edx, 16                 ;RDG
           mov  sp,bp
           pop  bp
           ret
;-----------------------------------------------------------------------------

;*****************************************************************************
;
; "NsmIOread16"
;
;  uint16 NsmIOread16(uint32 addr1)
;
;     Read a 16-bit value from a 32 bit IO base Address. The return value
;     is stored in AX.
;
;*****************************************************************************
PUBLIC _NsmIOread16
_NsmIOread16:
	   push bp
	   mov	bp,sp
	   mov	dx,[bp].NsmIOread16_Params.Near_Address
	   in	ax,dx
	   mov	sp,bp
	   pop	bp
	   ret
;-----------------------------------------------------------------------------

;*****************************************************************************
;
; "NsmIOread8"
;
;  uint16 NsmIOread8(uint32 addr1)
;
;     Read a 8-bit value from a 32 bit IO base Address. The return value
;     is stored in Al.
;
;*****************************************************************************
PUBLIC _NsmIOread8
_NsmIOread8:
	   push bp
	   mov	bp,sp
	   mov	dx,[bp].NsmIOread8_Params.Near_Address
	   in	al,dx
	   mov	sp,bp
	   pop	bp
	   ret
;-----------------------------------------------------------------------------

;*****************************************************************************
;
; "NsmIOread32mul"
;
;  Read Multiple 32-bit words from IO Address 'Addr'
;
;*****************************************************************************
PUBLIC _NsmIOread32mul
_NsmIOread32mul:
	   push  bp
	   mov	 bp,sp
	   mov	 dx,[bp].NsmIOread32mul_Params.Near_Address
	   push  es
	   push  di
	   les	 di,[bp].NsmIOread32mul_Params.Destination_Address
	   mov	 cx,[bp].NsmIOread32mul_Params.Dword_Count
_NsmIOread32mul1:
;RDG       insw
;RDG       add   dx,2
;RDG       insw
;RDG       sub   dx,2
;RDG       loopw _NsmIOread32mul1
  rep      insd                         ;RDG
	   pop	 di
	   pop	 es
	   mov	 sp,bp
	   pop	 bp
	   ret
;-----------------------------------------------------------------------------

;*****************************************************************************
;
; "NsmIOread16mul"
;
;  Read Multiple 16-bit words from IO Address 'Addr'
;
;*****************************************************************************
PUBLIC _NsmIOread16mul
_NsmIOread16mul:
	   push  bp
	   mov	 bp,sp
	   mov	 dx,[bp].NsmIOread16mul_Params.Near_Address
	   push  es
	   push  di
	   les	 di,[bp].NsmIOread16mul_Params.Destination_Address
	   mov	 cx,[bp].NsmIOread16mul_Params.Word_Count
	   rep	 insw
	   pop	 di
	   pop	 es
	   mov	 sp,bp
	   pop	 bp
	   ret
;-----------------------------------------------------------------------------

;*****************************************************************************
;
; "NsmIOread8mul"
;
;  Read Multiple 16-bit words from IO Address 'Addr'
;
;*****************************************************************************
PUBLIC _NsmIOread8mul
_NsmIOread8mul:
	   push  bp
	   mov	 bp,sp
	   mov	 dx,[bp].NsmIOread8mul_Params.Near_Address
	   push  es
	   push  di
	   les	 di,[bp].NsmIOread8mul_Params.Destination_Address
	   mov	 cx,[bp].NsmIOread8mul_Params.Byte_Count
	   rep	 insb
	   pop	 di
	   pop	 es
	   mov	 sp,bp
	   pop	 bp
	   ret
;-----------------------------------------------------------------------------

;*****************************************************************************
;
; "NsmIOwrite32"
;
; void NsmIOWrite32(uint32 addr1, uint32 value)
;
;    Write a 32-bit value to a 32 bit IO base Address.
;
;*****************************************************************************

PUBLIC _NsmIOwrite32
_NsmIOwrite32:
           push bp
           mov  bp,sp
           mov  dx,[bp].NsmIOwrite32_Params.Near_Address
;RDG       mov  ax,[bp].NsmIOwrite32_Params.Value.dblword.lw
;RDG       out  dx,ax
;RDG       add  dx,2
;RDG       mov  ax,[bp].NsmIOwrite32_Params.Value.dblword.hw
;RDG       out  dx,ax
           mov  eax,[bp].NsmIOwrite32_Params.Value.dblword ;RDG
           out  dx, eax                 ;RDG
           mov  sp,bp
           pop  bp
           ret

;-----------------------------------------------------------------------------

;*****************************************************************************
;
; "NsmIOwrite16"
;
; void NsmIOWrite32(uint32 addr1, uint32 value)
;
;    Write a 32-bit value to a 32 bit IO base Address.
;
;*****************************************************************************
PUBLIC _NsmIOwrite16
_NsmIOwrite16:
	   push bp
	   mov	bp,sp
	   mov	dx,[bp].NsmIOwrite16_Params.Near_Address
	   mov	ax,[bp].NsmIOwrite16_Params.Value
	   out	dx,ax
	   mov	sp,bp
	   pop	bp
	   ret
;-----------------------------------------------------------------------------

;*****************************************************************************
;
; "NsmIOwrite8"
;
; void NsmIOWrite8(uint32 addr1, uint32 value)
;
;    Write an 8-bit value to a 32 bit IO base Address.
;
;*****************************************************************************
PUBLIC _NsmIOwrite8
_NsmIOwrite8:
	   push bp
	   mov	bp,sp
	   mov	dx,[bp].NsmIOwrite8_Params.Near_Address
	   mov	al,[bp].NsmIOwrite8_Params.Value
	   out	dx,al
	   mov	sp,bp
	   pop	bp
	   ret
;-----------------------------------------------------------------------------

;*****************************************************************************
;
; "NsmIOwrite32mul"
;
;  Write Multiple 16-bit words from buffer to IO Address 'Addr'
;
;*****************************************************************************
PUBLIC _NsmIOwrite32mul
_NsmIOwrite32mul:
	   push  bp
	   mov	 bp,sp
	   mov	 dx,[bp].NsmIOwrite32mul_Params.Near_Address
	   push  ds
	   push  si
	   lds	 si,[bp].NsmIOwrite32mul_Params.Source_Address
	   mov	 cx,[bp].NsmIOwrite32mul_Params.Dword_Count
_NsmIOwrite32mul1:
;RDG       outsw
;RDG       add   dx,2
;RDG       outsw
;RDG       sub   dx,2
;RDG       loopw _NsmIOwrite32mul1
  rep      outsd                        ;RDG
	   pop	 si
	   pop	 ds
	   mov	 sp,bp
	   pop	 bp
	   ret
;-----------------------------------------------------------------------------

;*****************************************************************************
;
; "NsmIOwrite16mul"
;
;  Write Multiple 16-bit words from buffer to IO Address 'Addr'
;
;*****************************************************************************
PUBLIC _NsmIOwrite16mul
_NsmIOwrite16mul:
	   push  bp
	   mov	 bp,sp
	   mov	 dx,[bp].NsmIOwrite16mul_Params.Near_Address
	   push  ds
	   push  si
	   lds	 si,[bp].NsmIOwrite16mul_Params.Source_Address
	   mov	 cx,[bp].NsmIOwrite16mul_Params.Word_Count
	   rep	 outsw
	   pop	 si
	   pop	 ds
	   mov	 sp,bp
	   pop	 bp
	   ret
;-----------------------------------------------------------------------------

;*****************************************************************************
;
; "NsmIOwrite8mul"
;
;  Write Multiple 8-bit words from buffer to IO Address 'Addr'
;
;*****************************************************************************
PUBLIC _NsmIOwrite8mul
_NsmIOwrite8mul:
	   push  bp
	   mov	 bp,sp
	   mov	 dx,[bp].NsmIOwrite8mul_Params.Near_Address
	   push  ds
	   push  si
	   lds	 si,[bp].NsmIOwrite8mul_Params.Source_Address
	   mov	 cx,[bp].NsmIOwrite8mul_Params.Byte_Count
	   rep	 outsb
	   pop	 si
	   pop	 ds
	   mov	 sp,bp
	   pop	 bp
	   ret
;-----------------------------------------------------------------------------

;*****************************************************************************
;
;   "_NSM_MEMCPY"
;
;
;*****************************************************************************
;PUBLIC _NSM_MEMCPY
_NSM_MEMCPY:
	   push bp
	   mov  bp,sp
	   push di
	   push si
	   push ds
	   push es
	   mov	ds,[bp].NSM_MEMCPY_Params.Source_Address.sgm
	   mov	si,[bp].NSM_MEMCPY_Params.Source_Address.ofs
	   mov	es,[bp].NSM_MEMCPY_Params.Destination_Address.sgm
	   mov	di,[bp].NSM_MEMCPY_Params.Destination_Address.ofs
	   mov	cx,[bp].NSM_MEMCPY_Params.Block_Size
;RDG       mov  al,0
;RDG       shr  cx,1
;RDG       rcr  al,1
;RDG       shr  cx,1
;RDG       rcr  al,1
;RDG       rep  movsd 
;RDG       mov  cl,al
;RDG       shr  cx,7
;RDG       rep  movsw
;RDG       jnc  _NSM_MEMCPYX
;RDG       movsb
;RDG_NSM_MEMCPYX:
           cmp  cx, 12                  ;RDG
           jb   nmc02                   ;RDG
           test si, 1                   ;RDG
           jz   nmc01                   ;RDG
           movsb                        ;RDG
           dec  cx                      ;RDG
nmc01:                                  ;RDG
           test si, 2                   ;RDG
           jz   nmc02                   ;RDG
           movsw                        ;RDG
           sub  cx, 2                   ;RDG
nmc02:                                  ;RDG
           mov  ax, cx                  ;RDG
           shr  cx, 2                   ;RDG
  rep      movsd                        ;RDG
           mov  cx, ax                  ;RDG
           and  cx, 3                   ;RDG
  rep      movsb                        ;RDG
	   pop	es
	   pop	ds
	   pop  si
	   pop  di
	   mov  sp,bp
	   pop  bp
	   ret
;-----------------------------------------------------------------------------

;*****************************************************************************
;
;   "_NSM_MEMZERO"
;
;
;*****************************************************************************
;PUBLIC _NSM_MEMZERO
_NSM_MEMZERO:
	   push bp
	   mov  bp,sp
	   push di
	   mov	di,[bp].NSM_MEMZERO_Params.Near_Address
	   mov	cx,[bp].NSM_MEMZERO_Params.Block_Size
           mov  eax,0
           mov  bl,cl
           shr  cx,2
           rep  stosd 
	   mov	cl,bl
           and  cl,3
           rep  stosb
	   pop  di
	   mov  sp,bp
	   pop  bp
	   ret
;-----------------------------------------------------------------------------
_TEXT	   ends
	   end

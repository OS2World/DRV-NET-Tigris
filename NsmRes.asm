;*****************************************************************************
;*              National Semiconductor Company Confidential                  *
;*                                                                           *
;*                          National Semiconductor                           *
;*                       NDIS 2.0.1 MAC device driver                        *
;*       Code for National Semiconductor's CDI resident driver portion.      *
;*                                                                           *
;*      Source Name:    NSMRES.ASM                                           *
;*      Authors:                                                             *
;*                       Frank Dimambro                                      *
;*									     *
;*	$Log:   /home/crabapple/nsclib/tech/ndis2/nsm/vcs/nsmres.asv  $								     *
;
;	Rev 1.16(?)	06/15/99
;		Modified ISR to perserve higher order byte by changing "pushad" and 
;		"popad" to "pushadd" and "popadd".
;		Modified "TransferData" routine to correct actual lenght  of data
;		being copied into RxData.
;	
;	   Rev 1.15   09/22/95 19:21:40   frd
;	Modified NSMSTDmessage, since it had a bug which affected 
;	message display.
;	
;	   Rev 1.13   09/14/95 16:33:10   frd
;	Made modifications to support the Euphrates driver.
;	
;	   Rev 1.12   23 May 1995 14:23:40   FRD
;	 
;	
;	   Rev 1.9.1.0   23 May 1995 10:36:08   FRD
;	 
;	
;	   Rev 1.11   17 May 1995 14:46:50   FRD
;	Added __aNulshr to NSM to avoid library functions 
;	being added to the end of the driver i.e. Part that
;	gets thrown away.
;	
;	   Rev 1.10   17 May 1995 14:43:02   FRD
;	Added NsmWaitTime to the NSM.
;	
;	   Rev 1.9   04 May 1995 14:33:28   FRD
;	New hardware ISR to minimize system stack usage. Now swaps onto own stack before saving registers and switches onto another stack before calling Indication Complete in DOS.
;	
;	   Rev 1.8   01 May 1995 13:51:04   FRD
;	Modified source file to allow it to be build in the nsclib 
;	directory structure.
;	
;	   Rev 1.6   26 Apr 1995 09:53:18   FRD
;	 
;	
;	   Rev 1.5   20 Apr 1995 15:34:32   FRD
;	 
;	
;	   Rev 1.4   16 Apr 1995 12:48:06   FRD
;	Fixed Bug which was leading to a system Hang when 
;	the Dynamic = Yes keyword was present in the
;	protocol.ini file used in Lan-Manager ans WFWG
;	v3.1.
;	The system was attempting a driver reset request
;	which in turn causes a HPA software Interrupt at
;	that point Interupts were disaabled therefore the
;	reset would never complete. The system just polled
;	waiting for the reset to complete. 
;	The solution was to add HSMEnableNicInts into the
;	Bind routine.
;	
;	   Rev 1.0   12 Apr 1995 14:08:06   FRD
;	 
;	
;	   Rev 1.3   12 Apr 1995 13:22:46   FRD
;	 
;*
;*	   Rev 1.2   12 Apr 1995 13:19:58   FRD
;*
;*
;*	   Rev 1.3   12 Apr 1995 12:44:10   FRD
;*
;*									     *
;*****************************************************************************
INCLUDE cdi.inc
INCLUDE cdimsgs.inc
INCLUDE nsm.inc
INCLUDE hsm.inc
INCLUDE NSMdef.inc
INCLUDE NSMSTDIO.inc

.seq
.386
IFDEF OS2
DGROUP		group	_DATA,CONST,_BSS,FAR_BSS
CGROUP		group	_TEXT
ENDIF
;*****************************************************************************
; External Hardware specific module definitions.
;*****************************************************************************
extern DRInit:NEAR16
extern PutCSt:NEAR16
extern _HsmInitialize:NEAR16
extern _HsmReset:NEAR16
extern _HsmOpen:NEAR16
extern _HsmClose:NEAR16
extern _HsmForceInterrupt:NEAR16
extern _HsmSetRxMode:NEAR16
extern _HsmSetMacAddr:NEAR16
extern _HsmGetTxDescList:NEAR16
extern _HsmTransmit:NEAR16
extern _HsmRxFlowControl:NEAR16
extern _HsmRxFreePkt:NEAR16
extern _HsmRxCopyPkt:NEAR16
extern _HsmMulticastLoad:NEAR16
extern _HsmUpdateStatistics:NEAR16
extern _HsmSetRxLookahead:NEAR16
extern _HsmForceInterrupt:NEAR16
extern _HsmEnableNicInts:NEAR16
extern _HsmDisableNicInts:NEAR16
extern _HsmService:NEAR16
extern _HsmTimerEvent:NEAR16
extern _HsmSetMediaType:NEAR16
IFDEF OS2
extern Std_EMsg:FAR16
extern Std_EMsg1:FAR16
extern Hsm_Short_Name:FAR16
ELSE
extern Std_EMsg:NEAR16
extern Std_EMsg1:NEAR16
extern Hsm_Short_Name:NEAR16
ENDIF
;-----------------------------------------------------------------------------
;
;****** CPU & Segment Declarations, and Driver Device Header ******
;
IFDEF   OS2
_DATA	segment dword public use16 'DATA'	;OS/2 uses data & code segments.
PUBLIC OS2_Hdr
;*****************************************************************************
; OS/2 Driver Device Header.
;*****************************************************************************
OS2_Hdr	header {-1,08080h,offset Strat,0,'INFOMR$ ',0,0,0,0}
;-----------------------------------------------------------------------------

;*****************************************************************************
; OS/2 Driver Access to global information table selectors.
;*****************************************************************************
PUBLIC OS2_Global_sel
OS2_Global_sel dw 0
;-----------------------------------------------------------------------------
ELSE
_TEXT	segment public use16 'CODE'	;DOS Driver contained in one segment.
		assume	cs:_TEXT,ds:_TEXT
;*****************************************************************************
; DOS Driver Device Header.
;*****************************************************************************
PUBLIC DOS_Hdr
DOS_Hdr header	{-1,0C000h,offset Strat,offset StratI,{0}}
ENDIF
;
;****** Driver Parameter Tables and Variables ******
;
IFDEF OS2
OS2_Date_Time   Date_Time {}
ENDIF
;-----------------------------------------------------------------------------

;*****************************************************************************
; Hsm Context Definition.
;*****************************************************************************
PUBLIC NICContext
NICContext  db	 HSM_CONTEXT_SIZE  dup (0)
;-----------------------------------------------------------------------------

;*****************************************************************************
; Storage place for media type from keyword.
;*****************************************************************************
PUBLIC MediaTypeStore
MediaTypeStore	 db	HSM_MEDIA_AUTO_CONFIG
;-----------------------------------------------------------------------------

;*****************************************************************************
; Driver request dispatch table.
;*****************************************************************************
RqDisp  Rq_Disp {offset RqUnsp,offset RqUnsp,offset RqUnsp,offset RqSetA,\
		 offset RqOpen,offset RqClos,offset RqRSet,offset RqSetF,\
		 offset RqAddM,offset RqDelM,offset RqUpdS,offset RqClrS,\
		 offset RqIReq,offset RqUnsp,offset RqSetL}
;-----------------------------------------------------------------------------

;*****************************************************************************
; Common characteristics table
;*****************************************************************************
PUBLIC cc_tbl
;cc_tbl	cctable <CCTSIZE,PMAJ_VN,PMIN_VN,0,MAJ_VN,MIN_VN,\
;		 ?,OEM_SHORT_NAME,<>,UL_MAC,UI_MAC,LL_PHYS,LI_MAC,\
;		 0,0,offset System,offset mc_spec,\
;		 offset mc_stat,offset ddsptbl,0,0,0>
cc_tbl	label	cctable ; MASM 6.0 cannot make valid table. (ToT)
	dw	CCTSIZE
	db	PMAJ_VN, PMIN_VN	; NDIS version.
	dw	0
	db	MAJ_VN, MIN_VN		; MAC version.
	cctmff	<>
	db	OEM_SHORT_NAME
	db	18 - @SizeStr(%OEM_SHORT_NAME) dup (0)
	db	UL_MAC, UI_MAC		; upper
	db	LL_PHYS, LI_MAC		; lower
	dw	?			; module ID
	dw	?			; module DS
	dd	offset System
	dd	offset mc_spec
	dd	offset mc_stat
	dd	offset ddsptbl
	dd	0
	dd	0,0
;-----------------------------------------------------------------------------

;*****************************************************************************
; MAC-specific characteristics table
;*****************************************************************************
PUBLIC mc_spec
;mc_spec specific   <MCSIZE,'DIX+802.3',ADDRLEN,<08,00,17,00,00,00>,<>,0,\
;                    offset Max_No_Multicast,10000000,?,?,?,PKTMAX,\
;                    PKTMAX,PKTMAX,0,0,PKTMAX,008h,000h,017h,020h,\
;					<>,offset VndMsg,0,0,2,MAX_TX_FRAGS>
mc_spec	label	specific
	dw	MCSIZE
	db	'DIX+802.3'
	db	16 - @SizeStr(<DIX+802.3>) dup (0)
	dw	ADDRLEN
	db	08, 00, 17, 00, 00, 00, 10 dup (0)
	db	16 dup (0)
	dd	0
	dd	offset Max_No_Multicast
	dd	10000000
	mc_mssf1	<>	; byte record
	mc_mssf2	<>	; byte record
	mc_mssf3	<>	; word record
	dw	PKTMAX		; max frame size.
	dd	PKTMAX		; total tx cap.
	dw	PKTMAX		; tx buffer size.
	dd	PKTMAX		; total rx cap. - org:0
	dw	PKTMAX		; rx buffer size.
	db	8, 0, 17h
	db	20h
	dd	offset VndMsg
	dw	0		; IRQ level
	dw	2		; queue depth
	dw	NSM_MAX_TX_FRAGS	; max frags in desc.
	dd	0
;-----------------------------------------------------------------------------

;*****************************************************************************
; MAC-specific status table.  Counters set to -1 are not maintained.
;*****************************************************************************
PUBLIC mc_stat
mc_stat status  {MSSIZE,-1,?,?,offset mc_stat2,-1,0,0,-1,0,-1,\
                 -1,0,-1,-1,-1,-1,-1,0,-1,-1,-1,-1,-1,-1,-1}
;-----------------------------------------------------------------------------

;*****************************************************************************
; SNMP Statistics table pointer.
;*****************************************************************************
PUBLIC NSSNAIStableptr
NSSNAIStableptr   dd	offset NICContext.HsmContext.Interfacename
PUBLIC NSSNAISIntName
NSSNAISIntName	  db	'SNMPSTK$'
PUBLIC NSSNAISIntVerMaj
NSSNAISIntVerMaj  db	1
PUBLIC NSSNAISIntVerMin
NSSNAISIntVerMin  db	0
PUBLIC NSSNAISStatBufSz
NSSNAISStatBufSz  dw	(offset NICContext.HsmContext.TxPktQueue) - (offset NICContext.HsmContext.StatBufMajVer)+4
;-----------------------------------------------------------------------------

;*****************************************************************************
; 802.3 Media-Specific Statistics.  Items marked "N.A." are not applicable.
;*****************************************************************************
mc_stat2 status2 {MS2SIZE,1,0,-1,0,-1,-1,0,-1,-1,-1,-1,-1,-1,0,-1,0}
;-----------------------------------------------------------------------------

;*****************************************************************************
; Some more statistics? God knows when these are used?????.
;*****************************************************************************
RxSPCt  dw      0               ;Input undersize-packet count. 
RxLPCt  dw      0               ;Input oversize-packet count.
RxRBECt dw      0               ;Input buffers-exhausted count.
RxRDECt dw      0               ;Input descriptors-exhausted count.
TxEXDCt dw      0               ;Output excessive-deferrals count.
TxHBLCt dw      0               ;Output heartbeat-lost count.
;-----------------------------------------------------------------------------

;*****************************************************************************
; Driver dispatch table (NDIS "upper" dispatch table).  All are offset/segment.
;*****************************************************************************
PUBLIC ddsptbl
ddsptbl dd_sptbl {offset cc_tbl,offset RQuest,offset TxData,\
		  offset RxData,offset RxRelease,offset IndOn,\
		  offset IndOff}
;-----------------------------------------------------------------------------

;*****************************************************************************
; Protocol-Request Dispatch Table.
;*****************************************************************************
pdsptbl pd_sptbl {}
;-----------------------------------------------------------------------------

;*****************************************************************************
; Structure use to calculate statistics maintained in the driver.
;*****************************************************************************
KeptStats Kept_Stats {}
;-----------------------------------------------------------------------------

;*****************************************************************************
; NDIS Multicast Address List format.
;*****************************************************************************
Multicast_Address_Table equ     $
Max_No_Multicast        dw      32   ;Maximum number of list entries.
Current_No_Multicast    dw      0
Multicast_Address_List  db      (32*16) dup (0) ;Multicast address list (16 bytes per entry).
;-----------------------------------------------------------------------------

;*****************************************************************************
; NSM Multicast Address List format.
;*****************************************************************************
NSM_Mcast_Addr_List     HsmMulticastTableEntry 32 dup ({{},?})
;-----------------------------------------------------------------------------

;*****************************************************************************
; Status Indications.
;*****************************************************************************
Status_Indicate    db   0        ;Reset Status Indications flag.
Rx_Indicate        db   ON        ;Receive Status Indications flag.
IntReq_Indicate    db   0        ;InterruptRequest Status Indications flag.
align	2
Indication         dw   ON
IndicationComplete dd   0
StatusIndication   dd   0                         

IFDEF OS2
IF 0
public CtxHdl_IndicComp, CtxHdl_IndicComp2
CtxHdl_IndicComp   dd	?	; Context handle for NsmCtxIndicCopm.
CtxHdl_IndicComp2  dd	?	; Another context handle.
ENDIF
;Ctx_Indic_Req      db	0	; more request flag
ENDIF
;-----------------------------------------------------------------------------

;*****************************************************************************
; Pointer to heap at end of the driver (dos) or data segment (os/2).
;*****************************************************************************
PUBLIC NSM_Heap
NSM_Heap	dw     End_Mark
;-----------------------------------------------------------------------------

;*****************************************************************************
; Pointer to heap at end of the driver (dos) or data segment (os/2).
;*****************************************************************************
PUBLIC Tx_Immed_Start
Tx_Immed_Start	dw     ?
PUBLIC Tx_Immed_End
Tx_Immed_End	dw     ?
PUBLIC Tx_Immed_Wr
Tx_Immed_Wr	dw     ?
PUBLIC Tx_Immed_Rd
Tx_Immed_Rd	dw     ?
PUBLIC Tx_Immed_Start_Phys
Tx_Immed_Start_Phys	dd	?
Tx_Immed_Sem	db	0, 0
;-----------------------------------------------------------------------------

;*****************************************************************************
; Transfer data HSM interface discriptor storage area.
;*****************************************************************************
		db		'TranDataBuf='
PUBLIC TranDataBuf
TranDataBuf	HsmPktDesc	<>
		HsmFrag 	7 dup (<>)
		db		'TranDataBuf1='
PUBLIC TranDataBuf1
TranDataBuf1	HsmPktDesc	<>
		HsmFrag 	7 dup (<>)
		db		'TranDataBufEnd',0
;-----------------------------------------------------------------------------

IFDEF OS2
;*****************************************************************************
; Pointer to Phys to User vir Virtual address store after Immediate data.
;*****************************************************************************
PUBLIC Rx_Phys_Store
Rx_Phys_Store  dw	 18 dup (?)
;-----------------------------------------------------------------------------
ENDIF
;*****************************************************************************
; Transfer Data Variables storage area.
;*****************************************************************************
RxPktSize         dw    ?
RxPktRemaining    dw    ?
RxPktStatus	  dw	?
RxPktHandle	  dw	?
TdStatus	  dw	?
RxDoneSoFar       dw    0ffh
;-----------------------------------------------------------------------------

;*****************************************************************************
; Storage Location for in Timer and device Interrupt chain.
;*****************************************************************************
TimerChain        dd    0
IntChain          dd    0
;-----------------------------------------------------------------------------

;*****************************************************************************
; Storage Location for in Timer count.
;*****************************************************************************
Timer_Count	  db	0
;-----------------------------------------------------------------------------

;*****************************************************************************
; Storage Location for in software critical section semaphores.
;*****************************************************************************
IFNDEF  OS2
InCritical        db    0
IntrState         dw    0
ELSE
InCriticalTX      db    0
InCriticalRX      db    0
IntrStateTX       dw    0
IntrStateRX       dw    0
ENDIF
;-----------------------------------------------------------------------------

;*****************************************************************************
; Storage Location for code/Data Segment Ending offset address.
;*****************************************************************************
PUBLIC dta_end
dta_end 	dw	 0FFFFh
;-----------------------------------------------------------------------------

IFDEF	OS2
;*****************************************************************************
; Store for OS/2 device-help routine.
;*****************************************************************************
PUBLIC DH_Addr
DH_Addr dd	0		;Address of OS/2 device-help routine.
;-----------------------------------------------------------------------------
ELSE
;*****************************************************************************
; Temporary Stack Space for MAC Interrupt service routine.
;*****************************************************************************
BotStak        db     1024*2   dup (0)	;Auxiliary stack area.
TopStak equ    $                    ;("Top-of-stack" pointer).
BotStakind	db     1024*1	dup (0)	;Auxiliary stack area.
TopStakind equ	$		     ;("Top-of-stack" pointer).
;-----------------------------------------------------------------------------
ENDIF

;*****************************************************************************
; Protocol Data segment Address.
;*****************************************************************************
protDS  dw      0               ;Protocol's DS-reg. value.
;-----------------------------------------------------------------------------

;*****************************************************************************
; Driver Status Flags.
;*****************************************************************************
align	2
PUBLIC dvrflags
dvrflags dvr_flags{}		;Internal MAC Status flags.
;-----------------------------------------------------------------------------

;*****************************************************************************
; Driver Vender Message String.
;*****************************************************************************
VndMsg  db      'National Semiconductor Corp. DP83815 10/100 MacPhyter3v PCI Adapter',0 ;Vendor I.D. string.
;-----------------------------------------------------------------------------

;*****************************************************************************
; Driver Files opened handles array.
;*****************************************************************************
	align   word
DvrHndls dw     MAXDVRINST dup (0)      ;Array of opened drivers.
;-----------------------------------------------------------------------------

;*****************************************************************************
; Protocol-Manager File handle Store.
;*****************************************************************************
	align   word
PUBLIC pmgrhdl
pmgrhdl dw	0	      ;Protocol-Manager "Open Handle".
PUBLIC action
action	dw	0
;-----------------------------------------------------------------------------

;*****************************************************************************
; Protocol-Manager Request block Store.
;*****************************************************************************
PUBLIC pmblock
pmblock pm_req_block{}		;Protocol-Manager request block
;-----------------------------------------------------------------------------

;*****************************************************************************
; Protocol-Manager Module Name Store.
;*****************************************************************************
PUBLIC pmgrnam
pmgrnam db	'PROTMAN$',0	;DOS Protocol Manager Device Name.
;-----------------------------------------------------------------------------

;*****************************************************************************
; Carriage return line feed used by NsmLogErrorMessage.
;*****************************************************************************
New_line	   db	    CR,LF,0
;-----------------------------------------------------------------------------

;*****************************************************************************
; Software CRC data Table.   v2.0d
;*****************************************************************************
result	   dd 0
;-----------------------------------------------------------------------------

;*****************************************************************************
; Software CRC data Table.   v2.0d
;*****************************************************************************
	   align 4
crc32table dd 000000000h, 077073096h, 0ee0e612ch, 0990951bah, 0076dc419h
	   dd 0706af48fh, 0e963a535h, 09e6495a3h, 00edb8832h, 079dcb8a4h
	   dd 0e0d5e91eh, 097d2d988h, 009b64c2bh, 07eb17cbdh, 0e7b82d07h
	   dd 090bf1d91h, 01db71064h, 06ab020f2h, 0f3b97148h, 084be41deh
	   dd 01adad47dh, 06ddde4ebh, 0f4d4b551h, 083d385c7h, 0136c9856h
	   dd 0646ba8c0h, 0fd62f97ah, 08a65c9ech, 014015c4fh, 063066cd9h
	   dd 0fa0f3d63h, 08d080df5h, 03b6e20c8h, 04c69105eh, 0d56041e4h
	   dd 0a2677172h, 03c03e4d1h, 04b04d447h, 0d20d85fdh, 0a50ab56bh
	   dd 035b5a8fah, 042b2986ch, 0dbbbc9d6h, 0acbcf940h, 032d86ce3h
	   dd 045df5c75h, 0dcd60dcfh, 0abd13d59h, 026d930ach, 051de003ah
	   dd 0c8d75180h, 0bfd06116h, 021b4f4b5h, 056b3c423h, 0cfba9599h
	   dd 0b8bda50fh, 02802b89eh, 05f058808h, 0c60cd9b2h, 0b10be924h
	   dd 02f6f7c87h, 058684c11h, 0c1611dabh, 0b6662d3dh, 076dc4190h
	   dd 001db7106h, 098d220bch, 0efd5102ah, 071b18589h, 006b6b51fh
	   dd 09fbfe4a5h, 0e8b8d433h, 07807c9a2h, 00f00f934h, 09609a88eh
	   dd 0e10e9818h, 07f6a0dbbh, 0086d3d2dh, 091646c97h, 0e6635c01h
	   dd 06b6b51f4h, 01c6c6162h, 0856530d8h, 0f262004eh, 06c0695edh
	   dd 01b01a57bh, 08208f4c1h, 0f50fc457h, 065b0d9c6h, 012b7e950h
	   dd 08bbeb8eah, 0fcb9887ch, 062dd1ddfh, 015da2d49h, 08cd37cf3h
	   dd 0fbd44c65h, 04db26158h, 03ab551ceh, 0a3bc0074h, 0d4bb30e2h
	   dd 04adfa541h, 03dd895d7h, 0a4d1c46dh, 0d3d6f4fbh, 04369e96ah
	   dd 0346ed9fch, 0ad678846h, 0da60b8d0h, 044042d73h, 033031de5h
	   dd 0aa0a4c5fh, 0dd0d7cc9h, 05005713ch, 0270241aah, 0be0b1010h
	   dd 0c90c2086h, 05768b525h, 0206f85b3h, 0b966d409h, 0ce61e49fh
	   dd 05edef90eh, 029d9c998h, 0b0d09822h, 0c7d7a8b4h, 059b33d17h
	   dd 02eb40d81h, 0b7bd5c3bh, 0c0ba6cadh, 0edb88320h, 09abfb3b6h
	   dd 003b6e20ch, 074b1d29ah, 0ead54739h, 09dd277afh, 004db2615h
	   dd 073dc1683h, 0e3630b12h, 094643b84h, 00d6d6a3eh, 07a6a5aa8h
	   dd 0e40ecf0bh, 09309ff9dh, 00a00ae27h, 07d079eb1h, 0f00f9344h
	   dd 08708a3d2h, 01e01f268h, 06906c2feh, 0f762575dh, 0806567cbh
	   dd 0196c3671h, 06e6b06e7h, 0fed41b76h, 089d32be0h, 010da7a5ah
	   dd 067dd4acch, 0f9b9df6fh, 08ebeeff9h, 017b7be43h, 060b08ed5h
	   dd 0d6d6a3e8h, 0a1d1937eh, 038d8c2c4h, 04fdff252h, 0d1bb67f1h
	   dd 0a6bc5767h, 03fb506ddh, 048b2364bh, 0d80d2bdah, 0af0a1b4ch
	   dd 036034af6h, 041047a60h, 0df60efc3h, 0a867df55h, 0316e8eefh
	   dd 04669be79h, 0cb61b38ch, 0bc66831ah, 0256fd2a0h, 05268e236h
	   dd 0cc0c7795h, 0bb0b4703h, 0220216b9h, 05505262fh, 0c5ba3bbeh
	   dd 0b2bd0b28h, 02bb45a92h, 05cb36a04h, 0c2d7ffa7h, 0b5d0cf31h
	   dd 02cd99e8bh, 05bdeae1dh, 09b64c2b0h, 0ec63f226h, 0756aa39ch
	   dd 0026d930ah, 09c0906a9h, 0eb0e363fh, 072076785h, 005005713h
	   dd 095bf4a82h, 0e2b87a14h, 07bb12baeh, 00cb61b38h, 092d28e9bh
	   dd 0e5d5be0dh, 07cdcefb7h, 00bdbdf21h, 086d3d2d4h, 0f1d4e242h
	   dd 068ddb3f8h, 01fda836eh, 081be16cdh, 0f6b9265bh, 06fb077e1h
	   dd 018b74777h, 088085ae6h, 0ff0f6a70h, 066063bcah, 011010b5ch
	   dd 08f659effh, 0f862ae69h, 0616bffd3h, 0166ccf45h, 0a00ae278h
	   dd 0d70dd2eeh, 04e048354h, 03903b3c2h, 0a7672661h, 0d06016f7h
	   dd 04969474dh, 03e6e77dbh, 0aed16a4ah, 0d9d65adch, 040df0b66h
	   dd 037d83bf0h, 0a9bcae53h, 0debb9ec5h, 047b2cf7fh, 030b5ffe9h
	   dd 0bdbdf21ch, 0cabac28ah, 053b39330h, 024b4a3a6h, 0bad03605h
	   dd 0cdd70693h, 054de5729h, 023d967bfh, 0b3667a2eh, 0c4614ab8h
	   dd 05d681b02h, 02a6f2b94h, 0b40bbe37h, 0c30c8ea1h, 05a05df1bh
	   dd 02d02ef8dh
;-----------------------------------------------------------------------------

;*****************************************************************************
; Resident data end marker.
;*****************************************************************************
End_Mark      equ      $
;-----------------------------------------------------------------------------

IFDEF OS2
_DATA	     ends
CONST	      SEGMENT	WORD PUBLIC 'CONST'
CONST         ENDS
FAR_BSS       SEGMENT   WORD PUBLIC 'FAR_BSS'
FAR_BSS       ENDS
_BSS          SEGMENT   WORD PUBLIC 'BSS'
_BSS          ENDS

_TEXT	   segment word public use16 'CODE'
		assume	cs:CGROUP,ds:_DATA
PUBLIC Strat
ENDIF
;
;****** Driver Functional Routines *******
;
;*****************************************************************************
;
;   "Strategy" routine. This routine accepts only one INIT request packet.
;   All other functions are handled by direct procedure calls.  At entry,
;   ES:BX points to the request packet.    The bulk of the initialization
;   logic is at the end of the code segment (below), so it can be "kicked
;   out" of memory to save space after initialization is done.
;
;*****************************************************************************
IFNDEF  OS2
SaveBX  dw      ?               ;Saved INIT packet offset.
SaveES  dw      ?               ;Saved INIT packet segment.
ENDIF
Strat:
IFNDEF  OS2
	mov     cs:SaveBX,bx    ;DOS entry -- save "strategy" packet ptr.
	mov     cs:SaveES,es
	retf                    ;Exit (wait for "Device Interrupt").

StratI:
	pushf       ;DOS "Device Interrupt" -- save needed regs.
	push    es
	push    bx
	les     bx,cs:dword ptr SaveBX ;Load "strategy" packet ptr.
ENDIF
	cld
	cmp	es:[bx].SRqPkt.SRqOp,INIT ;Is this an INIT packet?
	jne     Noinit             ;No, get out.
	jmp     DRInit             ;Go initialize this driver & exit.
Noinit:
	mov     es:[bx].SRqPkt.SRqStat,mask done or \
					mask strat_err or \
					STBADCMD
				   ; If Not Initialisation, Could be an
				   ; Request opcode error,Open or Close.
	cmp     es:[bx].SRqPkt.SRqOp,OPEN ;Is this an OPEN opcode?
	jne     Noopen             ; No. Go Check for A Close opcode.
	jmp     Exit_Done          ; Yes. Go post "done" in request header.
Noopen:                            ;      status field.
	cmp     es:[bx].SRqPkt.SRqOp,CLOSE
				   ;Is this a CLOSE opcode?
	jne     Noclose            ; No. Exit with Request opcode error
	jmp     Exit_Done          ; Yes. Go post "done" in request header.
Noclose:                           ;      status field.
	cmp	es:[bx].SRqPkt.SRqOp,READ
				   ;Is this a READ opcode?
	jne     StratI_quit
	push    es
	push    bx                 ;Store Address of the request Packet.
	push	offset NICContext
	call	_HsmUpdateStatistics
	add	sp,2
	pop	bx
	mov     di,word ptr es:[bx].SRqPkt.xfer_addr_off
	mov     ax,word ptr es:[bx].SRqPkt.xfer_addr_seg
	mov     es,ax
	mov	si,offset NICContext.HsmContext.StatBufMajVer
	mov     cx,word ptr NSSNAISStatBufSz
	shr     cx,1
	rep     movsw
	pop	es
	mov     cx,word ptr NSSNAISStatBufSz
	mov     word ptr es:[bx].SRqPkt.xfer_size,cx
Exit_Done:                         ;     Posted Earlier.
	mov     es:[bx].SRqPkt.SRqStat,mask done
				   ;Post "done" in request header.
StratI_quit:                       ;      status field.
IFNDEF  OS2
	pop     bx                 ;Reload remaining regs.
	pop     es
	popf
ENDIF
	retf                    ;Exit.
;-----------------------------------------------------------------------------

;*****************************************************************************
;
;   "SYSTEM" Entry-point.  Used by the protocol manager or the upper protocol
;   during the Bind process.  Only a Bind request will be accepted.
;
;*****************************************************************************
public	System		; << debug >>
System:
	push	bp	       ;OS/2 -- set up frame ptr.
	mov     bp,sp
	push    ds              ;Set our DS-reg.
	mov     ds, [bp].stdparm.std_ds
	push    si              ;Save needed regs.
	push    di
	push	es
	mov	es, [bp].stdparm.std_ds
	test	mc_stat.ms_flag,mask bind_status
				;Are we already "bound"?
	jz      SyOpCh          ;No, check for "bind" opcode.
SyFail: mov     ax,GEN_FAILURE  ;Error!  post "General Failure" error code.
	jmp     SyExit          ;Invalid request!  Exit with error code.
SyOpCh:
	cmp	[bp].syparm.sy_op,BIND ;Is this a "bind" request?
	jne     SyFail          ;No, exit with error code.
SyBind:
	push	offset NICContext
	call	_HsmInitialize	   ; Initialise adapter.
	add	sp,2
	cmp	ax,HsmOK
	jne	SyExit
	push	offset NICContext
	call	_HsmReset	   ; Initialise adapter.
	add	sp,2
	or	mc_stat.ms_flag,mask bind_status or mask hardware_status
	push	es
	mov	ax,0
	push	ax
	mov	al,byte ptr MediaTypeStore
	mov	ah,0
	push	ax
	push	offset NICContext
	call	_HsmSetMediaType	   ; Initialise adapter.
	add	sp,6
	pop	es
	call	Nsm_Init
	les	di,[bp].syparm.sy_par2
				  ; Return our common-characteristics
				  ; table ptr.
	mov     es:[di].farptr.ofs,offset cc_tbl
	mov     es:[di].farptr.sgm,ds
	les     di,[bp].syparm.sy_par1
				  ; Save protocol module's DS-reg. value.
	mov     ax,es:[di].cctable.cc_ds
	mov     protDS,ax
	push    ds                ; Copy protocol's dispatch table for
				  ; our use.
	lds     si,es:[di].cctable.cc_ldsp
	pop     es
	mov     di,offset pdsptbl
	mov     cx,PDSIZE
	rep     movsb
	push    es                ; Reload DS-reg.
	pop     ds
	xor	bx,bx		;Get desired IRQ
				;number in bx.
	mov	bl,byte ptr mc_spec.mc_int.dblbyte.lb
IFDEF   OS2
	mov     ax,NsmIsr
				;Get interrupt-routine offset.
;	mov	dh,1		;Specify "shared interrupt" (DH-reg. = 1).
;				;Doesn't work with MSOS2.......
;	mov	dx,DH_SETIRQ	;Have OS/2 install our interrupt vector.

;	mov	dh,1		;At first, try "Shared". if error,
;	mov	dl,DH_SETIRQ	; retry "Exclusive".
	mov	dx,(1 shl 8)+DH_SETIRQ
	call	dword ptr [DH_Addr]
	jnc	short SetIRQ_ok
	xor	bx,bx
	mov     ax,NsmIsr
	mov	bl,byte ptr mc_spec.mc_int.dblbyte.lb
;	mov	dh,0		;Exclusive
;	mov	dl,DH_SETIRQ
	mov	dx,DH_SETIRQ

	call	dword ptr DH_Addr
	mov	ax,INT_CONFLICT ;Get error code.
	jc      SyExit          ;If our vector is already being used, sayonara!
SetIRQ_ok:
	mov	dl,DH_TICKCOUNT ;Set up timer-routine call every 2 "ticks".
	mov     ax,offset TimerIsr
	mov     bx,2
	call	dword ptr DH_Addr
ELSE
	add     bl,INT_TABLE1  ;Get actual interrupt vector number.
	cmp     bl,00Fh        ;Are we using IRQ8-IRQ15?
	jbe     I_IRQ2         ;No, post IRQ number & vector number.
	add     bl,INT_TABLE2  ;Offset to PC/AT vector numbers.
I_IRQ2:
	mov     al,bl
	mov     ah,GETIRQ
	push    cs              ;Save CS-reg.
	int     021h            ;Do desired DOS function.
	pop     ds              ;Reload DS-reg.
	mov     IntChain.farptr.ofs,bx
	mov     IntChain.farptr.sgm,es
	mov     ah,SETIRQ       ;Get "set IRQ" function code & vector.
	mov     dx,offset NsmIsr
				;Get interrupt-routine offset.
	push    cs              ;Save CS-reg.
	int     021h            ;Do desired DOS function.
	pop     ds              ;Reload DS-reg.
	mov	cx,word ptr mc_spec.mc_int
	mov     ch,cl           ;Get desired IRQ level.
	and     cl,007h         ;Get 8259 IRQ number.
	mov     ah,1            ;Get desired interrupt-disable mask.
	shl     ah,cl
	not     ah              ;Post interrupt-enable mask above.
	cmp     ch,8            ;Are we using IRQ 0-7?
	jb      I_Msk1          ;Yes, post interrupt mask bits below.
	in      al,IMR2         ;Unmask our interrupt line.
	call    Delay           ;(Delay before next I-O command).
	and     al,ah
	out	IMR2,al
	jmp     MSKSET
I_Msk1:
	in      al,IMR1         ;Unmask our interrupt line.
	call    Delay           ;(Delay before next I-O command).
	and     al,ah
	out     IMR1,al
MSKSET:
	mov     ah,GETIRQ
	mov     al,TIMER_INT    ;Save DOS timer vector.
	push    cs              ;Save CS-reg.
	int     021h            ;Do desired DOS function.
	pop     ds              ;Reload DS-reg.
	mov     TimerChain.farptr.ofs,bx
	mov     TimerChain.farptr.sgm,es
	mov     ah,SETIRQ
	mov     al,TIMER_INT    ;Install our timer-interrupt vector.
	mov     dx,offset TimerIsr
	push    cs              ;Save CS-reg.
	int	021h		;Do desired DOS function.
	pop     ds              ;Reload DS-reg.
ENDIF
	push	word ptr NICContext.HsmContext.rxLookaheadSize
	push	offset NICContext
	call	_HsmSetRxLookahead
	add     sp,4
	mov	ax,0
	push    ax
	push	offset NICContext
	call	_HsmSetRxMode	; Set receive packet mode.
	add	sp,4
	push	offset NICContext
	call	_HsmEnableNicInts
	add     sp,2
	mov	ax,SUCCESS	;Post "success" return code.
SyExit: pop     es              ;Reload regs.
	pop     di
	pop     si
	pop     ds
	pop     bp
	retf    SYRET           ;Pop parameters & exit.
Delay:  ret
;-----------------------------------------------------------------------------

;*****************************************************************************
;
;   "Nsm_Init"
;
;  Initialises all the NDIS tables and NSM variables required for operation.
;
;*****************************************************************************
Nsm_Init:
       mov    ax,word ptr NICContext.HsmContext.Irq.dblword.lw
       mov    word ptr mc_spec.mc_int,ax
       mov    cx,(sizeof NICContext.HsmContext.PermMacAddr)/2
       mov    di,offset mc_spec.mc_pnad
       mov    si,offset NICContext.HsmContext.PermMacAddr
       rep    movsw
       mov    cx,(sizeof NICContext.HsmContext.CurrMacAddr)/2
       mov    di,offset mc_spec.mc_cnad
       mov    si,offset NICContext.HsmContext.CurrMacAddr
       rep    movsw
       mov    ax,NICContext.HsmContext.MediaSpeed.dblword.lw
       cmp    ax,HW_SPEED_10_MBPS
       jne    Nsm_Init1
       mov    ax,10000000 mod 010000h
       mov    word ptr mc_spec.mc_speed.dblword.lw,ax
       mov    ax,10000000 / 010000h
       mov    word ptr mc_spec.mc_speed.dblword.hw,ax
       jmp    Nsm_Init10
Nsm_Init1:
       cmp    ax,HW_SPEED_100_MBPS
       jne    Nsm_Init10
       mov    ax,100000000 mod 010000h
       mov    word ptr mc_spec.mc_speed.dblword.lw,ax
       mov    ax,100000000 / 010000h
       mov    word ptr mc_spec.mc_speed.dblword.hw,ax
Nsm_Init10:
       mov    ax,64
       mov    NICContext.HsmContext.rxLookaheadSize,ax
       mov    NICContext.HsmContext.MulticastTable.farptr.ofs, \
					   offset NSM_Mcast_Addr_List
       mov    NICContext.HsmContext.MulticastTable.farptr.sgm,ds
       mov    NICContext.HsmContext.MulticastTableSize,0
       mov    ax,word ptr NICContext.HsmContext.txQSize
       mov    word ptr mc_spec.mc_txdth,ax
       mov    cx,PKTMAX
       mov    word ptr mc_spec.mc_1buftx,cx
;       mul    word ptr mc_spec.mc_txdth
       mul    cx
       mov    word ptr mc_spec.mc_tottx,ax
       mov    ax,word ptr NICContext.HsmContext.rxQSize
       mul    cx
       mov    word ptr mc_spec.mc_rxbf,ax
       ret
;-----------------------------------------------------------------------------

;*****************************************************************************
;
;   "RQuest"
;
;  Driver "Request" main routine. This logic handles miscellaneous driver
;  requests by calling the appropriate processing routine (below).
;
;*****************************************************************************
public	RQuest		; << debug >>
RQuest:
	push	bp		;Save BP-reg.
	mov     bp,sp           ;Set up frame ptr.
	pushf			;Save CPU interrupt status.
	mov	ax,UNSUPPORTED	;Get "unsupported" error-code.
	mov     bx,[bp].rqparm.rq_op   ;Get request opcode.
	cmp     bx,MAXREQ       ;Legal request opcode?
	jae     RqQuit1         ;No, exit with "unsupported" error code.
	push	si		;Save needed regs.
	push    di
	push    ds
	push    es
	mov     ds, [bp].stdparm.std_ds
	push    ds              ;Set ES-reg. same as DS-reg.
	pop     es
	shl     bx,1            ;Get offset into request dispatch table.
	test    mc_stat.ms_flag,mask open_close_status
	cli                     ;Test if adapter is open or closed.
	call    [bx+offset RqDisp]       ;Call appropriate request routine (below).
	pop	es		;Reload regs.
	pop     ds              ;All requests are atomic since we use
	pop     di              ;synchronous requests.
	pop     si
RqQuit1:
	popf
	pop	bp
	retf    RQRET           ;Pop parameters & exit.
;-----------------------------------------------------------------------------

;*****************************************************************************
;
;        Request codes 0,1,2,10,13 processor -- "All Unsupported".
;
;        All the above listed request functions have been left unsupported.
;
;*****************************************************************************

RqUnsp: mov     ax,UNSUPPORTED  ;Post "unsupported" error-code.
RqUnsX: ret                     ;Exit back to main request routine.
;-----------------------------------------------------------------------------

;*****************************************************************************
;
;       Request code 3 processor -- "SetStationAddress".
;
;*****************************************************************************
RqSetA:
	jnz	RqSetAX 		; If INVALID_FUNCTION if adapter is
					; opened.
	lds     si, dword ptr [bp].rqparm.rq_buf
	test    byte ptr ds:[si], 1     ;Check to ensure that the
	jz      RqSetA1                 ;address is physical
	mov     ax, BAD_PARAMETER
	jmp     RqSetAX1
RqSetA1:
	mov	cx,(sizeof NICContext.HsmContext.CurrMacAddr)/2
	mov     di,offset mc_spec.mc_cnad
	rep     movsw                   ; Move new station address to NIC
					; Context Current Node address.
	mov     ax,SUCCESS
	ret
RqSetAX:
	mov     ax,BAD_FUNCTION
RqSetAX1:
	ret
;-----------------------------------------------------------------------------

;*****************************************************************************
;
;       Request code 4 processor -- "OpenAdapter".
;
;*****************************************************************************
RqOpen:
	test	mc_stat.ms_flag,mask open_close_status 
	jnz     RqOpnX                    ;Check if driver is already open.
	mov	cx,(sizeof NICContext.HsmContext.CurrMacAddr)/2
	mov     si,offset mc_spec.mc_cnad
	mov	di,offset offset NICContext.HsmContext.CurrMacAddr
	rep     movsw
	push	offset NICContext.HsmContext.CurrMacAddr
	push	offset NICContext
	call	_HsmSetMacAddr	   ; set Adapter Physical address adapter.
	add     sp,4
	push	offset NICContext
	call	_HsmOpen	   ; Open adapter.
	add     sp,2
	or      mc_stat.ms_flag,mask open_close_status
					  ;Post this adapter "open".
	mov     ax,SUCCESS
	ret
RqOpnX:
	mov     ax,BAD_FUNCTION
	ret                               ;Exit back to main request routine.
;-----------------------------------------------------------------------------

;*****************************************************************************
;
;       Request code 5 processor -- "CloseAdapter".
;
;*****************************************************************************
RqClos:
	jz	RqCloX			  ;Check if driver is already closed.
	and     mc_stat.ms_flag,not mask open_close_status
					  ; Yes: Ignore the request.
	mov     di,offset Multicast_Address_List
	mov     cx,word ptr Current_No_Multicast
	shl     cx,3                      ; No:  Clear current multicast
	mov     ax,0                      ;      address table.
	rep     stosw
	mov     word ptr Current_No_Multicast,ax

	mov	di,word ptr NICContext.HsmContext.MulticastTable.dblword.lw
	mov     ax,((sizeof MacAddr)+2)/2
	mul	word ptr NICContext.HsmContext.MulticastTableSize
	mov     cx,ax
	mov     ax,0
	rep     stosw
	mov	word ptr NICContext.HsmContext.MulticastTableSize,ax

	push	offset NICContext
	call	_HsmMulticastLoad
	add     sp,2
	mov     ax,SUCCESS
	ret
RqCloX:
	mov     ax,BAD_FUNCTION
	ret                               ; Exit back to main request routine.
;-----------------------------------------------------------------------------

;*****************************************************************************
;
;       Request code 6 processor -- "ResetMAC".
;
;       This function resets the MAC software.
;
;*****************************************************************************
RqRSet:
IFNDEF  OS2
	test	dvrflags,mask Reset
	jnz     RqRSetX
	or      dvrflags,mask Reset
ELSE
	lock	bts	dvrflags,Reset
	jc	short RqRSetX
ENDIF
				;Post Reset flag
	push	offset NICContext
	call	_HsmForceInterrupt
	add     sp,2
RqRSetX:
	mov     ax,SUCCESS      ;Generate a Reset interrupt
	ret                     ;if none is pending.
;-----------------------------------------------------------------------------

;*****************************************************************************
;
;       Request code 7 processor -- "SetPacketFilter".
;
;       Select what kind of packets to be received.
;
;****************************************************************************
RqSetF:
	mov	bx,[bp].rqparm.rq_par
				;Get new "filter" mask.
				;Check if filtermask for any illegal bits
	cmp     bx,mask directed OR mask broadcast_mode OR mask promiscuous_mode
	jnbe    RqFErr          ; Yes: Post error code and exit.
	cli                     ; No:  Save new "filter" mask.
	mov     mc_stat.ms_fltr,bx
	mov     ax,00           ; Default to receive no packets.
	cmp     bx,0            ; A filtermask of 0 indicates that the MAC
				; must not indicate received packets to the
				; protocol.
	jne     RqSF2
	mov	di,word ptr NICContext.HsmContext.MulticastTable.dblword.lw
	mov	cx,word ptr NICContext.HsmContext.MulticastTableSize
	jcxz    RqSF6
RqSF1:
	mov     [di].HsmMulticastTableEntry.useCount,0
	add     di,sizeof(HsmMulticastTableEntry)
	loopw   RqSF1
	jmp     RqSF6
RqSF2:
	shr     bx,1            ; "Multicast" mode on?
	jnc     RqSF4           ;   No, check for "broadcast".
	or      ax,ACCEPT_CAM_QUALIFIED ; ACCEPT_ALL_MCASTS
					; ACCEPT_CAM_QUALIFIED

	mov	di,word ptr NICContext.HsmContext.MulticastTable.dblword.lw
	mov	cx,word ptr NICContext.HsmContext.MulticastTableSize
	jcxz	RqSF4
RqSF3:
	mov     [di].HsmMulticastTableEntry.useCount,1
	add     di,sizeof(HsmMulticastTableEntry)
	loopw   RqSF3

				;       Set HSMSetRxMode "multicast" bit.
RqSF4:  shr     bx,1            ; Check if "Broadcast" mode on?
	jnc     RqSF5           ;       No, check for "promiscuous".
	or      ax,ACCEPT_ALL_BCASTS
				;       Set HSMSetRxMode "broadcast" bit.
RqSF5:  shr     bx,1            ; Check if "Promiscuous" mode on?
	jnc     RqSF6           ;        No, set new operating mode.
	or      ax,ACCEPT_ALL_PHYS + ACCEPT_ALL_BCASTS + ACCEPT_ALL_MCASTS
RqSF6:                          ;        Set HSMSetRxMode "promiscuous" bits.
	push    ax
	push	offset NICContext
	call	_HsmSetRxMode	; Set receive packet mode.
	add     sp,2
	pop     ax
	test    ax,ACCEPT_ALL_MCASTS
	jnz     RqSF7
	push	offset NICContext
	call	_HsmMulticastLoad
	add     sp,2
RqSF7:
	mov     ax,SUCCESS       ;Post "success" code.
	ret                      ;Exit back to main request routine.
RqFErr: mov     ax,BAD_PARAMETER ;Error -- post error code.
	ret                      ;Exit back to main request routine.
;-----------------------------------------------------------------------------

;*****************************************************************************
;
;       Request code 8 processor -- "AddMulticastAddress".
;
;       Adds the given multicast address to the multcast address table if not
;       already there and checks the validity off the given address.
;
;*****************************************************************************
RqAddM:
	les	di,[bp].rqparm.rq_buf	 ; Load the Pointer to the multicast
	call    Valid_McAddr             ; Check if Address is valid.
	jc      RqAddMErr1
	mov     bx,word ptr Current_No_Multicast
	cmp     Max_No_Multicast,bx      ; Check if table is full
	jna     RqAddMErr2
	test    mc_stat.ms_flag,mask open_close_status
	jz      RqAddMErr2               ;Check if driver is closed.
	mov     di,si
	mov     ax,ds                    ;    No: Proceed to addition to
	mov     es,ax                    ;        table.
	lds     si,[bp].rqparm.rq_buf    ;    Yes:  Exit with Bad Function.
	mov     cx,ADDRLEN/2             ; Setup to add new multicast address
	rep     movsw                    ; to the end of the table, pointed
	mov     ds,ax                    ; to by ES:DI
	inc     word ptr Current_No_Multicast
	call    NSM_Upd_Mcast
	push	offset NICContext
	call	_HsmMulticastLoad
	add     sp,2
	mov     ax,SUCCESS
	ret                               ;Exit back to main request routine.
RqAddMErr1:
	mov     ax,BAD_PARAMETER
	ret                               ;Exit back to main request routine.
RqAddMErr2:
	mov     ax,BAD_FUNCTION
	ret                               ;Exit back to main request routine.
;-----------------------------------------------------------------------------

;*****************************************************************************
;
;       Request code 9 processor -- "DeleteMulticastAddress".
;
;       Removes the given address and repacks the multicast address table.
;
;****************************************************************************
RqDelM:
	les	di,[bp].rqparm.rq_buf	 ; Load the Pointer to the multicast
	call    Valid_McAddr             ; Check if Address is valid.
	jnc     RqDelM1
	cmp     si,0                     ;  No: Exit with Invalid parameter.
	je      RqDelMErr2
	jmp     RqDelM2
RqDelM1:
	cmp     si,offset Multicast_Address_List
					 ; Check if table is empty
	jna     RqDelMErr2               ;  Yes: Proceed to deletion from
RqDelM2:                                 ;       table.
	test    mc_stat.ms_flag,mask open_close_status
	jz      RqDelMErr1               ;Check if driver is closed.
	mov     ax,ds                    ;  Yes:Exit with Bad function.
	mov     es,ax                    ; Setup to overwrite multicast
	mov     di,si                    ; address being deleted by the last
	mov     si,word ptr Current_No_Multicast
	dec     si                       ; address in the table.
	mov     cl,4
	shl     si,cl
	add     si,offset Multicast_Address_List
	mov     cx,ADDRLEN/2
	rep     movsw
	dec     word ptr Current_No_Multicast
	call    NSM_Upd_Mcast
	push	offset NICContext
	call	_HsmMulticastLoad
	add     sp,2
	mov     ax,SUCCESS               ;Post "success" code.
	ret                              ;Exit back to main request routine.
RqDelMErr1:
	mov     ax,BAD_FUNCTION
	ret                              ;Exit back to main request routine.
RqDelMErr2:
	mov     ax,BAD_PARAMETER
	ret                              ;Exit back to main request routine.
;-----------------------------------------------------------------------------

;*****************************************************************************
;
;       "NSM_Upd_Mcast".
;
;        Copies the contents of the NSM multicast address table to the
;    CDI defined multicast address table.
;
;****************************************************************************
NSM_Upd_Mcast:
	mov	di,word ptr NICContext.HsmContext.MulticastTable.farptr.ofs
	mov     si,offset Multicast_Address_List
	mov     cx,word ptr Current_No_Multicast
	mov	word ptr NICContext.HsmContext.MulticastTableSize,cx
	cmp     cx,0
	jne     NSM_Upd_Mcast1
	mov     [di].HsmMulticastTableEntry.useCount,0
	jmp     NSM_Upd_McastX
NSM_Upd_Mcast1:
	mov     bx,1
NSM_Upd_Mcast2:
	push    cx
	push    si
	push    di
	mov     cx,(sizeof MacAddr)/2
	rep     movsw
	mov     [di],bx
	pop     di
	pop     si
	pop     cx
	add     di,sizeof HsmMulticastTableEntry
	add     si,16
	loopw   NSM_Upd_Mcast2
NSM_Upd_McastX:
	ret
;-----------------------------------------------------------------------------

;*****************************************************************************
;
;       Request code 10 processor -- "UpdateStatistics".
;
;        This logic updates all relevant statistics
;
;****************************************************************************
RqUpdS: jz	RqUdpSX       ;Not open?  Exit with "Invalid Function" code!
	test	dvrflags,mask NDIS_Stats_Available
	jz      RqUdpSX       ; Check if the NDIS Statistics can be accessed.
	push	offset NICContext
	call	_HsmUpdateStatistics
	add	sp,2

	mov	si,offset NICContext.HsmContext.RxPktsOK
	mov	bx,offset KeptStats.RxPackets
	mov	di,offset mc_stat.ms_ipkt
	call	NSMUpdateStatistics

	mov	si,offset NICContext.HsmContext.TxPktsOK
	mov	bx,offset KeptStats.TxPackets
	mov	di,offset mc_stat.ms_opkt
	call	NSMUpdateStatistics

	mov	ax,SUCCESS    ;  No:  Return Unsupported
			      ;  Yes: Return Success.
	ret
RqUdpSX:
	mov     ax,BAD_FUNCTION
	ret                    ;Exit back to main request routine.
;-----------------------------------------------------------------------------

;*****************************************************************************
;
;       Request code 11 processor -- "ClearStatistics".
;
;        This logic clears all relevant statistics
;
;****************************************************************************
RqClrS: jz      RqClrSX       ;Not open?  Exit with "Invalid Function" code!
	test    dvrflags,mask NDIS_Stats_Available
	jz      RqClrS1       ; Check if the NDIS Statistics can be accessed.
			      ;  No:  Return Unsupported
	push	offset NICContext
	call	_HsmUpdateStatistics
	add	sp,2

	mov	mc_stat.ms_ipkt.dblword.lw,0  ;Reset input statistics.
	mov     mc_stat.ms_ipkt.dblword.hw,0
	mov	si,offset NICContext.HsmContext.RxPktsOK
	mov	di,offset KeptStats.RxPackets
	call	NSMClearStatistics

	mov	mc_stat.ms_opkt.dblword.lw,0  ;Reset output statistics.
	mov     mc_stat.ms_opkt.dblword.hw,0
	mov	si,offset NICContext.HsmContext.TxPktsOK
	mov	di,offset KeptStats.TxPackets
	call	NSMClearStatistics

	mov	ax,SUCCESS
RqClrS1:
	ret
RqClrSX:
	mov     ax,BAD_FUNCTION
	ret                  ;Exit back to main request routine.
;-----------------------------------------------------------------------------

;*****************************************************************************
;
;	"NSMClearStatistics".
;
;	 This function is used to clear the current NDIS tables from
;     the NSSNIAS table
;
;****************************************************************************
NSMClearStatistics:
	mov	ax,[si].dblword.hw
	mov	[di].dblword.hw,ax
	mov	ax,[si].dblword.lw
	mov	[di].dblword.lw,ax
	ret
;-----------------------------------------------------------------------------

;*****************************************************************************
;
;	"NSMUpdateStatistics".
;
;	 This function is used to updates the current NDIS tables from
;     the NSSNIAS table
;
;****************************************************************************
NSMUpdateStatistics:
	mov	ax,[si].dblword.hw
	sub	ax,[bx].dblword.hw
	mov	[di].dblword.hw,ax
	mov	ax,[si].dblword.lw
	sbb	ax,[bx].dblword.lw
	mov	[di].dblword.lw,ax
	ret
;-----------------------------------------------------------------------------

;*****************************************************************************
;
;        Request code 12 processor -- "InterruptRequest".
;
;        This function requests that the MAC generate an asynchronous
;        interrupt status indication back to the protocol.
;
;*****************************************************************************
RqIReq:
	lock	or      dvrflags,mask Int_req
			      ;Post InterrupRequest flag
	push	offset NICContext
	call	_HsmForceInterrupt
	add     sp,2
RqIReqX:                      ;Generate a dummy interrupt if none is pending.
	ret                   ;Exit back to main request routine.
;-----------------------------------------------------------------------------

;*****************************************************************************
;
;        Request code 14 processor -- "SetLookahead".
;
;        Sets the length of the lookahead information for ReceiveLookahead.
;
;*****************************************************************************
RqSetL:
	mov     bx,[bp].rqparm.rq_par
				 ;Get length of lookahead information.
	cmp	bx,word ptr NICContext.HsmContext.rxLookaheadSize
				 ;Is the lookahead length < minimum?
	jna     RqSetLXit        ;Yes, return success.
	cmp     bx,NSM_MAX_LOOKAHEAD_SIZE
				 ;No,Is the lookahead length > maximum?
	ja      RqSetLErr        ;Yes, return invalid_parameter.
				 ;Set the lookaheadlength=length parameter of
	push    bx
	push	offset NICContext
	call	_HsmSetRxLookahead
	add     sp,4
RqSetLXit:
	xor     ax,ax            ;Post "success" code.
	ret                      ;Exit back to main request routine.
				 ;the request.
RqSetLErr:
	mov     ax, BAD_PARAMETER
	ret                      ;Exit back to main request routine.
				 ;the request.
;-----------------------------------------------------------------------------

;*****************************************************************************
;
;       "Valid_McAddr"
;
;       Finds out wether an address pointed to by ES:DI is a valid Multicast
;       address. Then checks to see if it exists in the multi-cast address
;       table. On returning from this routine there is one of 4 conditions
;       satisfied, listed below.
;             1. The address pointed to by ES:DI is a physical or broadcast
;                address.
;                In which case return a Null pointer in SI and set the carry
;                flag.
;             2. The address pointed to by ES:DI is a valid multi-cast
;                address and the multi-cast address table is empty.
;                This time clear the carry flag and return the offset
;                pointer to the start of the multi-cast address table in SI.
;             3. The address pointed to by ES:DI is a valid multi-cast
;                address and is present in the multi-cast address table.
;                This time set the carry flag and return the offset
;                pointer to the multi-cast address, in the multi-cast address
;                table, in SI.
;             4. The address pointed to by ES:DI is a valid multi-cast
;                address and is not present in the multi-cast address table.
;                This time clear the carry flag and return the offset
;                pointer to the end of the multi-cast address table in SI.
;        The ability to satisfy these conditions alows this routine to be
;        use in the ADD and DELETE Multicast Address functions.
;*****************************************************************************
Valid_McAddr:
	mov     si,0
	test    byte ptr es:[di],1   ; Check If address is Multi-cast.
	jz      Valid_McAddrX2       ;  No:  Exit with SI=0, C=1.
	push    di                   ; Check If address is Broadcast
	mov     ax,0ffffh            ;  Yes: Exit with SI=0, C=1.
	mov     cx,ADDRLEN/2         ;  No: Go check if a the Multi-cast
	repe    scasw                ;      table has entries.
	pop     di
	jz      Valid_McAddrX2
	mov     si,offset Multicast_Address_List
	mov     cx,word ptr Current_No_Multicast
	jcxz    Valid_McAddrX1       ; Check If Multi-cast address table
Valid_McAddr1:                       ; has entries.
	push    si                   ;  No:  Exit with SI=offset to Multi-cast
	push    di                   ;       address table, C=0.
	push    cx                   ;  Yes: Check Multi-cast address pointed
	mov     cx,ADDRLEN/2         ;       by ES:DI against each entry in
	repe    cmpsw                ;       table. Until A match is found or
	pop     cx                   ;       the end of the table is reached.
	pop     di                   ; Check if multi-cast addresses match
	pop     si                   ;  Yes: Exit with SI=offset to matching
	je      Valid_McAddrX2       ;       Multi-cast in table. C=1.
	add     si,16                ;  No:  Check if the end of the multi-
	loopw   Valid_McAddr1        ;       cast table has been reached
Valid_McAddrX1:                      ;        Yes: Exit with SI=offset to
	clc                          ;             end of the multi-cast
	ret                          ;             address table,C=0.
Valid_McAddrX2:                      ;        No:  Check next address in
	stc                          ;             table.
	ret
;-----------------------------------------------------------------------------

;*****************************************************************************
;
;      "NSSNAIS_UpdatStat"
;
;	This function is call by the NSSNAIS software to force an update
;  of the Statistics table.
;
;****************************************************************************
PUBLIC NSSNAIS_UpdatStat
NSSNAIS_UpdatStat:
       pushf
       pushad
       push	  ds
       push	  es
IFDEF  OS2
       mov	ax,DGROUP
ELSE
       mov	  ax,cs
ENDIF
       mov	  ds,ax
       mov	  es,ax
       push	  offset NICContext
       call	  _HsmUpdateStatistics
       add	  sp,2
       pop	  es
       pop	  ds
       popad
       popf
       retf
;----------------------------------------------------------------------------

;*****************************************************************************
;
;      "_NsmStartTimer"
;
;       Start the callback timer.  "Interval" is expressed in ms.  If timer
;   expires without a call to NsmStopTimer( ), the Nsm will call
;   HsmTimerEvent( ).
;
;****************************************************************************
PUBLIC _NsmStartTimer
_NsmStartTimer:
	   push bp
	   mov  bp,sp
	   mov	ax,[bp].NsmStartTimer_Params.interval
	   mov	dl,TIMER_GRANULARITY
	   div	dl
	   jnz	_NsmStartTimer1
	   mov	al,1
_NsmStartTimer1:
	   pushf
	   cli
	   mov	byte ptr Timer_Count,al
	   lock	or	dvrflags,mask Timer_On
	   popf
	   mov	sp,bp
	   pop	bp
	   ret
;----------------------------------------------------------------------------

;****************************************************************************
;
;      "_NsmStopTimer"
;
;       Stop the callback timer.
;
;****************************************************************************
PUBLIC _NsmStopTimer
_NsmStopTimer:
	   push bp
	   mov  bp,sp
	   lock	and	dvrflags,not mask Timer_On
	   mov	sp,bp
	   pop	bp
	   ret
;----------------------------------------------------------------------------

;****************************************************************************
;
;      "_NsmGetTime"
;
;      Return the current timer tick value in ms.
;
;****************************************************************************
PUBLIC _NsmGetTime
_NsmGetTime:
       ret
;----------------------------------------------------------------------------

;****************************************************************************
;
;      NsmWaitTime( )
;
;	   Pause execution for the specified number of milliseconds.
;      NsmWaitTime is preferred over any other method when performing fixed
;      time delays from within Hsm's as this gives the Nsm the opportunity
;      to share the CPU with the rest of the operating system.
;
;****************************************************************************
PUBLIC _NsmWaitTime
_NsmWaitTime:
	push	bp
	mov	bp,sp
IFNDEF NECWARP
	mov	cx,[bp].NsmWaitTime_Params.interval
_NsmWaitTime1:		    ; For CX = AX down to 0
	mov     al,00000000b
	out     CNTR_CMD,al
	in      al,COUNNTER0
	mov     bl,al
	in      al,COUNNTER0
	mov     bh,al        ; Get the current timer count, Store in BX.
_NsmWaitTime2:
	mov     al,00000000b ; Get Next Timer Count,Store in AX.
	out     CNTR_CMD,al
	in      al,COUNNTER0
	mov     ah,al
	in      al,COUNNTER0
	xchg    al,ah
	cmp     ax,bx        ; If Current Time > Next Time then
	jb	_NsmWaitTime3
			     ;	Time elapssed = Current Time - Next Time
	mov     dx,ax        ; Else
	mov     ax,0FFFFH    ;  Timer has wrapped round.
	sub     ax,dx        ;   Time elapsed = FFFF - Next Time
	add     ax,bx        ;                          + Current Time
	jmp	_NsmWaitTime4
_NsmWaitTime3:		    ; There is 2387 per millisecond.
	sub     ax,bx
	not     ax
	inc     ax
_NsmWaitTime4:
	cmp     ax,2387      ; If elapsed time < 2387 ticks
	jb	_NsmWaitTime2
			     ;	Try another next time.
	loopw	_NsmWaitTime1
			     ; End For CX.
			     ; There is 2387 per millisecond.
ELSE
; DH_PROCBLOC; 1ms timeout; non-interruptible
	push	di
	pushf
_NsmWaitTime1:
	mov	cx,[bp].NsmWaitTime_Params.interval
	mov	bx, offset _NsmWaitTime	; id_low
	xor	di,di		; interval_high
	mov	ax,cs		; id_high
	cli
;	mov	dh,01		; non-interruptible
;	mov	dl,DH_PROCBLOC
	mov	dx,0104h
	call	dword ptr [DH_Addr]
	jnc	short _NsmWaitTime1	; event wakeup
	jnz	short _NsmWaitTime1	; interrupted
	popf
	pop	di
ENDIF
	pop	bp
	ret
;----------------------------------------------------------------------------

;*****************************************************************************
;
;      "IndicationOn"
;
;      The Protocol calls this function to indicate that it is ready to receive
;      communications from the MAC. Since these calls can be nested the flag
;      Indication must be a counter. Indications are on when equal to 0.
;      If there is data in system memory that could not be sent to the protocol
;      when indications were off then an interrupt must be generated to allow
;      this data to be transferred.
;
;*****************************************************************************
IndOn:
	cli			;Disable CPU interrupts.
	push	bp		;Parameters are passed in a far pascal format.
	mov     bp,sp           ;That is they are passed on the stack and the
	push    ds              ;called routine must remove them on return.
	mov     ds, [bp].stdparm.std_ds
IFNDEF  OS2
	inc	word ptr Indication
	cmp	word ptr Indication,ON
ELSE
	lock	inc	word ptr Indication
ENDIF
	jne	IndOnX
	push	FALSE
	push	offset NICContext
	call	_HsmRxFlowControl
	lock	btr	dvrflags,RxNotComplete
	jnc	short IndOn1
	call	_HsmForceInterrupt
IndOn1:
	add	sp,4
IndOnX:
	mov     ax,SUCCESS      ;Default Return "success".
	pop	ds		;Reload DS-reg.
	pop     bp
	retf    INDRET          ;Pop parameters & exit.
;-----------------------------------------------------------------------------

;*****************************************************************************
;
;       "IndicationOff"
;
;       Disbles MAC Indications to protocol. The Protocol calls this function
;       to indicate that it is NOT ready to receive communications from the MAC.
;       Since these calls can be nested the flag Indication must be a counter.
;       Indications are on when equal to 0.
;
;*****************************************************************************
IndOff:
	cli                         ;Disable CPU interrupts.
	push	bp
	mov     bp,sp
	push    ds                  ;Set our DS-reg.
	mov	ds, [bp].stdparm.std_ds

	lock	dec	word ptr Indication ;Decrement "indications" level.

	push	TRUE
	push	offset NICContext
        call    _HsmRxFlowControl                       ;HsmRxFlowControl
	add	sp,4

IndOffX:
	mov	ax,SUCCESS	    ;Default Return "success".
	pop	ds		;Reload DS-reg.
	pop     bp
	retf    INDRET          ;Pop parameters & exit.
;-----------------------------------------------------------------------------

;*****************************************************************************
;
;       "_NsmHsmStateChange"
;
;*****************************************************************************
PUBLIC _NsmHsmStateChange
_NsmHsmStateChange:
	    ret
;-----------------------------------------------------------------------------

;*****************************************************************************
;
;       "TransmitChain"
;
;        This routine takes the transmit buffer descriptors given
;        by the protocol and puts them onto the TxQue
;
;*****************************************************************************
public	TxData		; << debug >>
TxData:
	push	bp		   ; Save BP-reg.
	mov     bp,sp              ;Set up frame ptr.
	push    ds                 ;Set our DS-reg.
	mov     ds, [bp].stdparm.std_ds
	push    si                 ;Save working regs.
	push    di
	push	es
	pushf
	mov	es, [bp].stdparm.std_ds
	mov	ax,BAD_FUNCTION    ;Get "Invalid Function" code.
	test	dvrflags,mask Reset
				   ;Is a hardware reset pending?
	jnz     TxExit             ;Yes?? Exit with "Invalid Function" code.
	test    mc_stat.ms_flag,mask open_close_status
				   ;Is adapter currently open?
	jz      TxExit             ;No?? Exit with "Invalid Function" code.
	lds     si,[bp].txparm.tx_buf
				   ;Get transmit-buffer frame ptr.
	mov     ax,ds:[si].txframedcr.tx_immlen
				   ;Get immediate-data byte count.
	cmp     ax,MXIMMED         ;Too much immediate data?
	ja      TxBadP             ;Yes?? Sayonara!!
TxDscr:
	mov	cx,ds:[si].txframedcr.tx_bufcnt
TxDscr1:
	mov	dx,cx
				   ;Get transmit-buffer descriptors count.
	cmp     cx,NSM_MAX_TX_FRAGS    ;Too many descriptors for our logic?
	ja      TxBadP             ;Yes?? Sayonara!!
        or      cx,cx              ;Any buffer descriptors(i.e.regular data)?
        jnz     TxLen              ;Yes, accumulate packet length.
	or      ax,ax              ;If no descriptors, any immediate data?
	jz      TxBadP     ;No, Null or Invalid packet post error code.
	jmp     TxQue1     ;Otherwise Queue packet in TxQ.
TxLen:  add     si, SIZEOF txbufdcr
	mov     bx, ds:[si].txbufdcr.tx_dcrlen
				   ;Calculate Packet Length.
	add     ax, bx
TxLen1: loopw   TxLen
	or      ax,ax              ;Check if TxPacket = 0
	jz      TxBadP             ; Yes: Exit with Invalid Packet
	cmp     ax,PKTMAX          ; No: Check if TxPacket <= 1514
	jbe     TxQue1             ;      No: Exit with Invalid Packet
TxBadP: mov     ax,BAD_PARAMETER
TxExit: jmp     TxReturn
TxQue1:
	mov     ax,es
	mov	ds,ax
	inc	dx
	push	dx
	push	offset NICContext
	call	_HsmGetTxDescList
	add	sp,4
	cmp	dx,0
	jne     TxQue2
	cmp     ax,0
	jne	TxQue2
	inc	NICContext.HsmContext.TxNoResource
	mov	ax,NO_RESOURCES
	jmp     TxReturn
TxQue2:
	mov     es,dx
	mov     di,ax              ; Load up HsmPkt descriptor.
	mov     bx,di
	push	bx	;1
	mov	ax,[bp].txparm.handle
	mov	cx,[bp].txparm.ProtID
	mov	es:[bx].HsmPktDesc.handle.Handle_str.handle,ax
	mov     es:[bx].HsmPktDesc.handle.Handle_str.ProtID,cx
	push    ds	;2
	mov	ax,ds
	lds     si,[bp].txparm.tx_buf
	mov     cx,[si].txframedcr.tx_immlen
	mov     es:[bx].HsmPktDesc.descByteCnt.dblword.lw,cx
	mov     es:[bx].HsmPktDesc.descByteCnt.dblword.hw,0
	add     di,HsmPktDesc.frags
	or	cx,cx
	jz	TxQue6_1
	mov     es:[di].HsmFrag.cnt.dblword.lw,cx
	mov     es:[di].HsmFrag.cnt.dblword.hw,0
	lds	si,[si].txframedcr.tx_immbuf
	push	di	;3
IFNDEF  OS2
	pushf		;4
	cli
ELSE
TxQue2_21:
	pushf
	mov	al,1
TxQue2_22:
	test	es:[Tx_Immed_Sem],al
	jnz	short TxQue2_22
	cli
	xchg	es:[Tx_Immed_Sem],al
	or	al,al
	jz	short TxQue2_23
	popf
	jmp	short TxQue2_21
TxQue2_23:
ENDIF
	mov	di,word ptr es:Tx_Immed_Wr
	mov	ax,di
IFDEF OS2
	add	ax,64+16+2	     ; Immed data + Phys to Uvirs + null
ELSE
	add	ax,64		     ; Immed data
ENDIF				     ; terminatro
	cmp     ax,word ptr es:Tx_Immed_End
	jb	TxQue3
	mov	ax,word ptr es:Tx_Immed_Start
TxQue3:
	mov	word ptr es:Tx_Immed_Wr,ax
IFDEF  OS2
	mov	byte ptr es:[Tx_Immed_Sem],0
	mov	word ptr es:[di+64],0	; GDT Selector count
ENDIF
	popf		;4

IFDEF OS2
	mov	dx,es
	mov	ax,di
IF NOT NSM_DOUBLE_BUFFER EQ 1
	test	word ptr es:NICContext.HsmContext.hsmOptions,NEED_TX_PHYSADDRS
	jz	Tx_Que5
	push	ds	  ;4  ;Get 32-bit fragment physical address.
;	push	si
;	push	bx
	mov	ax,es
	mov	ds,ax
;	mov	si,di
;	mov	dl,DH_VIRTOPHYS
;	call	DH_Addr
;	mov	dx,bx
;	xchg	ax,dx

	mov	ax,di
	sub	ax,word ptr [Tx_Immed_Start]
	add	ax,word ptr [Tx_Immed_Start_Phys]
	mov	dx,word ptr [Tx_Immed_Start_Phys +2]

;	pop	bx
;	pop	si
	pop	ds	;4
ENDIF
ELSE
	mov	dx,es	       ;Get 32-bit fragment physical address.
	mov     ax,di
IF NOT NSM_DOUBLE_BUFFER EQ 1
	test	word ptr es:NICContext.HsmContext.hsmOptions,NEED_TX_PHYSADDRS
	jz      Tx_Que5
	xor     dx,dx
	mov     ax,es          ;Get 32-bit fragment physical address.
	shl     ax,1
	rcl     dx,1
	shl     ax,1
	rcl     dx,1
	shl     ax,1
	rcl     dx,1
	shl     ax,1
	rcl     dx,1
	add     ax,si
	adc	dx,0
ENDIF
ENDIF
Tx_Que5:
	rep	movsb
	pop	di	;3
	pop     ds	;2
	mov	es:[di].HsmFrag.fptr.farptr.sgm,dx
	mov	es:[di].HsmFrag.fptr.farptr.ofs,ax

; dx:ax = physical address. and what is this?
;IFDEF OS2
;	mov	si,es:[bx].HsmPktDesc.frags.fptr.farptr.ofs
;	add	si,64
;	mov	ax,0
;	mov	[si],ax
;ENDIF
	lds     si,[bp].txparm.tx_buf
	mov     cx,[si].txframedcr.tx_bufcnt
	cmp     cx,0
	jz     TxPacket
TxQue6:
	mov     dx,HSM_MAX_TX_FRAGS
	add     si,txframedcr.tx_dcr1
	add     di,sizeof HsmFrag
	dec     dx
	jnz	short TxQue7
	jmp	Tx_Que9

TxQue6_1:
	pop	ax	;2  ds  discard
	lds     si,[bp].txparm.tx_buf
	mov     cx,[si].txframedcr.tx_bufcnt
	mov     dx,HSM_MAX_TX_FRAGS
	add     si,txframedcr.tx_dcr1
;	add     di,HsmPktDesc.frags

TxQue7:
	push    cx
	mov     cx,[si].txbufdcr.tx_dcrlen
	mov     es:[di].HsmFrag.cnt.dblword.lw,cx
	mov     es:[di].HsmFrag.cnt.dblword.hw,0
	add     es:[bx].HsmPktDesc.descByteCnt.dblword.lw,cx
	adc     es:[bx].HsmPktDesc.descByteCnt.dblword.hw,0
			     ;Get 32-bit fragment physical address.
	push    dx
	mov	ax,[si].txbufdcr.tx_dcrbuf.farptr.sgm
	mov	dx,[si].txbufdcr.tx_dcrbuf.farptr.ofs
IFDEF OS2
	cmp	[si].txbufdcr.tx_dcrtyp,DS_GDT
	je	Tx_Que8
IF NOT NSM_DOUBLE_BUFFER EQ 1
	test	word ptr es:NICContext.HsmContext.hsmOptions,NEED_TX_PHYSADDRS
	jnz	Tx_Que8_1
ENDIF
    ; Convert to Virtual Address.
	push	si
	push	bx
	mov	bx,sp
	mov	si,ss:[bx+08H]
	mov	si,es:[si].HsmPktDesc.frags.fptr.farptr.ofs
	add	si,MXIMMED
	inc	word ptr es:[si]
	inc	word ptr es:[si]
	mov	bx,es:[si]
	mov	si,es:[si+bx]
	push	si
	mov	bx,dx
	mov	dh,1
	mov	dl,DH_PHYSTOGDT
	call	es:DH_Addr
	mov	dx,0
	pop	ax
	pop	bx
	pop	si
	jmp	Tx_Que8_1
ENDIF
Tx_Que8:
IF NOT NSM_DOUBLE_BUFFER EQ 1
	test	word ptr es:NICContext.HsmContext.hsmOptions,NEED_TX_PHYSADDRS
	jz	Tx_Que8_1
   ; Convert to Physical Address.
IFDEF OS2
	push	ds	      ;Get 32-bit fragment physical address.
	push	si
	push	bx
	mov	ds,ax
	mov	si,dx
	mov	dl,DH_VIRTOPHYS
	call	DH_Addr
	mov	dx,bx
	pop	bx
	pop	si
	pop	ds
ELSE
	xor	ax,ax		;Get 32-bit fragment physical address.
	mov	dx,[si].txbufdcr.tx_dcrbuf.farptr.sgm
	shl	dx,1
	rcl	ax,1
	shl	dx,1
	rcl	ax,1
	shl	dx,1
	rcl	ax,1
	shl	dx,1
	rcl	ax,1
	add	dx,[si].txbufdcr.tx_dcrbuf.farptr.ofs
	adc	ax,0
ENDIF
ENDIF
Tx_Que8_1:
	mov	es:[di].HsmFrag.fptr.farptr.sgm,ax
	mov	es:[di].HsmFrag.fptr.farptr.ofs,dx
	pop     dx
	dec	dx
	jz	Tx_Que9
	add     di,sizeof HsmFrag
	jmp     Tx_Que10
Tx_Que9:
	les     bx,es:[bx].HsmPktDesc.lLink
	mov     ax,[bp].txparm.handle
	mov     es:[bx].HsmPktDesc.handle.Handle_str.handle,ax
	mov     ax,[bp].txparm.ProtID
	mov     es:[bx].HsmPktDesc.handle.Handle_str.ProtID,ax
	mov     es:[bx].HsmPktDesc.descByteCnt.dblword.lw,0
	mov     es:[bx].HsmPktDesc.descByteCnt.dblword.hw,0
	mov     di,bx
	add     di,HsmPktDesc.frags
	mov     dx,HSM_MAX_TX_FRAGS
Tx_Que10:
	add     si,sizeof txbufdcr
	pop     cx
	dec	cx
	jz	TxPacket
	jmp	TxQue7
TxPacket:
	pop	bx
	mov	ax,es
	mov     ds,ax
	push    es
	push    bx
	push	offset NICContext
        call	_HsmTransmit
	add     sp,6
	mov     ax,REQ_QUEUED
TxReturn:
	popf
	pop	es
	pop     di
	pop     si
	pop     ds
	pop     bp
	retf    TXRET
;-----------------------------------------------------------------------------

;*****************************************************************************
;
;        "_NSMTransmitComplete"
;
;          Notify the driver module of the completion of a packet transmit.
;  Status indicates success or failure. Could be called from HsmTransmit()
;  or NicService(). Nsm could pass himself a "handle" in HsmPktDesc.handle.
;
;*****************************************************************************
PUBLIC _NsmTransmitComplete
_NsmTransmitComplete:
	push    bp
	mov	bp,sp
	cmp	[bp].NsmTxComp_Params.handle.handle,0
        je      _NSMTransmitCompleteX1
	push	[bp].NsmTxComp_Params.handle.ProtID
	push    word ptr cc_tbl.cc_ID    ; Push details required for
	push	[bp].NsmTxComp_Params.handle.handle
					 ; Txconfirm.
	mov     ax,[bp].NsmTxComp_Params.status
					 ; Get HSM Transmit status
	cmp     ax,0                     ; Check if packet transmitted
					 ; successfully.
	jnz     _NSMTransmitCompleteX    ; No: Txconfirm with general failure.
	xor     ax,ax                    ; Yes: Txconfirm with success.
	jmp	_NSMTransmitComplete1
_NSMTransmitCompleteX:
	mov     ax,GEN_FAILURE
_NSMTransmitComplete1:
	push    ax
	push	word ptr protDS
	cld
	call    dword ptr pdsptbl.pd_txc ; Txconfirm call.
_NSMTransmitCompleteX1:
	pop	bp
	ret
;-----------------------------------------------------------------------------

;*****************************************************************************
;
;        "_NsmRxLookahead"
;
;        Notify the driver of the arrival of a received packet. "data" points
;   start of the data, "len" is the size of the lookahead area, "pktSize" is
;   the total size of the packet in bytes, if known.
;        For pipelined adapters this is set to -1."Status" is the error
;   status of the packet, if known.  If not doing pipelining, Nsm should call
;   HsmRxCopyData from this function to copy out contents of packet.
;   NsmRxLookahead can return a value that indicates that the packet should
;   be discarded. Another return code would indicate that the Nsm can't
;   accept the packet at this time.  Hsm should accept a call to
;   HsmRxCopyData regardless of whether or not we are doing pipelining.
;
;*****************************************************************************
PUBLIC _NsmRxLookahead
_NsmRxLookahead:
	push	bp
	mov     bp,sp
	lock	and	dvrflags,not mask RxTxferdataDone
_NsmRxLookahead1:
IFNDEF  OS2
	cmp     word ptr Indication,ON
	je      _NsmRxLookahead2
	jmp	_NsmRxLookahead4
ELSE
	cli		; refuse intr. for a short time.
	lock	sub	word ptr Indication,1
	jc	short _NsmRxLookahead2
	lock	inc	word ptr Indication
	sti
	lock	or	dvrflags,mask RxNotComplete
	mov	ax,NsmRxNotNow
	jmp	short _NsmRxLookaheadX
ENDIF
_NsmRxLookahead2:
        lock	or      dvrflags,mask Indic_comp
IFNDEF  OS2
	cli
	dec	word ptr Indication ;The MAC must disable Indications
ENDIF
if 0
	push	TRUE		    ;before calling RxLookahead.
	push	offset NICContext
	call	_HsmRxFlowControl
	add	sp,4
endif
	sti

	push	word ptr cc_tbl.cc_ID
	mov     byte ptr Rx_Indicate,OFF
	mov     ax, [bp].NsmRxLookahead_Params.pktsz
	push	ax
	mov	word ptr RxPktSize,ax
	mov     ax,[bp].NsmRxLookahead_Params.status
	mov	word ptr RxPktStatus,ax
	mov	ax,[bp].NsmRxLookahead_Params.handle
	mov	word ptr RxPktHandle,ax
	push	[bp].NsmRxLookahead_Params.LookaheadLength
	push	[bp].NsmRxLookahead_Params.LookaheadPointer.farptr.sgm
	push    [bp].NsmRxLookahead_Params.LookaheadPointer.farptr.ofs
	push	ds
	push    offset Rx_Indicate
	push	word ptr protDS
	cld
	call    dword ptr pdsptbl.pd_rxla       ; Call ReceiveLookahead.
	cmp     byte ptr Rx_Indicate,OFF        ; Are indications on?
	jne	_NsmRxLookahead4

;	cli
	lock	inc	word ptr Indication	; No,turn indications on.
if 0
	push	FALSE
	push	offset NICContext
	call	_HsmRxFlowControl
	add	sp,4
endif
	sti
_NsmRxLookahead4:
	test	dvrflags,mask RxTxferdataDone
	jnz	_NsmRxLookahead5
	mov	ax,NsmRxPktDiscard
	jmp     _NsmRxLookaheadX
_NsmRxLookahead5:
	mov	ax,NsmOK
_NsmRxLookaheadX:
	pop	bp
	ret
;-----------------------------------------------------------------------------

;*****************************************************************************
;
;        "TransferData"
;
;         This routine is called by the protocol from within ReceiveLookahead
;         to transfer received frame data from the MAC to the protocol.
;
;*****************************************************************************

;; define a temporary variable to hold receiving data len
;	dwBytes	dw	0	;Nani sunnen. use stack!!
dwBytes		equ	[bp-2]

RxData:
;	push	bp		;Save BP-reg.
;	mov     bp,sp           ;Set up frame ptr.
	enter	2,0
	push    ds              ;Set our DS-reg.
	mov     ds,[bp].stdparm.std_ds
	push	es		;Set our DS-reg.
	mov	es,[bp].stdparm.std_ds
	push	si		;Save working regs.
	push    di
	pushf
IFDEF OS2
	mov	word ptr Rx_Phys_Store,0
ENDIF
	mov	ax,word ptr RxPktSize
	sub	ax,[bp].tdparm.td_offset
	mov	word ptr RxPktRemaining,ax
	mov	bx,offset TranDataBuf
	lds	si,[bp].tdparm.td_bytes
	mov     ax,0
	mov     [si],ax
;;
;;;;	mov	cs:dwBytes, ax
	mov	dwBytes, ax
;;
	lds     si,[bp].tdparm.td_desc
	mov	dx,ds:[si].tdbufcnt.td_bufcnt
	add	si,tdbufcnt.td_buf
RxData2:
	mov     cx,HSM_MAX_RX_FRAGS
	mov	es:[bx].HsmPktDesc.descByteCnt.dblword.lw,0
	mov	es:[bx].HsmPktDesc.descByteCnt.dblword.hw,0
	mov     di,bx
	add     di,HsmPktDesc.frags
RxData4:
	mov	ax,ds:[si].tdbufdcr.tdb_len
	cmp     word ptr es:RxPktRemaining,ax
	jae	RxData41
	mov	ax,word ptr es:RxPktRemaining
RxData41:
	sub     word ptr es:RxPktRemaining,ax
	add     es:[bx].HsmPktDesc.descByteCnt.dblword.lw,ax
	mov     es:[di].HsmFrag.cnt.dblword.lw,ax
	mov     es:[di].HsmFrag.cnt.dblword.hw,0
	push    ds
	push    si
	lds     si,[bp].tdparm.td_bytes
;;
;;;;	add	cs:dwBytes, ax
	add	dwBytes, ax
;;
	add     [si],ax
	pop	si
	pop     ds
	push	dx
	mov	ax,ds:[si].tdbufdcr.tdb_ptr.farptr.sgm
	mov	dx,ds:[si].tdbufdcr.tdb_ptr.farptr.ofs
IFDEF OS2
	cmp	[si].tdbufdcr.tdb_typ,DS_GDT
	je	RxData42
IF NOT NSM_DOUBLE_BUFFER EQ 1
	test	word ptr es:NICContext.HsmContext.hsmOptions,NEED_RX_PHYSADDRS
	jnz	RxData6
ENDIF
    ; Convert to Virtual Address.
	push	cx
	push	si
	push	bx
	mov	si,offset Rx_Phys_Store
	inc	word ptr es:[si]
	inc	word ptr es:[si]
	mov	bx,es:[si]
	mov	si,es:[si+bx]
	push	si
	mov	cx,es:[di].HsmFrag.cnt.dblword.lw
	mov	bx,dx
	mov	dh,1
	mov	dl,DH_PHYSTOGDT
	call	es:DH_Addr
	mov	dx,0
	pop	ax
	pop	bx
	pop	si
	pop	cx
	jmp	RxData6
ENDIF
RxData42:
IF NOT NSM_DOUBLE_BUFFER EQ 1
	test	word ptr es:NICContext.HsmContext.hsmOptions,NEED_RX_PHYSADDRS
	jz      RxData6
   ; Convert to Physical Address.
IFDEF OS2
	push	ds	      ;Get 32-bit fragment physical address.
	push	si
	push	bx
	mov	ds,ax
	mov	si,dx
	mov	dl,DH_VIRTOPHYS
	call	DH_Addr
	mov	dx,bx
	xchg	ax,dx
	pop	bx
	pop	si
	pop	ds
ELSE
	xor	ax,ax		;Get 32-bit fragment physical address.
	mov	dx,[si].txbufdcr.tx_dcrbuf.farptr.sgm
	shl	dx,1
	rcl	ax,1
	shl	dx,1
	rcl	ax,1
	shl	dx,1
	rcl	ax,1
	shl	dx,1
	rcl	ax,1
	add	dx,[si].txbufdcr.tx_dcrbuf.farptr.ofs
	adc	ax,0
ENDIF
ENDIF
RxData6:
	mov	es:[di].HsmFrag.fptr.farptr.sgm,ax
	mov	es:[di].HsmFrag.fptr.farptr.ofs,dx
	pop	dx	   ; Transfer data buffer descriptor counter.
	cmp     word ptr es:RxPktRemaining,0
	jne     RxData5
	jmp     RxData7
RxData5:
	add     di,sizeof HsmFrag
	add     si,sizeof tdbufdcr
	dec     dx
	jz	RxData7
	dec	cx	  ; Hsm fragment counter.
	jz	RxData51
	jmp	RxData4
RxData51:
	or	es:[bx].HsmPktDesc.descByteCnt,HSM_MORE
	mov     bx,offset TranDataBuf1
	jmp     RxData2
RxData7:
	lds     si,[bp].tdparm.td_bytes
;;
;;;;	mov	bx, cs:dwBytes
	mov	bx, dwBytes
	mov [si], bx
;;

;;	mov     bx,[si]

	mov	ax,es
	mov     ds,ax
	push	offset TdStatus
	push	offset TranDataBuf
	push	bx
	push	word ptr [bp].tdparm.td_offset
	push	word ptr RxPktHandle
	push	offset NICContext
	call	_HsmRxCopyPkt
	add	sp,12
	lock	or	dvrflags,mask RxTxferdataDone
	mov	ax,SUCCESS	;Post "success" code.
RxDataX:
	popf
	pop	di
	pop     si
	pop	es
	pop	ds
;	pop     bp
	leave
	retf    RXRET           ;Pop parameters & exit.
;-----------------------------------------------------------------------------

;*****************************************************************************
;
;       "_NsmRxComplete"
;
;       Only called when doing receive pipelining (see
;   HsmContext.hsmOptions/nsmOptions). NsmRxLookahead would have been called
;   earlier with the size set to -1.
;       This upcall from the driver indicates that the receive packet is now
;   available for transfer.
;
;*****************************************************************************
PUBLIC _NsmRxComplete
_NsmRxComplete:
	ret
;-----------------------------------------------------------------------------

;*****
;        "NsmReceiveChain"
;*****
PUBLIC _NsmReceiveChain
_NsmReceiveChain::

NRC_handle	equ	bp+6
NRC_size	equ	bp+8
NRC_buf		equ	bp+10
NRC_count	equ	bp-10
NRC_len		equ	bp-8
NRC_ptr		equ	bp-6
NRC_ind		equ	bp-2

	cmp	word ptr [Indication],ON
	jnz	short _NsmRxChainX1
	lock dec word ptr [Indication]
	enter	10,0

	mov	ax,[NRC_buf]
	mov	dx,[NRC_buf][2]
	mov	cx,[NRC_size]
	mov	word ptr [NRC_count],1
	mov	[NRC_len],cx
	mov	[NRC_ptr],ax
	mov	[NRC_ptr][2],dx
	mov	byte ptr [NRC_ind],OFF
	mov	ax,ss
	mov	bx,sp
	mov	dx,bx
	add	bx,8

	push	word ptr [cc_tbl.cc_ID]
	push	cx
	push	word ptr [NRC_handle]
	push	ax		; descriptor
	push	dx
	push	ax		; Rx indication
	push	bx
	push	word ptr [protDS]
	cld
	call	dword ptr [pdsptbl.pd_rxch]

	cmp	byte ptr [NRC_ind],OFF
	setz	dh
	cmp	ax,1
	setbe	cl
	setnz	al	; queued - OK, other - Discard(include success)
	shr	dx,8
	mov	ch,0
	mov	ah,0
	shl	cx,Indic_comp
	lock add word ptr [Indication],dx
	lock or  [dvrflags],cx
	leave
	retn

_NsmRxChainX1:
	lock or [dvrflags], mask RxNotComplete
	mov	ax,NsmRxNotNow
	retn
;-----


;*****************************************************************************
;
;        "RxRelease"
;
;        This is a "dummy" routine, since this driver does not issue
;        ReceiveChain requests.
;
;*****************************************************************************
RxRelease:
IFNDEF  NECWARP
	mov     ax,UNSUPPORTED  ;Just indicate an error.
ELSE
	; the pair of NsmReceiveChain.
RR_handle	equ	bp+8
RR_MACDS	equ	bp+6

	push	bp
	mov	bp,sp
	push	ds
	mov	ds,[RR_MACDS]
	push	word ptr [RR_handle]
	push	offset NICContext
	call	_HsmRxFreePkt
	add	sp,4
	xor	ax,ax
	pop	ds
	pop	bp
ENDIF
	retf    RRRET           ;Pop parameters & exit.
;-----------------------------------------------------------------------------

;*****************************************************************************
;
;   "_NsmEnterCriticalSection"
;
;         Enter a critical section of code. Mode specifies what kind of
;   critical section you wish to protect.  This could be one of three values:
;   transmit, receive, all.
;
;*****************************************************************************
PUBLIC _NsmEnterCriticalSection
_NsmEnterCriticalSection:
IFNDEF  OS2
	   cmp   InCritical,0
	   jne   _NsmEnterCriticalSection1
	   pushf
	   cli
	   pop   ax
	   and   ax,mask Enabled_Ints
	   mov   word ptr IntrState,ax
_NsmEnterCriticalSection1:
	   inc   InCritical
_NsmEnterCriticalSectionX:
	   ret
ELSE
; cli may not be enough in SMP Environ.
_NsmEnterCrtclFlg	equ	bp+6	;flag - TX=0, RX=1, ALL=2
	push	bp
	mov	bp,sp
	push	si
	push	di
	mov	ax,[_NsmEnterCrtclFlg]
	mov	si,offset InCriticalTX
	mov	di,offset InCriticalRX
	mov	bx,offset IntrStateTX
	cmp	ax,HSM_TX_CRITICAL_SECTION
	jz	short _NsmEnterCrtclSecTXRX
	xchg	si,di
	mov	bx,offset IntrStateRX
	cmp	ax,HSM_RX_CRITICAL_SECTION
	jz	short _NsmEnterCrtclSecTXRX
	cmp	ax,HSM_ALL_CRITICAL_SECTION
	jz	short _NsmEnterCrtclSecALL
	jmp	short _NsmEnterCrtclSecErr
_NsmEnterCrtclSecTXRX:
	mov	al,1
	test	[si],al
	jnz	short _NsmEnterCrtclSecTXRX
	pushf
	cli
	xchg	[si],al
	test	al,al
	jz	short _NsmEnterCrtclSecX
	popf
	jmp	short _NsmEnterCrtclSecTXRX
_NsmEnterCrtclSecALL:
	mov	ax,0101h
	test	[si],al
	jnz	short _NsmEnterCrtclSecALL
	test	[di],al
	jnz	short _NsmEnterCrtclSecALL
	pushf
	cli
	xchg	[si],al
	test	al,al
	jnz	short _NsmEnterCrtclSecA1
	xchg	[di],ah
	test	ax,ax
	jz	short _NsmEnterCrtclSecX
	mov	byte ptr [si],0		;clear RX semaphore.
_NsmEnterCrtclSecA1:
	popf
	jmp	short _NsmEnterCrtclSecALL
_NsmEnterCrtclSecX:
				;CPU flags store.
	pop	word ptr [bx]	;ALL/RX use same address.
_NsmEnterCrtclSecErr:
	pop	di
	pop	si
	pop	bp
	ret
ENDIF
;-----------------------------------------------------------------------------

;*****************************************************************************
;
;   "_NsmEnterCriticalSection"
;
;         Leave a critical section of code.
;
;*****************************************************************************
PUBLIC _NsmLeaveCriticalSection
_NsmLeaveCriticalSection:
IFNDEF  OS2
	   cmp   InCritical,0
	   je    _NsmLeaveCriticalSectionX
	   dec   InCritical
	   jnz   _NsmLeaveCriticalSectionX
	   pushf
	   pop   ax
	   or    ax,word ptr IntrState
	   push  ax
	   popf
_NsmLeaveCriticalSectionX:
	   ret
ELSE
_NsmLeaveCrtclFlg	equ	bp+6	;flag - TX=0, RX=1, ALL=2
	push	bp
	mov	bp,sp
	push	si
	push	di
	mov	ax,[_NsmLeaveCrtclFlg]
	mov	dx,[IntrStateTX]
	mov	si,offset InCriticalTX
	mov	di,offset InCriticalRX
	cmp	ax,HSM_TX_CRITICAL_SECTION
	jz	short _NsmLeaveCrtclSecTXRX
	xchg	si,di
	mov	dx,[IntrStateRX]
	cmp	ax,HSM_RX_CRITICAL_SECTION
	jz	short _NsmLeaveCrtclSecTXRX
	cmp	ax,HSM_ALL_CRITICAL_SECTION
	jnz	short _NsmLeaveCrtclSecX
_NsmLeaveCrtclSecALL:
	mov	byte ptr [di],0
_NsmLeaveCrtclSecTXRX:
	mov	byte ptr [si],0
	test	dx,mask Enabled_Ints
	jz	short _NsmLeaveCrtclSecX	;previous intr state.
	sti
_NsmLeaveCrtclSecX:
	pop	di
	pop	si
	pop	bp
	ret
ENDIF
;-----------------------------------------------------------------------------

;*****************************************************************************
; Interrupt Service Routine.
;*****************************************************************************
TimerIsr:

IFDEF	OS2
	pushf			 ;OS/2 -- save CPU flags.
	cli
;	test	dvrflags,mask InTmrIsr
	lock	bts	dvrflags, InTmrIsr
	jc	short TimerIsrX
ELSE
	pushf
	call	dword ptr cs:TimerChain
				;Call Shared Interrupt routine.
	cli
	test	cs:dvrflags,mask InTmrIsr
	jz	TimerIsr1	; Are we currently in the Interrupt Service
	jmp	TimerIsrX	; Routine.
ENDIF
TimerIsr1:
IFDEF OS2
;	or	dvrflags,mask InTmrIsr
ELSE
	or	cs:dvrflags,mask InTmrIsr
	pushad
	push	ds
	push	es
	mov	cx,cs
	mov	ds,cx		;Set our DS-reg.
ENDIF
	cld
	sti
	test	dvrflags,mask Timer_On
	jz	TimerIsr3
	cmp	byte ptr Timer_Count,0
	jne	TimerIsr2
	lock	and	dvrflags,not mask Timer_On
	push	offset NICContext
	call	_HsmTimerEvent
	add	sp,2
	jmp	TimerIsr3
TimerIsr2:
	dec	byte ptr Timer_Count
TimerIsr3:
	lock	and	dvrflags,not mask InTmrIsr
IFNDEF OS2
	pop	es
	pop	ds
	popad
ENDIF
TimerIsrX:
IFDEF OS2
	popf
	retf
ELSE
	iret
ENDIF
;-----------------------------------------------------------------------------

;*****************************************************************************
; Interrupt Service Routine.
;*****************************************************************************
public	NsmIsr		; << debug >>
NsmIsr:
IFDEF	OS2		;KakiKaete Miyou.
			; Don't check machine status (CR0 low 16bit) register.
			; I guess 32bit OS/2 (Ver.2.0 or later) never use
			; real/protect mode switch, since 386 has V86 mode.
			; Moreover, OS receives interrupts and routes them to drivers.
	cli			;Real I-O interrupt -- disable CPU interrupts.
	lock bts dvrflags, InIsr	; already in service?
	jc	short NsmIsrOther
	cld
	push	offset NICContext	; disable H/W interrupt
        call	_HsmDisableNicInts	; and check Interrupt reason.
	add	sp,2

	test	al,2		; Receive event clear w/o process.
	setnz	dl		; Attention: ISR is "read-clear" register.
	mov	dh,0
	shl	dx, RxNotComplete	; bit 7
	lock or  dvrflags, dx	; delayed process after Indication On.
	shr	ax,1		; events acceptable now exist.
	jc	short NsmIsrIn

	push	offset NICContext	; re-enable H/W interrupt
	call	_HsmEnableNicInts
	add     sp,2
	lock btr dvrflags, InIsr	; clear semaphore
NsmIsrOther:
	stc	; Carry flag set means this interrupt may be done
	retf	; by other drivers. Required for shared Interrupt.


NsmIsrIn:
	mov	al,byte ptr mc_spec.mc_int.dblbyte.lb
				;OS/2 -- issue EOI thru the system.
	mov     dl,DH_EOI       ;(Required by OS/2 V2.0).
	call	dword ptr DH_Addr

	sti			;Re-enable CPU Interrupts.

NsmIsr0:
	push	offset NICContext
	call	_HsmService	;H/W-depended Interrupt service.
	add	sp,2

;IF 0
	push	offset NICContext	; check interrupt events occured
        call	_HsmDisableNicInts	; during previous service.
	add	sp,2

	test	al,2
	setnz	dl
	mov	dh,0
	shl	dx, RxNotComplete
	lock or  dvrflags, dx
	shr	ax,1
	jc	short NsmIsr0
;ENDIF

	lock btr  dvrflags, Indic_comp	;if some frames were received, 
	jnc	short NsmIsr1		; call IndicationComplete.
	push	word ptr cc_tbl.cc_ID
	push	word ptr protDS
	cld
	call	dword ptr pdsptbl.pd_icmp

NsmIsr1:
	test	dvrflags,(mask Int_req) or (mask Reset)
	jnz	short NsmIsrLP

NsmIsrExit:
	cli
	push	offset NICContext
	call	_HsmEnableNicInts
	add     sp,2
	lock and  dvrflags, not mask InIsr
	clc
	retf


NsmIsrLP:
IsrIntReq:
	cli
	cmp	word ptr Indication,ON       ;Are Indications enabled?
	jne	short NsmIsrExit
	lock btr  dvrflags, Int_req	     ;InterruptRequest?
	jnc	short IsrReset

	lock dec  word ptr Indication ;The MAC must disable Indications
	sti

	push	word ptr cc_tbl.cc_ID
	push    PAD
		;Status_Indicate allows the protocol to indicate if it
		;will allow any more indications after this one.
	mov      byte ptr IntReq_Indicate,OFF
	push     ds
	push     offset IntReq_Indicate
	push     INTERRUPT
	push	 word ptr protDS
	cld
	call     dword ptr pdsptbl.pd_stat

	cmp      byte ptr IntReq_Indicate,OFF ;Are indications on?
	mov	ah,0
	setz	al
	lock add  word ptr Indication,ax	;No,turn indications on.

	push    word ptr cc_tbl.cc_ID
	push	word ptr protDS
	cld
	call    dword ptr pdsptbl.pd_icmp


IsrReset:
	cli
	cmp	word ptr Indication,ON
	jne	short NsmIsrExit
	lock btr  dvrflags, Reset
	jnc	short IsrChkMore
	lock dec  word ptr Indication ;The MAC must disable Indications
	sti

	push	word ptr cc_tbl.cc_ID
	push    PAD
		;Status_Indicate allows the protocol to indicate if it
		;will allow any more indications after this one.
	mov      byte ptr Status_Indicate,OFF
	push     ds
	push     offset Status_Indicate
	push     STARTRESET
	push	 word ptr protDS
	cld
	call     dword ptr pdsptbl.pd_stat
	cmp      byte ptr Status_Indicate,OFF ;Are indications on?
	mov	ah,0
	setnz	al	; [On and Off] or [Leave and Off].
	lock sub  word ptr Indication,ax ; indication depth is 2.

	push     word ptr cc_tbl.cc_ID
	push     PAD
		;Status_Indicate allows the protocol to indicate if it
		;will allow any more indications after this one.
	mov      byte ptr Status_Indicate,OFF
	push     ds
	push     offset Status_Indicate
	push     ENDRESET
	push	 protDS
	cld
	call     dword ptr pdsptbl.pd_stat
	cmp      byte ptr Status_Indicate,OFF ;Are indications on?
	mov	ah,0
	setz	al
	lock add  word ptr Indication,ax

	push    word ptr cc_tbl.cc_ID
	push	word ptr protDS
	cld
	call    dword ptr pdsptbl.pd_icmp


IsrChkMore:	; more request checks are added below
	jmp	near ptr NsmIsr1

ELSE		; don't use below


IFDEF	OS2
	cli			;Real I-O interrupt -- disable CPU interrupts.
	smsw    ax              ;Get 80286 (and up) CPU machine-status word.
	push    ax              ;Save machine-status word for exit.
	test    al,001h         ;Are we currently in protected mode?
	jnz     IRDisI          ;Yes, disable interrupts.
	mov	dl,DH_REALTOPRO ;No, Change from Real to protected mode.
	call	dword ptr DH_Addr
IRDisI:
	; Are we currently in the Interrupt Service Routine.

;	test	dvrflags,mask InIsr
	lock	bts	dvrflags, InIsr
	jnc	short NsmIsr2
ELSE
	test	cs:dvrflags,mask InIsr
	jz	NsmIsr2 	;  No, Post Interrupt Service Routine Flag.
				;  Yes, Someone else owns the interrupt
ENDIF
IFDEF	OS2
	lock	or	dvrflags,mask NotOurInt
ELSE
	or	cs:dvrflags,mask NotOurInt
ENDIF
	jmp	IsrExit1

NsmIsr2:
IFDEF	OS2
;	or	dvrflags,mask InIsr
ELSE
	or	cs:dvrflags,mask InIsr
	push	ax
	push	bx
	push	cx
	mov	ax,ss		;Use our own stack.
	mov	bx,sp
	mov	cx,cs
	mov	ss,cx
	mov	sp,offset TopStak
	pushad			;DOS -- save CPU regs.
	push	ds
	push    es
	mov     cx,cs
	mov	ds,cx		;Set our DS-reg.
ENDIF
	cld
	push	offset NICContext
        call    _HsmDisableNicInts
	add     sp,2
        cmp	ax,0		;Did we generate interrupt
	jne	NsmIsr1 	;Yes, continue.
				;No, pass to someone else.
	lock	or	dvrflags,mask NotOurInt
	jmp	IRExit
NsmIsr1:
IFDEF OS2
	mov	al,byte ptr mc_spec.mc_int.dblbyte.lb
				;OS/2 -- issue EOI thru the system.
	mov     dl,DH_EOI       ;(Required by OS/2 V2.0).
	call	dword ptr DH_Addr
ELSE
	mov	al,EOI
	cmp     byte ptr mc_spec.mc_int.dblbyte.lb,8
	jb      ONLYEOI1
	out     ICR2,al         ;Issue EOI to 2nd 8259, if required.
ONLYEOI1:
	out     ICR1,al         ;Issue EOI to 1st 8259.
ENDIF
	sti			;Re-enable CPU Interrupts.
	push	offset NICContext
	call	_HsmService
	add	sp,2
if 0
IFDEF OS2
	test	dvrflags, mask Indic_comp
	jz	NsmIsr3
	push    word ptr cc_tbl.cc_ID
	push	word ptr protDS
	cld
	call	dword ptr pdsptbl.pd_icmp
	and	dvrflags, not mask Indic_comp
NsmIsr3:
ENDIF
endif
	test	dvrflags,mask Int_req	     ;InterruptRequest?
	jz      Reset?                       ;No, check for Reset
	cmp     word ptr Indication,ON       ;Are Indications enabled?
	jne     Reset?                       ;No, check for Reset
	lock	and	dvrflags, not (mask Int_req) ;Clear InterruptRequest flag

	cli
	lock	dec	word ptr Indication ;The MAC must disable Indications
if 0
	push	TRUE		    ;before calling Interrupt.
	push	offset NICContext
	call	_HsmRxFlowControl
	add	sp,4
endif
	sti

	push	word ptr cc_tbl.cc_ID
	push    PAD
		;Status_Indicate allows the protocol to indicate if it
		;will allow any more indications after this one.
	mov      byte ptr IntReq_Indicate,OFF
	push     ds
	push     offset IntReq_Indicate
	push     INTERRUPT
	push	 word ptr protDS
	cld
	call     dword ptr pdsptbl.pd_stat
	cmp      byte ptr IntReq_Indicate,OFF ;Are indications on?
	jne	 IntReq_Off

	cli
	lock	inc	word ptr Indication	      ;No,turn indications on.
if 0
	push	FALSE
	push	offset NICContext
	call	_HsmRxFlowControl
	add	sp,4
endif
	sti

IntReq_Off:
;IFDEF OS2
if 0
	push    word ptr cc_tbl.cc_ID
	push	word ptr protDS
	cld
	call    dword ptr pdsptbl.pd_icmp
endif
;ELSE
	lock	or      dvrflags,mask Indic_comp  ; Request IndicationComplete
;ENDIF                                     ; function.
Reset?: test    dvrflags,mask Reset       ;Is there a reset pending?
	jz      ChkMore?                  ;No, check for any more interrupts.
;
;       Check required here to see wether the HSM is transmitting or not
;       if so then don't reset device.
;
	cmp     word ptr Indication,ON ;Yes, Are Indications enabled?
	jne	ChkMore?	       ;No, Exit routine.

	cli
	lock	dec	word ptr Indication ;The MAC must disable Indications
if 0
	push	TRUE		    ;before calling StartReset
	push	offset NICContext
	call	_HsmRxFlowControl
	add	sp,4
endif
	sti

	push	word ptr cc_tbl.cc_ID
	push    PAD
		;Status_Indicate allows the protocol to indicate if it
		;will allow any more indications after this one.
	mov      byte ptr Status_Indicate,OFF
	push     ds
	push     offset Status_Indicate
	push     STARTRESET
	push	 word ptr protDS
	cld
	call     dword ptr pdsptbl.pd_stat
	cmp      byte ptr Status_Indicate,OFF ;Are indications on?
	jne	 INDS_OFF1

	cli
	lock	inc	word ptr Indication	     ;No,turn indications on.
if 0
	push	FALSE
	push	offset NICContext
	call	_HsmRxFlowControl
	add	sp,4
endif
	sti

INDS_OFF1:

	cli
	lock	dec	word ptr Indication ;The MAC must disable Indications
if 0
	push	TRUE		    ;before calling EndReset.
	push	offset NICContext
	call	_HsmRxFlowControl
	add	sp,4
endif
	sti

	push     word ptr cc_tbl.cc_ID
	push     PAD
		;Status_Indicate allows the protocol to indicate if it
		;will allow any more indications after this one.
	mov      byte ptr Status_Indicate,OFF
	push     ds
	push     offset Status_Indicate
	push     ENDRESET
	push	 protDS
	cld
	call     dword ptr pdsptbl.pd_stat
	cmp      byte ptr Status_Indicate,OFF ;Are indications on?
	jne	 INDS_OFF2

	cli
	lock	inc	word ptr Indication	      ;No,turn indications on.
if 0
	push	FALSE
	push	offset NICContext
	call	_HsmRxFlowControl
	add	sp,4
endif
	sti

INDS_OFF2:
;IFDEF OS2
if 0
	push    word ptr cc_tbl.cc_ID
	push	word ptr protDS
	cld
	call    dword ptr pdsptbl.pd_icmp
endif
;ELSE
	lock	or      dvrflags, mask Indic_comp     ;Set indication complete flag.
;ENDIF
	lock	and     dvrflags,not (mask Reset)     ;Clear Reset Flag.
ChkMore?:
	cli
;
	cmp	word ptr Indication, ON
	jnz	short IRExit	; if Indication is OFF, leave interrupt disable.
;
	push	offset NICContext
	call	_HsmEnableNicInts
	add     sp,2
IRExit:
	lock	and	dvrflags,not mask InIsr
IFNDEF OS2
	pop	es		;DOS -- reload all registers.
	pop     ds
	popad
	mov	ss,ax		;restore original stack
	mov	sp,bx
	pop	cx
	pop	bx
	pop	ax
ENDIF
IFNDEF OS2
	test	cs:dvrflags, mask Indic_comp  ; Is indicationcomplete requested?
	jz      IsrExit1                   ; No,check for transmitted packets.
	test	cs:dvrflags, mask Indic_called	;Already in indication Complete?
	jnz	IsrExit1
	or	cs:dvrflags, mask Indic_called
ELSE
	lock	btr	dvrflags, Indic_comp
	jnc	short IsrExit1
;	mov	byte ptr Ctx_Indic_Req, 1	; clear in NsmArmIndicComp
;	lock	bts	dvrflags, Indic_called
;	jc	short IsrExit1
ENDIF

IFNDEF OS2
	push	ax
	push	bx
	push	cx
	mov	ax,ss		;Use our own stack.
	mov	bx,sp
	mov	cx,cs
	mov	ss,cx
	mov	sp,offset TopStakind
	pushad			;DOS -- save CPU regs.
	push	ds
	push    es
	mov     cx,cs
	mov	ds,cx		;Set our DS-reg.

	and	dvrflags, not mask Indic_comp
ENDIF
;IFNDEF  OS2
	sti                                ;Yes,clear the request flag.
	push	word ptr cc_tbl.cc_ID
	push	word ptr protDS
	cld
	call	dword ptr pdsptbl.pd_icmp
;ELSE
;	mov	eax, Hook_Data	; not use
;	mov	ebx,[CtxHdl_IndicComp]
;	mov	ecx,-1
;	mov	dl, DH_ARMCTXHOOK
;	call	dword ptr [DH_Addr]
;ENDIF
	cli
IFNDEF OS2
	pop	es		;DOS -- reload all registers.
	pop     ds
	popad
	mov	ss,ax		;restore original stack
	mov	sp,bx
	pop	cx
	pop	bx
	pop	ax
	and	cs:dvrflags, not mask Indic_called
ELSE
	and	dvrflags, not mask Indic_called
ENDIF

IsrExit1:
IFDEF	OS2
	test	dvrflags,mask NotOurInt
ELSE
	test	cs:dvrflags,mask NotOurInt
ENDIF
	jnz	IsrChain
IFDEF	OS2
	pop	ax		;OS/2 -- get saved machine-status word.
	test    al,001h         ;Was this a "real-mode" interrupt?
	jnz     IRBye           ;No, just exit.
	mov	dl,DH_PROTOREAL ;Yes, Change from protected to real mode.
	call	dword ptr DH_Addr
IRBye:  clc
	retf
ELSE
	iret			;Exit.
ENDIF
IsrChain:
IFDEF	OS2
	lock	and	dvrflags,not mask NotOurInt
	pop	ax		;OS/2 -- get saved machine-status word.
	test    al,001h         ;Was this a "real-mode" interrupt?
	jnz	IRBye2		;No, just exit.
	mov	dl,DH_PROTOREAL ;Yes, Change from protected to real mode.
	call	dword ptr DH_Addr
IRBye2:	stc
	retf
ELSE
	and	cs:dvrflags,not mask NotOurInt
	pushf
	call	dword ptr cs:IntChain	 ;Call Shared Interrupt routine.
	iret				 ;Exit.
ENDIF

IFDEF  OS2	; call IndicationComplete in Kernel time.
public	NsmCtxIndicComp
NsmCtxIndicComp:
	pushf
	pushad
	push	ds
	push	es
	mov	ax,DGROUP
	mov	ds,ax
	mov	es,ax
NsmCtxIndicCompL:
	cli
	mov	al,0
	xchg	Ctx_Indic_Req,al
	or	al,al
	jz	short NsmCtxIndicCompE
	sti                                ;Yes,clear the request flag.
	push	word ptr cc_tbl.cc_ID
	push	word ptr protDS
	cld
	call	dword ptr pdsptbl.pd_icmp
	jmp	short NsmCtxIndicCompL
NsmCtxIndicCompE:
	lock and	dvrflags, not mask Indic_called
	pop	es
	pop	ds
	popad
	popf
	retf
ENDIF
ENDIF
;-----------------------------------------------------------------------------

;*****************************************************************************
;
;   "_NsmHsmEvent"
;
;
;*****************************************************************************
PUBLIC _NsmHsmEvent
_NsmHsmEvent:
	   ret
;-----------------------------------------------------------------------------

;*****************************************************************************
;
;	"_NsmLogicalToPhysical
;
;	 Convert a logical address to a physical address.  The physical
;  address returned should be a flat address (not segment:offset).
;
;*****************************************************************************
PUBLIC _NsmLogicalToPhysical
_NsmLogicalToPhysical:
	   push  bp
	   mov	 bp,sp
IFDEF OS2
	   push	 ds
	   push	 si
	   push	 bx
	   mov	 ds,[bp].NsmLogicalToPhysical_Params.\
				       Logical_Address.farptr.sgm
	   mov	 si,[bp].NsmLogicalToPhysical_Params.\
				       Logical_Address.farptr.ofs
	   mov	 dl,DH_VIRTOPHYS
	   call	 DH_Addr
	   mov	 dx,bx
	   xchg  ax,dx
	   pop	 bx
	   pop	 si
	   pop	 ds
ELSE
	   xor	 dx,dx		;Get 32-bit fragment physical address.
	   mov	 ax,[bp].NsmLogicalToPhysical_Params.\
				       Logical_Address.farptr.sgm
	   shl	 ax,1
	   rcl	 dx,1
	   shl	 ax,1
	   rcl	 dx,1
	   shl	 ax,1
	   rcl	 dx,1
	   shl	 ax,1
	   rcl	 dx,1
	   add	 ax,[bp].NsmLogicalToPhysical_Params.\
				       Logical_Address.farptr.ofs
	   adc	 dx,0
ENDIF
	   pop	 bp
	   ret
;-----------------------------------------------------------------------------

;*****************************************************************************
;
;    "NsmLogMessage"
;
;       Write an error message to whatever error logging mechanism the
;   Nsm supports. Hsm should use "\r\n" for line formatting.
;
;*****************************************************************************
PUBLIC _NsmStdMessage
_NsmStdMessage:
	push	bp
	mov     bp,sp
	test	dvrflags,mask Init_comp
	jnz	_NsmStdMessageX
	push	ds
	push	di
	mov	cx,[bp].NsmStdMessage_Params.Error_Code
	cmp	cx,STDMSG_INV_MEDIA
	jae	_NsmStdMessage1
	mov	di,offset Std_EMsg
	call	PutCSt
	mov	di, offset Hsm_Short_Name
	call	PutCSt
_NsmStdMessage1:
	mov	di,offset Std_EMsg1
_NsmStdMessage2:
	jcxz	_NsmStdMessage3
	mov	di,[di]
	dec	cx
_NsmStdMessage3:
	call	PutCSt
	pop	di
	pop     ds
_NsmStdMessageX:
	pop	bp
	ret
;-----------------------------------------------------------------------------

;*****************************************************************************
;
;   "__aNlshl"
;
;     Microsoft C compiler v7.0 generates this function call, which results
;  in it being added to the the end of the driver. This is not good since
;  the bottom of the driver is discarded after initialisation, therefore
;  this function has to be added to the NSM so the library isn't needed.
;     This function gives a long word logical shift left.
;
;*****************************************************************************
PUBLIC __aNlshl
__aNlshl:
IFDEF  NECWARP
	shld	dx,ax,cl
	shl	ax,cl
ELSE
	   xor	ch,ch
	   jcxz __aNlshlX
__aNlshl1:
	   shl  ax,1
	   rcl  dx,1
	   loopw __aNlshl1
ENDIF
__aNlshlX:
	   ret
;-----------------------------------------------------------------------------

;*****************************************************************************
;
;   "__aNulshr"
;
;     Microsoft C compiler v7.0 generates this function call, which results
;  in it being added to the the end of the driver. This is not good since
;  the bottom of the driver is discarded after initialisation, therefore
;  this function has to be added to the NSM so the library isn't needed.
;     This function gives a unsigned long word logical shift right.
;
;*****************************************************************************
PUBLIC __aNulshr
__aNulshr:
IFDEF  NECWARP
	shrd	ax,dx,cl
	shr	dx,cl
ELSE
	   xor  ch,ch
	   jcxz __aNulshrX
__aNulshr1:
	   shr  dx,1
	   rcr  ax,1
	   loopw __aNulshr1
ENDIF
__aNulshrX:
	   ret
;-----------------------------------------------------------------------------

;*****************************************************************************
;
;   "_NSM_MEMCPY"
;
;
;*****************************************************************************
PUBLIC _NSM_MEMCPY
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
	   mov	al,0
	   shr  cx,1
	   rcr	al,1
	   shr  cx,1
	   rcr	al,1
	   cld
	   rep	movsd
	   mov	cl,al
	   shr	cx,7
	   rep	movsw
	   jnc  _NSM_MEMCPYX
	   movsb
_NSM_MEMCPYX:
	   pop	es
	   pop	ds
	   pop  si
	   pop  di
	   pop	bp
	   ret
;-----------------------------------------------------------------------------

;*****************************************************************************
;
;   "_NSM_MEMZERO"
;
;
;*****************************************************************************
PUBLIC _NSM_MEMZERO
_NSM_MEMZERO:
	   push bp
	   mov  bp,sp
	   push di
	   mov	di,[bp].NSM_MEMZERO_Params.Near_Address
	   mov	cx,[bp].NSM_MEMZERO_Params.Block_Size
           mov  eax,0
           mov  bl,cl
           shr  cx,2
	   cld
	   rep	stosd
	   mov	cl,bl
           and  cl,3
           rep  stosb
	   pop  di
	   pop	bp
	   ret
;-----------------------------------------------------------------------------

;*****************************************************************************
;
;   "crc32"
;
;
;*****************************************************************************
PUBLIC _crc32
_crc32:
	push	bp
	mov	bp, sp
	push	eax
	push	ecx
	push	edx
	push	si
	push	di
	push	ds
	push	es
	mov	ax,ds
	mov	es,ax
	lds	si, [bp].crc32_Params.Buffer	; ptr
	mov	di, [bp].crc32_Params.Len	; len
	sub	edx, edx	; crcval = ~0
	not	edx		;
	cld

;
;	as an optimization, we only read DWORDs from the data buffer
;	and we do as many DWORDs as we can before worrying about 
;	trailing bytes.
;
	cmp	di, 4
	jl	trailingBytes	; less than one DWORD?
	shr	di, 2		; compute number of full DWORDs
dwordLoop:
	movzx	bx, dl
	lodsd
	xor	bl, al
	shl	bx, 2
	mov	ecx, es:crc32table[bx]
	shr	edx, 8		; crctmp = crcval >> 8
	xor	edx, ecx
	movzx	bx, dl
	xor	bl, ah
	shr	eax, 16
	shl	bx, 2
	mov	ecx, es:crc32table[bx]
	shr	edx, 8		; crctmp = crcval >> 8
	xor	edx, ecx
	movzx	bx, dl
	xor	bl, al
	shl	bx, 2
	mov	ecx, es:crc32table[bx]
	shr	edx, 8		; crctmp = crcval >> 8
	xor	edx, ecx
	movzx	bx, dl
	xor	bl, ah
	shl	bx, 2
	mov	ecx, es:crc32table[bx]
	shr	edx, 8		; crctmp = crcval >> 8
	xor	edx, ecx
	dec	di
	jnz	dwordLoop
	mov	di, [bp].crc32_Params.Len	; get byte count back
	and	di, 3				; any trailers?
	jz	fini
trailingBytes:
;
;	now handle any remaining bytes
;
	movzx	bx, dl
	lodsd
	xor	bl, al
	shl	bx, 2
	mov	ecx, es:crc32table[bx]
	shr	edx, 8		; crctmp = crcval >> 8
	xor	edx, ecx
	dec	di
	jz	fini
	movzx	bx, dl
	xor	bl, ah
	shr	eax, 16
	shl	bx, 2
	mov	ecx, es:crc32table[bx]
	shr	edx, 8		; crctmp = crcval >> 8
	xor	edx, ecx
	dec	di
	jz	fini
	movzx	bx, dl
	xor	bl, al
	shl	bx, 2
	mov	ecx, es:crc32table[bx]
	shr	edx, 8		; crctmp = crcval >> 8
	xor	edx, ecx
fini:
	not	edx
	mov	es:result, edx
	pop	es
	pop	ds
	pop	di
	pop	si
	pop	edx
	pop	ecx
	pop	eax
	mov	ax, word ptr es:result.dblword.lw
	mov	dx, word ptr es:result.dblword.hw
	pop	bp
	ret
;-----------------------------------------------------------------------------

IFDEF OS2
_TEXT	  ends
ELSE
_TEXT	ends
_DATA	segment public use16 'DATA'
_DATA	ends
ENDIF
    end

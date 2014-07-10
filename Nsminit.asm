;*****************************************************************************
;*              National Semiconductor Company Confidential                  *
;*                                                                           *
;*                          National Semiconductor                           *
;*                       NDIS 2.0.1 MAC device driver                        *
;*   Code for National Semiconductor's CDI driver Initialisation  portion.   *
;*                                                                           *
;*      Source Name:    NSMINIT.ASM                                          *
;*      Authors:                                                             *
;*                       Frank DiMambro                                      *
;*                                                                           *
;*   $Log:   /home/crabapple/nsclib/tech/ndis2/nsm/vcs/nsminit.asv  $		 *
;	
;	   Rev 1.15  05/17/00 03:46p IP
;			Fixed EMM386 conflict 	
;	   Rev 1.4   10/06/95 13:17:38   frd
;	No change.
;	
;	   Rev 1.3   10/06/95 13:17:18   frd
;	
;	   Rev 1.0   09/14/95 16:55:58   frd
;	Initial revision.
;	
;	   Rev 1.14   26 May 1995 12:29:34   FRD
;	
;	
;	   Rev 1.13   26 May 1995 11:48:56   FRD
;	
;	
;	   Rev 1.12   17 May 1995 16:54:00   FRD
;	
;	
;	   Rev 1.11   17 May 1995 16:50:26   FRD
;	
;	
;	   Rev 1.10   17 May 1995 16:35:46   FRD
;	Removing Bus type Banner text.
;	
;	   Rev 1.9   17 May 1995 14:35:40   FRD
;	Removing the Nms_Delay external declaration
;	
;	   Rev 1.8   01 May 1995 13:52:08   FRD
;	Modified include definitions to allow the driver to be
;	built in the nsclib directory.
;	
;	   Rev 1.7   26 Apr 1995 09:53:08   FRD
;	
;	
;	   Rev 1.6   12 Apr 1995 16:53:50   FRD
;	
;	
;	   Rev 1.0   12 Apr 1995 14:08:20   FRD
;	
;	
;	   Rev 1.5   12 Apr 1995 13:13:20   FRD
;	
;*
;*	   Rev 1.4   12 Apr 1995 13:11:46   FRD
;*
;*                                                                           *
;*****************************************************************************

INCLUDE cdi.inc
INCLUDE nsm.inc
INCLUDE hsm.inc
INCLUDE NSMdef.inc
INCLUDE NSMSTDIO.inc

IFDEF OS2
extern OS2_Hdr:FAR16
extern OS2_Global_sel:FAR16
extern Rx_Phys_Store:FAR16
extern DH_Addr:FAR16
extern action:FAR16
extern NICContext:FAR16
extern cc_tbl:FAR16
extern mc_stat:FAR16
extern mc_spec:FAR16
extern pmgrnam:FAR16
extern pmgrhdl:FAR16
extern pmblock:FAR16
extern ddsptbl:FAR16
extern dvrflags:FAR16
extern MediaTypeStore:FAR16
extern NSSNAIStableptr:FAR16
extern NSSNAISIntName:FAR16
extern NSSNAISIntVerMaj:FAR16
extern NSSNAISIntVerMin:FAR16
extern NSSNAISStatBufSz:FAR16
extern TranDataBuf:FAR16
extern TranDataBuf1:FAR16
extern Tx_Immed_Start:FAR16
extern Tx_Immed_Rd:FAR16
extern Tx_Immed_Wr:FAR16
extern Tx_Immed_End:FAR16
extern Tx_Immed_Start_Phys:FAR16
extern NSM_Heap:FAR16
extern dta_end:FAR16
extern _HsmInitContext:NEAR16
extern _HsmFindAdapter:NEAR16
extern _HsmValidateContext:NEAR16
extern _HsmCheckNic:NEAR16
extern NSSNAIS_UpdatStat:NEAR16
extern TTLMsg:FAR16
extern TTLMsgEnd:FAR16
extern Banner:FAR16
extern OS_Banner:FAR16
extern NDIS_VStr:FAR16
extern Dvr_Ver:FAR16
extern New_line:FAR16
extern INSTDvr:FAR16
extern PMEMsg:FAR16
extern PMVEMsg:FAR16
extern PIEMsg:FAR16
extern PICEMsg:FAR16
extern HiMMsg:FAR16
extern PEMsg1:FAR16
extern PEMsgK1:FAR16
extern PEMsgK2:FAR16
extern PEMsgK3:FAR16
extern PEMsgK4:FAR16
extern PEMsgK5:FAR16
extern PEMsgK6:FAR16
extern PEMsgK7:FAR16
extern PEMsgK8:FAR16
extern PEMsgK9:FAR16
extern PEMsgK10:FAR16
extern PEMsgK11:FAR16
extern PEMsgK12:FAR16
extern PEMsgK13:FAR16
extern PEMsgK14:FAR16
extern PEMsgK15:FAR16
extern PEMsgK16:FAR16
extern PEMsgK17:FAR16
extern PEMsgK18:FAR16
extern PEMsgD1:FAR16
extern PEMsgD2:FAR16
extern CfgMsg1:FAR16
extern CfgMsg2:FAR16
extern Std_EMsg:FAR16
extern Std_EMsg1:FAR16
extern CfgWMsg1:FAR16
extern CfgWTotal:FAR16
extern CfgWMsg2:FAR16
extern CfgWGt1:FAR16
extern CfgWMsg3:FAR16
extern GFEMsg:FAR16
extern PCISys:FAR16
extern EISASys:FAR16
extern MCASys:FAR16
extern ISASys:FAR16
extern PnPSys:FAR16
extern SysBad:FAR16
extern EisaDv:FAR16
extern NoEisaDv:FAR16
extern Find_Err:FAR16
extern Find_Err1:FAR16
extern TGMsg:FAR16
;extern CtxHdl_IndicComp:FAR16
;extern CtxHdl_IndicComp2:FAR16
;extern NsmCtxIndicComp:FAR16
;extern NsmCtxIndicComp2:FAR16
_TEXT	segment word public use16 'CODE'
		assume  cs:_TEXT,ds:_DATA
ELSE
extern DOS_Hdr:NEAR16
extern NICContext:NEAR16
extern cc_tbl:NEAR16
extern mc_stat:NEAR16
extern mc_spec:NEAR16
extern pmgrnam:NEAR16
extern pmgrhdl:NEAR16
extern pmblock:NEAR16
extern ddsptbl:NEAR16
extern dvrflags:NEAR16
extern MediaTypeStore:NEAR16
extern NSSNAIStableptr:NEAR16
extern NSSNAISIntName:NEAR16
extern NSSNAISIntVerMaj:NEAR16
extern NSSNAISIntVerMin:NEAR16
extern NSSNAISStatBufSz:NEAR16
extern TranDataBuf:NEAR16
extern TranDataBuf1:NEAR16
extern Tx_Immed_Start:NEAR16
extern Tx_Immed_Rd:NEAR16
extern Tx_Immed_Wr:NEAR16
extern Tx_Immed_End:NEAR16
extern Tx_Immed_Start_Phys:NEAR16
extern NSM_Heap:NEAR16
extern dta_end:NEAR16
extern _HsmInitContext:NEAR16
extern _HsmFindAdapter:NEAR16
extern _HsmValidateContext:NEAR16
extern _HsmCheckNic:NEAR16
extern NSSNAIS_UpdatStat:NEAR16
extern TTLMsg:NEAR16
extern TTLMsgEnd:NEAR16
extern Banner:NEAR16
extern OS_Banner:NEAR16
extern NDIS_VStr:NEAR16
extern Dvr_Ver:NEAR16
extern New_line:NEAR16
extern INSTDvr:NEAR16
extern PMEMsg:NEAR16
extern PMVEMsg:NEAR16
extern PIEMsg:NEAR16
extern PICEMsg:NEAR16
extern HiMMsg:NEAR16
extern PEMsg1:NEAR16
extern PEMsgK1:NEAR16
extern PEMsgK2:NEAR16
extern PEMsgK3:NEAR16
extern PEMsgK4:NEAR16
extern PEMsgK5:NEAR16
extern PEMsgK6:NEAR16
extern PEMsgK7:NEAR16
extern PEMsgK8:NEAR16
extern PEMsgK9:NEAR16
extern PEMsgK10:NEAR16
extern PEMsgK11:NEAR16
extern PEMsgK12:NEAR16
extern PEMsgK13:NEAR16
extern PEMsgK14:NEAR16
extern PEMsgK15:NEAR16
extern PEMsgK16:NEAR16
extern PEMsgK17:NEAR16
extern PEMsgK18:NEAR16
extern PEMsgD1:NEAR16
extern PEMsgD2:NEAR16
extern CfgMsg1:NEAR16
extern CfgMsg2:NEAR16
extern Std_EMsg:NEAR16
extern Std_EMsg1:NEAR16
extern CfgWMsg1:NEAR16
extern CfgWTotal:NEAR16
extern CfgWMsg2:NEAR16
extern CfgWGt1:NEAR16
extern CfgWMsg3:NEAR16
extern GFEMsg:NEAR16
extern PCISys:NEAR16
extern EISASys:NEAR16
extern MCASys:NEAR16
extern ISASys:NEAR16
extern PnPSys:NEAR16
extern SysBad:NEAR16
extern EisaDv:NEAR16
extern NoEisaDv:NEAR16
extern Find_Err:NEAR16
extern Find_Err1:NEAR16
extern TGMsg:NEAR16

_TEXT   segment dword public use16 'CODE'       ;DOS Driver contained in one segment.
		assume  cs:_TEXT,ds:_TEXT
ENDIF

;
;****** Driver Initialization ******
;
;*****************************************************************************
;
;      "Driver Initialization Routine"
;
;       This routine is called from the Strategy routine when an INIT packet
;       is received. At entry, ES:BX points to the INIT packet. The protocol
;       managers configuration data is accessed and used to configure the
;       driver. The driver is then "registered" with the protocol manager.
;       For OS/2, this code is then "dismissed" from memory. For DOS,
;       this code then becomes part of the output queue or input area.
;
;*****************************************************************************
PUBLIC DRInit
DRInit:
IFNDEF  OS2
	cli                     ;Disable CPU interrupts.
	mov     cs:SaveSP,sp    ;Switch to auxiliary stack.
	mov     cs:SaveSS,ss
	push    cs
	pop     ss
	mov     sp,offset StakTop
	sti                     ;Re-enable CPU interrupts.
	push    ax              ;Save remaining regs.
	push    cx
	push    dx
	push    si
	push    di
	push    ds
ENDIF
	push    bp              ;Save BP-reg.
	sub     sp,4            ;Reserve 2 words on stack.
	mov     bp,sp           ;Point to reserved words.
	mov     [bp],bx         ;Save packet address on stack.
	mov     [bp+2],es
IFDEF  OS2
;	int	3	; << debug >>
	mov     ax,es:word ptr[bx].SRqPkt.Dev_hlp_off
	mov     dx,es:word ptr[bx].SRqPkt.Dev_hlp_seg
	mov     word ptr DH_Addr.farptr.ofs,ax
	mov     word ptr DH_Addr.farptr.sgm,dx
IFNDEF OS2
	les     bx,[bp]          ;Reload INIT packet ptr.
	mov     word ptr dta_end,es:[bx].SRqPkt.Mem_end_seg
ENDIF
	mov     al,SYS_INFOSEG
	mov     dl,DH_GETDOSVAR
	call    dword ptr DH_Addr
	mov     es,ax
	mov     ax,es:[bx]
	mov     word ptr OS2_Global_sel,ax
	mov     ax,ds
ELSE
	mov     ax,cs
	mov     ds,ax
ENDIF
	mov     es,ax               ; Call HsmInitContext
	mov     ax,offset NICContext
	push    ax
	call    _HsmInitContext
	add     sp,2
	call    NSMInitContext
	mov	di,offset Banner    ;Display "title" message.
	call    PutCSt
	mov	di,offset OS_Banner    ;Display "title" message.
	call    PutCSt
	mov	cx,sizeof cctable.cc_name
	mov     di,offset cc_tbl.cctable.cc_name
				;open our current name
Hsm_NxtLtr:
	mov     al,[di]
	cmp     al,0
	je      Hsm_NxtLtr2
	mov     [di],al
	inc     di
	loopw   Hsm_NxtLtr
Nsm_NxtLtr2:
	cmp     cx,0
	jne     Nsm_NxtLtrX
Hsm_NxtLtr2:
	mov     al,'$'
	mov     [di],al
Nsm_NxtLtrX:
	stc
        mov     ax,kwordsAllowed.dblword.lw
        rcl     ax,1
	mov	Board1.ExpectedKeys.dblword.lw,ax
	mov     Board1.OptionalKeys.dblword.lw,ax

	mov	ax,kwordsAllowed.dblword.hw
        rcl     ax,1
	mov	Board1.ExpectedKeys.dblword.hw,ax
	mov     Board1.OptionalKeys.dblword.hw,ax

	mov	cx,MAXDVRINST
IFDEF   OS2
Trynewname:
	push    cx
	push    ds              ;Stack parameters to "open" our driver.
	push    offset cc_tbl.cctable.cc_name   ;open our current name
	push    ds
	push    offset pmgrhdl
	push    ds
	push    offset action
	push    0
	push    0               ; file size (long)
	push    0               ; file attribute
	push    1               ; open flag: file must exist
	push    SHARED_RW       ; open mode (share R/W private)
	push    0
	push    0               ; 0L reserved
	call    DOSOPEN
	test    ax,ax           ;Any errors?
	jnz     Name_ok         ;Current name ok
	inc     DriverInst
	push    word ptr pmgrhdl
				;Open was a success so must close
	call    DOSCLOSE        ;the open file and change driver name.
ELSE
Trynewname:
	push    cx
	mov     ah,OPEN_FILE    ;Open our driver.
	mov     al,SHARED_RW
	mov     dx,offset cc_tbl.cctable.cc_name
	push    cs              ;Save CS-reg.
	int     021h            ;Do desired DOS function.
	pop     ds              ;Reload DS-reg.
	jc      Name_ok

	inc     DriverInst
	mov     bx,ax           ;handle was returned in ax.
	mov     ah,CLOSE_FILE   ;Open was a success so must close
				;then open file and change driver name.
	push    cs              ;Save CS-reg.
	int     021h            ;Do desired DOS function.
	pop     ds              ;Reload DS-reg.
ENDIF
	mov     ax,ds
	mov     es,ax
	mov	cx,(sizeof cctable.cc_name)+3
	mov     al,0h
	mov     di,offset cc_tbl.cctable.cc_name
	repne   scasb
	sub     di,3
NoSet_Two:
	cmp     [di].dblbyte.lb,'0'
	jb      Set_Two
	cmp     [di].dblbyte.lb,'9'
	ja      Set_Two
	inc     [di].dblbyte.lb        ;Change the 2nd last character.
	jmp     Next_Try
Set_Two:
	mov     al,[di+1].dblbyte.lb
	mov     [di+2].dblbyte.lb,al
	mov     [di+1].dblbyte.lb,'1'
Next_Try:
	pop     cx
	loopw   Trynewname
TooManyDvrs:
	mov     di,offset INSTDvr
	call    PutCSt
	jmp     I_Fail
Name_ok:
	mov     al,byte ptr DriverInst
	cmp     al,0
	je      Name_ok1
	and     Board1.OptionalKeys,not mask PnPSerial_Fnd
Name_ok1:
	pop     cx
	mov     si, offset cc_tbl.cctable.cc_name
IFDEF OS2
	mov     di, offset OS2_Hdr.header.Hdr_Nam
ELSE
	mov     di, offset DOS_Hdr.header.Hdr_Nam
ENDIF
	mov     cx, 8
	mov     al,0
Next_DrvLtr:
	mov     ah,[si].dblbyte.lb
	inc     si
	cmp     al,ah
	je      Pad_Space
	mov     [di].dblbyte.lb,ah
	inc     di
	loopw   Next_DrvLtr
	jmp     Hardware_Srh
Pad_Space:
	mov     [di].dblbyte.lb,20h
	inc     di
	loopw   Next_DrvLtr
Hardware_Srh:
	mov	si, offset cc_tbl.cctable.cc_name
	mov	di, offset Hsm_Short_Name
;	mov	di, offset NICContext.HsmContext.DriverName
	mov     cx, (sizeof cctable.cc_name)+2
	rep	movsb
	mov	di,offset NICContext.HsmContext.pnpId
	mov	cx,4
Try_OEM_PnPId:
	mov	ax,[di].dblword.hw
	mov     Board1.DeviceID.dblword.hw,ax
	mov	ax,[di].dblword.lw
	mov	Board1.DeviceID.dblword.lw,ax
	push	di
	push	cx
	call	NsmFindHardware
	pop	cx
	pop	di
	cmp	byte ptr Board1.Cards_Presnt,0
	jne	Open_Protman
	dec	cx
	jcxz	Open_Protman
	add	di,4
	jmp	Try_OEM_PnPId
Open_Protman:
IFDEF OS2
	push    ds              ;Stack parameters for protocol-manager "open".
	push    offset pmgrnam
	push    ds
	push    offset pmgrhdl
	push    ds
	push    offset action
	push    0
	push    0               ; file size (long)
	push    0               ; file attribute
	push    1               ; open flag: file must exist
	push    SHARED_RW       ; open mode (share R/W private)
	push    0
	push    0               ; 0L reserved
	call    DOSOPEN
	test    ax,ax           ;Any errors?
	jz      I_opprot                ;Yes, sayonara!
	mov     di,offset PMEMsg
	call    PutCSt
	jmp     I_Fail
I_opprot:
	mov     pmblock.pm_req_block.pm_op,GET_PM_INFO
	push    0               ;Request protocol-manager data.
	push    0
	push    ds
	push    offset pmblock
	push    IONC_PM
	push    IOC_NC
	push    word ptr pmgrhdl
	call    DOSDEVIOCTL
	test    ax,ax           ;Any I/O errors?
	jz      I_gtprot
	mov     di,offset PIEMsg
	call    PutCSt
	jmp     I_Fail
I_gtprot:
ELSE
	mov     ah,OPEN_FILE    ;Open protocol-manager file.
	mov     al,SHARED_RW
	mov     dx,offset pmgrnam
	push    cs              ;Save CS-reg.
	int     021h            ;Do desired DOS function.
	pop     ds              ;Reload DS-reg.
	jnc     I_opprot        ;Error -- sayonara!
	mov     di,offset PMEMsg
	call    PutCSt
	jmp     I_Fail
I_opprot:
	mov     word ptr pmgrhdl,ax     ;Save file "handle".
	mov     pmblock.pm_req_block.pm_op,GET_PM_INFO
				;Request protocol-manager data.
	mov     bx,word ptr pmgrhdl
	mov     cx,PMSIZE
	mov     dx,offset pmblock
	mov     ah,IOCTL
	mov     al,RXCTRLDATA
	push    cs              ;Save CS-reg.
	int     021h            ;Do desired DOS function.
	pop     ds              ;Reload DS-reg.
	jnc     I_gtprot
	mov     di,offset PIEMsg
	call    PutCSt
	jmp     I_Fail
I_gtprot:
ENDIF
	mov     ax,word ptr pmblock.pm_req_block.pm_stat
				;Get protocol-manager return code.
	test    ax,ax           ;Any errors with protocol.ini information?
	jz      I_ckprotv       ;Yes, treat as initialization failure!
	mov     di,offset PICEMsg
	call    PutCSt
	jmp     I_Fail
I_ckprotv:
	cmp     pmblock.pm_req_block.pm_w1,PM_VEXP
				;Correct version of protocol manager?
	jae     I_pmvok         ;No, treat as initialization failure!
	mov     di,offset PMVEMsg
	call    PutCSt
	jmp     I_Fail
I_pmvok:
	les     bx,dword ptr pmblock.pm_req_block.pm_ptr1
				;Point to configuration-image data.
	call    Parse           ;"Parse" the data.
	test    ax,ax           ;Any errors?
	jz      I_SetS          ;No, set up segment values.
I_Err:  jmp     I_Fail          ;Error -- treat as initialization failure!
I_SetS:
	mov     di,offset Board1
	mov     bl,[di].BoardType.Cards_Presnt
	cmp     bl,0
	je      HSMFindCard
	mov     bh,0
; SLOT start is 0, default value is also 0. check always slot.
IFNDEF  NECWARP
	mov     al,byte ptr NICContext.HsmContext.Slot
	or      al,al
	jnz     Check_Slot
	jmp     Check_Serial
ENDIF
Check_Slot:
	dec     bx
	push    bx
	mov     al,byte ptr NICContext.HsmContext.Slot
	cmp     al,[di+bx].BoardType.slot
	je      Adapter_Found
	pop     bx
	cmp     bx,0
	jne     Check_Slot
; Skip other check. use PCI Slot only.
IFNDEF NECWARP
Check_Serial:
	mov     bl,[di].BoardType.Cards_Presnt
	mov     bh,0
	mov     ax,word ptr NICContext.HsmContext.uniqueId.dblword.hw
	or      ax,word ptr NICContext.HsmContext.uniqueId.dblword.lw
	jnz     Check_Next
	dec     bx
	push    bx
	jmp     Adapter_Found
Check_Next:
	dec     bx
	push    bx
	shl     bx,2
	mov     ax,NICContext.HsmContext.uniqueId.dblword.hw
	cmp     ax,[di+bx].BoardType.SerialID.dblword.hw
	jne     Check_Next1
	mov     ax,NICContext.HsmContext.uniqueId.dblword.lw
	cmp     ax,[di+bx].BoardType.SerialID.dblword.lw
	je      Adapter_Found
Check_Next1:
	pop     bx
	cmp     bx,0
	jne     Check_Next
ENDIF
	jmp     NoAdapter_Fail
Adapter_Found:
	pop     bx
	mov     al,[di+bx].BoardType.IntLines
	mov     NICContext.HsmContext.Irq.dblbyte.lb,al

	shl     bx,2
	mov     ax,[di+bx].BoardType.Cards_IOAddr.dblword.lw
	mov     NICContext.HsmContext.IOaddr.dblword.lw,ax

	mov     ax,[di+bx].BoardType.memaddrs.dblword.hw
	mov     NICContext.HsmContext.MEMaddr.dblword.hw,ax
	mov     ax,[di+bx].BoardType.memaddrs.dblword.lw
	mov     NICContext.HsmContext.MEMaddr.dblword.lw,ax
	jmp     I_SetS1
HSMFindCard:
	mov     ax,word ptr PnPEntry.farptr.sgm
	or      ax,word ptr PnPEntry.farptr.ofs
	jz      HSMFindCard1
	mov     di,offset Find_Err1
	call    PutCSt
	jmp     I_Fail
HSMFindCard1:
	cli
	mov     ax,offset NICContext
	push    ax
	call    _HsmFindAdapter ; Do Heristic method of finding an
	add     sp,2            ; adapter.
	sti
	cmp     ax,HsmOK         ;Any errors?
	je      I_SetS1          ;No, set up segment values.
	cmp     ax,HsmMultipleHsmsFound
	jne     I_FindFail
	mov     di,offset Find_Err
	call    PutCSt
I_FindFail:
	jmp     I_Fail
I_SetS1:

IFNDEF OS2
	mov     ah,DOSVERGET    ;Get DOS version number.
	push    cs              ;Save CS-reg.
	int     021h            ;Do desired DOS function.
	pop     ds              ;Reload DS-reg.
	cmp     al,5            ;Are we running under DOS V5.0 or better?
	jb      I_HardC         ;No, skip high-memory checks (can't load high).
	mov     ax,offset dvr_end ;Get ending driver offset.
	add     ax,15           ;"Round" ending offset up to a page boundary.
	shr     ax,4            ;Get number of driver "pages".
	mov     cx,cs           ;Get ending driver "page" address.
	add     ax,cx
	cmp     ax,word ptr dta_end
				 ;Can this driver fit in hi-memory?
	jbe     I_HardC          ;Yes, go check Hardware.
	mov     di,offset HiMMsg ;Display "Not enough high memory" message.
	call    PutCSt
	jmp     I_Fail           ;Go display "Initialization failure" & exit.
I_HardC:
ENDIF
	mov     ax,offset NICContext
	push	ax
	call	_HsmCheckNic
	add     sp,2
	cmp     ax,HsmOK        ;Any errors?
	jz      I_Hardok        ;Error -- treat as initialization failure!
	cmp     ax,HsmNotFound
	jne     I_NoHard
NoAdapter_Fail:
	mov	di,offset Std_EMsg
	call	PutCSt
	mov	di, offset Hsm_Short_Name
;	mov	di,offset NICContext.HsmContext.DriverName
	call    PutCSt
	mov	di,offset Std_EMsg1 + 2
	call    PutCSt
I_NoHard:
	jmp     I_Fail          ;Go display "Initialization failure" & exit.
I_Hardok:
	mov	di,offset CfgMsg1
	call	PutCSt
	mov	di, offset Hsm_Short_Name
;	mov	di,offset NICContext.HsmContext.DriverName
	call    PutCSt
	mov     di,offset CfgMsg2
	call    PutCSt
IFDEF   OS2
;	mov     ax,ds:header.Hdr_PCS    ;OS/2 -- get CS- and DS-selector values.
;	mov     bx,ds:header.Hdr_PDS
	mov	ax,cs			;Prot/Real selectors in header
	mov	bx,ds			; are now not used(reserved).
	and	ax,-4			;ring3 -> ring0 selector
ELSE
	mov     ax,cs           ;DOS -- get CS- and DS-reg. values.
	mov     bx,ds
ENDIF
	mov     cc_tbl.cctable.cc_ds,bx ;Initialize common-mode characteristics table.
	mov     cc_tbl.cctable.cc_spec.farptr.sgm,bx
	mov     cc_tbl.cctable.cc_stat.farptr.sgm,bx
	mov     cc_tbl.cctable.cc_udsp.farptr.sgm,bx
	mov     cc_tbl.cctable.cc_sys.farptr.sgm,ax
					   ;Initialize our "dispatch" table.
	mov     ddsptbl.dd_sptbl.dd_ctbl.farptr.sgm,bx
	mov     ddsptbl.dd_sptbl.dd_req.farptr.sgm,ax
	mov     ddsptbl.dd_sptbl.dd_tx.farptr.sgm,ax
	mov     ddsptbl.dd_sptbl.dd_rx.farptr.sgm,ax
	mov     ddsptbl.dd_sptbl.dd_rrel.farptr.sgm,ax
	mov     ddsptbl.dd_sptbl.dd_ion.farptr.sgm,ax
	mov     ddsptbl.dd_sptbl.dd_ioff.farptr.sgm,ax
					   ;Initialize MAC characteristics table.
	mov     mc_spec.specific.mc_list.farptr.sgm,bx
	mov     mc_spec.specific.mc_idno.farptr.sgm,bx
	or	mc_spec.specific.mc_flg2,mask Receive_type ; RXCHAIN
					   ;Initialize MAC status table.
	mov     mc_stat.status.ms_sptr.farptr.sgm,bx
	mov     pmblock.pm_req_block.pm_op,REGISTER_MODULE ;Set up "register module" parameters.
	mov     pmblock.pm_req_block.pm_ptr2.farptr.ofs,0
	mov     pmblock.pm_req_block.pm_ptr2.farptr.sgm,0
	mov     pmblock.pm_req_block.pm_ptr1.farptr.ofs,offset cc_tbl
	mov     pmblock.pm_req_block.pm_ptr1.farptr.sgm,bx
	mov     NSSNAIStableptr.farptr.sgm,bx

	mov	ax,word ptr NICContext.HsmContext.txQSize
	mov	word ptr mc_spec.specific.mc_txdth,ax
	mov	cx,PKTMAX
	mov	word ptr mc_spec.specific.mc_1buftx,cx
	mul	cx
	mov	word ptr mc_spec.specific.mc_tottx,ax
	mov	ax,word ptr NICContext.HsmContext.rxQSize
	mul	cx
	mov	word ptr mc_spec.specific.mc_rxbf,ax

IFDEF   OS2
	push    0               ;OS/2 -- set up "register module" call.
	push    0
	push    ds
	push    offset pmblock
	push    IONC_PM
	push    IOC_NC
	push    word ptr pmgrhdl
	call    DOSDEVIOCTL
	test    ax,ax           ;Any I/O errors?
	jnz     I_RErr          ;Yes, treat as initialization failure!
	mov     ax,word ptr pmblock.pm_req_block.pm_stat
				;Get protocol-manager status.
	test    ax,ax           ;Any errors?
	jz      I_IRQ0          ;No, close protocol manager.
I_RErr: jmp     I_Err           ;Treat as initialization failure & exit!
I_IRQ0: push    ds              ;Save DS-reg.
	push    word ptr pmgrhdl
				;Close the protocol manager.
	call    DOSCLOSE
	pop     ds              ;Reload DS-reg.
I_temp:
ELSE
I_temp: mov     bx,word ptr pmgrhdl
				;DOS -- set up "register module" call.
	mov     cx,PMSIZE
	mov     dx,offset pmblock
	mov     ah,IOCTL
	mov     al,RXCTRLDATA
	push    cs              ;Save CS-reg.
	int     021h            ;Do desired DOS function.
	pop     ds              ;Reload DS-reg.
	jnc     I_IRQ0          ;Any "register-module" errors?
	jmp     I_Err           ;Yes??  Treat as initialization failure!!
I_IRQ0: mov     bx,word ptr pmgrhdl
				;Close the protocol-manager.
	mov     ah,CLOSE_FILE
	push    cs              ;Save CS-reg.
	int     021h            ;Do desired DOS function.
	pop     ds              ;Reload DS-reg.
ENDIF
	les     bx,[bp]         ;Reload INIT packet ptr.
	mov     ax,offset DRInit
	add	ax,3
        and     ax,not 3
IFNDEF	OS2
	mov     word ptr NSM_Heap,ax
ELSE
	mov     es:[bx].SRqPkt.Code_end_off,ax
ENDIF                                     ;Set code limit.
	push	bx
	mov     ax,offset NICContext
	push    ax
	call    _HsmValidateContext
	add     sp,2
	pop     bx
	cmp     ax,HsmOK         ;Any errors?
	je	I_SetS2 	 ;No, set up segment values.
	jmp	I_Fail
I_SetS2:
;	mov	cx,word ptr NSM_Heap
;	mov     word ptr Tx_Immed_Start,cx
;	mov     word ptr Tx_Immed_Rd,cx
;	mov     word ptr Tx_Immed_Wr,cx
IFNDEF OS2
	mov     ax,MXIMMED
ELSE
	mov     ax,MXIMMED+16+2
ENDIF
	mul	byte ptr NICContext.HsmContext.txQSize
;	add     cx,ax
;	mov     word ptr Tx_Immed_End,cx
;	mov     word ptr NSM_Heap,cx
	push	es
	push	bx
	push	ax

	push	ds
	push	offset Tx_Immed_Start_Phys
	push	ax
	push	offset NICContext
	call	_NsmMallocPhys
	add	sp,4*2
	pop	cx
	pop	bx
	pop	es
	mov     word ptr Tx_Immed_Start,ax
	mov     word ptr Tx_Immed_Wr,ax
	add	cx,ax
	or	ax,dx
	jz	short I_Fail
	mov	ax,ds
	xor	ax,dx
	jnz	short I_Fail
	mov	word ptr Tx_Immed_End,cx
IFNDEF OS2
	mov     es:[bx].SRqPkt.Free_mem_seg,cs
				;Set segment of beginning of free memory.
	mov     cx,word ptr NSM_Heap
	mov     es:[bx].SRqPkt.Free_mem_off,cx
	mov     word ptr NICContext.HsmContext.DriverSize.dblword.lw,cx
	mov     word ptr NICContext.HsmContext.DriverSize.dblword.hw,0
	or      word ptr dvrflags,mask Init_comp
I_Exit: mov     es:[bx].SRqPkt.No_of_units,0
				;Exit -- set character-device values.
	cli                     ;Disable CPU interrupts & post "done" status.
	mov     es:[bx].SRqPkt.SRqStat,mask done
				;(So memory isn't dismissed until exit).
	add     sp,4            ;Unreserve 2 words on stack.
	pop     bp              ;Reload working regs.
	pop     ds
	pop     di
	pop     si
	pop     dx
	pop     cx
	pop     ax
	mov     ss,cs:SaveSS    ;Reset caller's stack.
	mov     sp,cs:SaveSP
	pop     bx              ;Reload remaining regs.
	pop     es
	popf
ELSE
	mov     cx,word ptr NSM_Heap
	mov	es:[bx].SRqPkt.Data_end_off,cx
        push    es                   ; Set data limit.
        mov     ax,ds
	mov	es,ax
IF  0	; Tx_Immed PhysToGDT is not used for this case:DP83815.
IF NOT NSM_DOUBLE_BUFFER EQ 1
	test	word ptr NICContext.HsmContext.hsmOptions,NEED_TX_PHYSADDRS
	jnz	short I_SelSetup1
ENDIF
        mov     cx,0
        mov     cl,byte ptr NICContext.HsmContext.txQSize
        mov     di,word ptr Tx_Immed_Wr
I_SelSetup:
	add     di,MXIMMED+2
	push    cx
	mov     cx,8
	mov     dl,DH_ALLOCGDT
	call    dword ptr DH_Addr
	pop     cx
	add     di,16
	loopw   I_SelSetup
ENDIF
I_SelSetup1:
	mov     di,offset Rx_Phys_Store+2
	mov     cx,8
	mov     dl,DH_ALLOCGDT
	call    dword ptr DH_Addr

	pop     es
I_CtxHook:
IF 0
	push	bx
	mov	eax,offset NsmCtxIndicComp
	mov	ebx,-1
	mov	dl,DH_ALLOCCTXHOOK
	call	dword ptr [DH_Addr]
	pop	bx
	jc	short I_Fail
	mov	dword ptr [CtxHdl_IndicComp],eax
	push	bx
	mov	eax,offset NsmCtxIndicComp2
	mov	ebx,-1
	mov	dl,DH_ALLOCCTXHOOK
	call	dword ptr [DH_Addr]
	pop	bx
	jc	short I_Fail
	mov	dword ptr [CtxHdl_IndicComp2],eax
ENDIF
	or      word ptr dvrflags,mask Init_comp
I_Exit:
        mov     es:[bx].SRqPkt.No_of_units,0
					  ;Exit -- set character-device values.
	mov     es:[bx].SRqPkt.Bpb_array_off,0
	mov     es:[bx].SRqPkt.Bpb_array_seg,0
	add     sp,4            ;Unreserve 2 words on stack.
	pop     bp              ;Reload BP-reg.
	pushf                   ;Save CPU flags & disable CPU interrupts.
	cli                     ;(So memory isn't "dismissed" until we exit).
	mov     es:[bx].SRqPkt.SRqStat,mask done
				;Post "done" packet status.
	popf                    ;Reload CPU flags.
ENDIF
	retf                    ;Exit.
;
; Initialization FAILURE!  Display message & set up "error" return.
;
I_Fail:
	mov     di,offset GFEMsg ;Display "failure to init" msg.
	call    PutCSt
	les     bx,[bp]         ;Get INIT packet pointer.
IFNDEF  OS2
	mov     DOS_Hdr.header.Dev_Att,0        ;DOS -- clear character flag.
	mov     word ptr DOS_Hdr.header.Hdr_Nam,02020h
						;"Blank" device name.
ENDIF
	mov     es:word ptr[bx].SRqPkt.Code_end_off,0
					;OS/2 -- install nothing.
	mov     es:word ptr[bx].SRqPkt.Data_end_off,0

	jmp     short I_Exit    ;Go take error return.
;----------------------------------------------------------------------------

;****************************************************************************
;
;      "NSMInitContext"
;
;   This subroutine looks at the HSMContext and as much as possible sets up
;   the NDIS tables.
;
;****************************************************************************
NSMInitContext:
	      mov  word ptr NICContext.HsmContext.DriverIFType,IF_TYPE_NDIS
	      mov  cx,3
	      mov  si,offset NDIS_VStr
	      mov  di,offset NICContext.HsmContext.DriverIFTypeSpec
	      push es
	      mov  ax,ds
	      mov  es,ax
	      rep  movsw
	      pop  es
	      mov  word ptr NICContext.HsmContext.sendRetries,0
	      test NICContext.HsmContext.hsmOptions,DOES_RX_MULTICOPIES
	      jz   NSMInitContext1
	      or   mc_spec.specific.mc_flg2,mask Multiple_tranferdatas
NSMInitContext1:
	      test NICContext.HsmContext.hsmOptions,NEED_TX_PHYSADDRS
	      jz  NSMInitContext2
	      and mc_spec.specific.mc_flg2,not mask Gdt_virt_addr_supported
NSMInitContext2:
	      mov  si,offset TTLMsg+2
	      mov  di,offset NICContext.HsmContext.DriverName
	      mov  cx,offset TTLMsgEnd
	      sub  cx,offset TTLMsg+1
              rep  movsb
              mov  byte ptr [di],'v'
              inc  di
              mov  si,offset Dvr_Ver
              mov  cx,4
              rep  movsb
	      mov  si,offset NSSNAISIntName
	      mov  di,offset NICContext.HsmContext.Interfacename
	      mov  cx,8
	      rep  movsb          ; Copy SNMP Interface name to NIC
				  ; Context.
	      mov  al,byte ptr NSSNAISIntVerMaj
	      mov  byte ptr NICContext.HsmContext.StatBufMajVer,al
	      mov  al,byte ptr NSSNAISIntVerMin
	      mov  byte ptr NICContext.HsmContext.StatBufMinVer,al
	      mov  ax,word ptr NSSNAISStatBufSz
	      mov  word ptr NICContext.HsmContext.StatBufSize,ax
	      mov  word ptr NICContext.HsmContext.OffsetUpdStats,offset NSSNAIS_UpdatStat
	      mov  word ptr NICContext.HsmContext.SegmentUpdStats,cs
	      mov  ax,word ptr Dvr_Ver.dblword.hw
	      mov  word ptr NICContext.HsmContext.DriverVersion.dblword.hw,ax
	      mov  ax,word ptr Dvr_Ver.dblword.lw
	      mov  word ptr NICContext.HsmContext.DriverVersion.dblword.lw,ax

	      mov  ax,HSM_MEDIA_10BASETfx or HSM_MEDIA_100BASEXfx
	      test ax,word ptr NICContext.HsmContext.mediaTypes
	      jz   NSMInitContext3
	      mov  ax,FDX_CAPABLE_DUPLEX_UNKNOWN
	      mov  word ptr word ptr NICContext.HsmContext.FullDuplexCapable,ax
	      jmp  NSMInitContext4
NSMInitContext3:
	      mov  ax,HALF_DUPLEX
	      mov  word ptr word ptr NICContext.HsmContext.FullDuplexCapable,ax
NSMInitContext4:
	      mov  ax,HSM_MEDIA_100BASEX or HSM_MEDIA_100BASEXfx
	      test ax,word ptr NICContext.HsmContext.mediaTypes
	      jz  NSMInitContext5
	      mov  ax,TOPO_10_100MBPS
	      mov  word ptr word ptr NICContext.HsmContext.Topology,ax
	      jmp  NSMInitContext6
NSMInitContext5:
	      mov  ax,TOPO_10MBPS_ETHERNET
	      mov  word ptr word ptr NICContext.HsmContext.Topology,ax
NSMInitContext6:


	; Initialise transfer data buffers.

	      mov  TranDataBuf.HsmPktDesc.lLink.farptr.sgm,ds
	      mov  TranDataBuf.HsmPktDesc.lLink.farptr.ofs,offset TranDataBuf1
IFDEF OS2
	      push si         ;Get 32-bit fragment physical address.
	      push bx
	      mov  si,offset TranDataBuf1
	      mov  dl,DH_VIRTOPHYS
	      call dword ptr DH_Addr
	      mov  dx,bx
	      xchg ax,dx
	      pop  bx
	      pop  si
ELSE
	      xor  dx,dx     ;Get 32-bit fragment physical address.
	      mov  ax,ds
	      shl  ax,1
	      rcl  dx,1
	      shl  ax,1
	      rcl  dx,1
	      shl  ax,1
	      rcl  dx,1
	      shl  ax,1
	      rcl  dx,1
	      add  ax,offset TranDataBuf1
	      adc  dx,0
ENDIF
	      mov  TranDataBuf.HsmPktDesc.pLink.farptr.sgm,dx
	      mov  TranDataBuf.HsmPktDesc.pLink.farptr.ofs,ax

NSMInitContextX:
	      ret
;-----------------------------------------------------------------------------

;*****************************************************************************
;
;      "NsmFindHardware"
;
;   This subroutine looks at the system and tries to find all the boards
; driver it then records all the instances IOBASE address. for use later.
;
;*****************************************************************************
;
public NsmFindHardware	; << debug >>
NsmFindHardware:
IFNDEF NECWARP
	   push  es
IFDEF OS2
	   mov   ax,0fh                 ; Allow ROM bios Routines to be run.
	   mov   bx,0h
	   mov   cx,0FFFFh
	   mov   dh,0
	   mov   dl,DH_PHYSTOUVIR
	   call  dword ptr DH_Addr
	   mov   ax,es                  ; Store OS/2 ROM Start virtual
	   mov   BIOSStart.farptr.sgm,ax; address
	   mov   BIOSStart.farptr.ofs,bx
	   mov   al,1                   ;Get access to global information
					;selector.
ELSE
	   mov   ax,0                   ; Get the PnP Config Mamager/Bios
	   mov   es,ax                  ; Entry point
	   mov   di,ax
	   mov   bx,VxDID
	   mov   ax,VxDFunc
	   int   02Fh
	   mov   ax,es
	   or    ax,di
	   jz    Find_PCI
	   mov   PnPEntry.farptr.sgm,es ; Store the Entry point address
	   mov   PnPEntry.farptr.ofs,di ; in 'PnPEntry'
	   mov   ax,CM_GetVersion
	   call  PnPEntry
	   test  ax,ax                  ; If No PnP Config Mamager/Bios
	   jz    Find_PCI               ; is present go check for an Other
Find_PnP1:                              ; systems, PCI,EISA,and PCMCIA.
	   push  ds                     ; Else go Find Instances of the cards
	   pop   es                     ;  This driver supports.
	   mov   di,offset Config_Buf
	   mov   bx,0                   ; Start with BX = CSN = 0
Find_PnP2:
	   mov   ax,CM_GetConfig
	   call  PnPEntry
	   test  ax,ax                  ; If there is no more config Info
	   jnz   Find_Exit              ;  then stop trying to find cards.
	   mov   si,offset Board1       ; Else
Find_PnP3:                              ;
	   push  si
	   push  di
	   mov   cx,sizeof BoardType.DeviceID
	   test  [di].PnP_Cfg_Bufs.sDevice_ID.dBusID,ISADEVICE
	   jz    Find_PnPISA
	   mov   eax,mask PnPSerial_Fnd
	   add   si,BoardType.DeviceID
	   add   di,PnP_Cfg_Bufs.sDevice_ID.dDevID
	   repe  cmpsb
	   jmp   Find_PnPBUSX
Find_PnPISA:
	   test  [di].PnP_Cfg_Bufs.sDevice_ID.dBusID,PNPISADEVICE
	   jz    Find_PnPPCI1
	   mov   eax,mask PnPSerial_Fnd
	   add   si,BoardType.DeviceID
	   add   di,PnP_Cfg_Bufs.sDevice_ID.dDevID
	   repe  cmpsb
	   jmp   Find_PnPBUSX
Find_PnPPCI1:
	   test  [di].PnP_Cfg_Bufs.sDevice_ID.dBusID,PCIDEVICE
	   jz    Find_PnPEISA1
	   mov   eax,mask Slot_Found
	   add   si,BoardType.DeviceID
	   add   di,PnP_Cfg_Bufs.sDevice_ID.dDevID
	   repe  cmpsb
	   jmp   Find_PnPBUSX
Find_PnPEISA1:
	   test  [di].PnP_Cfg_Bufs.sDevice_ID.dBusID,EISADEVICE
	   jz    Find_PnPPCMCIA1
	   mov   eax,mask Slot_Found
	   add   si,BoardType.DeviceID
	   add   di,PnP_Cfg_Bufs.sDevice_ID.dDevID
	   repe  cmpsb
	   jmp   Find_PnPBUSX
Find_PnPPCMCIA1:
	   test  [di].PnP_Cfg_Bufs.sDevice_ID.dBusID,PCMCIADEVICE
	   jz    Find_PnPMCA1
	   mov   eax,mask Slot_Found
	   add   si,BoardType.DeviceID
	   add   di,PnP_Cfg_Bufs.sDevice_ID.dDevID
	   repe  cmpsb
	   jmp   Find_PnPBUSX
Find_PnPMCA1:
	   test  [di].PnP_Cfg_Bufs.sDevice_ID.dBusID,MCADEVICE
	   jz    Find_PnPBUSX
	   mov   eax,mask Slot_Found
	   add   si,BoardType.DeviceID
	   add   di,PnP_Cfg_Bufs.sDevice_ID.dDevID
	   repe  cmpsb
Find_PnPBUSX:
	   pop   di
	   pop   si
	   jcxz  Find_PnP4
	   add   si,BR_ELE_SIZE
	   cmp   si,BoardN
	   jne   Find_PnP3
	   jmp   Find_PnP6
Find_PnP4:
	   push  ax
	   push  bx
	   push  si
	   mov   si,offset Board1
	   mov   bh,0
	   mov   bl,[si].BoardType.Cards_Presnt
	   mov   al,[di].PnP_Cfg_Bufs.bIRQRegs
	   mov   [si+bx].BoardType.IntLines,al

	   shl   bx,2
	   mov   ax,[di].PnP_Cfg_Bufs.wIOPort_Base
	   mov   [si+bx].BoardType.Cards_IOAddr.dblword.lw,ax

	   mov   ax,[di].PnP_Cfg_Bufs.dMemBase.dblword.hw
	   mov   [si+bx].BoardType.memaddrs.dblword.hw,ax
	   mov   ax,[di].PnP_Cfg_Bufs.dMemBase.dblword.lw
	   mov   [si+bx].BoardType.memaddrs.dblword.lw,ax

	   mov   ax,[di].PnP_Cfg_Bufs.sDevice_ID.dSerialNo.dblword.hw
	   mov   [si+bx].BoardType.SerialID.dblword.hw,ax
	   mov   ax,[di].PnP_Cfg_Bufs.sDevice_ID.dSerialNo.dblword.lw
	   mov   [si+bx].BoardType.SerialID.dblword.lw,ax

	   pop   si
	   pop   bx
	   pop   ax
	   cmp   byte ptr [si].BoardType.Cards_Presnt,1
	   jb    Find_PnP6
	   not   eax
	   and   [si].BoardType.OptionalKeys,eax
Find_PnP6:
	   inc   [si].BoardType.Cards_Presnt
	   inc   bx
	   jmp   Find_PnP2
Find_PCI:
ENDIF
	   mov	 ah,PCICONFIGINFO
	   mov   al,PCIBIOSPRESENT
IFDEF OS2
	   add   BIOSStart.farptr.ofs,INT_1A
	   pushf
	   call  BIOSStart
	   mov   BIOSStart.farptr.ofs,0h
ELSE
	   int   1Ah
ENDIF
	   jc    Find_EISA
	   test  ah,ah
	   jz    Find_PCI1
	   jmp   Find_EISA
Find_PCI1:
	   cmp   dx,'CP'
	   jne   Find_EISA
	   mov   di,offset PCISys
	   call  PutCSt
	   mov   di,offset Board1
	   mov	 si,0
Find_PCI2:
	   mov	 ah,PCICONFIGINFO
	   mov   al,PCIFINDDEVICE
	   mov   cx,[di].BoardType.DeviceID.dblword.lw
	   mov	 dx,[di].BoardType.DeviceID.dblword.hw
IFDEF OS2
	   add   BIOSStart.farptr.ofs,INT_1A
	   pushf
	   call  BIOSStart
	   mov   BIOSStart.farptr.ofs,0h
ELSE
	   int   1Ah
ENDIF
	   cmp   ah,SUCCESS
	   je    Find_PCI3


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
IF 0
Bus_Master_Chk:
	   push  bx
	   mov   bl,[di].BoardType.Cards_Presnt
	   mov   bh,0
	   shl   bx,2
	   mov	 [di+bx].BoardType.IntLines,cl
	   pop   bx
	   mov   ah,PCICONFIGINFO
	   mov	 al,PCIREADCONFIGWORD
           mov   di, 0004h

           int   1Ah

           test  cx, 0004h
           jz    Force_Bus_Master
           jmp   Find_EISA  ; This was previously here

Force_Bus_Master:
           or   cx, 0004h
	   push  bx
	   mov   bl,[di].BoardType.Cards_Presnt
	   mov   bh,0
	   shl   bx,2
	   mov	 [di+bx].BoardType.IntLines,cl
	   pop   bx

           mov   ah,PCICONFIGINFO
           mov   al, 000Ch
           mov   di, 0004h

           int   1Ah

           jmp   Bus_Master_Chk
ENDIF
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;



Find_PCI3:
	   push  si

	   push  bx
	   push  cx
	   mov	 cx,0
	   mov	 cl,bh
	   mov	 bh,bl
	   and	 bh,07
	   shr	 bl,3
	   mov	 ax,bx
	   mov	 bl,[di].BoardType.Cards_Presnt
	   mov   bh,0
	   shl   bx,2
	   mov	 [di+bx].BoardType.slot,cl
	   pop	 cx
	   pop	 bx
	   push  di
	   mov	 ah,PCICONFIGINFO
	   mov	 al,PCIREADCONFIGBYTE
	   mov	 di,PCICFGINTLINE
IFDEF OS2
	   add   BIOSStart.farptr.ofs,INT_1A
	   pushf
	   call  BIOSStart
	   mov   BIOSStart.farptr.ofs,0h
ELSE
	   int   1Ah
ENDIF
	   pop	 di
	   push  bx
	   mov   bl,[di].BoardType.Cards_Presnt
	   mov   bh,0
	   shl   bx,2
	   mov	 [di+bx].BoardType.IntLines,cl
	   pop   bx

;; causing incorrect PCI I/O addressing; lock-up
;;
;;	   mov   ah,PCICONFIGINFO
;;	   mov	 al,PCIREADCONFIGWORD
;;	   mov	 si,PCICFGBASEADDR0
;;Find_PCI4:

	   mov	 si,PCICFGBASEADDR0
Find_PCI4:
	   mov   ah,PCICONFIGINFO
	   mov	 al,PCIREADCONFIGWORD
	   push  di
	   mov	 di,si
IFDEF OS2
	   add   BIOSStart.farptr.ofs,INT_1A
	   pushf
	   call  BIOSStart
	   mov   BIOSStart.farptr.ofs,0h
ELSE
	   int   1Ah
ENDIF
	   pop	 di
	   test  cx,not 0Fh
	   jnz	 Find_PCI5
	   jmp	 Find_PCI8
Find_PCI5:
	   test  cx,PCI_BASE_ADDR_IO
	   jz	 Find_PCI6
	   and	 cx,not 03
	   push  bx
	   mov   bl,[di].BoardType.Cards_Presnt
	   mov   bh,0
	   shl   bx,2
	   mov	 [di+bx].BoardType.Cards_IOAddr.dblword.lw,cx
	   pop   bx
	   mov   ah,PCICONFIGINFO
	   mov	 al,PCIREADCONFIGWORD
	   push  di
	   add	 si,2
	   mov	 di,si
IFDEF OS2
	   add   BIOSStart.farptr.ofs,INT_1A
	   pushf
	   call  BIOSStart
	   mov   BIOSStart.farptr.ofs,0h
ELSE
	   int   1Ah
ENDIF
	   pop   di
	   push  bx
	   mov   bl,[di].BoardType.Cards_Presnt
	   mov   bh,0
	   shl   bx,2
	   mov	 [di+bx].BoardType.Cards_IOAddr.dblword.hw,cx
	   pop	 bx
	   jmp	 Find_PCI7
Find_PCI6:
	   and   cx,not 0fh
	   push  bx
	   mov   bl,[di].BoardType.Cards_Presnt
	   mov   bh,0
	   shl   bx,2
	   mov	 [di+bx].BoardType.memaddrs.dblword.lw,cx
	   pop   bx
	   mov   ah,PCICONFIGINFO
	   mov	 al,PCIREADCONFIGWORD
	   push  di
	   add	 si,2
	   mov	 di,si
IFDEF OS2
	   add   BIOSStart.farptr.ofs,INT_1A
	   pushf
	   call  BIOSStart
	   mov   BIOSStart.farptr.ofs,0h
ELSE
	   int   1Ah
ENDIF
	   pop   di
	   push  bx
	   mov   bl,[di].BoardType.Cards_Presnt
	   mov   bh,0
	   shl   bx,2
	   mov	 [di+bx].BoardType.memaddrs.dblword.hw,cx
	   pop	 bx
Find_PCI7:
	   add	 si,2
	   cmp	 si,PCICFGBASEADDR1
	   ja	 Find_PCI8
	   jmp	 Find_PCI4
Find_PCI8:
	   inc	 [di].BoardType.Cards_Presnt
	   cmp   [di].BoardType.Cards_Presnt,1
	   jbe	 Find_PCI9
	   and   dword ptr Board1.OptionalKeys,not mask Slot_Found
	   or	 dword ptr Board1.OptionalKeys,mask PnPSerial_Fnd
Find_PCI9:
	   pop	 si
	   inc   si
	   jmp	 Find_PCI2
Find_EISA:
	   mov   ah,EISACONFIGINFO
	   mov   al,BRIEF_CONFIG
	   mov   cl,0
IFDEF OS2
	   add   BIOSStart.farptr.ofs,INT_15
	   pushf
	   call  BIOSStart
	   mov   BIOSStart.farptr.ofs,0h
ELSE
	   int   15h
ENDIF
	   test  ah,ah
	   jz    Find_EISA1
	   cmp   ah,87h
	   jne   Find_PCMCIA
	   mov   di,offset EISASys
	   call  PutCSt
	   mov   di,offset SysBad
	   call  PutCSt
	   jmp   Find_PCMCIA
Find_EISA1:
	   mov   di,offset EISASys
	   call  PutCSt
	   mov   cx,0

Find_EISA2:
	   push  cx
	   mov   ah,EISACONFIGINFO
	   mov   al,BRIEF_CONFIG
IFDEF OS2
	   add   BIOSStart.farptr.ofs,INT_15
	   pushf
	   call  BIOSStart
	   mov   BIOSStart.farptr.ofs,0h
ELSE
	   int   15h
ENDIF
	   pop   cx
	   cmp   ah,80h
	   je	 Find_PCMCIA
	   cmp	 di,word ptr Board1.DeviceID.dblword.lw
	   je    Find_EISA3
	   jmp   Find_EISA6
Find_EISA3:
	   cmp   si,word ptr Board1.DeviceID.dblword.hw
	   je    Find_EISA4
	   jmp   Find_EISA6
Find_EISA4:
	   cmp   byte ptr Board1.Cards_Presnt,1
	   jb    Find_EISA5
	   and   dword ptr Board1.OptionalKeys,not mask Slot_Found
	   or    dword ptr Board1.OptionalKeys,mask PnPSerial_Fnd
Find_EISA5:
	   push  cx
	   inc   byte ptr Board1.Cards_Presnt
	   mov   ah,EISACONFIGINFO
	   mov   al,FULL_CONFIG
	   mov   ch,0
	   mov   si,offset Config_Buf
IFDEF OS2
	   add   BIOSStart.farptr.ofs,INT_15
	   pushf
	   call  BIOSStart
	   mov   BIOSStart.farptr.ofs,0h
ELSE
	   int   15h
ENDIF
	   mov   si,offset Board1
	   mov   di,offset Config_Buf
	   mov   bh,0
	   mov   bl,[si].BoardType.Cards_Presnt
	   dec   bl
	   pop   cx
	   mov   [si+bx].BoardType.slot,cl

	   mov   al,[di].EISA_Cfg_Bufs.Int_Config
	   mov   [si+bx].BoardType.IntLines,al

	   shl   bx,2
	   mov   ax,[di].EISA_Cfg_Bufs.IO_Base
	   mov   [si+bx].BoardType.Cards_IOAddr.dblword.lw,ax

	   mov   ax,word ptr [di].EISA_Cfg_Bufs.Mem_Addr_Mid
	   mov   [si+bx].BoardType.memaddrs.dblword.hw,ax
	   mov   ax,word ptr [di].EISA_Cfg_Bufs.Mem_Addr_Msb
	   mov   [si+bx].BoardType.memaddrs.dblword.lw,ax

Find_EISA6:
	   inc   cx
	   jmp   Find_EISA2

Find_PCMCIA:
	   mov   ah, PCMCIA_CS          ; Setup to call Card and socket
	   mov   al, GET_CARD_SERVICES_INFO
	   mov   bx, offset Config_Buf  ; services
	   push  ds
	   pop   es
	   mov   cx, sizeof GetCardServicesInfoArg
IFDEF OS2
	   push  ds
;          mov   ds, card_serv_ds       ;Set up card service's DS reg
;          call  es:card_serv_addr      ;Call device driver entry point
	   pop   ds
ELSE
	   int   1Ah                    ;Generate interrupt to driver
ENDIF

Find_MCA:
;          mov   ah,MCACONFIGINFO
;          mov   al,BRIEF_CONFIG
;          mov   cl,0
;IFDEF OS2
;          add   BIOSStart.farptr.ofs,INT_15
;          pushf
;          call  BIOSStart
;          mov   BIOSStart.farptr.ofs,0h
;ELSE
;          int   15h
;ENDIF
;          test  ah,ah
;          jz    Find_EISA1
;          cmp   ah,87h
;          je    Find_MCA1
;          mov   di,offset MCASys
;          call  PutCSt
;          jmp   Find_MCA
	   cli
	   mov   si,offset Board1
	   mov   cx,8h
	   mov   dx,MC_POSENL
	   mov   al,0
Find_MCA1:
	   or    al,MC_POSENB
	   out   dx,al
	   mov   dx,MC_IDHOFS    ;Point to Microchannel board I.D. bytes.
	   in    al,dx
	   mov   ah,al
	   mov   dx,MC_IDLOFS    ;Point to Microchannel board I.D. bytes.
	   in    al,dx
	   mov   si,offset Board1
Find_MCA2:
	   cmp   ax,[si].BoardType.DeviceID.dblword.lw
	   jne   Find_MCA3

	   in    al,dx
	   mov   bl,al
	   and   bx,MC_ADDRMASK
	   shr   bx,3
	   mov   ax,[si+bx].BoardType.IOBases   ;Get desired IO address.
	   mov   bh,0
	   mov   bl,[si].BoardType.Cards_Presnt
	   shl   bx,1
	   mov   [si+bx].BoardType.Cards_IOAddr.dblword.lw,ax
	   pop   bx
	   cmp   [si].BoardType.Cards_Presnt,1
	   jb    Find_MCA4
	   not   eax
	   and  [si].BoardType.OptionalKeys,not mask Slot_Found
Find_MCA4:
	   inc   [si].BoardType.Cards_Presnt
	   jmp   Find_MCA5
Find_MCA3:
	   add   si,BR_ELE_SIZE
	   cmp   si,BoardN
	   jne   Find_MCA2
Find_MCA5:
	   mov   dx,MC_POSENL
	   in    al,dx
	   and   al,07h
	   inc   al
	   dec   cx
	   jcxz  Find_ISA
	   jmp   Find_MCA1
Find_ISA:
	   sti
Find_Exit:
	   pop    es
	   ret

ELSE
; OEMHLP service. PCI Devices only.
	enter	8,0	; -2:BoardType pointer
			; -4:PCI SLOT index
			; -6:BusDevFunc
			; -8:register number
			; Open OEMHLP$ for PCI access
	push	es
	mov	ax,ds
	mov	dx,ss
	lea	bx,[bp-2]
	push	ax
	push	offset NameOEMHLP
	push	ax
	push	offset hdlOEMHLP
	push	dx
	push	bx			; action
	push	0
	push	0			; size - long
	push	0			; attribute
	push	1			; flag
	push	SHARED_RW		; mode
	push	0
	push	0			; reserved - long
	call	DOSOPEN
	test	ax,ax
	jnz	near ptr OEMPCI_Exit
	call	ioc_OEMPCI_Present
	jnz	near ptr OEMPCI_Close
	mov	di,offset PCISys
	call	PutCSt
	mov	[bp-2],offset Board1	; search device type
	mov	word ptr [bp-4],0	; PCI SLOT index
OEMPCI_Loop:
	mov	si,[bp-4]
	mov	bx,[bp-2]
	mov	cx,[bx].BoardType.DeviceID.dblword.lw
	mov	dx,[bx].BoardType.DeviceID.dblword.hw
	call	ioc_OEMPCI_Device
	jc	short OEMPCI_Close
	mov	[bp-6],bx		; BusDevFunc

;	mov	al,bh			; BusNumber
;	mov	al,[bp-4]		; SLOT= PCI SLOT index
;					; CANNOT USE "BUSNO"

	mov	si,[bp-2]
	mov	bl,[si].BoardType.Cards_Presnt
	mov	bh,0
	mov	al,bl			; PCI SLOT = Cards_Presnt
	shl	bx,2
	mov	[bx+si].BoardType.slot,al ; save PCI SLOT index.
				; Original code saves BUS Number.
				; DevFunc missing:-)
				; SLOT Key is not Device Number!

	mov	di,PCICFGINTLINE	; irq - 3ch
	mov	bx,[bp-6]
	call	ioc_OEMPCI_ReadWord
	jc	short OEMPCI_Close
	mov	si,[bp-2]
	mov	bl,[si].BoardType.Cards_Presnt
	mov	bh,0
	shl	bx,2
	mov	[bx+si].BoardType.IntLines,cl

	mov	di,PCICFGBASEADDR0	; 10h
OEMPCI_1:
	mov	bx,[bp-6]
	mov	[bp-8],di
	call	ioc_OEMPCI_ReadWord
	jc	short OEMPCI_Close
	test	cx,not 0fh		; FFF0h - unused register.
	jz	short OEMPCI_3
	test	cl,PCI_BASE_ADDR_IO	; 1h - I/O indicator
	jz	short OEMPCI_Mem
	and	cx,-4			; get I/O address
	mov	si,[bp-2]
	mov	bl,[si].BoardType.Cards_Presnt
	mov	bh,0
	shl	bx,2
	mov	[bx+si].BoardType.Cards_IOAddr,cx
	jmp	short OEMPCI_2
OEMPCI_Mem:
; BoardType.memaddrs cannot be required. Skip.

OEMPCI_2:
	mov	di,[bp-8]
	add	di,+4
	cmp	di,PCICFGBASEADDR1
	jna	short OEMPCI_1
OEMPCI_3:
	mov	bx,[bp-2]
	inc	[bx].BoardType.Cards_Presnt
	cmp	[bx].BoardType.Cards_Presnt,1
	jbe	short OEMPCI_4
	and	dword ptr [bx].BoardType.OptionalKeys,not mask Slot_Found
	or	dword ptr [bx].BoardType.OptionalKeys,mask PnPSerial_Fnd
OEMPCI_4:
	inc	word ptr [bp-4]
	jmp	near ptr OEMPCI_Loop		; near ptr 
OEMPCI_Close:
	push	word ptr [hdlOEMHLP]
	call	DOSCLOSE
OEMPCI_Exit:
	pop	es
	leave
	ret

ioc_OEMPCI_ReadWord:
	enter	10,0	; -10:parm  -5:data
	ror	bx,8
	mov	byte ptr [bp-10],3	; Read PCI Conf. Reg.
	mov	[bp-9],bx		; DevFuncBus
	mov	[bp-7],di		; register
	mov	byte ptr [bp-6],2	; size word - 2
	mov	bx,sp
	lea	di,[bp-5]
	call	ioc_OEMPCI_Entry
	or	al,ah
	mov	bl,[bp-5]
	mov	cx,[bp-4]		; register data
	or	al,bl
	neg	al
	leave
	ret

ioc_OEMPCI_Device:
	enter	10,0	; -9:parm   -3:data
	mov	byte ptr [bp-9],1	; Find PCI Device
	mov	[bp-8],cx		; DeviceID
	mov	[bp-6],dx		; VendorID
	mov	[bp-4],si		; Index
	lea	bx,[bp-9]
	lea	di,[bp-3]
	call	ioc_OEMPCI_Entry
	or	al,ah
	mov	cl,[bp-3]
	mov	bx,[bp-2]
	or	al,cl
	ror	bx,8
	neg	al
	leave
	ret

ioc_OEMPCI_Present:
	enter	6,0	; -1:parm    -6:data
	mov	bx,bp
	mov	di,sp
	mov	byte ptr [bp-1],0 	; query PCI BIOS Info.
	dec	bx
	call	ioc_OEMPCI_Entry
	or	al,ah
	or	al,[bp-6]		; ZF - PCI Present
	leave
	ret

ioc_OEMPCI_Entry:
	push	ss
	push	di		; data
	push	ss
	push	bx		; parm
	push	0bh		; func OEMHLP_PCI
	push	80h		; cat  IOCTL_OEMHLP
	push	word ptr [hdlOEMHLP]
	call	DOSDEVIOCTL
	ret
ENDIF

;-----------------------------------------------------------------------------

;*****************************************************************************
;
;      "PARSE Routine"
;
;   Subroutine to "parse" the configuration data from the PROTOCOL.INI file.
;   At entry, ES:BX points to the data to be "parsed".  At exit, the AX-reg.
;   contains the return code (0 = success).  All registers are saved.
;
;*****************************************************************************
;
Parse:  push    bp              ;Save BP-reg.
	sub     sp,4            ;Reserve 2 words on stack.
	mov     bp,sp           ;Point to reserved words.
	push    bx              ;Save remaining registers.
	push    cx
	push    dx
	push    si
	push    di
	push    es
P_NextM:
	push    es
	mov     ax,es            ;"Merge" both halves of "next-config" ptr.
	or      ax,bx            ;Check If result in AX is zero
	jnz     P_FirstK         ; Yes:- There is no more Protocol.ini modules
	pop     es               ;        to check.
	jmp     P_InvD           ; No:- Get Module keywords.
P_FirstK:
	lea     di,es:[bx].cf_tbl.cf_k1
				 ;Point to 1st configuration keyword.
P_NextK:
	mov     ax,es            ;"Merge" both halves of "next-keyword" ptr.
	or      ax,di            ;Check If result in AX is zero
	jnz     P_ValK           ; No:- Go check if keyword is for this
	pop	es		 ;	driver.
        test    dword ptr Board.FoundKeys,mask Dvrname_Found
                                 ; Yes:- Check if the module just read had
				 ;	 our drivername.
	jnz     P_MFound         ;      No:- The module is not for this
	mov	No_Of_BadKeys,0  ;	     Therefore there can't be any bad
        mov     dword ptr Board.FoundKeys,0
        les     bx,es:[bx]
	jmp	P_NextM 	 ;	     keywords. Go check more modules.
P_MFound:                        ;
	cmp     No_Of_BadKeys,0
	jz      P_BrdcfgOk
	jmp     P_InvK
P_BrdcfgOk:
	mov     ax,ds
	mov     si,bx            ;
	add     si,(cf_tbl.cf_nam - cf_tbl.cf_nxt)
				 ;      Yes:-The module for this driver has
	mov     di,offset cc_tbl.cctable.cc_name
				 ;             been found and there is no
	mov     bx,es            ;            keyword or parameter in error.
	mov     ds,bx            ; Since there is no error copy module name
	mov     es,ax            ; into Common Characteristics Table.
	mov     cx,16            ; Leave parse routine having successfully
	rep     movsb            ; found the driver protocol.ini module.
	mov     ds,ax
	mov     di,offset Board
	mov     si,[di].BrdStore.BrdCfgPtr
	push    di
	mov	di,offset Key_Wrd2
        shr     dword ptr Board.FoundKeys,1
        shr     [si].BoardType.ExpectedKeys,1
	shr	[si].BoardType.OptionalKeys,1

	mov     cl,0
P_BrdOk2:
	mov	ch,0
        shr     dword ptr Board.FoundKeys,1
        jnc     P_BrdOk3
	shr	[si].BoardType.OptionalKeys,1
	shr     [si].BoardType.ExpectedKeys,1
	jc      P_BrdOk4
	mov     ch,1
	jmp     P_BrdErr
P_BrdOk3:
	shr     [si].BoardType.ExpectedKeys,1
	jnc     P_BrdOk7
	shr     [si].BoardType.OptionalKeys,1
	jc      P_BrdOk6
	jmp     P_BrdErr
P_BrdOk7:
	shr     [si].BoardType.OptionalKeys,1
	jmp     P_BrdOk6
P_BrdErr:
	mov     al,UNEXPECTED_KEY
	cmp     ch,1
	je      P_BrdErr2
	mov     al,KEY_NOT_FOUND
P_BrdErr2:
	call    Parms_Err
	inc     cl
	jmp     P_BrdOk6
P_BrdOk4:
	push    ax
	call    [di].Kw_tbl.Kw_V_ser
	cmp     al,0
	je      P_BrdOk5
	call    Parms_Err
	inc     cl
P_BrdOk5:
	pop     ax
P_BrdOk6:
	add     di,KW_ELE_SIZE
	cmp     di,Key_WrdN
	jne     P_BrdOk2
	pop     di               ; Restore Board Type pointer.
	cmp     cl,0
	je      P_BrdOkN
	jmp     P_Err
P_BrdOkN:
	mov     ax,SUCCESS
	jmp     P_Done
P_ValK:                          ; Else Current module is not for this driver
	mov     si,offset Key_Wrd1 ;so mask off Keywords status byte bits for
P_OurNK:                         ; next module.
	push    di               ; Setup SI to point to keyword table.
	lea     di,es:[di].keyword.kw_nam
				 ; DI points to current module keyword.
	call    StrCmp           ; Check if module keyword compares success-
	pop     di               ; -fully with current keyword in table.
	je      P_SerK           ;  Yes:- Check If its parameter is valid.
	jmp     P_Nkey           ;  No:- Set bit 1 of Keywords status byte.
P_SerK:                          ;      (i.e. Potential Bad Parameter. )
	lea     dx,[di]
				 ; Load DX with pointer to the Parameter.
	call    word ptr [si].Kw_tbl.Kw_S_ser
	jmp     P_ValKey
P_Nkey:                          ;  Yes:- Go Pick up and possible Next keyword.
	add     si,KW_ELE_SIZE   ;  No:- Set bit 1 of Keywords status byte.
	cmp     si,offset Key_WrdN
				 ;        (i.e. Potential Bad Parameter. )
	jnz     P_OurNK          ; Point to Next keyword in table.
	mov     si,offset BadKeys
	mov     ax,No_Of_BadKeys
	shl     ax,2             ;
	add     si,ax            ;
	inc     No_Of_BadKeys    ;
	mov     [si].farptr.ofs,di
	mov     [si].farptr.sgm,es
				 ; Check If end of Keyword table has been
P_ValKey:                        ; reached.
	les     di,es:[di]       ;  Yes:- Set bit 0 of Keywords status byte.
	jmp     P_NextK          ;        (i.e. Potential Bad Keyword. )
				 ;        Pick up next Module keyword.
				 ;      No:- Setup for comparison with next
				 ;         keyword in table.
P_InvK:                          ; Display an Invalid keyword Error Message.
	mov     di,offset PEMsg1 ;
	call    PutCSt
	mov     bx,No_Of_BadKeys
	shl     bx,2
	mov     si,offset BadKeys
P_InvK1:
	mov     di,offset PEMsgK1
	call    PutCSt
	push    ds
	lds     di,[si+bx-4]
	add     di,keyword.kw_nam
	call    PutCSt
	pop     ds
	mov     di,offset PEMsgK2
	call    PutCSt
	sub     bx,4
	jnz     P_InvK1
	jmp     P_Err
P_InvD:
	mov     di,offset PEMsg1
	call    PutCSt
	mov     di,offset PEMsgD1
	call    PutCSt           ; Display an Drivername ???????? Not found
	mov     di,offset cc_tbl.cctable.cc_name
				 ; Error message.
	call    PutCSt
	mov     di,offset PEMsgD2
	call    PutCSt
P_Err:
	mov     ax,BAD_PARAMETER  ;Return "bad parameter".
P_Done: pop     es                ;Done -- reload registers.
	pop     di
	pop     si
	pop     dx
	pop     cx
	pop     bx
	add     sp,4            ;Unreserve 2 words on stack.
	pop     bp              ;Reload BP-reg.
	ret                     ;Exit.
;----------------------------------------------------------------------------

;****************************************************************************
;
;      "S_Name Routine"
;
;
;****************************************************************************
S_Name:
	push    di
	push    si
	mov     si,offset cc_tbl.cctable.cc_name
				;Compare our name against PROTOCOL.INI name.
	mov     di,dx
	lea     di,[di].keyword.kw_p1.Parameter
	call    StrCmp          ;Does PROTOCOL.INI name match our name?
	jne	S_NameX
        or      dword ptr Board.FoundKeys,mask Dvrname_Found
S_NameX:
	pop	si
S_Exit:
	pop    di
	clc                     ;Clear carry flag.
	ret                     ;Exit
;----------------------------------------------------------------------------

;****************************************************************************
;
;      "S_Base Routine"
;
;
;****************************************************************************
S_Base:
	 push   di
	 push	si
         or     dword ptr Board.FoundKeys,mask Io_Found
         call   Parms_check
	 jne	S_Base1
	 lea    di,[di].keyword.kw_p1.Parameter
	 mov    ax,es:[di].dblword.lw
	 mov    Board.Io_Used,ax
	 mov    al,SUCCESS
S_Base1:
	 mov    Board.Io_Valid,al
S_BaseX:
	 pop    si
	 pop    di
	 ret
;----------------------------------------------------------------------------

;****************************************************************************
;
;      "V_Base Routine"
;
;
;****************************************************************************
V_Base:
       push     si
       push     di
       mov      al,Board.Io_Valid
       cmp      al,0
       jne      V_BaseX
       mov      di,offset Board
       mov      ax,[di].BrdStore.Io_Used
       mov      NICContext.HsmContext.IOaddr,ax
       mov      al,SUCCESS
V_BaseX:
       pop     di
       pop     si
       ret
;
;  The following unused code checks the IOBase parameter to verify that
;  it is truely valid. If used then the HSM would have to specify an
;  ennumerated type for the valid IOBase addresses, then the enumerated
;  type would be instanceiated within an array held in the NSM. Therefore
;  the IObase parameter can be checked against this array in the NSM
;  to verify its validity.
;
;       push    si
;       push    di
;       mov     al,Board.Io_Valid
;       cmp     al,0
;       jne     V_BaseX
;       lea     si,[si].BoardType.Iobases
;       mov     di,offset Board.Io_Used
;V_Base1:
;       mov     ax,[si]
;       cmp     ax,0
;       je      V_BaseEX
;       cmp     ax,[di]
;       je      V_Base2
;       add     si,2
;       jmp     V_Base1
;V_Base2:
;       mov     al,SUCCESS
;       jmp     V_BaseX
;V_BaseEX:
;       mov     al,BAD_IOBASE
;V_BaseX:
;       pop     di
;       pop     si
;       ret
;-----------------------------------------------------------------------------

;****************************************************************************
;
;      "S_Ram Routine"
;
;
;****************************************************************************
S_Ram:
	 push   di
	 push	si
         or     dword ptr Board.FoundKeys,mask Ram_Found
         call   Parms_check
	 jne	S_Ram1
	 lea    di,[di].keyword.kw_p1.Parameter
	 mov    ax,es:[di].dblword.hw
	 mov    Board.Ram_Used.dblword.hw,ax
	 mov    ax,es:[di].dblword.lw
	 mov    Board.Ram_Used.dblword.lw,ax
	 mov    al,SUCCESS
S_Ram1:
	 mov    Board.Ram_Valid,al
S_RamX:
	 pop    si
	 pop    di
	 ret
;----------------------------------------------------------------------------

;****************************************************************************
;
;      "V_Ram Routine"
;
;
;****************************************************************************
V_Ram:
       push     si
       push     di
       mov      al,Board.Int_Valid
       cmp      al,0
       jne      V_RamX
       mov      di,offset Board
       mov      ax,[di].BrdStore.Ram_Used.dblword.lw
       mov      NICContext.HsmContext.MEMaddr.dblword.lw,ax
       mov      ax,[di].BrdStore.Ram_Used.dblword.hw
       mov      NICContext.HsmContext.MEMaddr.dblword.hw,ax
       mov      al,SUCCESS
V_RamX:
       pop      di
       pop      si
       ret
;----------------------------------------------------------------------------

;*****************************************************************************
;
;      "S_Irq Routine"
;
;
;*****************************************************************************
S_Irq:
	 push   di
	 push	si
         or     dword ptr Board.FoundKeys,mask Int_Found
         call   Parms_check
	 jne	S_Irq1
	 lea    di,[di].keyword.kw_p1.Parameter
	 mov    al,es:[di].dblbyte.lb
	 mov    Board.Int_Used,al
	 mov    al,SUCCESS
S_Irq1:
	 mov    Board.Int_Valid,al
S_IrqX:
	 pop    si
	 pop    di
	 ret
;----------------------------------------------------------------------------

;****************************************************************************
;
;      "V_Irq Routine"
;
;
;****************************************************************************
V_Irq:
       push     si
       push     di
       mov      al,Board.Int_Valid
       cmp      al,0
       jne      V_IrqX
       mov      di,offset Board
       mov      ah,0
       mov      al,[di].BrdStore.Int_Used
       mov      NICContext.HsmContext.Irq.dblword.lw,ax
       mov      al,SUCCESS
V_IrqX:
       pop      di
       pop      si
       ret
;
;  The following unused code checks the Interrupt parameter to verify that
;  it is truely valid. If used then the HSM would have to specify an
;  ennumerated type for the valid INTERRUPT line values, then the enumerated
;  type would be instanceiated within an array held in the NSM. Therefore
;  the INTERRUPT parameter can be checked against this array in the NSM
;  to verify its validity.
;
;       push    si
;       push    di
;       mov     al,Board.Int_Valid
;       cmp     al,0
;       jne     V_IrqX
;       lea     si,[si].BoardType.IntLines
;       mov     di,offset Board.Int_Used
;V_Irq1:
;       mov     al,[si]
;       cmp     al,0
;       je      V_IrqEX
;       cmp     al,[di]
;       je      V_Irq2
;       inc     si
;       jmp     V_Irq1
;V_Irq2:
;       mov     al,SUCCESS
;       jmp     V_IrqX
;V_IrqEX:
;       mov     al,BAD_INTLINE
;V_IrqX:
;       pop     di
;       pop     si
;       ret
;----------------------------------------------------------------------------

;****************************************************************************
;
;      "S_Slot Routine"
;
;
;****************************************************************************
S_Slot:
	 push   di
	 push	si
         or     dword ptr Board.FoundKeys,mask Slot_Found
         call   Parms_check
	 jne	S_Slot1
	 lea    di,[di].keyword.kw_p1.Parameter
	 mov    al,es:[di]
	 mov    Board.Slot_Used,al
	 mov    al,SUCCESS
S_Slot1:
	 mov    Board.Slot_Valid,al
S_SlotX:
	 pop    si
	 pop    di
	 ret
;----------------------------------------------------------------------------

;****************************************************************************
;
;      "V_Slot Routine"
;
;
;****************************************************************************
V_Slot:
       push     si
       push     di
       mov      al,Board.Slot_Valid
       cmp      al,0
       jne      V_SlotX
       mov      di,offset Board
       mov      al,[di].BrdStore.Slot_Used
       mov      NICContext.HsmContext.Slot,al
       mov      al,SUCCESS
V_SlotX:
       pop      di
       pop      si
       ret
;----------------------------------------------------------------------------

;****************************************************************************
;
;      "S_Net Routine"
;
;****************************************************************************
S_Net:
	 push   di
	 push	si
         or     dword ptr Board.FoundKeys,mask Netaddr_Found
         call   Parms_check
	 jne	S_Net1x
	 mov    cx,(2*ADDRLEN)+1
	 cmp    cx,es:[di].keyword.kw_p1.Param_len
	 mov    al,BAD_ENET_ADDR
	 jne    S_Net1x
	 dec    cx
	 lea    di,[di].keyword.kw_p1.Parameter
	 mov    si,offset Board.Netaddr_Used
S_Net1:  mov    al,es:[di]
	 cmp    al, '0'
	 jae    S_Net2
	 mov    al,BAD_ENET_ADDR
	 jne    S_Net1x
S_Net2:
	cmp      al, '9'            ; Else Check Character <= '9', go convert.
	ja       S_Net3             ; Else Check Character < 'A', invalid
	jmp      S_Net7             ; Else Check Character <= 'F', go convert.
S_Net3:
	cmp      al,'F'             ; Else Check Character < 'a', invalid
	jbe      S_Net8
	cmp      al,'a'             ; Else Check Character <= 'f', go convert.
	jae      S_Net5             ; Else invalid.
	mov      al,BAD_ENET_ADDR
	jne      S_Net1x
S_Net5:
	cmp      al,'f'
	jbe      S_Net6
	mov      al,BAD_ENET_ADDR
	jne      S_Net1x
S_Net6:
	sub      al,57h             ; If chacacter in range 'a' and 'f'
	jmp      S_Net9             ; subtract 57 to get hex value.
S_Net7:
	sub      al,30h             ; If chacacter in range '0' and '9'
	jmp      S_Net9             ; subtract 30 to get hex value.
S_Net8:
	sub      al,37h             ; If chacacter in range 'A' and 'F'
S_Net9:
	test     cl,1               ; subtract 37 to get hex value.
	jnz      S_NetA             ; If CL is on an even byte then store in
	mov      ah,al              ; AH the current numeric nibble then go
	jmp      S_NetC             ; get next char else Shift the last
S_NetA:                             ; numeric Nibble to the upper half
	shl     ah,4                ; of AH and add the current numeric
	add     ah,al               ; nibble.
	cmp     si,offset Board.Netaddr_Used
				    ; Check first byte of address to see
	jne     S_NetB              ; If address is a group address then
	test    ah,01               ; first bit of first byte will be set
	jz      S_NetB              ; therefore netaddress in protocol.ini
	mov     al,MULTCAST_ADDR    ; is not valid.
	jne     S_Net1x

S_NetB:
	mov      [si],ah            ; Store in Permanent node address.
	inc      si                 ; Move to next location of address.
S_NetC:
	inc      di                 ; Move to next character of address
	dec      cl                 ; string and decrement remaining
	jnz      S_Net1             ; character, if some left convert else
	mov      al,SUCCESS         ; Exit with success.
S_Net1x:
	mov     Board.Netaddr_Valid,al
S_NetX:
	pop     si
	pop     di
	ret
;----------------------------------------------------------------------------

;****************************************************************************
;
;      "V_Net Routine"
;
;
;****************************************************************************
V_Net:
       push     bx
       push     cx
       push     si
       push     di
       push     es
       mov      al,Board.Netaddr_Valid
       cmp      al,0
       jne      V_NetX
       mov      ax, ds
       mov      es, ax
       lea      bx, offset Board
       lea      si, [bx].BrdStore.Netaddr_Used
       lea      di, NICContext.HsmContext.CurrMacAddr
       mov      cx, 6
  rep  movsb
       mov      al,SUCCESS
V_NetX:
       pop      es
       pop      di
       pop      si
       pop      cx
       pop      bx
       ret
	 ret
;----------------------------------------------------------------------------

;****************************************************************************
;
;      "S_Serial Routine"
;
;****************************************************************************
S_Serial:
	push    di
	push	si
        or      dword ptr Board.FoundKeys,mask PnPSerial_Fnd
        call    Parms_check
	jne	S_Serial1x

	lea     di,[di].keyword.kw_p1.Parameter
	mov     ax,es:[di].dblword.hw
	mov     Board.Serial_Used.dblword.hw,ax
	mov     ax,es:[di].dblword.lw
	mov     Board.Serial_Used.dblword.lw,ax

;       mov     cx,9       ; PnP SERIAL number of nibbles + 1
;       cmp     cx,es:[di].keyword.kw_p1.Param_len
;       mov     al,BAD_PNP_SERIAL
;       jne     S_Serial1x
;       dec     cx
;       lea     di,[di].keyword.kw_p1.Parameter
;       mov     si,offset Board.Serial_Used
;S_Serial1:
;       mov      al,es:[di]
;       cmp      al, '0'
;       jae      S_Serial2
;       mov      al,BAD_PNP_SERIAL
;       jne      S_Serial1x
;S_Serial2:
;       cmp      al, '9'            ; Else Check Character <= '9', go convert.
;       ja       S_Serial3          ; Else Check Character < 'A', invalid
;       jmp      S_Serial7          ; Else Check Character <= 'F', go convert.
;S_Serial3:
;       cmp      al,'F'             ; Else Check Character < 'a', invalid
;       jbe      S_Serial8
;       cmp      al,'a'             ; Else Check Character <= 'f', go convert.
;       jae      S_Serial5          ; Else invalid.
;       mov      al,BAD_PNP_SERIAL
;       jne      S_Serial1x
;S_Serial5:
;       cmp      al,'f'
;       jbe      S_Serial6
;       mov      al,BAD_PNP_SERIAL
;       jne      S_Serial1x
;S_Serial6:
;       sub      al,57h             ; If chacacter in range 'a' and 'f'
;       jmp      S_Serial9          ; subtract 57 to get hex value.
;S_Serial7:
;       sub      al,30h             ; If chacacter in range '0' and '9'
;       jmp      S_Serial9             ; subtract 30 to get hex value.
;S_Serial8:
;       sub      al,37h             ; If chacacter in range 'A' and 'F'
;S_Serial9:
;       shl      dword ptr [si],4
;       add      byte ptr [si],al
;S_SerialB:                         ; Move to next character of address
;       inc     di                  ; string and decrement remaining
;       loopw   S_Serial1           ; character, if some left convert else
	mov     al,SUCCESS          ; Exit with success.
S_Serial1x:
	mov     Board.Serial_Valid,al
S_SerialX:
	pop     si
	pop     di
	ret
;----------------------------------------------------------------------------

;****************************************************************************
;
;      "V_Serial Routine"
;
;
;****************************************************************************
V_Serial:
       push     di
       mov      di,offset Board
       mov      ax,[di].BrdStore.Serial_Used.dblword.hw
       mov      NICContext.HsmContext.uniqueId.dblword.hw,ax
       mov      ax,[di].BrdStore.Serial_Used.dblword.lw
       mov      NICContext.HsmContext.uniqueId.dblword.lw,ax
       mov      al,[di].BrdStore.Serial_Valid
       pop      di
       ret
;----------------------------------------------------------------------------

;****************************************************************************
;
;      "S_Pcmcia Routine"
;
;****************************************************************************
S_Pcmcia:
	 push   di
	 push	si
         or     dword ptr Board.FoundKeys,mask Pcmcia_Found
         call   Parms_check
	 jne	S_Pcmcia1x
	 mov    al,SUCCESS          ; Exit with success.
S_Pcmcia1x:
;        mov    Board.Serial_Valid,al
S_PcmciaX:
	 pop    si
	 pop    di
	 ret
;----------------------------------------------------------------------------

;****************************************************************************
;
;      "V_Pcmcia Routine"
;
;
;****************************************************************************
V_Pcmcia:
       mov      di,offset Board
       ret
;----------------------------------------------------------------------------

;****************************************************************************
;
;      "S_Bussize Routine"
;
;****************************************************************************
S_Bussize:
	 push   di
	 push	si
         or     dword ptr Board.FoundKeys,mask Pcmcia_Found
         call   Parms_check
	 jne	S_Bussize1x
	 mov    al,SUCCESS          ; Exit with success.
S_Bussize1x:
;        mov    Board.Serial_Valid,al
S_BussizeX:
	 pop    si
	 pop    di
	 ret
;----------------------------------------------------------------------------

;****************************************************************************
;
;      "V_Bussize Routine"
;
;
;****************************************************************************
V_Bussize:
       mov      di,offset Board
       ret
;----------------------------------------------------------------------------

;****************************************************************************
;
;      "S_TxQueue Routine"
;
;
;****************************************************************************
S_TxQueue:
	 push   di
	 push	si
         or     dword ptr Board.FoundKeys,mask TxQue_Found
         call   Parms_check
	 jne	S_TxQueue1
	 lea    di,[di].keyword.kw_p1.Parameter
	 mov    al,es:[di]
	 mov    Board.TxQueue_Used,al
	 mov    al,SUCCESS
S_TxQueue1:
	 mov    Board.TxQueue_Valid,al
S_TxQueueX:
	 pop    si
	 pop    di
	 ret
;----------------------------------------------------------------------------

;****************************************************************************
;
;      "V_TxQueue Routine"
;
;
;****************************************************************************
V_TxQueue:
       push     si
       push     di
       mov      al,Board.TxQueue_Valid
       cmp      al,0
       jne      V_TxQueueX
       mov      di,offset Board
       mov      al,[di].BrdStore.TxQueue_Used
       cmp      al,NSM_TXQUEUE_MIN
       jb       V_TxQueue1

       cmp      al,NSM_TXQUEUE_MAX
       ja	V_TxQueue2
       jmp	V_TxQueue3
V_TxQueue1:
       mov	al,NSM_TXQUEUE_MIN
       mov      di,offset PEMsgK16
       call     PutCSt
       jmp	V_TxQueue3
V_TxQueue2:
       mov	al,NSM_TXQUEUE_MAX
       mov      di,offset PEMsgK17
       call     PutCSt
V_TxQueue3:
       mov      byte ptr NICContext.HsmContext.reqTxQSize,al
       mov      al,SUCCESS
V_TxQueueX:
       pop      di
       pop      si
       ret
;----------------------------------------------------------------------------

;****************************************************************************
;
;      "S_RxQueue Routine"
;
;
;****************************************************************************
S_RxQueue:
	 push   di
	 push	si
         or     dword ptr Board.FoundKeys,mask RxQue_Found
         call   Parms_check
	 jne	S_RxQueue1
	 lea    di,[di].keyword.kw_p1.Parameter
	 mov    al,es:[di]
	 mov    Board.RxQueue_Used,al
	 mov    al,SUCCESS
S_RxQueue1:
	 mov    Board.RxQueue_Valid,al
S_RxQueueX:
	 pop    si
	 pop    di
	 ret
;----------------------------------------------------------------------------

;****************************************************************************
;
;      "V_RxQueue Routine"
;
;
;****************************************************************************
V_RxQueue:
       push     si
       push     di
       mov      al,Board.RxQueue_Valid
       cmp      al,0
       jne      V_RxQueueX
       mov      di,offset Board
       mov	al,[di].BrdStore.RxQueue_Used
       mov      byte ptr NICContext.HsmContext.reqRxQSize,al
       mov      al,SUCCESS
V_RxQueueX:
       pop      di
       pop      si
       ret
;----------------------------------------------------------------------------

;****************************************************************************
;
;      "S_TxBurstCnt Routine"
;
;****************************************************************************
S_TxBurstCnt:
	 push   di
	 push	si
         or     dword ptr Board.FoundKeys,mask Pcmcia_Found
         call   Parms_check
	 jne	S_TxBurstCnt1x
	 mov    al,SUCCESS          ; Exit with success.
S_TxBurstCnt1x:
;        mov    Board.Serial_Valid,al
S_TxBurstCntX:
	 pop    si
	 pop    di
	 ret
;----------------------------------------------------------------------------

;****************************************************************************
;
;      "V_TxBurstCnt Routine"
;
;
;****************************************************************************
V_TxBurstCnt:
       mov      di,offset Board
       ret
;----------------------------------------------------------------------------

;****************************************************************************
;
;      "S_RxBurstCnt Routine"
;
;****************************************************************************
S_RxBurstCnt:
	 push   di
	 push	si
         or     dword ptr Board.FoundKeys,mask Pcmcia_Found
         call   Parms_check
	 jne	S_RxBurstCnt1x
	 mov    al,SUCCESS          ; Exit with success.
S_RxBurstCnt1x:
;        mov    Board.Serial_Valid,al
S_RxBurstCntX:
	 pop    si
	 pop    di
	 ret
;----------------------------------------------------------------------------

;****************************************************************************
;
;      "V_RxBurstCnt Routine"
;
;
;****************************************************************************
V_RxBurstCnt:
       ret
;----------------------------------------------------------------------------

;****************************************************************************
;
;      "S_RxEarlySz Routine"
;
;****************************************************************************
S_RxEarlySz:
	 push   di
	 push   si
	 or     Board.FoundKeys,mask Pcmcia_Found
	 call   Parms_check
	 jne    S_RxEarlySz1x
	 mov    al,SUCCESS          ; Exit with success.
S_RxEarlySz1x:
;        mov    Board.Serial_Valid,al
S_RxEarlySzX:
	 pop    si
	 pop    di
	 ret
;----------------------------------------------------------------------------

;****************************************************************************
;
;      "V_RxEarlySze Routine"
;
;
;****************************************************************************
V_RxEarlySz:
       push     di
       mov      di,offset Board
       pop      di
       ret
;----------------------------------------------------------------------------

;****************************************************************************
;
;      "S_Mediatype Routine"
;
;****************************************************************************
S_Mediatype:
	 push   di
	 push	si
         or     dword ptr Board.FoundKeys,mask Media_Found
         call   Parms_check
	 jne	S_Mediatype1X
	 mov    al,SUCCESS          ; Exit with success.
S_Mediatype1:
	 lea    di,[di].keyword.kw_p1.Parameter
	 mov    si,offset Media1
S_Mediatype2:
	 push   si
	 lea    si,[si].Media_Param.Media_String
	 call   StrCmp
	 pop    si
	 je     S_Mediatype3
	 add    si,sizeof Media_Param
	 cmp    si,MediasN
	 jne    S_Mediatype2
	 mov    al,BAD_MEDIA_TYPE
	 jmp    S_Mediatype1X
S_Mediatype3:
         mov    ah,[si].Media_Param.Media_Code.dblbyte.lb
         mov    byte ptr Board.Media_Used,ah
S_Mediatype1X:
	 mov    Board.Media_Valid,al
S_MediatypeX:
	 pop    si
	 pop    di
	 ret
;----------------------------------------------------------------------------

;****************************************************************************
;
;      "V_Mediatype Routine"
;
;
;****************************************************************************
V_Mediatype:
       mov      al,byte ptr Board.Media_Used
       test     al,byte ptr NICContext.HsmContext.mediaTypes
       jnz      V_MediatypeEX
       mov      byte ptr MediaTypeStore,al
       mov      al,SUCCESS
       jmp      V_MediatypeE
V_MediatypeEX:
       mov      al,BAD_MEDIA_TYPE
V_MediatypeE:
       mov      al,byte ptr Board.Media_Valid
       ret
;----------------------------------------------------------------------------

;****************************************************************************
;
;      "S_SoftCRC Routine"
;
;****************************************************************************
S_SoftCRC:
	 push	di
	 push   si
	 or	dword ptr dword ptr Board.FoundKeys,mask Softcrc_Fnd
	 call   Parms_check
	 jne	S_SoftCRC1X
	 mov    al,SUCCESS          ; Exit with success.
S_SoftCRC1:
	 lea    di,[di].keyword.kw_p1.Parameter
	 mov	si,offset On_String
S_SoftCRC2:
	 push   si
	 lea	si,[si].On_Off_Param.On_Off_String
	 call   StrCmp
	 pop    si
	 je	S_SoftCRC3
	 add	si,sizeof On_Off_Param
	 cmp	si,On_Off_End
	 jne	S_SoftCRC2
	 mov	al,BAD_ON_OFF
	 jmp	S_SoftCRC1X
S_SoftCRC3:
	 mov	ah,[si].On_Off_Param.On_Off_Code
	 mov	byte ptr Board.SoftCRC_Used,ah
S_SoftCRC1X:
	 mov	Board.SoftCRC_Valid,al
S_SoftCRCX:
	 pop	si
	 pop    di
	 ret
;----------------------------------------------------------------------------

;****************************************************************************
;
;      "V_SoftCRC Routine"
;
;
;****************************************************************************
V_SoftCRC:
       mov	al,byte ptr Board.SoftCRC_Used
       cmp	al,1
       jne	V_SoftCRC1
       or	word ptr NICContext.HsmContext.hsmOptions,DOES_SOFT_CRC
       or	word ptr NICContext.HsmContext.nsmOptions,DOES_SOFT_CRC
       jmp	V_SoftCRC2
V_SoftCRC1:
       and	word ptr NICContext.HsmContext.hsmOptions,not DOES_SOFT_CRC
       and	word ptr NICContext.HsmContext.nsmOptions,not DOES_SOFT_CRC
V_SoftCRC2:
       mov	al,byte ptr Board.SoftCRC_Valid
       ret
;----------------------------------------------------------------------------

;****************************************************************************
;
;      "S_SoftCRC Routine"
;
;****************************************************************************
S_No32BitIO:
	 push	di
	 push   si
	 or	dword ptr dword ptr Board.FoundKeys,mask No32BitIO_Fnd
	 call   Parms_check
	 jne	S_No32BitIO1X
	 mov    al,SUCCESS          ; Exit with success.
S_No32BitIO1:
	 lea    di,[di].keyword.kw_p1.Parameter
	 mov	si,offset On_String
S_No32BitIO2:
	 push   si
	 lea	si,[si].On_Off_Param.On_Off_String
	 call   StrCmp
	 pop    si
	 je	S_No32BitIO3
	 add	si,sizeof On_Off_Param
	 cmp	si,On_Off_End
	 jne	S_No32BitIO2
	 mov	al,BAD_ON_OFF
	 jmp	S_No32BitIO1X
S_No32BitIO3:
	 mov	ah,[si].On_Off_Param.On_Off_Code
	 mov	byte ptr Board.No32BitIO_Used,ah
S_No32BitIO1X:
	 mov	Board.No32BitIO_Valid,al
S_No32BitIOX:
	 pop	si
	 pop    di
	 ret
;----------------------------------------------------------------------------

;****************************************************************************
;
;      "V_No32BitIO Routine"
;
;
;****************************************************************************
V_No32BitIO:
       mov	al,byte ptr Board.No32BitIO_Used
       cmp	al,1
       jne	V_No32BitIO1
       or	word ptr NICContext.HsmContext.nsmOptions,DONT_USE_32BIT_IO
       jmp	V_No32BitIO2
V_No32BitIO1:
       and	word ptr NICContext.HsmContext.nsmOptions,not DONT_USE_32BIT_IO
V_No32BitIO2:
       mov	al,byte ptr Board.No32BitIO_Valid
       ret
;----------------------------------------------------------------------------

;****************************************************************************
;
;      "Parms_check Routine"
;
;****************************************************************************
Parms_check:
	 mov    di,dx
	 cmp    es:[di].keyword.kw_pno,1
	 jne    Parms_check1
	 mov    ax,[si].Kw_tbl.Kw_type
	 cmp    ax,es:[di].keyword.kw_p1.Param_typ
	 jne    Parms_check2
	 ret
Parms_check1:
	 mov    al,TOO_MANY_PARAMS
	 ret
Parms_check2:
	 mov    al,TYPE_ERROR
	 ret
;----------------------------------------------------------------------------

;****************************************************************************
;
;      "Parms_Err Routine"
;
;****************************************************************************
Parms_Err:
	push    di
	cmp     cl,0
	jne     Parms_Err1
	mov     di,offset PEMsg1
	call    PutCSt
Parms_Err1:
	mov     di,offset PEMsgK1
	call    PutCSt
	pop     di
	call    PutCSt
	push    di
	cmp     al,TOO_MANY_PARAMS
	jne     Parms_Err2
	mov     di,offset PEMsgK5
	jmp     Parms_Err14
Parms_Err2:
	cmp     al,TYPE_ERROR
	jne     Parms_Err3
	mov     di,offset PEMsgK6
	jmp     Parms_Err14
Parms_Err3:
	cmp     al,KEY_NOT_FOUND
	jne     Parms_Err4
	mov     di,offset PEMsgK3
	jmp     Parms_Err14
Parms_Err4:
	cmp     al,UNEXPECTED_KEY
	jne     Parms_Err5
	mov     di,offset PEMsgK4
	jmp     Parms_Err14
Parms_Err5:
	cmp     al,STRING_TOO_LONG
	jne     Parms_Err6
	mov     di,offset PEMsgK7
	jmp     Parms_Err14
Parms_Err6:
	cmp     al,BAD_BRD_CONFIG
	jne     Parms_Err7
	mov     di,offset PEMsgK8
	jmp     Parms_Err14
Parms_Err7:
	cmp     al,BAD_IOBASE
	jne     Parms_Err8
	mov     di,offset PEMsgK8
	jmp     Parms_Err14
Parms_Err8:
	cmp     al,BAD_INTLINE
	jne     Parms_Err9
	mov     di,offset PEMsgK9
	jmp     Parms_Err14
Parms_Err9:
	cmp     al,BAD_RAMADDR
	jne     Parms_Err10
	mov     di,offset PEMsgK10
	jmp     Parms_Err14
Parms_Err10:
	cmp     al,BAD_ENET_ADDR
	jne     Parms_Err11
	mov     di,offset PEMsgK11
	jmp     Parms_Err14
Parms_Err11:
	cmp     al,MULTCAST_ADDR
	jne     Parms_Err12
	mov     di,offset PEMsgK12
	jmp     Parms_Err14
Parms_Err12:
	cmp     al,BAD_PNP_SERIAL
	jne	Parms_Err13
	mov     di,offset PEMsgK13
	jmp     Parms_Err14
Parms_Err13:
	cmp     al,BAD_MEDIA_TYPE
	jne	Parms_Err14
	mov	di,offset PEMsgK14
	jmp	Parms_Err15
Parms_Err14:
	cmp	al,BAD_ON_OFF
	jne	Parms_Err16
	mov	di,offset PEMsgK15
Parms_Err15:
	call    PutCSt
Parms_Err16:
	pop     di
	ret
;----------------------------------------------------------------------------

;*****************************************************************************
;
;	"_NsmMallocPhys"
;
;       Allocate memory on behalf of the Hsm module.  Can only be called
;   during HsmInitialize( ).  This memory MUST be free'd if HsmShutdown
;   is called.  Hsm's should only use this for memory requirements that may
;   vary as a result of keyword option settings (i.e. TxQSize, RxQSize).
;
;*****************************************************************************
PUBLIC _NsmMallocPhys
_NsmMallocPhys:
IFDEF  OS2
_NsmMalcLin	equ	bp-4
_NsmMalcOff	equ	bp-6
_NsmMalcVMSel	equ	bp-2
	enter	6,0
	push	es
	push	esi
	push	edi
ELSE
	   push bp
	   mov	bp,sp
	   push es
	   push di
ENDIF
;IFDEF OS2
;	   int	3
;	   push bx
;
;	   mov	bx,[bp].NSMMalloc_Params.Mem_Size
;	   mov	ax,0
;	   mov	dh,0
;	   mov	dl,DH_ALLOCPHYS
;	   call	dword ptr DH_Addr
;	   les	di,[bp].NSMMallocPhys_Params.physaddr
;	   mov	es:[di].dblword.hw,ax
;	   mov	es:[di].dblword.lw,bx
;
;	   mov	cx,[bp].NSMMalloc_Params.Mem_Size
;	   mov	dh,1
;	   mov	dl,DH_PHYSTOUVIR
;	   call	dword ptr DH_Addr
;	   mov	dx,es
;	   mov	ax,bx
;	   pop	bx
;ELSE
_NsmMallocPhysR:
	   mov	dx,[bp].NSMMallocPhys_Params.Mem_Size
	   cmp	dx,2560			; if size >2.5Kbytes
	   ja	near ptr _NsmMallocPhysVM ; then use VMAlloc,
	   mov  ax,word ptr NSM_Heap	; else use Heap.
	   add  dx,ax
	   cmp  dx,word ptr dta_end
	   jbe	_NsmMallocPhys1
_NsmMallocPhysE:
	   mov  ax,0
	   mov  dx,0
	   jmp	_NsmMallocPhysX
_NsmMallocPhys1:
	   mov	word ptr NSM_Heap,dx
IFDEF OS2
		; Dword Alignment check by Linear Address.
	mov	word ptr [_NsmMalcOff],ax
	movzx	esi,ax			;start offset
	mov	ax,ds
	mov	dl,DH_VIRTTOLIN
	call	dword ptr [DH_Addr]
	jc	short _NsmMallocPhysE
	mov	dword ptr [_NsmMalcLin],eax
	and	ax,3
	jz	short _NsmMallocPhysD
	add	ax,-4
	sub	ax,word ptr [_NsmMalcOff]
	neg	ax
	mov	word ptr [NSM_Heap],ax
	jmp	_NsmMallocPhysR		;retry dword check

		; Page Alignment check by Linear Address.
_NsmMallocPhysD:
	movzx	esi,word ptr [NSM_Heap]
	mov	ax,ds
	dec	si			; end offset
	mov	dl,DH_VIRTTOLIN
	call	dword ptr [DH_Addr]
	jc	short _NsmMallocPhysE
	mov	esi,[_NsmMalcLin]
	mov	di,ax
	shr	eax,12
	shr	esi,12
	cmp	eax,esi
	jz	short _NsmMallocPhysP	; this range is in a page.
	inc	di
	and	di,0fffh
	sub	word ptr [NSM_Heap],di	; top virtual address of next page.
	jmp	short _NsmMallocPhysR	; retry at page top.
_NsmMallocPhysP:
	mov	ax,[_NsmMalcOff]

	   mov	dx,ds
	   push dx
	   push ax
	   mov	si,ax
	   mov	dl,DH_VIRTOPHYS
	   call	dword ptr DH_Addr
	   les	di,[bp].NSMMallocPhys_Params.physaddr
	   mov	es:[di].dblword.hw,ax
	   mov	es:[di].dblword.lw,bx
ELSE
	   mov	dx,cs
	   mov	di, 40h 	      ;Are Virtual DMA Services Present?
	   mov	es, di
	   mov	di, 7bh
	   test	byte ptr es:[di], 20h
	   push	dx
	   push	ax
	   jnz	_NsmMallocPhys2	      ; yes, let him do it
				      ; No, Compute physical address myself.
	   xor	 cx,cx		      ;Get 32-bit fragment physical address.
	   mov	 bx,dx
	   shl	 bx,1
	   rcl	 cx,1
	   shl	 bx,1
	   rcl	 cx,1
	   shl	 bx,1
	   rcl	 cx,1
	   shl	 bx,1
	   rcl	 cx,1
	   add	 bx,ax
	   adc	 cx,0
	   mov	 dx,cx
	   mov	 ax,bx
	   jmp	_NsmMallocPhys3

_NsmMallocPhys2:

;; Added following codes to resolve emm386 conflict
           mov  bx, ax                  ; (bx) = offset of logical address
;;
	   mov	ax,cs
	   mov	es,ax
	   mov	di,offset Config_Buf

;; Added following codes to resolve emm386 conflict
           xor  eax, eax                                ;
           mov  ax, [bp].NSMMalloc_Params.Mem_Size  ;
           mov  es:[di].MemDesc.MDSize, eax             ; set Region Size
           mov  ax, bx                                  ;
           mov  es:[di].MemDesc.MDOffset, eax           ; set Offset
           mov  es:[di].MemDesc.MDSegment, dx           ; set Segment
;;

	   mov	ax,8103h
	   mov	dx,0006h

;; Added following codes to resolve emm386 conflict
           mov  dx,0000h                                ; adjust FLAGS
;;

	   int	4bh

;; Added following codes to resolve emm386 conflict
           jc   _NsmMallocPhysError
;;

;; Removed following codes to resolve emm386 conflict
;;	   jnc	_NsmMallocPhys3
;;
	   mov	ax,word ptr es:[di].MemDesc.MDPhysAddr.dblword.lw
	   mov	dx,word ptr es:[di].MemDesc.MDPhysAddr.dblword.hw
_NsmMallocPhys3:
	   les	di,[bp].NSMMallocPhys_Params.physaddr
	   mov	es:[di].dblword.hw,dx
	   mov	es:[di].dblword.lw,ax
ENDIF
	   pop	ax
	   pop	dx
_NsmMallocPhysX:
;ENDIF
IFDEF  OS2
	pop	edi
	pop	esi
	pop	es
	leave
ELSE
	   pop	di
	   pop	es
	   pop	bp
ENDIF
	   ret

;;
;; Added following codes to resolve emm386 conflict
_NsmMallocPhysError:
           xor  ax, ax
           xor  dx, dx
           pop  di              ; dummy pop instead of "POP AX"
           pop  di              ; dummy pop instead of "POP DX"
           jmp  _NsmMallocPhysX
;;
;----------------------------------------------------------------------------

;	"_NsmMallocPhysVM"
;
;       Allocate large physical memory by DevHlp_VMAlloc. Only be called
;   during HsmValidateContext() in OS/2 mode. 
IFDEF  OS2
public	_NsmMallocPhysVM	; << debug >>
_NsmMallocPhysVM:
	push	ss	;Allocate GDT Selector for Virtual address.
	pop	es
	lea	di,[_NsmMalcVMSel]
	mov	cx,1
	mov	dl,DH_ALLOCGDT
	call	dword ptr [DH_Addr]
	jc	short _NsmMallocPhysVME1

;Convert variable's Virtual addr to Linear addr 
; to save Physical addr returned by VMAlloc

	movzx	esi,[bp].NSMMallocPhys_Params.physaddr.dblword.lw
	mov	ax,[bp].NSMMallocPhys_Params.physaddr.dblword.hw		;selector
	mov	dl,DH_VIRTTOLIN
	call	dword ptr [DH_Addr]
	jc	short _NsmMallocPhysVME3

	mov	edi,eax			;Get Memory block.
	movzx	ecx,[bp].NSMMallocPhys_Params.Mem_Size
	mov	eax, VMDHA_FIXED or VMDHA_CONTIG or VMDHA_USEHIGHMEM
	add	cx,3			; for dword align.
	mov	dl,DH_VMALLOC
	call	dword ptr [DH_Addr]
	jc	short _NsmMallocPhysVME2

	push	eax			;use if error.
	mov	ebx,eax			;Check dword alignment.
	and	eax,3
	jz	short _NsmMallocPhysVM1	; align is ok.
	add	al,-4
	neg	al
	les	di,[bp].NSMMallocPhys_Params.physaddr
	add	es:[di],eax
	add	ebx,eax

_NsmMallocPhysVM1:		;Convert to GDT selector for 16:16 access.
	movzx	ecx,[bp].NSMMallocPhys_Params.Mem_Size
	mov	ax,[_NsmMalcVMSel]
	mov	dl,DH_LINTOGDT
	call	dword ptr [DH_Addr]
	pop	edi			;discard linear address
	jc	short _NsmMallocPhysVME1
	mov	dx,[_NsmMalcVMSel]	;Virtual address.
	xor	ax,ax			;CANNOT use at INIT time.
_NsmMallocPhysVMX:
	pop	edi
	pop	esi
	pop	es
	leave
	ret
_NsmMallocPhysVME1:
	pop	eax
	mov	dl,DH_VMFREE
	call	dword ptr [DH_Addr]	;Free memory
_NsmMallocPhysVME2:
	mov	ax,[_NsmMalcVMSel]
	mov	dl,DH_FREEGDT
	call	dword ptr [DH_Addr]	;Free selector
_NsmMallocPhysVME3:
	xor	ax,ax
	xor	dx,dx
	jmp	short _NsmMallocPhysVMX
ENDIF


;*****************************************************************************
;
;	"_NsmMalloc"
;
;       Allocate memory on behalf of the Hsm module.  Can only be called
;   during HsmInitialize( ).  This memory MUST be free'd if HsmShutdown
;   is called.  Hsm's should only use this for memory requirements that may
;   vary as a result of keyword option settings (i.e. TxQSize, RxQSize).
;
;*****************************************************************************
PUBLIC _NsmMalloc
_NsmMalloc:
	   push bp
	   mov  bp,sp
	   mov  dx,[bp].NSMMalloc_Params.Mem_Size
	   mov  ax,word ptr NSM_Heap
	   add  dx,ax
	   cmp  dx,word ptr dta_end
	   jbe  _NsmMalloc1
	   mov  ax,0
	   mov  dx,0
	   jmp  _NsmMallocX
_NsmMalloc1:
	   mov  word ptr NSM_Heap,dx
IFDEF OS2
	   mov  dx,ds
ELSE
	   mov  dx,cs
ENDIF
_NsmMallocX:
	   mov  sp,bp
	   pop  bp
	   ret
;-------------------------------------------------------------------------------

;*****************************************************************************
;
;       "_NsmFree/_NsmFreePhys"
;
;       Free memory previously allocated using NsmMalloc( ).  This should
;   only be called during HsmShutdown( ).
;
;*****************************************************************************
PUBLIC _NsmFree
_NsmFree:
PUBLIC _NsmFreePhys
_NsmFreePhys:
	   push bp
	   mov  bp,sp

_NsmFreeX:
	   mov  sp,bp
	   pop  bp
	   ret
;-------------------------------------------------------------------------------

;****************************************************************************
;
;      "STRCMP Routine"
;
;   Subroutine to compare two ASCII strings for equality.  At entry, DS:SI
;   points to one string, and ES:DI points to the other.   The Z-flag is
;   set at exit if the two strings match.  The SI- and DI-regs. are lost.
;****************************************************************************
StrCmp: push    ax              ;Save AX-reg.
	push    si
	push    di
StrNxt: mov     ah,[si]         ;Get next byte from each string.
	inc     si
	mov     al,es:[di]
	inc     di
	test    ax,ax           ;End of both strings?
	jz      StrXit         ;Yes, go exit.
	cmp     al,'a'          ;Is 2nd byte below 'a'?
	jb      StrChr         ;Yes, not lower-case -- compare bytes.
	cmp     al,'z'          ;Is 2nd byte above 'z'?
	ja      StrChr         ;Yes, not lower-case -- compare bytes.
	sub     al,'a'-'A'      ;Convert byte to upper-case.
StrChr: cmp     ah,al           ;Compare next byte of each string.
	je      StrNxt         ;If equal, check for string end.
StrXit: pop     di
	pop     si
	pop     ax              ;Reload AX-reg.
	ret                     ;Exit.
;-----------------------------------------------------------------------------

;*****************************************************************************
;
;      "PutCSt Routine"
;
;      Subroutine to display CRT messages.
;
;*****************************************************************************
PUBLIC PutCSt
PutCSt:
PutDSt: push    ax              ;Data-segment message -- save AX-reg.
	mov     ax,ds           ;Use DS-reg. as message base.
PutStr: push    bx              ;Save remaining regs.
	push    cx
	push    dx
	push    si
	push    di
	push    ds
	push    es
	mov     ds,ax           ;Set DS- and ES-reg. to message segment.
	mov     es,ax
	xor     ax,ax           ;Put a "null" in AX-reg.
	mov     cx,0FFFFh       ;Set scan-count limit in CX-reg.
	push    di              ;Scan for terminating "null".
	repne   scasb
	pop     di
	not     cx              ;Get string length - 1 (don't include "null").
	dec     cx
IFDEF   OS2
	push    1               ;OS/2 -- output string to STDOUT.
	push    cx
	push    ds
	push    di
	call    DOSPUTMESSAGE
ELSE
	mov     dx,di           ;DOS -- set string pointer in DS:DX
	mov     bx,STDOUT       ;Specify STDOUT device.
	mov     ah,WRITEF       ;Output this string.
	push    cs              ;Save CS-reg.
	int     021h            ;Do desired DOS function.
	pop     ds              ;Reload DS-reg.
ENDIF
	sti
	pop     es              ;Reload regs.
	pop     ds
	pop     di
	pop     si
	pop     dx
	pop     cx
	pop     bx
	pop     ax
	ret                       ;Exit.
;----------------------------------------------------------------------------

;*****************************************************************************
;
;      "PUTWORD Routine"
;
;      Subroutine to displays a word or an number of bytes form the start of
;   the word. BX is passed as a parameter for the number of characters
;
;
;*****************************************************************************
PutWord:
	   mov   al,' '            ;Put a Space in AX-reg.
	   mov   cx,0FFFFh         ;Set scan-count limit in CX-reg.
	   push  di                ;Scan for terminating "null".
	   repne scasb
	   pop   di
	   not   cx                ;Get string length - 1
	   dec   cx                ;(don't include "Space").
	   cmp   cx,bx
	   jbe   Hard1_Absent1
	   mov   cx,bx
Hard1_Absent1:
IFDEF   OS2
	   push  1                 ;OS/2 -- output string to STDOUT.
	   push  cx
	   push  ds
	   push  di
	   call DOSPUTMESSAGE
ELSE
	   mov   dx,di             ;DOS -- set string pointer in DS:DX
	   mov   bx,STDOUT         ;Specify STDOUT device.
	   mov   ah,WRITEF         ;Output this string.
	   push  cs                ;Save CS-reg.
	   int   021h              ;Do desired DOS function.
	   pop   ds                ;Reload DS-reg.
ENDIF
	   ret
;-------------------------------------------------------------------------------

IFDEF OS2
;*****************************************************************************
;
;      "Device Helper Routine"
;
;       Subroutine to do initialization calls to the OS/2 "Device-Help" logic.
;
;*****************************************************************************
DevHlp: push    ds              ;Save DS-reg.
	call    dword ptr DH_Addr
				;Call OS/2 "Device-Help" logic.
	pop     ds              ;Reload DS-reg.
	jnc     PutStX          ;If no errors, exit.
	pop     di              ;Error!  Discard return address.
	jmp     I_Fail          ;Go display "failure-to-init" message.
PutStX: ret
;-------------------------------------------------------------------------------

_TEXT     ends
_DATA   segment word public use16 'DATA'
ENDIF
;*****************************************************************************
; Start address of the PC Bios Rom.
;*****************************************************************************
BIOSStart dd    ?
;-----------------------------------------------------------------------------

;*****************************************************************************
; Heap for Configuration buffer.
;*****************************************************************************
Config_Buf        dd    280*2 +1440  dup (?)
;-----------------------------------------------------------------------------

;*****************************************************************************
; Plug-n-Play Entry Point.
;*****************************************************************************
PnPEntry        dd      ?
;-----------------------------------------------------------------------------

;*****************************************************************************
; Plug-n-Play Initiation Key.
;*****************************************************************************
PnPInitKey      db      06Ah,0B5h,0DAh,0EDh
		db      0F6h,0FBh,07Dh,0BEh
		db      0DFh,06Fh,037h,01Bh
		db      00Dh,086h,0C3h,061h
		db      0B0h,058h,02Ch,016h
		db      08Bh,045h,0A2h,0D1h
		db      0E8h,074h,03Ah,09Dh
		db      0CEh,0E7h,073h,039h
;-----------------------------------------------------------------------------

;*****************************************************************************
; Plug-n-Play Read_port IO Address store.
;*****************************************************************************
PnPRdIOaddr     dw      0203H
;-----------------------------------------------------------------------------

;*****************************************************************************
; Plug-n-Play Current CSN store.
;*****************************************************************************
PnPCSN_Store    dw      01h
;-----------------------------------------------------------------------------

;*****************************************************************************
; Plug-n-Play Card ID store.
;*****************************************************************************
		db     'PnPCardID='
PnPCardID       db     9 dup (0)
;-----------------------------------------------------------------------------

;*****************************************************************************
; Bitmask store for allowed keywords.
;*****************************************************************************
kwordsAllowed	 dd	 HSM_KEYWORDS
Hsm_Name	 db	 HSM_HSM_NAME,0
PUBLIC Hsm_Short_Name
Hsm_Short_Name	 db	 18 dup (0)
;-----------------------------------------------------------------------------

;*****************************************************************************
; Table of keyword definitions (15 bytes max.) and related processing offsets.
;*****************************************************************************
Key_Wrd1   Kw_tbl {'DRIVERNAME',1,offset S_Name,0}
Key_Wrd2   Kw_tbl {'IOBASE',0,offset S_Base,offset V_Base}
Key_Wrd3   Kw_tbl {'RAMADDRESS',1,offset S_Ram,offset V_Ram}
Key_Wrd4   Kw_tbl {'INTERRUPT',0,offset S_Irq,offset V_Irq}
Key_Wrd5   Kw_tbl {'SLOT',0,offset S_Slot,offset V_Slot}
Key_Wrd6   Kw_tbl {'NETADDRESS',1,offset S_Net,offset V_Net}
Key_Wrd7   Kw_tbl {'SERIAL',0,offset S_Serial,offset V_Serial}
Key_Wrd8   Kw_tbl {'PCMCIA',1,offset S_Pcmcia,offset V_Pcmcia}
Key_Wrd9   Kw_tbl {'BUSSIZE',0,offset S_Bussize,offset V_Bussize}
Key_Wrd10  Kw_tbl {'TXQUEUE',0,offset S_TxQueue,offset V_TxQueue}
Key_Wrd11  Kw_tbl {'RXQUEUE',0,offset S_RxQueue,offset V_RxQueue}
Key_Wrd12  Kw_tbl {'TXBURSTCOUNT',0,offset S_TxBurstCnt,offset V_TxBurstCnt}
Key_Wrd13  Kw_tbl {'RXBURSTCOUNT',0,offset S_RxBurstCnt,offset V_RxBurstCnt}
Key_Wrd14  Kw_tbl {'RXEARLYSIZE',0,offset S_RxEarlySz,offset V_RxEarlySz}
Key_Wrd15  Kw_tbl {'MEDIATYPE',1,offset S_Mediatype,offset V_Mediatype}
Key_Wrd16  Kw_tbl {'BROADCASTFILTER',0,offset S_TxBurstCnt,offset V_TxBurstCnt}
Key_Wrd17  Kw_tbl {'DRVDEBUG',0,offset S_RxBurstCnt,offset V_RxBurstCnt}
Key_Wrd18  Kw_tbl {'RXEARLYSIZE',0,offset S_RxEarlySz,offset V_RxEarlySz}
Key_Wrd19  Kw_tbl {'SOFTCRC',1,offset S_SoftCRC,offset V_SoftCRC}
Key_Wrd20  Kw_tbl {'NO32BITIO',1,offset S_No32BitIO,offset V_No32BitIO}
Key_WrdN equ	$
;-----------------------------------------------------------------------------

;*****************************************************************************
; Table of Addresses of Protocol.ini bad keywords.
;*****************************************************************************
No_Of_BadKeys   dw 0
BadKeys dd      40 dup (0)
;-----------------------------------------------------------------------------

;*****************************************************************************
; Configured board information store.
;*****************************************************************************
DriverInst      db      0
;-----------------------------------------------------------------------------

;*****************************************************************************
; Configured board information store.
;*****************************************************************************
Board   BrdStore  {?,offset Board1,?}
;-----------------------------------------------------------------------------

;*****************************************************************************
; Table of possible board configurations from this driver.
;*****************************************************************************
Board1  BoardType {'TIGRIS',?,<0,0,0,0,1,1>\
		    ,<0,0,0,0,1,0>,{},{},0,{},{},{}}
BoardN  equ       $
;-----------------------------------------------------------------------------

;*****************************************************************************
; Table of possible Boolean values.
;*****************************************************************************
On_String  On_Off_Param {1,"ON"}
Off_String On_Off_Param {0,"OFF"}
On_Off_End equ $
;-----------------------------------------------------------------------------

;*****************************************************************************
; Table of possible Networking media supported by this driver.
;*****************************************************************************
Media1  Media_Param {HSM_MEDIA_AUTO_CONFIG,"AUTO"}
Media2  Media_Param {HSM_MEDIA_10AUI,"AUI"}
Media3  Media_Param {HSM_MEDIA_10BASE2,"THIN"}
Media4  Media_Param {HSM_MEDIA_10BASET,"UTP"}
Media5  Media_Param {HSM_MEDIA_10BASETfx,"UTPFULL"}
Media6  Media_Param {HSM_MEDIA_100BASEX,"FAST"}
Media7  Media_Param {HSM_MEDIA_100BASEXfx,"FASTFULL"}
MediasN equ         $
;-----------------------------------------------------------------------------
IFDEF  NECWARP
;
; 'OEMHLP$' servies, instead of direct call to BIOS Physical address.
;  driver name and handle for 'OEMHLP$' access.
NameOEMHLP	db	'OEMHLP$',0
hdlOEMHLP	dw	?
ENDIF

IFNDEF  OS2
;
; DOS "Auxiliary Stack" area, used only during initialization.
;
SaveSS  dw      ?               ;Saved stack segment.
SaveSP  dw      ?               ;Saved stack pointer.
	dw      256 dup (?)     ;Auxiliary stack area.
StakTop equ     $               ;("Top-of-stack" pointer).
dvr_end     equ        $
_TEXT           ends
ELSE
_DATA   ends
ENDIF
end

;*****************************************************************************
;*              National Semiconductor Company Confidential                  *
;*                                                                           *
;*                          National Semiconductor                           *
;*                       NDIS 2.0.1 MAC device driver                        *
;*           Code for National Semiconductor's AT/LANTIC Board               *
;*                                                                           *
;*	Source Name:	NSMDEF.inc					     *
;*	Authors:							     *
;*                       Frank Dimambro                                      *
;*									     *
;*	$Log:   /home/crabapple/nsclib/tech/ndis2/include/vcs/nsmdef.inv  $								     *
;	
;	   Rev 1.1   10/12/95 14:30:52   frd
;	Updated minor revision number of driver from 2 to 3.
;	
;	   Rev 1.0   09/14/95 17:25:30   frd
;	Initial revision.
;	
;	   Rev 1.13   24 May 1995 17:13:38   FRD
;	 
;	
;	   Rev 1.12   24 May 1995 11:25:52   FRD
;	 
;	
;	   Rev 1.11   24 May 1995 09:26:06   FRD
;	 
;	
;	   Rev 1.0   24 May 1995 09:25:18   FRD
;	 
;	
;	   Rev 1.10   17 May 1995 16:37:46   FRD
;	Updated driver version to 1.0
;	
;	   Rev 1.9   17 May 1995 15:55:02   FRD
;	 
;	
;	   Rev 1.8   17 May 1995 15:14:30   FRD
;	 
;	
;	   Rev 1.7   17 May 1995 14:12:04   FRD
;	 
;	
;	   Rev 1.6   17 May 1995 14:08:00   FRD
;	Added parameter definitions for NsmWaitTime
;	
;	   Rev 1.4   01 May 1995 13:02:18   FRD
;	 
;	
;	   Rev 1.0   01 May 1995 12:59:28   FRD
;	Add in Timer interrupt service routine flag to avoid
;	re-entrance.
;	
;	   Rev 1.3   16 Apr 1995 12:37:04   FRD
;	Added PCI constants
;	
;	   Rev 1.2   12 Apr 1995 13:57:40   FRD
;	 
;*									     *
;*****************************************************************************

NSMDEF_INC_ equ 1

	.lfcond       ; List conditionally-unassembled source lines.
	.seq
	.386p	      ; Processor Type....Important that this occurs after
		      ; model definition.
;*****************************************************************************
; Make sure OEM_SHORT_NAME is a valid size.
;*****************************************************************************
.erre (@SizeStr(%OEM_SHORT_NAME) GT 2) AND (@SizeStr(%OEM_SHORT_NAME) LT 10)
;-----------------------------------------------------------------------------

;*****************************************************************************
; General NDIS Driver Definitions
;*****************************************************************************
MAJ_VN	   equ	1		;MAC major version number.
MIN_VN     equ  5               ;MAC minor version number.
PMAJ_VN    equ	02h		;Protocol Manager major version number.
PMIN_VN    equ	00h		;Protocol Manager minor version number.
PM_VEXP    equ	00001h		;Expected Protocol-Manager min/maj version no.
PKTMAX	   equ	1514		;Maximum Ethernet physical packet size.
PKTMIN	   equ	60		;Minimum Ethernet physical packet size.
MXIMMED    equ	64		;Maximum output immediate-data size.
MXIPKTS    equ	4		;Maximum input packets per interrupt (127 max).
ADDRLEN    equ	6		;Number of bytes in Ethernet address.
DS_GDT	   equ	2		;Virtual (GDT) pointer type.
DefMxTx    equ	4		;Number of transmit que elements supported.
CR	   equ	00Dh		;ASCII carriage-return.
LF	   equ	00Ah		;ASCII line-feed.
TRUE	   equ	1h
FALSE	   equ	0h
MAXDVRINST equ	8h		; Maximum Number if driver instances.
MAXIOLOCS  equ	8h
MAXINTLINES		equ	8h
TIMER_GRANULARITY	equ    54
;-----------------------------------------------------------------------------

;*****************************************************************************
; Constants for the Memory required for Hardware dependent blocks
;*****************************************************************************
Size_Data	equ		0E00h
Size_Func	equ		800h
;-----------------------------------------------------------------------------

;*****************************************************************************
; Interval Timer constants.
;*****************************************************************************
COUNNTER0	equ	040h	;IO address of interval timer.
CNTR_CMD	equ	043h	;Command address for interval timer
;-----------------------------------------------------------------------------

;*****************************************************************************
; Int 21 function calls
;*****************************************************************************
DOSVERGET	equ	030h	;Interrupt 21 function get DOS version.
SETIRQ		equ	025h	;Interrupt 21 setup interrupt service routine.
GETIRQ		equ	035h	;Interrupt 21 get interrupt service routine.
WRITEF		equ	040h	;Interrupt 21 writefile function routine.
OPEN_FILE       equ     03Dh    ;Interrupt 21 Open file with handle function.
CLOSE_FILE      equ     03Eh    ;Interrupt 21 Close file with handle function.
IOCTL           equ     044h    ;Interrupt 21 IO control function.
;-----------------------------------------------------------------------------

;*****************************************************************************
; Int 1Ah function calls
;*****************************************************************************
GET_CLOCK_COUNT  equ	00h	;Interrupt 1Ah function get Clock Cnt.
GET_TIME	 equ	02h	;Interrupt 1Ah function get Current time.
GET_DATE	 equ	04h	;Get Current Date.
NDIS_START_YEAR	 equ	1970	;Start time for NDIS
SCNDS_PER_YEAR	 equ	SCNDS_PER_HOUR*(365*24+6)
SCNDS_PER_DAY	 equ	24*SCNDS_PER_HOUR
SCNDS_PER_HOUR	 equ	60*SCNDS_PER_MIN
SCNDS_PER_MIN	 equ	60
;-----------------------------------------------------------------------------

;*****************************************************************************
; DOS driver file access calls
;*****************************************************************************
INIT		equ	00h	;Driver initialisation call.
OPEN            equ     0Dh     ;Driver Open call.
CLOSE           equ     0Eh     ;Driver Close call.
READ            equ     04h     ;Driver Read call.
WRITE           equ     08h     ;Driver Write call.
;-----------------------------------------------------------------------------

;*****************************************************************************
; Interrupt Mapping constants.
;*****************************************************************************
INT_TABLE1	equ	008h	;Table if interrups for IRQ0 to IRQ7
INT_TABLE2      equ     060h    ;Table if interrups for IRQ8 to IRQ15
TIMER_INT	equ	008h	;Timer Interrupt line.
;-----------------------------------------------------------------------------

;*****************************************************************************
; Writefile standard handles
;*****************************************************************************
STDOUT		equ	001h	; Handle to standard output.
;-----------------------------------------------------------------------------

;*****************************************************************************
; Openfile parameters.
;*****************************************************************************
SHARED_RW      equ	0C2h	; Handle to standard output.
;-----------------------------------------------------------------------------

;*****************************************************************************
; IOCTL subfunctions.
;*****************************************************************************
RXCTRLDATA     equ	002h	;Receive control data from char device.
;-----------------------------------------------------------------------------

;*****************************************************************************
; PC Interrupt control Access Constants.
;*****************************************************************************
EOI	equ	020h		;End-of-interrupt command.
ICR1    equ     020h            ;1st Interrupt-Control port.
IMR1    equ     021h            ;1st Interrupt-Mask port.
ICR2    equ     0A0h            ;2nd Interrupt-Control port.
IMR2	equ	0A1h		;2nd Interrupt-Mask port.
;-----------------------------------------------------------------------------

;*****************************************************************************
; PC PCI Constants
;*****************************************************************************
PCICONFIGINFO	equ	 0B1h
PCIBIOSPRESENT	equ	 001h
INT_1A		equ	 0FE6Eh
;-----------------------------------------------------------------------------

;*****************************************************************************
; PC EISA Constants
;*****************************************************************************
EISACONFIGINFO	equ	 0D8h
INT_15		equ	 0F859h
BRIEF_CONFIG	equ	 000h
FULL_CONFIG	equ	 001h
BAD_EISA_CONFIG equ	 087h
SLOT1_ADDR	equ	 01000h
LAST_SLOT_ADR	equ	 08000h
;-----------------------------------------------------------------------------

;*****************************************************************************
; PC Microchannel Constants.
;*****************************************************************************
MC_POSENL	equ	0096h	;Offset to POS Enable byte.
MC_IDLOFS       equ     0100h   ;Offset to card I.D. Lowbyte
MC_IDHOFS       equ     0101h   ;Offset to card I.D. Lowbyte
MC_OSDB1        equ     0102h   ;Offset to Option select data byte 1 address.
MC_OSDB2        equ     0103h   ;Offset to Option select data byte 2 address.
MC_ETHAD        equ     0082h   ;Offset to permanent Ethernet station address.
MC_IMASK        equ     0006h   ;Mask to extract POS interrupt-vector value.
MC_AMASK        equ     00F0h   ;Mask to extract POS IO base address value.
MC_POSENB       equ     0008h   ;Bit to enable POS.
MC_ADDRMASK     equ     00F0h   ;IO address in pos reg 1 mask.
;-----------------------------------------------------------------------------

;*****************************************************************************
; PC PCMCIA Card Services BIOS entry Constant.
;*****************************************************************************
PCMCIA_CS	equ	00AFh	; Card services int 1Ah function call.
;-----------------------------------------------------------------------------

;*****************************************************************************
; PC PCMCIA Client Services FunctionsConstants.
;*****************************************************************************
GET_CARD_SERVICES_INFO		equ	0Bh
REGISTER_CLIENT			equ	10h
DEREGISTER_CLIENT		equ	02h
GET_STATUS			equ	0Ch
RESET_CARD			equ	11h
SET_EVENT_MASK			equ	31h
GET_EVENT_MASK			equ	2Eh
;-----------------------------------------------------------------------------

;*****************************************************************************
; PC PCMCIA Resource Management Functions
;*****************************************************************************
REQUEST_IO			equ	1Fh
RELEASE_IO			equ	1Bh
REQUEST_IRQ			equ	20h
RELEASE_IRQ			equ	1Ch
REQUEST_WINDOW			equ	21h
RELEASE_WINDOW			equ	1Dh
MODIFY_WINDOW			equ	17h
MAP_MEM_PAGE			equ	14h
REQUEST_SOCKET_MASK		equ	22h
RELEASE_SOCKET_MASK		equ	2Fh
REQUEST_CONFIGURATION		equ	30h
GET_CONFIGURATION_INFO		equ	04h
MODIFY_CONFIGURATION		equ	27h
RELEASE_CONFIGURATION		equ	1Eh
;-----------------------------------------------------------------------------

;*****************************************************************************
; PC PCMCIA Client Utilities Functions
;*****************************************************************************
GET_FIRST_TUPLE			equ	07h
GET_NEXT_TUPLE			equ	0Ah
GET_TUPLE_DATA			equ	0Dh
GET_FIRST_REGION		equ	06h
GET_NEXT_REGION			equ	09h
GET_FIRST_PARTITION		equ	05h
GET_NEXT_PARTITION		equ	08h
;-----------------------------------------------------------------------------

;*****************************************************************************
; PC PCMCIA Bulk Memory Services Functions
;*****************************************************************************
OPEN_MEMORY			equ	18h
READ_MEMORY			equ	19h
WRITE_MEMORY			equ	24h
COPY_MEMORY			equ	01h
REGISTER_ERASE_QUEUE		equ	0Fh
CHECK_ERASE_QUEUE		equ	26h
DEREGISTER_ERASE_QUEUE		equ	25h
CLOSE_MEMORY			equ	00h
;-----------------------------------------------------------------------------

;*****************************************************************************
; PC PCMCIA Advanced Client Services Functions
;*****************************************************************************
RETURN_SS_ENTRY			equ	23h
MAP_LOG_SOCKET			equ	12h
MAP_PHY_SOCKET			equ	15h
MAP_LOG_WINDOW			equ	13h
MAP_PHY_WINDOW			equ	16h
REGISTER_MTD			equ	1Ah
REGISTER_TIMER			equ	28h
SET_REGION			equ	29h
VALIDATE_CIS			equ	2Bh
REQUEST_EXCLUSIVE		equ	2Ch
RELEASE_EXCLUSIVE		equ	2Dh
GET_FIRST_CLIENT		equ	0Eh
GET_NEXT_CLIENT			equ	2Ah
GET_CLIENT_INFO			equ	03h
ADD_SOCKET_SERVICES		equ	32h
REPLACE_SOCKET_SERVICES		equ	33h
VENDOR_SPECIFIC			equ	34h
ADJUST_RESOURCE_INFO		equ	35h
;-----------------------------------------------------------------------------

;*****************************************************************************
; PC PCMCIA Return Codes from CS 2.0 spec appendix C
;*****************************************************************************
SUCCESS				equ	00h
BAD_ADAPTER			equ	01h
BAD_ATTRIBUTE			equ	02h
BAD_BASE			equ	03h
BAD_EDC				equ	04h
BAD_IRQ				equ	06h
BAD_OFFSET			equ	07h
BAD_PAGE			equ	08h
READ_FAILURE			equ	09h
BAD_SIZE			equ	0ah
BAD_SOCKET			equ	0bh
BAD_TYPE			equ	0dh
BAD_VCC				equ	0eh
BAD_VPP				equ	0fh
BAD_WINDOW			equ	11h
WRITE_FAILURE			equ	12h
NO_CARD				equ	14h
UNSUPPORTED_FUNCTION 		equ	15h
UNSUPPORTED_MODE		equ	16h
BAD_SPEED			equ	17h
BUSY				equ	18h
GENERAL_FAILURE 		equ	19h
WRITE_PROTECTED			equ	1ah
BAD_ARG_LENGTH			equ	1bh
BAD_ARGS			equ	1ch
CONFIGURATION_LOCKED		equ	1dh
IN_USE				equ	1eh
NO_MORE_ITEMS			equ	1fh
OUT_OF_RESOURCE			equ	20h
BAD_HANDLE			equ	21h
;-----------------------------------------------------------------------------

;*****************************************************************************
; PC PCMCIA Event Codes from CS 2.0 spec appendix B
;*****************************************************************************
BATTERY_DEAD			equ	0001h
BATTERY_LOW			equ	0002h
CARD_INSERTION			equ	0040h
CARD_LOCK			equ	0003h
CARD_READY			equ	0004h
CARD_REMOVAL			equ	0005h
CARD_RESET			equ	0011h
CARD_UNLOCK			equ	0006h
CLIENT_INFO			equ	0014h
EJECTION_COMPLETE		equ	0007h
EJECTION_REQUEST		equ	0008h
ERASE_COMPLETE			equ	0081h
EXCLUSIVE_COMPLETE		equ	000dh
EXCLUSIVE_REQUEST		equ	000eh
INSERTION_COMPLETE		equ	0009h
INSERTION_REQUEST		equ	000ah
PM_RESUME			equ	000bh
PM_SUSPEND			equ	000ch
REGISTRATION_COMPLETE		equ	0082h
RESET_COMPLETE			equ	0080h
RESET_PHYSICAL			equ	000fh
RESET_REQUEST			equ	0010h
MTD_REQUEST			equ	0012h
TIMER_EXPIRED			equ	0015h
SS_UPDATED			equ	0016h
;-----------------------------------------------------------------------------

;*****************************************************************************
; PC PCI Constants
;*****************************************************************************
PCICONFIGINFO	       equ	 0B1h
PCIBIOSPRESENT	       equ	 001h
PCIFINDDEVICE	       equ	 002h
PCIREADCONFIGBYTE      equ	 008h
PCIREADCONFIGWORD      equ	 009h
PCIREADCONFIGDWORD     equ	 00Ah
INT_1A		equ	 0FE6Eh
;-----------------------------------------------------------------------------

;*****************************************************************************
; PC PCI Register offsets
;*****************************************************************************
PCICFGVENDORID	       equ	 00h
PCICFGDeviceId	       equ	 02h
PCICFGCOMMAND	       equ	 04h
PCICFGSTATUS	       equ	 06h
PCICFGREVISIONID       equ	 08h
PCICFGCLASSCODE        equ	 09h
PCICFGCACHELINESIZE    equ	 0Ch
PCICFGLATENCYTIMER     equ	 0Dh
PCICFGHEADERTYPE       equ	 0Eh
PCICFGBIST	       equ	 0Fh
PCICFGBASEADDR0	       equ	 10h
PCICFGBASEADDR1	       equ	 14h
PCICFGBASEADDR2	       equ	 18h
PCICFGBASEADDR3	       equ	 1Ch
PCICFGBASEADDR4        equ	 24h
PCICFGBaseAddr5	       equ	 28h
PCICFGROMBASEADDR      equ	 30h
PCICFGINTLINE	       equ	 3Ch
PCICFGINTPIN	       equ	 3Dh
PCICFGMINGNT	       equ	 3Eh
PCICFGMAXLAT	       equ	 3Fh
;-----------------------------------------------------------------------------

;*****************************************************************************
; PC PCI Base Address Constants
 ;*****************************************************************************
PCI_BASE_ADDR_IO       equ	 01h
;-----------------------------------------------------------------------------

;*****************************************************************************
; PC EISA Constants
;*****************************************************************************
EISACONFIGINFO	equ	 0D8h
INT_1AH		equ	 0F859h
BRIEF		equ	 000h
SLOT1_ADDR	equ	 01000h
LAST_SLOT_ADR	equ	 08000h
;-----------------------------------------------------------------------------

;*****************************************************************************
; PC Plug-n-Play Constants
;*****************************************************************************
VxDID	      equ	 00034h
VxDFunc       equ	 01684h
ISADEVICE     equ	 001h
EISADEVICE    equ	 002h
PCIDEVICE     equ	 004h
PCMCIADEVICE  equ	 0008h
PNPISADEVICE  equ	 0010h
MCADEVICE     equ	 0020h
MAX_MEM_REGS  equ	 0009h
MAX_IO_PORTS  equ	 0014h
MAX_IRQS      equ	 0007h
MAX_DMA_CHANNELS equ	 0007h
CM_GetVersion equ	 0000h
CM_GetConfig  equ	 0001h
PNPWRIOADDR   equ	 0279h
PNPWRDATA     equ	 0A79h
PNP_MAX_TRIES equ	 0003h
PNPINITKEYSZ  equ	 0020h
PNP_DELAY     equ	 0004h
;-----------------------------------------------------------------------------

;*****************************************************************************
; Strategy Request packet error codes
;*****************************************************************************
STBADCMD equ	00003h		;"Strategy" packet "Unknown command" status.
;-----------------------------------------------------------------------------

;*****************************************************************************
; Common-Characteristics Table Constants.
;*****************************************************************************
UL_MAC	equ	1		;Protocol level at upper boundary.
UI_MAC  equ     1               ;Type of interface at upper boundary.
LL_PHYS equ     0               ;Protocol level at lower boundary.
LI_MAC	equ	1		;Type of interface at lower boundary.
;-----------------------------------------------------------------------------

;*****************************************************************************
; MAC service specific characteristics table Receive type opcodes.
;*****************************************************************************
RXCHAIN 	 equ	1h
RXLOOKAHEAD	 equ	0h
;-----------------------------------------------------------------------------

;*****************************************************************************
; MAC service status flags opcodes and flags.
;*****************************************************************************
HARDWARE_ABSENT  equ	00000h
START_DIAG_FAIL  equ	00001h
CONFIG_FAULT	 equ	00002h
HARD_FAULT	 equ	00003h
SOFT_FAULT	 equ	00004h
FULLY_FUNCTIONAL equ	00007h
CLOSED		 equ	0h
UNBOUND 	 equ	0h
;-----------------------------------------------------------------------------

;*****************************************************************************
; Protocol Manager IOCTL category code and function code
;*****************************************************************************
IOC_NC	equ	081h
IONC_PM equ	058h
;-----------------------------------------------------------------------------

;*****************************************************************************
; Protocol-Manager Opcodes.
;*****************************************************************************
GET_PM_INFO	equ	1
REGISTER_MODULE equ     2
;-----------------------------------------------------------------------------

;*****************************************************************************
; System-Request Opcodes.
;*****************************************************************************
BIND		equ	2
;-----------------------------------------------------------------------------

;*****************************************************************************
; Standard NDIS "Return" Codes.
;*****************************************************************************
SUCCESS 	 equ	0000h
WAIT_FOR_REL     equ    0001h
REQ_QUEUED	 equ	0002h
UNRECOG_FRAME	 equ	0003h
REJECT_FRAME	 equ	0004h
FORWARD_FRAME	 equ	0005h
NO_RESOURCES     equ    0006h
BAD_PARAMETER    equ    0007h
BAD_FUNCTION     equ    0008h
UNSUPPORTED	 equ	0009h
HARD_ERROR	 equ	000Ah
TRAN_ERROR	 equ	000Bh
DEST_ERROR	 equ	000CH
HARD_FAILURE	 equ	0024h
INT_CONFLICT	 equ	0026h
NET_DISCONNECT	 equ	002Ah
WRONG_OS         equ    002Bh
GEN_FAILURE      equ    00FFh
;-----------------------------------------------------------------------------

;*****************************************************************************
; Protocol.ini Keyword parameter Error Codes.
;*****************************************************************************
TOO_MANY_PARAMS  equ	0001h
TYPE_ERROR	 equ	0002h
STRING_TOO_LONG	 equ	0003h
KEY_NOT_FOUND	 equ	0004h
UNEXPECTED_KEY	 equ	0005h
BAD_BRD_CONFIG	 equ	0006h
BAD_IOBASE	 equ	0007h
BAD_INTLINE	 equ	0008h
BAD_RAMADDR	 equ	0009h
BAD_ENET_ADDR	 equ	000Ah
MULTCAST_ADDR	 equ	000Bh
BAD_PNP_SERIAL	 equ	000Ch
BAD_MEDIA_TYPE	 equ	000Dh
BAD_ON_OFF	 equ	000Eh
;-----------------------------------------------------------------------------

;*****************************************************************************
; Number of bytes to be returned on exit from MAC Direct Primitives.
;*****************************************************************************
RQRET	equ	SIZEOF rqparm - 6 ; Return bytes from Request routines.
SYRET	equ	(SIZEOF syparm) - 6 ; Return bytes from System Requests
TXRET	equ	(sizeof txparm) - 6 ; Return bytes from Transmitchain routine.
RXRET	equ	(sizeof tdparm) - 6 ; Return bytes from Transferdata routine.
RRRET	equ	004h		  ; Return byte from Receive Release.
				  ; Function Not Implemented.
PMSIZE	    equ	SIZEOF pm_req_block
KW_ELE_SIZE equ SIZEOF Kw_tbl
BR_ELE_SIZE equ SIZEOF BoardType
INDRET	equ	2
;-----------------------------------------------------------------------------

;*****************************************************************************
; Protocol Indication codes.
;*****************************************************************************
STARTRESET	 equ	3
ENDRESET         equ    5
INTERRUPT        equ    4
PAD              equ    0
ON               equ    0
OFF		 equ	0FFh
;-----------------------------------------------------------------------------

;*****************************************************************************
; OS/2 opcode definitions for device-driver helper functions.
;*****************************************************************************
DH_YIELD	equ	002h	;Yield CPU.
DH_PROCBLOCK	equ	004h	;Block thread.
DH_PHYSTOVIR	equ	015h	;Convert physical address to virtual.
DH_VIRTOPHYS	equ	016h	;Convert virtual address to physical.
DH_PHYSTOUVIR	equ	017h	;Convert physical address to user virtual.
DH_ALLOCPHYS	equ	018h	;Allocate Physical block of memory.
DH_SETIRQ	equ	01Bh	;Set an IRQ interrupt-routine handler.
DH_GETDOSVAR	equ	024h	;Gets Addres of Kernel Variable.
DH_ALLOCGDT	equ	02Dh	;Allocate GDT Selectors.
DH_PHYSTOGDT	equ	02Eh	;Convert physical address to GDT selector.
DH_REALTOPRO	equ	02Fh	;Change from real to protected mode.
DH_PROTOREAL	equ	030h	;Change from protected to real mode.
DH_EOI		equ	031h	;Issue EOI's (for IBM's OS/2 V2.0).
DH_UNPHYSTOVIR	equ	032h	;Mark completion of PhysToVir logic.
DH_TICKCOUNT	equ	033h	;Set timer-routine handler.
DH_FREEGDT	equ	053h	;Free GDT Selector.

DH_VMALLOC	equ	057h	;Allocate physical block of memory.
VMDHA_FIXED	equ	1 shl 1	;Fixed memory.
VMDHA_CONTIG	equ	1 shl 3	;Comtiguous memory.
VMDHA_USEHIGHMEM equ	1 shl 11 ;Above 16MB address memory.

DH_VMFREE	equ	058h	;Free physical block of memory.
DH_VIRTTOLIN	equ	05Bh	;Convert virtual address to linear.
DH_LINTOGDT	equ	05Ch	;Convert linear address to GDT selector.
DH_ALLOCCTXHOOK equ	063h	;Allocate a context hoook.
DH_ARMCTXHOOK	equ	065h	;Arm a context hook.
;-----------------------------------------------------------------------------

;*****************************************************************************
; OS/2 Offset in Global information table if Time/Date.
;*****************************************************************************
SYS_INFOSEG	equ	1
Time_Date	equ	0
;-----------------------------------------------------------------------------

;*****************************************************************************
; "PROTOCOL.INI" Information structure constants.
;*****************************************************************************
STRLENGTH  equ	 16		;Maximum Lenght of protocol.ini parameter str.
KW_INT	   equ	 0		;Keyword is a long integer.
KW_STR	   equ	 1		;Keyword is a text string.
;-----------------------------------------------------------------------------

;*****************************************************************************
; Strategy Request packet status word
;*****************************************************************************
sr_stat		RECORD	strat_err:1,
			reserved1:5,
			busy:1,
			done:1,
			error_Codes:8;
;-----------------------------------------------------------------------------

;*****************************************************************************
; 80x86 Flags register bits
;*****************************************************************************
Cpu_flags	RECORD Overflow:1,
		       Direction:1,
		       Enabled_Ints:1,
		       Trap:1,
		       Sign:1,
		       Zero:1,
		       Flag_Unused3:1,
		       Auxiliary_Carry:1,
		       Flag_Unused2:1,
		       Parity:1,
		       Flag_Unused1:1,
		       Carry:1
;-----------------------------------------------------------------------------

;*****************************************************************************
; Device Driver status flags
;*****************************************************************************
dvr_flags	RECORD Tx_Active:1,
		       Timer_On:1,
		       NotOurInt:1,
		       InIsr:1,
		       InTmrIsr:1,
		       RxTxferdataDone:1,
		       OneRxDone:1,
		       RxNotComplete:1,
		       Reset:1,
		       Int_req:1,
		       Indications:1,
		       Indic_called:1,
		       Indic_comp:1,
		       Init_comp:1,
		       NDIS_Stats_Available:1=1;
;-----------------------------------------------------------------------------

;*****************************************************************************
; Common Characteristics Table Module Function flags
;*****************************************************************************
cctmff		RECORD	reserved2:29=0,
			dynamic_bind:1=FALSE,
			lower_boundry_bind:1=FALSE,
			upper_boundry_bind:1=TRUE;
;-----------------------------------------------------------------------------

;*****************************************************************************
; MAC-service specific characteristics table flags definition.
;*****************************************************************************
mc_mssf3     RECORD	notused:15=0,
			Mac_lookaframesize:1=FALSE;
mc_mssf2     RECORD	Multiple_tranferdatas:1=FALSE,
			Gdt_virt_addr_supported:1=TRUE,
			Srb_supported:1=FALSE,
			Int_request_supported:1=TRUE,
			Open_close_Adp_supported:1=TRUE,
			Resetmac_supported:1=TRUE,
			IBM_sr_supported:1=FALSE,
			Receive_type:1=RXLOOKAHEAD;
mc_mssf1     RECORD	Loopback_supported:1=FALSE,
			InitDiag_supported:1=FALSE,
			Stats_Always_current:1=FALSE,
			Soft_station_address:1=TRUE,
			Promiscuous_supported:1=TRUE,
			Func_grp_addr_supported:1=FALSE,
			Multicast_supported:1=TRUE,
			Broadcast_supported:1=TRUE;
;-----------------------------------------------------------------------------

;*****************************************************************************
; MAC-service specific Statistics table Status flags definition.
;*****************************************************************************
ms_mssf     RECORD	reserved3:26=0,
			diag_in_progress:1=FALSE,
			open_close_status:1=CLOSED,
			bind_status:1=UNBOUND,
			hardware_status:3;
;-----------------------------------------------------------------------------

;*****************************************************************************
; MAC-service specific Statistics table Current packet filter
;*****************************************************************************
ms_cpf		RECORD	reserved4:12,
			source_routed:1,
			promiscuous_mode:1,
			broadcast_mode:1,
			directed:1;
;-----------------------------------------------------------------------------

;*****************************************************************************
; Record to remember items read from protocol.ini and those expected.
; for a particular board type.
;*****************************************************************************
Keywords   RECORD	reserved5:11,
			No32BitIO_Fnd:1,
			Softcrc_Fnd:1,
			Nicdebug_Fnd:1,
			Dvrdebug_Fnd:1,
			Broadcast_Fnd:1,
			Media_Found:1,
			RxEarlySize:1,
			RxBurCnt_Fnd:1,
			TxBurCnt_Fnd:1,
			RxQue_Found:1,
			TxQue_Found:1,
			Bussize_Found:1,
			Pcmcia_Found:1,
			PnPSerial_Fnd:1,
			Netaddr_Found:1,
			Slot_Found:1,
			Int_Found:1,
			Ram_Found:1,
			Io_Found:1,
			Dvrname_Found:1;
;-----------------------------------------------------------------------------

;*****************************************************************************
; PnP Device ID flags definition.
;*****************************************************************************
Dev_Flags  RECORD	Undefined:29,
			Config_locked:1,
			Enabled:1,
			Initailised:1;
;-----------------------------------------------------------------------------

;*****************************************************************************
; PnP Device Memory Range attributes definition.
;*****************************************************************************
Mem_attrib RECORD	Undefined1:9,
			Access_type:1,
			Mem_type:1,
			Memory_width2:2,
			Memory_width1:2,
			Decode_type:1;
;-----------------------------------------------------------------------------

;*****************************************************************************
; PnP Device Interrupt line attributes definition.
;*****************************************************************************
Irq_attrib RECORD	Undefined2:5,
			Shared_Irq:1,
			Level:1,
			Trigger:1;
;-----------------------------------------------------------------------------

;*****************************************************************************
; PnP Device DMA channel attributes definition.
;*****************************************************************************
DMA_attrib RECORD	Undefined3:6,
			C_Type_timming:1,
			Shered_Dma:1,
			Reserved_0:1=0,
			Speed_support:2,
			WordMode_stat:1,
			ByteMode_stat:1,
			bus_mast_stat:1,
			TxF_Preference:2;
;-----------------------------------------------------------------------------

;*****************************************************************************
; PnP Configuration control bits definition.
;*****************************************************************************
Cfg_Ctrl   RECORD	Reset_CSN:1,
			Wait_For_Key:1,
			Reset_Logical:1;
;-----------------------------------------------------------------------------

;*****************************************************************************
; PCMCIA Port attribute bits definition.
;*****************************************************************************
PCMCIA_PORTAttribs RECORD  pa16BitIO:1, 	; Port supports 16 bit access
			   paForceAliasAccess:1,; Check aliased addresses also
			   paFirstShared:1,	; First client on shared port
			   paSharable:1;	; Set if port can be shared
;-----------------------------------------------------------------------------

;*****************************************************************************
; PCMCIA CardRegister bits definition.
;*****************************************************************************
PCMCIA_tCardRegisters RECORD crCopy:1,	 ; Port supports 16 bit access
			     crPin:1,	 ; Check aliased addresses also
			     crStatus:1, ; First client on shared port
			     crOption:1; ; Set if option register valid
;-----------------------------------------------------------------------------

;*****************************************************************************
; PCMCIA CardRegister bits definition.
;*****************************************************************************
PCMCIA_tConfigAttributes RECORD caVpp2ChangeValid:1,
			  caVpp1ChangeValid:1,	; Set if Vpp1 to be changed
			  caVccChangeValid:1,	; Set if Vcc to be changed
			  caIRQChangeValid:1,	; IRQ steering to be changed
			  caEnableIRQSteering:1,; Set to enable IRQ steering
			  caExclusiveUse:1;	; RequestExclusive granted
;-----------------------------------------------------------------------------

;*****************************************************************************
; PCMCIA Interrupt line attribute bits definition.
;*****************************************************************************
PCMCIA_IRQAttribs RECORD  qaPulseIRQAlloc:1,	; Return if pulse mode intpt
			  qaReserved:4,
			  qaFirstShared:1,	; First client on shared IRQ
			  qaForcePulse:1,	; Force pulse mode interrupt
			  qaDynamicShare:1,	; Dynamically shared IRQ
			  qaTimeMultShare:1;	; Set for time multiplexed IRQ
;-----------------------------------------------------------------------------

;*****************************************************************************
; Combined DOS and OS/2 Driver Device Header.
;*****************************************************************************
header	struct
	dd	?		;Link to next device-header block.
Dev_Att dw	?		;Device attributes (defined above).
	dw	?		;Strategy routine offset.
	dw      0               ;Null "IDC" routine offset.
Hdr_Nam db	8 dup (0)	;Default driver name (filled in later).
IFDEF OS2
Hdr_PCS dw	?		;Protect-mode CS selector of strategy entry pt.
Hdr_PDS dw	?		;Protect-mode DS selector.
	dw	?		;Real-mode CS segment of strategy entry pt.
	dw	?		;Real-mode DS segment.
ENDIF
header	ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; "Strategy" Request packet layout.
;*****************************************************************************
SRqPkt	struct
SRqLen  db      ?               ;Length in bytes of packet.
SRqUnit db      ?               ;Subunit number of block device.
SRqOp   db      ?               ;Opcode.
SRqStat	dw	?		;Status word.
	db	4  dup (?)	;(Reserved).
SRqDevL dd	?		;Device multiple-request link (OS/2 only)
	union			;reserved in MS-DOS.
	 struct
			 db	?
	  union
	   struct
	    Mem_end_off	 dw	?	       ;MS-DOS provides
	    Mem_end_seg	 dw	?	       ;Remaining free space.
	   ends
	   struct
	    Dev_hlp_off	 dw	?	       ;OS/2 provides driver device
	    Dev_hlp_seg	 dw	?	       ;helper functions.
	   ends
	   struct
	    xfer_addr_off dw	?	       ;For Read Opcode
	    xfer_addr_seg dw	?	       ;Tranfer address for data.
	    xfer_size	  dw	?	       ;Number of bytes transferred.
	   ends
	  ends
	  Cfg_off	 dw	?
	  Cfg_seg	 dw	?
	 ends
	 struct
	   No_of_units	 db	?		;.
	   union
	    Free_mem_off   dw	?		; MS-DOS Returns
	    Code_end_off   dw	?		; OS/2 Returns
	   ends
	   union
	    Free_mem_seg   dw	?		; MS-DOS Returns
	    Data_end_off   dw	?		; OS/2 Returns
	   ends
	   Bpb_array_off	 dw	?
	   Bpb_array_seg	 dw	?
	 ends
	ends
SRqPkt ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; Table of days in month and varible store for Time_Convert function.
;*****************************************************************************
Year	  struct
January		db	31	 ; January
February	db	28	 ; February
March		db	31	 ; March
April		db	30	 ; April
May		db	31	 ; May
June		db	30	 ; June
July		db	31	 ; July
August		db	31	 ; August
September	db	30	 ; September
October		db	31	 ; October
November	db	30	 ; November
December	db	31	 ; December
Year	       ends
;-----------------------------------------------------------------------------

IFDEF OS2
;*****************************************************************************
; DosGetDateTime output structure layout.
;*****************************************************************************
Date_Time struct
	   dd		?		;Milliseconds
	   dd		?		;(Irrelevent.)
 Hours	   db		?		;Current number of hours into the day.
 Minutes   db		?		;Current number of Minutes.
 Seconds   db		?		;Current number of Seconds.
	   db		?		;
	   dw		?		;(Irrelevent.)
	   dw		?		;(Irrelevent.)
 Day	   db		?		;Current day of the month.
 Month	   db		?		;Current Month of the Year.
 Year	   dw		?		;Current Year.
Date_Time ends
;-----------------------------------------------------------------------------
ENDIF

;*****************************************************************************
; Driver request dispatch table.
;*****************************************************************************
Rq_Disp struct
	dw	?	; 0:  (Illegal).
	dw	?	; 1:  InitiateDiagnostics	(unsupported).
	dw	?	; 2:  ReadErrorLog		(unsupported).
	dw	?	; 3:  SetStationAddress.
	dw	?	; 4:  OpenAdapter.
	dw	?	; 5:  CloseAdapter.
	dw	?	; 6:  ResetMAC.
	dw	?	; 7:  SetPacketFilter.
	dw	?	; 8:  AddMulticastAddress.
	dw	?	; 9:  DeleteMulticastAddress.
	dw	?	;10:  UpdateStatistics		(unsupported).
	dw	?	;11:  ClearStatistics	(partially supported).
	dw	?	;12:  InterruptRequest.
	dw	?	;13:  SetFunctionalAddress	(unsupported).
	dw	?	;14:  SetLookahead.
Rq_Disp ends
MAXREQ	equ	(SIZEOF(Rq_Disp))/2	 ;(Maximum request opcode).
;-----------------------------------------------------------------------------

;*****************************************************************************
; Common characteristics table
;*****************************************************************************
cctable struct
cc_size dw	?		 ;Size of this table.
 c_maj	db	?		 ;Major an Minor NDIS version
 c_min	db	?
	dw	?		 ;reserved
	db	?		 ;Major and Minor MAC version.
	db	?
	cctmff	{}		 ; function flags
cc_name db	OEM_SHORT_NAME
	db	18 - @SizeStr(%OEM_SHORT_NAME) dup (0)
	db	?		 ; upper module type
	db	?		 ; upper module interface
	db	?		 ; lower module type
	db	?		 ; lower module interface
cc_ID	dw	?		 ; module ID (set by protmgr)
cc_ds	dw	?		 ; module DS
cc_sys	dd	0		 ; system dispatch entry
cc_spec dd	0		 ; specific characteristics table
cc_stat dd	0		 ; mac status table
cc_udsp dd	0		 ; Driver "main" dispatch table
cc_ldsp dd	?		 ; no lower dispatch table
	dd	?		 ; reserved
	dd	?		 ; reserved
cctable ends
CCTSIZE equ	sizeof(cctable)
;-----------------------------------------------------------------------------

;*****************************************************************************
; MAC-specific characteristics table
;*****************************************************************************
specific struct
mc_size	  dw	 ?		 ;Size of this table.
	  db	 16 dup (?)
mc_alen   dw	 ?		 ;Length of node address.
mc_pnad   db	 16 dup (?)	 ;Previous node address.
mc_cnad   db	 16 dup (?)	 ;Current node address.
mc_func   dd	 ?		 ;Current 802.5 functional address (unused).
mc_list   dd	 ?		 ;Multicast address list pointer.
mc_speed  dd	 ?		 ;Data-link speed (10MB/sec).
mc_flg1	  mc_mssf1  {}		 ;Service flags.
mc_flg2	  mc_mssf2  {}
mc_flg3	  mc_mssf3  {}
	  dw	 ?		;Maximum I/O packet size.
mc_tottx  dd	 ?		;Total transmit-buffer space.
mc_1buftx dw	 ?		;Individual transmit-buffer size.
mc_rxbf   dd	 ?		;Total receive-buffer space set by init.
	  dw	 ?		;Minimum receive-buffer size.
	  db	 ?
	  db	 ?
	  db	 ?		;IEEE vendor code (National)
	  db	 ?		;Vendor adapter code.
mc_idno	  dd	 ?		;Vendor I.D. message address.
mc_int	  dw	 ?		;IRQ level used.
mc_txdth  dw	 ?		;Transmit-queue depth supported.
	  dw	 ?		;Maximum number of I/O descriptor blocks.
	  dd	 ?		;Reserve??
specific ends
MCSIZE	 equ	SIZEOF (specific)
;-----------------------------------------------------------------------------

;*****************************************************************************
; MAC-specific status table.  Counters set to -1 are not maintained.
;*****************************************************************************
status	struct
ms_size dw	?	       ;Size of this table.
	dd	-1	       ;Date/time of last "Diagnostic" command.
ms_flag ms_mssf	{}	       ;MAC status flags (0 initially).
ms_fltr	ms_cpf	{}	       ;Current packet filter (0 initially).
ms_sptr dd	0	       ;Pointer to media-specific statistics.
ms_date	dd	-1	       ;Date/time of last "Clear statistics" command.
ms_ipkt dd	0	       ;Total frames received (without errors).
ms_icrc dd	0	       ;Receive CRC errors (status, not bad packets).
ms_ibc	dd	-1	       ;Total bytes received (without errors).
ms_miss dd	0	       ;Frames lost due to no memory (missed packets).
ms_mpkt dd	-1	       ;Multicast frames received.
ms_bpkt dd	-1	       ;Broadcast frames received.
	dd	0	       ;Frame error on receive (obsolete).
	dd	-1	       ;Frame exceeding maximum size (obsolete).
	dd	-1	       ;Frame smaller than minimum size (obsolete).
	dd	-1	       ;Multicast bytes received (obsolete).
	dd	-1	       ;Broadcast bytes received (obsolete).
ms_ierr dd      -1             ;Total bad input frames (received with errors).
ms_opkt dd      0              ;Total frames transmitted.
ms_obc  dd      -1             ;Total bytes transmitted.
ms_omul dd      -1             ;Multicast frames sent.
ms_obrd dd      -1             ;Broadcast frames sent.
	dd      -1             ;Broadcast bytes sent (obsolete).
	dd      -1             ;Multicast bytes sent (obsolete).
ms_xto  dd      -1             ;Total transmission timeouts.
ms_xerr dd	-1	       ;Total transmission errors.
status ends
MSSIZE	equ	SIZEOF(status)
;-----------------------------------------------------------------------------

;*****************************************************************************
; Statistics Kept from MAC-specific status table.
;*****************************************************************************
Kept_Stats   struct
 RxPackets    dd	0	  ;Receive statistics.
 TxPackets    dd	0	  ;Receive statistics.
 RxCrcErrors  dd	0	  ;CRC errors.
 RxOverflow   dd	0	  ;Transmit statistics.
 RxFaeErrors  dd	0	  ;Reset FAE errors.
 MissPackets  dd	0	  ;Reset missed packets.
Kept_Stats   ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; MAC-specific status table.  Counters set to -1 are not maintained.
;*****************************************************************************
status2 struct
	dw	?		;Size of this table.
	dw      1               ;802.3 statistics version level.
ms_fae  dd      0               ;Total input frame-alignment errors.
	dd      -1              ;(Obsolete).
ms_orun dd      0               ;Total input overrun errors.
	dd      -1              ;(Obsolete).
	dd      -1              ;Total deferred-transmission frames (N.A.).
ms_o16c dd      0               ;Total output 16-collision errors.
	dd      -1              ;(Obsolete).
	dd      -1              ;Total output late-collision errors (N.A.).
	dd      -1              ;Total output 1-collision frames (N.A.).
	dd      -1              ;Total output multiple-collision frames (N.A.).
	dd      -1              ;Total output CD-test frames (N.A.).
	dd      -1              ;(Obsolete).
ms_crsl dd      0               ;Total output carrier-sense lost errors.
	dd      -1              ;(Obsolete).
ms_urun dd	0		;Total output underrun errors.
status2 ends
MS2SIZE equ	SIZEOF (status2)
;-----------------------------------------------------------------------------

;*****************************************************************************
; Driver dispatch table (NDIS "upper" dispatch table).	All are offset/segment.
;*****************************************************************************
dd_sptbl struct
dd_ctbl   dd	 ?	    ;Pointer to common-characteristics table.
dd_req	  dd	 ?	    ;Driver request entry-point.
dd_tx	  dd	 ?	    ;TransmitChain (output) entry-point.
dd_rx	  dd	 ?	    ;TransferData (input) entry-point.
dd_rrel   dd	 ?	    ;ReceiveRelease entry-point.
dd_ion	  dd	 ?	    ;IndicationOn entry-point.
dd_ioff   dd	 ?	    ;IndicationOff entry-point.
dd_sptbl ends
DDSIZE	equ	SIZEOF(dd_sptbl)
;-----------------------------------------------------------------------------

;*****************************************************************************
; Protocol-Request Dispatch Table.
;*****************************************************************************
pd_sptbl struct
pd_ctbl   dd	 0	  ;Protocol characteristic table pointer.
pd_if	  dd	 0	  ;Interface flags.
pd_reqc   dd	 0	  ;Request-confirmation entry-point.
pd_txc	  dd	 0	  ;Transmit-confimation entry-point.
pd_rxla   dd	 0	  ;Receive-lookahead entry-point (unused by us).
pd_icmp   dd	 0	  ;Indication-complete entry-point.
pd_rxch   dd	 0	  ;Receive-chain entry-point.
pd_stat   dd	 0	  ;Status-indication entry-point.
pd_sptbl ends
PDSIZE	equ	SIZEOF(pd_sptbl)
;-----------------------------------------------------------------------------

;*****************************************************************************
; "PROTOCOL.INI" Parameter-entry layout.
;*****************************************************************************
param struct
 Param_typ	dw	?		;Parameter type.
 Param_len	dw	?		;Parameter length.
 union Parameter
   Number	dw	?		;Numeric Parameter value.
   String	dw	STRLENGTH dup (?)
					;Null terminated String Parameter
 ends					;value
param ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; "PROTOCOL.INI" keyword-entry layout.	The parameters are repeated as needed.
;*****************************************************************************
keyword struct
 kw_nxt	 dd	?		 ;Pointer to next keyword entry.
 kw_prv  dd	?		 ;Pointer to previous keyword entry.
 kw_nam  db	16 dup (?)	 ;ASCII keyword name.
 kw_pno	 dw	?		 ;Number of parameters that follow:
 kw_p1	 param	{}
keyword ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; "PROTOCOL.INI" driver configuration-table entry layout.
;*****************************************************************************
cf_tbl	struct
 cf_nxt dd	 ?		 ;Next-driver pointer.
 cf_prv dd	 ?		 ;Last-driver pointer.
 cf_nam db	 16 dup (?)	 ;Logical driver name (ASCII).
 cf_k1	keyword {}		 ;Offset of 1st keyword for this driver.
cf_tbl  ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; Valid Keyword Element Structure.
;*****************************************************************************
Kw_tbl	struct
 kw_nam    db	 16 dup (?)	 ;Keyword (ASCII).
 Kw_type   dw	 ?		 ;Type expected for that keyword.
 Kw_S_ser  dw	 ?		 ;Offset to keyword Storing routine.
 Kw_V_ser  dw	 ?		 ;Offset to keyword verification routine.
Kw_tbl	ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; BoardType Information Structure.
;*****************************************************************************
BoardType struct
   BoardName	db	STRLENGTH dup (0)
   DeviceID	dd	?
   ExpectedKeys Keywords {}
   OptionalKeys Keywords {}
   IOBases	dw	MAXIOLOCS dup (0)
   IrqLines	db	MAXIOLOCS dup (0)
   Cards_Presnt db	?
   slot 	db	MAXIOLOCS dup (0)
   SerialID	dd	MAXIOLOCS dup (0)
   Cards_IOAddr	dw	MAXIOLOCS dup (0)
   IntLines	db	MAXIOLOCS dup (0)
   memaddrs	dw	MAXIOLOCS dup (0)
BoardType ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; BrdStore Structure used to store information gained from Protocol.ini.
;*****************************************************************************
BrdStore struct
   FoundKeys	   dd		?
   BrdCfgPtr	   dw		?
   Phyaddr_Valid   db		?
   Phyaddr_Used    db		ADDRLEN dup (?)
   Io_Valid	   db		?
   Io_Used	   dw		?
   Ram_Valid	   db		?
   Ram_Used	   dd		?
   Int_Valid	   db		?
   Int_Used	   db		?
   Pcmcia_Valid	   db		?
   Pcmcia_Used	   db		?
   Slot_Valid	   db		?
   Slot_Used	   db		?
   Netaddr_Valid   db		?
   Netaddr_Used    db		ADDRLEN dup (?)
   Serial_Valid    db		?
   Serial_Used	   db		ADDRLEN/2 dup (?)
   TxQueue_Valid   db		?
   TxQueue_Used	   db		?
   RxQueue_Valid   db		?
   RxQueue_Used	   db		?
   TxBurst_Valid   db		?
   TxBurst_Used	   db		?
   RxBurst_Valid   db		?
   RxBurst_Used	   db		?
   RxEarlySz_Valid db		?
   RxEarlySz_Used  db		?
   Media_Valid	   db		?
   Media_Used	   db		?
   SoftCRC_Valid   db		?
   SoftCRC_Used	   db		?
   No32BitIO_Valid db		?
   No32BitIO_Used  db		?
BrdStore ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; Standard stack-frame "header" for all driver requests.
;*****************************************************************************
stdparm struct
	dw      ?               ;Old BP-reg.
	dd      ?               ;Exit address (far).
std_ds  dw      ?               ;Our DS-reg. value.
stdparm ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; System-request frame layout.
;*****************************************************************************
syparm	struct
	stdparm {?}
sy_op	dw	?		;Request opcode.
sy_par3 dw      ?               ;Word parameter  (or 0).
sy_par2 dd      ?               ;Dword parameter (or 0).
sy_par1 dd      ?               ;Dword parameter (or 0).
syparm  ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; Driver-request frame layout.
;*****************************************************************************
rqparm	struct
	stdparm {?}
rq_op   dw      ?               ;Request opcode.
rq_buf  dd      ?               ;Buffer pointer (or 0).
rq_par  dw      ?               ;Word parameter (or 0).
rq_hdl  dw      ?               ;Handle I.D. number.
rq_pmid dw      ?               ;Protocol module I.D. number.
rqparm  ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; Transmit handle Structure.
;*****************************************************************************
Handle_str struct
 handle   dw	 ?		  ;Handle I.D. number.
 ProtID	  dw	 ?		  ;Protocol module I.D. number.
Handle_str ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; TransmitChain (output) frame layout.
;*****************************************************************************
txparm	struct
	stdparm {?}
tx_buf	dd	?	       ;Buffer pointer (virtual).
	Handle_str	<>     ;Handle number and Protocol Module I.D.number.
txparm  ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; Output buffer descriptor layout (for TransmitChain).
;*****************************************************************************
txbufdcr  struct
tx_dcrtyp  db	  ?		 ;Buffer pointer type (0 = phys., 2 = virtual).
	   db	  ?		 ;(Unused).
tx_dcrlen  dw	  ?		 ;I/O data length.
tx_dcrbuf  dd	  ?		 ;Data buffer pointer (physical or virtual).
txbufdcr  ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; Output buffer layout (for TransmitChain).
;*****************************************************************************
txframedcr struct
tx_immlen    dw	  ?		 ;Immediate-data byte count.
tx_immbuf    dd	  ?		 ;Immediate-data buffer pointer (virtual).
tx_bufcnt    dw	  ?		 ;Number of buffer descriptors that follow:
tx_dcr1	     txbufdcr	 {?}	 ;Offset of 1st buffer descriptor.
txframedcr ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; TransferData frame layout
;*****************************************************************************
tdparm	  struct
	   stdparm {?}
td_desc	   dd	  ?		 ;Buffer pointer (virtual).
td_offset  dw	  ?		 ;Starting input frame offset.
td_bytes   dd	  ?		 ;Number of bytes input.
tdparm	  ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; Buffer descriptor layout (for TransferData).
;*****************************************************************************
tdbufdcr  struct
tdb_typ    db	  ?		  ;Reserved word.
tdb_res    db	  ?		  ;Reserved word.
tdb_len    dw	  ?		  ;Length of data block.
tdb_ptr    dd	  ?		  ;Address of data block.
tdbufdcr  ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; Buffer descriptors count (for TransferData).
;*****************************************************************************
tdbufcnt  struct
td_bufcnt    dw	  ?		  ;Number of transfer data blocks.
td_buf	     tdbufdcr{?}
tdbufcnt  ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; Protocol Request Block Structure Definition.
;*****************************************************************************
pm_req_block struct
 pm_op	      dw	 0	  ;Request opcode.
 pm_stat      dw	 0	  ;Ending status.
 pm_ptr1      dd	 0	  ;Pointer 1.
 pm_ptr2      dd	 0	  ;Pointer 2.
 pm_w1	      dw	 0	  ;Word 1.
pm_req_block ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; Hardware Dependent Function Vector table Structure.
;*****************************************************************************
Info_Type  struct
 Data_Start    dw	 ?
 Data_End      dw	 ?
 Func_Start    dw	 ?
 Func_End      dw	 ?
Info_Type   ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; PnP Device ID structure.
;*****************************************************************************
Device_ID	struct
  dBusID	dd	 ?	  ; Bus Type, 0 is undefined
  dDevID	dd	 ?	  ; Physical device ID, 0xFFFFFFFFh if undef.
  dSerialNo	dd	 ?	  ; Device instance serial number,0 if undef.
				  ;  Class code for PCI, 0xFFFFFFFFh if undef.
  dLogID	dd	 ?	  ; Logical device ID for PnP ISA
				  ;  Class code for PCI, 0xFFFFFFFFh if undef.
  dFlags	Dev_Flags {}	  ; Device configuration status.
Device_ID	ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; PnP Bus structure.
;*****************************************************************************
Bus_Access     union
  struct	 PnPAccess
   bCSN 	 db	 ?	  ; Card Slot Number deternimed during Isolate
   bLogiDevNo	 db	 ?	  ; Logical Device Number.
   bReadPortAdr  dw	 ?	  ; Relocatable read port address.
		 ends
  struct	 EISAAccess
   bSlotNo	 db	 ?	  ; Card slot number.
   bFuncNo	 db	 ?	  ; (E)ISA sub-function no, Multi func cards.
   wEISAreserved dw	 ?
		 ends
  struct	 PCIAccess
   bBusNo	 db	 ?	  ; Bus number 0-255.
   bDevFuncNo	 db	 ?	  ; Device (bits 7:3) and func (bits 2:0) IDs.
   wPCIreserved  dw	 ?
		 ends
Bus_Access    ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; PnP Configuration Buffer structure.
;*****************************************************************************
PnP_Cfg_Bufs	struct
 sDevice_ID	Device_ID  {}	  ; ID Information.
 uBusAccess	Bus_Access {{}}	  ; Info to allow config access
 wNumMemWind	dw	   ?	  ;Number of memory windows.
 dMemBase	dd	   MAX_MEM_REGS dup (?)
				  ; Memory Window base.
 dMemLen	dd	   MAX_MEM_REGS dup (?)
				  ; Memory Window Length.
 dMemAttrib	Mem_attrib MAX_MEM_REGS dup ({})
				  ; Memory Window Attributes.
 wNumIOPorts	dw	   ?	  ; Number of IO ports.
 wIOPort_Base	dw	   MAX_IO_PORTS dup (?)
				  ; I/O Port base
 wIOPort_Len	dw	   MAX_IO_PORTS dup (?)
				  ; I/O Port length.
 wNumIRQs	dw	   ?	  ; Number of IRQ info.
 bIRQRegs	db	   MAX_IRQS dup (?)
				  ; IRQ list
 bIRQAttrib	Irq_attrib MAX_IRQS dup ({})
				  ; IRQ Attribute list
 wNumDMAs	dw	   ?	  ; Number of DMA Channels
 bDMALst	db	   MAX_DMA_CHANNELS dup (?)
				  ; DMA list
 bDMAAttrib	DMA_attrib MAX_DMA_CHANNELS dup ({})
				  ; DMA Attribute list
 bReserved1	db	   3 dup  (?)
PnP_Cfg_Bufs   ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; PnP Register structure.
;*****************************************************************************
PnP_Regs	struct
 Set_Rd_Data	db	   ?	  ;Read Port address setting Register,
 Ser_Isolation	db	   ?	  ;Serial Isolation Register.
 Config_Ctrl	db	   ?	  ;Configuration control Register.
 Wake_CSN	db	   ?	  ;
 Resourse_Data	db	   ?	  ;Access port to cards resource data.
 Status 	db	   ?	  ;Card PnP status.
 CSN		db	   ?	  ;Card Sequence Number.
 Logic_Dv_Num	db	   ?	  ;Logical Device Number.
 Reserved1	db	   28h dup (?)
 Activate	db	   ?	  ;Card Activation Register.
 IO_Rng_Check	db	   ?	  ;Card IO range check register.
 Reserved2	db	   0eh dup (?)
 Mem_Base	dw	   ?	  ; Shared Memory base address
 Mem_Ctrl	db	   ?	  ; Shared Memory control register.
 Mem_Upp_Lim	dw	   ?	  ; Shared Memory Upper address limit.
 Reserved3	db	   1Bh dup (?)
 IOBase_lb	db	   ?	  ; IObase Descriptor Low bute.
 IOBase_hb	db	   ?	  ; IObase Descriptor High byte.
 Reserved4	db	   0Eh dup (?)
 Int_Level	db	   ?	  ; Interrupt level.
 Reserved5	db	   85h dup (?)
 PnPMCR 	db	   ?	  ; Mode configuration register.
 Reserved6	db	   0Fh dup (?)
PnP_Regs	ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; PnP Configuration Buffer structure.
;*****************************************************************************
EISA_Cfg_Bufs	struct
BoardID      dw	 ?	; First and Second byte of board ID.
ProductNo    db	 ?	; First and Second hex digit of product number.
	     db	 ?	; Third digit of product number/1-digit revisopn
			; number.
Slot_Info    db	 ?	; ID and Slot information
Misc_ID      db	 ?	; Miscellaneous ID information.
Cfu_Ver_Maj  db	 ?	; Major Configuration Utility revision Level.
Cfu_Ver_Min  db	 ?	; Minor Configuration Utility revision Level.
Fst_Sel      db	 ?	; 1st Selection.
Snd_Sel      db	 ?	; 2nd Selection.
Trd_Sel	     db	 ?	; 3rd Selection.
frt_Sel	     db	 ?	; 4th Selection.
fth_Sel	     db	 ?	; 5th Selection.
Not_Used     db	 21 dup (?)
Func_Info    db	 ?	; Function Information
Type_Str     db	 0Ch dup (?)
Not_Used1    db	 68 dup (?)
Mem_Config   db	 ?
Mem_Size     db	 ?
Mem_Addr_Lsb db	 ?
Mem_Addr_Mid db	 ?
Mem_Addr_Msb db	 ?
Mem_Size_Lsb db	 ?
Mem_Size_Msb db	 ?
Not_Used2    db	 56 dup (?)
Int_Config   db  ?
Reserved     db  ?
Not_Used3    db	 12 dup (?)
DMA_Config   db	 ?
Burst_Txf    db  ?
Not_Used4    db	 6 dup (?)
IO_Range     db  ?
IO_Base      dw  ?
Not_Used5    db	 150 dup (?)
EISA_Cfg_Bufs	ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; PCMCIA GetClientInfo request structure.
;*****************************************************************************
GetClientInfo0	     struct	  ; Function 03h
 maxLen		      dw     ?	  ;Maximum number of bytes allocated
 gc_infoLen	      dw     ?	  ;Number of bytes needed by client
 gc_attributes	      dw     ?	  ;See tClientAttributes
 gc_revision	      dw     ?	  ;BCD value of client revision
 gc_csLevel	      dw     ?	  ;BCD value of CS release
 revDate	      dw     ?	  ; tPackedDate:	Revision date
 nameOff	      dw     ?	  ;Offset of client name string
 nameLen	      dw     ?	  ;Length of client name string (>=1)
 gc_StringOff	      dw     ?	  ;Offset of vendor name string
 gc_StringLen	      dw     ?	  ;Length of vendor name string	(>=1)
 dataBuffer	      db     240 DUP (?)	;IMPLEMENTATION SPECIFIC
GetClientInfo0	     ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; PCMCIA GetConfigInfo request structure.
;*****************************************************************************
GetConfigInfoArg     struct	  ; Function 04h
 gi_socket	      dw     ?	  ;Logical socket number
 gi_attributes	      dw     ?	  ;See tConfigAttributes
 gi_vcc		      db     ?	  ;Vcc setting
 gi_vpp1	      db     ?	  ;Vpp1 setting
 gi_vpp2	      db     ?	  ;Vpp2 setting
 gi_interfaceType     db     ?	  ;See tInterfaceType
 gi_configBase	      dd     ?	  ;Card base address of config registers
 gi_status	      db     ?	  ;Card Status reg. setting (if present)
 gi_pin		      db     ?	  ;Card Pin reg. setting (if present)
 gi_copy	      db     ?	  ;Card Copy reg. setting (if present)
 gi_option	      db     ?	  ;Card Option reg. setting (if present)
 gi_presentRegs	      db     ?	  ;Supported card regs., See tCardRegister
 firstDevType	      db     ?	  ;From Device ID tuple
 funcCode	      db     ?	  ;From Function ID tuple
 sysInitMask	      db     ?	  ;From Function ID tuple
 manufCode	      dw     ?	  ;From Manufacturer ID tuple
 manufInfo	      dw     ?	  ;From Manufacturer ID tuple
 validRegs	      db     ?	  ;Valid card regs., See tCardRegisters
 gi_assignedIRQ	      db     ?	  ;IRQ assigned to Card, See tIRQEnum
 IRQAttributes	      dw     ?	  ;See tIRQAttributes
 gi_port1Base	      dw     ?	  ;Base port address for IO range 1
 gi_port1Size	      db     ?	  ;Number of contiguous ports in range 1
 gi_port1Attribs      db     ?	  ;See tPortAttributes
 gi_port2Base	      dw     ?	  ;Base port address for IO range 2
 gi_port2Size	      db     ?	  ;Number of contiguous ports in range 2
 gi_port2Attribs      db     ?	  ;See tPortAttributes
 gi_IOAddrLines	      db     ?	  ;Number of IO address lines decoded
GetConfigInfoArg     ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; PCMCIA GetFirstTuple/GetNextTuple request structure.
;*****************************************************************************
GetFirstTupleArg     struct	  ; Functions: 07h/0Ah
 gf_socket	      dw     ?	  ;Logical socket number
 gf_attributes	      dw     ?	  ;See tTupleAttributes
 gf_desiredTuple      db     ?	  ;Tuple code to be located (0xFF = all)
 gf_tupleOffset	      db     ?	  ;Index into tuple data - GetTupleData
 gf_flags	      dw     ?	  ;These 3 fields are set and used by
 gf_linkOffset	      dd     ?	  ;CS. The caller should not modify them
 gf_CISOffset	      dd     ?	  ;between calls to GetFirst/NextTuple.
 tupleCode	      db     ?	  ;Tuple code found
 tupleLink	      db     ?	  ;Tuple link field
GetFirstTupleArg     ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; PCMCIA GetCardServicesInfo request structure.
;*****************************************************************************
GetCardServicesInfoArg struct	  ; Function 0Bh
	gs_infoLen	dw   ?	  ;Number of bytes returned by CS
	signature	db   2 DUP (?)	 ;CS installed ([0]=='C' && [1]=='S')
	numSockets	dw   ?	  ;Number of sockets supported by CS
	gs_revision	dw   ?	  ;BCD value of vendor's CS revision
	gs_csLevel	dw   ?	  ;BCD value of CS release
	gs_StringOff	dw   ?	  ;Offset of vendor string
	gs_StringLen	dw   ?	  ;Length of vendor string (>=1)
	vendorString	db   240 DUP (?)   ;IMPLEMENTATION SPECIFIC
GetCardServicesInfoArg ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; PCMCIA GetStatus request structure.
;*****************************************************************************
GetStatusArg	     struct	  ; Function = 0Ch
 gs_socket	      dw     ?	  ;Logical socket number
 cardState	      dw     ?	  ;Current state of Card, See tStateMask
 socketState	      dw     ?	  ;Changes in socket state, See tStateMask
GetStatusArg	     ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; PCMCIA GetTupleData request structure.
;*****************************************************************************
GetTupleDataArg	     struct	  ; Functions:	GetTupleData = 0Dh
 gt_socket	      dw     ?	  ;Logical socket number
 gt_attributes	      dw     ?	  ;See tTupleAttributes
 gt_desiredTuple      db     ?	  ;Tuple code to be located (0xFF = all)
 gt_tupleOffset	      db     ?	  ;Index into tuple data - GetTupleData
 gt_flags	      dw     ?	  ;These 3 fields are set and used by
 gt_linkOffset	      dd     ?	  ;CS. The caller should not modify them
 gt_CISOffset	      dd     ?	  ;between calls to GetFirst/NextTuple.
 tupleDataMax	      dw     ?	  ;Max size alloc'd for tuple data
 tupleDataLen	      dw     ?	  ;Size of tuple returned by GetTupleData
 tupleData	      db     256 DUP (?)	;IMPLEMENTATION SPECIFIC
GetTupleDataArg	     ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; PCMCIA RegisterClient request structure.
;*****************************************************************************
RegisterClientArg    struct	  ; Function 10h
 rg_attributes	      dw     ?	  ;See tClientAttributes
 rg_eventMask	      dw     ?	  ;See tEventMask
 client_data	      dw     ?
 client_area_seg      dw     ?
 client_area_offset   dw     ?
 client_reserved      dw     ?
 version	      dw     ?	  ;CS version client expects
RegisterClientArg    ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; PCMCIA MapMemPage request structure.
;*****************************************************************************
MapMemPageArg	     struct	  ; Function = 14h
 cardOffset	      dd     ?	  ;Card offset to beginning of page
 pageNumber	      db     ?	  ;Page # to be aligned to cardOffset
MapMemPageArg	     ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; PCMCIA MapWindow request structure.
;*****************************************************************************
MapWindowArg	     struct	  ; Function = 16h
 phyAdapter	      db     ?	  ;Physical (SS) adapter number
 phyWindow	      db     ?	  ;Physical (SS) window number
MapWindowArg	     ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; PCMCIA ModifyWindow request structure.
;*****************************************************************************
ModifyWindowArg	     struct	  ; Function = 17h
 mw_attributes	      dw     ?	  ;See tWindowAttributes
 mw_accessSpeed	      db     ?	  ;Window speed, See tAccessSpeed
ModifyWindowArg	     ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; PCMCIA ReleaseIRQ request structure.
;*****************************************************************************
ReleaseIRQArg	     struct	  ; Function 1Ch
 rl_socket	      dw     ?	  ;Logical socket number
 rl_attributes	      dw     ?	  ;See tIRQAttributes
 rl_assignedIRQ	      db     ?	  ;IRQ assigned to Card, See tIRQEnum
ReleaseIRQArg	     ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; PCMCIA ReleaseConfig request structure.
;*****************************************************************************
ReleaseConfigArg     struct	  ; Function 1Eh
	socket	      dw     ?	  ;Logical socket number
ReleaseConfigArg     ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; PCMCIA RequestIO request structure.
;*****************************************************************************
RequestIOArg	     struct	  ; Function 1Fh, also ReleaseIO = 1Bh
 io_socket	      dw     ?	  ;Logical socket number
 io_port1Base	      dw     ?	  ;Base port address for IO range 1
 io_port1Size	      db     ?	  ;Number of contiguous ports in range 1
 io_port1Attribs      db     ?	  ;See tPortAttributes
 io_port2Base	      dw     ?	  ;Base port address for IO range 2
 io_port2Size	      db     ?	  ;Number of contiguous ports in range 2
 io_port2Attribs      db     ?	  ;See tPortAttributes
 io_IOAddrLines	      db     ?	  ;Number of IO address lines decoded
RequestIOArg	     ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; PCMCIA RequestIRQ request structure.
;*****************************************************************************
RequestIRQArg	     struct	  ; Function 20h
 ri_socket	      dw     ?	  ;Logical socket number
 ri_attributes	      dw     ?	  ;See tIRQAttributes
 ri_assignedIRQ	      db     ?	  ;IRQ assigned to Card, See tIRQEnum
 IRQInfo1	      db     ?	  ;First PCMCIA IRQ byte, See tIRQInfo
 IRQInfo2	      dw     ?	  ;Optional IRQ bytes, See tIRQMask
RequestIRQArg	     ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; PCMCIA RequestWindow request structure.
;*****************************************************************************
RequestWindowArg     struct	  ; Function = 21h
 rw_socket	      dw     ?	  ;Logical socket number
 rw_attributes	      dw     ?	  ;See tWindowAttributes
 base		      dd     ?	  ;System base address for window
 rw_size	      dd     ?	  ;Size of window in bytes
 rw_accessSpeed	      db     ?	  ;Window speed, See tAccessSpeed
RequestWindowArg     ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; PCMCIA RequestSocketMask request structure.
;*****************************************************************************
RequestSocketMaskArg struct	  ; Function 22h
 rs_socket	      dw     ?	  ;Logical socket number
 rs_eventMask	      dw     ?	  ;Current event mask, See tEventMask
RequestSocketMaskArg ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; PCMCIA ModifyConfig request structure.
;*****************************************************************************
ModifyConfigArg	     struct	  ; Function 27h
 mc_socket	      dw     ?	  ;Logical socket number
 mc_attributes	      dw     ?	  ;See tConfigAttributes
 mc_vcc		      db     ?	  ;Vcc setting
 mc_vpp1	      db     ?	  ;Vpp1 setting
 mc_vpp2	      db     ?	  ;Vpp2 setting
ModifyConfigArg	     ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; PCMCIA GetEventMask request structure.
;*****************************************************************************
GetEventMaskArg	     struct	  ; Function 2Eh, and Set EventMask = 31h
 ge_attributes	      dw     ?	  ;See tSocketAttributes
 ge_eventMask	      dw     ?	  ;Current/new event mask, See EventMask
 ge_socket	      dw     ?	  ;Logical socket number
GetEventMaskArg	     ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; PCMCIA RequestConfig request structure.
;*****************************************************************************
RequestConfigArg     struct	  ; Function 30h
 rc_socket	      dw     ?	  ;Logical socket number
 rc_attributes	      dw     ?	  ;See tConfigAttributes
 rc_vcc		      db     ?	  ;Vcc setting
 rc_vpp1	      db     ?	  ;Vpp1 setting
 rc_vpp2	      db     ?	  ;Vpp2 setting
 rc_interfaceType     db     ?	  ;See tInterfaceType
 rc_configBase	      dd     ?	  ;Card base address of config registers
 rc_status	      db     ?	  ;Card Status reg. setting (if present)
 rc_pin		      db     ?	  ;Card Pin reg. setting (if present)
 rc_copy	      db     ?	  ;Card Copy reg. setting (if present)
 rc_option	      db     ?	  ;Card Option reg. setting (if present)
 rc_presentRegs	      db     ?	  ;Configured card regs., tCardRegisters
RequestConfigArg     ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; Structure defined for interaction with Virtual DMA Services
;*****************************************************************************
MemDesc	struc
MDSize		dd	?	; size in bytes
MDOffset	dd	?	; offset
MDSegment	dw	?	; segment/selector
MDBufID		dw	?	; 16-bit buffer id
MDPhysAddr	dd	?	; 32-bit physical address
MemDesc	ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; Media_Type keyword valid parameters keyword request structure.
;*****************************************************************************
On_Off_Param	  struct
 On_Off_Code	db	     ?
 On_Off_String	db	     4	dup  (?)
On_Off_Param	  ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; Media_Type keyword valid parameters keyword request structure.
;*****************************************************************************
Media_Param    struct
 Media_Code	dw	     ?
 Media_String	db	     20	dup  (?)
Media_Param    ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; Stack Structure for NSM calls.
;*****************************************************************************
Near_Params struct
 Ret_Addr    dw	    ?
 bp_ptr	     dw	    ?
Near_Params ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; NsmStartTimer function stack paramaters Structure.
;*****************************************************************************
crc32_Params struct
 Near_Params	  <>
 Buffer		  dd	?
 Len		  dw	?
crc32_Params ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; NsmStartTimer function stack paramaters Structure.
;*****************************************************************************
NsmStartTimer_Params struct
 Near_Params	  <>
 Context	  dw	?
 interval	  dw	?
NsmStartTimer_Params ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; NsmStopTimer function stack paramaters Structure.
;*****************************************************************************
NsmStopTimer_Params struct
 Near_Params	  <>
 Context	  dw	?
NsmStopTimer_Params ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; NsmWaitTime_Params function stack paramaters Structure.
;*****************************************************************************
NsmWaitTime_Params struct
 Near_Params	  <>
 Context	  dw	?
 interval	  dw	?
NsmWaitTime_Params ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; NsmLogMessage function stack paramaters Structure.
;*****************************************************************************
NsmStdMessage_Params struct
 Near_Params	  <>
 Context	  dw	?
 Error_Code	  dw	?
NsmStdMessage_Params ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; NSMMalloc function stack paramaters Structure.
;*****************************************************************************
NSMMalloc_Params struct
 Near_Params	  <>
 Context	  dw  ?
 Mem_Size	  dw  ?
NSMMalloc_Params ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; NSMMallocPhys function stack paramaters Structure.
;*****************************************************************************
NSMMallocPhys_Params struct
 Near_Params	  <>
 Context	  dw  ?
 Mem_Size	  dw  ?
 physaddr	  dd  ?
NSMMallocPhys_Params ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; NsmFree function stack paramaters Structure.
;*****************************************************************************
NsmFree_Params struct
 Near_Params	  <>
 Context	  dw  ?
 Mem_Size	  dw  ?
NsmFree_Params ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; NsmFreePhys function stack paramaters Structure.
;*****************************************************************************
NsmFreePhys_Params struct
 Near_Params	  <>
 Context	  dw  ?
 Mem_Size	  dw  ?
NsmFreePhys_Params ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; NsmLogicaltoPhysical function stack paramaters Structure.
;*****************************************************************************
NsmLogicaltoPhysical_Params struct
 Near_Params	  <>
 Context	  dw   ?
 Address	  dd   ?
NsmLogicaltoPhysical_Params ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; NSM_SET32_Params function stack paramaters Structure.
;*****************************************************************************
NSM_SET32_Params struct
 Near_Params	  <>
 Address	  dd	?
 Value		  dd	?
NSM_SET32_Params ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; NsmRxLookahead_Params function stack paramaters Structure.
;*****************************************************************************
NsmRxLookahead_Params struct
 Near_Params	  <>
 Context	  dw	?
 handle		  dw	?
 LookaheadPointer dd	?
 LookaheadLength  dw	?
 pktsz		  dw	?
 status		  dw	?
 PktDesc	  dd	?
NsmRxLookahead_Params ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; HsmRxCopyPkt_Params function stack paramaters Structure.
;*****************************************************************************
HsmRxCopyPkt_Params  struct
 Near_Params	  <>
 Context	  dw	?
 handle		  dw	?
 offset_into_pkt  dw	?
 count		  dw	?
 pktDesc	  dd	?
 status 	  dw	?
HsmRxCopyPkt_Params  ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; NsmRxComplete_Params function stack paramaters Structure.
;*****************************************************************************
NsmRxComplete_Params struct
 Near_Params	  <>
 Context	  dw	?
 handle 	  dw	?
 pktSize	  dw	?
 status 	  dw	?
NsmRxComplete_Params ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; NsmTxComp_Params function stack paramaters Structure.
;*****************************************************************************
NsmTxComp_Params struct
		  Near_Params	  <>
 Context	  dw  ?
 handle		  Handle_str  <>
 status		  dw	?
NsmTxComp_Params ends
;-----------------------------------------------------------------------------

;*****************************************************************************
; NsmLogicalToPhysical function stack paramaters Structure.
;*****************************************************************************
NsmLogicalToPhysical_Params struct
 Near_Params	 <>
 Logical_Address dd   ?
NsmLogicalToPhysical_Params ends
;-----------------------------------------------------------------------------

IFDEF OS2
;*****************************************************************************
; OS/2 API function declarations.
;*****************************************************************************
	extern	DOSCLOSE:FAR16
	extern  DOSDEVIOCTL:FAR16
	extern  DOSOPEN:FAR16
	extern  DOSPUTMESSAGE:FAR16
;-----------------------------------------------------------------------------
ENDIF

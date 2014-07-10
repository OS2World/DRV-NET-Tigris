;*****************************************************************************
;*              National Semiconductor Company Confidential                  *
;*                                                                           *
;*                          National Semiconductor                           *
;*                       NDIS 2.0.1 MAC device driver                        *
;*   Code for National Semiconductor's CDI driver Initialisation  portion.   *
;*                                                                           *
;*	Source Name:	NSMMSGS.ASM					     *
;*      Authors:                                                             *
;*			 Frank DiMambro 				     *
;*                                                                           *
;*   $Log:   /home/crabapple/nsclib/tech/ndis2/nsm/vcs/nsmmsgs.asv  $								     *
;	
;	   Rev 1.2   10/06/95 13:18:28   frd
;	Fixed problem with Panner text.
;*****************************************************************************
INCLUDE cdi.inc
INCLUDE oem.inc
INCLUDE nsm.inc
INCLUDE hsm.inc
INCLUDE NSMdef.inc
INCLUDE NSMSTDIO.inc

.seq
;;
.386p
;;

IFDEF OS2
_DATA	segment word public use16 'DATA'
ELSE
_TEXT	segment dword public use16 'CODE'	;DOS Driver contained in one segment.
		assume  cs:_TEXT,ds:_TEXT
ENDIF
;*****************************************************************************
; Driver Initialisation Variables.
;*****************************************************************************
PUBLIC Banner
Banner	 db	OEM_BANNER,0		; in hsm.h or hsm.inc

PUBLIC TTLMsg
TTLMsg   db     CR,LF,OEM_SHORT_NAME,' 10/100 ',OEM_FAMILY_NAME
	 db	' ',OEM_HSM_NAME
PUBLIC TTLMsgEnd
TTLMsgEnd db ' '

	 db	'Adapters '
PUBLIC OS_Banner
OS_Banner:
IFNDEF OS2
	 db	' DOS '
ELSE
	 db	 ' OS/2 '
ENDIF
	 db	'NDIS '
PUBLIC NDIS_VStr
NDIS_VStr db    '2.0.1'
	 db	' Driver Version:'
PUBLIC Dvr_Ver
Dvr_Ver:
	 db	MAJ_VN+030h,'.',(MIN_VN/10)+030h,(MIN_VN mod 10)+030h
;	 db	' Beta.'
	 db	CR,LF
	 db	OEM_COPYRIGHT
PUBLIC New_line
New_line db	 CR,LF,0
PUBLIC INSTDvr
INSTDvr  db	'***  There are too many driver instances, only '
IF MAXDVRINST GE 10
	 db	(MAXDVRINST/10)+030h,(MAXDVRINST MOD 10)+030h
ELSE
	 db	(MAXDVRINST MOD 10)+030h
ENDIF
	 db	' are allowed.',CR,LF,0
PUBLIC PMEMsg
PMEMsg	 db	'***  Protocol Manager file could not be opened.',CR,LF,0
PUBLIC PMVEMsg
PMVEMsg  db	'***  Incorrect Protocol Manager Version.',CR,LF,0
PUBLIC PIEMsg
PIEMsg	 db	'***  Error getting PROTOCOL.INI information.',CR,LF,0
PUBLIC PICEMsg
PICEMsg  db	'***  Corrupted PROTOCOL.INI information.',CR,LF,0
PUBLIC HiMMsg
HiMMsg	 db	'***  Not enough high memory for this driver',CR,LF,0
PUBLIC PEMsg1
PEMsg1	 db	'***  Syntax error in PROTOCOL.INI - ',CR,LF,0
PUBLIC PEMsgK1
PEMsgK1  db	'***  Keyword ',60h,0
PUBLIC PEMsgK2
PEMsgK2  db	60h,' is not a valid keyword for this driver.',CR,LF,0
PUBLIC PEMsgK3
PEMsgK3  db	60h,' was expected for this driver configuration.',CR,LF,0
PUBLIC PEMsgK4
PEMsgK4  db	60h,' was not expected for this driver configuration.',CR,LF,0
PUBLIC PEMsgK5
PEMsgK5  db	60h,' has too many parameters.',CR,LF,0
PUBLIC PEMsgK6
PEMsgK6  db	60h,' has the wrong parameter type.',CR,LF,0
PUBLIC PEMsgK7
PEMsgK7  db	60h,' has a string parameter which is too long.',CR,LF,0
PUBLIC PEMsgK8
PEMsgK8  db	60h,' specifies an invalid I/O Base Address.',CR,LF,0
PUBLIC PEMsgK9
PEMsgK9  db	60h,' specifies an invalid Interrupt line.',CR,LF,0
PUBLIC PEMsgK10
PEMsgK10 db	60h,' specifies an invalid Memory address.',CR,LF,0
PUBLIC PEMsgK11
PEMsgK11 db	60h,' specifies an invalid Address String.',CR,LF,0
PUBLIC PEMsgK12
PEMsgK12 db	60h,' should specify a Physical not Multi-cast Address.'
	 db     CR,LF,0
PUBLIC PEMsgK13
PEMsgK13 db	60h,' specifies an invalid PnP Serial number String.'
	 db     CR,LF,0
PUBLIC PEMsgK14
PEMsgK14 db	60h,' specifies an invalid Media type String.'
	 db	CR,LF,0
PUBLIC PEMsgK15
PEMsgK15 db	60h,' should specify "On" or "Off".'
	 db     CR,LF,0
PUBLIC PEMsgK16
PEMsgK16 db	' *** TXQUEUE keyword parameter is less than minimum,'
	 db     ' Defaulting to min TxQueue.',CR,LF,0
PUBLIC PEMsgK17
PEMsgK17 db	' *** TXQUEUE keyword parameter exceeds maximum,'
	 db     ' Defaulting to Max TxQueue.',CR,LF,0
PUBLIC PEMsgK18
PEMsgK18 db	' *** RXQUEUE keyword parameter is less than minimum,'
	 db     ' Defaulting to min TxQueue.',CR,LF,0
PUBLIC PEMsgK19
PEMsgK19 db	' *** RXQUEUE keyword parameter exceeds maximum,'
	 db     ' Defaulting to Max TxQueue.',CR,LF,0
PUBLIC PEMsgD1
PEMsgD1  db	'***  Drivername ',60h,0
PUBLIC PEMsgD2
PEMsgD2  db	60h,' Not found.',CR,LF,0
PUBLIC CfgMsg1
CfgMsg1 db	'Adapter for ',0
PUBLIC CfgMsg2
CfgMsg2 db	' driver was found successfully.',CR,LF,0

PUBLIC Std_EMsg
Std_EMsg db	' *** Adapter for ',0

PUBLIC Std_EMsg1
Std_EMsg1 dw	offset Std_EMsg2+2
	  db	' driver not found in the system.',CR,LF,0
PUBLIC Std_EMsg2
Std_EMsg2 dw	offset Std_EMsg3+2
	  db	' driver not assigned necessary resources.',CR,LF,0
PUBLIC Std_EMsg3
Std_EMsg3 dw	offset Std_EMsg4+2
	  db	' driver failed selftest ',CR,LF,0
PUBLIC Std_EMsg4
Std_EMsg4 dw	offset Std_EMsg5+2
	  db	' driver placed in an 8-bit slot. ',CR,LF,0
PUBLIC Std_EMsg5
Std_EMsg5 dw	offset Std_EMsg6+2
	  db	' is an Un-supported prototype. ',CR,LF,0

PUBLIC Std_EMsg6
Std_EMsg6 dw	offset Std_EMsg7+2
	  db	' *** Media selected not supported on physical'
	  db	' layer. ',CR,LF,0
PUBLIC Std_EMsg7
Std_EMsg7 dw	offset Std_EMsg8+2
	  db	' *** Invalid queue size specified. ',CR,LF,0
PUBLIC Std_EMsg8
Std_EMsg8 dw	offset Std_EMsg8+2
	  db	' *** RXBURSTCOUNT is too large; being reduced to its'
	  db	' maximum value. ',CR,LF,0
PUBLIC Std_EMsg9
Std_EMsg9 dw	offset Std_EMsg10+2
	  db	' *** "RXBURSTTIMEOUT is too large; being reduced'
	  db	' to its maximum value. ',CR,LF,0
PUBLIC Std_EMsg10
Std_EMsg10 dw	0
	   db	' *** Interrupt level specified does not match hardware'
	   db	' settings. ',CR,LF,0

PUBLIC CfgWMsg1
CfgWMsg1   db	'Warning there is '
PUBLIC CfgWTotal
CfgWTotal  db	'0'
	   db   ' Inactive ',0
PUBLIC CfgWMsg2
CfgWMsg2   db	' adapter',0
PUBLIC CfgWGt1
CfgWGt1    db	's',0
PUBLIC CfgWMsg3
CfgWMsg3   db	' in the system.',CR,LF,0
PUBLIC GFEMsg
GFEMsg	 db	'Initialization failure.',CR,LF,0
PUBLIC PCISys
PCISys	 db	0,'PC uses the PCI Bus system.',CR,LF,0
PUBLIC EISASys
EISASys  db	0,'PC uses an EISA bus system.',CR,LF,0
PUBLIC MCASys
MCASys	 db	0,'PC uses an MCA system.',CR,LF,0
PUBLIC ISASys
ISASys	 db	0,'PC uses an ISA system.',CR,LF,0
PUBLIC PnPSys
PnPSys	 db	0,'PC uses a Plug-n-Play system.',CR,LF,0
PUBLIC SysBad
SysBad	 db	'System Configuration Information not complete,'
	 db     ' re-run system configuration.',CR,LF,0
PUBLIC EisaDv
EisaDv	 db	' Network Adapter also uses EISA',CR,LF,0
PUBLIC NoEisaDv
NoEisaDv db	' Network Adapter Not Configured for EISA, ',
		'Defaulting to Plug-n-Play ISA.',CR,LF,0
PUBLIC Find_Err
Find_Err db	' *** Multiple adapter configuration error.',CR,LF,0
PUBLIC Find_Err1
Find_Err1 db	' *** Configuration manager unable to find adapter.',CR,LF,0
PUBLIC TGMsg
TGMsg	 db	'Hello dudes, One day this will give a sensible message.',CR,LF,0
;-----------------------------------------------------------------------------
IFDEF OS2
_DATA	  ends
ELSE
_TEXT	  ends
ENDIF
END

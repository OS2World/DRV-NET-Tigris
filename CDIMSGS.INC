
option expr32
option casemap:none

; Begin of file C:\quoc\proj\euphyter\sw\ndis2\cdimsgs.h
;  
; **      tech/include/cdimsgs.h 
; ** 
; **	Definitions of cdi standard messages
; **
; **  $Archive:   /tmp_mnt/n/crabapple/nsclib/tech/include/vcs/cdimsgs.h_v  $
; **
; **  $Author:   rdunlap  $
; **  $Revision:   1.2  $
; **
; **  $Log:   /tmp_mnt/n/crabapple/nsclib/tech/include/vcs/cdimsgs.h_v  $
; **	
; **	   Rev 1.2   05/07/96 11:32:36   rdunlap
; **	add PVCS keywords for archive, author, revision, & log;
; 
;     1 : /* 
;     2 : **      tech/include/cdimsgs.h 
;     3 : ** 
;     4 : **	Definitions of cdi standard messages
;     5 : **
;     6 : **  $Archive:   /tmp_mnt/n/crabapple/nsclib/tech/include/vcs/cdimsgs.h_v  $
;     7 : **
;     8 : **  $Author:   rdunlap  $
;     9 : **  $Revision:   1.2  $
;    10 : **
;    11 : **  $Log:   /tmp_mnt/n/crabapple/nsclib/tech/include/vcs/cdimsgs.h_v  $
;    12 : **	
;    13 : **	   Rev 1.2   05/07/96 11:32:36   rdunlap
;    14 : **	add PVCS keywords for archive, author, revision, & log;
;    15 : */
;    16 : #ifndef	_cdimsgs_h_
;    17 : #define _cdimsgs_h_ 1
_cdimsgs_h_		EQU		1t
;  HSM standard message numbers, passed to NsmStdMessage()  
;    18 : 
;    19 : /* HSM standard message numbers, passed to NsmStdMessage()  */
;    20 : #define	STDMSG_NO_CARD		0
STDMSG_NO_CARD		EQU		0t
;    21 : #define STDMSG_NO_RESOURCES	1
STDMSG_NO_RESOURCES		EQU		1t
;    22 : #define STDMSG_BIST_FAIL	2
STDMSG_BIST_FAIL		EQU		2t
;    23 : #define	STDMSG_SLOT_SIZE	3
STDMSG_SLOT_SIZE		EQU		3t
;    24 : #define	STDMSG_HW_REVISION	4
STDMSG_HW_REVISION		EQU		4t
;    25 : #define STDMSG_INV_MEDIA	5
STDMSG_INV_MEDIA		EQU		5t
;    26 : #define	STDMSG_INV_QSIZE	6
STDMSG_INV_QSIZE		EQU		6t
;    27 : #define STDMSG_NO_QUEUE		7
STDMSG_NO_QUEUE		EQU		7t
;    28 : #define STDMSG_MAX_BURSTCOUNT	8
STDMSG_MAX_BURSTCOUNT		EQU		8t
;    29 : #define	STDMSG_MAX_BURSTTIMEOUT	9
STDMSG_MAX_BURSTTIMEOUT		EQU		9t
;    30 : #define	STDMSG_IRQ_MISMATCH	10
STDMSG_IRQ_MISMATCH		EQU		10t
;    31 : #define NUM_STDMSGS		11
NUM_STDMSGS		EQU		11t
;  HSM standard messages strings 
;    32 : 
;    33 : /* HSM standard messages strings */
;    34 : #define MSG_NO_CARD \
;    35 :         "No adapter of appropriate type located.\r\n"
MSG_NO_CARD		TEXTEQU		<"No adapter of appropriate type located.\r\n">
;    36 : #define MSG_NO_RESOURCES \
;    37 :         "Adapter not assigned necessary resources.\r\n"
MSG_NO_RESOURCES		TEXTEQU		<"Adapter not assigned necessary resources.\r\n">
;    38 : #define MSG_BIST_FAIL \
;    39 :         "Adapter failed self test.\r\n"
MSG_BIST_FAIL		TEXTEQU		<"Adapter failed self test.\r\n">
;    40 : #define MSG_SLOT_SIZE \
;    41 :         "Adapter placed in an 8-bit slot.\r\n"
MSG_SLOT_SIZE		TEXTEQU		<"Adapter placed in an 8-bit slot.\r\n">
;    42 : #define MSG_HW_REVISION \
;    43 : 	"Prototype Adapter Not Supported\r\n"
MSG_HW_REVISION		TEXTEQU		<"Prototype Adapter Not Supported\r\n">
;    44 : #define MSG_INV_MEDIA \
;    45 :         "Media selected not supported on physical layer.\r\n"
MSG_INV_MEDIA		TEXTEQU		<"Media selected not supported on physical layer.\r\n">
;    46 : #define MSG_INV_QSIZE \
;    47 :         "Invalid queue size specified.\r\n"
MSG_INV_QSIZE		TEXTEQU		<"Invalid queue size specified.\r\n">
;    48 : #define MSG_NO_QUEUE \
;    49 :         "Insufficient memory to allocate transmit queue.\r\n"
MSG_NO_QUEUE		TEXTEQU		<"Insufficient memory to allocate transmit queue.\r\n">
;    50 : #define MSG_MAX_BURSTCOUNT \
;    51 :         "RXBURSTCOUNT is too large; being reduced to its maximum value.\r\n"
MSG_MAX_BURSTCOUNT		TEXTEQU		<"RXBURSTCOUNT is too large; being reduced to its maximum value.\r\n">
;    52 : #define MSG_MAX_BURSTTIMEOUT \
;    53 :         "RXBURSTTIMEOUT is too large; being reduced to its maximum value.\r\n"
MSG_MAX_BURSTTIMEOUT		TEXTEQU		<"RXBURSTTIMEOUT is too large; being reduced to its maximum value.\r\n">
;    54 : #define MSG_IRQ_MISMATCH \
;    55 :         "Interrupt level specified does not match hardware settings.\r\n"
MSG_IRQ_MISMATCH		TEXTEQU		<"Interrupt level specified does not match hardware settings.\r\n">
;    56 : 
;    57 : #endif
; End of file C:\quoc\proj\euphyter\sw\ndis2\cdimsgs.h

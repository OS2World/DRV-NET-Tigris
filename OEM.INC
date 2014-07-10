;  
;	oem.inc - OEM specific values and strings
; 	
; 	$Archive:   /nsclib/tech/include/vcs/oem.inv  $
; 	$Author:   croussel  $
; 	$Revision:   1.4  $
; 
; 	$Log:   /nsclib/tech/include/vcs/oem.inv  $
;	
;	   Rev 1.4   10/06/95 09:18:54   croussel
;	change IBM's OEM_FAMILY_NAME from "EtherStreamer" to "100/10"
;	
;	   Rev 1.3   11 Sep 1995 11:41:54   croussel
;	
;	swap bytes in IBM's OEM_PNP_VENDOR to match the byte order
;	required by the HSMs.
;	
;	   Rev 1.2   08/28/95 09:16:14   croussel
;	IBM Copyright message has a comma between phrases, not a period
;	
;	   Rev 1.1   25 Aug 1995 10:33:22   croussel
;	
;	added NSC_COPYRIGHT, NSC_PNP_VENDOR, OEM_IS_NSC
;	added IBM's OEM_PNP_VENDOR, modified IBM's OEM_COPYRIGHT
;	
;	   Rev 1.0   15 Aug 1995 16:43:02   croussel
;	Initial revision.
; 	
;	

NSC_COPYRIGHT   equ     "(c) Copyright 1999 National Semiconductor. All Rights Reserved."
NSC_PNP_VENDOR	equ	633Ah

ifndef	INTEL
ifndef	IBM
OEM_NAME	equ	"National Semiconductor"
OEM_FAMILY_NAME equ     "MacPhyter"
OEM_COPYRIGHT	equ	NSC_COPYRIGHT
OEM_IS_NSC	equ	1
endif
endif

ifdef	INTEL
OEM_NAME	equ	"Intel Corporation"
OEM_FAMILY_NAME equ	"EtherExpress(TM)"
OEM_COPYRIGHT	equ	"Copyright 1995, Intel Corporation.  All Rights Reserved."
endif

ifdef	IBM
OEM_NAME	equ	"IBM"
OEM_FAMILY_NAME equ	"100/10"
OEM_COPYRIGHT	equ	"Copyright (c) 1995 IBM Corporation, All Rights Reserved."
OEM_PNP_VENDOR	equ	4D24h
endif


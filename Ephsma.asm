; ephsma.asm - assembler parts of ephsm.c

include	ephsma.inc

extern	_NsmEnterCriticalSection : near16
extern	_NsmLeaveCriticalSection : near16
extern	_NsmTransmitComplete : near16
extern	_NsmRxLookahead : near16
extern	_NsmRxComplete : near16
extern	_NsmReceiveChain: near16
extern	_HsmUpdateStatistics : near16
extern	_epRxInit : near16
;extern	_epComputeHashTableIndex : near16

.386

_DATA	segment	word public use16 'DATA'
_DATA	ends

_TEXT	segment	word public use16 'CODE'
	assume	cs:_TEXT, ds:_DATA

IFDEF  NECWARP
public	_HsmGetTxDescList
_HsmGetTxDescList	proc	near	; call in TxData/Nsmres.asm

hsm		equ	bp+4
nfrags		equ	bp+6

txpd		equ	bp-4
ndesc		equ	bp-6

	enter	6,0
	push	si
	mov	bx,word ptr [hsm]
	mov	ax,word ptr [nfrags]
IF HSM_MAX_TX_FRAGS EQ 4
	add	ax,3
	shr	ax,2
ELSE
	mov	cx,HSM_MAX_TX_FRAGS
	add	ax,HSM_MAX_TX_FRAGS -1
	xor	dx,dx
	div	cx
ENDIF
	xor	cx,cx
	mov	word ptr [ndesc],ax
	mov	word ptr [txpd],cx
	mov	word ptr [txpd+2],cx

	push	HSM_TX_CRITICAL_SECTION
	push	bx
	call	_NsmEnterCriticalSection

	pop	bx
	mov	cx,word ptr [ndesc]
	mov	al,byte ptr [bx].HsmContext.txQFree
	sub	al,cl
	push	bx
	jc	short loc_HGDL_3
	mov	byte ptr [bx].HsmContext.txQFree,al

	les	si,dword ptr [bx].EuphPhyterHsmContext.txDescHead
	xor	eax,eax
	mov	[txpd],si
	mov	[txpd+2],es
	dec	cx
	jz	short loc_HGDL_2
loc_HGDL_1:
	dec	cx
	mov	es:[si].HsmPktDesc.descByteCnt,eax
	les	si,dword ptr es:[si].HsmPktDesc.lLink
	jnz	short loc_HGDL_1
loc_HGDL_2:
	mov	edx,dword ptr es:[si].HsmPktDesc.lLink
	mov	dword ptr es:[si].HsmPktDesc.lLink,eax
	mov	dword ptr es:[si].HsmPktDesc.pLink,eax
	mov	dword ptr es:[si].HsmPktDesc.descByteCnt,eax
	mov	dword ptr [bx].EuphPhyterHsmContext.txDescHead,edx
loc_HGDL_3:
;	push	HSM_TX_CRITICAL_SECTION
;	push	bx
	call	_NsmLeaveCriticalSection
	add	sp,2*2

	mov	ax,word ptr [txpd]
	mov	dx,word ptr [txpd+2]
	pop	si
	leave
	retn
_HsmGetTxDescList	endp

align	2

IF NSM_DOUBLE_BUFFER EQ 0
public	_HsmTransmit
_HsmTransmit	proc	near	; call from TxData/Nsmres.asm
	enter	8,0
	push	si
	push	di
	push	fs
	
	mov	bx,[hsm]
	push	HSM_TX_CRITICAL_SECTION
	push	bx
	call	_NsmEnterCriticalSection
	pop	bx

;    txdFirst = ep->txFreeHead;

	mov	cx,[bp+6]
	mov	si,[bp+8]
	mov	ax,word ptr [bx].EuphPhyterHsmContext.txFreeHead
	mov	dx,word ptr [bx+2].EuphPhyterHsmContext.txFreeHead
	mov	[bp-8],cx
	mov	[bp-6],si
	mov	[bp-4],ax
	mov	[bp-2],dx
	push	bx


;    do {
;       pktSize = (uint16) txpd->descByteCnt;
;       frag = (pHsmFrag ) &txpd->frags[0];

loc_HT1:
	les	si,dword ptr [bp-8]
	mov	cx,word ptr es:[si].HsmPktDesc.descByteCnt
	lea	bx,[si].HsmPktDesc.frags
	jcxz	short loc_HT4

;       do (pktSize) {
;//       for (n=0; ((n < HSM_MAX_TX_FRAGS) && (pktSize)); n++ ) {
;          if (cmdsts = frag->cnt & DSIZE) {
;  
;          }
;       }

loc_HT2:
	mov	dx,word ptr es:[bx].HsmFrag.cnt
	and	dx,DSIZE
	jz	short loc_HT3
;            txd = ep->txFreeHead;
;            ep->txFreeHead = (pEuphPhyterDesc) txd->lLink;
	lfs	di,[bp-4]
	mov	eax,fs:[di].EuphPhyterDesc.lLink
	mov	[bp-4],eax
;             txd->handle = (uint32) txpdSave;
	mov	eax,[bp+6]
	mov	fs:[di].EuphPhyterDesc.handle,eax
;             txd->bufPhys = frag->fptr;
	mov	eax,es:[bx].HsmFrag.fptr
	mov	fs:[di].EuphPhyterDesc.bufPhys,eax
;             txd->cmdsts = cmdsts | OWN | MORE;
	mov	word ptr fs:[di].EuphPhyterDesc.cmdsts,dx
	mov	word ptr fs:[di+2].EuphPhyterDesc.cmdsts,highword(OWN or MORE)
loc_HT3:
;             ++frag;
	add	bx,sizeof(HsmFrag)
;             pktSize -= (uint16) cmdsts;
	sub	cx,dx
	ja	short loc_HT2

;    } while ( txpd = (pHsmPktDesc) txpd->lLink );
loc_HT4:
	mov	eax,es:[si].HsmPktDesc.lLink
	mov	[bp-8],eax
	test	eax,eax
	jnz	short loc_HT1


;    txd->cmdsts &= ~MORE;
;    txd->lLink = 0;             /* last one will terminate list */
;    txd->pLink = 0;

	pop	bx
	mov	dword ptr fs:[di].EuphPhyterDesc.lLink,eax
	mov	dword ptr fs:[di].EuphPhyterDesc.pLink,eax
	mov	word ptr fs:[di+2].EuphPhyterDesc.cmdsts,highword(OWN)
	mov	ecx,[bp-4]
	mov	edx,dword ptr [bx].EuphPhyterHsmContext.txFreeHead
	mov	dword ptr [bx].EuphPhyterHsmContext.txFreeHead,ecx
	mov	[bp-4],edx
	push	bx


;    /* add descriptor list to the transmit list */
;    if( !ep->txHead ) {         /* is list empty? */

	cmp	eax,dword ptr [bx].EuphPhyterHsmContext.txHead
	jnz	short loc_HT5

;        ep->txHead = txdFirst;  /* yes, start a new one */
;        ep->txTail = txd;                   /* it's now the last */
;        IOW32( txdp, txdFirst->physAddr );
;        IOW32( cr, TxENA );             /* fire up transmit */

	mov	dword ptr [bx].EuphPhyterHsmContext.txHead,edx
	mov	word ptr [bx].EuphPhyterHsmContext.txTail,di
	mov	word ptr [bx+2].EuphPhyterHsmContext.txTail,fs
	lfs	di,[bp-4]
	mov	dx,[bx].HsmContext.IOaddr
	mov	eax,dword ptr fs:[di].EuphPhyterDesc.physAddr
	add	dx,txdp
	out	dx,eax
	add	dx,(cr - txdp)
	mov	eax,TxENA
	out	dx,eax
	jmp	short loc_HT6


;    } else {
;        NSM_SET32( ep->txTail->pLink, txdFirst->physAddr );
;        ep->txTail->lLink = (uint32) txdFirst;
;        ep->txTail = txd;                   /* it's now the last */
;        IOW32( cr, TxENA );             /* fire up transmit */

loc_HT5:
	les	si,dword ptr [bx].EuphPhyterHsmContext.txTail
	mov	word ptr [bx].EuphPhyterHsmContext.txTail,di
	mov	word ptr [bx+2].EuphPhyterHsmContext.txTail,fs
	lfs	di,[bp-4]
	mov	es:[si].EuphPhyterDesc.lLink,edx
	mov	eax,fs:[di].EuphPhyterDesc.physAddr
	mov	es:[si].EuphPhyterDesc.pLink,eax
	mov	dx,[bx].HsmContext.IOaddr
IF NOT cr EQ 0
	add	dx,cr
ENDIF
	mov	eax,TxENA
	out	dx,eax

;    }
;
;    NsmLeaveCriticalSection( hsm, HSM_TX_CRITICAL_SECTION );
;    return( HsmOK );

loc_HT6:
	call	_NsmLeaveCriticalSection
	add	sp,2*2
	mov	ax,HsmOK
	pop	fs
	pop	di
	pop	si
	leave
	retn
_HsmTransmit	endp
ENDIF

align	2

public	_HsmRxFreePkt
_HsmRxFreePkt	proc	near
; hsm	equ	bp+4
HRFP_handle	equ	bp+6

	push	bp
	mov	bp,sp
	push	si
	push	di
	push	HSM_RX_CRITICAL_SECTION
	push	[hsm]
	call	_NsmEnterCriticalSection
	pop	bx

	mov	ax,[HRFP_handle]
	xor	edx,edx
	cmp	edx,[bx].EuphPhyterHsmContext.rxBusyHead
	push	bx
	jz	near ptr loc_HRFP_X
	les	si,[bx].EuphPhyterHsmContext.rxBusyHead
loc_HRFP_1:
	cmp	ax,word ptr es:[si].EuphPhyterDesc.handle
	jz	short loc_HRFP_2
	cmp	edx,es:[si].EuphPhyterDesc.lLink
	jz	near ptr loc_HRFP_X
	mov	di,si
	mov	cx,es
	les	si,es:[si].EuphPhyterDesc.lLink
	jmp	short loc_HRFP_1
loc_HRFP_2:
	mov	eax,es:[si].EuphPhyterDesc.lLink
	mov	bp,es
	cmp	si,word ptr [bx].EuphPhyterHsmContext.rxBusyHead
	jnz	short loc_HRFP_3
	cmp	bp,word ptr [bx].EuphPhyterHsmContext.rxBusyHead[2]
	jnz	short loc_HRFP_3
	mov	[bx].EuphPhyterHsmContext.rxBusyHead,eax
	jmp	short loc_HRFP_4
loc_HRFP_3:
	cmp	si,word ptr [bx].EuphPhyterHsmContext.rxBusyTail
	mov	es,cx
	mov	es:[di].EuphPhyterDesc.lLink,eax
	mov	es,bp
	jnz	short loc_HRFP_4
	cmp	bp,word ptr [bx].EuphPhyterHsmContext.rxBusyTail[2]
	jnz	short loc_HRFP_4
	mov	word ptr [bx].EuphPhyterHsmContext.rxBusyTail,di
	mov	word ptr [bx].EuphPhyterHsmContext.rxBusyTail[2],cx

loc_HRFP_4:
	mov	es:[si].EuphPhyterDesc.lLink,edx
	mov	es:[si].EuphPhyterDesc.pLink,edx
	mov	es:[si].EuphPhyterDesc.cmdsts,RX_MAX_PACKET_SIZE or INCCRC

	cmp	edx,[bx].EuphPhyterHsmContext.rxHead
	mov	eax,es:[si].EuphPhyterDesc.physAddr
	jnz	short loc_HRFP_5
	mov	dx,[bx].HsmContext.IOaddr
	mov	word ptr [bx].EuphPhyterHsmContext.rxHead,si
	mov	word ptr [bx].EuphPhyterHsmContext.rxHead[2],bp
	add	dx,rxdp
	out	dx,eax
	jmp	short loc_HRFP_6
loc_HRFP_5:
	les	di,[bx].EuphPhyterHsmContext.rxTail
	mov	word ptr es:[di].EuphPhyterDesc.lLink,si
	mov	word ptr es:[di].EuphPhyterDesc.lLink[2],bp
	mov	es:[di].EuphPhyterDesc.pLink,eax
loc_HRFP_6:
	mov	dx,[bx].HsmContext.IOaddr
	mov	word ptr [bx].EuphPhyterHsmContext.rxTail,si
	mov	word ptr [bx].EuphPhyterHsmContext.rxTail[2],bp
IF NOT cr EQ 0
	add	dx,cr
ENDIF
	mov	eax,RxENA
	out	dx,eax
loc_HRFP_X:
	call	_NsmLeaveCriticalSection
	add	sp,4
	mov	ax,HsmOK
	pop	di
	pop	si
	pop	bp
	retn
_HsmRxFreePkt	endp

align	2

public	_HsmServiceTransmit
_HsmServiceTransmit	proc	near	; call from HsmService/ephsm.c
; hsm	equ	bp+4
HST_txd		equ	bp-4
HST_txdl	equ	bp-8
HST_txHandle	equ	bp-12
HST_cmdsts	equ	bp-14

	enter 14,0


;    if ( ( txd = ep->txHead ) && !( ( cmdsts = txd->cmdsts ) & OWN ) ) {
;        traffic = TRUE;

	mov	bx,[hsm]
	mov	eax,[bx].EuphPhyterHsmContext.txHead
	test	eax,eax
	jz	short loc_HST_false1


;        txdLast = txd;
;        while ( cmdsts & MORE ) {   /* find last fragment */
;           if ( !( txdLast = (pEuphPhyterDesc) txdLast->lLink ) || 
;                (( cmdsts = txdLast->cmdsts ) & OWN ) ) {
;              traffic = FALSE;
;              break;
;           }
;        }
	push	si
	push	di
	mov	[HST_txd],eax
	mov	[HST_txdl],eax
loc_HST_2:
	les	si,[HST_txdl]
	mov	ax,word ptr es:[si].EuphPhyterDesc.cmdsts[2]
	test	ax,highword(OWN)
	jnz	short loc_HST_false2	; in action
	test	ax,highword(MORE)
	jz	short loc_HST_3
	mov	eax,es:[si].EuphPhyterDesc.lLink
	test	eax,eax
	mov	[HST_txdl],eax
	jnz	short loc_HST_2
loc_HST_false2:
	pop	di
	pop	si
loc_HST_false1:
IF FALSE EQ 0
	xor	ax,ax
ELSE
	mov	ax,FALSE
ENDIF
	leave
	ret
loc_HST_3:
	mov	[HST_cmdsts],ax


;        if ( traffic ) {
;           NsmEnterCriticalSection( hsm, HSM_TX_CRITICAL_SECTION );

	push	fs
	push	HSM_TX_CRITICAL_SECTION
	push	bx
	call	_NsmEnterCriticalSection

;           txpd = (pHsmPktDesc) txdLast->handle;
;           txHandle = txpd->handle;
;//           do {
;              if ( !ep->txDescHead ) {
;                 ep->txDescHead = txpd;
;              } else {
;                 ep->txDescTail->lLink = ( uint32 ) txpd;
;              }
;                 ep->txDescTail = txdpl;
;//              ep->txDescTail = txpd;
;//              txpd = ( pHsmPktDesc ) txpd->lLink;
;//              ++hsm->txQFree;
;              hsm->txQFree += txpdcnt;
;//           } while ( txpd );


	pop	bx
	les	si,[HST_txdl]
	mov	edx,es:[si].EuphPhyterDesc.handle
	les	si,es:[si].EuphPhyterDesc.handle
	mov	eax,es:[si].HsmPktDesc.handle
	mov	[HST_txHandle],eax
	xor	eax,eax
	sub	cx,cx
loc_HST_4:
	inc	cx
	cmp	eax,es:[si].HsmPktDesc.lLink
	jz	short loc_HST_5
	les	si,es:[si].HsmPktDesc.lLink
	jnz	short loc_HST_4
loc_HST_5:
	cmp	eax,[bx].EuphPhyterHsmContext.txDescHead
	jz	short loc_HST_6
	lfs	di,[bx].EuphPhyterHsmContext.txDescTail
	mov	fs:[di].HsmPktDesc.lLink,edx
	jmp	short loc_HST_7
loc_HST_6:
	mov	[bx].EuphPhyterHsmContext.txDescHead,edx
loc_HST_7:
	mov	word ptr [bx].EuphPhyterHsmContext.txDescTail,si
	mov	word ptr [bx].EuphPhyterHsmContext.txDescTail[2],es
	add	[bx].HsmContext.txQFree,cl


;          ep->txHead = (pEuphPhyterDesc) txdLast->lLink;
;          txdLast->lLink = 0;

	les	si,[HST_txdl]
	mov	edx,es:[si].EuphPhyterDesc.lLink
	mov	es:[si].EuphPhyterDesc.lLink,eax
	mov	[bx].EuphPhyterHsmContext.txHead,edx


;           if ( !ep->txFreeHead) {
;             ep->txFreeHead = txd;
;           } else {
;              ep->txFreeTail->lLink = ( uint32 ) txd;
;              ep->txFreeTail->pLink = txd->physAddr;
;           }
;           ep->txFreeTail = (pEuphPhyterDesc) txdLast;


	mov	ecx,[HST_txdl]
	cmp	eax,[bx].EuphPhyterHsmContext.txFreeHead
	jz	short loc_HST_8
	lfs	di,[HST_txd]
	les	si,[bx].EuphPhyterHsmContext.txFreeTail
	mov	eax,fs:[di].EuphPhyterDesc.physAddr
	mov	word ptr es:[si].EuphPhyterDesc.lLink,di
	mov	word ptr es:[si].EuphPhyterDesc.lLink[2],fs
	mov	es:[si].EuphPhyterDesc.pLink,eax
	jmp	short loc_HST_9
loc_HST_8:
	mov	eax,[HST_txd]
	mov	[bx].EuphPhyterHsmContext.txFreeHead,eax
loc_HST_9:
	mov	[bx].EuphPhyterHsmContext.txFreeTail,ecx


;           NsmLeaveCriticalSection( hsm, HSM_TX_CRITICAL_SECTION );
;           txsts = ( uint16 ) ((cmdsts >> 16) & 0x7ff);
;           NsmTransmitComplete( hsm, txHandle, (int) txsts );
;           txFirst = TRUE;

	push	bx
	call	_NsmLeaveCriticalSection
	mov	ax,[HST_cmdsts]
	pop	bx
	pop	cx		; stack adjust
	pop	fs
;	and	ax,7ffh
	and	ax,7f0h		; remove collision count
	mov	dx,[HST_txHandle]
	mov	cx,[HST_txHandle][2]
	push	ax
	push	cx
	push	dx
	push	bx
	call	_NsmTransmitComplete
	add	sp,4*2
	mov	ax,TRUE
	pop	di
	pop	si
	leave
	ret

;        }
;  #endif
;   } /* end while */
_HsmServiceTransmit	endp

align	2

;       if ( sts & ( RxOK | RxERR | RxORN ) ) {         /* any receive events? */
;          /* process receive events */
public	_HsmServiceReceive
_HsmServiceReceive	proc	near

IF 0  ; ReceiveLookahead
;          rxd = ep->rxHead;
;          NSM_GET32( cmdsts, rxd->cmdsts );
;          if( cmdsts & OWN ) {
;             traffic = TRUE;

	push	bp
	mov	bp,sp

	push	si
	mov	bx,[hsm]
	les	si,[bx].EuphPhyterHsmContext.rxHead
	mov	al,byte ptr es:[si].EuphPhyterDesc.cmdsts[3]
	test	al,high(highword(OWN))
	jnz	short loc_HSR_0
loc_HSR_False:
IF FALSE EQ 0
	xor	ax,ax
ELSE
	mov	ax,FALSE
ENDIF
	pop	si
	pop	bp
	ret

loc_HSR_0:
;            /*********** for DRIVERS (not HSMMON) ***************/
;            if( !ep->rxLookState ) {
;                dsize = (uint16)(cmdsts & DSIZE);
;                dsize -= CRC_SIZE;
	mov	cx,word ptr es:[si].EuphPhyterDesc.cmdsts
	mov	ax,word ptr es:[si].EuphPhyterDesc.cmdsts[2]
	and	cx,DSIZE
	and	ax,not highword(OWN or INCCRC)
	sub	cx,CRC_SIZE
	cmp	[bx].EuphPhyterHsmContext.rxLookState,0
	jnz	short loc_HSR_NI
	mov	edx,es:[si].EuphPhyterDesc.buf


;                if( cmdsts & OK ) {
;                    /* packet is good, show it to the upper layers */
	test	ax,highword(OK)
	jz	short loc_HSR_I1
	xor	ax,ax

;                    NsmRxLookahead( hsm, (void *)rxd, rxd->buf,
;                        hsm->rxLookaheadSize, dsize,
;                        /* for real drivers, just show no errors */
;                        0,
;                        NULL );
loc_HSR_I2:
	push	0
	push	0	; PktDesc Dword
	push	ax	; status
	push	cx	; pktsz
	push	word ptr [bx].HsmContext.rxLookaheadSize
	push	edx	; LookaheadPointer
;	push	es
	push	si	; handle
	push	bx	; Context
	call	_NsmRxLookahead
	add	sp,9*2
	cmp	ax,NsmRxNotNow		; Now Indication OFF
	jnz	short loc_HSR_1
	jmp	short loc_HSR_False


;                } else if(( hsm->rxMode & ACCEPT_ALL_ERRORS ) &&
;                      ( cmdsts &(RUNT|TOOLONG|RXISERR|CRCERR|FAERR))) {
;                    rxsts = (uint16) ((cmdsts >> 18)
;                        );
;                    if( !rxsts )            /* no error indicated? */
;                        rxsts = HSM_RX_CRC_ERROR;   /* then force one */
;                    NsmRxLookahead( hsm, (void *)rxd, rxd->buf,
;                        hsm->rxLookaheadSize, dsize, rxsts, NULL );
;                }

loc_HSR_I1:
	test	[bx].HsmContext.rxMode, ACCEPT_ALL_ERRORS
	jz	short loc_HSR_1
	test	ax,highword(RUNT or TOOLONG or RXISERR or CRCERR or FAERR)
	jz	short loc_HSR_1
	shr	ax,2
	jnz	short loc_HSR_I2
	mov	ax,HSM_RX_CRC_ERROR
	jmp	short loc_HSR_I2

loc_HSR_NI:
;            } else {
;                /* NsmRxLookahead is already been called.  We need
;                ** to call NsmRxComplete instead */
;                dsize = (uint16)(cmdsts & DSIZE);
;                dsize -= CRC_SIZE;
;                if( ep->rxLookState == RX_LOOK_PENDING ) {
	mov	dl,RX_LOOK_IDLE
	xchg	[bx].EuphPhyterHsmContext.rxLookState,dl
	cmp	dl,RX_LOOK_PENDING
	jnz	short loc_HSR_1

;                    ep->rxLookState = RX_LOOK_IDLE;
;                    if( cmdsts & OK ) {
;                        rxsts = 0;
;                    } else {
;                        rxsts = (uint16) ((cmdsts >> 18) & 0x1f);
;                        if( !rxsts )        /* no error indicated? */
;                            rxsts = HSM_RX_CRC_ERROR; /* then force one */
;                    }
	test	ax,highword(OK)
	jnz	short loc_HSR_NI1
	and	ax,7ch
	shr	ax,2
	jnz	short loc_HSR_NI2
	mov	ax,HSM_RX_CRC_ERROR
	jmp	short loc_HSR_NI2
loc_HSR_NI1:
	xor	ax,ax
loc_HSR_NI2:

;                    NsmRxComplete( hsm, (void *)rxd, dsize, rxsts );
	push	ax	; status
	push	cx	; pktSize
;	push	es
	push	si	; handle
	push	bx	; Context
	call	_NsmRxComplete	; Nsmres do nothing
	add	sp,4*2

;                } else {    /* rxLookState != RX_LOOK_PENDING */
;                    ep->rxLookState = RX_LOOK_IDLE;
;                }
;            }

loc_HSR_1:

;            /* give the descriptor (and buffer) back to the chip */
;            rxd->cmdsts = RX_MAX_PACKET_SIZE | INCCRC;
;            NsmEnterCriticalSection( hsm, HSM_RX_CRITICAL_SECTION );
;
;            /* advance to the next descriptor */
;            ep->rxHead = (pEuphPhyterDesc) rxd->lLink;
;            rxd->lLink = 0;
;            rxd->pLink = 0;

	push	di
	push	HSM_RX_CRITICAL_SECTION
	push	word ptr [hsm]
	call	_NsmEnterCriticalSection
	pop	bx
	les	si,[bx].EuphPhyterHsmContext.rxHead
	xor	eax,eax
	mov	edx,es:[si].EuphPhyterDesc.lLink
	mov	es:[si].EuphPhyterDesc.lLink,eax
	mov	es:[si].EuphPhyterDesc.pLink,eax
	mov	es:[si].EuphPhyterDesc.cmdsts,(RX_MAX_PACKET_SIZE or INCCRC)
	mov	[bx].EuphPhyterHsmContext.rxHead,edx

;            if( !ep->rxHead ) {
;                ep->rxHead = rxd;
;//                IOW32 (rxdp, ep->rxHead->physAddr );
	test	edx,edx
	mov	cx,es
	jnz	short loc_HSR_2
	mov	word ptr [bx].EuphPhyterHsmContext.rxHead,si
	mov	word ptr [bx].EuphPhyterHsmContext.rxHead[2],cx
	jmp	short loc_HSR_3

;            } else {
;                ep->rxTail->lLink = (uint32) rxd;
;                /* make this an autonomous 32-bit operation,
;                ** even on 16-bit compilers */
;                NSM_SET32( ep->rxTail->pLink, rxd->physAddr );
;            }
loc_HSR_2:
	mov	edx,es:[si].EuphPhyterDesc.physAddr
	les	di,[bx].EuphPhyterHsmContext.rxTail
	mov	es:[di].EuphPhyterDesc.pLink,edx
	mov	word ptr es:[di].EuphPhyterDesc.lLink,si
	mov	word ptr es:[di].EuphPhyterDesc.lLink[2],cx
loc_HSR_3:

;            ep->rxTail = rxd;
;            IOW32( cr, RxENA );

	mov	dx,[bx].HsmContext.IOaddr
	mov	word ptr [bx].EuphPhyterHsmContext.rxTail,si
	mov	word ptr [bx].EuphPhyterHsmContext.rxTail[2],cx
IF NOT cr EQ 0
	add	dx,cr
ENDIF
;	mov	eax,RxENA	; eax=0
	mov	al,RxENA
	out	dx,eax

;            NsmLeaveCriticalSection( hsm, HSM_RX_CRITICAL_SECTION );

	push	bx
	call	_NsmLeaveCriticalSection
	add	sp,2*2
	mov	ax,TRUE
	pop	di
	pop	si
	pop	bp
	ret

;            /*************** for ALL *********************/
;          }
;  #ifndef NSM_SUPPLIES_RX_BUFFERS
;       /***************** for DRIVERS (not HSMMON) *********************/
;       } else if(( sts & RxEARLY )&&( rxd = ep->rxHead )) {
;           ep->imrValue &= ~RxEARLY;
;  #endif
;       }
ELSE  ; ReceiveChain
	enter	2,0
; hsm	equ	bp+4
HSR_rc	equ	bp-2

	push	si
	mov	bx,[hsm]
	cmp	[bx].EuphPhyterHsmContext.rxHead,0
	jz	short loc_HSR_false
	les	si,[bx].EuphPhyterHsmContext.rxHead
	mov	ax,word ptr es:[si].EuphPhyterDesc.cmdsts[2]
	test	ah,high(highword(OWN))
	jnz	short loc_HSR_0
loc_HSR_false:
	mov	ax,FALSE
	pop	si
	leave
	retn

loc_HSR_D:
	mov	ax,NsmRxPktDiscard
	jmp	short loc_HSR_1

loc_HSR_0:
	test	ax,highword(OK)
	jz	short loc_HSR_D
	mov	ax,word ptr es:[si].EuphPhyterDesc.cmdsts
	mov	cx,word ptr es:[si].EuphPhyterDesc.handle
	and	ax,DSIZE
	sub	ax,CRC_SIZE
	push	es:[si].EuphPhyterDesc.buf
	push	ax
	push	cx
	push	bx
;
; software multicast check (ToT)
IF 0
	push	es:[si].EuphPhyterDesc.buf
	push	bx
	call	_HsmCheckMulticast
	add	sp,3*2
	cmp	ax,NsmOK
	jnz	short loc_HSR_M1
ENDIF
;
	call	_NsmReceiveChain
loc_HSR_M1:
	pop	bx
	add	sp,4*2
	cmp	ax,NsmRxNotNow
	jz	short loc_HSR_false
loc_HSR_1:
	mov	[HSR_rc],ax

	push	di
	push	HSM_RX_CRITICAL_SECTION
	push	bx
	call	_NsmEnterCriticalSection
	pop	bx

	les	si,[bx].EuphPhyterHsmContext.rxHead
	xor	eax,eax
	mov	edx,es:[si].EuphPhyterDesc.lLink
	mov	es:[si].EuphPhyterDesc.lLink,eax
	mov	es:[si].EuphPhyterDesc.pLink,eax
	mov	[bx].EuphPhyterHsmContext.rxHead,edx
	cmp	word ptr [HSR_rc],NsmOK
	jz	short loc_HSR_RQ
loc_HSR_RC:
	mov	es:[si].EuphPhyterDesc.cmdsts,RX_MAX_PACKET_SIZE or INCCRC
	cmp	eax,[bx].EuphPhyterHsmContext.rxHead
	mov	cx,es
	mov	eax,es:[si].EuphPhyterDesc.physAddr
	jz	short loc_HSR_RC1
	les	di,[bx].EuphPhyterHsmContext.rxTail
	mov	es:[di].EuphPhyterDesc.pLink,eax
	mov	word ptr es:[di].EuphPhyterDesc.lLink,si
	mov	word ptr es:[di].EuphPhyterDesc.lLink[2],cx
	jmp	short loc_HSR_RC2
loc_HSR_RC1:
	mov	dx,[bx].HsmContext.IOaddr
	mov	word ptr [bx].EuphPhyterHsmContext.rxHead,si
	mov	word ptr [bx].EuphPhyterHsmContext.rxHead[2],cx
	add	dx,rxdp
	out	dx,eax
loc_HSR_RC2:
	mov	word ptr [bx].EuphPhyterHsmContext.rxTail,si
	mov	word ptr [bx].EuphPhyterHsmContext.rxTail[2],cx
	mov	dx,[bx].HsmContext.IOaddr
IF NOT cr EQ 0
	add	dx,cr
ENDIF
	mov	eax,RxENA
	out	dx,eax
	jmp	short loc_HSR_X

loc_HSR_RQ:
	cmp	eax,[bx].EuphPhyterHsmContext.rxBusyHead
	mov	cx,es
	jz	short loc_HSR_RQ1
	les	di,[bx].EuphPhyterHsmContext.rxBusyTail
	mov	word ptr es:[di].EuphPhyterDesc.lLink,si
	mov	word ptr es:[di].EuphPhyterDesc.lLink[2],cx
	jmp	short loc_HSR_RQ2
loc_HSR_RQ1:
	mov	word ptr [bx].EuphPhyterHsmContext.rxBusyHead,si
	mov	word ptr [bx].EuphPhyterHsmContext.rxBusyHead[2],cx
loc_HSR_RQ2:
	mov	word ptr [bx].EuphPhyterHsmContext.rxBusyTail,si
	mov	word ptr [bx].EuphPhyterHsmContext.rxBusyTail[2],cx

loc_HSR_X:
	push	bx
	call	_NsmLeaveCriticalSection
	add	sp,4
	mov	ax,TRUE
	pop	di
	pop	si
	leave
	retn
ENDIF

_HsmServiceReceive	endp

align	2

public	_HsmService
_HsmService	proc	near

	enter	4,0
; hsm		equ	bp+4
HS_sts		equ	bp-4


;    sts = ep->isrValue & ep->imrValue;
;    ep->isrValue &= !ep->imrValue;

	mov	bx,[hsm]
	mov	eax,[bx].EuphPhyterHsmContext.isrValue
	mov	edx,[bx].EuphPhyterHsmContext.imrValue
	and	eax,edx		; sts
	not	edx
	and	[bx].EuphPhyterHsmContext.isrValue,edx
	mov	[HS_sts],eax
	jz	short loc_HS_HP	; check tx/rx always.

;    while ( sts ) {
;
;       /* process all other low priority stuff */
;       if( sts &( MIBINT | TXRCMP | RXRCMP | RxSOVR | SWINT ) ) {

	test	eax,MIBINT or TXRCMP or RXRCMP or RxSOVR or SWINT
	jz	short loc_HS_HP

;           if( sts & MIBINT ) {
;               HsmUpdateStatistics( hsm );
;           }

;	test	dword ptr [HS_sts],MIBINT
	test	word ptr [HS_sts],lowword(MIBINT)
	jz	short loc_HS_LP1
	push	bx
	call	_HsmUpdateStatistics
	pop	bx

;           /* receiver has been reset, re-enable him */
;           if(( sts & RXRCMP )&& ep->rxResetPending ) {

loc_HS_LP1:
;	test	dword ptr [HS_sts],RXRCMP
	test	word ptr [HS_sts][2],highword(RXRCMP)
	jz	short loc_HS_LP2
	cmp	[bx].EuphPhyterHsmContext.rxResetPending,0
	jz	short loc_HS_LP2

;               epDebug("(RXRCMP)");
;               ep->rxResetPending = FALSE;
;               ep->imrValue &= ~RXRCMP;
;               ep->imrValue |= (RxOK | RxERR | RxORN | RxSOVR);
;               if( hsm->nsmOptions & DO_RX_PIPELINING ) {
;                   ep->imrValue |= RxEARLY;
;               }

	mov	[bx].EuphPhyterHsmContext.rxResetPending,FALSE
;	test	[bx].HsmContext.nsmOptions, DO_RX_PIPELINING
	test	word ptr [bx].HsmContext.nsmOptions, lowword(DO_RX_PIPELINING)
	mov	eax, RxOK or RxERR or RxORN or RxSOVR
	jz	short loc_HS_LP1_2
	or	eax, RxEARLY
loc_HS_LP1_2:
	and	[bx].EuphPhyterHsmContext.imrValue, not RXRCMP
	or	[bx].EuphPhyterHsmContext.imrValue, eax

;               IOW32( imr, ep->imrValue );
;               epRxInit( hsm );
;               IOW32( cr, RxENA );
;           }

	mov	dx,[bx].HsmContext.IOaddr
	mov	eax,[bx].EuphPhyterHsmContext.imrValue
	add	dx,imr
	push	bx
	out	dx,eax
	call	_epRxInit
	pop	bx
	mov	dx,[bx].HsmContext.IOaddr
IF NOT cr EQ 0
	add	dx,cr
ENDIF
	mov	eax,RxENA
	out	dx,eax


;           /* software interrupt */
;           if( sts & SWINT ) {
;               ;       /* do nothing */
;           }
;       }

loc_HS_LP2:
;	test	[HS_sts],SWINT
;	jz	short loc_HS_HP

loc_HS_HP:

;       do {
;          traffic = FALSE;
;          loopCount++;
;
;          if ( sts & ( TxOK | TxERR | TxURN ) ) {
;              while( HsmServiceTransmit( hsm ) );
;
;          if ( sts & ( RxOK | RxERR | RxORN | SWINT ) ) {         /* any receive events? */
;             while( HsmServiceReceive( hsm ));
;          } else if(( sts & RxEARLY )&&( rxd = ep->rxHead )) {
;              ep->imrValue &= ~RxEARLY;
;          }
;
;       } while ( traffic && loopCount < 10 );
;       ep->isrValue |= IOR32( isr );
;       sts = ep->isrValue & ep->imrValue;
;       ep->isrValue &= !ep->imrValue;
;    }
;    return( HsmOK );

	test	dword ptr [HS_sts], TxOK or TxERR or TxURN
	jz	short loc_HS_HP2
	push	bx
loc_HS_tx:
	call	_HsmServiceTransmit
	test	ax,ax
	jnz	short loc_HS_tx
	pop	bx
loc_HS_HP2:
	test	dword ptr [HS_sts], RxOK or RxERR or RxORN or SWINT
	jz	short loc_HS_Exit
	push	bx
loc_HS_rx:
	call	_HsmServiceReceive
	test	ax,ax
	jnz	short loc_HS_rx
	pop	bx
loc_HS_Exit:
	mov	ax,HsmOK
	leave
	ret
_HsmService	endp

align	2

public	_HsmDisableNicInts
_HsmDisableNicInts	proc	near
	push	bp
	mov	bp,sp
; hsm	equ	bp+4

;    IOW32( ier, 0 );            /* disable all EuphPhyter interrupts */

	mov	bx,[hsm]
	mov	dx,[bx].HsmContext.IOaddr
	add	dx,ier
	xor	eax,eax
	out	dx,eax

;    val = IOR32( isr );

	add	dx,(isr - ier)
	in	eax,dx

;    ep->isrValue |= val;

	or	[bx].EuphPhyterHsmContext.isrValue,eax

;    if( ep->isrValue & ep->imrValue )
;       rc |= 1;

	mov	edx,[bx].EuphPhyterHsmContext.imrValue
	test	[bx].EuphPhyterHsmContext.isrValue,edx
	setnz	cl

;//    if( ( val & ( RxOK | RxERR | RxORN | RxSOVR )) &&
;//        !( ep->imrValue & ( RxOK | RxERR | RxORN | RxSOVR )) )
;    if( (val & ~ep->imrValue) & ( RxOK | RxERR | RxORN | RxSOVR ))
;       rc |= 2;

	not	edx
	and	eax,edx		; masked events include Rx ones?
	test	eax,RxOK or RxERR or RxORN or RxSOVR
	setnz	al
	mov	ah,0
	shl	ax,1
	or	al,cl

;    return( rc );

	pop	bp
	retn
_HsmDisableNicInts	endp

public	_HsmEnableNicInts
_HsmEnableNicInts	proc	near
; hsm	equ	bp+4
	push	bp
	mov	bp,sp
	mov	bx,[hsm]

;    IOW32( ier, IE );           /* enable all currently unmasked ints */

	mov	dx,[bx].HsmContext.IOaddr
	add	dx,ier
IF NOT IE EQ 1
	mov	eax,IE
ELSE
	xor	eax,eax
	inc	ax
ENDIF
	out	dx,eax
	pop	bp
	ret
_HsmEnableNicInts	endp


IF 0
align	2

; int HsmCheckMulticast( HsmContext *hsm, pHsmPktDesc buf );
; buf is not a descriptor. it is virtual address of frame data.

public	_HsmCheckMulticast
_HsmCheckMulticast	proc	near

; hsm	equ	bp+4
HCM_buf	equ	bp+6

	push	bp
	mov	bp,sp

	les	bx,[HCM_buf]
	test	byte ptr es:[bx],1
	jz	short loc_HCM_uni
	cmp	dword ptr es:[bx],-1
	jnz	short loc_HCM_0
	cmp	word ptr es:[bx+4],-1
	jnz	short loc_HCM_0
loc_HCM_uni:
	mov	ax,NsmOK
	pop	bp
	ret

loc_HCM_0:
	push	si
	mov	si,[hsm]
	test	[si].HsmContext.rxMode,ACCEPT_CAM_QUALIFIED
	jz	short loc_HCM_false
	push	si
	push	es
	push	bx
	call	_epComputeHashTableIndex
	mov	cx,ax
	add	sp,4
	shr	ax,4
	pop	bx
	shl	ax,1
	and	cx,0fh		; bit number
	mov	si,ax		; hash table index
	bt	word ptr [bx+si].EuphPhyterHsmContext.hashTable,cx
	jnc	short loc_HCM_false
	mov	ax,NsmOK
	pop	si
	pop	bp
	ret
loc_HCM_false:
	mov	ax,NsmRxPktDiscard
	pop	si
	pop	bp
	ret
_HsmCheckMulticast	endp

ENDIF

align	2

public	_epComputeHashTableIndex
_epComputeHashTableIndex	proc	near

eCHTI_addr	equ	bp+4
POLYNOMIAL_be	equ	 04C11DB7h
POLYNOMIAL_le	equ	0EDB88320h

	push	bp
	mov	bp,sp
	mov	ch,3
	les	bx,[eCHTI_addr]
	or	eax,-1
loc_eCHTI_1:
	mov	bp,es:[bx]
	mov	cl,10h
	inc	bx
loc_eCHTI_2:
IF 1
		; big endian
	shl	eax,1
	rcl	dx,1
	xor	dx,bp
	shr	dx,1
	sbb	edx,edx
	and	edx,POLYNOMIAL_be
ELSE
		; little endian
	shr	eax,1
	rcl	dx,1
	xor	dx,bp
	shr	dx,1
	sbb	edx,edx
	and	edx,POLYNOMIAL_le
ENDIF
	xor	eax,edx
	shr	bp,1
	dec	cl
	jnz	short loc_eCHTI_2
	inc	bx
	dec	ch
	jnz	short loc_eCHTI_1
IF 1
		; DP83815 - the 9 least significant bits
		; The 9 most significant bits of Big Endian CRC32.
	shr	eax,23
ELSE
		; DP83820 - the 11 most significant bits
		; The 11 most significant bits of Big Endian CRC32.
	shr	eax,21
ENDIF
	pop	bp
	ret
_epComputeHashTableIndex	endp

align	2

public	_HsmMulticastLoad
_HsmMulticastLoad	proc	near
	enter	6,0
HML_stable	equ	bp-6
HML_rfcrSave	equ	bp-4

	mov	bx,[hsm]
	push	si
	push	di
	push	gs
	push	HSM_ALL_CRITICAL_SECTION
	push	bx
	call	_NsmEnterCriticalSection
	pop	si
IF 1
	mov	cx,16	; DP83815 64bytes
ELSE
	mov	cx,64	; DP83820 256bytes
ENDIF
	mov	dx,[si].HsmContext.IOaddr
	lea	di,[si].EuphPhyterHsmContext.hashTable
	add	dx,rfcr
	in	eax,dx
	mov	[HML_rfcrSave],eax
	xor	eax,eax
	mov	bx,ds
	mov	es,bx
	rep	stosd

	mov	cx,[si].HsmContext.MulticastTableSize
	jcxz	short loc_HML_3
	lgs	di,[si].HsmContext.MulticastTable
	mov	[HML_stable],cx
loc_HML_1:
	cmp	gs:[di].HsmMulticastTableEntry.useCount,0
	jz	short loc_HML_2
	lea	bx,[di].HsmMulticastTableEntry.macAddr
	push	gs
	push	bx
	call	_epComputeHashTableIndex
	mov	bx,ax
	add	sp,4
	mov	cx,ax
	shr	bx,3
	mov	dx,1
	and	cl,15
	and	bl,-2
	shl	dx,cl
	or	[si].EuphPhyterHsmContext.hashTable[bx],dx
loc_HML_2:
	add	di,sizeof (HsmMulticastTableEntry)
	dec	word ptr [HML_stable]
	jnz	short loc_HML_1
loc_HML_3:
	mov	dx,[si].HsmContext.IOaddr
IF 1
	mov	di,200h		; DP83815  hash table index starts from 0x200.
	mov	cx,32		; hash table size is 64bytes.
ELSE
	mov	di,100h		; DP83820  hash table index starts from 0x100.
	mov	cx,128		; hash table size is 256bytes.
ENDIF
	push	si
	add	dx,rfcr
	lea	si,[si].EuphPhyterHsmContext.hashTable
	xor	eax,eax
loc_HML_4:
	mov	ax,di
	out	dx,eax
	add	dx,(rfdr - rfcr)
	mov	ax,[si]
	add	di,2
	add	si,2
	out	dx,eax
	add	dx,(rfcr - rfdr)
	dec	cx
	jnz	short loc_HML_4
	mov	eax,[HML_rfcrSave]
	out	dx,eax
	call	NsmLeaveCriticalSection
	add	sp,4
	mov	ax,HsmOK
	pop	gs
	pop	di
	pop	si
	leave
	ret
_HsmMulticastLoad	endp

ENDIF

_TEXT	ends
end

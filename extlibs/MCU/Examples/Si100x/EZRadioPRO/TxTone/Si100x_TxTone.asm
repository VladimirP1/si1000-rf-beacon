;--------------------------------------------------------
; File Created by SDCC : free open source ANSI-C Compiler
; Version 3.6.8 #9947 (MINGW32)
;--------------------------------------------------------
	.module Si100x_TxTone
	.optsdcc -mmcs51 --model-small
	
;--------------------------------------------------------
; Public variables in this module
;--------------------------------------------------------
	.globl _phyWrite_PARM_2
	.globl _T0_ISR
	.globl _main
	.globl _LED
	.globl _SDN
	.globl _IRQ
	.globl _NSS1
	.globl _SPI0EN
	.globl _TXBMT0
	.globl _NSS0MD0
	.globl _NSS0MD1
	.globl _RXOVRN0
	.globl _MODF0
	.globl _WCOL0
	.globl _SPIF0
	.globl _AD0CM0
	.globl _AD0CM1
	.globl _AD0CM2
	.globl _AD0WINT
	.globl _AD0BUSY
	.globl _AD0INT
	.globl _BURSTEN
	.globl _AD0EN
	.globl _CCF0
	.globl _CCF1
	.globl _CCF2
	.globl _CCF3
	.globl _CCF4
	.globl _CCF5
	.globl _CR
	.globl _CF
	.globl _P
	.globl _F1
	.globl _OV
	.globl _RS0
	.globl _RS1
	.globl _F0
	.globl _AC
	.globl _CY
	.globl _T2XCLK
	.globl _T2RCLK
	.globl _TR2
	.globl _T2SPLIT
	.globl _TF2CEN
	.globl _TF2LEN
	.globl _TF2L
	.globl _TF2H
	.globl _SI
	.globl _ACK
	.globl _ARBLOST
	.globl _ACKRQ
	.globl _STO
	.globl _STA
	.globl _TXMODE
	.globl _MASTER
	.globl _PX0
	.globl _PT0
	.globl _PX1
	.globl _PT1
	.globl _PS0
	.globl _PT2
	.globl _PSPI0
	.globl _SPI1EN
	.globl _TXBMT1
	.globl _NSS1MD0
	.globl _NSS1MD1
	.globl _RXOVRN1
	.globl _MODF1
	.globl _WCOL1
	.globl _SPIF1
	.globl _EX0
	.globl _ET0
	.globl _EX1
	.globl _ET1
	.globl _ES0
	.globl _ET2
	.globl _ESPI0
	.globl _EA
	.globl _RI0
	.globl _TI0
	.globl _RB80
	.globl _TB80
	.globl _REN0
	.globl _MCE0
	.globl _S0MODE
	.globl _IT0
	.globl _IE0
	.globl _IT1
	.globl _IE1
	.globl _TR0
	.globl _TF0
	.globl _TR1
	.globl _TF1
	.globl __XPAGE
	.globl _PCA0CP4
	.globl _PCA0CP0
	.globl _PCA0
	.globl _PCA0CP3
	.globl _PCA0CP2
	.globl _PCA0CP1
	.globl _PCA0CP5
	.globl _TMR2
	.globl _TMR2RL
	.globl _ADC0LT
	.globl _ADC0GT
	.globl _ADC0
	.globl _TMR3
	.globl _TMR3RL
	.globl _TOFF
	.globl _DP
	.globl _VDM0CN
	.globl _PCA0CPH4
	.globl _PCA0CPL4
	.globl _PCA0CPH0
	.globl _PCA0CPL0
	.globl _PCA0H
	.globl _PCA0L
	.globl _SPI0CN
	.globl _EIP2
	.globl _EIP1
	.globl _SMB0ADM
	.globl _SMB0ADR
	.globl _P2MDIN
	.globl _P1MDIN
	.globl _P0MDIN
	.globl _B
	.globl _RSTSRC
	.globl _PCA0CPH3
	.globl _PCA0CPL3
	.globl _PCA0CPH2
	.globl _PCA0CPL2
	.globl _PCA0CPH1
	.globl _PCA0CPL1
	.globl _ADC0CN
	.globl _EIE2
	.globl _EIE1
	.globl _FLWR
	.globl _IT01CF
	.globl _XBR2
	.globl _XBR1
	.globl _XBR0
	.globl _ACC
	.globl _PCA0PWM
	.globl _PCA0CPM4
	.globl _PCA0CPM3
	.globl _PCA0CPM2
	.globl _PCA0CPM1
	.globl _PCA0CPM0
	.globl _PCA0MD
	.globl _PCA0CN
	.globl _P0MAT
	.globl _P2SKIP
	.globl _P1SKIP
	.globl _P0SKIP
	.globl _PCA0CPH5
	.globl _PCA0CPL5
	.globl _REF0CN
	.globl _PSW
	.globl _P1MAT
	.globl _PCA0CPM5
	.globl _TMR2H
	.globl _TMR2L
	.globl _TMR2RLH
	.globl _TMR2RLL
	.globl _REG0CN
	.globl _TMR2CN
	.globl _P0MASK
	.globl _ADC0LTH
	.globl _ADC0LTL
	.globl _ADC0GTH
	.globl _ADC0GTL
	.globl _SMB0DAT
	.globl _SMB0CF
	.globl _SMB0CN
	.globl _P1MASK
	.globl _ADC0H
	.globl _ADC0L
	.globl _ADC0TK
	.globl _ADC0CF
	.globl _ADC0MX
	.globl _ADC0PWR
	.globl _ADC0AC
	.globl _IREF0CN
	.globl _IP
	.globl _FLKEY
	.globl _FLSCL
	.globl _PMU0CF
	.globl _OSCICL
	.globl _OSCICN
	.globl _OSCXCN
	.globl _SPI1CN
	.globl _ONESHOT
	.globl _EMI0TC
	.globl _RTC0KEY
	.globl _RTC0DAT
	.globl _RTC0ADR
	.globl _EMI0CF
	.globl _EMI0CN
	.globl _CLKSEL
	.globl _IE
	.globl _SFRPAGE
	.globl _P2DRV
	.globl _P2MDOUT
	.globl _P1DRV
	.globl _P1MDOUT
	.globl _P0DRV
	.globl _P0MDOUT
	.globl _SPI0DAT
	.globl _SPI0CKR
	.globl _SPI0CFG
	.globl _P2
	.globl _CPT0MX
	.globl _CPT1MX
	.globl _CPT0MD
	.globl _CPT1MD
	.globl _CPT0CN
	.globl _CPT1CN
	.globl _SBUF0
	.globl _SCON0
	.globl _CRC0CNT
	.globl _DC0CN
	.globl _CRC0AUTO
	.globl _DC0CF
	.globl _TMR3H
	.globl _CRC0FLIP
	.globl _TMR3L
	.globl _CRC0IN
	.globl _TMR3RLH
	.globl _CRC0CN
	.globl _TMR3RLL
	.globl _CRC0DAT
	.globl _TMR3CN
	.globl _P1
	.globl _PSCTL
	.globl _CKCON
	.globl _TH1
	.globl _TH0
	.globl _TL1
	.globl _TL0
	.globl _TMOD
	.globl _TCON
	.globl _PCON
	.globl _TOFFH
	.globl _SPI1DAT
	.globl _TOFFL
	.globl _SPI1CKR
	.globl _SPI1CFG
	.globl _DPH
	.globl _DPL
	.globl _SP
	.globl _P0
	.globl _divideWithRounding_PARM_2
	.globl _RadioStatus
	.globl _PortInit
	.globl _SYSCLK_Init
	.globl _SPI1_Init
	.globl _phyWrite
	.globl _phyRead
	.globl _RadioInit
	.globl _RadioInitSoftwareReset
	.globl _delay
	.globl _SetTimeOut
	.globl _ClearTimeOut
	.globl _setTRxFrequency
	.globl _divideWithRounding
	.globl _setTxPower
;--------------------------------------------------------
; special function registers
;--------------------------------------------------------
	.area RSEG    (ABS,DATA)
	.org 0x0000
G$P0$0$0 == 0x0080
_P0	=	0x0080
G$SP$0$0 == 0x0081
_SP	=	0x0081
G$DPL$0$0 == 0x0082
_DPL	=	0x0082
G$DPH$0$0 == 0x0083
_DPH	=	0x0083
G$SPI1CFG$0$0 == 0x0084
_SPI1CFG	=	0x0084
G$SPI1CKR$0$0 == 0x0085
_SPI1CKR	=	0x0085
G$TOFFL$0$0 == 0x0085
_TOFFL	=	0x0085
G$SPI1DAT$0$0 == 0x0086
_SPI1DAT	=	0x0086
G$TOFFH$0$0 == 0x0086
_TOFFH	=	0x0086
G$PCON$0$0 == 0x0087
_PCON	=	0x0087
G$TCON$0$0 == 0x0088
_TCON	=	0x0088
G$TMOD$0$0 == 0x0089
_TMOD	=	0x0089
G$TL0$0$0 == 0x008a
_TL0	=	0x008a
G$TL1$0$0 == 0x008b
_TL1	=	0x008b
G$TH0$0$0 == 0x008c
_TH0	=	0x008c
G$TH1$0$0 == 0x008d
_TH1	=	0x008d
G$CKCON$0$0 == 0x008e
_CKCON	=	0x008e
G$PSCTL$0$0 == 0x008f
_PSCTL	=	0x008f
G$P1$0$0 == 0x0090
_P1	=	0x0090
G$TMR3CN$0$0 == 0x0091
_TMR3CN	=	0x0091
G$CRC0DAT$0$0 == 0x0091
_CRC0DAT	=	0x0091
G$TMR3RLL$0$0 == 0x0092
_TMR3RLL	=	0x0092
G$CRC0CN$0$0 == 0x0092
_CRC0CN	=	0x0092
G$TMR3RLH$0$0 == 0x0093
_TMR3RLH	=	0x0093
G$CRC0IN$0$0 == 0x0093
_CRC0IN	=	0x0093
G$TMR3L$0$0 == 0x0094
_TMR3L	=	0x0094
G$CRC0FLIP$0$0 == 0x0095
_CRC0FLIP	=	0x0095
G$TMR3H$0$0 == 0x0095
_TMR3H	=	0x0095
G$DC0CF$0$0 == 0x0096
_DC0CF	=	0x0096
G$CRC0AUTO$0$0 == 0x0096
_CRC0AUTO	=	0x0096
G$DC0CN$0$0 == 0x0097
_DC0CN	=	0x0097
G$CRC0CNT$0$0 == 0x0097
_CRC0CNT	=	0x0097
G$SCON0$0$0 == 0x0098
_SCON0	=	0x0098
G$SBUF0$0$0 == 0x0099
_SBUF0	=	0x0099
G$CPT1CN$0$0 == 0x009a
_CPT1CN	=	0x009a
G$CPT0CN$0$0 == 0x009b
_CPT0CN	=	0x009b
G$CPT1MD$0$0 == 0x009c
_CPT1MD	=	0x009c
G$CPT0MD$0$0 == 0x009d
_CPT0MD	=	0x009d
G$CPT1MX$0$0 == 0x009e
_CPT1MX	=	0x009e
G$CPT0MX$0$0 == 0x009f
_CPT0MX	=	0x009f
G$P2$0$0 == 0x00a0
_P2	=	0x00a0
G$SPI0CFG$0$0 == 0x00a1
_SPI0CFG	=	0x00a1
G$SPI0CKR$0$0 == 0x00a2
_SPI0CKR	=	0x00a2
G$SPI0DAT$0$0 == 0x00a3
_SPI0DAT	=	0x00a3
G$P0MDOUT$0$0 == 0x00a4
_P0MDOUT	=	0x00a4
G$P0DRV$0$0 == 0x00a4
_P0DRV	=	0x00a4
G$P1MDOUT$0$0 == 0x00a5
_P1MDOUT	=	0x00a5
G$P1DRV$0$0 == 0x00a5
_P1DRV	=	0x00a5
G$P2MDOUT$0$0 == 0x00a6
_P2MDOUT	=	0x00a6
G$P2DRV$0$0 == 0x00a6
_P2DRV	=	0x00a6
G$SFRPAGE$0$0 == 0x00a7
_SFRPAGE	=	0x00a7
G$IE$0$0 == 0x00a8
_IE	=	0x00a8
G$CLKSEL$0$0 == 0x00a9
_CLKSEL	=	0x00a9
G$EMI0CN$0$0 == 0x00aa
_EMI0CN	=	0x00aa
G$EMI0CF$0$0 == 0x00ab
_EMI0CF	=	0x00ab
G$RTC0ADR$0$0 == 0x00ac
_RTC0ADR	=	0x00ac
G$RTC0DAT$0$0 == 0x00ad
_RTC0DAT	=	0x00ad
G$RTC0KEY$0$0 == 0x00ae
_RTC0KEY	=	0x00ae
G$EMI0TC$0$0 == 0x00af
_EMI0TC	=	0x00af
G$ONESHOT$0$0 == 0x00af
_ONESHOT	=	0x00af
G$SPI1CN$0$0 == 0x00b0
_SPI1CN	=	0x00b0
G$OSCXCN$0$0 == 0x00b1
_OSCXCN	=	0x00b1
G$OSCICN$0$0 == 0x00b2
_OSCICN	=	0x00b2
G$OSCICL$0$0 == 0x00b3
_OSCICL	=	0x00b3
G$PMU0CF$0$0 == 0x00b5
_PMU0CF	=	0x00b5
G$FLSCL$0$0 == 0x00b6
_FLSCL	=	0x00b6
G$FLKEY$0$0 == 0x00b7
_FLKEY	=	0x00b7
G$IP$0$0 == 0x00b8
_IP	=	0x00b8
G$IREF0CN$0$0 == 0x00b9
_IREF0CN	=	0x00b9
G$ADC0AC$0$0 == 0x00ba
_ADC0AC	=	0x00ba
G$ADC0PWR$0$0 == 0x00ba
_ADC0PWR	=	0x00ba
G$ADC0MX$0$0 == 0x00bb
_ADC0MX	=	0x00bb
G$ADC0CF$0$0 == 0x00bc
_ADC0CF	=	0x00bc
G$ADC0TK$0$0 == 0x00bd
_ADC0TK	=	0x00bd
G$ADC0L$0$0 == 0x00bd
_ADC0L	=	0x00bd
G$ADC0H$0$0 == 0x00be
_ADC0H	=	0x00be
G$P1MASK$0$0 == 0x00bf
_P1MASK	=	0x00bf
G$SMB0CN$0$0 == 0x00c0
_SMB0CN	=	0x00c0
G$SMB0CF$0$0 == 0x00c1
_SMB0CF	=	0x00c1
G$SMB0DAT$0$0 == 0x00c2
_SMB0DAT	=	0x00c2
G$ADC0GTL$0$0 == 0x00c3
_ADC0GTL	=	0x00c3
G$ADC0GTH$0$0 == 0x00c4
_ADC0GTH	=	0x00c4
G$ADC0LTL$0$0 == 0x00c5
_ADC0LTL	=	0x00c5
G$ADC0LTH$0$0 == 0x00c6
_ADC0LTH	=	0x00c6
G$P0MASK$0$0 == 0x00c7
_P0MASK	=	0x00c7
G$TMR2CN$0$0 == 0x00c8
_TMR2CN	=	0x00c8
G$REG0CN$0$0 == 0x00c9
_REG0CN	=	0x00c9
G$TMR2RLL$0$0 == 0x00ca
_TMR2RLL	=	0x00ca
G$TMR2RLH$0$0 == 0x00cb
_TMR2RLH	=	0x00cb
G$TMR2L$0$0 == 0x00cc
_TMR2L	=	0x00cc
G$TMR2H$0$0 == 0x00cd
_TMR2H	=	0x00cd
G$PCA0CPM5$0$0 == 0x00ce
_PCA0CPM5	=	0x00ce
G$P1MAT$0$0 == 0x00cf
_P1MAT	=	0x00cf
G$PSW$0$0 == 0x00d0
_PSW	=	0x00d0
G$REF0CN$0$0 == 0x00d1
_REF0CN	=	0x00d1
G$PCA0CPL5$0$0 == 0x00d2
_PCA0CPL5	=	0x00d2
G$PCA0CPH5$0$0 == 0x00d3
_PCA0CPH5	=	0x00d3
G$P0SKIP$0$0 == 0x00d4
_P0SKIP	=	0x00d4
G$P1SKIP$0$0 == 0x00d5
_P1SKIP	=	0x00d5
G$P2SKIP$0$0 == 0x00d6
_P2SKIP	=	0x00d6
G$P0MAT$0$0 == 0x00d7
_P0MAT	=	0x00d7
G$PCA0CN$0$0 == 0x00d8
_PCA0CN	=	0x00d8
G$PCA0MD$0$0 == 0x00d9
_PCA0MD	=	0x00d9
G$PCA0CPM0$0$0 == 0x00da
_PCA0CPM0	=	0x00da
G$PCA0CPM1$0$0 == 0x00db
_PCA0CPM1	=	0x00db
G$PCA0CPM2$0$0 == 0x00dc
_PCA0CPM2	=	0x00dc
G$PCA0CPM3$0$0 == 0x00dd
_PCA0CPM3	=	0x00dd
G$PCA0CPM4$0$0 == 0x00de
_PCA0CPM4	=	0x00de
G$PCA0PWM$0$0 == 0x00df
_PCA0PWM	=	0x00df
G$ACC$0$0 == 0x00e0
_ACC	=	0x00e0
G$XBR0$0$0 == 0x00e1
_XBR0	=	0x00e1
G$XBR1$0$0 == 0x00e2
_XBR1	=	0x00e2
G$XBR2$0$0 == 0x00e3
_XBR2	=	0x00e3
G$IT01CF$0$0 == 0x00e4
_IT01CF	=	0x00e4
G$FLWR$0$0 == 0x00e5
_FLWR	=	0x00e5
G$EIE1$0$0 == 0x00e6
_EIE1	=	0x00e6
G$EIE2$0$0 == 0x00e7
_EIE2	=	0x00e7
G$ADC0CN$0$0 == 0x00e8
_ADC0CN	=	0x00e8
G$PCA0CPL1$0$0 == 0x00e9
_PCA0CPL1	=	0x00e9
G$PCA0CPH1$0$0 == 0x00ea
_PCA0CPH1	=	0x00ea
G$PCA0CPL2$0$0 == 0x00eb
_PCA0CPL2	=	0x00eb
G$PCA0CPH2$0$0 == 0x00ec
_PCA0CPH2	=	0x00ec
G$PCA0CPL3$0$0 == 0x00ed
_PCA0CPL3	=	0x00ed
G$PCA0CPH3$0$0 == 0x00ee
_PCA0CPH3	=	0x00ee
G$RSTSRC$0$0 == 0x00ef
_RSTSRC	=	0x00ef
G$B$0$0 == 0x00f0
_B	=	0x00f0
G$P0MDIN$0$0 == 0x00f1
_P0MDIN	=	0x00f1
G$P1MDIN$0$0 == 0x00f2
_P1MDIN	=	0x00f2
G$P2MDIN$0$0 == 0x00f3
_P2MDIN	=	0x00f3
G$SMB0ADR$0$0 == 0x00f4
_SMB0ADR	=	0x00f4
G$SMB0ADM$0$0 == 0x00f5
_SMB0ADM	=	0x00f5
G$EIP1$0$0 == 0x00f6
_EIP1	=	0x00f6
G$EIP2$0$0 == 0x00f7
_EIP2	=	0x00f7
G$SPI0CN$0$0 == 0x00f8
_SPI0CN	=	0x00f8
G$PCA0L$0$0 == 0x00f9
_PCA0L	=	0x00f9
G$PCA0H$0$0 == 0x00fa
_PCA0H	=	0x00fa
G$PCA0CPL0$0$0 == 0x00fb
_PCA0CPL0	=	0x00fb
G$PCA0CPH0$0$0 == 0x00fc
_PCA0CPH0	=	0x00fc
G$PCA0CPL4$0$0 == 0x00fd
_PCA0CPL4	=	0x00fd
G$PCA0CPH4$0$0 == 0x00fe
_PCA0CPH4	=	0x00fe
G$VDM0CN$0$0 == 0x00ff
_VDM0CN	=	0x00ff
G$DP$0$0 == 0x8382
_DP	=	0x8382
G$TOFF$0$0 == 0x8685
_TOFF	=	0x8685
G$TMR3RL$0$0 == 0x9392
_TMR3RL	=	0x9392
G$TMR3$0$0 == 0x9594
_TMR3	=	0x9594
G$ADC0$0$0 == 0xbebd
_ADC0	=	0xbebd
G$ADC0GT$0$0 == 0xc4c3
_ADC0GT	=	0xc4c3
G$ADC0LT$0$0 == 0xc6c5
_ADC0LT	=	0xc6c5
G$TMR2RL$0$0 == 0xcbca
_TMR2RL	=	0xcbca
G$TMR2$0$0 == 0xcdcc
_TMR2	=	0xcdcc
G$PCA0CP5$0$0 == 0xd3d2
_PCA0CP5	=	0xd3d2
G$PCA0CP1$0$0 == 0xeae9
_PCA0CP1	=	0xeae9
G$PCA0CP2$0$0 == 0xeceb
_PCA0CP2	=	0xeceb
G$PCA0CP3$0$0 == 0xeeed
_PCA0CP3	=	0xeeed
G$PCA0$0$0 == 0xfaf9
_PCA0	=	0xfaf9
G$PCA0CP0$0$0 == 0xfcfb
_PCA0CP0	=	0xfcfb
G$PCA0CP4$0$0 == 0xfefd
_PCA0CP4	=	0xfefd
G$_XPAGE$0$0 == 0x00aa
__XPAGE	=	0x00aa
;--------------------------------------------------------
; special function bits
;--------------------------------------------------------
	.area RSEG    (ABS,DATA)
	.org 0x0000
G$TF1$0$0 == 0x008f
_TF1	=	0x008f
G$TR1$0$0 == 0x008e
_TR1	=	0x008e
G$TF0$0$0 == 0x008d
_TF0	=	0x008d
G$TR0$0$0 == 0x008c
_TR0	=	0x008c
G$IE1$0$0 == 0x008b
_IE1	=	0x008b
G$IT1$0$0 == 0x008a
_IT1	=	0x008a
G$IE0$0$0 == 0x0089
_IE0	=	0x0089
G$IT0$0$0 == 0x0088
_IT0	=	0x0088
G$S0MODE$0$0 == 0x009f
_S0MODE	=	0x009f
G$MCE0$0$0 == 0x009d
_MCE0	=	0x009d
G$REN0$0$0 == 0x009c
_REN0	=	0x009c
G$TB80$0$0 == 0x009b
_TB80	=	0x009b
G$RB80$0$0 == 0x009a
_RB80	=	0x009a
G$TI0$0$0 == 0x0099
_TI0	=	0x0099
G$RI0$0$0 == 0x0098
_RI0	=	0x0098
G$EA$0$0 == 0x00af
_EA	=	0x00af
G$ESPI0$0$0 == 0x00ae
_ESPI0	=	0x00ae
G$ET2$0$0 == 0x00ad
_ET2	=	0x00ad
G$ES0$0$0 == 0x00ac
_ES0	=	0x00ac
G$ET1$0$0 == 0x00ab
_ET1	=	0x00ab
G$EX1$0$0 == 0x00aa
_EX1	=	0x00aa
G$ET0$0$0 == 0x00a9
_ET0	=	0x00a9
G$EX0$0$0 == 0x00a8
_EX0	=	0x00a8
G$SPIF1$0$0 == 0x00b7
_SPIF1	=	0x00b7
G$WCOL1$0$0 == 0x00b6
_WCOL1	=	0x00b6
G$MODF1$0$0 == 0x00b5
_MODF1	=	0x00b5
G$RXOVRN1$0$0 == 0x00b4
_RXOVRN1	=	0x00b4
G$NSS1MD1$0$0 == 0x00b3
_NSS1MD1	=	0x00b3
G$NSS1MD0$0$0 == 0x00b2
_NSS1MD0	=	0x00b2
G$TXBMT1$0$0 == 0x00b1
_TXBMT1	=	0x00b1
G$SPI1EN$0$0 == 0x00b0
_SPI1EN	=	0x00b0
G$PSPI0$0$0 == 0x00be
_PSPI0	=	0x00be
G$PT2$0$0 == 0x00bd
_PT2	=	0x00bd
G$PS0$0$0 == 0x00bc
_PS0	=	0x00bc
G$PT1$0$0 == 0x00bb
_PT1	=	0x00bb
G$PX1$0$0 == 0x00ba
_PX1	=	0x00ba
G$PT0$0$0 == 0x00b9
_PT0	=	0x00b9
G$PX0$0$0 == 0x00b8
_PX0	=	0x00b8
G$MASTER$0$0 == 0x00c7
_MASTER	=	0x00c7
G$TXMODE$0$0 == 0x00c6
_TXMODE	=	0x00c6
G$STA$0$0 == 0x00c5
_STA	=	0x00c5
G$STO$0$0 == 0x00c4
_STO	=	0x00c4
G$ACKRQ$0$0 == 0x00c3
_ACKRQ	=	0x00c3
G$ARBLOST$0$0 == 0x00c2
_ARBLOST	=	0x00c2
G$ACK$0$0 == 0x00c1
_ACK	=	0x00c1
G$SI$0$0 == 0x00c0
_SI	=	0x00c0
G$TF2H$0$0 == 0x00cf
_TF2H	=	0x00cf
G$TF2L$0$0 == 0x00ce
_TF2L	=	0x00ce
G$TF2LEN$0$0 == 0x00cd
_TF2LEN	=	0x00cd
G$TF2CEN$0$0 == 0x00cc
_TF2CEN	=	0x00cc
G$T2SPLIT$0$0 == 0x00cb
_T2SPLIT	=	0x00cb
G$TR2$0$0 == 0x00ca
_TR2	=	0x00ca
G$T2RCLK$0$0 == 0x00c9
_T2RCLK	=	0x00c9
G$T2XCLK$0$0 == 0x00c8
_T2XCLK	=	0x00c8
G$CY$0$0 == 0x00d7
_CY	=	0x00d7
G$AC$0$0 == 0x00d6
_AC	=	0x00d6
G$F0$0$0 == 0x00d5
_F0	=	0x00d5
G$RS1$0$0 == 0x00d4
_RS1	=	0x00d4
G$RS0$0$0 == 0x00d3
_RS0	=	0x00d3
G$OV$0$0 == 0x00d2
_OV	=	0x00d2
G$F1$0$0 == 0x00d1
_F1	=	0x00d1
G$P$0$0 == 0x00d0
_P	=	0x00d0
G$CF$0$0 == 0x00df
_CF	=	0x00df
G$CR$0$0 == 0x00de
_CR	=	0x00de
G$CCF5$0$0 == 0x00dd
_CCF5	=	0x00dd
G$CCF4$0$0 == 0x00dc
_CCF4	=	0x00dc
G$CCF3$0$0 == 0x00db
_CCF3	=	0x00db
G$CCF2$0$0 == 0x00da
_CCF2	=	0x00da
G$CCF1$0$0 == 0x00d9
_CCF1	=	0x00d9
G$CCF0$0$0 == 0x00d8
_CCF0	=	0x00d8
G$AD0EN$0$0 == 0x00ef
_AD0EN	=	0x00ef
G$BURSTEN$0$0 == 0x00ee
_BURSTEN	=	0x00ee
G$AD0INT$0$0 == 0x00ed
_AD0INT	=	0x00ed
G$AD0BUSY$0$0 == 0x00ec
_AD0BUSY	=	0x00ec
G$AD0WINT$0$0 == 0x00eb
_AD0WINT	=	0x00eb
G$AD0CM2$0$0 == 0x00ea
_AD0CM2	=	0x00ea
G$AD0CM1$0$0 == 0x00e9
_AD0CM1	=	0x00e9
G$AD0CM0$0$0 == 0x00e8
_AD0CM0	=	0x00e8
G$SPIF0$0$0 == 0x00ff
_SPIF0	=	0x00ff
G$WCOL0$0$0 == 0x00fe
_WCOL0	=	0x00fe
G$MODF0$0$0 == 0x00fd
_MODF0	=	0x00fd
G$RXOVRN0$0$0 == 0x00fc
_RXOVRN0	=	0x00fc
G$NSS0MD1$0$0 == 0x00fb
_NSS0MD1	=	0x00fb
G$NSS0MD0$0$0 == 0x00fa
_NSS0MD0	=	0x00fa
G$TXBMT0$0$0 == 0x00f9
_TXBMT0	=	0x00f9
G$SPI0EN$0$0 == 0x00f8
_SPI0EN	=	0x00f8
G$NSS1$0$0 == 0x0094
_NSS1	=	0x0094
G$IRQ$0$0 == 0x0081
_IRQ	=	0x0081
G$SDN$0$0 == 0x00a6
_SDN	=	0x00a6
G$LED$0$0 == 0x0095
_LED	=	0x0095
;--------------------------------------------------------
; overlayable register banks
;--------------------------------------------------------
	.area REG_BANK_0	(REL,OVR,DATA)
	.ds 8
;--------------------------------------------------------
; internal ram data
;--------------------------------------------------------
	.area DSEG    (DATA)
G$RadioStatus$0$0 == 0x0043
_RadioStatus	=	0x0043
LSi100x_TxTone.setTRxFrequency$frequency$1$45==.
_setTRxFrequency_frequency_1_45:
	.ds 4
LSi100x_TxTone.setTRxFrequency$nominalCarrierFrequency$1$46==.
_setTRxFrequency_nominalCarrierFrequency_1_46:
	.ds 2
LSi100x_TxTone.divideWithRounding$divisor$1$49==.
_divideWithRounding_PARM_2:
	.ds 4
;--------------------------------------------------------
; overlayable items in internal ram 
;--------------------------------------------------------
	.area	OSEG    (OVR,DATA)
LSi100x_TxTone.phyWrite$value$1$23==.
_phyWrite_PARM_2:
	.ds 1
	.area	OSEG    (OVR,DATA)
	.area	OSEG    (OVR,DATA)
LSi100x_TxTone.delay$reload$1$38==.
_delay_reload_1_38:
	.ds 2
	.area	OSEG    (OVR,DATA)
LSi100x_TxTone.SetTimeOut$reload$1$40==.
_SetTimeOut_reload_1_40:
	.ds 2
;--------------------------------------------------------
; Stack segment in internal ram 
;--------------------------------------------------------
	.area	SSEG
__start__stack:
	.ds	1

;--------------------------------------------------------
; indirectly addressable internal ram data
;--------------------------------------------------------
	.area ISEG    (DATA)
;--------------------------------------------------------
; absolute internal ram data
;--------------------------------------------------------
	.area IABS    (ABS,DATA)
	.area IABS    (ABS,DATA)
;--------------------------------------------------------
; bit data
;--------------------------------------------------------
	.area BSEG    (BIT)
;--------------------------------------------------------
; paged external ram data
;--------------------------------------------------------
	.area PSEG    (PAG,XDATA)
;--------------------------------------------------------
; external ram data
;--------------------------------------------------------
	.area XSEG    (XDATA)
;--------------------------------------------------------
; absolute external ram data
;--------------------------------------------------------
	.area XABS    (ABS,XDATA)
;--------------------------------------------------------
; external initialized ram data
;--------------------------------------------------------
	.area XISEG   (XDATA)
	.area HOME    (CODE)
	.area GSINIT0 (CODE)
	.area GSINIT1 (CODE)
	.area GSINIT2 (CODE)
	.area GSINIT3 (CODE)
	.area GSINIT4 (CODE)
	.area GSINIT5 (CODE)
	.area GSINIT  (CODE)
	.area GSFINAL (CODE)
	.area CSEG    (CODE)
;--------------------------------------------------------
; interrupt vector 
;--------------------------------------------------------
	.area HOME    (CODE)
__interrupt_vect:
	ljmp	__sdcc_gsinit_startup
	reti
	.ds	7
	ljmp	_T0_ISR
;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	.area HOME    (CODE)
	.area GSINIT  (CODE)
	.area GSFINAL (CODE)
	.area GSINIT  (CODE)
	.globl __sdcc_gsinit_startup
	.globl __sdcc_program_startup
	.globl __start__stack
	.globl __mcs51_genXINIT
	.globl __mcs51_genXRAMCLEAR
	.globl __mcs51_genRAMCLEAR
	.area GSFINAL (CODE)
	ljmp	__sdcc_program_startup
;--------------------------------------------------------
; Home
;--------------------------------------------------------
	.area HOME    (CODE)
	.area HOME    (CODE)
__sdcc_program_startup:
	ljmp	_main
;	return from main will return to caller
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CSEG    (CODE)
;------------------------------------------------------------
;Allocation info for local variables in function 'main'
;------------------------------------------------------------
;status                    Allocated to registers 
;------------------------------------------------------------
	G$main$0$0 ==.
	C$Si100x_TxTone.c$150$0$0 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:150: void main (void)
;	-----------------------------------------
;	 function main
;	-----------------------------------------
_main:
	ar7 = 0x07
	ar6 = 0x06
	ar5 = 0x05
	ar4 = 0x04
	ar3 = 0x03
	ar2 = 0x02
	ar1 = 0x01
	ar0 = 0x00
	C$Si100x_TxTone.c$154$1$15 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:154: PCA0MD   &= ~0x40;                  // disable F930 watchdog
	anl	_PCA0MD,#0xbf
	C$Si100x_TxTone.c$164$1$15 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:164: PortInit ();
	lcall	_PortInit
	C$Si100x_TxTone.c$165$1$15 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:165: SYSCLK_Init ();
	lcall	_SYSCLK_Init
	C$Si100x_TxTone.c$166$1$15 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:166: SPI1_Init ();
	lcall	_SPI1_Init
	C$Si100x_TxTone.c$168$1$15 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:168: EA = 1;
;	assignBit
	setb	_EA
	C$Si100x_TxTone.c$170$1$15 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:170: RadioStatus = RadioInit();
	lcall	_RadioInit
	mov	_RadioStatus,dpl
	C$Si100x_TxTone.c$173$1$15 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:173: while(RadioStatus);
00101$:
	mov	a,_RadioStatus
	jnz	00101$
	C$Si100x_TxTone.c$178$1$15 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:178: setTRxFrequency (915000000L);
	mov	dptr,#0xcac0
	mov	b,#0x89
	mov	a,#0x36
	lcall	_setTRxFrequency
	C$Si100x_TxTone.c$194$1$15 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:194: setTxPower(7);                      // Full power
	mov	dpl,#0x07
	lcall	_setTxPower
	C$Si100x_TxTone.c$197$1$15 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:197: phyWrite(EZRADIOPRO_MODULATION_MODE_CONTROL_2, 0x00);
	mov	_phyWrite_PARM_2,#0x00
	mov	dpl,#0x71
	lcall	_phyWrite
	C$Si100x_TxTone.c$203$1$15 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:203: phyWrite(EZRADIOPRO_GPIO1_CONFIGURATION, 0x12);
	mov	_phyWrite_PARM_2,#0x12
	mov	dpl,#0x0c
	lcall	_phyWrite
	C$Si100x_TxTone.c$204$1$15 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:204: phyWrite(EZRADIOPRO_GPIO2_CONFIGURATION, 0x15);
	mov	_phyWrite_PARM_2,#0x15
	mov	dpl,#0x0d
	lcall	_phyWrite
	C$Si100x_TxTone.c$208$1$15 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:208: phyWrite(EZRADIOPRO_OPERATING_AND_FUNCTION_CONTROL_1,(EZRADIOPRO_TXON|EZRADIOPRO_XTON));
	mov	_phyWrite_PARM_2,#0x09
	mov	dpl,#0x07
	lcall	_phyWrite
	C$Si100x_TxTone.c$210$1$15 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:210: delay(ONE_MS);                     // Wait 2 ms
	mov	dptr,#0x00d0
	lcall	_delay
	C$Si100x_TxTone.c$212$1$15 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:212: status = phyRead(EZRADIOPRO_DEVICE_STATUS);
	mov	dpl,#0x02
	lcall	_phyRead
	mov	a,dpl
	C$Si100x_TxTone.c$215$1$15 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:215: if((status& 0x03)==0x02)
	anl	a,#0x03
	mov	r7,a
	cjne	r7,#0x02,00110$
	C$Si100x_TxTone.c$218$2$16 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:218: while(1)
00105$:
	C$Si100x_TxTone.c$220$3$17 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:220: LED = !LED;
	cpl	_LED
	C$Si100x_TxTone.c$221$3$17 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:221: delay(TWENTY_FIVE_MS);
	mov	dptr,#0x1458
	lcall	_delay
	C$Si100x_TxTone.c$222$3$17 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:222: delay(TWENTY_FIVE_MS);
	mov	dptr,#0x1458
	lcall	_delay
	C$Si100x_TxTone.c$223$3$17 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:223: delay(TWENTY_FIVE_MS);
	mov	dptr,#0x1458
	lcall	_delay
	C$Si100x_TxTone.c$224$3$17 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:224: delay(TWENTY_FIVE_MS);
	mov	dptr,#0x1458
	lcall	_delay
	C$Si100x_TxTone.c$228$1$15 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:228: while(1);
	sjmp	00105$
00110$:
	sjmp	00110$
	C$Si100x_TxTone.c$229$1$15 ==.
	XG$main$0$0 ==.
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'PortInit'
;------------------------------------------------------------
	G$PortInit$0$0 ==.
	C$Si100x_TxTone.c$259$1$15 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:259: void PortInit (void)
;	-----------------------------------------
;	 function PortInit
;	-----------------------------------------
_PortInit:
	C$Si100x_TxTone.c$262$1$19 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:262: XBR1    |= 0x40;                    // Enable SPI1 (3 wire mode)
	orl	_XBR1,#0x40
	C$Si100x_TxTone.c$263$1$19 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:263: P1MDOUT |= 0x15;                    // MOSI, SCK, NSS, push-pull
	orl	_P1MDOUT,#0x15
	C$Si100x_TxTone.c$264$1$19 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:264: SFRPAGE  = CONFIG_PAGE;
	mov	_SFRPAGE,#0x0f
	C$Si100x_TxTone.c$265$1$19 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:265: P1DRV   |= 0x15;                    // MOSI, SCK, NSS, high current mode
	orl	_P1DRV,#0x15
	C$Si100x_TxTone.c$266$1$19 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:266: SFRPAGE  = LEGACY_PAGE;
	mov	_SFRPAGE,#0x00
	C$Si100x_TxTone.c$270$1$19 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:270: P2MDOUT |= 0x40;                    // SDN P2.6 push-pull
	orl	_P2MDOUT,#0x40
	C$Si100x_TxTone.c$271$1$19 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:271: SFRPAGE  = CONFIG_PAGE;
	mov	_SFRPAGE,#0x0f
	C$Si100x_TxTone.c$272$1$19 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:272: P2DRV   |= 0x40;                    // SDN  P2.6 high-current mode
	orl	_P2DRV,#0x40
	C$Si100x_TxTone.c$273$1$19 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:273: SFRPAGE  = LEGACY_PAGE;
	mov	_SFRPAGE,#0x00
	C$Si100x_TxTone.c$287$1$19 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:287: SDN      = 0;                       // SDN low before enabling crossbar
;	assignBit
	clr	_SDN
	C$Si100x_TxTone.c$289$1$19 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:289: XBR2    |= 0x40;                    // enable Crossbar
	orl	_XBR2,#0x40
	C$Si100x_TxTone.c$290$1$19 ==.
	XG$PortInit$0$0 ==.
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'SYSCLK_Init'
;------------------------------------------------------------
	G$SYSCLK_Init$0$0 ==.
	C$Si100x_TxTone.c$300$1$19 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:300: void SYSCLK_Init (void)
;	-----------------------------------------
;	 function SYSCLK_Init
;	-----------------------------------------
_SYSCLK_Init:
	C$Si100x_TxTone.c$302$1$21 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:302: CLKSEL    = 0x14;                   // use 10 MHz clock
	mov	_CLKSEL,#0x14
	C$Si100x_TxTone.c$303$1$21 ==.
	XG$SYSCLK_Init$0$0 ==.
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'SPI1_Init'
;------------------------------------------------------------
	G$SPI1_Init$0$0 ==.
	C$Si100x_TxTone.c$312$1$21 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:312: void SPI1_Init()
;	-----------------------------------------
;	 function SPI1_Init
;	-----------------------------------------
_SPI1_Init:
	C$Si100x_TxTone.c$315$1$22 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:315: SPI1CFG   = 0x40;                   // master mode
	mov	_SPI1CFG,#0x40
	C$Si100x_TxTone.c$316$1$22 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:316: SPI1CN    = 0x00;                   // 3 wire master mode
	mov	_SPI1CN,#0x00
	C$Si100x_TxTone.c$317$1$22 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:317: SPI1CKR   = 0x00;                   // Use SYSCLK/2
	mov	_SPI1CKR,#0x00
	C$Si100x_TxTone.c$318$1$22 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:318: SPI1CN   |= 0x01;                   // enable SPI
	orl	_SPI1CN,#0x01
	C$Si100x_TxTone.c$319$1$22 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:319: NSS1 = 1;                           // set NSS high
;	assignBit
	setb	_NSS1
	C$Si100x_TxTone.c$320$1$22 ==.
	XG$SPI1_Init$0$0 ==.
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'phyWrite'
;------------------------------------------------------------
;value                     Allocated with name '_phyWrite_PARM_2'
;reg                       Allocated to registers r7 
;------------------------------------------------------------
	G$phyWrite$0$0 ==.
	C$Si100x_TxTone.c$335$1$22 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:335: void phyWrite (U8 reg, U8 value)
;	-----------------------------------------
;	 function phyWrite
;	-----------------------------------------
_phyWrite:
	mov	r7,dpl
	C$Si100x_TxTone.c$338$1$24 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:338: NSS1 = 0;                           // drive NSS low
;	assignBit
	clr	_NSS1
	C$Si100x_TxTone.c$339$1$24 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:339: SPIF1 = 0;                          // clear SPIF
;	assignBit
	clr	_SPIF1
	C$Si100x_TxTone.c$340$1$24 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:340: SPI1DAT = (reg | 0x80);             // write reg address
	mov	a,#0x80
	orl	a,r7
	mov	_SPI1DAT,a
	C$Si100x_TxTone.c$341$1$24 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:341: while(!TXBMT1);                     // wait on TXBMT
00101$:
	jnb	_TXBMT1,00101$
	C$Si100x_TxTone.c$342$1$24 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:342: SPI1DAT = value;                    // write value
	mov	_SPI1DAT,_phyWrite_PARM_2
	C$Si100x_TxTone.c$343$1$24 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:343: while(!TXBMT1);                     // wait on TXBMT
00104$:
	jnb	_TXBMT1,00104$
	C$Si100x_TxTone.c$344$1$24 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:344: while((SPI1CFG & 0x80) == 0x80);    // wait on SPIBSY
00107$:
	mov	a,#0x80
	anl	a,_SPI1CFG
	mov	r7,a
	cjne	r7,#0x80,00129$
	sjmp	00107$
00129$:
	C$Si100x_TxTone.c$346$1$24 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:346: SPIF1 = 0;                          // leave SPIF cleared
;	assignBit
	clr	_SPIF1
	C$Si100x_TxTone.c$347$1$24 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:347: NSS1 = 1;                           // drive NSS high
;	assignBit
	setb	_NSS1
	C$Si100x_TxTone.c$348$1$24 ==.
	XG$phyWrite$0$0 ==.
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'phyRead'
;------------------------------------------------------------
;reg                       Allocated to registers r7 
;value                     Allocated to registers 
;------------------------------------------------------------
	G$phyRead$0$0 ==.
	C$Si100x_TxTone.c$357$1$24 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:357: U8 phyRead (U8 reg)
;	-----------------------------------------
;	 function phyRead
;	-----------------------------------------
_phyRead:
	mov	r7,dpl
	C$Si100x_TxTone.c$362$1$26 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:362: NSS1 = 0;                           // drive NSS low
;	assignBit
	clr	_NSS1
	C$Si100x_TxTone.c$363$1$26 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:363: SPIF1 = 0;                          // cleat SPIF
;	assignBit
	clr	_SPIF1
	C$Si100x_TxTone.c$364$1$26 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:364: SPI1DAT = ( reg );                  // write reg address
	mov	_SPI1DAT,r7
	C$Si100x_TxTone.c$365$1$26 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:365: while(!TXBMT1);                     // wait on TXBMT
00101$:
	jnb	_TXBMT1,00101$
	C$Si100x_TxTone.c$366$1$26 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:366: SPI1DAT = 0x00;                     // write anything
	mov	_SPI1DAT,#0x00
	C$Si100x_TxTone.c$367$1$26 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:367: while(!TXBMT1);                     // wait on TXBMT
00104$:
	jnb	_TXBMT1,00104$
	C$Si100x_TxTone.c$368$1$26 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:368: while((SPI1CFG & 0x80) == 0x80);    // wait on SPIBSY
00107$:
	mov	a,#0x80
	anl	a,_SPI1CFG
	mov	r7,a
	cjne	r7,#0x80,00129$
	sjmp	00107$
00129$:
	C$Si100x_TxTone.c$369$1$26 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:369: value = SPI1DAT;                    // read value
	mov	dpl,_SPI1DAT
	C$Si100x_TxTone.c$370$1$26 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:370: SPIF1 = 0;                          // leave SPIF cleared
;	assignBit
	clr	_SPIF1
	C$Si100x_TxTone.c$371$1$26 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:371: NSS1 = 1;                           // drive NSS low
;	assignBit
	setb	_NSS1
	C$Si100x_TxTone.c$373$1$26 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:373: return value;
	C$Si100x_TxTone.c$374$1$26 ==.
	XG$phyRead$0$0 ==.
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'RadioInit'
;------------------------------------------------------------
;status                    Allocated to registers r7 
;------------------------------------------------------------
	G$RadioInit$0$0 ==.
	C$Si100x_TxTone.c$393$1$26 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:393: U8 RadioInit(void)
;	-----------------------------------------
;	 function RadioInit
;	-----------------------------------------
_RadioInit:
	C$Si100x_TxTone.c$397$1$28 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:397: SDN = 0;
;	assignBit
	clr	_SDN
	C$Si100x_TxTone.c$399$1$28 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:399: delay(TWENTY_FIVE_MS);
	mov	dptr,#0x1458
	lcall	_delay
	C$Si100x_TxTone.c$401$1$28 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:401: status = phyRead(EZRADIOPRO_DEVICE_VERSION); // check version
	mov	dpl,#0x01
	lcall	_phyRead
	mov	r7,dpl
	C$Si100x_TxTone.c$402$1$28 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:402: if(status == 0xFF)
	cjne	r7,#0xff,00107$
	C$Si100x_TxTone.c$403$1$28 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:403: return RADIO_INIT_ERROR_SPI;
	mov	dpl,#0x02
	sjmp	00119$
00107$:
	C$Si100x_TxTone.c$404$1$28 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:404: else if (status == 0x00)
	mov	a,r7
	jnz	00104$
	C$Si100x_TxTone.c$405$1$28 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:405: return  RADIO_INIT_ERROR_SPI;
	mov	dpl,#0x02
	sjmp	00119$
00104$:
	C$Si100x_TxTone.c$406$1$28 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:406: else if (status < MIN_RADIO_VERSION)
	cjne	r7,#0x05,00148$
00148$:
	jnc	00108$
	C$Si100x_TxTone.c$407$1$28 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:407: return RADIO_INIT_ERROR_VERSION;
	mov	dpl,#0x03
	sjmp	00119$
00108$:
	C$Si100x_TxTone.c$409$1$28 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:409: status = phyRead(EZRADIOPRO_INTERRUPT_STATUS_2);
	mov	dpl,#0x04
	lcall	_phyRead
	C$Si100x_TxTone.c$411$1$28 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:411: if((status & EZRADIOPRO_IPOR)==0)
	mov	a,dpl
	mov	r7,a
	jb	acc.0,00117$
	C$Si100x_TxTone.c$414$2$29 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:414: return RadioInitSoftwareReset();
	lcall	_RadioInitSoftwareReset
	sjmp	00119$
00117$:
	C$Si100x_TxTone.c$416$1$28 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:416: else if((status & EZRADIOPRO_ICHIPRDY)==0)
	mov	a,r7
	jb	acc.1,00118$
	C$Si100x_TxTone.c$419$2$30 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:419: phyWrite(EZRADIOPRO_INTERRUPT_ENABLE_2, EZRADIOPRO_ENCHIPRDY);
	mov	_phyWrite_PARM_2,#0x02
	mov	dpl,#0x06
	lcall	_phyWrite
	C$Si100x_TxTone.c$422$2$30 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:422: SetTimeOut(TWO_MS);
	mov	dptr,#0x01a0
	lcall	_SetTimeOut
	C$Si100x_TxTone.c$423$2$30 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:423: while(IRQ)
00111$:
	jnb	_IRQ,00113$
	C$Si100x_TxTone.c$425$3$31 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:425: if(TIMEOUT_T0)
	jb	_ET0,00111$
	C$Si100x_TxTone.c$426$3$31 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:426: return RADIO_INIT_ERROR_XTAL;
	mov	dpl,#0x04
	sjmp	00119$
00113$:
	C$Si100x_TxTone.c$428$2$30 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:428: ClearTimeOut();
	lcall	_ClearTimeOut
00118$:
	C$Si100x_TxTone.c$430$1$28 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:430: return RADIO_INIT_SUCCESS; // success
	mov	dpl,#0x00
00119$:
	C$Si100x_TxTone.c$431$1$28 ==.
	XG$RadioInit$0$0 ==.
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'RadioInitSoftwareReset'
;------------------------------------------------------------
;status                    Allocated to registers 
;------------------------------------------------------------
	G$RadioInitSoftwareReset$0$0 ==.
	C$Si100x_TxTone.c$449$1$28 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:449: U8 RadioInitSoftwareReset(void)
;	-----------------------------------------
;	 function RadioInitSoftwareReset
;	-----------------------------------------
_RadioInitSoftwareReset:
	C$Si100x_TxTone.c$454$1$33 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:454: phyWrite(EZRADIOPRO_INTERRUPT_ENABLE_1, 0);
	mov	_phyWrite_PARM_2,#0x00
	mov	dpl,#0x05
	lcall	_phyWrite
	C$Si100x_TxTone.c$455$1$33 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:455: phyWrite(EZRADIOPRO_INTERRUPT_ENABLE_2, 0);
	mov	_phyWrite_PARM_2,#0x00
	mov	dpl,#0x06
	lcall	_phyWrite
	C$Si100x_TxTone.c$456$1$33 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:456: status = phyRead(EZRADIOPRO_INTERRUPT_STATUS_1);
	mov	dpl,#0x03
	lcall	_phyRead
	C$Si100x_TxTone.c$457$1$33 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:457: status = phyRead(EZRADIOPRO_INTERRUPT_STATUS_2);
	mov	dpl,#0x04
	lcall	_phyRead
	C$Si100x_TxTone.c$460$1$33 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:460: phyWrite(EZRADIOPRO_OPERATING_AND_FUNCTION_CONTROL_1, (EZRADIOPRO_SWRES|EZRADIOPRO_XTON));
	mov	_phyWrite_PARM_2,#0x81
	mov	dpl,#0x07
	lcall	_phyWrite
	C$Si100x_TxTone.c$463$1$33 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:463: SetTimeOut(TWO_MS);
	mov	dptr,#0x01a0
	lcall	_SetTimeOut
	C$Si100x_TxTone.c$464$1$33 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:464: while(IRQ)
00103$:
	jnb	_IRQ,00105$
	C$Si100x_TxTone.c$466$2$34 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:466: if(TIMEOUT_T0)
	jb	_ET0,00103$
	C$Si100x_TxTone.c$467$2$34 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:467: return RADIO_INIT_ERROR_NO_IRQ;
	mov	dpl,#0x01
	sjmp	00113$
00105$:
	C$Si100x_TxTone.c$469$1$33 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:469: ClearTimeOut();
	lcall	_ClearTimeOut
	C$Si100x_TxTone.c$471$1$33 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:471: status = phyRead(EZRADIOPRO_INTERRUPT_STATUS_2);
	mov	dpl,#0x04
	lcall	_phyRead
	mov	a,dpl
	C$Si100x_TxTone.c$473$1$33 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:473: if((status & EZRADIOPRO_ICHIPRDY)==0)
	jb	acc.1,00112$
	C$Si100x_TxTone.c$476$2$35 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:476: phyWrite(EZRADIOPRO_INTERRUPT_ENABLE_2, EZRADIOPRO_ENCHIPRDY);
	mov	_phyWrite_PARM_2,#0x02
	mov	dpl,#0x06
	lcall	_phyWrite
	C$Si100x_TxTone.c$479$2$35 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:479: SetTimeOut(TWO_MS);
	mov	dptr,#0x01a0
	lcall	_SetTimeOut
	C$Si100x_TxTone.c$480$2$35 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:480: while(IRQ)
00108$:
	jnb	_IRQ,00110$
	C$Si100x_TxTone.c$482$3$36 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:482: if(TIMEOUT_T0)
	jb	_ET0,00108$
	C$Si100x_TxTone.c$483$3$36 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:483: return RADIO_INIT_ERROR_XTAL;
	mov	dpl,#0x04
	sjmp	00113$
00110$:
	C$Si100x_TxTone.c$485$2$35 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:485: ClearTimeOut();
	lcall	_ClearTimeOut
00112$:
	C$Si100x_TxTone.c$488$1$33 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:488: return RADIO_INIT_SUCCESS; // success
	mov	dpl,#0x00
00113$:
	C$Si100x_TxTone.c$489$1$33 ==.
	XG$RadioInitSoftwareReset$0$0 ==.
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'delay'
;------------------------------------------------------------
;ticks                     Allocated to registers r6 r7 
;reload                    Allocated with name '_delay_reload_1_38'
;------------------------------------------------------------
	G$delay$0$0 ==.
	C$Si100x_TxTone.c$499$1$33 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:499: void delay (U16 ticks)
;	-----------------------------------------
;	 function delay
;	-----------------------------------------
_delay:
	mov	r6,dpl
	mov	r7,dph
	C$Si100x_TxTone.c$503$1$38 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:503: reload.U16 = -ticks;
	clr	c
	clr	a
	subb	a,r6
	mov	r6,a
	clr	a
	subb	a,r7
	mov	r7,a
	mov	(_delay_reload_1_38 + 0),r6
	mov	(_delay_reload_1_38 + 1),r7
	C$Si100x_TxTone.c$505$1$38 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:505: TR0 = 0;
;	assignBit
	clr	_TR0
	C$Si100x_TxTone.c$506$1$38 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:506: TF0 = 0;
;	assignBit
	clr	_TF0
	C$Si100x_TxTone.c$508$1$38 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:508: TMOD      = 0x01;    // T0 uses prescaler
	mov	_TMOD,#0x01
	C$Si100x_TxTone.c$509$1$38 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:509: CKCON     = 0x02;    // divide by 48 prescaler
	mov	_CKCON,#0x02
	C$Si100x_TxTone.c$511$1$38 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:511: TL0     = reload.U8[LSB];
	mov	_TL0,_delay_reload_1_38
	C$Si100x_TxTone.c$512$1$38 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:512: TH0     = reload.U8[MSB];
	mov	_TH0,(_delay_reload_1_38 + 0x0001)
	C$Si100x_TxTone.c$514$1$38 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:514: TR0 = 1;
;	assignBit
	setb	_TR0
	C$Si100x_TxTone.c$515$1$38 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:515: while (!TF0);
00101$:
	jnb	_TF0,00101$
	C$Si100x_TxTone.c$516$1$38 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:516: TR0 = 0;
;	assignBit
	clr	_TR0
	C$Si100x_TxTone.c$517$1$38 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:517: TF0 = 0;
;	assignBit
	clr	_TF0
	C$Si100x_TxTone.c$518$1$38 ==.
	XG$delay$0$0 ==.
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'SetTimeOut'
;------------------------------------------------------------
;ticks                     Allocated to registers r6 r7 
;reload                    Allocated with name '_SetTimeOut_reload_1_40'
;------------------------------------------------------------
	G$SetTimeOut$0$0 ==.
	C$Si100x_TxTone.c$529$1$38 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:529: void SetTimeOut (U16 ticks)
;	-----------------------------------------
;	 function SetTimeOut
;	-----------------------------------------
_SetTimeOut:
	mov	r6,dpl
	mov	r7,dph
	C$Si100x_TxTone.c$533$1$40 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:533: reload.U16 = -ticks;
	clr	c
	clr	a
	subb	a,r6
	mov	r6,a
	clr	a
	subb	a,r7
	mov	r7,a
	mov	(_SetTimeOut_reload_1_40 + 0),r6
	mov	(_SetTimeOut_reload_1_40 + 1),r7
	C$Si100x_TxTone.c$535$1$40 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:535: TR0 = 0;
;	assignBit
	clr	_TR0
	C$Si100x_TxTone.c$536$1$40 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:536: TF0 = 0;
;	assignBit
	clr	_TF0
	C$Si100x_TxTone.c$538$1$40 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:538: TMOD      = 0x01;    // T0 uses prescaler
	mov	_TMOD,#0x01
	C$Si100x_TxTone.c$539$1$40 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:539: CKCON     = 0x02;    // divide by 48 prescaler
	mov	_CKCON,#0x02
	C$Si100x_TxTone.c$541$1$40 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:541: TL0     = reload.U8[LSB];
	mov	_TL0,_SetTimeOut_reload_1_40
	C$Si100x_TxTone.c$542$1$40 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:542: TH0     = reload.U8[MSB];
	mov	_TH0,(_SetTimeOut_reload_1_40 + 0x0001)
	C$Si100x_TxTone.c$544$1$40 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:544: TR0 = 1;
;	assignBit
	setb	_TR0
	C$Si100x_TxTone.c$545$1$40 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:545: ET0 = 1;
;	assignBit
	setb	_ET0
	C$Si100x_TxTone.c$546$1$40 ==.
	XG$SetTimeOut$0$0 ==.
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'T0_ISR'
;------------------------------------------------------------
	G$T0_ISR$0$0 ==.
	C$Si100x_TxTone.c$558$1$40 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:558: INTERRUPT(T0_ISR, INTERRUPT_TIMER0)
;	-----------------------------------------
;	 function T0_ISR
;	-----------------------------------------
_T0_ISR:
	C$Si100x_TxTone.c$560$1$42 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:560: ET0 = 0;
;	assignBit
	clr	_ET0
	C$Si100x_TxTone.c$561$1$42 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:561: TF0 = 0;
;	assignBit
	clr	_TF0
	C$Si100x_TxTone.c$562$1$42 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:562: TR0 = 0;
;	assignBit
	clr	_TR0
	C$Si100x_TxTone.c$563$1$42 ==.
	XG$T0_ISR$0$0 ==.
	reti
;	eliminated unneeded mov psw,# (no regs used in bank)
;	eliminated unneeded push/pop psw
;	eliminated unneeded push/pop dpl
;	eliminated unneeded push/pop dph
;	eliminated unneeded push/pop b
;	eliminated unneeded push/pop acc
;------------------------------------------------------------
;Allocation info for local variables in function 'ClearTimeOut'
;------------------------------------------------------------
	G$ClearTimeOut$0$0 ==.
	C$Si100x_TxTone.c$572$1$42 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:572: void ClearTimeOut (void)
;	-----------------------------------------
;	 function ClearTimeOut
;	-----------------------------------------
_ClearTimeOut:
	C$Si100x_TxTone.c$574$1$44 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:574: ET0 = 0;
;	assignBit
	clr	_ET0
	C$Si100x_TxTone.c$575$1$44 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:575: TF0 = 0;
;	assignBit
	clr	_TF0
	C$Si100x_TxTone.c$576$1$44 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:576: TR0 = 0;
;	assignBit
	clr	_TR0
	C$Si100x_TxTone.c$577$1$44 ==.
	XG$ClearTimeOut$0$0 ==.
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'setTRxFrequency'
;------------------------------------------------------------
;frequency                 Allocated with name '_setTRxFrequency_frequency_1_45'
;frequencyBandSelect       Allocated to registers r2 
;nominalCarrierFrequency   Allocated with name '_setTRxFrequency_nominalCarrierFrequency_1_46'
;------------------------------------------------------------
	G$setTRxFrequency$0$0 ==.
	C$Si100x_TxTone.c$587$1$44 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:587: void   setTRxFrequency (U32 frequency)
;	-----------------------------------------
;	 function setTRxFrequency
;	-----------------------------------------
_setTRxFrequency:
	mov	_setTRxFrequency_frequency_1_45,dpl
	mov	(_setTRxFrequency_frequency_1_45 + 1),dph
	mov	(_setTRxFrequency_frequency_1_45 + 2),b
	mov	(_setTRxFrequency_frequency_1_45 + 3),a
	C$Si100x_TxTone.c$592$1$46 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:592: if (frequency > 480000000L )
	clr	c
	clr	a
	subb	a,_setTRxFrequency_frequency_1_45
	mov	a,#0x38
	subb	a,(_setTRxFrequency_frequency_1_45 + 1)
	mov	a,#0x9c
	subb	a,(_setTRxFrequency_frequency_1_45 + 2)
	mov	a,#0x1c
	subb	a,(_setTRxFrequency_frequency_1_45 + 3)
	jc	00109$
	ljmp	00102$
00109$:
	C$Si100x_TxTone.c$594$2$47 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:594: frequency -= 480000000L;
	mov	a,(_setTRxFrequency_frequency_1_45 + 1)
	add	a,#0xc8
	mov	(_setTRxFrequency_frequency_1_45 + 1),a
	mov	a,(_setTRxFrequency_frequency_1_45 + 2)
	addc	a,#0x63
	mov	(_setTRxFrequency_frequency_1_45 + 2),a
	mov	a,(_setTRxFrequency_frequency_1_45 + 3)
	addc	a,#0xe3
	mov	(_setTRxFrequency_frequency_1_45 + 3),a
	C$Si100x_TxTone.c$595$1$46 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:595: frequencyBandSelect  = frequency / 20000000L;
	mov	__divulong_PARM_2,#0x00
	mov	(__divulong_PARM_2 + 1),#0x2d
	mov	(__divulong_PARM_2 + 2),#0x31
	mov	(__divulong_PARM_2 + 3),#0x01
	mov	dpl,_setTRxFrequency_frequency_1_45
	mov	dph,(_setTRxFrequency_frequency_1_45 + 1)
	mov	b,(_setTRxFrequency_frequency_1_45 + 2)
	mov	a,(_setTRxFrequency_frequency_1_45 + 3)
	lcall	__divulong
	mov	r2,dpl
	C$Si100x_TxTone.c$596$2$47 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:596: frequency -= (U32)frequencyBandSelect * 20000000L;
	mov	__mullong_PARM_2,r2
	mov	(__mullong_PARM_2 + 1),#0x00
	mov	(__mullong_PARM_2 + 2),#0x00
	mov	(__mullong_PARM_2 + 3),#0x00
	mov	dptr,#0x2d00
	mov	b,#0x31
	mov	a,#0x01
	push	ar2
	lcall	__mullong
	mov	r4,dpl
	mov	r5,dph
	mov	r6,b
	mov	r7,a
	mov	a,_setTRxFrequency_frequency_1_45
	clr	c
	subb	a,r4
	mov	_setTRxFrequency_frequency_1_45,a
	mov	a,(_setTRxFrequency_frequency_1_45 + 1)
	subb	a,r5
	mov	(_setTRxFrequency_frequency_1_45 + 1),a
	mov	a,(_setTRxFrequency_frequency_1_45 + 2)
	subb	a,r6
	mov	(_setTRxFrequency_frequency_1_45 + 2),a
	mov	a,(_setTRxFrequency_frequency_1_45 + 3)
	subb	a,r7
	mov	(_setTRxFrequency_frequency_1_45 + 3),a
	C$Si100x_TxTone.c$597$2$47 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:597: frequency  = divideWithRounding(frequency, 625);
	mov	_divideWithRounding_PARM_2,#0x71
	mov	(_divideWithRounding_PARM_2 + 1),#0x02
	clr	a
	mov	(_divideWithRounding_PARM_2 + 2),a
	mov	(_divideWithRounding_PARM_2 + 3),a
	mov	dpl,_setTRxFrequency_frequency_1_45
	mov	dph,(_setTRxFrequency_frequency_1_45 + 1)
	mov	b,(_setTRxFrequency_frequency_1_45 + 2)
	mov	a,(_setTRxFrequency_frequency_1_45 + 3)
	lcall	_divideWithRounding
	mov	_setTRxFrequency_frequency_1_45,dpl
	mov	(_setTRxFrequency_frequency_1_45 + 1),dph
	mov	(_setTRxFrequency_frequency_1_45 + 2),b
	mov	(_setTRxFrequency_frequency_1_45 + 3),a
	pop	ar2
	C$Si100x_TxTone.c$598$2$47 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:598: frequency <<= 1;
	mov	a,_setTRxFrequency_frequency_1_45
	add	a,_setTRxFrequency_frequency_1_45
	mov	_setTRxFrequency_frequency_1_45,a
	mov	a,(_setTRxFrequency_frequency_1_45 + 1)
	rlc	a
	mov	(_setTRxFrequency_frequency_1_45 + 1),a
	mov	a,(_setTRxFrequency_frequency_1_45 + 2)
	rlc	a
	mov	(_setTRxFrequency_frequency_1_45 + 2),a
	mov	a,(_setTRxFrequency_frequency_1_45 + 3)
	rlc	a
	mov	(_setTRxFrequency_frequency_1_45 + 3),a
	C$Si100x_TxTone.c$599$2$47 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:599: frequencyBandSelect |= 0x20;
	orl	ar2,#0x20
	ljmp	00103$
00102$:
	C$Si100x_TxTone.c$603$2$48 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:603: frequency -= 240000000L;
	mov	a,(_setTRxFrequency_frequency_1_45 + 1)
	add	a,#0xe4
	mov	(_setTRxFrequency_frequency_1_45 + 1),a
	mov	a,(_setTRxFrequency_frequency_1_45 + 2)
	addc	a,#0xb1
	mov	(_setTRxFrequency_frequency_1_45 + 2),a
	mov	a,(_setTRxFrequency_frequency_1_45 + 3)
	addc	a,#0xf1
	mov	(_setTRxFrequency_frequency_1_45 + 3),a
	C$Si100x_TxTone.c$604$1$46 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:604: frequencyBandSelect  = frequency / 10000000L;
	mov	__divulong_PARM_2,#0x80
	mov	(__divulong_PARM_2 + 1),#0x96
	mov	(__divulong_PARM_2 + 2),#0x98
	mov	(__divulong_PARM_2 + 3),#0x00
	mov	dpl,_setTRxFrequency_frequency_1_45
	mov	dph,(_setTRxFrequency_frequency_1_45 + 1)
	mov	b,(_setTRxFrequency_frequency_1_45 + 2)
	mov	a,(_setTRxFrequency_frequency_1_45 + 3)
	lcall	__divulong
	mov	r4,dpl
	mov	ar2,r4
	C$Si100x_TxTone.c$605$2$48 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:605: frequency -= (U32)frequencyBandSelect * 10000000L;
	mov	__mullong_PARM_2,r2
	mov	(__mullong_PARM_2 + 1),#0x00
	mov	(__mullong_PARM_2 + 2),#0x00
	mov	(__mullong_PARM_2 + 3),#0x00
	mov	dptr,#0x9680
	mov	b,#0x98
	clr	a
	push	ar2
	lcall	__mullong
	mov	r4,dpl
	mov	r5,dph
	mov	r6,b
	mov	r7,a
	mov	a,_setTRxFrequency_frequency_1_45
	clr	c
	subb	a,r4
	mov	_setTRxFrequency_frequency_1_45,a
	mov	a,(_setTRxFrequency_frequency_1_45 + 1)
	subb	a,r5
	mov	(_setTRxFrequency_frequency_1_45 + 1),a
	mov	a,(_setTRxFrequency_frequency_1_45 + 2)
	subb	a,r6
	mov	(_setTRxFrequency_frequency_1_45 + 2),a
	mov	a,(_setTRxFrequency_frequency_1_45 + 3)
	subb	a,r7
	mov	(_setTRxFrequency_frequency_1_45 + 3),a
	C$Si100x_TxTone.c$606$2$48 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:606: frequency  = divideWithRounding(frequency, 625);
	mov	_divideWithRounding_PARM_2,#0x71
	mov	(_divideWithRounding_PARM_2 + 1),#0x02
	clr	a
	mov	(_divideWithRounding_PARM_2 + 2),a
	mov	(_divideWithRounding_PARM_2 + 3),a
	mov	dpl,_setTRxFrequency_frequency_1_45
	mov	dph,(_setTRxFrequency_frequency_1_45 + 1)
	mov	b,(_setTRxFrequency_frequency_1_45 + 2)
	mov	a,(_setTRxFrequency_frequency_1_45 + 3)
	lcall	_divideWithRounding
	mov	_setTRxFrequency_frequency_1_45,dpl
	mov	(_setTRxFrequency_frequency_1_45 + 1),dph
	mov	(_setTRxFrequency_frequency_1_45 + 2),b
	mov	(_setTRxFrequency_frequency_1_45 + 3),a
	pop	ar2
	C$Si100x_TxTone.c$607$2$48 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:607: frequency <<= 2;
	mov	a,_setTRxFrequency_frequency_1_45
	add	a,_setTRxFrequency_frequency_1_45
	mov	_setTRxFrequency_frequency_1_45,a
	mov	a,(_setTRxFrequency_frequency_1_45 + 1)
	rlc	a
	mov	(_setTRxFrequency_frequency_1_45 + 1),a
	mov	a,(_setTRxFrequency_frequency_1_45 + 2)
	rlc	a
	mov	(_setTRxFrequency_frequency_1_45 + 2),a
	mov	a,(_setTRxFrequency_frequency_1_45 + 3)
	rlc	a
	mov	(_setTRxFrequency_frequency_1_45 + 3),a
	mov	a,_setTRxFrequency_frequency_1_45
	add	a,_setTRxFrequency_frequency_1_45
	mov	_setTRxFrequency_frequency_1_45,a
	mov	a,(_setTRxFrequency_frequency_1_45 + 1)
	rlc	a
	mov	(_setTRxFrequency_frequency_1_45 + 1),a
	mov	a,(_setTRxFrequency_frequency_1_45 + 2)
	rlc	a
	mov	(_setTRxFrequency_frequency_1_45 + 2),a
	mov	a,(_setTRxFrequency_frequency_1_45 + 3)
	rlc	a
	mov	(_setTRxFrequency_frequency_1_45 + 3),a
00103$:
	C$Si100x_TxTone.c$610$1$46 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:610: frequencyBandSelect |= 0x40;        // set sbsel
	mov	a,#0x40
	orl	a,r2
	mov	_phyWrite_PARM_2,a
	C$Si100x_TxTone.c$612$1$46 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:612: nominalCarrierFrequency.U16 = (U16)frequency;
	mov	r4,_setTRxFrequency_frequency_1_45
	mov	r5,(_setTRxFrequency_frequency_1_45 + 1)
	mov	(_setTRxFrequency_nominalCarrierFrequency_1_46 + 0),r4
	mov	(_setTRxFrequency_nominalCarrierFrequency_1_46 + 1),r5
	C$Si100x_TxTone.c$614$1$46 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:614: phyWrite(EZRADIOPRO_FREQUENCY_BAND_SELECT, frequencyBandSelect);
	mov	dpl,#0x75
	lcall	_phyWrite
	C$Si100x_TxTone.c$615$1$46 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:615: phyWrite(EZRADIOPRO_NOMINAL_CARRIER_FREQUENCY_1, nominalCarrierFrequency.U8[MSB]);
	mov	_phyWrite_PARM_2,(_setTRxFrequency_nominalCarrierFrequency_1_46 + 0x0001)
	mov	dpl,#0x76
	lcall	_phyWrite
	C$Si100x_TxTone.c$616$1$46 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:616: phyWrite(EZRADIOPRO_NOMINAL_CARRIER_FREQUENCY_0, nominalCarrierFrequency.U8[LSB]);
	mov	_phyWrite_PARM_2,_setTRxFrequency_nominalCarrierFrequency_1_46
	mov	dpl,#0x77
	lcall	_phyWrite
	C$Si100x_TxTone.c$618$1$46 ==.
	XG$setTRxFrequency$0$0 ==.
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'divideWithRounding'
;------------------------------------------------------------
;divisor                   Allocated with name '_divideWithRounding_PARM_2'
;value                     Allocated to registers r4 r5 r6 r7 
;------------------------------------------------------------
	G$divideWithRounding$0$0 ==.
	C$Si100x_TxTone.c$628$1$46 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:628: U32   divideWithRounding (U32 value, U32 divisor)
;	-----------------------------------------
;	 function divideWithRounding
;	-----------------------------------------
_divideWithRounding:
	mov	r4,dpl
	mov	r5,dph
	mov	r6,b
	mov	r7,a
	C$Si100x_TxTone.c$630$1$50 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:630: value += (divisor >> 1);
	mov	a,(_divideWithRounding_PARM_2 + 3)
	clr	c
	rrc	a
	mov	r3,a
	mov	a,(_divideWithRounding_PARM_2 + 2)
	rrc	a
	mov	r2,a
	mov	a,(_divideWithRounding_PARM_2 + 1)
	rrc	a
	mov	r1,a
	mov	a,_divideWithRounding_PARM_2
	rrc	a
	add	a,r4
	mov	r4,a
	mov	a,r1
	addc	a,r5
	mov	r5,a
	mov	a,r2
	addc	a,r6
	mov	r6,a
	mov	a,r3
	addc	a,r7
	mov	r7,a
	C$Si100x_TxTone.c$631$1$50 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:631: value /= divisor;
	mov	__divulong_PARM_2,_divideWithRounding_PARM_2
	mov	(__divulong_PARM_2 + 1),(_divideWithRounding_PARM_2 + 1)
	mov	(__divulong_PARM_2 + 2),(_divideWithRounding_PARM_2 + 2)
	mov	(__divulong_PARM_2 + 3),(_divideWithRounding_PARM_2 + 3)
	mov	dpl,r4
	mov	dph,r5
	mov	b,r6
	mov	a,r7
	lcall	__divulong
	C$Si100x_TxTone.c$632$1$50 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:632: return value;
	C$Si100x_TxTone.c$633$1$50 ==.
	XG$divideWithRounding$0$0 ==.
	ret
;------------------------------------------------------------
;Allocation info for local variables in function 'setTxPower'
;------------------------------------------------------------
;power                     Allocated to registers r7 
;------------------------------------------------------------
	G$setTxPower$0$0 ==.
	C$Si100x_TxTone.c$643$1$50 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:643: void setTxPower (U8 power)
;	-----------------------------------------
;	 function setTxPower
;	-----------------------------------------
_setTxPower:
	mov	r7,dpl
	C$Si100x_TxTone.c$645$1$52 ==.
;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:645: phyWrite(EZRADIOPRO_TX_POWER, 0x18|power); // set bit 3 and 4
	mov	a,#0x18
	orl	a,r7
	mov	_phyWrite_PARM_2,a
	mov	dpl,#0x6d
	lcall	_phyWrite
	C$Si100x_TxTone.c$646$1$52 ==.
	XG$setTxPower$0$0 ==.
	ret
	.area CSEG    (CODE)
	.area CONST   (CODE)
	.area XINIT   (CODE)
	.area CABS    (ABS,CODE)

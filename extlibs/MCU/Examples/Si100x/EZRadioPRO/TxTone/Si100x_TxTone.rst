                                      1 ;--------------------------------------------------------
                                      2 ; File Created by SDCC : free open source ANSI-C Compiler
                                      3 ; Version 3.6.8 #9947 (MINGW32)
                                      4 ;--------------------------------------------------------
                                      5 	.module Si100x_TxTone
                                      6 	.optsdcc -mmcs51 --model-small
                                      7 	
                                      8 ;--------------------------------------------------------
                                      9 ; Public variables in this module
                                     10 ;--------------------------------------------------------
                                     11 	.globl _phyWrite_PARM_2
                                     12 	.globl _T0_ISR
                                     13 	.globl _main
                                     14 	.globl _LED
                                     15 	.globl _SDN
                                     16 	.globl _IRQ
                                     17 	.globl _NSS1
                                     18 	.globl _SPI0EN
                                     19 	.globl _TXBMT0
                                     20 	.globl _NSS0MD0
                                     21 	.globl _NSS0MD1
                                     22 	.globl _RXOVRN0
                                     23 	.globl _MODF0
                                     24 	.globl _WCOL0
                                     25 	.globl _SPIF0
                                     26 	.globl _AD0CM0
                                     27 	.globl _AD0CM1
                                     28 	.globl _AD0CM2
                                     29 	.globl _AD0WINT
                                     30 	.globl _AD0BUSY
                                     31 	.globl _AD0INT
                                     32 	.globl _BURSTEN
                                     33 	.globl _AD0EN
                                     34 	.globl _CCF0
                                     35 	.globl _CCF1
                                     36 	.globl _CCF2
                                     37 	.globl _CCF3
                                     38 	.globl _CCF4
                                     39 	.globl _CCF5
                                     40 	.globl _CR
                                     41 	.globl _CF
                                     42 	.globl _P
                                     43 	.globl _F1
                                     44 	.globl _OV
                                     45 	.globl _RS0
                                     46 	.globl _RS1
                                     47 	.globl _F0
                                     48 	.globl _AC
                                     49 	.globl _CY
                                     50 	.globl _T2XCLK
                                     51 	.globl _T2RCLK
                                     52 	.globl _TR2
                                     53 	.globl _T2SPLIT
                                     54 	.globl _TF2CEN
                                     55 	.globl _TF2LEN
                                     56 	.globl _TF2L
                                     57 	.globl _TF2H
                                     58 	.globl _SI
                                     59 	.globl _ACK
                                     60 	.globl _ARBLOST
                                     61 	.globl _ACKRQ
                                     62 	.globl _STO
                                     63 	.globl _STA
                                     64 	.globl _TXMODE
                                     65 	.globl _MASTER
                                     66 	.globl _PX0
                                     67 	.globl _PT0
                                     68 	.globl _PX1
                                     69 	.globl _PT1
                                     70 	.globl _PS0
                                     71 	.globl _PT2
                                     72 	.globl _PSPI0
                                     73 	.globl _SPI1EN
                                     74 	.globl _TXBMT1
                                     75 	.globl _NSS1MD0
                                     76 	.globl _NSS1MD1
                                     77 	.globl _RXOVRN1
                                     78 	.globl _MODF1
                                     79 	.globl _WCOL1
                                     80 	.globl _SPIF1
                                     81 	.globl _EX0
                                     82 	.globl _ET0
                                     83 	.globl _EX1
                                     84 	.globl _ET1
                                     85 	.globl _ES0
                                     86 	.globl _ET2
                                     87 	.globl _ESPI0
                                     88 	.globl _EA
                                     89 	.globl _RI0
                                     90 	.globl _TI0
                                     91 	.globl _RB80
                                     92 	.globl _TB80
                                     93 	.globl _REN0
                                     94 	.globl _MCE0
                                     95 	.globl _S0MODE
                                     96 	.globl _IT0
                                     97 	.globl _IE0
                                     98 	.globl _IT1
                                     99 	.globl _IE1
                                    100 	.globl _TR0
                                    101 	.globl _TF0
                                    102 	.globl _TR1
                                    103 	.globl _TF1
                                    104 	.globl __XPAGE
                                    105 	.globl _PCA0CP4
                                    106 	.globl _PCA0CP0
                                    107 	.globl _PCA0
                                    108 	.globl _PCA0CP3
                                    109 	.globl _PCA0CP2
                                    110 	.globl _PCA0CP1
                                    111 	.globl _PCA0CP5
                                    112 	.globl _TMR2
                                    113 	.globl _TMR2RL
                                    114 	.globl _ADC0LT
                                    115 	.globl _ADC0GT
                                    116 	.globl _ADC0
                                    117 	.globl _TMR3
                                    118 	.globl _TMR3RL
                                    119 	.globl _TOFF
                                    120 	.globl _DP
                                    121 	.globl _VDM0CN
                                    122 	.globl _PCA0CPH4
                                    123 	.globl _PCA0CPL4
                                    124 	.globl _PCA0CPH0
                                    125 	.globl _PCA0CPL0
                                    126 	.globl _PCA0H
                                    127 	.globl _PCA0L
                                    128 	.globl _SPI0CN
                                    129 	.globl _EIP2
                                    130 	.globl _EIP1
                                    131 	.globl _SMB0ADM
                                    132 	.globl _SMB0ADR
                                    133 	.globl _P2MDIN
                                    134 	.globl _P1MDIN
                                    135 	.globl _P0MDIN
                                    136 	.globl _B
                                    137 	.globl _RSTSRC
                                    138 	.globl _PCA0CPH3
                                    139 	.globl _PCA0CPL3
                                    140 	.globl _PCA0CPH2
                                    141 	.globl _PCA0CPL2
                                    142 	.globl _PCA0CPH1
                                    143 	.globl _PCA0CPL1
                                    144 	.globl _ADC0CN
                                    145 	.globl _EIE2
                                    146 	.globl _EIE1
                                    147 	.globl _FLWR
                                    148 	.globl _IT01CF
                                    149 	.globl _XBR2
                                    150 	.globl _XBR1
                                    151 	.globl _XBR0
                                    152 	.globl _ACC
                                    153 	.globl _PCA0PWM
                                    154 	.globl _PCA0CPM4
                                    155 	.globl _PCA0CPM3
                                    156 	.globl _PCA0CPM2
                                    157 	.globl _PCA0CPM1
                                    158 	.globl _PCA0CPM0
                                    159 	.globl _PCA0MD
                                    160 	.globl _PCA0CN
                                    161 	.globl _P0MAT
                                    162 	.globl _P2SKIP
                                    163 	.globl _P1SKIP
                                    164 	.globl _P0SKIP
                                    165 	.globl _PCA0CPH5
                                    166 	.globl _PCA0CPL5
                                    167 	.globl _REF0CN
                                    168 	.globl _PSW
                                    169 	.globl _P1MAT
                                    170 	.globl _PCA0CPM5
                                    171 	.globl _TMR2H
                                    172 	.globl _TMR2L
                                    173 	.globl _TMR2RLH
                                    174 	.globl _TMR2RLL
                                    175 	.globl _REG0CN
                                    176 	.globl _TMR2CN
                                    177 	.globl _P0MASK
                                    178 	.globl _ADC0LTH
                                    179 	.globl _ADC0LTL
                                    180 	.globl _ADC0GTH
                                    181 	.globl _ADC0GTL
                                    182 	.globl _SMB0DAT
                                    183 	.globl _SMB0CF
                                    184 	.globl _SMB0CN
                                    185 	.globl _P1MASK
                                    186 	.globl _ADC0H
                                    187 	.globl _ADC0L
                                    188 	.globl _ADC0TK
                                    189 	.globl _ADC0CF
                                    190 	.globl _ADC0MX
                                    191 	.globl _ADC0PWR
                                    192 	.globl _ADC0AC
                                    193 	.globl _IREF0CN
                                    194 	.globl _IP
                                    195 	.globl _FLKEY
                                    196 	.globl _FLSCL
                                    197 	.globl _PMU0CF
                                    198 	.globl _OSCICL
                                    199 	.globl _OSCICN
                                    200 	.globl _OSCXCN
                                    201 	.globl _SPI1CN
                                    202 	.globl _ONESHOT
                                    203 	.globl _EMI0TC
                                    204 	.globl _RTC0KEY
                                    205 	.globl _RTC0DAT
                                    206 	.globl _RTC0ADR
                                    207 	.globl _EMI0CF
                                    208 	.globl _EMI0CN
                                    209 	.globl _CLKSEL
                                    210 	.globl _IE
                                    211 	.globl _SFRPAGE
                                    212 	.globl _P2DRV
                                    213 	.globl _P2MDOUT
                                    214 	.globl _P1DRV
                                    215 	.globl _P1MDOUT
                                    216 	.globl _P0DRV
                                    217 	.globl _P0MDOUT
                                    218 	.globl _SPI0DAT
                                    219 	.globl _SPI0CKR
                                    220 	.globl _SPI0CFG
                                    221 	.globl _P2
                                    222 	.globl _CPT0MX
                                    223 	.globl _CPT1MX
                                    224 	.globl _CPT0MD
                                    225 	.globl _CPT1MD
                                    226 	.globl _CPT0CN
                                    227 	.globl _CPT1CN
                                    228 	.globl _SBUF0
                                    229 	.globl _SCON0
                                    230 	.globl _CRC0CNT
                                    231 	.globl _DC0CN
                                    232 	.globl _CRC0AUTO
                                    233 	.globl _DC0CF
                                    234 	.globl _TMR3H
                                    235 	.globl _CRC0FLIP
                                    236 	.globl _TMR3L
                                    237 	.globl _CRC0IN
                                    238 	.globl _TMR3RLH
                                    239 	.globl _CRC0CN
                                    240 	.globl _TMR3RLL
                                    241 	.globl _CRC0DAT
                                    242 	.globl _TMR3CN
                                    243 	.globl _P1
                                    244 	.globl _PSCTL
                                    245 	.globl _CKCON
                                    246 	.globl _TH1
                                    247 	.globl _TH0
                                    248 	.globl _TL1
                                    249 	.globl _TL0
                                    250 	.globl _TMOD
                                    251 	.globl _TCON
                                    252 	.globl _PCON
                                    253 	.globl _TOFFH
                                    254 	.globl _SPI1DAT
                                    255 	.globl _TOFFL
                                    256 	.globl _SPI1CKR
                                    257 	.globl _SPI1CFG
                                    258 	.globl _DPH
                                    259 	.globl _DPL
                                    260 	.globl _SP
                                    261 	.globl _P0
                                    262 	.globl _divideWithRounding_PARM_2
                                    263 	.globl _RadioStatus
                                    264 	.globl _PortInit
                                    265 	.globl _SYSCLK_Init
                                    266 	.globl _SPI1_Init
                                    267 	.globl _phyWrite
                                    268 	.globl _phyRead
                                    269 	.globl _RadioInit
                                    270 	.globl _RadioInitSoftwareReset
                                    271 	.globl _delay
                                    272 	.globl _SetTimeOut
                                    273 	.globl _ClearTimeOut
                                    274 	.globl _setTRxFrequency
                                    275 	.globl _divideWithRounding
                                    276 	.globl _setTxPower
                                    277 ;--------------------------------------------------------
                                    278 ; special function registers
                                    279 ;--------------------------------------------------------
                                    280 	.area RSEG    (ABS,DATA)
      000000                        281 	.org 0x0000
                           000080   282 G$P0$0$0 == 0x0080
                           000080   283 _P0	=	0x0080
                           000081   284 G$SP$0$0 == 0x0081
                           000081   285 _SP	=	0x0081
                           000082   286 G$DPL$0$0 == 0x0082
                           000082   287 _DPL	=	0x0082
                           000083   288 G$DPH$0$0 == 0x0083
                           000083   289 _DPH	=	0x0083
                           000084   290 G$SPI1CFG$0$0 == 0x0084
                           000084   291 _SPI1CFG	=	0x0084
                           000085   292 G$SPI1CKR$0$0 == 0x0085
                           000085   293 _SPI1CKR	=	0x0085
                           000085   294 G$TOFFL$0$0 == 0x0085
                           000085   295 _TOFFL	=	0x0085
                           000086   296 G$SPI1DAT$0$0 == 0x0086
                           000086   297 _SPI1DAT	=	0x0086
                           000086   298 G$TOFFH$0$0 == 0x0086
                           000086   299 _TOFFH	=	0x0086
                           000087   300 G$PCON$0$0 == 0x0087
                           000087   301 _PCON	=	0x0087
                           000088   302 G$TCON$0$0 == 0x0088
                           000088   303 _TCON	=	0x0088
                           000089   304 G$TMOD$0$0 == 0x0089
                           000089   305 _TMOD	=	0x0089
                           00008A   306 G$TL0$0$0 == 0x008a
                           00008A   307 _TL0	=	0x008a
                           00008B   308 G$TL1$0$0 == 0x008b
                           00008B   309 _TL1	=	0x008b
                           00008C   310 G$TH0$0$0 == 0x008c
                           00008C   311 _TH0	=	0x008c
                           00008D   312 G$TH1$0$0 == 0x008d
                           00008D   313 _TH1	=	0x008d
                           00008E   314 G$CKCON$0$0 == 0x008e
                           00008E   315 _CKCON	=	0x008e
                           00008F   316 G$PSCTL$0$0 == 0x008f
                           00008F   317 _PSCTL	=	0x008f
                           000090   318 G$P1$0$0 == 0x0090
                           000090   319 _P1	=	0x0090
                           000091   320 G$TMR3CN$0$0 == 0x0091
                           000091   321 _TMR3CN	=	0x0091
                           000091   322 G$CRC0DAT$0$0 == 0x0091
                           000091   323 _CRC0DAT	=	0x0091
                           000092   324 G$TMR3RLL$0$0 == 0x0092
                           000092   325 _TMR3RLL	=	0x0092
                           000092   326 G$CRC0CN$0$0 == 0x0092
                           000092   327 _CRC0CN	=	0x0092
                           000093   328 G$TMR3RLH$0$0 == 0x0093
                           000093   329 _TMR3RLH	=	0x0093
                           000093   330 G$CRC0IN$0$0 == 0x0093
                           000093   331 _CRC0IN	=	0x0093
                           000094   332 G$TMR3L$0$0 == 0x0094
                           000094   333 _TMR3L	=	0x0094
                           000095   334 G$CRC0FLIP$0$0 == 0x0095
                           000095   335 _CRC0FLIP	=	0x0095
                           000095   336 G$TMR3H$0$0 == 0x0095
                           000095   337 _TMR3H	=	0x0095
                           000096   338 G$DC0CF$0$0 == 0x0096
                           000096   339 _DC0CF	=	0x0096
                           000096   340 G$CRC0AUTO$0$0 == 0x0096
                           000096   341 _CRC0AUTO	=	0x0096
                           000097   342 G$DC0CN$0$0 == 0x0097
                           000097   343 _DC0CN	=	0x0097
                           000097   344 G$CRC0CNT$0$0 == 0x0097
                           000097   345 _CRC0CNT	=	0x0097
                           000098   346 G$SCON0$0$0 == 0x0098
                           000098   347 _SCON0	=	0x0098
                           000099   348 G$SBUF0$0$0 == 0x0099
                           000099   349 _SBUF0	=	0x0099
                           00009A   350 G$CPT1CN$0$0 == 0x009a
                           00009A   351 _CPT1CN	=	0x009a
                           00009B   352 G$CPT0CN$0$0 == 0x009b
                           00009B   353 _CPT0CN	=	0x009b
                           00009C   354 G$CPT1MD$0$0 == 0x009c
                           00009C   355 _CPT1MD	=	0x009c
                           00009D   356 G$CPT0MD$0$0 == 0x009d
                           00009D   357 _CPT0MD	=	0x009d
                           00009E   358 G$CPT1MX$0$0 == 0x009e
                           00009E   359 _CPT1MX	=	0x009e
                           00009F   360 G$CPT0MX$0$0 == 0x009f
                           00009F   361 _CPT0MX	=	0x009f
                           0000A0   362 G$P2$0$0 == 0x00a0
                           0000A0   363 _P2	=	0x00a0
                           0000A1   364 G$SPI0CFG$0$0 == 0x00a1
                           0000A1   365 _SPI0CFG	=	0x00a1
                           0000A2   366 G$SPI0CKR$0$0 == 0x00a2
                           0000A2   367 _SPI0CKR	=	0x00a2
                           0000A3   368 G$SPI0DAT$0$0 == 0x00a3
                           0000A3   369 _SPI0DAT	=	0x00a3
                           0000A4   370 G$P0MDOUT$0$0 == 0x00a4
                           0000A4   371 _P0MDOUT	=	0x00a4
                           0000A4   372 G$P0DRV$0$0 == 0x00a4
                           0000A4   373 _P0DRV	=	0x00a4
                           0000A5   374 G$P1MDOUT$0$0 == 0x00a5
                           0000A5   375 _P1MDOUT	=	0x00a5
                           0000A5   376 G$P1DRV$0$0 == 0x00a5
                           0000A5   377 _P1DRV	=	0x00a5
                           0000A6   378 G$P2MDOUT$0$0 == 0x00a6
                           0000A6   379 _P2MDOUT	=	0x00a6
                           0000A6   380 G$P2DRV$0$0 == 0x00a6
                           0000A6   381 _P2DRV	=	0x00a6
                           0000A7   382 G$SFRPAGE$0$0 == 0x00a7
                           0000A7   383 _SFRPAGE	=	0x00a7
                           0000A8   384 G$IE$0$0 == 0x00a8
                           0000A8   385 _IE	=	0x00a8
                           0000A9   386 G$CLKSEL$0$0 == 0x00a9
                           0000A9   387 _CLKSEL	=	0x00a9
                           0000AA   388 G$EMI0CN$0$0 == 0x00aa
                           0000AA   389 _EMI0CN	=	0x00aa
                           0000AB   390 G$EMI0CF$0$0 == 0x00ab
                           0000AB   391 _EMI0CF	=	0x00ab
                           0000AC   392 G$RTC0ADR$0$0 == 0x00ac
                           0000AC   393 _RTC0ADR	=	0x00ac
                           0000AD   394 G$RTC0DAT$0$0 == 0x00ad
                           0000AD   395 _RTC0DAT	=	0x00ad
                           0000AE   396 G$RTC0KEY$0$0 == 0x00ae
                           0000AE   397 _RTC0KEY	=	0x00ae
                           0000AF   398 G$EMI0TC$0$0 == 0x00af
                           0000AF   399 _EMI0TC	=	0x00af
                           0000AF   400 G$ONESHOT$0$0 == 0x00af
                           0000AF   401 _ONESHOT	=	0x00af
                           0000B0   402 G$SPI1CN$0$0 == 0x00b0
                           0000B0   403 _SPI1CN	=	0x00b0
                           0000B1   404 G$OSCXCN$0$0 == 0x00b1
                           0000B1   405 _OSCXCN	=	0x00b1
                           0000B2   406 G$OSCICN$0$0 == 0x00b2
                           0000B2   407 _OSCICN	=	0x00b2
                           0000B3   408 G$OSCICL$0$0 == 0x00b3
                           0000B3   409 _OSCICL	=	0x00b3
                           0000B5   410 G$PMU0CF$0$0 == 0x00b5
                           0000B5   411 _PMU0CF	=	0x00b5
                           0000B6   412 G$FLSCL$0$0 == 0x00b6
                           0000B6   413 _FLSCL	=	0x00b6
                           0000B7   414 G$FLKEY$0$0 == 0x00b7
                           0000B7   415 _FLKEY	=	0x00b7
                           0000B8   416 G$IP$0$0 == 0x00b8
                           0000B8   417 _IP	=	0x00b8
                           0000B9   418 G$IREF0CN$0$0 == 0x00b9
                           0000B9   419 _IREF0CN	=	0x00b9
                           0000BA   420 G$ADC0AC$0$0 == 0x00ba
                           0000BA   421 _ADC0AC	=	0x00ba
                           0000BA   422 G$ADC0PWR$0$0 == 0x00ba
                           0000BA   423 _ADC0PWR	=	0x00ba
                           0000BB   424 G$ADC0MX$0$0 == 0x00bb
                           0000BB   425 _ADC0MX	=	0x00bb
                           0000BC   426 G$ADC0CF$0$0 == 0x00bc
                           0000BC   427 _ADC0CF	=	0x00bc
                           0000BD   428 G$ADC0TK$0$0 == 0x00bd
                           0000BD   429 _ADC0TK	=	0x00bd
                           0000BD   430 G$ADC0L$0$0 == 0x00bd
                           0000BD   431 _ADC0L	=	0x00bd
                           0000BE   432 G$ADC0H$0$0 == 0x00be
                           0000BE   433 _ADC0H	=	0x00be
                           0000BF   434 G$P1MASK$0$0 == 0x00bf
                           0000BF   435 _P1MASK	=	0x00bf
                           0000C0   436 G$SMB0CN$0$0 == 0x00c0
                           0000C0   437 _SMB0CN	=	0x00c0
                           0000C1   438 G$SMB0CF$0$0 == 0x00c1
                           0000C1   439 _SMB0CF	=	0x00c1
                           0000C2   440 G$SMB0DAT$0$0 == 0x00c2
                           0000C2   441 _SMB0DAT	=	0x00c2
                           0000C3   442 G$ADC0GTL$0$0 == 0x00c3
                           0000C3   443 _ADC0GTL	=	0x00c3
                           0000C4   444 G$ADC0GTH$0$0 == 0x00c4
                           0000C4   445 _ADC0GTH	=	0x00c4
                           0000C5   446 G$ADC0LTL$0$0 == 0x00c5
                           0000C5   447 _ADC0LTL	=	0x00c5
                           0000C6   448 G$ADC0LTH$0$0 == 0x00c6
                           0000C6   449 _ADC0LTH	=	0x00c6
                           0000C7   450 G$P0MASK$0$0 == 0x00c7
                           0000C7   451 _P0MASK	=	0x00c7
                           0000C8   452 G$TMR2CN$0$0 == 0x00c8
                           0000C8   453 _TMR2CN	=	0x00c8
                           0000C9   454 G$REG0CN$0$0 == 0x00c9
                           0000C9   455 _REG0CN	=	0x00c9
                           0000CA   456 G$TMR2RLL$0$0 == 0x00ca
                           0000CA   457 _TMR2RLL	=	0x00ca
                           0000CB   458 G$TMR2RLH$0$0 == 0x00cb
                           0000CB   459 _TMR2RLH	=	0x00cb
                           0000CC   460 G$TMR2L$0$0 == 0x00cc
                           0000CC   461 _TMR2L	=	0x00cc
                           0000CD   462 G$TMR2H$0$0 == 0x00cd
                           0000CD   463 _TMR2H	=	0x00cd
                           0000CE   464 G$PCA0CPM5$0$0 == 0x00ce
                           0000CE   465 _PCA0CPM5	=	0x00ce
                           0000CF   466 G$P1MAT$0$0 == 0x00cf
                           0000CF   467 _P1MAT	=	0x00cf
                           0000D0   468 G$PSW$0$0 == 0x00d0
                           0000D0   469 _PSW	=	0x00d0
                           0000D1   470 G$REF0CN$0$0 == 0x00d1
                           0000D1   471 _REF0CN	=	0x00d1
                           0000D2   472 G$PCA0CPL5$0$0 == 0x00d2
                           0000D2   473 _PCA0CPL5	=	0x00d2
                           0000D3   474 G$PCA0CPH5$0$0 == 0x00d3
                           0000D3   475 _PCA0CPH5	=	0x00d3
                           0000D4   476 G$P0SKIP$0$0 == 0x00d4
                           0000D4   477 _P0SKIP	=	0x00d4
                           0000D5   478 G$P1SKIP$0$0 == 0x00d5
                           0000D5   479 _P1SKIP	=	0x00d5
                           0000D6   480 G$P2SKIP$0$0 == 0x00d6
                           0000D6   481 _P2SKIP	=	0x00d6
                           0000D7   482 G$P0MAT$0$0 == 0x00d7
                           0000D7   483 _P0MAT	=	0x00d7
                           0000D8   484 G$PCA0CN$0$0 == 0x00d8
                           0000D8   485 _PCA0CN	=	0x00d8
                           0000D9   486 G$PCA0MD$0$0 == 0x00d9
                           0000D9   487 _PCA0MD	=	0x00d9
                           0000DA   488 G$PCA0CPM0$0$0 == 0x00da
                           0000DA   489 _PCA0CPM0	=	0x00da
                           0000DB   490 G$PCA0CPM1$0$0 == 0x00db
                           0000DB   491 _PCA0CPM1	=	0x00db
                           0000DC   492 G$PCA0CPM2$0$0 == 0x00dc
                           0000DC   493 _PCA0CPM2	=	0x00dc
                           0000DD   494 G$PCA0CPM3$0$0 == 0x00dd
                           0000DD   495 _PCA0CPM3	=	0x00dd
                           0000DE   496 G$PCA0CPM4$0$0 == 0x00de
                           0000DE   497 _PCA0CPM4	=	0x00de
                           0000DF   498 G$PCA0PWM$0$0 == 0x00df
                           0000DF   499 _PCA0PWM	=	0x00df
                           0000E0   500 G$ACC$0$0 == 0x00e0
                           0000E0   501 _ACC	=	0x00e0
                           0000E1   502 G$XBR0$0$0 == 0x00e1
                           0000E1   503 _XBR0	=	0x00e1
                           0000E2   504 G$XBR1$0$0 == 0x00e2
                           0000E2   505 _XBR1	=	0x00e2
                           0000E3   506 G$XBR2$0$0 == 0x00e3
                           0000E3   507 _XBR2	=	0x00e3
                           0000E4   508 G$IT01CF$0$0 == 0x00e4
                           0000E4   509 _IT01CF	=	0x00e4
                           0000E5   510 G$FLWR$0$0 == 0x00e5
                           0000E5   511 _FLWR	=	0x00e5
                           0000E6   512 G$EIE1$0$0 == 0x00e6
                           0000E6   513 _EIE1	=	0x00e6
                           0000E7   514 G$EIE2$0$0 == 0x00e7
                           0000E7   515 _EIE2	=	0x00e7
                           0000E8   516 G$ADC0CN$0$0 == 0x00e8
                           0000E8   517 _ADC0CN	=	0x00e8
                           0000E9   518 G$PCA0CPL1$0$0 == 0x00e9
                           0000E9   519 _PCA0CPL1	=	0x00e9
                           0000EA   520 G$PCA0CPH1$0$0 == 0x00ea
                           0000EA   521 _PCA0CPH1	=	0x00ea
                           0000EB   522 G$PCA0CPL2$0$0 == 0x00eb
                           0000EB   523 _PCA0CPL2	=	0x00eb
                           0000EC   524 G$PCA0CPH2$0$0 == 0x00ec
                           0000EC   525 _PCA0CPH2	=	0x00ec
                           0000ED   526 G$PCA0CPL3$0$0 == 0x00ed
                           0000ED   527 _PCA0CPL3	=	0x00ed
                           0000EE   528 G$PCA0CPH3$0$0 == 0x00ee
                           0000EE   529 _PCA0CPH3	=	0x00ee
                           0000EF   530 G$RSTSRC$0$0 == 0x00ef
                           0000EF   531 _RSTSRC	=	0x00ef
                           0000F0   532 G$B$0$0 == 0x00f0
                           0000F0   533 _B	=	0x00f0
                           0000F1   534 G$P0MDIN$0$0 == 0x00f1
                           0000F1   535 _P0MDIN	=	0x00f1
                           0000F2   536 G$P1MDIN$0$0 == 0x00f2
                           0000F2   537 _P1MDIN	=	0x00f2
                           0000F3   538 G$P2MDIN$0$0 == 0x00f3
                           0000F3   539 _P2MDIN	=	0x00f3
                           0000F4   540 G$SMB0ADR$0$0 == 0x00f4
                           0000F4   541 _SMB0ADR	=	0x00f4
                           0000F5   542 G$SMB0ADM$0$0 == 0x00f5
                           0000F5   543 _SMB0ADM	=	0x00f5
                           0000F6   544 G$EIP1$0$0 == 0x00f6
                           0000F6   545 _EIP1	=	0x00f6
                           0000F7   546 G$EIP2$0$0 == 0x00f7
                           0000F7   547 _EIP2	=	0x00f7
                           0000F8   548 G$SPI0CN$0$0 == 0x00f8
                           0000F8   549 _SPI0CN	=	0x00f8
                           0000F9   550 G$PCA0L$0$0 == 0x00f9
                           0000F9   551 _PCA0L	=	0x00f9
                           0000FA   552 G$PCA0H$0$0 == 0x00fa
                           0000FA   553 _PCA0H	=	0x00fa
                           0000FB   554 G$PCA0CPL0$0$0 == 0x00fb
                           0000FB   555 _PCA0CPL0	=	0x00fb
                           0000FC   556 G$PCA0CPH0$0$0 == 0x00fc
                           0000FC   557 _PCA0CPH0	=	0x00fc
                           0000FD   558 G$PCA0CPL4$0$0 == 0x00fd
                           0000FD   559 _PCA0CPL4	=	0x00fd
                           0000FE   560 G$PCA0CPH4$0$0 == 0x00fe
                           0000FE   561 _PCA0CPH4	=	0x00fe
                           0000FF   562 G$VDM0CN$0$0 == 0x00ff
                           0000FF   563 _VDM0CN	=	0x00ff
                           008382   564 G$DP$0$0 == 0x8382
                           008382   565 _DP	=	0x8382
                           008685   566 G$TOFF$0$0 == 0x8685
                           008685   567 _TOFF	=	0x8685
                           009392   568 G$TMR3RL$0$0 == 0x9392
                           009392   569 _TMR3RL	=	0x9392
                           009594   570 G$TMR3$0$0 == 0x9594
                           009594   571 _TMR3	=	0x9594
                           00BEBD   572 G$ADC0$0$0 == 0xbebd
                           00BEBD   573 _ADC0	=	0xbebd
                           00C4C3   574 G$ADC0GT$0$0 == 0xc4c3
                           00C4C3   575 _ADC0GT	=	0xc4c3
                           00C6C5   576 G$ADC0LT$0$0 == 0xc6c5
                           00C6C5   577 _ADC0LT	=	0xc6c5
                           00CBCA   578 G$TMR2RL$0$0 == 0xcbca
                           00CBCA   579 _TMR2RL	=	0xcbca
                           00CDCC   580 G$TMR2$0$0 == 0xcdcc
                           00CDCC   581 _TMR2	=	0xcdcc
                           00D3D2   582 G$PCA0CP5$0$0 == 0xd3d2
                           00D3D2   583 _PCA0CP5	=	0xd3d2
                           00EAE9   584 G$PCA0CP1$0$0 == 0xeae9
                           00EAE9   585 _PCA0CP1	=	0xeae9
                           00ECEB   586 G$PCA0CP2$0$0 == 0xeceb
                           00ECEB   587 _PCA0CP2	=	0xeceb
                           00EEED   588 G$PCA0CP3$0$0 == 0xeeed
                           00EEED   589 _PCA0CP3	=	0xeeed
                           00FAF9   590 G$PCA0$0$0 == 0xfaf9
                           00FAF9   591 _PCA0	=	0xfaf9
                           00FCFB   592 G$PCA0CP0$0$0 == 0xfcfb
                           00FCFB   593 _PCA0CP0	=	0xfcfb
                           00FEFD   594 G$PCA0CP4$0$0 == 0xfefd
                           00FEFD   595 _PCA0CP4	=	0xfefd
                           0000AA   596 G$_XPAGE$0$0 == 0x00aa
                           0000AA   597 __XPAGE	=	0x00aa
                                    598 ;--------------------------------------------------------
                                    599 ; special function bits
                                    600 ;--------------------------------------------------------
                                    601 	.area RSEG    (ABS,DATA)
      000000                        602 	.org 0x0000
                           00008F   603 G$TF1$0$0 == 0x008f
                           00008F   604 _TF1	=	0x008f
                           00008E   605 G$TR1$0$0 == 0x008e
                           00008E   606 _TR1	=	0x008e
                           00008D   607 G$TF0$0$0 == 0x008d
                           00008D   608 _TF0	=	0x008d
                           00008C   609 G$TR0$0$0 == 0x008c
                           00008C   610 _TR0	=	0x008c
                           00008B   611 G$IE1$0$0 == 0x008b
                           00008B   612 _IE1	=	0x008b
                           00008A   613 G$IT1$0$0 == 0x008a
                           00008A   614 _IT1	=	0x008a
                           000089   615 G$IE0$0$0 == 0x0089
                           000089   616 _IE0	=	0x0089
                           000088   617 G$IT0$0$0 == 0x0088
                           000088   618 _IT0	=	0x0088
                           00009F   619 G$S0MODE$0$0 == 0x009f
                           00009F   620 _S0MODE	=	0x009f
                           00009D   621 G$MCE0$0$0 == 0x009d
                           00009D   622 _MCE0	=	0x009d
                           00009C   623 G$REN0$0$0 == 0x009c
                           00009C   624 _REN0	=	0x009c
                           00009B   625 G$TB80$0$0 == 0x009b
                           00009B   626 _TB80	=	0x009b
                           00009A   627 G$RB80$0$0 == 0x009a
                           00009A   628 _RB80	=	0x009a
                           000099   629 G$TI0$0$0 == 0x0099
                           000099   630 _TI0	=	0x0099
                           000098   631 G$RI0$0$0 == 0x0098
                           000098   632 _RI0	=	0x0098
                           0000AF   633 G$EA$0$0 == 0x00af
                           0000AF   634 _EA	=	0x00af
                           0000AE   635 G$ESPI0$0$0 == 0x00ae
                           0000AE   636 _ESPI0	=	0x00ae
                           0000AD   637 G$ET2$0$0 == 0x00ad
                           0000AD   638 _ET2	=	0x00ad
                           0000AC   639 G$ES0$0$0 == 0x00ac
                           0000AC   640 _ES0	=	0x00ac
                           0000AB   641 G$ET1$0$0 == 0x00ab
                           0000AB   642 _ET1	=	0x00ab
                           0000AA   643 G$EX1$0$0 == 0x00aa
                           0000AA   644 _EX1	=	0x00aa
                           0000A9   645 G$ET0$0$0 == 0x00a9
                           0000A9   646 _ET0	=	0x00a9
                           0000A8   647 G$EX0$0$0 == 0x00a8
                           0000A8   648 _EX0	=	0x00a8
                           0000B7   649 G$SPIF1$0$0 == 0x00b7
                           0000B7   650 _SPIF1	=	0x00b7
                           0000B6   651 G$WCOL1$0$0 == 0x00b6
                           0000B6   652 _WCOL1	=	0x00b6
                           0000B5   653 G$MODF1$0$0 == 0x00b5
                           0000B5   654 _MODF1	=	0x00b5
                           0000B4   655 G$RXOVRN1$0$0 == 0x00b4
                           0000B4   656 _RXOVRN1	=	0x00b4
                           0000B3   657 G$NSS1MD1$0$0 == 0x00b3
                           0000B3   658 _NSS1MD1	=	0x00b3
                           0000B2   659 G$NSS1MD0$0$0 == 0x00b2
                           0000B2   660 _NSS1MD0	=	0x00b2
                           0000B1   661 G$TXBMT1$0$0 == 0x00b1
                           0000B1   662 _TXBMT1	=	0x00b1
                           0000B0   663 G$SPI1EN$0$0 == 0x00b0
                           0000B0   664 _SPI1EN	=	0x00b0
                           0000BE   665 G$PSPI0$0$0 == 0x00be
                           0000BE   666 _PSPI0	=	0x00be
                           0000BD   667 G$PT2$0$0 == 0x00bd
                           0000BD   668 _PT2	=	0x00bd
                           0000BC   669 G$PS0$0$0 == 0x00bc
                           0000BC   670 _PS0	=	0x00bc
                           0000BB   671 G$PT1$0$0 == 0x00bb
                           0000BB   672 _PT1	=	0x00bb
                           0000BA   673 G$PX1$0$0 == 0x00ba
                           0000BA   674 _PX1	=	0x00ba
                           0000B9   675 G$PT0$0$0 == 0x00b9
                           0000B9   676 _PT0	=	0x00b9
                           0000B8   677 G$PX0$0$0 == 0x00b8
                           0000B8   678 _PX0	=	0x00b8
                           0000C7   679 G$MASTER$0$0 == 0x00c7
                           0000C7   680 _MASTER	=	0x00c7
                           0000C6   681 G$TXMODE$0$0 == 0x00c6
                           0000C6   682 _TXMODE	=	0x00c6
                           0000C5   683 G$STA$0$0 == 0x00c5
                           0000C5   684 _STA	=	0x00c5
                           0000C4   685 G$STO$0$0 == 0x00c4
                           0000C4   686 _STO	=	0x00c4
                           0000C3   687 G$ACKRQ$0$0 == 0x00c3
                           0000C3   688 _ACKRQ	=	0x00c3
                           0000C2   689 G$ARBLOST$0$0 == 0x00c2
                           0000C2   690 _ARBLOST	=	0x00c2
                           0000C1   691 G$ACK$0$0 == 0x00c1
                           0000C1   692 _ACK	=	0x00c1
                           0000C0   693 G$SI$0$0 == 0x00c0
                           0000C0   694 _SI	=	0x00c0
                           0000CF   695 G$TF2H$0$0 == 0x00cf
                           0000CF   696 _TF2H	=	0x00cf
                           0000CE   697 G$TF2L$0$0 == 0x00ce
                           0000CE   698 _TF2L	=	0x00ce
                           0000CD   699 G$TF2LEN$0$0 == 0x00cd
                           0000CD   700 _TF2LEN	=	0x00cd
                           0000CC   701 G$TF2CEN$0$0 == 0x00cc
                           0000CC   702 _TF2CEN	=	0x00cc
                           0000CB   703 G$T2SPLIT$0$0 == 0x00cb
                           0000CB   704 _T2SPLIT	=	0x00cb
                           0000CA   705 G$TR2$0$0 == 0x00ca
                           0000CA   706 _TR2	=	0x00ca
                           0000C9   707 G$T2RCLK$0$0 == 0x00c9
                           0000C9   708 _T2RCLK	=	0x00c9
                           0000C8   709 G$T2XCLK$0$0 == 0x00c8
                           0000C8   710 _T2XCLK	=	0x00c8
                           0000D7   711 G$CY$0$0 == 0x00d7
                           0000D7   712 _CY	=	0x00d7
                           0000D6   713 G$AC$0$0 == 0x00d6
                           0000D6   714 _AC	=	0x00d6
                           0000D5   715 G$F0$0$0 == 0x00d5
                           0000D5   716 _F0	=	0x00d5
                           0000D4   717 G$RS1$0$0 == 0x00d4
                           0000D4   718 _RS1	=	0x00d4
                           0000D3   719 G$RS0$0$0 == 0x00d3
                           0000D3   720 _RS0	=	0x00d3
                           0000D2   721 G$OV$0$0 == 0x00d2
                           0000D2   722 _OV	=	0x00d2
                           0000D1   723 G$F1$0$0 == 0x00d1
                           0000D1   724 _F1	=	0x00d1
                           0000D0   725 G$P$0$0 == 0x00d0
                           0000D0   726 _P	=	0x00d0
                           0000DF   727 G$CF$0$0 == 0x00df
                           0000DF   728 _CF	=	0x00df
                           0000DE   729 G$CR$0$0 == 0x00de
                           0000DE   730 _CR	=	0x00de
                           0000DD   731 G$CCF5$0$0 == 0x00dd
                           0000DD   732 _CCF5	=	0x00dd
                           0000DC   733 G$CCF4$0$0 == 0x00dc
                           0000DC   734 _CCF4	=	0x00dc
                           0000DB   735 G$CCF3$0$0 == 0x00db
                           0000DB   736 _CCF3	=	0x00db
                           0000DA   737 G$CCF2$0$0 == 0x00da
                           0000DA   738 _CCF2	=	0x00da
                           0000D9   739 G$CCF1$0$0 == 0x00d9
                           0000D9   740 _CCF1	=	0x00d9
                           0000D8   741 G$CCF0$0$0 == 0x00d8
                           0000D8   742 _CCF0	=	0x00d8
                           0000EF   743 G$AD0EN$0$0 == 0x00ef
                           0000EF   744 _AD0EN	=	0x00ef
                           0000EE   745 G$BURSTEN$0$0 == 0x00ee
                           0000EE   746 _BURSTEN	=	0x00ee
                           0000ED   747 G$AD0INT$0$0 == 0x00ed
                           0000ED   748 _AD0INT	=	0x00ed
                           0000EC   749 G$AD0BUSY$0$0 == 0x00ec
                           0000EC   750 _AD0BUSY	=	0x00ec
                           0000EB   751 G$AD0WINT$0$0 == 0x00eb
                           0000EB   752 _AD0WINT	=	0x00eb
                           0000EA   753 G$AD0CM2$0$0 == 0x00ea
                           0000EA   754 _AD0CM2	=	0x00ea
                           0000E9   755 G$AD0CM1$0$0 == 0x00e9
                           0000E9   756 _AD0CM1	=	0x00e9
                           0000E8   757 G$AD0CM0$0$0 == 0x00e8
                           0000E8   758 _AD0CM0	=	0x00e8
                           0000FF   759 G$SPIF0$0$0 == 0x00ff
                           0000FF   760 _SPIF0	=	0x00ff
                           0000FE   761 G$WCOL0$0$0 == 0x00fe
                           0000FE   762 _WCOL0	=	0x00fe
                           0000FD   763 G$MODF0$0$0 == 0x00fd
                           0000FD   764 _MODF0	=	0x00fd
                           0000FC   765 G$RXOVRN0$0$0 == 0x00fc
                           0000FC   766 _RXOVRN0	=	0x00fc
                           0000FB   767 G$NSS0MD1$0$0 == 0x00fb
                           0000FB   768 _NSS0MD1	=	0x00fb
                           0000FA   769 G$NSS0MD0$0$0 == 0x00fa
                           0000FA   770 _NSS0MD0	=	0x00fa
                           0000F9   771 G$TXBMT0$0$0 == 0x00f9
                           0000F9   772 _TXBMT0	=	0x00f9
                           0000F8   773 G$SPI0EN$0$0 == 0x00f8
                           0000F8   774 _SPI0EN	=	0x00f8
                           000094   775 G$NSS1$0$0 == 0x0094
                           000094   776 _NSS1	=	0x0094
                           000081   777 G$IRQ$0$0 == 0x0081
                           000081   778 _IRQ	=	0x0081
                           0000A6   779 G$SDN$0$0 == 0x00a6
                           0000A6   780 _SDN	=	0x00a6
                           000095   781 G$LED$0$0 == 0x0095
                           000095   782 _LED	=	0x0095
                                    783 ;--------------------------------------------------------
                                    784 ; overlayable register banks
                                    785 ;--------------------------------------------------------
                                    786 	.area REG_BANK_0	(REL,OVR,DATA)
      000000                        787 	.ds 8
                                    788 ;--------------------------------------------------------
                                    789 ; internal ram data
                                    790 ;--------------------------------------------------------
                                    791 	.area DSEG    (DATA)
                           000043   792 G$RadioStatus$0$0 == 0x0043
                           000043   793 _RadioStatus	=	0x0043
                           000000   794 LSi100x_TxTone.setTRxFrequency$frequency$1$45==.
      000008                        795 _setTRxFrequency_frequency_1_45:
      000008                        796 	.ds 4
                           000004   797 LSi100x_TxTone.setTRxFrequency$nominalCarrierFrequency$1$46==.
      00000C                        798 _setTRxFrequency_nominalCarrierFrequency_1_46:
      00000C                        799 	.ds 2
                           000006   800 LSi100x_TxTone.divideWithRounding$divisor$1$49==.
      00000E                        801 _divideWithRounding_PARM_2:
      00000E                        802 	.ds 4
                                    803 ;--------------------------------------------------------
                                    804 ; overlayable items in internal ram 
                                    805 ;--------------------------------------------------------
                                    806 	.area	OSEG    (OVR,DATA)
                           000000   807 LSi100x_TxTone.phyWrite$value$1$23==.
      000012                        808 _phyWrite_PARM_2:
      000012                        809 	.ds 1
                                    810 	.area	OSEG    (OVR,DATA)
                                    811 	.area	OSEG    (OVR,DATA)
                           000000   812 LSi100x_TxTone.delay$reload$1$38==.
      000012                        813 _delay_reload_1_38:
      000012                        814 	.ds 2
                                    815 	.area	OSEG    (OVR,DATA)
                           000000   816 LSi100x_TxTone.SetTimeOut$reload$1$40==.
      000012                        817 _SetTimeOut_reload_1_40:
      000012                        818 	.ds 2
                                    819 ;--------------------------------------------------------
                                    820 ; Stack segment in internal ram 
                                    821 ;--------------------------------------------------------
                                    822 	.area	SSEG
      000016                        823 __start__stack:
      000016                        824 	.ds	1
                                    825 
                                    826 ;--------------------------------------------------------
                                    827 ; indirectly addressable internal ram data
                                    828 ;--------------------------------------------------------
                                    829 	.area ISEG    (DATA)
                                    830 ;--------------------------------------------------------
                                    831 ; absolute internal ram data
                                    832 ;--------------------------------------------------------
                                    833 	.area IABS    (ABS,DATA)
                                    834 	.area IABS    (ABS,DATA)
                                    835 ;--------------------------------------------------------
                                    836 ; bit data
                                    837 ;--------------------------------------------------------
                                    838 	.area BSEG    (BIT)
                                    839 ;--------------------------------------------------------
                                    840 ; paged external ram data
                                    841 ;--------------------------------------------------------
                                    842 	.area PSEG    (PAG,XDATA)
                                    843 ;--------------------------------------------------------
                                    844 ; external ram data
                                    845 ;--------------------------------------------------------
                                    846 	.area XSEG    (XDATA)
                                    847 ;--------------------------------------------------------
                                    848 ; absolute external ram data
                                    849 ;--------------------------------------------------------
                                    850 	.area XABS    (ABS,XDATA)
                                    851 ;--------------------------------------------------------
                                    852 ; external initialized ram data
                                    853 ;--------------------------------------------------------
                                    854 	.area XISEG   (XDATA)
                                    855 	.area HOME    (CODE)
                                    856 	.area GSINIT0 (CODE)
                                    857 	.area GSINIT1 (CODE)
                                    858 	.area GSINIT2 (CODE)
                                    859 	.area GSINIT3 (CODE)
                                    860 	.area GSINIT4 (CODE)
                                    861 	.area GSINIT5 (CODE)
                                    862 	.area GSINIT  (CODE)
                                    863 	.area GSFINAL (CODE)
                                    864 	.area CSEG    (CODE)
                                    865 ;--------------------------------------------------------
                                    866 ; interrupt vector 
                                    867 ;--------------------------------------------------------
                                    868 	.area HOME    (CODE)
      000000                        869 __interrupt_vect:
      000000 02 00 11         [24]  870 	ljmp	__sdcc_gsinit_startup
      000003 32               [24]  871 	reti
      000004                        872 	.ds	7
      00000B 02 02 7B         [24]  873 	ljmp	_T0_ISR
                                    874 ;--------------------------------------------------------
                                    875 ; global & static initialisations
                                    876 ;--------------------------------------------------------
                                    877 	.area HOME    (CODE)
                                    878 	.area GSINIT  (CODE)
                                    879 	.area GSFINAL (CODE)
                                    880 	.area GSINIT  (CODE)
                                    881 	.globl __sdcc_gsinit_startup
                                    882 	.globl __sdcc_program_startup
                                    883 	.globl __start__stack
                                    884 	.globl __mcs51_genXINIT
                                    885 	.globl __mcs51_genXRAMCLEAR
                                    886 	.globl __mcs51_genRAMCLEAR
                                    887 	.area GSFINAL (CODE)
      00006A 02 00 0E         [24]  888 	ljmp	__sdcc_program_startup
                                    889 ;--------------------------------------------------------
                                    890 ; Home
                                    891 ;--------------------------------------------------------
                                    892 	.area HOME    (CODE)
                                    893 	.area HOME    (CODE)
      00000E                        894 __sdcc_program_startup:
      00000E 02 00 6D         [24]  895 	ljmp	_main
                                    896 ;	return from main will return to caller
                                    897 ;--------------------------------------------------------
                                    898 ; code
                                    899 ;--------------------------------------------------------
                                    900 	.area CSEG    (CODE)
                                    901 ;------------------------------------------------------------
                                    902 ;Allocation info for local variables in function 'main'
                                    903 ;------------------------------------------------------------
                                    904 ;status                    Allocated to registers 
                                    905 ;------------------------------------------------------------
                           000000   906 	G$main$0$0 ==.
                           000000   907 	C$Si100x_TxTone.c$150$0$0 ==.
                                    908 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:150: void main (void)
                                    909 ;	-----------------------------------------
                                    910 ;	 function main
                                    911 ;	-----------------------------------------
      00006D                        912 _main:
                           000007   913 	ar7 = 0x07
                           000006   914 	ar6 = 0x06
                           000005   915 	ar5 = 0x05
                           000004   916 	ar4 = 0x04
                           000003   917 	ar3 = 0x03
                           000002   918 	ar2 = 0x02
                           000001   919 	ar1 = 0x01
                           000000   920 	ar0 = 0x00
                           000000   921 	C$Si100x_TxTone.c$154$1$15 ==.
                                    922 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:154: PCA0MD   &= ~0x40;                  // disable F930 watchdog
      00006D 53 D9 BF         [24]  923 	anl	_PCA0MD,#0xbf
                           000003   924 	C$Si100x_TxTone.c$164$1$15 ==.
                                    925 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:164: PortInit ();
      000070 12 00 ED         [24]  926 	lcall	_PortInit
                           000006   927 	C$Si100x_TxTone.c$165$1$15 ==.
                                    928 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:165: SYSCLK_Init ();
      000073 12 01 0E         [24]  929 	lcall	_SYSCLK_Init
                           000009   930 	C$Si100x_TxTone.c$166$1$15 ==.
                                    931 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:166: SPI1_Init ();
      000076 12 01 12         [24]  932 	lcall	_SPI1_Init
                           00000C   933 	C$Si100x_TxTone.c$168$1$15 ==.
                                    934 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:168: EA = 1;
                                    935 ;	assignBit
      000079 D2 AF            [12]  936 	setb	_EA
                           00000E   937 	C$Si100x_TxTone.c$170$1$15 ==.
                                    938 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:170: RadioStatus = RadioInit();
      00007B 12 01 67         [24]  939 	lcall	_RadioInit
      00007E 85 82 43         [24]  940 	mov	_RadioStatus,dpl
                           000014   941 	C$Si100x_TxTone.c$173$1$15 ==.
                                    942 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:173: while(RadioStatus);
      000081                        943 00101$:
      000081 E5 43            [12]  944 	mov	a,_RadioStatus
      000083 70 FC            [24]  945 	jnz	00101$
                           000018   946 	C$Si100x_TxTone.c$178$1$15 ==.
                                    947 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:178: setTRxFrequency (915000000L);
      000085 90 CA C0         [24]  948 	mov	dptr,#0xcac0
      000088 75 F0 89         [24]  949 	mov	b,#0x89
      00008B 74 36            [12]  950 	mov	a,#0x36
      00008D 12 02 89         [24]  951 	lcall	_setTRxFrequency
                           000023   952 	C$Si100x_TxTone.c$194$1$15 ==.
                                    953 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:194: setTxPower(7);                      // Full power
      000090 75 82 07         [24]  954 	mov	dpl,#0x07
      000093 12 04 5E         [24]  955 	lcall	_setTxPower
                           000029   956 	C$Si100x_TxTone.c$197$1$15 ==.
                                    957 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:197: phyWrite(EZRADIOPRO_MODULATION_MODE_CONTROL_2, 0x00);
      000096 75 12 00         [24]  958 	mov	_phyWrite_PARM_2,#0x00
      000099 75 82 71         [24]  959 	mov	dpl,#0x71
      00009C 12 01 21         [24]  960 	lcall	_phyWrite
                           000032   961 	C$Si100x_TxTone.c$203$1$15 ==.
                                    962 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:203: phyWrite(EZRADIOPRO_GPIO1_CONFIGURATION, 0x12);
      00009F 75 12 12         [24]  963 	mov	_phyWrite_PARM_2,#0x12
      0000A2 75 82 0C         [24]  964 	mov	dpl,#0x0c
      0000A5 12 01 21         [24]  965 	lcall	_phyWrite
                           00003B   966 	C$Si100x_TxTone.c$204$1$15 ==.
                                    967 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:204: phyWrite(EZRADIOPRO_GPIO2_CONFIGURATION, 0x15);
      0000A8 75 12 15         [24]  968 	mov	_phyWrite_PARM_2,#0x15
      0000AB 75 82 0D         [24]  969 	mov	dpl,#0x0d
      0000AE 12 01 21         [24]  970 	lcall	_phyWrite
                           000044   971 	C$Si100x_TxTone.c$208$1$15 ==.
                                    972 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:208: phyWrite(EZRADIOPRO_OPERATING_AND_FUNCTION_CONTROL_1,(EZRADIOPRO_TXON|EZRADIOPRO_XTON));
      0000B1 75 12 09         [24]  973 	mov	_phyWrite_PARM_2,#0x09
      0000B4 75 82 07         [24]  974 	mov	dpl,#0x07
      0000B7 12 01 21         [24]  975 	lcall	_phyWrite
                           00004D   976 	C$Si100x_TxTone.c$210$1$15 ==.
                                    977 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:210: delay(ONE_MS);                     // Wait 2 ms
      0000BA 90 00 D0         [24]  978 	mov	dptr,#0x00d0
      0000BD 12 02 2E         [24]  979 	lcall	_delay
                           000053   980 	C$Si100x_TxTone.c$212$1$15 ==.
                                    981 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:212: status = phyRead(EZRADIOPRO_DEVICE_STATUS);
      0000C0 75 82 02         [24]  982 	mov	dpl,#0x02
      0000C3 12 01 44         [24]  983 	lcall	_phyRead
      0000C6 E5 82            [12]  984 	mov	a,dpl
                           00005B   985 	C$Si100x_TxTone.c$215$1$15 ==.
                                    986 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:215: if((status& 0x03)==0x02)
      0000C8 54 03            [12]  987 	anl	a,#0x03
      0000CA FF               [12]  988 	mov	r7,a
      0000CB BF 02 1C         [24]  989 	cjne	r7,#0x02,00110$
                           000061   990 	C$Si100x_TxTone.c$218$2$16 ==.
                                    991 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:218: while(1)
      0000CE                        992 00105$:
                           000061   993 	C$Si100x_TxTone.c$220$3$17 ==.
                                    994 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:220: LED = !LED;
      0000CE B2 95            [12]  995 	cpl	_LED
                           000063   996 	C$Si100x_TxTone.c$221$3$17 ==.
                                    997 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:221: delay(TWENTY_FIVE_MS);
      0000D0 90 14 58         [24]  998 	mov	dptr,#0x1458
      0000D3 12 02 2E         [24]  999 	lcall	_delay
                           000069  1000 	C$Si100x_TxTone.c$222$3$17 ==.
                                   1001 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:222: delay(TWENTY_FIVE_MS);
      0000D6 90 14 58         [24] 1002 	mov	dptr,#0x1458
      0000D9 12 02 2E         [24] 1003 	lcall	_delay
                           00006F  1004 	C$Si100x_TxTone.c$223$3$17 ==.
                                   1005 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:223: delay(TWENTY_FIVE_MS);
      0000DC 90 14 58         [24] 1006 	mov	dptr,#0x1458
      0000DF 12 02 2E         [24] 1007 	lcall	_delay
                           000075  1008 	C$Si100x_TxTone.c$224$3$17 ==.
                                   1009 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:224: delay(TWENTY_FIVE_MS);
      0000E2 90 14 58         [24] 1010 	mov	dptr,#0x1458
      0000E5 12 02 2E         [24] 1011 	lcall	_delay
                           00007B  1012 	C$Si100x_TxTone.c$228$1$15 ==.
                                   1013 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:228: while(1);
      0000E8 80 E4            [24] 1014 	sjmp	00105$
      0000EA                       1015 00110$:
      0000EA 80 FE            [24] 1016 	sjmp	00110$
                           00007F  1017 	C$Si100x_TxTone.c$229$1$15 ==.
                           00007F  1018 	XG$main$0$0 ==.
      0000EC 22               [24] 1019 	ret
                                   1020 ;------------------------------------------------------------
                                   1021 ;Allocation info for local variables in function 'PortInit'
                                   1022 ;------------------------------------------------------------
                           000080  1023 	G$PortInit$0$0 ==.
                           000080  1024 	C$Si100x_TxTone.c$259$1$15 ==.
                                   1025 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:259: void PortInit (void)
                                   1026 ;	-----------------------------------------
                                   1027 ;	 function PortInit
                                   1028 ;	-----------------------------------------
      0000ED                       1029 _PortInit:
                           000080  1030 	C$Si100x_TxTone.c$262$1$19 ==.
                                   1031 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:262: XBR1    |= 0x40;                    // Enable SPI1 (3 wire mode)
      0000ED 43 E2 40         [24] 1032 	orl	_XBR1,#0x40
                           000083  1033 	C$Si100x_TxTone.c$263$1$19 ==.
                                   1034 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:263: P1MDOUT |= 0x15;                    // MOSI, SCK, NSS, push-pull
      0000F0 43 A5 15         [24] 1035 	orl	_P1MDOUT,#0x15
                           000086  1036 	C$Si100x_TxTone.c$264$1$19 ==.
                                   1037 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:264: SFRPAGE  = CONFIG_PAGE;
      0000F3 75 A7 0F         [24] 1038 	mov	_SFRPAGE,#0x0f
                           000089  1039 	C$Si100x_TxTone.c$265$1$19 ==.
                                   1040 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:265: P1DRV   |= 0x15;                    // MOSI, SCK, NSS, high current mode
      0000F6 43 A5 15         [24] 1041 	orl	_P1DRV,#0x15
                           00008C  1042 	C$Si100x_TxTone.c$266$1$19 ==.
                                   1043 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:266: SFRPAGE  = LEGACY_PAGE;
      0000F9 75 A7 00         [24] 1044 	mov	_SFRPAGE,#0x00
                           00008F  1045 	C$Si100x_TxTone.c$270$1$19 ==.
                                   1046 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:270: P2MDOUT |= 0x40;                    // SDN P2.6 push-pull
      0000FC 43 A6 40         [24] 1047 	orl	_P2MDOUT,#0x40
                           000092  1048 	C$Si100x_TxTone.c$271$1$19 ==.
                                   1049 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:271: SFRPAGE  = CONFIG_PAGE;
      0000FF 75 A7 0F         [24] 1050 	mov	_SFRPAGE,#0x0f
                           000095  1051 	C$Si100x_TxTone.c$272$1$19 ==.
                                   1052 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:272: P2DRV   |= 0x40;                    // SDN  P2.6 high-current mode
      000102 43 A6 40         [24] 1053 	orl	_P2DRV,#0x40
                           000098  1054 	C$Si100x_TxTone.c$273$1$19 ==.
                                   1055 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:273: SFRPAGE  = LEGACY_PAGE;
      000105 75 A7 00         [24] 1056 	mov	_SFRPAGE,#0x00
                           00009B  1057 	C$Si100x_TxTone.c$287$1$19 ==.
                                   1058 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:287: SDN      = 0;                       // SDN low before enabling crossbar
                                   1059 ;	assignBit
      000108 C2 A6            [12] 1060 	clr	_SDN
                           00009D  1061 	C$Si100x_TxTone.c$289$1$19 ==.
                                   1062 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:289: XBR2    |= 0x40;                    // enable Crossbar
      00010A 43 E3 40         [24] 1063 	orl	_XBR2,#0x40
                           0000A0  1064 	C$Si100x_TxTone.c$290$1$19 ==.
                           0000A0  1065 	XG$PortInit$0$0 ==.
      00010D 22               [24] 1066 	ret
                                   1067 ;------------------------------------------------------------
                                   1068 ;Allocation info for local variables in function 'SYSCLK_Init'
                                   1069 ;------------------------------------------------------------
                           0000A1  1070 	G$SYSCLK_Init$0$0 ==.
                           0000A1  1071 	C$Si100x_TxTone.c$300$1$19 ==.
                                   1072 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:300: void SYSCLK_Init (void)
                                   1073 ;	-----------------------------------------
                                   1074 ;	 function SYSCLK_Init
                                   1075 ;	-----------------------------------------
      00010E                       1076 _SYSCLK_Init:
                           0000A1  1077 	C$Si100x_TxTone.c$302$1$21 ==.
                                   1078 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:302: CLKSEL    = 0x14;                   // use 10 MHz clock
      00010E 75 A9 14         [24] 1079 	mov	_CLKSEL,#0x14
                           0000A4  1080 	C$Si100x_TxTone.c$303$1$21 ==.
                           0000A4  1081 	XG$SYSCLK_Init$0$0 ==.
      000111 22               [24] 1082 	ret
                                   1083 ;------------------------------------------------------------
                                   1084 ;Allocation info for local variables in function 'SPI1_Init'
                                   1085 ;------------------------------------------------------------
                           0000A5  1086 	G$SPI1_Init$0$0 ==.
                           0000A5  1087 	C$Si100x_TxTone.c$312$1$21 ==.
                                   1088 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:312: void SPI1_Init()
                                   1089 ;	-----------------------------------------
                                   1090 ;	 function SPI1_Init
                                   1091 ;	-----------------------------------------
      000112                       1092 _SPI1_Init:
                           0000A5  1093 	C$Si100x_TxTone.c$315$1$22 ==.
                                   1094 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:315: SPI1CFG   = 0x40;                   // master mode
      000112 75 84 40         [24] 1095 	mov	_SPI1CFG,#0x40
                           0000A8  1096 	C$Si100x_TxTone.c$316$1$22 ==.
                                   1097 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:316: SPI1CN    = 0x00;                   // 3 wire master mode
      000115 75 B0 00         [24] 1098 	mov	_SPI1CN,#0x00
                           0000AB  1099 	C$Si100x_TxTone.c$317$1$22 ==.
                                   1100 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:317: SPI1CKR   = 0x00;                   // Use SYSCLK/2
      000118 75 85 00         [24] 1101 	mov	_SPI1CKR,#0x00
                           0000AE  1102 	C$Si100x_TxTone.c$318$1$22 ==.
                                   1103 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:318: SPI1CN   |= 0x01;                   // enable SPI
      00011B 43 B0 01         [24] 1104 	orl	_SPI1CN,#0x01
                           0000B1  1105 	C$Si100x_TxTone.c$319$1$22 ==.
                                   1106 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:319: NSS1 = 1;                           // set NSS high
                                   1107 ;	assignBit
      00011E D2 94            [12] 1108 	setb	_NSS1
                           0000B3  1109 	C$Si100x_TxTone.c$320$1$22 ==.
                           0000B3  1110 	XG$SPI1_Init$0$0 ==.
      000120 22               [24] 1111 	ret
                                   1112 ;------------------------------------------------------------
                                   1113 ;Allocation info for local variables in function 'phyWrite'
                                   1114 ;------------------------------------------------------------
                                   1115 ;value                     Allocated with name '_phyWrite_PARM_2'
                                   1116 ;reg                       Allocated to registers r7 
                                   1117 ;------------------------------------------------------------
                           0000B4  1118 	G$phyWrite$0$0 ==.
                           0000B4  1119 	C$Si100x_TxTone.c$335$1$22 ==.
                                   1120 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:335: void phyWrite (U8 reg, U8 value)
                                   1121 ;	-----------------------------------------
                                   1122 ;	 function phyWrite
                                   1123 ;	-----------------------------------------
      000121                       1124 _phyWrite:
      000121 AF 82            [24] 1125 	mov	r7,dpl
                           0000B6  1126 	C$Si100x_TxTone.c$338$1$24 ==.
                                   1127 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:338: NSS1 = 0;                           // drive NSS low
                                   1128 ;	assignBit
      000123 C2 94            [12] 1129 	clr	_NSS1
                           0000B8  1130 	C$Si100x_TxTone.c$339$1$24 ==.
                                   1131 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:339: SPIF1 = 0;                          // clear SPIF
                                   1132 ;	assignBit
      000125 C2 B7            [12] 1133 	clr	_SPIF1
                           0000BA  1134 	C$Si100x_TxTone.c$340$1$24 ==.
                                   1135 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:340: SPI1DAT = (reg | 0x80);             // write reg address
      000127 74 80            [12] 1136 	mov	a,#0x80
      000129 4F               [12] 1137 	orl	a,r7
      00012A F5 86            [12] 1138 	mov	_SPI1DAT,a
                           0000BF  1139 	C$Si100x_TxTone.c$341$1$24 ==.
                                   1140 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:341: while(!TXBMT1);                     // wait on TXBMT
      00012C                       1141 00101$:
      00012C 30 B1 FD         [24] 1142 	jnb	_TXBMT1,00101$
                           0000C2  1143 	C$Si100x_TxTone.c$342$1$24 ==.
                                   1144 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:342: SPI1DAT = value;                    // write value
      00012F 85 12 86         [24] 1145 	mov	_SPI1DAT,_phyWrite_PARM_2
                           0000C5  1146 	C$Si100x_TxTone.c$343$1$24 ==.
                                   1147 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:343: while(!TXBMT1);                     // wait on TXBMT
      000132                       1148 00104$:
      000132 30 B1 FD         [24] 1149 	jnb	_TXBMT1,00104$
                           0000C8  1150 	C$Si100x_TxTone.c$344$1$24 ==.
                                   1151 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:344: while((SPI1CFG & 0x80) == 0x80);    // wait on SPIBSY
      000135                       1152 00107$:
      000135 74 80            [12] 1153 	mov	a,#0x80
      000137 55 84            [12] 1154 	anl	a,_SPI1CFG
      000139 FF               [12] 1155 	mov	r7,a
      00013A BF 80 02         [24] 1156 	cjne	r7,#0x80,00129$
      00013D 80 F6            [24] 1157 	sjmp	00107$
      00013F                       1158 00129$:
                           0000D2  1159 	C$Si100x_TxTone.c$346$1$24 ==.
                                   1160 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:346: SPIF1 = 0;                          // leave SPIF cleared
                                   1161 ;	assignBit
      00013F C2 B7            [12] 1162 	clr	_SPIF1
                           0000D4  1163 	C$Si100x_TxTone.c$347$1$24 ==.
                                   1164 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:347: NSS1 = 1;                           // drive NSS high
                                   1165 ;	assignBit
      000141 D2 94            [12] 1166 	setb	_NSS1
                           0000D6  1167 	C$Si100x_TxTone.c$348$1$24 ==.
                           0000D6  1168 	XG$phyWrite$0$0 ==.
      000143 22               [24] 1169 	ret
                                   1170 ;------------------------------------------------------------
                                   1171 ;Allocation info for local variables in function 'phyRead'
                                   1172 ;------------------------------------------------------------
                                   1173 ;reg                       Allocated to registers r7 
                                   1174 ;value                     Allocated to registers 
                                   1175 ;------------------------------------------------------------
                           0000D7  1176 	G$phyRead$0$0 ==.
                           0000D7  1177 	C$Si100x_TxTone.c$357$1$24 ==.
                                   1178 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:357: U8 phyRead (U8 reg)
                                   1179 ;	-----------------------------------------
                                   1180 ;	 function phyRead
                                   1181 ;	-----------------------------------------
      000144                       1182 _phyRead:
      000144 AF 82            [24] 1183 	mov	r7,dpl
                           0000D9  1184 	C$Si100x_TxTone.c$362$1$26 ==.
                                   1185 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:362: NSS1 = 0;                           // drive NSS low
                                   1186 ;	assignBit
      000146 C2 94            [12] 1187 	clr	_NSS1
                           0000DB  1188 	C$Si100x_TxTone.c$363$1$26 ==.
                                   1189 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:363: SPIF1 = 0;                          // cleat SPIF
                                   1190 ;	assignBit
      000148 C2 B7            [12] 1191 	clr	_SPIF1
                           0000DD  1192 	C$Si100x_TxTone.c$364$1$26 ==.
                                   1193 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:364: SPI1DAT = ( reg );                  // write reg address
      00014A 8F 86            [24] 1194 	mov	_SPI1DAT,r7
                           0000DF  1195 	C$Si100x_TxTone.c$365$1$26 ==.
                                   1196 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:365: while(!TXBMT1);                     // wait on TXBMT
      00014C                       1197 00101$:
      00014C 30 B1 FD         [24] 1198 	jnb	_TXBMT1,00101$
                           0000E2  1199 	C$Si100x_TxTone.c$366$1$26 ==.
                                   1200 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:366: SPI1DAT = 0x00;                     // write anything
      00014F 75 86 00         [24] 1201 	mov	_SPI1DAT,#0x00
                           0000E5  1202 	C$Si100x_TxTone.c$367$1$26 ==.
                                   1203 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:367: while(!TXBMT1);                     // wait on TXBMT
      000152                       1204 00104$:
      000152 30 B1 FD         [24] 1205 	jnb	_TXBMT1,00104$
                           0000E8  1206 	C$Si100x_TxTone.c$368$1$26 ==.
                                   1207 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:368: while((SPI1CFG & 0x80) == 0x80);    // wait on SPIBSY
      000155                       1208 00107$:
      000155 74 80            [12] 1209 	mov	a,#0x80
      000157 55 84            [12] 1210 	anl	a,_SPI1CFG
      000159 FF               [12] 1211 	mov	r7,a
      00015A BF 80 02         [24] 1212 	cjne	r7,#0x80,00129$
      00015D 80 F6            [24] 1213 	sjmp	00107$
      00015F                       1214 00129$:
                           0000F2  1215 	C$Si100x_TxTone.c$369$1$26 ==.
                                   1216 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:369: value = SPI1DAT;                    // read value
      00015F 85 86 82         [24] 1217 	mov	dpl,_SPI1DAT
                           0000F5  1218 	C$Si100x_TxTone.c$370$1$26 ==.
                                   1219 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:370: SPIF1 = 0;                          // leave SPIF cleared
                                   1220 ;	assignBit
      000162 C2 B7            [12] 1221 	clr	_SPIF1
                           0000F7  1222 	C$Si100x_TxTone.c$371$1$26 ==.
                                   1223 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:371: NSS1 = 1;                           // drive NSS low
                                   1224 ;	assignBit
      000164 D2 94            [12] 1225 	setb	_NSS1
                           0000F9  1226 	C$Si100x_TxTone.c$373$1$26 ==.
                                   1227 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:373: return value;
                           0000F9  1228 	C$Si100x_TxTone.c$374$1$26 ==.
                           0000F9  1229 	XG$phyRead$0$0 ==.
      000166 22               [24] 1230 	ret
                                   1231 ;------------------------------------------------------------
                                   1232 ;Allocation info for local variables in function 'RadioInit'
                                   1233 ;------------------------------------------------------------
                                   1234 ;status                    Allocated to registers r7 
                                   1235 ;------------------------------------------------------------
                           0000FA  1236 	G$RadioInit$0$0 ==.
                           0000FA  1237 	C$Si100x_TxTone.c$393$1$26 ==.
                                   1238 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:393: U8 RadioInit(void)
                                   1239 ;	-----------------------------------------
                                   1240 ;	 function RadioInit
                                   1241 ;	-----------------------------------------
      000167                       1242 _RadioInit:
                           0000FA  1243 	C$Si100x_TxTone.c$397$1$28 ==.
                                   1244 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:397: SDN = 0;
                                   1245 ;	assignBit
      000167 C2 A6            [12] 1246 	clr	_SDN
                           0000FC  1247 	C$Si100x_TxTone.c$399$1$28 ==.
                                   1248 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:399: delay(TWENTY_FIVE_MS);
      000169 90 14 58         [24] 1249 	mov	dptr,#0x1458
      00016C 12 02 2E         [24] 1250 	lcall	_delay
                           000102  1251 	C$Si100x_TxTone.c$401$1$28 ==.
                                   1252 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:401: status = phyRead(EZRADIOPRO_DEVICE_VERSION); // check version
      00016F 75 82 01         [24] 1253 	mov	dpl,#0x01
      000172 12 01 44         [24] 1254 	lcall	_phyRead
      000175 AF 82            [24] 1255 	mov	r7,dpl
                           00010A  1256 	C$Si100x_TxTone.c$402$1$28 ==.
                                   1257 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:402: if(status == 0xFF)
      000177 BF FF 05         [24] 1258 	cjne	r7,#0xff,00107$
                           00010D  1259 	C$Si100x_TxTone.c$403$1$28 ==.
                                   1260 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:403: return RADIO_INIT_ERROR_SPI;
      00017A 75 82 02         [24] 1261 	mov	dpl,#0x02
      00017D 80 47            [24] 1262 	sjmp	00119$
      00017F                       1263 00107$:
                           000112  1264 	C$Si100x_TxTone.c$404$1$28 ==.
                                   1265 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:404: else if (status == 0x00)
      00017F EF               [12] 1266 	mov	a,r7
      000180 70 05            [24] 1267 	jnz	00104$
                           000115  1268 	C$Si100x_TxTone.c$405$1$28 ==.
                                   1269 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:405: return  RADIO_INIT_ERROR_SPI;
      000182 75 82 02         [24] 1270 	mov	dpl,#0x02
      000185 80 3F            [24] 1271 	sjmp	00119$
      000187                       1272 00104$:
                           00011A  1273 	C$Si100x_TxTone.c$406$1$28 ==.
                                   1274 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:406: else if (status < MIN_RADIO_VERSION)
      000187 BF 05 00         [24] 1275 	cjne	r7,#0x05,00148$
      00018A                       1276 00148$:
      00018A 50 05            [24] 1277 	jnc	00108$
                           00011F  1278 	C$Si100x_TxTone.c$407$1$28 ==.
                                   1279 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:407: return RADIO_INIT_ERROR_VERSION;
      00018C 75 82 03         [24] 1280 	mov	dpl,#0x03
      00018F 80 35            [24] 1281 	sjmp	00119$
      000191                       1282 00108$:
                           000124  1283 	C$Si100x_TxTone.c$409$1$28 ==.
                                   1284 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:409: status = phyRead(EZRADIOPRO_INTERRUPT_STATUS_2);
      000191 75 82 04         [24] 1285 	mov	dpl,#0x04
      000194 12 01 44         [24] 1286 	lcall	_phyRead
                           00012A  1287 	C$Si100x_TxTone.c$411$1$28 ==.
                                   1288 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:411: if((status & EZRADIOPRO_IPOR)==0)
      000197 E5 82            [12] 1289 	mov	a,dpl
      000199 FF               [12] 1290 	mov	r7,a
      00019A 20 E0 05         [24] 1291 	jb	acc.0,00117$
                           000130  1292 	C$Si100x_TxTone.c$414$2$29 ==.
                                   1293 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:414: return RadioInitSoftwareReset();
      00019D 12 01 C7         [24] 1294 	lcall	_RadioInitSoftwareReset
      0001A0 80 24            [24] 1295 	sjmp	00119$
      0001A2                       1296 00117$:
                           000135  1297 	C$Si100x_TxTone.c$416$1$28 ==.
                                   1298 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:416: else if((status & EZRADIOPRO_ICHIPRDY)==0)
      0001A2 EF               [12] 1299 	mov	a,r7
      0001A3 20 E1 1D         [24] 1300 	jb	acc.1,00118$
                           000139  1301 	C$Si100x_TxTone.c$419$2$30 ==.
                                   1302 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:419: phyWrite(EZRADIOPRO_INTERRUPT_ENABLE_2, EZRADIOPRO_ENCHIPRDY);
      0001A6 75 12 02         [24] 1303 	mov	_phyWrite_PARM_2,#0x02
      0001A9 75 82 06         [24] 1304 	mov	dpl,#0x06
      0001AC 12 01 21         [24] 1305 	lcall	_phyWrite
                           000142  1306 	C$Si100x_TxTone.c$422$2$30 ==.
                                   1307 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:422: SetTimeOut(TWO_MS);
      0001AF 90 01 A0         [24] 1308 	mov	dptr,#0x01a0
      0001B2 12 02 57         [24] 1309 	lcall	_SetTimeOut
                           000148  1310 	C$Si100x_TxTone.c$423$2$30 ==.
                                   1311 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:423: while(IRQ)
      0001B5                       1312 00111$:
      0001B5 30 81 08         [24] 1313 	jnb	_IRQ,00113$
                           00014B  1314 	C$Si100x_TxTone.c$425$3$31 ==.
                                   1315 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:425: if(TIMEOUT_T0)
      0001B8 20 A9 FA         [24] 1316 	jb	_ET0,00111$
                           00014E  1317 	C$Si100x_TxTone.c$426$3$31 ==.
                                   1318 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:426: return RADIO_INIT_ERROR_XTAL;
      0001BB 75 82 04         [24] 1319 	mov	dpl,#0x04
      0001BE 80 06            [24] 1320 	sjmp	00119$
      0001C0                       1321 00113$:
                           000153  1322 	C$Si100x_TxTone.c$428$2$30 ==.
                                   1323 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:428: ClearTimeOut();
      0001C0 12 02 82         [24] 1324 	lcall	_ClearTimeOut
      0001C3                       1325 00118$:
                           000156  1326 	C$Si100x_TxTone.c$430$1$28 ==.
                                   1327 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:430: return RADIO_INIT_SUCCESS; // success
      0001C3 75 82 00         [24] 1328 	mov	dpl,#0x00
      0001C6                       1329 00119$:
                           000159  1330 	C$Si100x_TxTone.c$431$1$28 ==.
                           000159  1331 	XG$RadioInit$0$0 ==.
      0001C6 22               [24] 1332 	ret
                                   1333 ;------------------------------------------------------------
                                   1334 ;Allocation info for local variables in function 'RadioInitSoftwareReset'
                                   1335 ;------------------------------------------------------------
                                   1336 ;status                    Allocated to registers 
                                   1337 ;------------------------------------------------------------
                           00015A  1338 	G$RadioInitSoftwareReset$0$0 ==.
                           00015A  1339 	C$Si100x_TxTone.c$449$1$28 ==.
                                   1340 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:449: U8 RadioInitSoftwareReset(void)
                                   1341 ;	-----------------------------------------
                                   1342 ;	 function RadioInitSoftwareReset
                                   1343 ;	-----------------------------------------
      0001C7                       1344 _RadioInitSoftwareReset:
                           00015A  1345 	C$Si100x_TxTone.c$454$1$33 ==.
                                   1346 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:454: phyWrite(EZRADIOPRO_INTERRUPT_ENABLE_1, 0);
      0001C7 75 12 00         [24] 1347 	mov	_phyWrite_PARM_2,#0x00
      0001CA 75 82 05         [24] 1348 	mov	dpl,#0x05
      0001CD 12 01 21         [24] 1349 	lcall	_phyWrite
                           000163  1350 	C$Si100x_TxTone.c$455$1$33 ==.
                                   1351 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:455: phyWrite(EZRADIOPRO_INTERRUPT_ENABLE_2, 0);
      0001D0 75 12 00         [24] 1352 	mov	_phyWrite_PARM_2,#0x00
      0001D3 75 82 06         [24] 1353 	mov	dpl,#0x06
      0001D6 12 01 21         [24] 1354 	lcall	_phyWrite
                           00016C  1355 	C$Si100x_TxTone.c$456$1$33 ==.
                                   1356 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:456: status = phyRead(EZRADIOPRO_INTERRUPT_STATUS_1);
      0001D9 75 82 03         [24] 1357 	mov	dpl,#0x03
      0001DC 12 01 44         [24] 1358 	lcall	_phyRead
                           000172  1359 	C$Si100x_TxTone.c$457$1$33 ==.
                                   1360 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:457: status = phyRead(EZRADIOPRO_INTERRUPT_STATUS_2);
      0001DF 75 82 04         [24] 1361 	mov	dpl,#0x04
      0001E2 12 01 44         [24] 1362 	lcall	_phyRead
                           000178  1363 	C$Si100x_TxTone.c$460$1$33 ==.
                                   1364 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:460: phyWrite(EZRADIOPRO_OPERATING_AND_FUNCTION_CONTROL_1, (EZRADIOPRO_SWRES|EZRADIOPRO_XTON));
      0001E5 75 12 81         [24] 1365 	mov	_phyWrite_PARM_2,#0x81
      0001E8 75 82 07         [24] 1366 	mov	dpl,#0x07
      0001EB 12 01 21         [24] 1367 	lcall	_phyWrite
                           000181  1368 	C$Si100x_TxTone.c$463$1$33 ==.
                                   1369 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:463: SetTimeOut(TWO_MS);
      0001EE 90 01 A0         [24] 1370 	mov	dptr,#0x01a0
      0001F1 12 02 57         [24] 1371 	lcall	_SetTimeOut
                           000187  1372 	C$Si100x_TxTone.c$464$1$33 ==.
                                   1373 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:464: while(IRQ)
      0001F4                       1374 00103$:
      0001F4 30 81 08         [24] 1375 	jnb	_IRQ,00105$
                           00018A  1376 	C$Si100x_TxTone.c$466$2$34 ==.
                                   1377 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:466: if(TIMEOUT_T0)
      0001F7 20 A9 FA         [24] 1378 	jb	_ET0,00103$
                           00018D  1379 	C$Si100x_TxTone.c$467$2$34 ==.
                                   1380 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:467: return RADIO_INIT_ERROR_NO_IRQ;
      0001FA 75 82 01         [24] 1381 	mov	dpl,#0x01
      0001FD 80 2E            [24] 1382 	sjmp	00113$
      0001FF                       1383 00105$:
                           000192  1384 	C$Si100x_TxTone.c$469$1$33 ==.
                                   1385 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:469: ClearTimeOut();
      0001FF 12 02 82         [24] 1386 	lcall	_ClearTimeOut
                           000195  1387 	C$Si100x_TxTone.c$471$1$33 ==.
                                   1388 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:471: status = phyRead(EZRADIOPRO_INTERRUPT_STATUS_2);
      000202 75 82 04         [24] 1389 	mov	dpl,#0x04
      000205 12 01 44         [24] 1390 	lcall	_phyRead
      000208 E5 82            [12] 1391 	mov	a,dpl
                           00019D  1392 	C$Si100x_TxTone.c$473$1$33 ==.
                                   1393 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:473: if((status & EZRADIOPRO_ICHIPRDY)==0)
      00020A 20 E1 1D         [24] 1394 	jb	acc.1,00112$
                           0001A0  1395 	C$Si100x_TxTone.c$476$2$35 ==.
                                   1396 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:476: phyWrite(EZRADIOPRO_INTERRUPT_ENABLE_2, EZRADIOPRO_ENCHIPRDY);
      00020D 75 12 02         [24] 1397 	mov	_phyWrite_PARM_2,#0x02
      000210 75 82 06         [24] 1398 	mov	dpl,#0x06
      000213 12 01 21         [24] 1399 	lcall	_phyWrite
                           0001A9  1400 	C$Si100x_TxTone.c$479$2$35 ==.
                                   1401 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:479: SetTimeOut(TWO_MS);
      000216 90 01 A0         [24] 1402 	mov	dptr,#0x01a0
      000219 12 02 57         [24] 1403 	lcall	_SetTimeOut
                           0001AF  1404 	C$Si100x_TxTone.c$480$2$35 ==.
                                   1405 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:480: while(IRQ)
      00021C                       1406 00108$:
      00021C 30 81 08         [24] 1407 	jnb	_IRQ,00110$
                           0001B2  1408 	C$Si100x_TxTone.c$482$3$36 ==.
                                   1409 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:482: if(TIMEOUT_T0)
      00021F 20 A9 FA         [24] 1410 	jb	_ET0,00108$
                           0001B5  1411 	C$Si100x_TxTone.c$483$3$36 ==.
                                   1412 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:483: return RADIO_INIT_ERROR_XTAL;
      000222 75 82 04         [24] 1413 	mov	dpl,#0x04
      000225 80 06            [24] 1414 	sjmp	00113$
      000227                       1415 00110$:
                           0001BA  1416 	C$Si100x_TxTone.c$485$2$35 ==.
                                   1417 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:485: ClearTimeOut();
      000227 12 02 82         [24] 1418 	lcall	_ClearTimeOut
      00022A                       1419 00112$:
                           0001BD  1420 	C$Si100x_TxTone.c$488$1$33 ==.
                                   1421 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:488: return RADIO_INIT_SUCCESS; // success
      00022A 75 82 00         [24] 1422 	mov	dpl,#0x00
      00022D                       1423 00113$:
                           0001C0  1424 	C$Si100x_TxTone.c$489$1$33 ==.
                           0001C0  1425 	XG$RadioInitSoftwareReset$0$0 ==.
      00022D 22               [24] 1426 	ret
                                   1427 ;------------------------------------------------------------
                                   1428 ;Allocation info for local variables in function 'delay'
                                   1429 ;------------------------------------------------------------
                                   1430 ;ticks                     Allocated to registers r6 r7 
                                   1431 ;reload                    Allocated with name '_delay_reload_1_38'
                                   1432 ;------------------------------------------------------------
                           0001C1  1433 	G$delay$0$0 ==.
                           0001C1  1434 	C$Si100x_TxTone.c$499$1$33 ==.
                                   1435 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:499: void delay (U16 ticks)
                                   1436 ;	-----------------------------------------
                                   1437 ;	 function delay
                                   1438 ;	-----------------------------------------
      00022E                       1439 _delay:
      00022E AE 82            [24] 1440 	mov	r6,dpl
      000230 AF 83            [24] 1441 	mov	r7,dph
                           0001C5  1442 	C$Si100x_TxTone.c$503$1$38 ==.
                                   1443 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:503: reload.U16 = -ticks;
      000232 C3               [12] 1444 	clr	c
      000233 E4               [12] 1445 	clr	a
      000234 9E               [12] 1446 	subb	a,r6
      000235 FE               [12] 1447 	mov	r6,a
      000236 E4               [12] 1448 	clr	a
      000237 9F               [12] 1449 	subb	a,r7
      000238 FF               [12] 1450 	mov	r7,a
      000239 8E 12            [24] 1451 	mov	(_delay_reload_1_38 + 0),r6
      00023B 8F 13            [24] 1452 	mov	(_delay_reload_1_38 + 1),r7
                           0001D0  1453 	C$Si100x_TxTone.c$505$1$38 ==.
                                   1454 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:505: TR0 = 0;
                                   1455 ;	assignBit
      00023D C2 8C            [12] 1456 	clr	_TR0
                           0001D2  1457 	C$Si100x_TxTone.c$506$1$38 ==.
                                   1458 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:506: TF0 = 0;
                                   1459 ;	assignBit
      00023F C2 8D            [12] 1460 	clr	_TF0
                           0001D4  1461 	C$Si100x_TxTone.c$508$1$38 ==.
                                   1462 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:508: TMOD      = 0x01;    // T0 uses prescaler
      000241 75 89 01         [24] 1463 	mov	_TMOD,#0x01
                           0001D7  1464 	C$Si100x_TxTone.c$509$1$38 ==.
                                   1465 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:509: CKCON     = 0x02;    // divide by 48 prescaler
      000244 75 8E 02         [24] 1466 	mov	_CKCON,#0x02
                           0001DA  1467 	C$Si100x_TxTone.c$511$1$38 ==.
                                   1468 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:511: TL0     = reload.U8[LSB];
      000247 85 12 8A         [24] 1469 	mov	_TL0,_delay_reload_1_38
                           0001DD  1470 	C$Si100x_TxTone.c$512$1$38 ==.
                                   1471 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:512: TH0     = reload.U8[MSB];
      00024A 85 13 8C         [24] 1472 	mov	_TH0,(_delay_reload_1_38 + 0x0001)
                           0001E0  1473 	C$Si100x_TxTone.c$514$1$38 ==.
                                   1474 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:514: TR0 = 1;
                                   1475 ;	assignBit
      00024D D2 8C            [12] 1476 	setb	_TR0
                           0001E2  1477 	C$Si100x_TxTone.c$515$1$38 ==.
                                   1478 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:515: while (!TF0);
      00024F                       1479 00101$:
      00024F 30 8D FD         [24] 1480 	jnb	_TF0,00101$
                           0001E5  1481 	C$Si100x_TxTone.c$516$1$38 ==.
                                   1482 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:516: TR0 = 0;
                                   1483 ;	assignBit
      000252 C2 8C            [12] 1484 	clr	_TR0
                           0001E7  1485 	C$Si100x_TxTone.c$517$1$38 ==.
                                   1486 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:517: TF0 = 0;
                                   1487 ;	assignBit
      000254 C2 8D            [12] 1488 	clr	_TF0
                           0001E9  1489 	C$Si100x_TxTone.c$518$1$38 ==.
                           0001E9  1490 	XG$delay$0$0 ==.
      000256 22               [24] 1491 	ret
                                   1492 ;------------------------------------------------------------
                                   1493 ;Allocation info for local variables in function 'SetTimeOut'
                                   1494 ;------------------------------------------------------------
                                   1495 ;ticks                     Allocated to registers r6 r7 
                                   1496 ;reload                    Allocated with name '_SetTimeOut_reload_1_40'
                                   1497 ;------------------------------------------------------------
                           0001EA  1498 	G$SetTimeOut$0$0 ==.
                           0001EA  1499 	C$Si100x_TxTone.c$529$1$38 ==.
                                   1500 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:529: void SetTimeOut (U16 ticks)
                                   1501 ;	-----------------------------------------
                                   1502 ;	 function SetTimeOut
                                   1503 ;	-----------------------------------------
      000257                       1504 _SetTimeOut:
      000257 AE 82            [24] 1505 	mov	r6,dpl
      000259 AF 83            [24] 1506 	mov	r7,dph
                           0001EE  1507 	C$Si100x_TxTone.c$533$1$40 ==.
                                   1508 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:533: reload.U16 = -ticks;
      00025B C3               [12] 1509 	clr	c
      00025C E4               [12] 1510 	clr	a
      00025D 9E               [12] 1511 	subb	a,r6
      00025E FE               [12] 1512 	mov	r6,a
      00025F E4               [12] 1513 	clr	a
      000260 9F               [12] 1514 	subb	a,r7
      000261 FF               [12] 1515 	mov	r7,a
      000262 8E 12            [24] 1516 	mov	(_SetTimeOut_reload_1_40 + 0),r6
      000264 8F 13            [24] 1517 	mov	(_SetTimeOut_reload_1_40 + 1),r7
                           0001F9  1518 	C$Si100x_TxTone.c$535$1$40 ==.
                                   1519 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:535: TR0 = 0;
                                   1520 ;	assignBit
      000266 C2 8C            [12] 1521 	clr	_TR0
                           0001FB  1522 	C$Si100x_TxTone.c$536$1$40 ==.
                                   1523 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:536: TF0 = 0;
                                   1524 ;	assignBit
      000268 C2 8D            [12] 1525 	clr	_TF0
                           0001FD  1526 	C$Si100x_TxTone.c$538$1$40 ==.
                                   1527 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:538: TMOD      = 0x01;    // T0 uses prescaler
      00026A 75 89 01         [24] 1528 	mov	_TMOD,#0x01
                           000200  1529 	C$Si100x_TxTone.c$539$1$40 ==.
                                   1530 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:539: CKCON     = 0x02;    // divide by 48 prescaler
      00026D 75 8E 02         [24] 1531 	mov	_CKCON,#0x02
                           000203  1532 	C$Si100x_TxTone.c$541$1$40 ==.
                                   1533 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:541: TL0     = reload.U8[LSB];
      000270 85 12 8A         [24] 1534 	mov	_TL0,_SetTimeOut_reload_1_40
                           000206  1535 	C$Si100x_TxTone.c$542$1$40 ==.
                                   1536 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:542: TH0     = reload.U8[MSB];
      000273 85 13 8C         [24] 1537 	mov	_TH0,(_SetTimeOut_reload_1_40 + 0x0001)
                           000209  1538 	C$Si100x_TxTone.c$544$1$40 ==.
                                   1539 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:544: TR0 = 1;
                                   1540 ;	assignBit
      000276 D2 8C            [12] 1541 	setb	_TR0
                           00020B  1542 	C$Si100x_TxTone.c$545$1$40 ==.
                                   1543 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:545: ET0 = 1;
                                   1544 ;	assignBit
      000278 D2 A9            [12] 1545 	setb	_ET0
                           00020D  1546 	C$Si100x_TxTone.c$546$1$40 ==.
                           00020D  1547 	XG$SetTimeOut$0$0 ==.
      00027A 22               [24] 1548 	ret
                                   1549 ;------------------------------------------------------------
                                   1550 ;Allocation info for local variables in function 'T0_ISR'
                                   1551 ;------------------------------------------------------------
                           00020E  1552 	G$T0_ISR$0$0 ==.
                           00020E  1553 	C$Si100x_TxTone.c$558$1$40 ==.
                                   1554 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:558: INTERRUPT(T0_ISR, INTERRUPT_TIMER0)
                                   1555 ;	-----------------------------------------
                                   1556 ;	 function T0_ISR
                                   1557 ;	-----------------------------------------
      00027B                       1558 _T0_ISR:
                           00020E  1559 	C$Si100x_TxTone.c$560$1$42 ==.
                                   1560 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:560: ET0 = 0;
                                   1561 ;	assignBit
      00027B C2 A9            [12] 1562 	clr	_ET0
                           000210  1563 	C$Si100x_TxTone.c$561$1$42 ==.
                                   1564 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:561: TF0 = 0;
                                   1565 ;	assignBit
      00027D C2 8D            [12] 1566 	clr	_TF0
                           000212  1567 	C$Si100x_TxTone.c$562$1$42 ==.
                                   1568 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:562: TR0 = 0;
                                   1569 ;	assignBit
      00027F C2 8C            [12] 1570 	clr	_TR0
                           000214  1571 	C$Si100x_TxTone.c$563$1$42 ==.
                           000214  1572 	XG$T0_ISR$0$0 ==.
      000281 32               [24] 1573 	reti
                                   1574 ;	eliminated unneeded mov psw,# (no regs used in bank)
                                   1575 ;	eliminated unneeded push/pop psw
                                   1576 ;	eliminated unneeded push/pop dpl
                                   1577 ;	eliminated unneeded push/pop dph
                                   1578 ;	eliminated unneeded push/pop b
                                   1579 ;	eliminated unneeded push/pop acc
                                   1580 ;------------------------------------------------------------
                                   1581 ;Allocation info for local variables in function 'ClearTimeOut'
                                   1582 ;------------------------------------------------------------
                           000215  1583 	G$ClearTimeOut$0$0 ==.
                           000215  1584 	C$Si100x_TxTone.c$572$1$42 ==.
                                   1585 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:572: void ClearTimeOut (void)
                                   1586 ;	-----------------------------------------
                                   1587 ;	 function ClearTimeOut
                                   1588 ;	-----------------------------------------
      000282                       1589 _ClearTimeOut:
                           000215  1590 	C$Si100x_TxTone.c$574$1$44 ==.
                                   1591 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:574: ET0 = 0;
                                   1592 ;	assignBit
      000282 C2 A9            [12] 1593 	clr	_ET0
                           000217  1594 	C$Si100x_TxTone.c$575$1$44 ==.
                                   1595 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:575: TF0 = 0;
                                   1596 ;	assignBit
      000284 C2 8D            [12] 1597 	clr	_TF0
                           000219  1598 	C$Si100x_TxTone.c$576$1$44 ==.
                                   1599 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:576: TR0 = 0;
                                   1600 ;	assignBit
      000286 C2 8C            [12] 1601 	clr	_TR0
                           00021B  1602 	C$Si100x_TxTone.c$577$1$44 ==.
                           00021B  1603 	XG$ClearTimeOut$0$0 ==.
      000288 22               [24] 1604 	ret
                                   1605 ;------------------------------------------------------------
                                   1606 ;Allocation info for local variables in function 'setTRxFrequency'
                                   1607 ;------------------------------------------------------------
                                   1608 ;frequency                 Allocated with name '_setTRxFrequency_frequency_1_45'
                                   1609 ;frequencyBandSelect       Allocated to registers r2 
                                   1610 ;nominalCarrierFrequency   Allocated with name '_setTRxFrequency_nominalCarrierFrequency_1_46'
                                   1611 ;------------------------------------------------------------
                           00021C  1612 	G$setTRxFrequency$0$0 ==.
                           00021C  1613 	C$Si100x_TxTone.c$587$1$44 ==.
                                   1614 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:587: void   setTRxFrequency (U32 frequency)
                                   1615 ;	-----------------------------------------
                                   1616 ;	 function setTRxFrequency
                                   1617 ;	-----------------------------------------
      000289                       1618 _setTRxFrequency:
      000289 85 82 08         [24] 1619 	mov	_setTRxFrequency_frequency_1_45,dpl
      00028C 85 83 09         [24] 1620 	mov	(_setTRxFrequency_frequency_1_45 + 1),dph
      00028F 85 F0 0A         [24] 1621 	mov	(_setTRxFrequency_frequency_1_45 + 2),b
      000292 F5 0B            [12] 1622 	mov	(_setTRxFrequency_frequency_1_45 + 3),a
                           000227  1623 	C$Si100x_TxTone.c$592$1$46 ==.
                                   1624 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:592: if (frequency > 480000000L )
      000294 C3               [12] 1625 	clr	c
      000295 E4               [12] 1626 	clr	a
      000296 95 08            [12] 1627 	subb	a,_setTRxFrequency_frequency_1_45
      000298 74 38            [12] 1628 	mov	a,#0x38
      00029A 95 09            [12] 1629 	subb	a,(_setTRxFrequency_frequency_1_45 + 1)
      00029C 74 9C            [12] 1630 	mov	a,#0x9c
      00029E 95 0A            [12] 1631 	subb	a,(_setTRxFrequency_frequency_1_45 + 2)
      0002A0 74 1C            [12] 1632 	mov	a,#0x1c
      0002A2 95 0B            [12] 1633 	subb	a,(_setTRxFrequency_frequency_1_45 + 3)
      0002A4 40 03            [24] 1634 	jc	00109$
      0002A6 02 03 4C         [24] 1635 	ljmp	00102$
      0002A9                       1636 00109$:
                           00023C  1637 	C$Si100x_TxTone.c$594$2$47 ==.
                                   1638 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:594: frequency -= 480000000L;
      0002A9 E5 09            [12] 1639 	mov	a,(_setTRxFrequency_frequency_1_45 + 1)
      0002AB 24 C8            [12] 1640 	add	a,#0xc8
      0002AD F5 09            [12] 1641 	mov	(_setTRxFrequency_frequency_1_45 + 1),a
      0002AF E5 0A            [12] 1642 	mov	a,(_setTRxFrequency_frequency_1_45 + 2)
      0002B1 34 63            [12] 1643 	addc	a,#0x63
      0002B3 F5 0A            [12] 1644 	mov	(_setTRxFrequency_frequency_1_45 + 2),a
      0002B5 E5 0B            [12] 1645 	mov	a,(_setTRxFrequency_frequency_1_45 + 3)
      0002B7 34 E3            [12] 1646 	addc	a,#0xe3
      0002B9 F5 0B            [12] 1647 	mov	(_setTRxFrequency_frequency_1_45 + 3),a
                           00024E  1648 	C$Si100x_TxTone.c$595$1$46 ==.
                                   1649 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:595: frequencyBandSelect  = frequency / 20000000L;
      0002BB 75 12 00         [24] 1650 	mov	__divulong_PARM_2,#0x00
      0002BE 75 13 2D         [24] 1651 	mov	(__divulong_PARM_2 + 1),#0x2d
      0002C1 75 14 31         [24] 1652 	mov	(__divulong_PARM_2 + 2),#0x31
      0002C4 75 15 01         [24] 1653 	mov	(__divulong_PARM_2 + 3),#0x01
      0002C7 85 08 82         [24] 1654 	mov	dpl,_setTRxFrequency_frequency_1_45
      0002CA 85 09 83         [24] 1655 	mov	dph,(_setTRxFrequency_frequency_1_45 + 1)
      0002CD 85 0A F0         [24] 1656 	mov	b,(_setTRxFrequency_frequency_1_45 + 2)
      0002D0 E5 0B            [12] 1657 	mov	a,(_setTRxFrequency_frequency_1_45 + 3)
      0002D2 12 04 6C         [24] 1658 	lcall	__divulong
      0002D5 AA 82            [24] 1659 	mov	r2,dpl
                           00026A  1660 	C$Si100x_TxTone.c$596$2$47 ==.
                                   1661 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:596: frequency -= (U32)frequencyBandSelect * 20000000L;
      0002D7 8A 12            [24] 1662 	mov	__mullong_PARM_2,r2
      0002D9 75 13 00         [24] 1663 	mov	(__mullong_PARM_2 + 1),#0x00
      0002DC 75 14 00         [24] 1664 	mov	(__mullong_PARM_2 + 2),#0x00
      0002DF 75 15 00         [24] 1665 	mov	(__mullong_PARM_2 + 3),#0x00
      0002E2 90 2D 00         [24] 1666 	mov	dptr,#0x2d00
      0002E5 75 F0 31         [24] 1667 	mov	b,#0x31
      0002E8 74 01            [12] 1668 	mov	a,#0x01
      0002EA C0 02            [24] 1669 	push	ar2
      0002EC 12 04 D1         [24] 1670 	lcall	__mullong
      0002EF AC 82            [24] 1671 	mov	r4,dpl
      0002F1 AD 83            [24] 1672 	mov	r5,dph
      0002F3 AE F0            [24] 1673 	mov	r6,b
      0002F5 FF               [12] 1674 	mov	r7,a
      0002F6 E5 08            [12] 1675 	mov	a,_setTRxFrequency_frequency_1_45
      0002F8 C3               [12] 1676 	clr	c
      0002F9 9C               [12] 1677 	subb	a,r4
      0002FA F5 08            [12] 1678 	mov	_setTRxFrequency_frequency_1_45,a
      0002FC E5 09            [12] 1679 	mov	a,(_setTRxFrequency_frequency_1_45 + 1)
      0002FE 9D               [12] 1680 	subb	a,r5
      0002FF F5 09            [12] 1681 	mov	(_setTRxFrequency_frequency_1_45 + 1),a
      000301 E5 0A            [12] 1682 	mov	a,(_setTRxFrequency_frequency_1_45 + 2)
      000303 9E               [12] 1683 	subb	a,r6
      000304 F5 0A            [12] 1684 	mov	(_setTRxFrequency_frequency_1_45 + 2),a
      000306 E5 0B            [12] 1685 	mov	a,(_setTRxFrequency_frequency_1_45 + 3)
      000308 9F               [12] 1686 	subb	a,r7
      000309 F5 0B            [12] 1687 	mov	(_setTRxFrequency_frequency_1_45 + 3),a
                           00029E  1688 	C$Si100x_TxTone.c$597$2$47 ==.
                                   1689 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:597: frequency  = divideWithRounding(frequency, 625);
      00030B 75 0E 71         [24] 1690 	mov	_divideWithRounding_PARM_2,#0x71
      00030E 75 0F 02         [24] 1691 	mov	(_divideWithRounding_PARM_2 + 1),#0x02
      000311 E4               [12] 1692 	clr	a
      000312 F5 10            [12] 1693 	mov	(_divideWithRounding_PARM_2 + 2),a
      000314 F5 11            [12] 1694 	mov	(_divideWithRounding_PARM_2 + 3),a
      000316 85 08 82         [24] 1695 	mov	dpl,_setTRxFrequency_frequency_1_45
      000319 85 09 83         [24] 1696 	mov	dph,(_setTRxFrequency_frequency_1_45 + 1)
      00031C 85 0A F0         [24] 1697 	mov	b,(_setTRxFrequency_frequency_1_45 + 2)
      00031F E5 0B            [12] 1698 	mov	a,(_setTRxFrequency_frequency_1_45 + 3)
      000321 12 04 25         [24] 1699 	lcall	_divideWithRounding
      000324 85 82 08         [24] 1700 	mov	_setTRxFrequency_frequency_1_45,dpl
      000327 85 83 09         [24] 1701 	mov	(_setTRxFrequency_frequency_1_45 + 1),dph
      00032A 85 F0 0A         [24] 1702 	mov	(_setTRxFrequency_frequency_1_45 + 2),b
      00032D F5 0B            [12] 1703 	mov	(_setTRxFrequency_frequency_1_45 + 3),a
      00032F D0 02            [24] 1704 	pop	ar2
                           0002C4  1705 	C$Si100x_TxTone.c$598$2$47 ==.
                                   1706 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:598: frequency <<= 1;
      000331 E5 08            [12] 1707 	mov	a,_setTRxFrequency_frequency_1_45
      000333 25 08            [12] 1708 	add	a,_setTRxFrequency_frequency_1_45
      000335 F5 08            [12] 1709 	mov	_setTRxFrequency_frequency_1_45,a
      000337 E5 09            [12] 1710 	mov	a,(_setTRxFrequency_frequency_1_45 + 1)
      000339 33               [12] 1711 	rlc	a
      00033A F5 09            [12] 1712 	mov	(_setTRxFrequency_frequency_1_45 + 1),a
      00033C E5 0A            [12] 1713 	mov	a,(_setTRxFrequency_frequency_1_45 + 2)
      00033E 33               [12] 1714 	rlc	a
      00033F F5 0A            [12] 1715 	mov	(_setTRxFrequency_frequency_1_45 + 2),a
      000341 E5 0B            [12] 1716 	mov	a,(_setTRxFrequency_frequency_1_45 + 3)
      000343 33               [12] 1717 	rlc	a
      000344 F5 0B            [12] 1718 	mov	(_setTRxFrequency_frequency_1_45 + 3),a
                           0002D9  1719 	C$Si100x_TxTone.c$599$2$47 ==.
                                   1720 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:599: frequencyBandSelect |= 0x20;
      000346 43 02 20         [24] 1721 	orl	ar2,#0x20
      000349 02 03 FF         [24] 1722 	ljmp	00103$
      00034C                       1723 00102$:
                           0002DF  1724 	C$Si100x_TxTone.c$603$2$48 ==.
                                   1725 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:603: frequency -= 240000000L;
      00034C E5 09            [12] 1726 	mov	a,(_setTRxFrequency_frequency_1_45 + 1)
      00034E 24 E4            [12] 1727 	add	a,#0xe4
      000350 F5 09            [12] 1728 	mov	(_setTRxFrequency_frequency_1_45 + 1),a
      000352 E5 0A            [12] 1729 	mov	a,(_setTRxFrequency_frequency_1_45 + 2)
      000354 34 B1            [12] 1730 	addc	a,#0xb1
      000356 F5 0A            [12] 1731 	mov	(_setTRxFrequency_frequency_1_45 + 2),a
      000358 E5 0B            [12] 1732 	mov	a,(_setTRxFrequency_frequency_1_45 + 3)
      00035A 34 F1            [12] 1733 	addc	a,#0xf1
      00035C F5 0B            [12] 1734 	mov	(_setTRxFrequency_frequency_1_45 + 3),a
                           0002F1  1735 	C$Si100x_TxTone.c$604$1$46 ==.
                                   1736 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:604: frequencyBandSelect  = frequency / 10000000L;
      00035E 75 12 80         [24] 1737 	mov	__divulong_PARM_2,#0x80
      000361 75 13 96         [24] 1738 	mov	(__divulong_PARM_2 + 1),#0x96
      000364 75 14 98         [24] 1739 	mov	(__divulong_PARM_2 + 2),#0x98
      000367 75 15 00         [24] 1740 	mov	(__divulong_PARM_2 + 3),#0x00
      00036A 85 08 82         [24] 1741 	mov	dpl,_setTRxFrequency_frequency_1_45
      00036D 85 09 83         [24] 1742 	mov	dph,(_setTRxFrequency_frequency_1_45 + 1)
      000370 85 0A F0         [24] 1743 	mov	b,(_setTRxFrequency_frequency_1_45 + 2)
      000373 E5 0B            [12] 1744 	mov	a,(_setTRxFrequency_frequency_1_45 + 3)
      000375 12 04 6C         [24] 1745 	lcall	__divulong
      000378 AC 82            [24] 1746 	mov	r4,dpl
      00037A 8C 02            [24] 1747 	mov	ar2,r4
                           00030F  1748 	C$Si100x_TxTone.c$605$2$48 ==.
                                   1749 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:605: frequency -= (U32)frequencyBandSelect * 10000000L;
      00037C 8A 12            [24] 1750 	mov	__mullong_PARM_2,r2
      00037E 75 13 00         [24] 1751 	mov	(__mullong_PARM_2 + 1),#0x00
      000381 75 14 00         [24] 1752 	mov	(__mullong_PARM_2 + 2),#0x00
      000384 75 15 00         [24] 1753 	mov	(__mullong_PARM_2 + 3),#0x00
      000387 90 96 80         [24] 1754 	mov	dptr,#0x9680
      00038A 75 F0 98         [24] 1755 	mov	b,#0x98
      00038D E4               [12] 1756 	clr	a
      00038E C0 02            [24] 1757 	push	ar2
      000390 12 04 D1         [24] 1758 	lcall	__mullong
      000393 AC 82            [24] 1759 	mov	r4,dpl
      000395 AD 83            [24] 1760 	mov	r5,dph
      000397 AE F0            [24] 1761 	mov	r6,b
      000399 FF               [12] 1762 	mov	r7,a
      00039A E5 08            [12] 1763 	mov	a,_setTRxFrequency_frequency_1_45
      00039C C3               [12] 1764 	clr	c
      00039D 9C               [12] 1765 	subb	a,r4
      00039E F5 08            [12] 1766 	mov	_setTRxFrequency_frequency_1_45,a
      0003A0 E5 09            [12] 1767 	mov	a,(_setTRxFrequency_frequency_1_45 + 1)
      0003A2 9D               [12] 1768 	subb	a,r5
      0003A3 F5 09            [12] 1769 	mov	(_setTRxFrequency_frequency_1_45 + 1),a
      0003A5 E5 0A            [12] 1770 	mov	a,(_setTRxFrequency_frequency_1_45 + 2)
      0003A7 9E               [12] 1771 	subb	a,r6
      0003A8 F5 0A            [12] 1772 	mov	(_setTRxFrequency_frequency_1_45 + 2),a
      0003AA E5 0B            [12] 1773 	mov	a,(_setTRxFrequency_frequency_1_45 + 3)
      0003AC 9F               [12] 1774 	subb	a,r7
      0003AD F5 0B            [12] 1775 	mov	(_setTRxFrequency_frequency_1_45 + 3),a
                           000342  1776 	C$Si100x_TxTone.c$606$2$48 ==.
                                   1777 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:606: frequency  = divideWithRounding(frequency, 625);
      0003AF 75 0E 71         [24] 1778 	mov	_divideWithRounding_PARM_2,#0x71
      0003B2 75 0F 02         [24] 1779 	mov	(_divideWithRounding_PARM_2 + 1),#0x02
      0003B5 E4               [12] 1780 	clr	a
      0003B6 F5 10            [12] 1781 	mov	(_divideWithRounding_PARM_2 + 2),a
      0003B8 F5 11            [12] 1782 	mov	(_divideWithRounding_PARM_2 + 3),a
      0003BA 85 08 82         [24] 1783 	mov	dpl,_setTRxFrequency_frequency_1_45
      0003BD 85 09 83         [24] 1784 	mov	dph,(_setTRxFrequency_frequency_1_45 + 1)
      0003C0 85 0A F0         [24] 1785 	mov	b,(_setTRxFrequency_frequency_1_45 + 2)
      0003C3 E5 0B            [12] 1786 	mov	a,(_setTRxFrequency_frequency_1_45 + 3)
      0003C5 12 04 25         [24] 1787 	lcall	_divideWithRounding
      0003C8 85 82 08         [24] 1788 	mov	_setTRxFrequency_frequency_1_45,dpl
      0003CB 85 83 09         [24] 1789 	mov	(_setTRxFrequency_frequency_1_45 + 1),dph
      0003CE 85 F0 0A         [24] 1790 	mov	(_setTRxFrequency_frequency_1_45 + 2),b
      0003D1 F5 0B            [12] 1791 	mov	(_setTRxFrequency_frequency_1_45 + 3),a
      0003D3 D0 02            [24] 1792 	pop	ar2
                           000368  1793 	C$Si100x_TxTone.c$607$2$48 ==.
                                   1794 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:607: frequency <<= 2;
      0003D5 E5 08            [12] 1795 	mov	a,_setTRxFrequency_frequency_1_45
      0003D7 25 08            [12] 1796 	add	a,_setTRxFrequency_frequency_1_45
      0003D9 F5 08            [12] 1797 	mov	_setTRxFrequency_frequency_1_45,a
      0003DB E5 09            [12] 1798 	mov	a,(_setTRxFrequency_frequency_1_45 + 1)
      0003DD 33               [12] 1799 	rlc	a
      0003DE F5 09            [12] 1800 	mov	(_setTRxFrequency_frequency_1_45 + 1),a
      0003E0 E5 0A            [12] 1801 	mov	a,(_setTRxFrequency_frequency_1_45 + 2)
      0003E2 33               [12] 1802 	rlc	a
      0003E3 F5 0A            [12] 1803 	mov	(_setTRxFrequency_frequency_1_45 + 2),a
      0003E5 E5 0B            [12] 1804 	mov	a,(_setTRxFrequency_frequency_1_45 + 3)
      0003E7 33               [12] 1805 	rlc	a
      0003E8 F5 0B            [12] 1806 	mov	(_setTRxFrequency_frequency_1_45 + 3),a
      0003EA E5 08            [12] 1807 	mov	a,_setTRxFrequency_frequency_1_45
      0003EC 25 08            [12] 1808 	add	a,_setTRxFrequency_frequency_1_45
      0003EE F5 08            [12] 1809 	mov	_setTRxFrequency_frequency_1_45,a
      0003F0 E5 09            [12] 1810 	mov	a,(_setTRxFrequency_frequency_1_45 + 1)
      0003F2 33               [12] 1811 	rlc	a
      0003F3 F5 09            [12] 1812 	mov	(_setTRxFrequency_frequency_1_45 + 1),a
      0003F5 E5 0A            [12] 1813 	mov	a,(_setTRxFrequency_frequency_1_45 + 2)
      0003F7 33               [12] 1814 	rlc	a
      0003F8 F5 0A            [12] 1815 	mov	(_setTRxFrequency_frequency_1_45 + 2),a
      0003FA E5 0B            [12] 1816 	mov	a,(_setTRxFrequency_frequency_1_45 + 3)
      0003FC 33               [12] 1817 	rlc	a
      0003FD F5 0B            [12] 1818 	mov	(_setTRxFrequency_frequency_1_45 + 3),a
      0003FF                       1819 00103$:
                           000392  1820 	C$Si100x_TxTone.c$610$1$46 ==.
                                   1821 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:610: frequencyBandSelect |= 0x40;        // set sbsel
      0003FF 74 40            [12] 1822 	mov	a,#0x40
      000401 4A               [12] 1823 	orl	a,r2
      000402 F5 12            [12] 1824 	mov	_phyWrite_PARM_2,a
                           000397  1825 	C$Si100x_TxTone.c$612$1$46 ==.
                                   1826 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:612: nominalCarrierFrequency.U16 = (U16)frequency;
      000404 AC 08            [24] 1827 	mov	r4,_setTRxFrequency_frequency_1_45
      000406 AD 09            [24] 1828 	mov	r5,(_setTRxFrequency_frequency_1_45 + 1)
      000408 8C 0C            [24] 1829 	mov	(_setTRxFrequency_nominalCarrierFrequency_1_46 + 0),r4
      00040A 8D 0D            [24] 1830 	mov	(_setTRxFrequency_nominalCarrierFrequency_1_46 + 1),r5
                           00039F  1831 	C$Si100x_TxTone.c$614$1$46 ==.
                                   1832 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:614: phyWrite(EZRADIOPRO_FREQUENCY_BAND_SELECT, frequencyBandSelect);
      00040C 75 82 75         [24] 1833 	mov	dpl,#0x75
      00040F 12 01 21         [24] 1834 	lcall	_phyWrite
                           0003A5  1835 	C$Si100x_TxTone.c$615$1$46 ==.
                                   1836 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:615: phyWrite(EZRADIOPRO_NOMINAL_CARRIER_FREQUENCY_1, nominalCarrierFrequency.U8[MSB]);
      000412 85 0D 12         [24] 1837 	mov	_phyWrite_PARM_2,(_setTRxFrequency_nominalCarrierFrequency_1_46 + 0x0001)
      000415 75 82 76         [24] 1838 	mov	dpl,#0x76
      000418 12 01 21         [24] 1839 	lcall	_phyWrite
                           0003AE  1840 	C$Si100x_TxTone.c$616$1$46 ==.
                                   1841 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:616: phyWrite(EZRADIOPRO_NOMINAL_CARRIER_FREQUENCY_0, nominalCarrierFrequency.U8[LSB]);
      00041B 85 0C 12         [24] 1842 	mov	_phyWrite_PARM_2,_setTRxFrequency_nominalCarrierFrequency_1_46
      00041E 75 82 77         [24] 1843 	mov	dpl,#0x77
      000421 12 01 21         [24] 1844 	lcall	_phyWrite
                           0003B7  1845 	C$Si100x_TxTone.c$618$1$46 ==.
                           0003B7  1846 	XG$setTRxFrequency$0$0 ==.
      000424 22               [24] 1847 	ret
                                   1848 ;------------------------------------------------------------
                                   1849 ;Allocation info for local variables in function 'divideWithRounding'
                                   1850 ;------------------------------------------------------------
                                   1851 ;divisor                   Allocated with name '_divideWithRounding_PARM_2'
                                   1852 ;value                     Allocated to registers r4 r5 r6 r7 
                                   1853 ;------------------------------------------------------------
                           0003B8  1854 	G$divideWithRounding$0$0 ==.
                           0003B8  1855 	C$Si100x_TxTone.c$628$1$46 ==.
                                   1856 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:628: U32   divideWithRounding (U32 value, U32 divisor)
                                   1857 ;	-----------------------------------------
                                   1858 ;	 function divideWithRounding
                                   1859 ;	-----------------------------------------
      000425                       1860 _divideWithRounding:
      000425 AC 82            [24] 1861 	mov	r4,dpl
      000427 AD 83            [24] 1862 	mov	r5,dph
      000429 AE F0            [24] 1863 	mov	r6,b
      00042B FF               [12] 1864 	mov	r7,a
                           0003BF  1865 	C$Si100x_TxTone.c$630$1$50 ==.
                                   1866 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:630: value += (divisor >> 1);
      00042C E5 11            [12] 1867 	mov	a,(_divideWithRounding_PARM_2 + 3)
      00042E C3               [12] 1868 	clr	c
      00042F 13               [12] 1869 	rrc	a
      000430 FB               [12] 1870 	mov	r3,a
      000431 E5 10            [12] 1871 	mov	a,(_divideWithRounding_PARM_2 + 2)
      000433 13               [12] 1872 	rrc	a
      000434 FA               [12] 1873 	mov	r2,a
      000435 E5 0F            [12] 1874 	mov	a,(_divideWithRounding_PARM_2 + 1)
      000437 13               [12] 1875 	rrc	a
      000438 F9               [12] 1876 	mov	r1,a
      000439 E5 0E            [12] 1877 	mov	a,_divideWithRounding_PARM_2
      00043B 13               [12] 1878 	rrc	a
      00043C 2C               [12] 1879 	add	a,r4
      00043D FC               [12] 1880 	mov	r4,a
      00043E E9               [12] 1881 	mov	a,r1
      00043F 3D               [12] 1882 	addc	a,r5
      000440 FD               [12] 1883 	mov	r5,a
      000441 EA               [12] 1884 	mov	a,r2
      000442 3E               [12] 1885 	addc	a,r6
      000443 FE               [12] 1886 	mov	r6,a
      000444 EB               [12] 1887 	mov	a,r3
      000445 3F               [12] 1888 	addc	a,r7
      000446 FF               [12] 1889 	mov	r7,a
                           0003DA  1890 	C$Si100x_TxTone.c$631$1$50 ==.
                                   1891 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:631: value /= divisor;
      000447 85 0E 12         [24] 1892 	mov	__divulong_PARM_2,_divideWithRounding_PARM_2
      00044A 85 0F 13         [24] 1893 	mov	(__divulong_PARM_2 + 1),(_divideWithRounding_PARM_2 + 1)
      00044D 85 10 14         [24] 1894 	mov	(__divulong_PARM_2 + 2),(_divideWithRounding_PARM_2 + 2)
      000450 85 11 15         [24] 1895 	mov	(__divulong_PARM_2 + 3),(_divideWithRounding_PARM_2 + 3)
      000453 8C 82            [24] 1896 	mov	dpl,r4
      000455 8D 83            [24] 1897 	mov	dph,r5
      000457 8E F0            [24] 1898 	mov	b,r6
      000459 EF               [12] 1899 	mov	a,r7
      00045A 12 04 6C         [24] 1900 	lcall	__divulong
                           0003F0  1901 	C$Si100x_TxTone.c$632$1$50 ==.
                                   1902 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:632: return value;
                           0003F0  1903 	C$Si100x_TxTone.c$633$1$50 ==.
                           0003F0  1904 	XG$divideWithRounding$0$0 ==.
      00045D 22               [24] 1905 	ret
                                   1906 ;------------------------------------------------------------
                                   1907 ;Allocation info for local variables in function 'setTxPower'
                                   1908 ;------------------------------------------------------------
                                   1909 ;power                     Allocated to registers r7 
                                   1910 ;------------------------------------------------------------
                           0003F1  1911 	G$setTxPower$0$0 ==.
                           0003F1  1912 	C$Si100x_TxTone.c$643$1$50 ==.
                                   1913 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:643: void setTxPower (U8 power)
                                   1914 ;	-----------------------------------------
                                   1915 ;	 function setTxPower
                                   1916 ;	-----------------------------------------
      00045E                       1917 _setTxPower:
      00045E AF 82            [24] 1918 	mov	r7,dpl
                           0003F3  1919 	C$Si100x_TxTone.c$645$1$52 ==.
                                   1920 ;	C:\SiLabs\MCU\Examples\Si100x\EZRadioPRO\TxTone\Si100x_TxTone.c:645: phyWrite(EZRADIOPRO_TX_POWER, 0x18|power); // set bit 3 and 4
      000460 74 18            [12] 1921 	mov	a,#0x18
      000462 4F               [12] 1922 	orl	a,r7
      000463 F5 12            [12] 1923 	mov	_phyWrite_PARM_2,a
      000465 75 82 6D         [24] 1924 	mov	dpl,#0x6d
      000468 12 01 21         [24] 1925 	lcall	_phyWrite
                           0003FE  1926 	C$Si100x_TxTone.c$646$1$52 ==.
                           0003FE  1927 	XG$setTxPower$0$0 ==.
      00046B 22               [24] 1928 	ret
                                   1929 	.area CSEG    (CODE)
                                   1930 	.area CONST   (CODE)
                                   1931 	.area XINIT   (CODE)
                                   1932 	.area CABS    (ABS,CODE)

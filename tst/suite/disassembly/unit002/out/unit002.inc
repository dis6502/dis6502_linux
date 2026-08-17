;
; System equates
;
; OS EQUATES
; ----------
;
; Syntax:
; Use '=' for addresses
; and '<' for read addresses (ex: KBCODE)
; and '>' for write addresses (ex: STIMER)
; and '#' for values

;
; IO EQUATES
;
ICHID       equ $0000
ICDNO       equ $0001
ICCOM       equ $0002
ICSTA       equ $0003
ICBAL       equ $0004
ICBAH       equ $0005
ICPTL       equ $0006
ICPTH       equ $0007
ICBLL       equ $0008
ICBLH       equ $0009
ICAX1       equ $000A
ICAX2       equ $000B
ICAX3       equ $000C
ICAX4       equ $000D
ICAX5       equ $000E
ICAX6       equ $000F

;
; DISPLAY LIST EQUATES
;
ADLI        equ $0080; ANTIC display list interrupt mask
AVB         equ $0040; ANTIC vertical blank mask
ALMS        equ $0040; ANTIC load memory scan
AVSCR       equ $0020; ANTIC vertical scroll mask
AHSCR       equ $0010; ANTIC horizontal scroll mask
AJMP        equ $0001; ANTIC jump
AEMPTY1     equ $0000; ANTIC 1 empty scanline
AEMPTY2     equ $0010; ANTIC 2 empty scanlines
AEMPTY3     equ $0020; ANTIC 3 empty scanlines
AEMPTY4     equ $0030; ANTIC 4 empty scanlines
AEMPTY5     equ $0040; ANTIC 5 empty scanlines
AEMPTY6     equ $0050; ANTIC 6 empty scanlines
AEMPTY7     equ $0060; ANTIC 7 empty scanlines
AEMPTY8     equ $0070; ANTIC 8 empty scanlines

;
; OS VARIABLES FOR XL/XE
;

;
; PAGE 0
;
NGFLAG      equ $0001
CASINI      equ $0002
RAMLO       equ $0004
TRAMSZ      equ $0006
CMCMD       equ $0007
WARMST      equ $0008
BOOT        equ $0009
DOSVEC      equ $000A
DOSINI      equ $000C
APPMHI      equ $000E
POKMSK      equ $0010
BRKKEY      equ $0011
RTCLOK      equ $0012
BUFADR      equ $0015
ICCOMT      equ $0017
DSKFMS      equ $0018
DSKUTL      equ $001A
ABUFPT      equ $001C
ICHIDZ      equ $0020
ICDNOZ      equ $0021
ICCOMZ      equ $0022
ICSTAZ      equ $0023
ICBALZ      equ $0024
ICBAHZ      equ $0025
ICPTLZ      equ $0026
ICPTHZ      equ $0027
ICBLLZ      equ $0028
ICBLHZ      equ $0029
ICAX1Z      equ $002A
ICAX2Z      equ $002B
ICAX3Z      equ $002C
ICAX4Z      equ $002D
ICAX5Z      equ $002E
ICAX6Z      equ $002F
; STATUS   =   $30 Commented out for compatibility with SDX
CHKSUM      equ $0031
BUFRLO      equ $0032
BUFRHI      equ $0033
BFENLO      equ $0034
BFENHI      equ $0035
LTEMP       equ $0036
BUFRFL      equ $0038
RECVDN      equ $0039
XMTDON      equ $003A
CHKSNT      equ $003B
NOCKSM      equ $003C
BPTR        equ $003D
FTYPE       equ $003E
FEOF        equ $003F
FREQ        equ $0040
SOUNDR      equ $0041
CRITIC      equ $0042
FMSZPG      equ $0043
ZCHAIN      equ $004A
DSTAT       equ $004C
ATRACT      equ $004D
DRKMSK      equ $004E
COLRSH      equ $004F
TEMPCHR     equ $0050
HOLD1       equ $0051
LMARGN      equ $0052
RMARGN      equ $0053
ROWCRS      equ $0054
COLCRS      equ $0055
DINDEX      equ $0057
SAVMSC      equ $0058
OLDROW      equ $005A
OLDCOL      equ $005B
OLDCHR      equ $005D
OLDADR      equ $005E
FKDEF       equ $0060
PALNTS      equ $0062
LOGCOL      equ $0063
ADRESS      equ $0064
MLTTMP      equ $0066
SAVADR      equ $0068
RAMTOP      equ $006A; RAM TOP
BUFCNT      equ $006B
BUFSTR      equ $006C
BITMSK      equ $006E
SHFAMT      equ $006F
ROWAC       equ $0070
COLAC       equ $0072
ENDPT       equ $0074
DELTAR      equ $0076
DELTAC      equ $0077
KEYDEF      equ $0079
SWPFLG      equ $007B
HOLDCH      equ $007C
INSDAT      equ $007D
COUNTR      equ $007E
LOMEM       equ $0080

;
; PAGE 2
;
VDSLST      equ $0200
VPRCED      equ $0202
VINTER      equ $0204
VBREAK      equ $0206
VKEYBD      equ $0208
VSERIN      equ $020A
VSEROR      equ $020C
VSEROC      equ $020E
VTIMR1      equ $0210
VTIMR2      equ $0212
VTIMR4      equ $0214
VIMIRQ      equ $0216
CDTMV1      equ $0218
CDTMV2      equ $021A
CDTMV3      equ $021C
CDTMV4      equ $021E
CDTMV5      equ $0220
VVBLKI      equ $0222
VVBLKD      equ $0224
CDTMA1      equ $0226
CDTMA2      equ $0228
CDTMF3      equ $022A
SRTIMR      equ $022B
CDTMF4      equ $022C
INTEMP      equ $022D
CDTMF5      equ $022E
SDMCTL      equ $022F
SDLSTL      equ $0230
SDLSTH      equ $0231
SSKCTL      equ $0232
SPARE       equ $0233
LPENH       equ $0234
LPENV       equ $0235
BRKKY       equ $0236
VPIRQ       equ $0238
CDEVIC      equ $023A
CCOMND      equ $023B
CAUX1       equ $023C
CAUX2       equ $023D
TMPSIO      equ $023E
ERRFLG      equ $023F
DFLAGS      equ $0240
DBSECT      equ $0241
BOOTAD      equ $0242
COLDST      equ $0244
RECLEN      equ $0245
DSKTIM      equ $0246
PDVMSK      equ $0247
SHPDVS      equ $0248
PDMSK       equ $0249
RELADR      equ $024A
PPTMPA      equ $024C
PPTMPX      equ $024D
CHSALT      equ $026B
VSFLAG      equ $026C
KEYDIS      equ $026D
FINE        equ $026E
GPRIOR      equ $026F
PADDL0      equ $0270
PADDL1      equ $0271
PADDL2      equ $0272
PADDL3      equ $0273
PADDL4      equ $0274
PADDL5      equ $0275
PADDL6      equ $0276
PADDL7      equ $0277
STICK0      equ $0278
STICK1      equ $0279
STICK2      equ $027A
STICK3      equ $027B
PTRIG0      equ $027C
PTRIG1      equ $027D
PTRIG2      equ $027E
PTRIG3      equ $027F
PTRIG4      equ $0280
PTRIG5      equ $0281
PTRIG6      equ $0282
PTRIG7      equ $0283
STRIG0      equ $0284
STRIG1      equ $0285
STRIG2      equ $0286
STRIG3      equ $0287
HIBYTE      equ $0288
WMODE       equ $0289
BLIM        equ $028A
IMASK       equ $028B
JVECK       equ $028C
NEWADR      equ $028E
TXTROW      equ $0290
TXTCOL      equ $0291
TINDEX      equ $0293
TXTMSC      equ $0294
TXTOLD      equ $0296
CRETRY      equ $029C
HOLD3       equ $029D
SUBTMP      equ $029E
HOLD2       equ $029F
DMASK       equ $02A0
TMPLBT      equ $02A1
ESCFLG      equ $02A2
TABMAP      equ $02A3
LOGMAP      equ $02B2
INVFLG      equ $02B6
FILFLG      equ $02B7
TMPROW      equ $02B8
TMPCOL      equ $02B9
SCRFLG      equ $02BB
HOLD4       equ $02BC
DRETRY      equ $02BD
SHFLOC      equ $02BE
BOTSCR      equ $02BF
PCOLR0      equ $02C0
PCOLR1      equ $02C1
PCOLR2      equ $02C2
PCOLR3      equ $02C3
COLOR0      equ $02C4
COLOR1      equ $02C5
COLOR2      equ $02C6
COLOR3      equ $02C7
COLOR4      equ $02C8
RUNADR      equ $02C9
HIUSED      equ $02CB
ZHIUSE      equ $02CD
GBYTEA      equ $02CF
LOADAD      equ $02D1
ZLOADA      equ $02D3
DSCTLN      equ $02D5
ACMISR      equ $02D7
KRPDER      equ $02D9
KEYREP      equ $02DA
NOCLIK      equ $02DB
HELPFG      equ $02DC
DMASAV      equ $02DD
PBPNT       equ $02DE
PBUFSZ      equ $02DF
RUNAD       equ $02E0
INITAD      equ $02E2
RAMSIZ      equ $02E4
MEMTOP      equ $02E5
MEMLO       equ $02E7
HNDLOD      equ $02E9
DVSTAT      equ $02EA
CBAUDL      equ $02EE
CBAUDH      equ $02EF
CRSINH      equ $02F0
KEYDEL      equ $02F1
CH1         equ $02F2
CHACT       equ $02F3
CHBAS       equ $02F4
NEWROW      equ $02F5
NEWCOL      equ $02F6
ROWINC      equ $02F8
COLINC      equ $02F9
CHAR        equ $02FA
ATACHR      equ $02FB
CH          equ $02FC
FILDAT      equ $02FD
DSPFLG      equ $02FE
SSFLAG      equ $02FF

;
; PAGE 3
;
DDEVIC      equ $0300
DUNIT       equ $0301
DCOMND      equ $0302
DSTATS      equ $0303
DBUFLO      equ $0304
DBUFHI      equ $0305
DTIMLO      equ $0306
DUNUSE      equ $0307
DBYTLO      equ $0308
DBYTHI      equ $0309
DAUX1       equ $030A
DAUX2       equ $030B
TIMER1      equ $030C
ADDCOR      equ $030E
CASFLG      equ $030F
TIMER2      equ $0310
TEMP1       equ $0312
TEMP2       equ $0314
TEMP3       equ $0315
SAVIO       equ $0316
TIMFLG      equ $0317
STACKP      equ $0318
TSTAT       equ $0319
HATABS      equ $031A
PUPBT1      equ $033D
PUPBT2      equ $033E
PUPBT3      equ $033F
IOCB0       equ $0340
IOCB1       equ $0350
IOCB2       equ $0360
IOCB3       equ $0370
IOCB4       equ $0380
IOCB5       equ $0390
IOCB6       equ $03A0
IOCB7       equ $03B0
PRNBUF      equ $03C0
SUPERF      equ $03E8
CKEY        equ $03E9
CASSBT      equ $03EA
CARTCK      equ $03EB
DERRF       equ $03EC
ACMVAR      equ $03ED
BASICF      equ $03F8
MINTLK      equ $03F9
GINTLK      equ $03FA
CHLINK      equ $03FB
CASBUF      equ $03FD

;
; HARDWARE REGISTERS
;
; GTIA
;
M0PF        equ $D000
HPOSP0      equ $D000
M1PF        equ $D001
HPOSP1      equ $D001
M2PF        equ $D002
HPOSP2      equ $D002
M3PF        equ $D003
HPOSP3      equ $D003
P0PF        equ $D004
HPOSM0      equ $D004
P1PF        equ $D005
HPOSM1      equ $D005
P2PF        equ $D006
HPOSM2      equ $D006
P3PF        equ $D007
HPOSM3      equ $D007
M0PL        equ $D008
SIZEP0      equ $D008
M1PL        equ $D009
SIZEP1      equ $D009
M2PL        equ $D00A
SIZEP2      equ $D00A
M3PL        equ $D00B
SIZEP3      equ $D00B
P0PL        equ $D00C
SIZEM       equ $D00C
P1PL        equ $D00D
GRAFP0      equ $D00D
P2PL        equ $D00E
GRAFP1      equ $D00E
P3PL        equ $D00F
GRAFP2      equ $D00F
TRIG0       equ $D010
GRAFP3      equ $D010
TRIG1       equ $D011
GRAFM       equ $D011
TRIG2       equ $D012
COLPM0      equ $D012
TRIG3       equ $D013
COLPM1      equ $D013
PAL         equ $D014
COLPM2      equ $D014
COLPM3      equ $D015
COLPF0      equ $D016
COLPF1      equ $D017
COLPF2      equ $D018
COLPF3      equ $D019
COLBK       equ $D01A
PRIOR       equ $D01B
VDELAY      equ $D01C
GRACTL      equ $D01D
HITCLR      equ $D01E
CONSOL      equ $D01F

;
; POKEY
;
POT0        equ $D200
AUDF1       equ $D200
POT1        equ $D201
AUDC1       equ $D201
POT2        equ $D202
AUDF2       equ $D202
POT3        equ $D203
AUDC2       equ $D203
POT4        equ $D204
AUDF3       equ $D204
POT5        equ $D205
AUDC3       equ $D205
POT6        equ $D206
AUDF4       equ $D206
POT7        equ $D207
AUDC4       equ $D207
ALLPOT      equ $D208
AUDCTL      equ $D208
KBCODE      equ $D209
STIMER      equ $D209
RANDOM      equ $D20A
SKREST      equ $D20A
POTGO       equ $D20B
SERIN       equ $D20D
SEROUT      equ $D20D
IRQST       equ $D20E
IRQEN       equ $D20E
SKSTAT      equ $D20F
SKCTL       equ $D20F

;
; PIA
;
PORTA       equ $D300
PORTB       equ $D301
PACTL       equ $D302
PBCTL       equ $D303
;
; ANTIC
;
DMACLT      equ $D400
CHACTL      equ $D401
DLISTL      equ $D402
DLISTH      equ $D403
HSCROL      equ $D404
VSCROL      equ $D405
PMBASE      equ $D407
CHBASE      equ $D409
WSYNC       equ $D40A
VCOUNT      equ $D40B
PENH        equ $D40C
PENV        equ $D40D
NMIEN       equ $D40E
NMIST       equ $D40F
NMIRES      equ $D40F

;
; FLOATING POINT ROUTINES
;
AFP         equ $D800
FASC        equ $D8E6
IFP         equ $D9AA
FPI         equ $D9D2
ZFR0        equ $DA44
ZF1         equ $DA46
FSUB        equ $DA60
FADD        equ $DA66
FMUL        equ $DADB
FDIV        equ $DB28
PLYEVL      equ $DD40
FLD0R       equ $DD89
FLD0P       equ $DD8D
FLD1R       equ $DD98
FLD1P       equ $DD9C
FSTOR       equ $DDA7
FSTOP       equ $DDAB
FMOVE       equ $DDB6
EXP         equ $DDC0
EXP10       equ $DDCC
LOG         equ $DECD
LOG10       equ $DED1

;
; ROM VECTORS
;
DSKINV      equ $E453
CIOV        equ $E456
SIOV        equ $E459
SETVBV      equ $E45C
SYSVBV      equ $E45F
XITVBV      equ $E462
SIOINV      equ $E465
SENDEV      equ $E468
INTINV      equ $E46B
CIOINV      equ $E46E
SELFSV      equ $E471
WARMSV      equ $E474
COLDSV      equ $E477
RBLOKV      equ $E47A
CSOPIV      equ $E47D
PUPDIV      equ $E480
SELFTSV     equ $E483
PENTV       equ $E486
PHUNLV      equ $E489
PHINIV      equ $E48C
GPDVV       equ $E48F
;
; SpartaDos X equates
;
S_FLAG      equ $0700
SDXVER      equ $0701
SDXREV      equ $0702
JKERNEL     equ $0703
BLOCK_IO    equ $0706
JMISC       equ $0709
JLSIO       equ $070C
JHWCTRL     equ $0715
SIOHAND     equ $0718
SLVTAB      equ $0720
LDFTAB      equ $0730
DEVTAB      equ $0740
HANDTAB     equ $0750
FHANDLE     equ $0760
SIOCBIDX    equ $0760
DEVICE      equ $0761
NAME        equ $0762
NAMES       equ $076D
FMODE       equ $0778
OPMODE      equ $0778
FATR1       equ $0779
FATR2       equ $077A
DATE        equ $077B
TIME        equ $077E
DATESET     equ $0781
FAUX1       equ $0782
FAUX2       equ $0783
FAUX3       equ $0784
FAUX4       equ $0785
FAUX5       equ $0786
SYSCALL     equ $0787
DIRBUF      equ $0789
DIRFATR     equ $0789
DIRF1SEC    equ $078A
DIRFLEN     equ $078C
DIRFNAME    equ $078F
DIRFDATE    equ $079A
DIRFTIME    equ $079D
PATH        equ $07A0
JHDOSINI    equ $07E0
VDOS        equ $07E3
VCIO        equ $07E5
VEDIT       equ $07E7
VCRUNCH     equ $07E9
JFSYMBOL    equ $07EB
JMDVECT     equ $07EE
JEXT_ON     equ $07F1
JEXT_OFF    equ $07F4
JEXT_SW     equ $07F7
JCAR_SW     equ $07FA
JCAR_OLD    equ $07FD
JGETTD      equ $FFC0
JSETTD      equ $FFC3
JTDON       equ $FFC6
JFMTTD      equ $FFC9
JXCOMLI     equ $FFD2
JKEYON      equ $FFD5
;
; Code equates
;
L0082       equ $0082
L0085       equ $0085
L0087       equ $0087
L00A5       equ $00A5
L1234       equ $1234
L5678       equ $5678
LD3C1       equ $D3C1

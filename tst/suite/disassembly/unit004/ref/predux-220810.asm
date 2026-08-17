; 
            icl 'predux-220810.inc'
;
; Start of code
;
            
;
            .word HATABS+26
            .byte $00,$00,$00,$00,$00,$00,$68,$09,$00,$8D,$AF,$30,$30,$30,$30,$06
            .byte $08,$00,$09,$30,$2E,$30,$3A,$3E,$3B,$85,$AF,$30,$30,$30,$30,$30
            .byte $30,$30,$00,$30,$2E,$30,$4D,$47,$4D,$81,$01,$3B,$1E,$12,$15,$0D
            .byte $30,$30,$84,$01
            .byte '04.220810'
            jsr L1ECB
            sei
            ldx #$FF
            txs
            stx LE8C5
            lda #$00
            sta LD030
            sta GRAFM
            sta COLPM3
            sta L0092
            sta CHKSNT
            sta NOCKSM
            sta L00A4
            sta ICPTHZ
            jsr L2415
            jsr L42BB
            jsr LE0BA
            lda LMARGN
            bpl L03A2
            lsr
            lsr
            sta L042E
L03A2       lda #$33
            sta L1E2C+1
            lda #$C0
            sta L1D9D+1
            lda #$00
            sta GRAFM
            tay
L03B2       sta L4800,Y
            sta L4900,Y
            sta L4A00,Y
            sta L4B00,Y
            sta L5400,Y
            sta L54C0,Y
            iny
            bne L03B2
            lda #$20
L03C9       sta L4BF8,Y
            iny
            cpy #$08
            bcc L03C9
            jsr L0AC3
            dec NGFLAG
            ldx #$6F
            ldy #$DF
            jsr LE018
            inc NGFLAG
            inc CHKSNT
            inc NOCKSM
            jsr L4074
            lda #$00
            sta DSKUTL+1
            sta ABUFPT
            sta ABUFPT+1
            sta ABUFPT+2
            sta POKMSK
            sta BRKKEY
            jsr L1ECB
            lda #$01
            sta COLPF3
            lda LDC0D
            cli
L0400       jsr L086B
            bit DINDEX
            bpl L040A
            jmp L04DA
L040A       jsr L0758
            lda #$4D
L040F       sta L1E2C+1
L0412       lda #$00
            sta TEMPCHR
            sta ABUFPT+1
            sta ABUFPT+2
            sta RTCLOK
L041C       sta RTCLOK+1
            ldx #$00
            jsr L318C
            lda ABUFPT
            cmp #$05
L0427       bne L0442
            lda LD419+2
            and #$0F
L042E       bit LBF09
L0431       adc #$00
            sta DOSINI+1
            lda #$F1
            sta L0090
            jsr L2363
L043C       jsr L1ECB
            jsr L2412
L0442       jsr L04AC
            lda TEMPCHR
            beq L0456
            dey
            beq L0442
L044C       jsr L04AC
            dey
            beq L0456
            lda TEMPCHR
            bne L044C
L0456       jsr L04AC
L0459       lda RMARGN
            bne L0459
L045D       lda RMARGN
            beq L045D
            lda #$FF
            sta RTCLOK
            sec
            sbc ICBAHZ
            sta RTCLOK+1
            lda ABUFPT+2
            beq L0456
            lda ABUFPT+1
            cmp #$68
L0472       bcc L0456
            lda #$00
            sta RTCLOK
            sta RTCLOK+1
            ldx #$80
L047C       stx TEMPCHR
L047E       jsr L04AC
            dey
            bne L048B
            ldx TEMPCHR
            bne L047E
            dex
            bne L047C
L048B       lda TEMPCHR
            beq L0495
            jsr L04AC
            dey
L0493       bne L048B
L0495       clc
            lda DSKUTL+1
L0498       adc #$40
            sta DSKUTL+1
            lda ABUFPT
            adc #$01
            sta ABUFPT
            cmp #$06
            bcs L04A9
            jmp L0412
L04A9       jmp IOCB3+ICDNO
L04AC       jsr L0758
L04AF       jsr L3B64
            jsr L04BC
            ldy ICBAHZ
            lda ICPTHZ
            bmi L04D8
L04BB       rts
L04BC       lda L0261
            beq L04BB
            lda ICBLLZ
            and #$08
            bne L04BB
            sta OLDADR
            lda #$16
            sta ATACHR
            jsr L41A8
L04D1       bit ICBLLZ
            bmi L04D1
            jmp IOCB3+ICDNO
L04D8       pla
            pla
L04DA       jsr L443A
            jsr L2422
            lda #$33
            sta L1E2C+1
            ldx #$09
L04E7       jsr L318C
            lda #$12
            sta L00A4
            jsr L42BB
            lda #$00
            sta APPMHI
            sta LTEMP+1
            sta BUFRFL
            sta RECVDN
            sta XMTDON
            sta L00E0
            sta L0100
L0502       sta CDTMV5
            sta L0093
            lda LCA0B
            sta BPTR
L050C       jsr L415D
L050F       inc NOCKSM
            jsr L4074
            lda #$06
            sta CHBAS
            bne L051E
L051B       jsr L3C9E
L051E       lda #$00
L0520       sta POKMSK
            sta BRKKEY
            sta RTCLOK
L0526       sta RTCLOK+1
L0528       sta ICCOMZ
            sta ICSTAZ
            sta LE8F2
            sta L0140
            sta L00F1
            sta TEMPCHR
            lda #$FF
            sta L0111
            jsr L110D
            jsr L416E
L0541       lda LD419+2
            and #$0F
            asl
            tay
            ldx LEA24,Y
            stx L0554+1
            ldx LEA25,Y
            stx L0554+2
L0554       bit L1234
L0557       bpl L0541
            lsr
            sta FTYPE
            eor #$FF
            sta FEOF
            lda L00E0
            sta DOSINI+1
            jsr L3C73
            jsr L258C
L056A       jsr L0A4B
            lda #$00
            jsr L275B
            ldy #$00
            sty ABUFPT
            sty ABUFPT+2
            lda TXTMSC
            eor #$03
            sec
            sbc #$13
            asl
            rol ABUFPT
            asl
            rol ABUFPT
            asl
            rol ABUFPT
            sta DSKUTL+1
            lda L4C00
            eor #$83
            sec
            sbc #$08
            bpl L0597
            dec ABUFPT+2
L0597       asl
            rol ABUFPT+2
            asl
            rol ABUFPT+2
            asl
            rol ABUFPT+2
            sta ABUFPT+1
            lda #$80
            sta BUFADR+1
            lda #$07
            sta L00F0
            sta APPMHI+1
            lda #$00
            sta L0110
            lda #$02
            sta OLDCOL
            lda #$5A
L05B7       jsr L31E8
            jsr L2422
            lda #$F8
            sta LD026
            lda #$33
            sta L1E2C+1
            jsr L23E6
            lda #$C0
L05CC       ora FREQ
            sta L1D9D+1
            jsr L08C1
            ldx #$A2
            ldy #$E1
            jsr L20E5
            lda #$07
            sta CHBAS
            lda #$60
            sta SWPFLG
L05E4       jsr L21E9
            jsr L0758
            lda #$01
            sta LE644
            lda #$4F
            sta L1E2C+1
            jsr L4074
            inc SWPFLG
            bpl L05E4
            lda #$40
            sta L00F0
            sta APPMHI+1
            bne L060D
L0603       lda OLDCOL+1
            bne L060A
            jmp L051B
L060A       jsr L2B14
L060D       lda LE644
L0610       sta L025A
            jsr L2415
            ldy FTYPE
            cpy FEOF
            beq L066E
            lda LEA54,Y
            sta LCB24
            lda LEA64,Y
            sta LCB25
            lda LEA74,Y
            sta LCB26
            ldx #$0F
            stx OLDCOL
L0632       lda #$00
            sta L00E0,X
            sta L00F0,X
            lda #$FF
            sta L0110,X
            dex
            bne L0632
            stx SOUNDR
            stx DSKFMS
            jsr L2753
            jsr L25A2
            jsr L2A40
L064D       jsr L116C
            jsr L2083
            lda SWPFLG
            bne L065D
            lda #$01
            sta OLDCOL
            bne L0668
L065D       jsr L1ECB
            jsr L1225
            lda LE644
L0666       ora #$01
L0668       sta L025A
            jsr L2415
L066E       jsr L2753
            jsr L263D
            jsr L08C1
            jsr L0AC3
            jsr L0D07
            jsr L0AD6
            jsr L0C1A
            jsr L1ED6
            ldx #$01
            stx L0140
            stx L0150
            dex
            stx ABUFPT+3
            stx ICDNOZ
            stx L0092
            lda L025A
            sta LE644
            lda #$C0
            ora FREQ
            sta L1D9D+1
            lda #$4F
            sta L1E2C+1
            bne L06E2
L06A9       inc TEMPCHR
            jsr L1756
            lda ICDNOZ
            bne L0704
            jsr L1225
            jsr L0E25
            jsr L0D25
            jsr L26E4
            jsr L4019
            jsr L0AE7
            jsr L1060
            jsr L0F54
L06CA       lda RMARGN
            beq L06CA
            jsr L3FAA
            jsr L3BE0
            jsr L4074
            jsr L21E9
            lda ICHIDZ
            bmi L071A
L06DE       lda L00A3
            bne L06DE
L06E2       lda #$40
            sta L00A3
L06E6       bit L00A3
            bne L06E6
L06EA       jsr L077B
            jsr L1ED6
            jsr L0C5D
            jsr L0E9C
            jsr L0C1A
            lda L00F0
            beq L070A
            lda ABUFPT+3
            beq L06A9
            jmp L0603
L0704       jsr L32F6
            jmp L071D
L070A       lda L00E0
            beq L0714
L070E       jsr L0FCE
            jmp L06E2
L0714       jsr L3D69
            jmp IOCB3+ICDNO
L071A       jsr L2D00
L071D       jsr L2415
            jsr L1ECB
            lda #$33
            sta L1E2C+1
            lda #$00
            sta GRACTL
            sta BRKKEY
            sta RTCLOK+1
            jsr L0AC3
            jsr L0C1A
            jsr L1ED6
            jsr L1ECB
            jsr L2A40
            jsr L0AD6
            jsr L1ECB
            jsr L09A1
            lda #$4F
            sta L1E2C+1
            lda #$C0
            ora FREQ
            sta L1D9D+1
            jmp L06E2
L0758       inc TEMPCHR
L075A       lda RMARGN
            beq L075A
            jsr L3FAA
L0761       lda RMARGN
            bne L0761
            lda #$40
            sta L00A3
L0769       bit L00A3
            bne L0769
            jsr L077B
            jsr L0C1A
            jsr L1ED6
L0776       lda RMARGN
            beq L0776
            rts
L077B       lda L0092
            asl
            asl
            sta L0782+1
L0782       bne L0782
            sta L0092
            rts
            nop
            jmp L0857
            nop
L078C       jmp L083F
            nop
            jmp L0842
            nop
            lda L0090
            sta L0094
            lda #$84
            sta CHBAS
            ldx #$00
            stx ICBLHZ
            inx
            cpx OLDCOL
            beq L0818
L07A6       ldy L0110,X
            bmi L0813
            lda LCF93,Y
            and LE644
            beq L0813
            sty RAMLO+1
            ldy L00E0,X
            cpy #$40
            bcs L0813
            cpy #$20
            bcs L07C6
            lda LE26E,Y
            bmi L0813
            inc ICBLHZ
L07C6       bit L00A1
            bvs L07D4
            tya
            asl
            sbc #$4F
            adc L00F0,X
            beq L07D4
            bpl L0811
L07D4       clc
            lda ICCOMZ
            adc L00E0,X
            bcc L07DD
            lda #$FF
L07DD       sta ICCOMZ
            jsr L1208
            jsr L20AD
            lda #$F7
            sta DOSVEC+1
            sta DOSVEC
            lda #$9D
            sta DOSINI
            jsr L20F3
            bit L0092
            bpl L080A
            ldy L00E0,X
            lda LE226,Y
            tay
            lda LE2CA,Y
            jsr L4105
            jsr L417A
            lda #$12
            sta CHBAS
L080A       lda #$40
            sta L00E0,X
            sta L0140,X
L0811       sta L00F0,X
L0813       inx
            cpx #$10
            bcc L07A6
L0818       bit L0092
L081A       bpl L0826
L081C       jsr L4186
            lda ICBLHZ
            beq L083F
            jsr L4180
L0826       bit L00A1
            bmi L083F
            ldx L00E0
            lda LE26E,X
            bmi L083F
            txa
            sec
            adc L00F0
            sbc BPTR
            sbc #$20
            bpl L083D
            lda #$00
L083D       sta L00F0
L083F       lda #$F1
            .byte $0C
L0842       lda #$F0
            sta L0090
            lda LD419+2
            and #$07
            sta DRKMSK
            dec L0092
            lda #$07
            and LD419+2
            sta COLRSH
            rts
L0857       lda L0094
            sta L0090
            dec L0092
            rts
L085E       lda BUFRHI
            sta BFENHI
            lda BFENLO
            and #$03
            ora #$D8
            sta LTEMP
            rts
L086B       lda LD419+2
            and #$03
            clc
            adc #$02
            jsr L08EF
            lda LDBEA+1
            sta OLDCHR
            sta LD01F+1
            lda LDBEA+2
            ldy #$0D
L0883       sta LDBE7+1,Y
            dey
            bpl L0883
L0889       ldx #$00
            ldy #$D8
            stx BUFRHI
            sty BFENLO
            lda OLDCHR
            ldx #$06
            jsr L0BE1
            lda #$F2
            ldy #$23
L089C       sta LD851+1,Y
            sta LD878+2,Y
            dey
            bpl L089C
            ldy #$00
L08A7       lda LC100,Y
            lsr
            lsr
            lsr
            lsr
            tax
            lda LDBE7+1,X
            eor LC100,Y
            and #$0F
            eor LC100,Y
            sta LC100,Y
            iny
            bne L08A7
            rts
L08C1       lda #$01
            cmp OLDCOL
            bcs L08CF
            lsr
            cmp ATRACT
            bne L08CF
            lda LE8F3
L08CF       jsr L08EF
            jsr L1ECB
            lda LDBEA+1
            sta OLDCHR
            sta LD01F+1
            jsr L0889
            lda ICCOMZ
            rol
            rol
            rol
            and #$03
            tay
            lda LCFDD,Y
            sta LC116
            rts
L08EF       sta ICAX1Z
            asl
            asl
            asl
            sec
            sbc ICAX1Z
            tay
            ldx #$00
L08FA       lda LE14B,Y
            sta LDBE7+2,X
            lsr
            lsr
            lsr
            lsr
            sta LDBE7+1,X
            iny
            inx
            inx
            cpx #$0E
            bcc L08FA
            lda LDBE7+1
            sta L0090
            sta L0094
            lda LDBF4
            sta LD021+1
            lda LDBF4+1
            sta LD021+2
            lda #$08
            sta LDBF4
            jmp L0889
L0929       lda #$0A
            sta SAVMSC
L092D       jsr L4071
            bit ICPTHZ
            bmi L0929
            dec SAVMSC
            bne L092D
            rts
L0939       pha
            ldx #$40
            ldy #$51
            stx BUFRHI
            sty BFENLO
            ldx #$11
            lda #$30
            jsr L0BE1
            ldx #$40
            ldy #$D9
            stx BUFRHI
            sty BFENLO
            ldx #$11
            pla
            jsr L0BE1
            ldx #$40
            ldy #$D9
            stx BUFRHI
            sty BFENLO
            rts
L0960       ldx #$07
L0962       lda CDTMA2,X
            sta SDLSTH,X
            lda LE62F,X
            sta VPIRQ+1,X
            lda LE637,X
            sta DBSECT,X
            lda LD021+1,X
            sta RELADR+1,X
            lda LE656,X
            sta L0251,X
            dex
            bpl L0962
            lda LE63F
            sta L0259
            lda LE644
            sta L025A
            lda LE64B
            sta L025B
            lda L0090
            sta L025C
            lda #$00
            sta DRKMSK
            sta COLRSH
            rts
L09A1       ldx #$07
L09A3       lda SDLSTH,X
            sta CDTMA2,X
            lda VPIRQ+1,X
            sta LE62F,X
            lda DBSECT,X
            sta LE637,X
            lda RELADR+1,X
            sta LD021+1,X
            lda L0251,X
            sta LE656,X
            dex
            bpl L09A3
            lda L0259
            sta LE63F
            lda L025A
            sta LE644
            lda L025B
L09D3       sta LE64B
            lda L025C
            sta L0090
            rts
L09DC       jsr L0BF9
            ldy DOSINI+1
            lda LE226,Y
            pha
            sta LE3AE
            lda LE23E,Y
            ldx #$0F
            .byte $8F
            bcs L09D3
            lsr
            lsr
            lsr
            lsr
            sta LE3AF
            ldx #$A1
            ldy #$E3
            jsr L3230
            lda #$32
            jsr L3256
L0A03       inc SDLSTL
            dec L0096
            pla
            pha
L0A0A       jsr L3256
            pla
            beq L0A19
            sec
L0A11       sbc #$01
            lsr
            lsr
L0A15       clc
            adc #$0A
            .byte $0C
L0A19       lda #$37
L0A1B       jsr L3256
            jmp L0A24
L0A21       jsr L32BC
L0A24       lda L0096
            cmp #$27
            bcc L0A21
            rts
L0A2B       lda #$33
            .byte $0C
L0A2E       lda #$4D
            ldx #$F1
L0A32       sta L1E2C+1
            stx L0090
L0A37       lda #$C0
            sta L1D9D+1
            lda #$00
            sta DRKMSK
            sta COLRSH
            sta L025D
            sta L025E
            jmp L0BF9
L0A4B       lda #$00
            sta ICAX1Z
L0A4F       lda FTYPE
            jsr LE000
            ldy #$4C
            sty L0A61+2
            ldy #$80
            sty BFENLO
            ldy #$00
            sty BUFRHI
L0A61       lda L8000,Y
            asl
            asl
            tax
            sty ICBLHZ
L0A69       ldy #$00
            lda LC200,X
            sta (BUFRHI),Y
            iny
            lda LC280,X
            sta (BUFRHI),Y
            iny
            lda LC300,X
            sta (BUFRHI),Y
            iny
            lda LC380,X
            sta (BUFRHI),Y
            inx
            inc BFENLO
            txa
            and #$03
            bne L0A69
            lda BUFRHI
            adc #$04
L0A8E       sta BUFRHI
            bcs L0A98
            lda BFENLO
            sbc #$03
            sta BFENLO
L0A98       ldy ICBLHZ
            iny
            bne L0A61
            inc L0A61+2
            lda BFENLO
            cmp #$A0
            bne L0ABC
            lda LDBFF+1
            eor #$FF
            ora #$80
            bit LCF97
            bne L0ABA
            and ICAX1Z
            beq L0ABA
            eor #$03
            bne L0AC1
L0ABA       sta ICAX1Z
L0ABC       bit BFENLO
            bvc L0A61
            .byte $0C
L0AC1       sta ICAX1Z
L0AC3       ldx #$00
            txa
L0AC6       sta L4C00,X
            sta L4D00,X
            sta L4E00,X
            sta L4F00,X
            inx
            bne L0AC6
            rts
L0AD6       lda #$08
            sta L0091
            lda #$80
            sta BUFADR+1
            lda #$05
            sta ICHIDZ
            ldx #$53
            jmp L318C
L0AE7       lda TEMPCHR
            lsr
            .byte $A7,$22
            and #$C0
L0AEE       rol
            rol
            rol
            tay
            lda LCFDD,Y
            sta LC116
            lda BPTR
            asl
L0AFB       adc LE8BD,Y
L0AFE       sta ICBALZ
            dec ICSTAZ
            bpl L0B1B
            lda #$18
            sta ICSTAZ
            txa
L0B09       beq L0B1B
            and #$0F
            bne L0B10
            dex
L0B10       dex
            stx ICCOMZ
            lda LE2DE,Y
            beq L0B1B
            jsr L4105
L0B1B       lda TEMPCHR
            bit LE2C9
            bne L0B3E
            bit L00A1
            bmi L0B3B
            ldx L0100
            and LE2C0,X
            bne L0B3B
            ldx APPMHI+1
L0B30       beq L0B39
            dex
            stx APPMHI+1
            cpx L00F0
            bcs L0B3B
L0B39       stx L00F0
L0B3B       rts
L0B3C       lda TEMPCHR
L0B3E       and #$1F
            eor #$11
            ora CH
            ora ATACHR
            bne L0B4F
            lda #$18
            sta ATACHR
L0B4F       rts
L0B50       bit LCA0A
            bpl L0B50
            sta LDBFF+1
            lda LDC01
            rts
L0B5C       ldx LD419
            ldy LD419+1
            lda #$FF
            sta LDC01+1
            sta LDBFF+1
            sta LCA0A
            lda ICPTHZ
            and #$13
            cmp #$10
            bcc L0B76
            rts
L0B76       asl
            sta L0B7F+1
            lda LDBFF+1
            and #$10
L0B7F       bcc L0B7F
            bcc L0B89
            bcc L0B9E
            bcc L0BA6
            bcc L0BB6
L0B89       beq L0B93
            txa
            bpl L0B96
            tya
            bpl L0B99
            bmi L0BC8
L0B93       lda #$81
            .byte $0C
L0B96       lda #$C2
            .byte $0C
L0B99       lda #$C3
            jmp L0BC6
L0B9E       beq L0BA2
            lda #$80
L0BA2       eor #$81
            bne L0BC6
L0BA6       bne L0BAC
            cpy #$80
            bcs L0BAE
L0BAC       lda #$80
L0BAE       eor #$C2
            cpx #$80
            bcs L0BC6
            bcc L0BC4
L0BB6       bne L0BBC
            cpx #$80
            bcs L0BBE
L0BBC       lda #$80
L0BBE       eor #$C3
            cpy #$80
            bcs L0BC6
L0BC4       ora #$20
L0BC6       sta ICPTHZ
L0BC8       lda LDBFF+1
            and #$03
            tax
            lda LCFC0,X
            sta ICBAHZ
            lda LDBFF+1
            lsr
            lsr
            and #$03
            tax
            lda LCFC0,X
            sta ICPTLZ
            rts
L0BE1       pha
            ldy #$27
L0BE4       sta (BUFRHI),Y
            dey
            bpl L0BE4
            clc
            lda #$28
            adc BUFRHI
            sta BUFRHI
            bcc L0BF4
            inc BFENLO
L0BF4       pla
            dex
            bne L0BE1
            rts
L0BF9       lda #$30
            .byte $0C
L0BFC       lda #$00
            ldx #$27
L0C00       sta L4940,X
L0C03       dex
            bpl L0C00
            inx
            stx L00A2
            rts
L0C0A       sec
            ldx #$FF
L0C0D       inx
L0C0E       sbc #$64
            bcs L0C0D
L0C12       ldy #$0A
L0C14       dey
            adc #$0A
L0C17       bcc L0C14
            rts
L0C1A       ldy BRKKEY
L0C1C       beq L0C32
            sec
            lda DSKUTL+1
            sbc BRKKEY
            sta DSKUTL+1
            lda ABUFPT
            ldy BRKKEY
            bpl L0C2E
            sbc #$FF
            .byte $0C
L0C2E       sbc #$00
L0C30       sta ABUFPT
L0C32       sec
            lda #$08
            sbc DSKUTL+1
            and #$07
            sta DRKMSK
            ldy RTCLOK+1
            beq L0C53
            sec
            lda ABUFPT+1
            sbc RTCLOK+1
            sta ABUFPT+1
            lda ABUFPT+2
            ldy RTCLOK+1
            bpl L0C4F
            sbc #$FF
            .byte $0C
L0C4F       sbc #$00
            sta ABUFPT+2
L0C53       sec
            lda #$08
            sbc ABUFPT+1
            and #$07
            sta COLRSH
            rts
L0C5D       bit ICPTHZ
            bvs L0CC1
            lda BUFADR+1
            beq L0C72
            bmi L0C85
            cmp #$02
            beq L0CA6
            bit ICPTHZ
            bpl L0C76
            jmp L0D8D
L0C72       bit ICPTHZ
            bmi L0CA5
L0C76       lda L0AD6+1
            sta L0091
            ldx #$53
            jsr L318C
            lda #$80
            sta BUFADR+1
            rts
L0C85       bit ICPTHZ
            bpl L0CA5
            lda ICBAHZ
            ora ICPTLZ
            beq L0CA1
L0C8F       ldx #$5A
            jsr L318C
            jsr L0D8D
            lda #$01
            sta BUFADR+1
            lda L0AD6+1
            sta L0091
            rts
L0CA1       lda #$02
            sta BUFADR+1
L0CA5       rts
L0CA6       lda ICPTLZ
            ora ICBAHZ
            bne L0C8F
            dec L0091
            bne L0CA5
            lda #$00
            sta BUFADR+1
            sta JVECK
            lda L0AD6+1
            sta L0091
            ldx #$61
            jmp L318C
L0CC1       lda BUFADR+1
            beq L0CCE
            bmi L0CDE
            bit ICPTHZ
            bpl L0CD4
            jmp L0D8D
L0CCE       lda ICPTHZ
            and #$20
            bne L0CFA
L0CD4       ldx #$53
            jsr L318C
            lda #$80
            sta BUFADR+1
            rts
L0CDE       lda ICPTHZ
            and #$20
            bne L0CFB
            bit ICPTHZ
            bpl L0CFA
            lda ICBAHZ
            ora ICPTLZ
            beq L0CFA
            ldx #$5A
            jsr L318C
            jsr L0D8D
            lda #$01
            sta BUFADR+1
L0CFA       rts
L0CFB       lda #$00
            sta BUFADR+1
            sta JVECK
L0D02       ldx #$61
L0D04       jmp L318C
L0D07       lda L00A1
            and #$10
            bne L0D19
L0D0D       ldy L00E0
            lda LE256,Y
            cmp #$04
L0D14       bcs L0D19
            adc #$04
            .byte $0C
L0D19       lda #$07
            sta RTCLOK+2
            clc
            eor #$FF
            adc #$01
            sta BUFADR
            rts
L0D25       ldx #$00
            lda ICPTLZ
            jsr L0D30
            ldx #$02
            lda ICBAHZ
L0D30       clc
            beq L0D5F
            bpl L0D3B
            lda #$D0
            ldy #$00
            beq L0D3F
L0D3B       lda #$30
            ldy #$FF
L0D3F       adc POKMSK,X
            sta POKMSK,X
            tya
            adc BRKKEY,X
            sta BRKKEY,X
            bmi L0D52
L0D4A       cmp RTCLOK+2
            bcc L0D5E
            lda RTCLOK+2
            bne L0D58
L0D52       cmp BUFADR
            bcs L0D5E
            lda BUFADR
L0D58       sta BRKKEY,X
            lda #$00
            sta POKMSK,X
L0D5E       rts
L0D5F       lda BRKKEY,X
            sta CASINI
            bmi L0D6F
            ora POKMSK,X
            beq L0D5E
            lda #$50
            ldy #$FF
            bmi L0D73
L0D6F       lda #$B0
            ldy #$00
L0D73       adc POKMSK,X
            sta POKMSK,X
            tya
            adc BRKKEY,X
            sta BRKKEY,X
            eor CASINI
            bmi L0D86
            lda BRKKEY,X
            bpl L0D4A
            bmi L0D52
L0D86       lda #$00
            sta POKMSK,X
            sta BRKKEY,X
L0D8C       rts
L0D8D       lda L0150
            bne L0D8C
            lda ICPTLZ
            sta CHKSUM
            lda ICBAHZ
            sta BUFRLO
            ora CHKSUM
            beq L0E18
            lda APPMHI
            eor #$03
            beq L0E19
            lda #$80
            and LE644
            bne L0E18
            sta RAMLO+1
            jsr L20AD
            lda #$07
            sta RAMLO+1
            .byte $A7
            asl L8EE8
            .byte $F4,$02
            asl
            asl
            tax
            lda BUFRLO
            beq L0DCC
            inx
            cmp CHKSUM
            beq L0DCC
            inx
            lda CHKSUM
            beq L0DCC
            inx
L0DCC       txa
            inx
            stx L01B0
            asl
            adc #$A8
            sta DOSINI
            ldx BUFRLO
            inx
            lda LE1E0,X
            sta COLCRS+1
            clc
            adc WARMST
            sta WARMST
            ldx CHKSUM
            inx
            lda LE1E0,X
            sta COLCRS
            clc
            adc TRAMSZ
            sta TRAMSZ
            jsr L0F98
            bpl L0DFA
            lda #$00
            sta BOOT
            .byte $0C
L0DFA       lda #$60
            sta CDTMA1+1
L0DFF       jsr L20F3
L0E02       jsr L4186
L0E05       lda L00A1
            and #$20
            bne L0E13
            sec
L0E0C       lda #$10
            sbc L0100
L0E11       asl
            .byte $0C
L0E13       lda #$02
L0E15       sta L0150
L0E18       rts
L0E19       lda L0092
L0E1B       bne L0E18
L0E1D       lda #$84
            sta L0092
            sta L0093
            bne L0E05
L0E25       lda L009D
            and LE644
            bpl L0E93
            lda #$07
            sta RAMLO+1
            jsr L20AD
            clc
            lda WARMST
            adc COLCRS+1
            clc
            adc RTCLOK+1
            sta WARMST
            cmp #$40
            bcc L0E87
            clc
            ldx #$00
            lda COLCRS
            bpl L0E49
            dex
L0E49       adc TRAMSZ
L0E4B       sta TRAMSZ
            txa
            adc CMCMD
            and #$01
            beq L0E58
L0E54       ldx TRAMSZ
            bmi L0E87
L0E58       sta CMCMD
            clc
            ldx #$00
            lda BRKKEY
            bpl L0E62
            dex
L0E62       adc TRAMSZ
            sta TRAMSZ
            txa
            adc CMCMD
            and #$01
            sta CMCMD
            jsr L0F98
            bmi L0E87
            lda LD419+2
            and #$03
            tax
            lda LCFD9,X
            sta DOSVEC+1
            lda DOSINI
            cmp #$B0
            bcc L0E90
            eor #$01
            bne L0E8E
L0E87       lda #$00
            sta BOOT
            sta CDTMA1+1
L0E8E       sta DOSINI
L0E90       jsr L20F3
L0E93       lda L0150
            beq L0E9B
            dec L0150
L0E9B       rts
L0E9C       ldx #$0B
L0E9E       jsr L0F1D
            bcc L0EB9
            lda BRKKEY
            bmi L0EAB
            ora POKMSK
            bne L0EB9
L0EAB       lda #$00
            sta BRKKEY
            sta POKMSK
            lda DSKUTL+1
            and #$F8
            adc #$00
            sta DSKUTL+1
L0EB9       dex
            cpx #$09
            bcs L0E9E
L0EBE       jsr L0F1D
            bcc L0ED9
            lda BRKKEY
            bmi L0ED9
            lda #$00
            sta BRKKEY
            sta POKMSK
            lda DSKUTL+1
            adc #$06
            and #$F8
            sta DSKUTL+1
            bcc L0ED9
            inc ABUFPT
L0ED9       dex
            cpx #$06
            bcs L0EBE
L0EDE       jsr L0F1D
            bcc L0EF9
            lda RTCLOK+1
            bmi L0EEB
            ora RTCLOK
            bne L0EF9
L0EEB       lda #$00
            sta RTCLOK+1
L0EEF       sta RTCLOK
            lda ABUFPT+1
            and #$F8
            adc #$00
            sta ABUFPT+1
L0EF9       dex
            cpx #$03
            bcs L0EDE
L0EFE       jsr L0F1D
            bcc L0F19
            lda RTCLOK+1
            bmi L0F19
            lda #$00
            sta RTCLOK+1
            sta RTCLOK
            lda ABUFPT+1
            adc #$06
            and #$F8
            sta ABUFPT+1
L0F15       bcc L0F19
            inc ABUFPT+2
L0F19       dex
            bpl L0EFE
            rts
L0F1D       clc
            lda ICCOMT
            adc LCFAD,X
            tay
            clc
            lda DSKFMS
            adc LCFB9,X
            sta L0F2D+2
L0F2D       lda L8000,Y
            cmp #$2E
            beq L0F3A
            cmp #$2D
            beq L0F3A
            asl
            rts
L0F3A       lsr
            stx ICAX2Z
            sty KEYDEF
            lda L0F2D+2
            sta KEYDEF+1
            bcs L0F4D
            jsr L2668
            ldx ICAX2Z
            clc
            rts
L0F4D       jsr L25FB
            ldx ICAX2Z
            clc
            rts
L0F54       lda DSKFMS+1
            cmp #$14
            beq L0F78
            cmp #$11
            beq L0F78
            bit ICPTHZ
            bpl L0F93
            cmp #$2F
            beq L0F73
            and #$FC
            cmp #$38
            bne L0F93
            dec ICHIDZ
            bne L0F72
            inc ABUFPT+3
L0F72       rts
L0F73       dec ICHIDZ
            beq L0F73
            rts
L0F78       jsr L104F
            lda TEMPCHR
            and #$03
            bne L0F93
            lda L00F0
            cmp APPMHI+1
            bcs L0F93
            inc L00F0
            lda #$05
            jsr L4135
            lda #$14
            sta CHBAS
L0F93       lda #$05
            sta ICHIDZ
            rts
L0F98       sec
            lda WARMST
            sbc #$AC
            lsr
            lsr
            lsr
            clc
            adc #$F0
            eor #$F0
            clc
            adc DSKFMS
            sta BUFRLO
            sec
            lda TRAMSZ
            sbc #$AC
            tay
            lda CMCMD
            sbc #$00
            bmi L0FBD
            lsr
            tya
            ror
            lsr
            lsr
            bpl L0FC4
L0FBD       lsr
            tya
            ror
            lsr
            lsr
            ora #$C0
L0FC4       clc
            adc ICCOMT
            sta CHKSUM
            ldy #$00
            lda (CHKSUM),Y
            rts
L0FCE       lda #$00
            sta RAMLO+1
            jsr L20AD
            lda #$07
            sta RAMLO+1
            lda #$F1
            sta DOSVEC+1
            jsr L20F3
            lda #$00
            sta RAMLO+1
            lda #$F7
            sta DOSVEC+1
            sta DOSVEC
            lda #$9D
            sta DOSINI
            jsr L20F3
            lda #$07
            sta L00F0
            cmp APPMHI+1
            bcc L0FFB
            sta APPMHI+1
L0FFB       lda #$00
            sta L00E0
            sta L0100
L1002       sta CDTMV5
            sta DOSINI+1
            sta RTCLOK+1
            sta BRKKEY
            sta COLCRS
            sta COLCRS+1
            jsr L2161
L1012       lda #$12
            sta CHBAS
L1017       jsr L0758
L101A       jsr L1225
            lda #$00
L101F       sta RAMLO+1
            jsr L20AD
            inc DOSINI
            lda DOSINI
            eor #$A8
            sta BOOT
            jsr L20F3
            lda BOOT
            bne L1017
            ldx #$A2
            ldy #$E1
            jsr L20E5
            lda LE644
            and #$7F
            ora #$01
            sta LE644
            jsr L0D07
            lda LE26E
            and #$7F
            sta APPMHI
            rts
L104F       bit L009D
            bmi L105B
            lda LE644
            and #$7F
            sta LE644
L105B       lda #$CE
            sta L009D
            rts
L1060       lda LCFFF
            cmp #$05
            bne L104F
            lda OLDCOL
            lsr
            beq L104F
            lda L00A1
            and #$02
            bne L107C
            lda L00E0
            cmp #$11
            bcc L104F
            cmp #$13
            beq L104F
L107C       lda ICPTHZ
            and #$80
            ora ICBAHZ
            ora ICPTLZ
            bne L104F
            lda L009D
            bpl L1093
            inc L009D
            bne L1092
L108E       lda #$00
            sta L009E
L1092       rts
L1093       beq L10A9
            dec L009D
            and #$04
            bne L1092
            lda LE1B8
            ldx #$04
L10A0       asl
            rol LE1B8
            dex
            bne L10A0
            beq L1106
L10A9       inc L009E
            ldx L009E
            cpx #$10
            bcs L108E
            ldy L00E0,X
            cpy #$17
            bcs L10A9
            lda #$0F
            sta L009D
            sta ICBLHZ
            lda LE226,Y
            sbc #$00
            lsr
            tay
            lda LCFE5,Y
            sta LE1B8
            sec
            lda L01C0,X
            sbc ICCOMT
            ror ICBLHZ
            bmi L10D8
            eor #$FF
            adc #$00
L10D8       sta CHKSUM
            lsr
            tay
            sec
            lda L01D0,X
            sbc DSKFMS
            ror ICBLHZ
            bmi L10EA
            eor #$FF
            adc #$00
L10EA       sta BUFRLO
            lsr
            cpy BUFRLO
            ror ICBLHZ
            cmp CHKSUM
            lda ICBLHZ
            ror
            lsr
            lsr
            lsr
            tax
            lda LE183,X
            sta LE1B3
            lda LE184,X
            sta LE1B5
L1106       ldx #$B2
            ldy #$E1
            jmp L20E5
L110D       lda BPTR
            cmp #$08
            bcs L1115
            inc BPTR
L1115       lda #$10
            jsr LE000
            ldx #$01
            stx OLDCOL+1
            dex
L111F       txa
            lsr
            lsr
            lsr
            lsr
            tay
            clc
            lda LEA44,Y
            adc BPTR
            cmp #$14
            bcc L1131
            lda #$13
L1131       sta ICBLHZ
            clc
            adc #$03
            sta ICAX1Z
            lda LC000,X
            and #$1F
            beq L1168
            cmp #$17
            beq L1168
            bcc L114F
L1145       lda LD419+2
            and #$03
            clc
            adc ICBLHZ
            bcc L115B
L114F       lda LD419+2
            and #$0F
L1154       lsr
            beq L1165
            cmp ICAX1Z
            bcs L1154
L115B       inc OLDCOL+1
            eor LC000,X
            and #$1F
            eor LC000,X
L1165       sta LC000,X
L1168       inx
            bne L111F
            rts
L116C       jsr L1218
            ldx #$0F
L1171       lda #$FF
            sta L00E0,X
            lda #$00
            sta L53E8,X
            dex
            bne L1171
            sta SWPFLG
L117F       ldx FTYPE
            ldy L47D0,X
            iny
L1185       sty ICBLHZ
            lda LC000,Y
            beq L11F1
            eor LE8F2
            cmp #$1F
            bcs L11F1
            inc SWPFLG
            ldy LCFF8
            dec LCFF8
            ldx LCFE9,Y
            stx ICAX1Z
            sta L00E0,X
            lda ICBLHZ
            sta L0100,X
            lda #$40
            sta L00F0,X
            asl
            sta L0110,X
            sta L47B0,X
            asl
            sta L0150,X
            sta L0120,X
            sta L0130,X
            sta L01A0,X
            rol
            sta L0140,X
            lda #$0D
            sta L01B0,X
L11C8       lda LD419+2
            and #$1F
            beq L11C8
            tax
            lsr L4C20,X
            bcc L11C8
            jsr L16F9
            ldx ICAX1Z
            lda CRITIC
            sta L0160,X
            lda FMSZPG
            sta L0170,X
            lda FMSZPG+1
            sta L0180,X
            lda FMSZPG+2
            sta L0190,X
L11EE       jsr L1518
L11F1       ldy ICBLHZ
            iny
            tya
            and #$0F
            bne L1185
            ldy #$06
L11FB       sta CDTMV5,Y
L11FE       sta CDTMV1,Y
            dey
            bne L11FB
L1204       jsr L2338
            rts
L1208       ldy L0100,X
L120B       lda LC000,Y
            beq L1217
L1210       lda #$00
            sta LC000,Y
            dec OLDCOL+1
L1217       rts
L1218       lda #$0E
L121A       ldy #$0F
L121C       .byte $0C
L121D       tya
            dey
L121F       sta LCFE9,Y
            bne L121D
            rts
L1225       ldx #$01
            cpx OLDCOL
            beq L1271
            lda #$2C
            sta L135C
            stx OLDCOL
L1232       lda L00E0,X
L1234       bmi L123F
            stx ICAX2Z
            jsr L1272
            ldx ICAX2Z
            inc OLDCOL
L123F       inx
            cpx #$10
            bcc L1232
            lda OLDCOL
            lsr
            bne L1271
            jsr L08C1
            ldy FTYPE
            .byte $BF
            bne L1298
L1251       lda LC000,X
            bne L1266
            inx
            txa
            and #$0F
            bne L1251
            lda #$FA
            jsr L4105
            lda #$FA
            jsr L4105
L1266       lda #$17
            sta CHBAS
            lda OLDCOL+1
            bne L1271
            inc ABUFPT+3
L1271       rts
L1272       and #$60
            beq L1285
            asl
            bpl L127F
            asl
            bne L1282
            jmp L144F
L127F       jmp L13E3
L1282       jmp L13E3
L1285       lda L47B0,X
            ldy L01A0,X
            bne L128F
            and #$7F
L128F       and #$FD
            lsr
            adc #$00
            asl
            sta L47B0,X
L1298       lda #$00
            sta BOOT
            jsr L1497
            lda CRITIC
            eor FMSZPG+1
            and #$1F
            bne L12AD
            lda CRITIC
            and #$1F
            eor #$10
L12AD       sta L009F
            jsr L2430
            lda L0110,X
            bcs L12C5
            bmi L12C2
            tay
            lda #$FF
            sta L0110,X
            jsr L2347
L12C2       jmp L1358
L12C5       bpl L1300
            ldy LCFFF
            bpl L12D8
            jsr L29AE
            bcc L12D7
            jsr L1208
            jsr L1832
L12D7       rts
L12D8       dec LCFFF
            lda LCFF9,Y
            tay
            sty RAMLO+1
            sta L0110,X
            ora #$50
            sta DOSINI
            lda L00E0,X
            sta CDTMV5,Y
            sta DOSINI+1
L12EF       txa
            sta CDTMV1,Y
            lda #$00
            sta DOSVEC+1
            sta DOSVEC
            jsr L2161
            ldx ICAX2Z
L12FE       bpl L1305
L1300       sta RAMLO+1
L1302       jsr L20AD
L1305       lda FMSZPG+5
            sta WARMST
            lda FMSZPG+3
            sta TRAMSZ
            lda FMSZPG+4
            sta CMCMD
            jsr L248C
            sta BOOT
            beq L1323
            ldx ICAX2Z
            inc L47B0,X
            jsr L153E
            jmp L1329
L1323       lda L00A1
            and #$04
            sta BOOT
L1329       ldx ICAX2Z
            lda L47B0,X
            and #$03
            beq L134A
            cmp #$03
            beq L134A
            ldy L00E0,X
            lsr
            lda LE286,Y
            bcs L1343
            asl
            bpl L134A
            lsr
            .byte $0C
L1343       bpl L134A
            and #$3F
            sta DSTATS,X
L134A       lda L47B0,X
            and #$03
            tay
            lda L13DF,Y
            sta DOSVEC+1
            jsr L20F3
L1358       lda BOOT
            bne L135F
L135C       bit L13CE
L135F       lda L01A0,X
            beq L1369
            dec L01A0,X
            bne L1399
L1369       lda L47C0,X
            and #$FC
            eor #$10
            ora L009F
            beq L1384
            lda DSTATS,X
            beq L1399
            dec DSTATS,X
            bne L1399
            jsr L1723
            jmp L1399
L1384       jsr L281B
            bcs L1399
            lda L47B0,X
            ora #$80
            sta L47B0,X
            lda #$00
            sta DSTATS,X
            jsr L2851
L1399       lda L009F
            bne L13A0
            jsr L16B5
L13A0       jsr L165C
            jsr L1680
            jsr L1680
            ldx ICAX2Z
            lda L0150,X
            beq L13B3
            dec L0150,X
L13B3       lda TEMPCHR
            and #$03
            bne L13CD
            lda L00F0,X
            ldy L00E0,X
            cpy #$17
            beq L13C4
            adc #$01
            .byte $80
L13C4       asl
            cmp #$40
            bcc L13CB
            lda #$40
L13CB       sta L00F0,X
L13CD       rts
L13CE       lda L01A0,X
            bne L13CD
            lda LD419+2
            and #$03
            adc #$02
            sta L01A0,X
            bcc L13CD
L13DF       .byte $0B,$07,$0F,$00
L13E3       lda L0110,X
            bmi L144C
            sta RAMLO+1
            jsr L1497
            jsr L2430
            bcc L144C
            jsr L20AD
            ldx ICAX2Z
            lda FMSZPG+5
            sta WARMST
            lda FMSZPG+3
            sta TRAMSZ
            lda FMSZPG+4
            sta CMCMD
            lda L00E0,X
            cmp #$20
            beq L141E
L1409       dec L00E0,X
L140B       eor #$21
            beq L1415
            lda #$00
            sta BOOT
            beq L142F
L1415       sta DOSVEC
            lda L0140,X
            sta DOSINI
            sta BOOT
L141E       lda LD419+2
            and #$03
            tay
            lda LCFD9,Y
            sta DOSVEC+1
            lda DOSINI
            eor #$01
            sta DOSINI
L142F       lda L47C0,X
            bpl L1444
            lda #$40
            sta L00E0,X
            lda #$01
            sta DOSVEC+1
            lsr
            sta L0140,X
            lda #$96
            sta DOSINI
L1444       jmp L20F3
L1447       lda #$00
            sta L53E8,X
L144C       jmp L1832
L144F       lda L0110,X
            bmi L1447
            sta RAMLO+1
            jsr L20AD
            lda DOSINI
            cmp #$9C
            beq L1447
            cmp #$A7
            beq L1447
            inc DOSINI
            jsr L1497
            jsr L2430
            bcc L1447
            lda FMSZPG+5
            sta WARMST
            lda FMSZPG+3
            sta TRAMSZ
            lda FMSZPG+4
            sta CMCMD
            jsr L20F3
            lda L0120,X
            cmp #$80
            ror
            bpl L1486
            adc #$00
L1486       sta L0120,X
            lda L0130,X
            cmp #$80
            ror
            bpl L1493
            adc #$00
L1493       sta L0130,X
            rts
L1497       ldy L01A0,X
            bne L14F2
            clc
            lda L0120,X
            bpl L14A3
            dey
L14A3       adc L0160,X
            sta L0160,X
            sta CRITIC
            tya
            adc L0170,X
            sta L0170,X
            sta FMSZPG
            tay
            lda CRITIC
            lsr
            lsr
            lsr
            ora LE21E,Y
            sta L01C0,X
            sta KEYDEF
            ldy #$00
            clc
            lda L0130,X
            bpl L14CB
            dey
L14CB       adc L0180,X
            sta L0180,X
            sta FMSZPG+1
            tya
            adc L0190,X
            sta L0190,X
            sta FMSZPG+2
            tay
            lda FMSZPG+1
            lsr
            lsr
            lsr
            ora LE222,Y
            sta L01D0,X
            sta KEYDEF+1
            ldy #$00
            lda (KEYDEF),Y
            sta L47C0,X
            rts
L14F2       lda L0160,X
            sta CRITIC
            lda L0170,X
            sta FMSZPG
            lda L0180,X
            sta FMSZPG+1
            lda L0190,X
L1504       sta FMSZPG+2
            lda L01C0,X
            sta KEYDEF
L150B       lda L01D0,X
            sta KEYDEF+1
            ldy #$00
            lda (KEYDEF),Y
L1514       sta L47C0,X
            rts
L1518       ldy FMSZPG
            lda CRITIC
            lsr
L151D       lsr
            lsr
            ora LE21E,Y
            sta L01C0,X
            sta KEYDEF
            ldy FMSZPG+2
            lda FMSZPG+1
            lsr
            lsr
            lsr
L152E       ora LE222,Y
            sta L01D0,X
            sta KEYDEF+1
            ldy #$00
            lda (KEYDEF),Y
L153A       sta L47C0,X
            rts
L153E       ldy L00E0,X
            lda LE26E,Y
            and #$7F
            beq L1598
            ldy L0150,X
            bne L1598
            cmp #$03
            beq L1599
            ldy LCFFF
            bmi L1598
            sta ICBLHZ
            ldx LCFF8
            bmi L1598
            lda LD419+2
            and #$3F
            cmp ICBALZ
            bcs L1598
            lda RAMLO+1
            pha
            dec LCFFF
            lda LCFF9,Y
            sta RAMLO+1
            dec LCFF8
            lda LCFE9,X
            tax
            jsr L15C8
            lda #$00
            sta L01A0,X
            sta L53E8,X
            lda #$25
            sta L00E0,X
            sta L00F0,X
            ldy RAMLO+1
            sta CDTMV5,Y
            txa
            sta CDTMV1,Y
            tya
            sta L0110,X
            pla
            sta RAMLO+1
L1598       rts
L1599       ldy L0110,X
            lda JVECK,Y
            and #$7E
            ora L0092
            bne L15C7
            lda L028D
            ora NEWADR
            ora L028F
            ora TXTROW
            ora TXTCOL
            ora TXTCOL+1
            and LCF93,Y
            bne L15C7
            lda LD419+2
            cmp ICBALZ
            bcs L15C7
            lda #$04
            sta L0092
L15C7       rts
L15C8       lda FKDEF+1
            lsr
            lsr
            lsr
            lsr
            lsr
            ora PALNTS
            sta CHKSUM
            clc
            eor #$FF
            adc #$01
            bmi L15DC
            sta CHKSUM
L15DC       sta L0120,X
            lda LOGCOL
            lsr
            lsr
            lsr
            lsr
            lsr
            ora ADRESS
            sta BUFRLO
            clc
            eor #$FF
            adc #$01
            bmi L15F3
            sta BUFRLO
L15F3       sta L0130,X
            ldy ICAX2Z
            lda L0170,Y
            sta L0170,X
            lda L0190,Y
L1601       sta L0190,X
            lda L0160,Y
            sta L0160,X
            lda L0180,Y
            sta L0180,X
            sec
            lda #$1A
            sbc L00E0,Y
            sta L0150,Y
            lda LD419+2
            and #$03
            clc
            adc #$02
            sta L01A0,Y
            lda ICBLHZ
            tay
            iny
            sty ATACHR
            asl
            asl
            tay
            sec
            lda CHKSUM
            sbc BUFRLO
            bcc L1642
            cmp BUFRLO
            bcs L164F
            iny
            lda PALNTS
            eor ADRESS
            bmi L164D
            bpl L164F
L1642       iny
            adc CHKSUM
            bcc L164E
            lda PALNTS
            eor ADRESS
            bpl L164F
L164D       iny
L164E       iny
L164F       tya
            asl
            adc #$A8
            sta L0140,X
            iny
            tya
            sta L01B0,X
            rts
L165C       ldy #$C6
            lda L0120,X
            bmi L166A
            bne L1668
            ldy #$24
            .byte $0C
L1668       ldy #$E6
L166A       sty L1680
            ldy #$C6
            lda L0130,X
            bmi L167B
            bne L1679
            ldy #$24
            .byte $0C
L1679       ldy #$E6
L167B       sty L1682
            bne L1684
L1680       bit KEYDEF
L1682       bit KEYDEF+1
L1684       ldy #$00
            lda (KEYDEF),Y
            bmi L1693
            cmp #$2D
            beq L169F
            cmp #$2E
            beq L1699
            rts
L1693       lda #$02
            sta L01A0,X
            rts
L1699       jsr L2668
            ldx ICAX2Z
            rts
L169F       lda L00E0
            pha
            lda L00E0,X
            sta L00E0
            jsr L25FB
            pla
            sta L00E0
            ldx ICAX2Z
            bcc L1699
            jsr L1817
            sec
            rts
L16B5       lda L47C0,X
            cmp #$14
            beq L16C0
            cmp #$11
            bne L16F8
L16C0       lda TEMPCHR
            lsr
            bcs L16CE
            lda KEYDEF
            ldy KEYDEF+1
            jsr L402C
            ldx ICAX2Z
L16CE       lda L00F0,X
            cmp #$40
            bcs L16F8
            lda TEMPCHR
            and #$03
            bne L16F3
            inc L00F0,X
            ldy L0110,X
            bmi L16F3
            lda LCF93,Y
            and LE644
            beq L16F3
            lda CHBAS
            bne L16F3
            lda #$14
            sta CHBAS
L16F3       lda #$02
            sta L01A0,X
L16F8       rts
L16F9       lda TXTMSC,X
            asl
            rol
            rol
            rol
            tay
            and #$07
            sta FMSZPG
            tya
            and #$F0
            ror
L1709       sta CRITIC
            lda LOGMAP+2,X
L170E       eor TXTMSC,X
L1711       cmp #$28
            and #$07
            tay
L1716       lda LE21E,Y
            ora #$10
L171B       sta FMSZPG+1
            lda #$00
            rol
            sta FMSZPG+2
L1722       rts
L1723       ldy L00E0,X
            lda L0120,X
            beq L1733
L172A       lda PALNTS
            and #$80
            eor LE286,Y
            bpl L1722
L1733       lda L0130,X
            beq L1741
            lda ADRESS
L173A       and #$80
            eor LE286,Y
            bpl L1722
L1741       lda LD419+2
            bmi L1722
            lda LE286,Y
            and #$3F
            sta L01A0,X
            jsr L2A01
            beq L1722
            jmp L1817
L1756       lda COLPM3
            sta BUFCNT
            jsr L1851
            jsr L19CA
            jsr L1C87
            jsr L1A7C
            rts
L1768       jsr L4180
            ldx ICAX1Z
            bit L00A1
            bvs L1780
            lda APPMHI
            asl
            adc #$88
            asl
            sbc L00E0,X
            bmi L1793
            asl
            adc #$88
            asl
            .byte $0C
L1780       lda #$40
            sta SWPFLG
            lda L00F0,X
            sbc SWPFLG
            bmi L17BF
            beq L17BF
            sta L00F0,X
            lda #$11
            sta ATACHR
L1793       rts
L1794       lda L0140,Y
            .byte $4B
            php
            eor #$04
            adc #$24
            adc L00E0,X
            asl
            asl
            adc L00F0,X
            bmi L17CF
            beq L17CF
            sta L00F0,X
            rts
L17AA       lda L0140,Y
            beq L17BE
L17AF       ldx ICAX1Z
            lda L00E0,X
            lsr
            adc L00F0,X
            sbc #$1F
            bmi L17CF
            beq L17CF
            sta L00F0,X
L17BE       rts
L17BF       ldy L00E0,X
            ldx LE226,Y
            lda LE2CA,X
            jsr L4105
            jsr L417A
            ldx ICAX1Z
L17CF       clc
            lda L00E0,X
            adc ICCOMZ
            bcc L17D8
            lda #$FF
L17D8       sta ICCOMZ
            lda #$12
            sta ATACHR
            .byte $0C
L17E0       ldx ICAX1Z
            lda L00E0,X
            cmp #$20
            bcs L17EB
            jsr L1208
L17EB       lda #$00
            ldy RAMLO+1
            sta L01B0,X
            lda #$40
            sta L00E0,X
L17F6       sta L0140,X
            sta L00F0,X
            sta CDTMV5,Y
L17FE       lda #$9D
            sta CDTMA2,Y
L1803       lda #$F7
            sta LE656,Y
            lda LCF93,Y
L180B       ora LE64B
L180E       sta LE64B
            rts
L1812       lda L47B0,X
            bmi L182A
L1817       sec
L1818       lda #$00
L181A       sbc L0120,X
            sta L0120,X
            sec
            lda #$00
            sbc L0130,X
            sta L0130,X
            rts
L182A       lda #$04
            sta L01A0,X
            rts
L1830       ldx ICAX1Z
L1832       lda #$00
            sta L00F0,X
            lda #$FF
            sta L00E0,X
            ldy L0110,X
            bmi L1845
            sta L0110,X
            jsr L2347
L1845       txa
            ldx LCFF8
            inx
            stx LCFF8
            sta LCFE9,X
            rts
L1851       lda BUFCNT
L1853       and #$7E
            bne L185B
            sta JVECK
            rts
L185B       ldy #$00
            sty ICAX6Z
            ldy #$7F
            sty DSKUTL
            ldy #$07
            asl
L1866       dey
            asl
            bcc L1866
            pha
            sty RAMLO+1
            .byte $BF
            clc
            .byte $02
            bmi L18AE
            stx ICAX1Z
            lda STRIG0,Y
            bne L18AE
            lda PTRIG0,Y
            adc #$3B
            adc #$2F
            bcc L18EA
            sbc #$17
            sta L00C2
            bcs L188C
            eor #$FF
            adc #$01
L188C       sta L00C4
            clc
            lda PADDL4,Y
            adc #$3F
            adc #$29
            bcc L18EA
            sbc #$14
            sta L00C3
            bcs L18A2
            eor #$FF
            adc #$01
L18A2       sta L00C5
L18A4       lda L00E0,X
            and #$60
            beq L18CF
            cmp #$40
            bcc L18F8
L18AE       bne L18EA
            ldx L00C5
            lda L00C4
            cmp LE674,X
            bcs L18EA
            bit L00A1
            bmi L18C8
            sec
            lda L00F0
            sbc BPTR
            bcs L18C6
            lda #$00
L18C6       sta L00F0
L18C8       lda #$99
            sta CHBAS
            bne L18EA
L18CF       ldx L00C5
            lda L00C4
            cmp LE65F,X
            bcs L18EA
            lda LCF93,Y
            ora ICAX6Z
            sta ICAX6Z
L18DF       lda LCF93,Y
            and JVECK
            bne L18EA
            jsr L192E
L18EA       pla
            beq L18F2
L18ED       ldy RAMLO+1
            jmp L1866
L18F2       lda ICAX6Z
            sta JVECK
            rts
L18F8       stx L190A+1
L18FB       lda L01B0,X
            tax
L18FF       ldy #$0D
            jsr L1BFD
            bmi L18EA
            bit L00A1
            bmi L1921
L190A       ldx #$00
            lda L0140,X
            cmp #$B8
            lda L0100
            rol
            tax
            lda L00F0
L1918       sbc LE8A9,X
            bcs L191F
            lda #$00
L191F       sta L00F0
L1921       jsr L1830
            lda #$19
            sta CHBAS
            bne L18EA
L192B       stx ICDNOZ
            rts
L192E       ldx ICAX1Z
L1930       ldy BUFADR+1
            beq L192B
            lda #$1A
            sta CHBAS
            lda LD419+2
            and #$07
            ora #$08
            sta L01A0,X
            jsr L1812
            asl DSKUTL
            bcs L197C
            lda RTCLOK+1
            beq L1955
            eor #$FF
            adc #$01
            asl
            bne L1961
L1955       lda L0130,X
            beq L1963
            bmi L195F
            lda #$02
            .byte $0C
L195F       lda #$FE
L1961       sta RTCLOK+1
L1963       lda BRKKEY
            beq L196E
            eor #$FF
            adc #$01
            asl
            bne L197A
L196E       lda L0120,X
            beq L197C
            bmi L1978
            lda #$02
            .byte $0C
L1978       lda #$FE
L197A       sta BRKKEY
L197C       bit L00A1
            bvs L199E
            clc
            lda L00E0
            adc #$03
            sbc L00E0,X
            bcs L19A0
            bit L00A1
            bmi L199D
            eor #$FF
            lsr
            sta SWPFLG
            sec
            lda L00F0
            sbc SWPFLG
            bcs L199B
            lda #$00
L199B       sta L00F0
L199D       rts
L199E       lda #$20
L19A0       asl
            sta SWPFLG
            sec
            lda L00F0,X
            sbc SWPFLG
            sta L00F0,X
            beq L19AE
            bpl L19C9
L19AE       ldy L00E0,X
            clc
            tya
            adc ICCOMZ
            bcc L19B8
            lda #$FF
L19B8       sta ICCOMZ
L19BA       ldx LE226,Y
            lda LE2D4,X
            jsr L4105
            jsr L417A
            jsr L17E0
L19C9       rts
L19CA       ldy #$00
            lda BUFCNT
            bpl L19D4
            and #$7E
            bne L19D8
L19D4       sty TINDEX
            rts
L19D8       sty ICAX6Z
            ldy #$07
            asl
L19DD       dey
            asl
            bcc L19DD
            sty RAMLO+1
            pha
            lda PTRIG0,Y
            sbc PTRIG7
            sta L00C2
            lda STRIG0,Y
            sbc IMASK
            bcs L1A04
            cmp #$FF
            bne L1A0C
            lda L00C2
            cmp #$E9
            bcc L1A0C
            eor #$FF
            adc #$00
            bcc L1A0F
L1A04       bne L1A5F
            lda L00C2
            cmp #$18
            bcc L1A0F
L1A0C       jmp L1A5F
L1A0F       sta L00C4
            sec
            lda PADDL4,Y
            sbc STICK3
            sta L00C3
            bcs L1A26
            cmp #$EC
            bcc L1A5F
            eor #$FF
            adc #$00
            bcc L1A2A
L1A26       cmp #$15
            bcs L1A5F
L1A2A       sta L00C5
            ldy RAMLO+1
            .byte $BF
            clc
            .byte $02
            bmi L1A5F
            stx ICAX1Z
            lda L01B0,X
            tax
            ldy L01B0
            jsr L1BFD
            bmi L1A5F
            ldy RAMLO+1
            lda LCF93,Y
            ora ICAX6Z
            sta ICAX6Z
            ldx ICAX1Z
            lda L00E0,X
            and #$60
            beq L1A5C
            cmp #$40
            bcs L1A5F
            jsr L17E0
            jmp L1A5F
L1A5C       jsr L1768
L1A5F       pla
            beq L1A67
            ldy RAMLO+1
            jmp L19DD
L1A67       lda ICAX6Z
            sta TINDEX
            beq L1A7B
            lda LE644
            and #$7F
            sta LE644
            lda #$20
            sta SDMCTL
L1A7B       rts
L1A7C       lda BUFCNT
            and #$7E
            asl
            sta RAMTOP
            ldy #$06
L1A85       asl RAMTOP
            bcc L1AB2
            beq L1AB2
            ldx CDTMV1,Y
            bmi L1AB2
            sty RAMLO+1
            sty L1AAD+1
            sty ICAX2Z
            stx SAVADR
            lda JVECK,Y
            sta STATUS
            lda #$00
            sta ICAX6Z
            lda L00E0,X
            .byte $4B
            rts
            lsr
            sta MLTTMP+1
            lda RAMTOP
            bne L1ABB
L1AAD       ldy #$00
            lda ICAX6Z
            .byte $0C
L1AB2       lda #$00
            sta JVECK,Y
            dey
            bne L1A85
            rts
L1ABB       dey
            asl
            bcc L1ABB
            sty L1B25+1
            pha
            ldx ICAX2Z
            lda PTRIG0,X
            sbc PTRIG0,Y
            sta L00C2
            lda STRIG0,X
            sbc STRIG0,Y
            bcs L1AE5
            cmp #$FF
            bne L1B25
            lda L00C2
            cmp #$E9
            bcc L1B25
            eor #$FF
            adc #$00
            bcc L1AED
L1AE5       bne L1B25
            lda L00C2
            cmp #$18
            bcs L1B25
L1AED       sta L00C4
            sec
            lda PADDL4,X
            sbc PADDL4,Y
            sta L00C3
            bcs L1B04
            cmp #$EC
            bcc L1B25
            eor #$FF
            adc #$00
            bcc L1B08
L1B04       cmp #$15
            bcs L1B25
L1B08       sta L00C5
L1B0A       .byte $BF
            clc
            .byte $02
L1B0D       bmi L1B25
            sty RAMLO+1
            stx ICAX1Z
            lda L00E0,X
            and #$60
            ora MLTTMP+1
L1B19       lsr
            lsr
            lsr
            tax
L1B1D       lda L1BE5,X
            sta L1B23+1
L1B23       bne L1B23
L1B25       ldy #$00
            pla
            bne L1ABB
            jmp L1AAD
            ldx L00C5
L1B2F       lda L00C4
            cmp LE65F,X
            bcs L1B25
            lda LCF93,Y
            ora ICAX6Z
            sta ICAX6Z
L1B3D       lda LCF93,Y
            and STATUS
            bne L1B25
            lda LD419+2
            php
            bpl L1B4D
            jsr L1BD4
L1B4D       ldx ICAX1Z
            jsr L1812
            ldx SAVADR
            lda #$10
            sta L01A0,X
            plp
            bpl L1B25
            jsr L1BD4
            bpl L1B25
            jsr L1BF1
            bmi L1B25
            jsr L1830
            ldx ICAX2Z
            stx RAMLO+1
            ldy ICAX1Z
            ldx SAVADR
            jsr L1794
            jmp L1B25
            jsr L1BF1
            bmi L1B25
            ldx ICAX1Z
            ldy SAVADR
            jsr L1794
L1B83       ldx SAVADR
            jsr L1832
            pla
            jmp L1AAD
            jsr L1BF1
            bmi L1B25
            bpl L1B83
            bne L1BB3
            bne L1BC9
            ldy ICAX1Z
            lda SAVADR
            sta ICAX1Z
            lda ICAX2Z
            sta RAMLO+1
            .byte $0C
            ldy SAVADR
            ldx L00C5
            lda L00C4
            cmp LE674,X
            bcs L1BB0
L1BAD       jsr L17AA
L1BB0       jmp L1B25
L1BB3       jsr L1BF1
            bmi L1BB0
            jsr L17E0
            lda SAVADR
            sta ICAX1Z
            lda ICAX2Z
            sta RAMLO+1
            jsr L17E0
            jmp L1B25
L1BC9       jsr L1BF1
            bmi L1BB0
            jsr L1830
            jmp L1B25
L1BD4       lda ICAX2Z
            ldx RAMLO+1
            sta RAMLO+1
            stx ICAX2Z
            lda SAVADR
L1BDE       ldx ICAX1Z
            sta ICAX1Z
            stx SAVADR
            rts
L1BE5       php
            .byte $52
            adc L3BFF+1,X
            ror.w ROWAC
            .byte $72,$67,$00,$00
L1BF1       ldx SAVADR
            ldy ICAX1Z
            lda L01B0,X
            tax
            lda L01B0,Y
            tay
L1BFD       clc
L1BFE       lda L00C4
            adc LE6A5,X
L1C03       adc LE6A5,Y
            cmp #$17
            bcs L1C3D
            lda L00C3
L1C0C       adc LE689,X
            sec
            sbc LE689,Y
            bpl L1C40
            clc
            adc LE697,X
            bmi L1C3F
L1C1B       sta L00C6
L1C1D       sbc LE697,X
            eor #$FF
            adc #$01
            asl
            adc LE6B3,X
            sta BUFRHI
            lda #$00
            adc LE6C1,X
L1C2F       sta BFENLO
            lda LE6B3,Y
            sta BFENHI
            lda LE6C1,Y
            sta LTEMP
            bcc L1C66
L1C3D       lda #$FF
L1C3F       rts
L1C40       sta L1C57+1
            clc
            sbc LE697,Y
            bpl L1C3D
            eor #$FF
            sta L00C6
            lda LE6B3,X
            sta BUFRHI
L1C52       lda LE6C1,X
            sta BFENLO
L1C57       lda #$00
            asl
            adc LE6B3,Y
            sta BFENHI
            lda #$00
            adc LE6C1,Y
            sta LTEMP
L1C66       ldx L00C6
            ldy #$00
L1C6A       clc
            lda L00C2
            adc (BUFRHI),Y
            sec
            sbc (BFENHI),Y
            bpl L1C7C
            iny
            clc
            adc (BUFRHI),Y
            bmi L1C82
L1C7A       lsr
            rts
L1C7C       iny
            clc
            sbc (BFENHI),Y
            bmi L1C7A
L1C82       iny
            dex
            bpl L1C6A
            rts
L1C87       lda BUFCNT
            and #$7E
            beq L1CC1
            asl
            sta RAMTOP
            sta L1CB3+1
            ldy #$06
L1C95       asl RAMTOP
            bcc L1CBE
            ldx CDTMV1,Y
            bmi L1CBE
            lda L00E0,X
            and #$60
            bne L1CBE
            lda L01A0,X
            bne L1CBE
            lda L53E8,X
            sta L1CD8+1
            sty RAMLO+1
            stx SAVADR
L1CB3       lda #$00
            ldy #$06
            bpl L1CC7
L1CB9       tax
            bne L1CC7
L1CBC       ldy RAMLO+1
L1CBE       dey
L1CBF       bne L1C95
L1CC1       rts
L1CC2       ldy #$00
L1CC4       pla
            beq L1CBC
L1CC7       dey
            asl
            bcc L1CC7
            cpy RAMLO+1
            beq L1CB9
            ldx CDTMV1,Y
            bmi L1CB9
            pha
            lda L53E8,X
L1CD8       cmp #$00
            bne L1CC4
            lda L00E0,X
            and #$60
            cmp #$40
            bne L1CC4
            sty L1CC2+1
            ldy SAVADR
            sec
            lda L01D0,Y
            sbc L01D0,X
            beq L1D0A
            bcs L1CFF
            cmp #$FA
            bcc L1CC2
            lda L0130,Y
L1CFB       bmi L1CC2
L1CFD       bpl L1D29
L1CFF       cmp #$06
            bcs L1CC2
            lda L0130,Y
            bpl L1CC2
            bmi L1D29
L1D0A       sec
            lda L01C0,Y
            sbc L01C0,X
L1D11       beq L1CC2
            bcs L1D20
            cmp #$FA
L1D17       bcc L1CC2
L1D19       lda L0120,Y
L1D1C       bmi L1CC2
L1D1E       bpl L1D29
L1D20       cmp #$06
            bcs L1CC2
            lda L0120,Y
            bpl L1CC2
L1D29       lda #$08
            sta L01A0,Y
            inc L1D3A
            bne L1D36
            inc L1D3B
L1D36       pla
            jmp L1CBC
L1D3A       .byte $00
L1D3B       .byte $00
            pha
            lda #$5F
            sta COLPM0
            sta COLPF3
            lda #$5A
            sta LFFFE
            lda L00A3
            bne L1D53
            lda #$4C
            sta L135C
L1D53       lda #$00
            sta LD030
            pla
            rti
            pha
            lda OLDCHR
            sta LD021
            lda #$10
            ora L025E
            sta GRAFM
            cmp #$16
            bne L1D7B
            lda #$CF
            sta LFFFE
            lda L1E2C+1
            sta L1DEA+1
            lda #$74
            bne L1D85
L1D7B       lda #$0C
            sta LFFFE
            inc LFFFF
            lda #$76
L1D85       sta COLPM0
            lda #$23
            sta COLPF2
            sta COLPF3
            lda #$C0
            sta LDC01+1
            asl
            sta LDBFF+1
            asl
            sta LCA0A
L1D9D       lda #$D0
            ora L025D
            sta COLPF0
            lda L0090
            sta L1E2A+1
            sta L1DEC+1
            asl L00A2
            beq L1DCD
            stx L1DCB+1
            ldx #$0C
L1DB6       lda L5140,X
            sta L4940,X
            lda L514D,X
L1DBF       sta L494D,X
            lda L515A,X
            sta L495A,X
            dex
            bpl L1DB6
L1DCB       ldx #$00
L1DCD       pla
            rti
            pha
            sty L1E54+1
            cld
            lda #$01
            sta LFFFE
            lda #$1E
            sta LFFFF
            lda #$76
            sta COLPM0
            inc COLPF3
L1DE6       sbc #$0F
            bcs L1DE6
L1DEA       lda #$4F
L1DEC       ldy #$00
            cli
            cli
            cli
            cli
            cli
            cli
            cli
            cli
            cli
            cli
            cli
            cli
            cli
            cli
            cli
            cli
            cli
L1DFF       bcc L1DFF
L1E01       sta COLPF2
            sty LD021
            pla
            pla
            pla
L1E0A       bne L1E34
            pha
            sty L1E54+1
            cld
            lda LDD06
            eor #$0F
L1E16       sta L1E19+1
L1E19       bpl L1E19
L1E1B       cmp #$C9
            cmp #$C9
            cmp #$C9
            cmp #$C9
            cmp #$C9
            cmp #$C9
            cmp #$C5
            nop
L1E2A       ldy #$00
L1E2C       lda #$4F
            sta COLPF2
            sty LD021
L1E34       lda #$F7
L1E36       sta COLPM0
            sta COLPF3
            lda #$5E
            sta LFFFE
            lda #$1E
            sta LFFFF
            stx L1E52+1
            jsr L42BB
            jsr L0B5C
            jsr L3B2E
L1E52       ldx #$00
L1E54       ldy #$00
            asl L00A3
            lda #$00
            sta RMARGN
            pla
            rti
            pha
            lda #$01
            sta LD030
            sta COLPF3
            sta LD021
            sta RMARGN
            inc HOLD1
            bit L00A3
            bpl L1EA6
            lda LE644
            sta COLPM3
            lda LE64B
            sta VDELAY
            lda LE63F
            sta GRAFP3
            stx L1EA4+1
            ldx #$07
L1E89       lda LE62F,X
            sta HPOSP0,X
            lda LE637,X
            sta SIZEP0,X
            lda CDTMA2,X
            sta L53F8,X
            lda LE656,X
            sta LD026+1,X
            dex
            bpl L1E89
L1EA4       ldx #$00
L1EA6       lda #$10
            sta GRAFM
            lsr
            sta COLPF0
            lda #$4D
            sta COLPF2
            lda #$2F
            sta COLPM0
            lda #$3C
            sta LFFFE
            lda #$1D
            sta LFFFF
            pla
            rti
L1EC5       jsr L1EC8
L1EC8       jsr L1ECB
L1ECB       bit TRIG1
            bpl L1ECB
L1ED0       bit TRIG1
            bmi L1ED0
            rts
L1ED6       ldx #$F9
            lda LE8C5
            asl
            lda OLDCOL
            ror
            bne L1EE2
            dex
L1EE2       stx ICAX1Z
            lda DRKMSK
            sta L025D
            lda COLRSH
            sta L025E
            lda #$40
            sta L00A2
            ldx #$00
            clc
            lda #$07
            adc DSKUTL+1
            sta ICCOMT
            txa
            adc ABUFPT
            lsr
            ror ICCOMT
            lsr
            ror ICCOMT
            lsr
            ror ICCOMT
            clc
            lda #$07
            adc ABUFPT+1
            sta DSKFMS
L1F0E       txa
            adc ABUFPT+2
            lsr
L1F12       ror DSKFMS
            lsr
            ror DSKFMS
            lsr
            ror DSKFMS
            lda DSKFMS
            bpl L1F3B
            sta ICBLHZ
L1F20       lda LCF56,X
            asl
            sta BUFRHI
            lda LCF68,X
            sta BFENLO
            ldy #$26
            lda #$87
L1F2F       sta (BUFRHI),Y
            dey
            bpl L1F2F
            inx
            inc ICBLHZ
            bmi L1F20
            lda #$00
L1F3B       ora #$80
            sta L1F8F
            sta L1F9C
            sta L1FA9
            lda ICCOMT
            sta L1F8E
            sta L1F9B
            sta L1FA8
            stx ICAX2Z
            lda LCF56,X
            ora #$80
            bmi L1F5D
L1F5A       lda LCF56,X
L1F5D       asl
            sta L1F90+1
            sta L1F9D+1
            sta L1FAA+1
            sta L1F96+1
            sta L1FA3+1
            sta L1FB0+1
            bcc L1F8B
            beq L1FEE
            lda LCF68,X
            sta L1F90+2
            sta L1F9D+2
            sta L1FAA+2
            eor #$88
            sta L1F96+2
            sta L1FA3+2
            sta L1FB0+2
L1F8B       ldy #$26
L1F8D       .byte $BF
L1F8E       .byte $00
L1F8F       .byte $80
L1F90       sta L5000,Y
            lda LC100,X
L1F96       sta AFP,Y
            dey
            .byte $BF
L1F9B       .byte $00
L1F9C       .byte $80
L1F9D       sta L5000,Y
            lda LC100,X
L1FA3       sta AFP,Y
            dey
            .byte $BF
L1FA8       .byte $00
L1FA9       .byte $80
L1FAA       sta L5000,Y
            lda LC100,X
L1FB0       sta AFP,Y
            dey
            bpl L1F8D
            ldx L1F8F
            inx
            stx L1F8F
            stx L1F9C
            stx L1FA9
            lda L1F90+1
            cmp ICAX1Z
            bne L1FCD
            jsr L2039
L1FCD       cpx #$C0
            inc ICAX2Z
            ldx ICAX2Z
            bcc L1F5A
L1FD5       lda LCF56,X
            asl
            beq L1FEE
            sta BUFRHI
            lda LCF68,X
            sta BFENLO
            ldy #$26
            lda #$87
L1FE6       sta (BUFRHI),Y
            dey
            bpl L1FE6
            inx
            bpl L1FD5
L1FEE       lda #$07
            adc DSKFMS
            ora #$80
            sta DSKFMS
            clc
            lda #$13
            adc ICCOMT
            sta ICCOMT
            iny
            lda (ICCOMT),Y
            sta DSKFMS+1
            ldx #$0E
            lsr ICAX1Z
            bcs L2082
L2008       txa
            eor TEMPCHR
            and #$03
            bne L2025
            lda LD419+2
            eor LDD06
            and #$0F
            sta L202D+1
            and #$03
            ora FREQ
            tay
            lda LE1C2,Y
            sta LE8E3,X
L2025       dex
            bpl L2008
            lda LD419+2
            bne L2035
L202D       ldx #$00
            dex
            bmi L2035
            jsr L208B
L2035       lda #$54
            bne L2055
L2039       stx L2080+1
L203C       ldx #$00
            lda #$00
            sta L7CF0,X
            lda COLRSH
            eor #$07
            tax
            stx L203C+1
            ldy DRKMSK
            lda LCF93,Y
            sta L7CF0,X
            lda #$53
L2055       sta L2061+1
            ldx #$00
            stx BUFRHI
            stx BFENHI
L205E       lda LE8C5,X
L2061       cmp #$53
            bcs L2080
            sta BFENLO
            eor #$88
            sta LTEMP
            ldy LE8D4,X
            lda (BUFRHI),Y
            eor #$87
            bne L207D
            lda #$9E
            sta (BUFRHI),Y
            lda LE8E3,X
            sta (BFENHI),Y
L207D       inx
            bne L205E
L2080       ldx #$00
L2082       rts
L2083       ldx #$0E
L2085       jsr L208B
            dex
            bne L2085
L208B       lda LD419+2
            eor LDD06
            and #$7F
            cmp #$78
            bcs L208B
            adc LCF7A,X
            sta LE8D4,X
            tay
            lda #$00
            adc LCF69,X
            sta LE8C5,X
            cpy #$E8
            sbc #$53
            bcs L208B
            rts
L20AD       stx L20E2+1
            ldy RAMLO+1
            ldx LCF93,Y
            lda LE656,Y
            sta DOSVEC+1
            lda CDTMA2,Y
            sta DOSINI
            tya
            asl
            tay
            lda LE62F,Y
            sta TRAMSZ
            lda LE630,Y
            sta WARMST
            txa
            and LE63F
            beq L20D4
            lda #$FF
L20D4       sta CMCMD
            txa
            and LE644
            sta BOOT
            txa
            and LE64B
            sta DOSVEC
L20E2       ldx #$00
            rts
L20E5       stx CHKSUM
            sty BUFRLO
            ldy #$07
L20EB       lda (CHKSUM),Y
            sta RAMLO+1,Y
            dey
            bpl L20EB
L20F3       stx CASINI
            ldy RAMLO+1
            lda LCF93,Y
            tax
            lda TRAMSZ
            sta PTRIG0,Y
            lda CMCMD
            sta STRIG0,Y
            lda WARMST
            sta PADDL4,Y
            lda DOSVEC+1
            sta LE656,Y
            lda DOSINI
            sta CDTMA2,Y
            tya
            asl
            tay
            lda TRAMSZ
            sta LE62F,Y
            lda WARMST
L211E       sta LE630,Y
            txa
            eor #$FF
            tay
            lda CMCMD
            beq L212F
            txa
            ora LE63F
            bne L2133
L212F       tya
            and LE63F
L2133       sta LE63F
            lda DOSVEC
            beq L2140
            txa
            ora LE64B
            bne L2144
L2140       tya
            and LE64B
L2144       sta LE64B
            lda BOOT
            beq L2157
            lda WARMST
            cmp #$63
            bcc L2157
            txa
            ora LE644
            bne L215B
L2157       tya
            and LE644
L215B       sta LE644
            ldx CASINI
            rts
L2161       ldx RAMLO+1
            lda LCF9D,X
            sta BUFRHI
            lda LCFA5,X
            sta BFENLO
            lda #$80
            ldy #$08
            sta (BUFRHI),Y
            ldy #$35
            sta (BUFRHI),Y
            ldx DOSINI+1
            cpx #$18
            bcc L218E
            ldx #$0A
            ldy #$12
            jsr L21AB
            ldy #$12
            jsr L21AB
            ldy #$12
            jmp L21AB
L218E       lda LE226,X
            ldy #$12
            jsr L21AA
            ldx DOSINI+1
            lda LE23E,X
            pha
            lsr
            lsr
            lsr
            lsr
            ldy #$13
            jsr L21AA
            pla
            and #$0F
            ldy #$14
L21AA       tax
L21AB       lda LE55B,X
            sta (BUFRHI),Y
            iny
            iny
            iny
            lda LE565,X
            sta (BUFRHI),Y
            iny
            iny
            iny
            lda LE56F,X
            sta (BUFRHI),Y
            iny
            iny
            iny
            lda LE579,X
            sta (BUFRHI),Y
            iny
            iny
            iny
            lda LE583,X
            sta (BUFRHI),Y
            iny
            iny
            iny
            lda LE58D,X
            sta (BUFRHI),Y
            iny
            iny
            iny
            lda LE597,X
            sta (BUFRHI),Y
            iny
            iny
            iny
            lda LE5A1,X
            sta (BUFRHI),Y
            rts
L21E9       ldx #$0F
L21EB       stx ICAX2Z
            ldy L0110,X
            bmi L220C
            lda L00E0,X
            cmp #$20
            bcs L220C
            lda LCF93,Y
            and LE644
            beq L220C
            lda L00F0,X
            beq L220C
            dec L0140,X
            bpl L220C
            jsr L225D
L220C       ldx ICAX2Z
            dex
            bpl L21EB
            lda L00F0
            cmp #$08
            ldx BUFADR+1
            beq L2241
            bcs L223B
            cmp APPMHI+1
            lda TEMPCHR
            and #$07
            bcs L2226
            ora #$08
            sec
L2226       tay
            lda LCFC5,Y
            sta LE656
            lda TEMPCHR
            and #$1F
            bne L2238
            ldy #$08
            sty ATACHR
L2238       bcc L2253
            rts
L223B       lda #$F1
            sta LE656
            rts
L2241       bcs L224B
            lda TEMPCHR
            and #$07
            ora #$10
            bne L2226
L224B       lda LDBED+1
            sta LE656
            lda TEMPCHR
L2253       and #$07
            bne L225C
            lda #$1C
            sta CHBAS
L225C       rts
L225D       lda #$41
            sbc L00F0,X
            lsr
            lsr
            lsr
            sta L0140,X
            sec
L2268       lda LCF9D,Y
            sta BUFRHI
            lda LCFA5,Y
            sta BFENLO
            ldy #$3F
            lda #$00
            adc (BUFRHI),Y
            and #$07
            sta (BUFRHI),Y
            tax
            lda LE5AB,X
            ldy #$33
            sta (BUFRHI),Y
            ldy #$06
            sta (BUFRHI),Y
            lda LE5B3,X
            iny
            sta (BUFRHI),Y
            ldy #$34
            sta (BUFRHI),Y
            lda LE5BB,X
            ldy #$09
            sta (BUFRHI),Y
            ldy #$30
            sta (BUFRHI),Y
            lda LE5C3,X
            iny
            sta (BUFRHI),Y
            ldy #$0A
            sta (BUFRHI),Y
            lda LE5CB,X
            iny
            sta (BUFRHI),Y
            ldy #$32
            sta (BUFRHI),Y
            lda LE5D3,X
            ldy #$0C
            sta (BUFRHI),Y
            ldy #$2D
            sta (BUFRHI),Y
            lda LE5DB,X
            iny
            sta (BUFRHI),Y
            ldy #$0D
            sta (BUFRHI),Y
            lda LE5E3,X
            iny
            sta (BUFRHI),Y
            ldy #$2F
            sta (BUFRHI),Y
            txa
            and #$04
            beq L22D7
            lda #$38
L22D7       ldy #$01
            sta (BUFRHI),Y
            ldy #$04
            sta (BUFRHI),Y
            eor #$38
            ldy #$3A
            sta (BUFRHI),Y
            ldy #$37
            sta (BUFRHI),Y
            rts
L22EA       ldy #$00
            sty CHKSUM
            sty L232A+1
            sec
            ror
            ror CHKSUM
            lsr
            ror CHKSUM
            sta BUFRLO
            txa
            sec
            ror
            ror L232A+1
            lsr
            ror L232A+1
            sta L232A+2
            ldy #$3F
            lda (CHKSUM),Y
            and #$80
            sta DOSVEC
            ldx #$3C
            lda #$03
            sta ICAX3Z
L2315       dey
            lda (CHKSUM),Y
            sta ICBLHZ
            lda #$01
L231C       lsr ICBLHZ
            bit DOSVEC
            bpl L2327
            php
            lsr ICBLHZ
            rol
            plp
L2327       rol
            bcc L231C
L232A       sta L8000,X
            inx
            dec ICAX3Z
            bne L2315
            txa
            .byte $CB
            asl POKMSK
            .byte $DA
            rts
L2338       lda #$05
            ldy #$06
            .byte $0C
L233D       tya
            dey
            sta LCFF9,Y
            bne L233D
            rts
            ldy RAMLO+1
L2347       lda #$FF
            sta CDTMV5,Y
            sta CDTMV1,Y
            eor LCF93,Y
            and LE644
            sta LE644
            tya
            inc LCFFF
            ldy LCFFF
            sta LCFF9,Y
            rts
L2363       ldx #$28
L2365       stx TRAMSZ
            ldx DOSINI+1
            clc
            lda LCBCF,X
            adc #$CF
            sta ICAX4Z
            lda LCBE7,X
            adc #$CB
            sta ICAX5Z
            ldx #$06
L237A       lda LE19B,X
            sta RAMLO+1,X
            dex
            bpl L237A
            ldy DOSINI+1
            ldx LE29E,Y
            lda LE2B6,X
            sta DOSVEC+1
            lsr
            lsr
            lsr
            lsr
            sta LD026
            bpl L239B
L2395       lda WARMST
            adc OLDROW
            sta WARMST
L239B       ldy RAMLO+1
            lda (ICAX4Z),Y
            sta DOSINI
            lda RAMLO+1
            lsr
            adc #$50
            sta SAVMSC+1
            tax
            lda DOSINI
            jsr L22EA
            ldy #$3F
            lda (CHKSUM),Y
            and #$1F
            sta OLDROW
            jsr L20F3
            lda #$18
            jsr L23DC
            lda (ICAX4Z),Y
            beq L23C3
            .byte $0C
L23C3       lda SAVMSC+1
            sta DOSINI
            jsr L20F3
            lda #$E8
            jsr L23DC
            cpy #$08
            bcc L2395
            ldx ICAX4Z
            ldy ICAX5Z
            stx OLDADR+1
            sty FKDEF
            rts
L23DC       clc
            adc TRAMSZ
            sta TRAMSZ
            inc RAMLO+1
            ldy RAMLO+1
            rts
L23E6       lda #$00
            tax
L23E9       sta L5400,X
            inx
            bne L23E9
            ldx #$03
L23F1       stx RAMLO+1
            lda CDTMV5,X
            bpl L23FB
            txa
            bne L240C
L23FB       sta DOSINI+1
            lda #$00
L23FF       sta DOSVEC+1
            sta DOSVEC
            jsr L2161
            ldy RAMLO+1
            clc
            jsr L2268
L240C       ldx RAMLO+1
            dex
            bpl L23F1
            rts
L2412       lda #$FF
            .byte $0C
L2415       lda #$00
L2417       sta LE644
            lda #$40
            sta L00A3
            rts
L241F       lda #$FF
            .byte $0C
L2422       lda #$00
L2424       jsr L2417
L2427       bit L00A3
            bpl L2427
L242B       lda RMARGN
            beq L242B
            rts
L2430       bit DSKFMS
            bpl L248A
            sec
            lda CRITIC
            sbc DSKUTL+1
            tay
            lda FMSZPG
            sbc ABUFPT
            sta FMSZPG+4
            tya
            clc
            adc #$0C
            sta FMSZPG+3
            lda FMSZPG+4
            adc #$00
            sta FMSZPG+4
            beq L2463
            cmp #$FF
            beq L245D
            lsr
            bne L248A
            lda FMSZPG+3
            cmp #$7C
            bcc L2463
            bcs L248A
L245D       lda FMSZPG+3
            cmp #$DC
            bcc L248A
L2463       sec
            lda FMSZPG+1
            sbc ABUFPT+1
            tay
            lda FMSZPG+2
            sbc ABUFPT+2
            sta FMSZPG+6
            tya
            clc
            adc #$64
            sta FMSZPG+5
            tay
            lda FMSZPG+6
            adc #$00
            sta FMSZPG+6
            lsr
            bne L248A
            bcc L2487
            cpy #$18
            bcs L248A
            sec
            rts
L2487       cpy #$3C
            rts
L248A       clc
            rts
L248C       txa
            eor TEMPCHR
            lsr
            php
            ldy #$00
            sec
            lda KEYDEF+1
            sbc DSKFMS
            bcs L24A0
            eor #$FF
            adc #$01
            ldy #$F8
L24A0       sty ADRESS
            tay
            php
            ldx #$00
            sec
            lda KEYDEF
            sbc ICCOMT
            bcs L24B3
            eor #$FF
            adc #$01
            ldx #$F8
L24B3       stx PALNTS
            tax
            php
            asl
            asl
            asl
            asl
            sta L24C5+1
            lda #$46
            adc #$00
            sta L24C5+2
L24C5       lda L4600,Y
            sty CASINI
            cpx CASINI
            bcs L2532+1
            sta L250F+1
            bit PALNTS
            bpl L24D9
            eor #$FF
            adc #$01
L24D9       sta FKDEF+1
            lda ADRESS
            beq L24E1
            lda #$FE
L24E1       eor #$FF
            sta LOGCOL
            lda #$00
            plp
            rol
            plp
            rol
            tay
            plp
            bcs L251F
            lda L2527,Y
            sta L2507
            lda L252A+1,Y
            sta L2514
            lda DSKFMS
            sta L250A+2
            ldx ICCOMT
            lda #$80
            clc
            bcc L250F
L2507       inc L250A+2
L250A       ldy L8000,X
            bmi L251C
L250F       adc #$00
            bcc L2515
            clc
L2514       inx
L2515       dec CASINI
            bne L2507
            lda #$01
            rts
L251C       lda #$00
            rts
L251F       ldx ICAX2Z
L2521       lda L47B0,X
            and #$02
            rts
L2527       dec LCEEE
L252A       inc LCACA
            inx
            inx
L252F       inc LCECE
L2532       inc L7B8D
            and ICBALZ
            .byte $64
            bpl L253E
            eor #$FF
            adc #$00
L253E       sta LOGCOL
            lda PALNTS
            beq L2546
            lda #$FE
L2546       eor #$FF
            sta FKDEF+1
            plp
            bcs L2555
            lda KEYDEF+1
            ldx KEYDEF
            ldy ICCOMT
            bcc L255B
L2555       lda DSKFMS
            ldx ICCOMT
            ldy KEYDEF
L255B       sta L2575+2
            sty L2582+1
            lda #$00
            rol
            plp
            rol
            tay
            plp
            bcs L251F
            lda L252F,Y
            sta L257E
            lda #$80
            clc
            bcc L257A
L2575       ldy L8000,X
            bmi L2589
L257A       adc #$00
            bcc L2581
L257E       dec L2575+2
L2581       inx
L2582       cpx #$00
            bcc L2575
            lda #$01
            rts
L2589       lda #$00
            rts
L258C       ldx #$00
            ldy LF930
            beq L25A1
L2593       lda LF931,X
            and #$7F
            sta LF931,X
            txa
            .byte $CB,$FC
            dey
            bne L2593
L25A1       rts
L25A2       ldx #$00
            ldy LF930
L25A7       beq L25C7
L25A9       lda FTYPE
            cmp LF931,X
            bne L25C1
            lda LF932,X
            sta ICAX6Z
            lda LF933,X
            sta STATUS
            sty ICBLHZ
            jsr L25C8
            ldy ICBLHZ
L25C1       txa
            .byte $CB,$FC
            dey
            bne L25A9
L25C7       rts
L25C8       lda #$2D
            .byte $0C
L25CB       lda #$2E
            pha
            ldy #$00
            lda (ICAX6Z),Y
            cmp #$25
            beq L25E8
            pla
            inc STATUS
            sta (ICAX6Z),Y
            ldy #$03
            sta (ICAX6Z),Y
            inc STATUS
            sta (ICAX6Z),Y
            ldy #$00
            sta (ICAX6Z),Y
            rts
L25E8       pla
            iny
            sta (ICAX6Z),Y
            iny
            sta (ICAX6Z),Y
            inc STATUS
            inc STATUS
            inc STATUS
            sta (ICAX6Z),Y
            dey
            sta (ICAX6Z),Y
            rts
L25FB       ldx #$FC
            lda KEYDEF+1
            .byte $87
            bmi L25A7
            adc L2F86+1,Y
            ldx #$00
            ldy LF930
            beq L263B
L260C       lda LF932,X
            cmp ICAX6Z
            bne L2635
            lda LF933,X
            cmp STATUS
            bne L2635
L261A       lda L00E0
L261C       cmp LF934,X
            bcc L2635
L2621       lda LF931,X
            cmp FTYPE
            bne L2635
            ora #$80
            sta LF931,X
            jsr L25CB
            jsr L2668
            clc
            rts
L2635       txa
            .byte $CB,$FC
            dey
            bne L260C
L263B       sec
L263C       rts
L263D       ldx #$FF
L263F       inx
            cpx SOUNDR
            bcs L263C
            lda VTIMR1,X
            bit LE1C5
            beq L263F
            ora #$40
            sta ICAX3Z
            and #$C0
L2652       ldy VKEYBD,X
            sty ICAX6Z
            ldy VDSLST,X
            sty STATUS
            jsr L26AD
            lda VTIMR1,X
            cmp ICAX3Z
            bne L2652
            beq L263F
L2668       lda KEYDEF+1
            and #$FC
            sta STATUS
            lda KEYDEF
            and #$FC
            sta ICAX6Z
            ldx SOUNDR
            beq L268E
            dex
L2679       cmp VKEYBD,X
            bne L2685
            ldy VDSLST,X
            cpy STATUS
            beq L26A6
L2685       dex
            bpl L2679
            ldx SOUNDR
            cpx #$07
            bcs L26A1
L268E       inc SOUNDR
            sta VKEYBD,X
            eor KEYDEF
            tay
            lda STATUS
            sta VDSLST,X
            lda L26A2,Y
            sta VTIMR1,X
L26A1       rts
L26A2       rti
            cpy #$C0
            rti
L26A6       lda VTIMR1,X
            asl
            bmi L26A1
            ror
L26AD       ora #$40
            sta VTIMR1,X
            bmi L26C9
            cmp #$44
            bcs L26A1
            and #$03
            adc STATUS
            sta STATUS
            ldy #$01
            lda (ICAX6Z),Y
            and #$7F
            sta (ICAX6Z),Y
            iny
            bpl L26DA
L26C9       cmp #$C4
            bcs L26A1
            inc STATUS
            and #$03
            tay
            lda (ICAX6Z),Y
            and #$7F
            sta (ICAX6Z),Y
            inc STATUS
L26DA       lda (ICAX6Z),Y
            and #$7F
            sta (ICAX6Z),Y
            inc VTIMR1,X
            rts
L26E4       ldy SOUNDR
            beq L2752
            ldx #$00
            ldy #$00
L26EC       sty ICAX3Z
            lda VTIMR1,X
            asl
            bmi L272F
            and #$0E
            beq L274B
            dec VTIMR1,X
            lda VKEYBD,X
            sta ICAX6Z
            lda VTIMR1,X
            and #$07
            bcs L2717
            adc VDSLST,X
L270A       sta STATUS
            ldy #$01
L270E       lda (ICAX6Z),Y
            ora #$80
            sta (ICAX6Z),Y
            iny
            bpl L2727
L2717       tay
            lda VDSLST,X
L271B       adc #$00
L271D       sta STATUS
            lda (ICAX6Z),Y
            ora #$80
            sta (ICAX6Z),Y
            inc STATUS
L2727       lda (ICAX6Z),Y
            ora #$80
            sta (ICAX6Z),Y
            ldy ICAX3Z
L272F       lda VTIMR1,X
            and #$BF
            sta VTIMR1,X
            cpx ICAX3Z
            beq L274A
            sta VTIMR1,Y
            lda VDSLST,X
            sta VDSLST,Y
            lda VKEYBD,X
            sta VKEYBD,Y
L274A       iny
L274B       inx
            cpx SOUNDR
            bcc L26EC
            sty SOUNDR
L2752       rts
L2753       lda LE8F2
            lsr
            lsr
            lsr
            lsr
            lsr
L275B       sta ICBLHZ
            lda FTYPE
            asl
            tay
            ldx LEA24,Y
            lda LEA25,Y
            jsr LE095
            lda #$80
            jsr LE0A2
            beq L2776
            ldx #$00
            jsr L27F9
L2776       lda #$20
            jsr LE0A2
            sta LE8F3
L277E       lda #$08
            jsr LE0A2
            beq L27F8
            sta ICAX1Z
            dec ICBLHZ
            bmi L279C
L278B       jsr LE0A0
            jsr LE0A0
            lda #$20
            jsr LE0A2
            dec ICAX1Z
            bne L278B
            beq L277E
L279C       sta ZLOADA+1
            lda #$20
L27A1       lsr
            cmp ICAX1Z
            bcc L27A8
            bne L27A1
L27A8       sta L2824+1
            ldx #$01
L27AD       jsr L27F9
            jsr LE0A0
            sta ZLOADA+1,X
            lda #$80
            jsr LE0A2
            sta L4C20,X
L27BE       ldy #$00
            lda (BUFRHI),Y
            and #$FC
            cmp #$10
            beq L27D8
            lda (BUFRHI),Y
            .byte $80
L27CB       iny
            cmp LE906,Y
            bne L27CB
            tya
            ora #$10
            ldy #$00
            sta (BUFRHI),Y
L27D8       inx
            dec ICAX1Z
            bne L27AD
L27DD       cpx #$20
            bcs L27F4
            lda #$FF
            sta TXTMSC,X
            sta L4C00,X
            sta LOGMAP+2,X
            lda #$00
            sta L4C20,X
            inx
            bne L27DD
L27F4       jsr L2A8F
            clc
L27F8       rts
L27F9       lda #$04
            jsr LE0A2
            rol
            asl
            sta TXTMSC,X
            sta BUFRHI
            lda #$12
            jsr LE0A2
            eor TXTMSC,X
            sta LOGMAP+2,X
            eor TXTMSC,X
            rol
            asl
            sta L4C00,X
            sta BFENLO
            rts
L281B       lda KEYDEF+1
            lsr
            lsr
            eor KEYDEF
            sta L2828+1
L2824       ldy #$10
            sty CASINI
L2828       lda #$00
            cmp LOGMAP+2,Y
            bcs L283A
L282F       lsr CASINI
            bcs L2850
            sec
            tya
            sbc CASINI
            tay
            bcs L2828
L283A       beq L2846
L283C       lsr CASINI
            bcs L2850
            tya
            adc CASINI
            tay
            bcc L2828
L2846       lda KEYDEF
            cmp TXTMSC,Y
            bcc L282F
            bne L283C
            clc
L2850       rts
L2851       lda L00E0,X
            tax
            lda LE256,X
            sta BITMSK
            eor #$FF
            adc #$01
            sta BUFSTR
            tya
            asl
            asl
            asl
            sta ICAX1Z
            lda ZLOADA+1,Y
            sta L2882+1
            jsr L2A01
            tax
            beq L287E
            lda ICCOMZ
            and #$C0
            sta ICBLHZ
            lda LD419+2
            cmp ICBLHZ
            bcc L28EB
L287E       ldy #$02
            sty ICBLHZ
L2882       lda #$00
            ldy #$08
L2886       dey
            asl
            bcc L2886
            pha
            ldx LE1D8,Y
            lda BUFSTR,X
            ldx ICBLHZ
            sta SHFAMT,X
            ldx LE1D6,Y
            lda BUFSTR,X
            ldx ICBLHZ
            sta COLAC,X
            sty L28A9+1
            tya
            ora ICAX1Z
            tay
            lda L4500,Y
            sta ENDPT+1,X
L28A9       ldy #$08
            dex
            bmi L28BB
            stx ICBLHZ
            pla
            bne L2886
L28B3       sta SHFAMT,X
            sta COLAC,X
            dex
            bpl L28B3
            .byte $80
L28BB       pla
            ldy #$00
            lda LD419+2
            cmp #$AA
            bcs L28CB
            iny
            cmp #$55
            bcs L28CB
            iny
L28CB       ldx ICAX2Z
            lda SHFAMT,Y
            sta L0120,X
            lda COLAC,Y
            sta L0130,X
            ora L0120,X
            beq L28E5
            lda ENDPT+1,Y
            sta L53E8,X
            rts
L28E5       lda #$08
            sta L01A0,X
            rts
L28EB       txa
            bpl L28F5
            jsr L296E
            eor #$04
            bpl L28F8
L28F5       jsr L296E
L28F8       tax
            ldy L2882+1
            lda LD419+2
            cmp #$E0
            bcs L2909
            tya
            and LCF93,X
            bne L2955
L2909       tya
            and LCF92,X
            bne L2947
            tya
            and LCF94,X
            bne L2944
            lda LD419+2
            cmp #$E0
            bcs L2928
            tya
            and LCF91,X
            bne L2938
            tya
            and LCF95,X
            bne L2943
L2928       ldx ICAX2Z
            lda #$08
            sta L01A0,X
            lda #$00
            sta L0120,X
            sta L0130,X
            rts
L2938       tya
            and LCF95,X
            beq L2953
            lda LD419+2
            bpl L2953
L2943       inx
L2944       inx
            bpl L2955
L2947       tya
            and LCF94,X
            beq L2954
            lda LD419+2
            bmi L2944
            .byte $80
L2953       dex
L2954       dex
L2955       txa
            and #$07
            tay
            ldx LE1D8,Y
            lda BUFSTR,X
            ldx ICAX2Z
            sta L0120,X
            ldx LE1D6,Y
            lda BUFSTR,X
            ldx ICAX2Z
            sta L0130,X
            rts
L296E       ldx #$03
            sec
            lda ICCOMT
            sbc KEYDEF
            bcs L297D
            ldx #$01
            eor #$FF
            adc #$01
L297D       sta CHKSUM
            lsr
            tay
            sec
            lda DSKFMS
            sbc KEYDEF+1
            bcs L298D
            dex
            eor #$FF
            adc #$01
L298D       sta BUFRLO
            lsr
            cmp CHKSUM
            bcc L2997
            txa
            .byte $CB
            sed
L2997       cpy BUFRLO
            bcc L299E
            txa
            .byte $CB,$FC
L299E       lda L29A2,X
            rts
L29A2       .byte $00
            asl CASINI
            .byte $04,$07,$07,$03,$03
            ora (RAMLO+1,X)
            ora (RAMLO+1,X)
L29AE       ldx ZLOADA+1
            lda LE1E3,X
            sta ICAX1Z
            lda LD419+2
L29B9       clc
            sbc ICAX1Z
            bcc L29C4
            dex
            bne L29B9
L29C1       ldx ICAX2Z
            rts
L29C4       jsr L16F9
            jsr L2430
            bcc L29D1
            dex
            bne L29C4
            beq L29C1
L29D1       ldx ICAX2Z
            lda CRITIC
            sta L0160,X
            lda FMSZPG
            sta L0170,X
            lda FMSZPG+1
            sta L0180,X
            lda FMSZPG+2
            sta L0190,X
            lda #$80
            sta L47B0,X
            asl
            sta L0120,X
            sta L0130,X
            jsr L1518
            lda LD419+2
            and #$0F
            adc #$04
            sta L01A0,X
            rts
L2A01       ldx ICAX2Z
            lda L00E0,X
            cmp #$11
            bcc L2A0D
            cmp #$13
            bne L2A1D
L2A0D       ldy L0110,X
            bmi L2A1A
            lda LCF93,Y
            and LE644
            bne L2A1D
L2A1A       lda #$00
            rts
L2A1D       lda L00F0,X
            lsr
            lsr
            lsr
            lsr
            adc #$01
            sta ICBLHZ
            sec
            lda L0100
            ldy L00E0,X
            sbc LE226,Y
            bcc L2A36
            cmp ICBLHZ
            bcs L2A3D
L2A36       cpy #$0E
            lda #$00
            adc #$00
            rts
L2A3D       lda #$FF
            rts
L2A40       ldx #$03
L2A42       lda #$0F
            sta BUFRLO
            lda LE906,X
            asl
            rol BUFRLO
            asl
            rol BUFRLO
            asl
            rol BUFRLO
            sta CHKSUM
            txa
            ora #$10
            asl
            asl
            asl
            sta L2A61+1
            ldy #$07
L2A5F       lda (CHKSUM),Y
L2A61       sta L7800,Y
            dey
            bpl L2A5F
            ldy LE906,X
            lda LC100,Y
            sta LC110,X
            dex
            bpl L2A42
            rts
L2A74       ldx #$1F
L2A76       lda L77C0,X
            sta L7880,X
            dex
            bpl L2A76
            lda LDBF2
            and #$0F
            ora #$A0
            ldx #$03
L2A88       sta LC110,X
            dex
            bpl L2A88
L2A8E       rts
L2A8F       ldx #$00
            stx ICAX2Z
            txa
L2A94       sta L4500,X
            inx
            bne L2A94
L2A9A       cpx ZLOADA+1
            bcs L2A8E
            inx
            lda ZLOADA+1,X
            sta ICAX1Z
            lda #$FF
            sta ICBLHZ
L2AA9       inc ICBLHZ
            lsr ICAX1Z
            bcc L2B10
            txa
L2AB0       asl
            asl
            asl
            ora ICBLHZ
            tay
            lda L4500,Y
            bne L2B10
            inc ICAX2Z
            lda ICAX2Z
            sta L4500,Y
            lda TXTMSC,X
            sta L00C4
            lda L4C00,X
            sta L00C5
            ldy ICBLHZ
            lda LE1D8,Y
            sbc #$00
            asl
            asl
            sta L00C2
            sec
            lda LE1D6,Y
            sbc #$01
            asl
            asl
            sta L00C3
L2AE1       clc
            lda L00C4
            adc L00C2
            sta L00C4
            clc
            lda L00C5
            adc L00C3
            sta L00C5
            ldy ZLOADA+1
            .byte $80
L2AF3       dey
            beq L2AE1
            lda L00C4
            cmp TXTMSC,Y
            bne L2AF3
            lda L00C5
            cmp L4C00,Y
            bne L2AF3
            tya
            rol
            asl
            asl
            eor ICBLHZ
            tay
            lda ICAX2Z
            sta L4500,Y
L2B10       bne L2AA9
            beq L2A9A
L2B14       jsr L0960
            lda #$00
            sta OLDADR
            sta BRKKEY
            sta RTCLOK+1
            jsr L2415
            jsr L30C4
            lda #$40
            sta L00A2
            lda FTYPE
            sta FEOF
            jsr L2C08
            bcs L2B8A
            ldx #$AE
            jsr L318C
            lda #$16
            sta ATACHR
            jsr L0929
L2B3F       jsr L4071
            lda ICBAHZ
            beq L2B52
L2B46       jsr L2B9F
            ldy ICAX1Z
            bmi L2B56
            lda LCFC0,Y
            bne L2B46
L2B52       bit ICPTHZ
            bpl L2B3F
L2B56       ldy ADRESS+1
            lda FTYPE
            cmp FEOF
            beq L2B87
            sec
            lda LEAA4,Y
            sbc #$22
            asl
            sec
            rol
            asl
            asl
            asl
            sta ABUFPT+1
            lda #$00
            sta ABUFPT
            rol
            sta ABUFPT+2
            sec
            lda LEA84,Y
            sbc #$05
            sec
            rol
            asl
            asl
            rol ABUFPT
            asl
            rol ABUFPT
            asl
            rol ABUFPT
            sta DSKUTL+1
L2B87       jsr L0929
L2B8A       jsr L1ECB
            jsr L0AD6
            lda #$C0
            ora FREQ
            sta L1D9D+1
            lda #$33
            sta L1E2C+1
            jmp L09A1
L2B9F       ldx #$00
            stx ICAX1Z
            clc
            adc ADRESS+1
            tay
            ldx LEAE4,Y
            bmi L2C07
            cpx MLTTMP
            beq L2BCB
            lda L00A1
            lsr
            bcc L2C07
            sty ADRESS+1
            txa
            pha
            lda #$0E
            jsr L2C40
            pla
            sta MLTTMP
            lda #$09
            jsr L2C40
            ldy ADRESS+1
            ldx LEAE4,Y
L2BCB       sty ADRESS+1
            txa
            ldx #$00
            cmp LEAE3,Y
            bne L2BD6
            inx
L2BD6       cmp LEAE5,Y
            bne L2BDD
            inx
            inx
L2BDD       stx ICAX1Z
            lda ICAX1Z
            pha
            lda LEAC4,Y
            and #$0F
            pha
            eor LEAC4,Y
            sta LE8F2
            lda #$10
            sta ATACHR
            jsr L2C6A
            pla
L2BF7       sta FTYPE
            jsr L2C6A
            lda #$10
            sta CHBAS
            pla
            sta ICAX1Z
            jsr L0A4F
L2C07       rts
L2C08       lda ICCOMT
            lsr
            lsr
            sta BUFRHI
            lda DSKFMS
            lsr
            lsr
            sta BFENLO
            ldy #$1F
L2C16       dey
            bmi L2C3E
            lda LEA84,Y
            cmp BUFRHI
            bne L2C16
            lda LEAA4,Y
            cmp BFENLO
            bne L2C16
            lda LEAC4,Y
            and #$0F
            cmp FTYPE
            bne L2C16
            sty ADRESS+1
            lda LEAE4,Y
            sta MLTTMP
            lda #$09
            jsr L2C40
            clc
            rts
L2C3E       sec
            rts
L2C40       pha
            ldy MLTTMP
            ldx LEB4C,Y
            sec
            lda LCF7B,X
            adc LEB44,Y
            sta BUFRHI
            lda LCF6A,X
            adc #$00
            sta BFENLO
            ldx LEB54,Y
            ldy #$00
L2C5B       jsr L085E
            pla
            sta (BFENHI),Y
            pha
            jsr L3AFE
            dex
            bne L2C5B
            pla
            rts
L2C6A       ldx FTYPE
            ldy L47D0,X
            ldx #$0F
            lda #$00
L2C73       ora LC000,Y
            iny
            dex
            bpl L2C73
            ldy LC190
            tax
            bne L2C8B
            tya
            ldy #$0E
            and #$0F
            cmp #$0E
            bne L2C8B
            ldy #$08
L2C8B       sty ICAX1Z
            ldy FTYPE
            ldx LEB04,Y
            sec
            lda LCF7B,X
            adc LEB14,Y
            sta BUFRHI
            lda LCF6A,X
            adc #$00
            sta BFENLO
            jsr L085E
            lda LEB34,Y
            sta ICAX3Z
            ldx LEB24,Y
            dex
            beq L2CD8
L2CB0       jsr L085E
            ldy #$00
L2CB5       lda (BUFRHI),Y
            bpl L2CCC
            and #$EF
            cmp #$8D
            bcs L2CCC
            jsr L2CF5
            ora #$70
            adc #$02
L2CC6       adc #$03
            bcc L2CC6
            beq L2CD1
L2CCC       iny
            cpy ICAX3Z
            bcc L2CB5
L2CD1       jsr L3AFE
            dex
            bpl L2CB0
            rts
L2CD8       ldy #$00
L2CDA       lda (BUFRHI),Y
            bpl L2CEF
            cmp #$A0
            bcs L2CEF
            and #$0F
            cmp #$03
            bcc L2CEC
            cmp #$0C
            bne L2CEF
L2CEC       jsr L2CF5
L2CEF       iny
            cpy ICAX3Z
            bcc L2CDA
            rts
L2CF5       lda ICAX1Z
            sta (BFENHI),Y
            lda (BUFRHI),Y
            eor #$10
            sta (BUFRHI),Y
            rts
L2D00       lda TEMPCHR
            sta SAVMSC
            jsr L0960
            ldy L00E0
            lda #$00
            sta DOSINI+1
            sta BUFADR+1
            jsr L2422
            lda #$33
            ldx #$F8
            jsr L0A32
            cpy LMARGN
            beq L2D1F
            ldy #$FF
L2D1F       iny
            sty LMARGN
            lda #$C0
            sta L1D9D+1
            lda #$16
            sta ATACHR
            lda #$C0
            sta ICHIDZ
            jsr L1ECB
            jsr L0BF9
            cpy #$06
            bne L2D3C
            ror LMARGN
L2D3C       jsr L2A74
L2D3F       inc TEMPCHR
            jsr L1ECB
            jsr L4071
            jsr L3FA5
            jsr L0B3C
            lda #$C0
            sta L00A3
            lda ICPTHZ
            and #$80
            ora ICBAHZ
            ora ICPTLZ
            beq L2D63
            lda BUFADR+1
            bne L2D68
            inc BUFADR+1
            bne L2D65
L2D63       sta BUFADR+1
L2D65       jsr L2D7A
L2D68       lda ICHIDZ
            bne L2D3F
            lda SAVMSC
            sta TEMPCHR
            lda #$C0
            ora FREQ
            sta L1D9D+1
            jmp L0929
L2D7A       lda ICHIDZ
            asl
            bmi L2DA5
            bcs L2DA3
            beq L2DA2
            cmp #$0A
            bcs L2DA2
            bit ICPTHZ
            bmi L2D95
            tay
            lda LE8F3,Y
            pha
            lda LE8F2,Y
            pha
            rts
L2D95       lsr
            ora #$C0
            sta ICHIDZ
            jsr L2415
            lda #$16
            sta CHBAS
L2DA2       rts
L2DA3       bcs L2DFC
L2DA5       ror
            and #$BF
            sta ICHIDZ
            jsr L2415
            lda #$33
            ldx #$F8
            jsr L0A32
            lda #$F7
            jsr L0939
            jsr L23E6
            lda L00E0
            sta DOSINI+1
            ldx #$BA
            ldy #$E1
            jsr L20E5
            ldx #$02
L2DC9       inc RAMLO+1
            lda LE1C4,X
            sta TRAMSZ
            lda LE1C8,X
            sta WARMST
            lda LE1CC,X
            sta DOSINI
            jsr L20F3
            inx
L2DDE       cpx #$06
            bcc L2DC9
            jsr L2E3E
            jsr L2E32
            lda #$3E
            jsr L2424
            lda #$18
            sta GRACTL
            lda #$4D
            sta L1E2C+1
            ldx #$B3
            jmp L318C
L2DFC       ror
            ldy ICBAHZ
            beq L2E1B
            bmi L2E0B
            cmp #$84
            bcs L2E1B
            inc ICHIDZ
            bcc L2E11
L2E0B       cmp #$81
            bcc L2E1B
            dec ICHIDZ
L2E11       ldx #$FF
            jsr L2E36
L2E16       lda #$15
            sta CHBAS
L2E1B       jsr L2E32
            lda #$00
            sta OLDADR
            bit ICPTHZ
            bpl L2E31
            lda ICHIDZ
            and #$0F
            sta ICHIDZ
            lda #$16
            sta CHBAS
L2E31       rts
L2E32       ldx #$F1
            lda ICHIDZ
L2E36       and #$07
            tay
            txa
            sta LE657,Y
            rts
L2E3E       jsr L09DC
            ldx #$BC
            ldy #$E3
            jsr L3230
            inc SDLSTL
            sec
            lda BPTR
            sbc #$01
            and #$07
            adc #$68
            jsr L3256
            lda OLDCOL+1
            ldx #$14
            ldy #$12
            jsr L2E97
            ldx #$C6
            ldy #$E3
            jsr L3230
            clc
            lda FTYPE
            adc #$7A
            jsr L3256
            lda #$0F
            tax
L2E72       ldy L00E0,X
            cpy #$20
            sbc #$00
            dex
            bne L2E72
            eor #$0F
            ldx #$14
            ldy #$17
            jsr L2E97
            ldx #$B2
            ldy #$E3
            jsr L3230
            lda ICCOMZ
            and #$C0
            asl
            rol
            rol
            adc #$D0
            jmp L3256
L2E97       cmp #$01
            php
            stx L0096
            sty L0095
            jsr L0C0A
            sta LE505
            txa
            bne L2EAE
            ldx #$30
            tya
            bne L2EAE
            ldy #$30
L2EAE       stx LE503
            sty LE504
            ldx #$03
            ldy #$E5
            jsr L31F9
            lda #$0B
            jsr L3256
            plp
            beq L2EC8
            lda #$1C
            jsr L32BE
L2EC8       rts
            lda OLDADR
            asl
            tay
            lda LE8FD,Y
            pha
            lda LE8FC,Y
            pha
            rts
            jsr L2422
            lda #$00
            sta GRACTL
            jsr L0A2E
            lda #$F2
            jsr L0939
            ldx #$40
            ldy #$D9
            stx BUFRHI
            sty BFENLO
            ldx #$04
            lda #$F3
            jsr L0BE1
            lda #$0C
            sta ZCHAIN
            sta ZCHAIN+1
            lda LE644
            beq L2F0C
            lda ICBAHZ
            beq L2F1D
            jsr L2F74
            lda #$00
            sta LE644
L2F0C       jsr L2363
            lda #$0D
            sta L2FA0+1
            jsr L09DC
            jsr L2412
            jsr L1ECB
L2F1D       ldx #$E0
            ldy #$51
            stx BUFRHI
            sty BFENLO
            ldx #$0D
            lda #$30
            jsr L0BE1
            lda ZCHAIN+1
            sta L0095
            lda ZCHAIN
            sta L0096
            jsr L2F92
            rts
            lda ICBAHZ
            bne L2F50
            lda ICPTLZ
            beq L2F47
            lda #$15
            sta CHBAS
            dec OLDADR
L2F47       rts
            lda ICPTLZ
            bne L2F5A
            lda ICBAHZ
            beq L2F73
L2F50       jsr L2F74
            lda #$00
            sta LE644
            beq L2F6F
L2F5A       lda #$15
            sta CHBAS
            sta LE19F
            jsr L2363
            lda #$00
            sta LE19F
            lda #$0D
            sta L2FA0+1
L2F6F       lda #$01
            sta OLDADR
L2F73       rts
L2F74       clc
            bmi L2F84
            clc
            adc DOSINI+1
            cmp L00E0
            beq L2F8A
            bcc L2F8A
            lda #$00
            beq L2F8A
L2F84       adc DOSINI+1
L2F86       bpl L2F8A
            lda L00E0
L2F8A       sta DOSINI+1
            lda #$15
            sta CHBAS
            rts
L2F92       lda #$09
            sta L0096
            inc SDLSTL
            lda L2FA0+1
            cmp #$17
            beq L2FD6
L2FA0       lda #$0D
            jsr L3256
            ldx L0096
            cpx #$11
            bcs L2FAE
            jsr L32BC
L2FAE       lda #$18
            jsr L3256
            dec L0096
            jsr L312C
            inc L2FA0+1
            lda L2FA0+1
            cmp #$17
            bcs L2FD1
            lda #$10
            sta L2FF1+1
            lda L0095
            adc #$02
            sta L0095
            cmp #$18
            bcc L2F92
L2FD1       lda #$03
            sta OLDADR
            rts
L2FD6       ldy L2FF1+1
            cpy #$10
            bne L2FE8
            jsr L3256
            lda #$18
            jsr L3256
            jmp L2FF1
L2FE8       lda #$0B
            sta L0096
            lda #$00
            sta SDLSTL
L2FF1       ldy #$10
            lda (OLDADR+1),Y
            cmp #$FE
            beq L300C
            bcs L3002
            jsr L3256
            bmi L3014
            bpl L2FD1
L3002       lda #$26
            jsr L32BE
L3007       lda #$04
L3009       sta OLDADR
            rts
L300C       lda #$26
L300E       jsr L32BE
L3011       inc SDLSTL
L3014       inc L2FF1+1
L3017       bne L2FF1
L3019       .byte $00
L301A       lda OLDADR
L301C       beq L302E
            lda ICPTLZ
            beq L302D
L3022       clc
            adc L3019
L3026       cmp #$1A
            bcs L302D
            pha
            bcc L3050
L302D       rts
L302E       jsr L2422
            ldx #$33
            stx L1E2C+1
            ldx L025C
            stx L0090
            ldx LDBE7+1
            stx L0090
            lda #$00
            ldx #$27
L3044       sta L5140,X
            dex
            bpl L3044
            jsr L0BFC
            lda #$03
            pha
L3050       lda FTYPE
L3052       jsr LE000
            lda ICCOMT
            sta BUFRHI
            lda DSKFMS
            .byte $4B,$3F
            lsr
            lsr
            ror BUFRHI
            lsr
            ror BUFRHI
            ora #$4C
            sta BFENLO
            ldy #$00
            lda #$2C
            sta (BUFRHI),Y
            pla
            sta L3019
            sta CHKSUM
            lda #$4C
            sta BUFRLO
            lda LC12C
            pha
            lda #$01
            sta LC12C
            ldx #$68
            ldy #$51
            stx BUFRHI
            sty BFENLO
            jsr L085E
            lda #$0F
            sta OLDADR
L3090       lda RMARGN
            bne L3090
L3094       ldy #$27
L3096       .byte $B3
            and (L0091),Y
            .byte $33
            lda LC100,X
            sta (BFENHI),Y
            dey
            bpl L3096
            lda #$40
            clc
            adc CHKSUM
            sta CHKSUM
            bcc L30AD
            inc BUFRLO
L30AD       jsr L3AFE
            jsr L085E
            dec OLDADR
            bpl L3094
            pla
            sta LC12C
            jsr L0AC3
            ldx #$4F
            stx L1E2C+1
            rts
L30C4       lda OLDADR
            bne L30F8
            jsr L2422
            lda #$33
            sta L1E2C+1
            lda #$FE
            sta L0090
            lda #$F1
            sta LD021+1
            lda #$F0
            sta LD021+2
            lda #$D0
            sta L1D9D+1
            jsr L0BFC
            lda #$11
            jsr LE000
L30EB       jsr L30F9
            jsr L2C6A
L30F1       lda #$4F
            sta L1E2C+1
            inc OLDADR
L30F8       rts
L30F9       ldx #$40
            ldy #$51
            stx CHKSUM
L30FF       sty BUFRLO
            ldy #$D9
            stx BFENHI
            sty LTEMP
            lda #$11
            sta ICBLHZ
L310B       ldy #$27
L310D       lda (CHKSUM),Y
            tax
            lda LC100,X
            sta (BFENHI),Y
            dey
            bpl L310D
            lda #$28
            clc
            adc CHKSUM
            sta CHKSUM
            sta BFENHI
L3121       bcc L3127
            inc BUFRLO
            inc LTEMP
L3127       dec ICBLHZ
            bne L310B
            rts
L312C       lda L2FA0+1
            sec
            sbc #$07
            tay
            cpy #$06
            beq L3149
            cpy #$07
            beq L315D
            cpy #$08
            beq L3165
            cpy #$09
            beq L3177
            lda (OLDADR+1),Y
            jsr L3256
            rts
L3149       jsr L32BC
            lda DOSINI+1
            clc
            adc #$01
            jsr L0C0A
            pha
            tya
            jsr L32BE
            pla
            jmp L32BE
L315D       ldx DOSINI+1
            lda LE226,X
            jmp L3256
L3165       lda (OLDADR+1),Y
            jsr L0C0A
            sty LE4F7
            sta LE4F8
            ldx #$F5
            ldy #$E4
            jmp L31F9
L3177       lda (OLDADR+1),Y
            jsr L0C0A
            stx LE4FC
            sty LE4FD
            sta LE4FE
            ldx #$FC
            ldy #$E4
            jmp L31F9
L318C       ldy #$00
L318E       lda LE2E6,X
            bpl L3198
            cpy #$0D
            bcc L3199
            rts
L3198       inx
L3199       sta L5052,Y
            ora #$80
            sta L507A,Y
            iny
            cmp #$BA
            bcc L318E
            cmp #$DA
            bcs L318E
            adc #$A0
            bcs L3199
L31AE       stx CHKSUM
            sty BUFRLO
            lda #$00
            sta SWPFLG
            lda #$09
            sta L0096
            sta ZCHAIN
            lda #$0C
            sta L0095
            sta ZCHAIN+1
L31C2       lda #$01
            sta SDLSTL
L31C7       ldy SWPFLG
            inc SWPFLG
            lda (CHKSUM),Y
            cmp #$FE
            bcs L31DC
            pha
            jsr L3256
            pla
            cmp #$64
            bne L31C7
            beq L31C2
L31DC       bne L31F8
            lda #$26
            jsr L32BE
            jmp L31C2
L31E6       lda #$96
L31E8       sta SWPFLG
            jsr L0929
L31ED       jsr L4071
            bit ICPTHZ
            bmi L31F8
            dec SWPFLG
            bne L31ED
L31F8       rts
L31F9       stx L0098
            sty L0099
            jsr L32BC
            ldy #$00
            ldx L0096
            lda (L0098),Y
            and #$7F
            bpl L320E
L320A       lda (L0098),Y
            bmi L321B
L320E       cmp #$3A
            bcc L3217
            cmp #$5A
            bcs L3217
            inx
L3217       inx
L3218       iny
            bpl L320A
L321B       ldy #$00
            cpx #$27
            bcc L3241
            lda ZCHAIN
            sta L0096
            lda L0095
            adc #$01
            sta L0095
            cmp #$18
            bcc L3241
            rts
L3230       stx L0098
            sty L0099
            ldy #$00
            lda (L0098),Y
            sta L0095
            iny
            lda (L0098),Y
            sta L0096
            iny
            .byte $0C
L3241       ldy #$00
            lda (L0098),Y
            and #$7F
L3247       sty L009A
            jsr L32BE
            ldy L009A
            iny
L324F       bmi L324F
            lda (L0098),Y
            bpl L3247
            rts
L3256       tay
            ldx #$C4
            stx L325F+2
            ldx #$00
            iny
L325F       lda LFF00,X
            bmi L326C
L3264       inx
            bne L325F
            inc L325F+2
            bne L325F
L326C       dey
            bne L3264
            lda L325F+2
            stx L0098
            sta L0099
            lda (L0098),Y
            sta L32B6+1
            ldx SDLSTL
            beq L32A9
            and #$7F
            cmp #$54
            beq L329D
            cmp #$42
            beq L32A0
            cmp #$12
            beq L32A3
            cmp #$16
            beq L32A9
            cmp #$0A
            bcc L32A9
            cmp #$24
            bcs L32A9
            adc #$30
            .byte $0C
L329D       lda #$50
            .byte $0C
L32A0       lda #$46
            .byte $0C
L32A3       lda #$16
            ora #$80
            sta (L0098),Y
L32A9       sty SDLSTL
            ldx L0098
            ldy L0099
            jsr L31F9
            php
            ldy #$00
L32B6       lda #$00
            sta (L0098),Y
            plp
            rts
L32BC       lda #$30
L32BE       sta L0097
            ldy L0095
            clc
            lda LCF71,Y
            adc L0096
            sta L009B
            lda LCF60,Y
            adc #$00
            sta L009C
            lda L0097
            ldy #$00
            sta (L009B),Y
            ora #$80
            ldy #$28
            sta (L009B),Y
            inc L0096
            cmp #$BA
            bcc L32F5
            cmp #$DA
            bcs L32F5
            ldy #$01
            adc #$A0
            sta (L009B),Y
            ora #$80
            ldy #$29
            sta (L009B),Y
            inc L0096
L32F5       rts
L32F6       jsr L0960
            ldx #$6A
            jsr L318C
            jsr L3914
            jsr L0929
            lda L00A1
            and #$08
            bne L3357
L330A       lda #$09
            sta ATACHR
            jsr L33D5
            lda #$1B
            sta CHBAS
            jsr L3873
            lda L008E
            cmp #$F8
            bne L3337
            ldx #$89
            jsr L318C
            lda #$0E
            sta CHBAS
            lda #$40
            sta SWPFLG
L332E       jsr L3B08
            dec SWPFLG
            bne L332E
            beq L330A
L3337       cmp L0089
            beq L3357
            ldx L00E0
            beq L3378
            ldy L0100
            lda LE2D4,Y
            jsr L4135
            lda #$00
            sta L00E0
            sta CDTMV5
            sta DOSINI+1
L3351       ldx #$9B
            lda #$0C
            bne L3382
L3357       ldx ICDNOZ
            ldy L00F0,X
            sty L00F0
            ldy L00E0,X
            sty DOSINI+1
            sty L00E0
            sty CDTMV5
            ldx LE226,Y
            lda LE2CA,X
            jsr L4105
            jsr L4174
            ldx #$92
            lda #$0B
            bne L3382
L3378       stx L00F0
            inx
            stx L025A
            ldx #$A4
            lda #$0D
L3382       sta CHBAS
            jsr L318C
            lda #$28
            sta SWPFLG
            lda #$01
            jsr L2424
            jsr L38F7
            lda L00A1
            and #$08
            bne L33A1
L339A       jsr L3B08
            dec SWPFLG
            bne L339A
L33A1       jsr L1ECB
            jsr L09A1
            ldx ICDNOZ
            ldy L0110,X
            lda #$00
            sta ICDNOZ
            sta ABUFPT+3
            sta L00F0,X
            lda #$FF
            sta L0110,X
            sta L00E0,X
            jsr L2347
            jsr L1208
            jsr L1845
            jsr L0960
            ldx #$02
            stx L0090
            dex
            stx LD026+1
            stx L3BDF
            jmp L2422
L33D5       jsr L2422
            lda #$33
            sta L1E2C+1
            lda #$D0
            sta L1D9D+1
            lda #$F2
            sta L0090
            lda #$F0
            sta LD021+2
            jsr L0BFC
            jsr L0AC3
            jsr L23E6
            ldx #$0A
            ldy #$E9
            jsr LE018
            ldx #$68
            ldy #$D9
            stx BUFRHI
            sty BFENLO
            lda #$F8
            ldx #$10
            jsr L0BE1
            lda #$FF
            sta COUNTR
            lda #$FC
            sta L0084
            ldx #$68
            ldy #$D9
            stx BUFRHI
            sty BFENLO
            ldx #$10
L341C       txa
            lsr
            lda COUNTR
            ldy #$03
            sta (BUFRHI),Y
            ldy #$12
            sta (BUFRHI),Y
            lda L0084
            ldy #$15
            sta (BUFRHI),Y
            ldy #$24
            sta (BUFRHI),Y
            bcc L3436
            lda COUNTR
L3436       ldy #$13
            sta (BUFRHI),Y
            iny
            sta (BUFRHI),Y
            jsr L3AFE
            dex
            bne L341C
            ldx #$EA
            ldy #$51
            stx BUFRHI
            sty BFENLO
            lda #$F6
            sta INSDAT
            lda #$F7
            sta L0083
            lda #$F4
            sta HOLDCH
            lda #$F5
            sta L0082
            lda #$01
            sta L0088
            lda #$0C
            sta ICAX1Z
L3463       jsr L085E
            ldy #$00
            jsr L3575
            ldy #$04
            jsr L3575
            jsr L37EE
            ldy #$13
            jsr L3575
            ldy #$0F
            jsr L3575
            jsr L37EE
            jsr L3AFE
            dec ICAX1Z
            bne L3463
            jsr L1ECB
            lda #$05
            sta LOMEM+1
            lda #$22
            sta L0087
            lda #$00
            sta RAMLO+1
            jsr L20AD
            lda #$50
            ldx #$58
            ldy #$F1
            jsr L355F
            ldx ICDNOZ
            lda L0110,X
            sta RAMLO+1
            jsr L20AD
            ldx ICDNOZ
            clc
            lda #$50
            adc L0110,X
            ldx #$FF
            ldy #$00
            jsr L355F
            lda #$99
            sta L008D
            lda #$00
            sta COUNTR+1
            sta L0085
            sta L008F
            clc
            lda L0100
            adc #$03
            sta L008B
            ldy ICDNOZ
            ldx L00E0,Y
            lda L00F0,Y
            lsr
            lsr
            lsr
            lsr
            cmp #$03
            bcc L34E0
            lda #$02
L34E0       adc #$01
            adc LE226,X
            sta L008A
            lda COUNTR
            sta L0089
            lda #$58
            sta SWPFLG
            lda #$40
            sta L00A3
            jsr L2427
            lda #$4F
            sta L1E2C+1
L34FB       ldx L0089
            lda ICPTLZ
            beq L3515
            bmi L3509
            lda #$FF
            ldx L0084
            bne L350D
L3509       lda #$58
            ldx COUNTR
L350D       sta SWPFLG
            stx L0089
            lda #$40
            sta L00A3
L3515       stx LD9A1+2
            stx LD9A4
            stx LD9CB
            stx LD9CB+1
            stx L008E
            jsr L3A80
            bit ICPTHZ
            bpl L352E
            lda #$01
            sta L008D
L352E       ldx #$73
            jsr L3804
            lda #$00
            sta RAMLO+1
            jsr L20AD
            lda SWPFLG
            sta TRAMSZ
            jsr L20F3
            ldx ICDNOZ
            lda L0110,X
            sta RAMLO+1
            jsr L20AD
            lda SWPFLG
            eor #$A7
            sta TRAMSZ
            jsr L20F3
            jsr L1EC5
            jsr L4071
            lda L008D
            bne L34FB
            rts
L355F       sta DOSINI
            stx TRAMSZ
            sty DOSVEC+1
            ldx #$00
            stx DOSVEC
            stx CMCMD
            inx
            stx BOOT
            lda #$7A
            sta WARMST
            jmp L20F3
L3575       lda (BUFRHI),Y
            beq L3599
            lsr
            cmp #$7A
            bne L35AB
            lda LD419+2
            and #$1E
            cmp #$0A
            bcc L3589
            lda #$0A
L3589       tax
            lda L35AC,X
            sta L3596+1
            lda L35AC+1,X
            sta L3596+2
L3596       jmp L35AB
L3599       lda ICAX1Z
            cmp #$03
            bcc L35A8
            lda LD419+2
            and #$1E
            cmp #$06
            beq L3589
L35A8       jsr L3A01
L35AB       rts
L35AC       eor L6639,X
            and L3975,Y
            sta RECVDN
            clv
            and L3AD6,Y
L35B8       lda LOMEM
            beq L3628
            ldy COUNTR+1
            jsr L3789
            lda ICBAHZ
            jsr L3629
            ldy COUNTR+1
            lda ICBAHZ
            beq L35E0
            bmi L35D7
            iny
            cpy #$0D
            bcc L35DE
            ldy #$01
            bne L35DE
L35D7       dey
            bmi L35DC
            bne L35DE
L35DC       ldy #$0C
L35DE       sty COUNTR+1
L35E0       jsr L3789
            jsr L3634
            bit ICPTHZ
            bpl L3628
            jsr L3ADD
            lda (BUFRHI),Y
            cmp HOLDCH
            bne L3628
            jsr L085E
            jsr L3634
            jsr L3AF1
            jsr L085E
            jsr L3629
            sec
            lda #$0D
            sbc COUNTR+1
            tay
            lda #$B0
            sta (CHKSUM),Y
            lda #$00
            sta COUNTR+1
            dec LOMEM
            ldy LOMEM
            jsr L3789
            jsr L3AE8
            jsr L085E
            lda #$F2
            ldy LOMEM+1
            sta (BFENHI),Y
            lda #$0A
            sta CHBAS
L3628       rts
L3629       ldy LOMEM+1
L362B       lda HOLDCH
            sta (BUFRHI),Y
            lda #$F8
            sta (BFENHI),Y
            rts
L3634       ldy LOMEM+1
            lda INSDAT
            sta (BUFRHI),Y
            lda COUNTR
            sta (BFENHI),Y
            rts
L363F       ldx #$00
            ldy #$10
            lda #$06
            jsr L3671
            ldx #$10
            ldy #$20
            lda #$0A
            jsr L3671
            ldx #$20
            ldy #$30
            lda #$0E
            bne L3671
L3659       ldx #$70
            ldy #$60
            lda #$21
            jsr L3671
            ldx #$60
            ldy #$50
            lda #$1D
            jsr L3671
            ldx #$50
            ldy #$40
            lda #$19
L3671       sta ICAX3Z
            lda #$4C
            stx CHKSUM
            sta BUFRLO
            sty ICAX6Z
            sta STATUS
            ldx #$E0
            ldy #$51
            stx BUFRHI
            sty BFENLO
            ldy #$0C
L3687       sty ICAX1Z
            lda BUFRHI
            sta ICAX4Z
            lda BFENLO
            sta ICAX5Z
            lda (CHKSUM),Y
            beq L36CE
            bmi L36D6
            ldy ICAX3Z
            lda (BUFRHI),Y
            cmp INSDAT
            beq L36F2
            ldy ICAX1Z
            sec
            lda (CHKSUM),Y
            sbc #$01
            sta (CHKSUM),Y
            ldy ICAX3Z
            lda (BUFRHI),Y
            cmp L0083
            beq L36FE
            cmp #$FD
            beq L3706
            cmp #$FE
            bne L36BB
            jmp L3728
L36BB       cmp #$FF
            beq L36C5
            jsr L375E
            jsr L3750
L36C5       jsr L3AFE
            ldy ICAX1Z
            dey
            bne L3687
            rts
L36CE       ldy ICAX3Z
            jsr L3757
            jmp L36C5
L36D6       ldy ICAX3Z
            jsr L375E
            jsr L3750
            clc
            lda (CHKSUM),Y
            adc #$01
            sta (CHKSUM),Y
            bne L36C5
            ldy ICAX3Z
            jsr L085E
            jsr L362B
            jmp L36C5
L36F2       jsr L375E
            jsr L3750
            lda #$10
            sta (CHKSUM),Y
            bne L36C5
L36FE       ldy ICAX1Z
L3700       lda #$00
            sta (CHKSUM),Y
            beq L36C5
L3706       ldy ICAX1Z
            dey
            dey
            lda (CHKSUM),Y
            beq L36C5
            iny
            lda #$01
            sta (CHKSUM),Y
            jsr L3752
            jsr L3AFE
            ldy ICAX3Z
L371B       jsr L375E
            ldx ICAX4Z
            ldy ICAX5Z
            stx BUFRHI
            sty BFENLO
            bne L36C5
L3728       ldy ICAX1Z
            dey
            lda (CHKSUM),Y
            bne L3700
            jsr L3752
            lda #$01
            sta (CHKSUM),Y
            iny
            iny
            jsr L3752
            jsr L3AFE
            ldy ICAX3Z
            jsr L375E
            lda BUFRHI
            sec
            sbc #$50
            sta BUFRHI
            bcs L371B
            dec BFENLO
            bne L371B
L3750       ldy ICAX1Z
L3752       lda #$01
            sta (ICAX6Z),Y
            rts
L3757       jsr L085E
            lda #$F8
            bne L3765
L375E       jsr L085E
            lda (BFENHI),Y
            ora #$F0
L3765       sta DOSVEC+1
            ldx #$03
L3769       jsr L3ADD
            jsr L085E
            lda DOSVEC+1
            sta (BFENHI),Y
            dex
            bne L3769
            jsr L3ADD
            jsr L085E
            lda (BUFRHI),Y
            cmp HOLDCH
            bne L3786
            lda DOSVEC+1
            sta (BFENHI),Y
L3786       jmp L3AE8
L3789       lda LCF6B,Y
            sta BFENLO
            lda LCF7C,Y
            sta BUFRHI
            jmp L085E
L3796       sty ICAX3Z
            ldx #$E0
            ldy #$51
            stx BUFRHI
            sty BFENLO
            lda #$0C
            sta ICAX1Z
            ldx #$00
L37A6       jsr L085E
            ldy ICAX1Z
            lda (CHKSUM),Y
            pha
            lda #$00
            sta (CHKSUM),Y
            pla
            beq L37C0
            ldy ICAX3Z
            lda (BFENHI),Y
            ldy #$13
            sta (BFENHI),Y
            iny
            sta (BFENHI),Y
L37C0       ldy #$13
            lda (BFENHI),Y
            ora #$F0
            cmp COUNTR
            bne L37CB
            inx
L37CB       jsr L3AFE
            dec ICAX1Z
            bne L37A6
            cpx #$06
            beq L37DB
            bcc L37DE
            lda COUNTR
            .byte $0C
L37DB       lda #$F8
            .byte $0C
L37DE       lda L0084
            sta LD9A1+2
            sta LD9A4
            sta LD9CB
            sta LD9CB+1
            sta L008E
L37EE       ldx #$05
L37F0       lda HOLDCH,X
            tay
            lda L0082,X
            sta HOLDCH,X
            tya
            sta L0082,X
            dex
            bpl L37F0
            lda L0088
            eor #$FE
            sta L0088
L3803       rts
L3804       clc
            lda L008D
            beq L3803
            sed
            sbc #$00
            cld
L380D       sta L008D
            lsr
            lsr
            lsr
            lsr
            sta LE2EE,X
            lda L008D
            and #$0F
            sta LE2EF,X
            jmp L318C
L3820       lda COUNTR
            cmp L0089
            beq L386A
            lda TEMPCHR
            lsr
            bcs L3839
            lda L008F
            cmp COUNTR+1
            beq L384E
            bcs L3844
L3833       lda #$FF
            .byte $0C
L3836       lda #$01
            .byte $0C
L3839       lda #$00
            sta ICBAHZ
            lda ICPTHZ
            and #$0F
            sta ICPTHZ
            rts
L3844       ldx COUNTR+1
            bne L3836
            cmp #$07
            bcc L3836
            bcs L3833
L384E       lda ICPTHZ
            and #$0F
            ora #$90
            sta ICPTHZ
            lda #$00
            sta ICBAHZ
            lda LD419+2
            and #$0F
            adc #$00
            cmp #$0D
            bcc L3867
            sbc #$08
L3867       sta L008F
            rts
L386A       lda ICPTHZ
            and #$EF
            sta ICPTHZ
            jmp L0B5C
L3873       lda #$55
            sta L008C
            lda #$99
            sta L008D
L387B       inc TEMPCHR
            lda TEMPCHR
            lsr
L3880       lda RMARGN
            beq L3880
            adc #$FD
            sta HOLD1
            jsr L3820
            ldx #$00
            ldy #$4C
            stx CHKSUM
            sty BUFRLO
            jsr L35B8
            jsr L363F
            ldx #$30
            ldy #$4C
            stx CHKSUM
            sty BUFRLO
            ldy #$0E
            jsr L3796
            lda L008C
            lsr
            bne L38B0
            lda LD419+2
            bmi L38EA
L38B0       bit HOLD1
            bmi L38B0
            jsr L3820
            ldx #$70
            ldy #$4C
            stx CHKSUM
            sty BUFRLO
            jsr L35B8
            jsr L3659
            ldx #$40
            ldy #$4C
            stx CHKSUM
            sty BUFRLO
            ldy #$19
            jsr L3796
            lda TEMPCHR
            lsr
            bcs L38E7
            ldx #$7E
            jsr L3804
            lda L008D
            bne L38E7
            bcc L38E7
            lda #$1B
            sta ATACHR
L38E7       jsr L3B11
L38EA       lda L008D
            bne L387B
            sta LOMEM
            sta L0086
            dec L008C
            bne L387B
            rts
L38F7       lda #$00
            sta RAMLO+1
            jsr L2161
            lda #$40
            sta APPMHI+1
            ldx L00E0
            lda LE26E,X
            and #$7F
            sta APPMHI
            lda LE226,X
            sta L0100
            jmp L0D07
L3914       jsr L2422
            lda L00E0
            ldx #$F5
            ldy #$0B
            jsr L3947
            ldx #$1C
            ldy #$E5
            jsr L3939
            jsr L2422
            ldx ICDNOZ
            lda L00E0,X
            ldx #$F6
            ldy #$0C
            jsr L3947
            ldx #$26
            ldy #$E5
L3939       jsr L31AE
            jsr L241F
            lda #$4D
            sta L1E2C+1
            jmp L31E6
L3947       pha
            txa
            pha
            sty CHBAS
            jsr L0A2B
            pla
            jsr L0939
            pla
            sta DOSINI+1
            jsr L09DC
            jmp L2363
            lda INSDAT
L395F       sta (BUFRHI),Y
            lda COUNTR
            sta (BFENHI),Y
L3965       rts
            tya
            beq L3965
            cpy #$13
            beq L3965
            lda L0083
            jsr L395F
            jmp L3A01
L3975       tya
            beq L3984
            cpy #$13
            beq L3984
            lda #$FC
            sta (BUFRHI),Y
            lda L0084
            sta (BFENHI),Y
L3984       rts
            jsr L39E8
            bcc L39DF
            cmp HOLDCH
            bne L39DF
            jsr L3AEE
            lda (BUFRHI),Y
            beq L399E
            jsr L3AEB
            lda (BUFRHI),Y
            cmp #$FD
            bcs L39DF
L399E       jsr L39DF
            ldy ICAX3Z
            jsr L3A63
            jsr L3A33
            jsr L3AFE
            jsr L3A01
            jsr L3AFE
            jsr L3A33
            jmp L39DF
            jsr L39E8
            bcc L39DF
            beq L39C8
            jsr L3AD7
            lda (BUFRHI),Y
            cmp #$FD
            bcs L39DF
L39C8       jsr L39DF
            ldy ICAX3Z
            jsr L3A63
            jsr L3A01
            jsr L3AFE
            jsr L3A33
            jsr L3AFE
            jsr L3A01
L39DF       ldx ICAX4Z
            ldy ICAX5Z
            stx BUFRHI
            sty BFENLO
            rts
L39E8       sty ICAX3Z
            ldx BUFRHI
            ldy BFENLO
            stx ICAX4Z
            sty ICAX5Z
            lda ICAX1Z
            cmp #$03
            bcc L3A00
            ldy ICAX3Z
            jsr L3ADD
            lda (BUFRHI),Y
            sec
L3A00       rts
L3A01       jsr L3A26
            ldx #$03
L3A06       jsr L3ADD
            lda #$00
            sta (BUFRHI),Y
            dex
            bne L3A06
            jsr L3ADD
            lda (BUFRHI),Y
            eor HOLDCH
            bne L3A1B
            sta (BUFRHI),Y
L3A1B       ldx CHKSUM
            ldy BUFRLO
            stx BUFRHI
            sty BFENLO
            ldy ICAX3Z
            rts
L3A26       sty ICAX3Z
            ldx BUFRHI
            ldy BFENLO
            stx CHKSUM
            sty BUFRLO
            ldy ICAX3Z
            rts
L3A33       jsr L3A26
            ldx #$03
L3A38       jsr L3AF1
            lda #$00
            sta (BUFRHI),Y
            dex
            bne L3A38
            jsr L3AF1
            lda (BUFRHI),Y
            beq L3A1B
            cmp #$FD
            bcs L3A1B
            lda L0083
            sta (BUFRHI),Y
            lda BUFRHI
            sta ICAX6Z
            lda BFENLO
            and #$03
            ora #$D8
            sta STATUS
            lda COUNTR
            sta (ICAX6Z),Y
            bne L3A1B
L3A63       ldx #$FE
            jsr L3A73
            ldx #$FF
            jsr L3A73
            ldy ICAX3Z
            lda #$FD
            bne L3A79
L3A73       tya
            clc
            adc #$28
            tay
            txa
L3A79       sta (BUFRHI),Y
L3A7B       lda COUNTR
            sta (BFENHI),Y
            rts
L3A80       ldx L008B
            ldy L008A
            stx LOMEM
            sty L0086
            lda L0089
            cmp COUNTR
            beq L3A92
            stx L0086
            sty LOMEM
L3A92       ldy #$01
            jsr L3789
            jsr L085E
            lda #$0C
            sta ICAX1Z
            sec
            lda #$0D
            sbc LOMEM
            sta ICBLHZ
            lda #$0D
            sbc L0086
            sta ICAX2Z
L3AAB       ldy #$01
            lda ICBLHZ
            cmp ICAX1Z
            lda COUNTR
            bcc L3AB7
            lda #$F2
L3AB7       sta (BFENHI),Y
            ldy #$26
            lda ICAX2Z
            cmp ICAX1Z
            lda L0084
            bcc L3AC5
            lda #$F2
L3AC5       sta (BFENHI),Y
            clc
            lda BFENHI
            adc #$28
            sta BFENHI
            bcc L3AD2
            inc LTEMP
L3AD2       dec ICAX1Z
            bne L3AAB
L3AD6       rts
L3AD7       jsr L3ADD
            jsr L3ADD
L3ADD       bit L0088
            bmi L3AF5
L3AE1       inc BUFRHI
            bne L3AE7
L3AE5       inc BFENLO
L3AE7       rts
L3AE8       jsr L3AF1
L3AEB       jsr L3AF1
L3AEE       jsr L3AF1
L3AF1       bit L0088
            bmi L3AE1
L3AF5       lda BUFRHI
            bne L3AFB
            dec BFENLO
L3AFB       dec BUFRHI
            rts
L3AFE       clc
            lda BUFRHI
            adc #$28
            sta BUFRHI
            bcs L3AE5
            rts
L3B08       jsr L4071
            jsr L4071
            jsr L4071
L3B11       ldx #$00
            inx
            txa
            and #$07
            sta L3B11+1
            lsr
L3B1B       ldy #$00
            bcs L3B21
            ldy #$08
L3B21       lda LE624,X
            sta L7FA3,Y
            lda LE626,X
            sta L7FA4,Y
            rts
L3B2E       ldx #$FE
            stx LDBFF+1
            lda LDC01
            and #$78
            sta ICBLLZ
            dex
            stx LDBFF+1
            lda LDC01
            asl
            ldx #$BF
            stx LDBFF+1
            lda LDC01
            and #$10
            adc #$EF
            ror ICBLLZ
            ldx #$7F
            stx LDBFF+1
            lda LDC01
            asl
            ror ICBLLZ
            ldx #$FF
            stx LDC01+1
            stx LDBFF+1
            rts
L3B64       lda TEMPCHR
            and #$03
            bne L3BC2
            lda ICBLLZ
            and #$50
            asl
            asl
            bne L3B9F
            ldy DSTAT
            bcc L3B7B
            cpy #$0F
            bcc L3B7E
            iny
L3B7B       dey
            bpl L3B7F
L3B7E       iny
L3B7F       sty DSTAT
            tya
            cmp #$0A
            bcc L3B8B
            sbc #$0A
            ldy #$01
            .byte $0C
L3B8B       ldy #$30
            sty LE32F
            sta LE330
            lda #$80
            ora DSTAT
            sta LD416+2
            ldx #$42
            jsr L318C
L3B9F       lda ICBLLZ
            and #$42
            asl
            asl
            bne L3BC2
            ldy LCA0B
            bcc L3BB2
            cpy HATABS+34
            bcc L3BB5
            iny
L3BB2       dey
            bpl L3BB6
L3BB5       iny
L3BB6       sty LCA0B
            iny
            sty LE337
            ldx #$4C
            jsr L318C
L3BC2       lda ICBLLZ
            and #$44
            asl
            asl
            bne L3BDE
            bcc L3BD5
            ldx #$32
            jsr L318C
            lda #$00
            beq L3BDC
L3BD5       ldx #$39
            jsr L318C
            lda #$FF
L3BDC       sta ATRACT
L3BDE       rts
L3BDF       .byte $00
L3BE0       lsr L3BDF
            bcs L3BE9
            bit ICBLLZ
            bpl L3BEA
L3BE9       rts
L3BEA       ldx #$1C
            jsr L318C
            lda #$00
            sta L00A4
            sta DINDEX
            lda TEMPCHR
            sta SAVMSC
            jsr L3C5D
L3BFC       jsr L1ECB
L3BFF       bit ICBLLZ
            bpl L3C45
            bit ICPTHZ
            bmi L3C45
            lda #$BF
            jsr L0B50
            and #$08
            beq L3C42
            bit DINDEX
            bvs L3C32
            inc TEMPCHR
            jsr L4074
            jsr L3BC2
            jsr L3C6A
L3C1F       jsr LF935
L3C22       bcs L3C45
            lda ICBLLZ
            and #$42
            cmp #$40
            bne L3BFC
            sta DINDEX
            ldx #$22
            bne L3C3C
L3C32       lda ICBLLZ
            and #$42
            bne L3BFC
            sta DINDEX
            ldx #$1C
L3C3C       jsr L318C
            jmp L3BFC
L3C42       jmp IOCB3+ICDNO
L3C45       ldx #$29
            jsr L318C
L3C4A       jsr L3C5D
            bit ICPTHZ
            bmi L3C4A
            jsr L08C1
            lda SAVMSC
            sta TEMPCHR
            lda #$12
            sta L00A4
            rts
L3C5D       inc TEMPCHR
            jsr L4071
            jsr L3C6A
            bit ICBLLZ
            bpl L3C5D
            rts
L3C6A       jsr L3FAA
            jsr L21E9
            jmp L4071
L3C73       jsr L2422
            jsr L0A2B
            lda #$F2
            jsr L0939
            jsr L09DC
            jsr L2363
            clc
            lda BPTR
            adc #$68
            sta LE514
            ldx #$07
            ldy #$E5
            jsr L31AE
            jsr L241F
            lda #$4D
            sta L1E2C+1
            jmp L4074
L3C9E       lda #$0B
            sta CHBAS
            jsr L424D
            lda CHSALT
            ora VSFLAG
            ora KEYDIS
            ora L0093
            php
            bne L3CB9
            tax
            lda #$25
            bne L3CCE
L3CB9       ldx GPRIOR
            bne L3CD2
            lda PADDL0
            lsr
            bcc L3CC6
            ldx #$50
L3CC6       bit LCF96
            beq L3CCE
            sec
            sbc #$03
L3CCE       stx PADDL0
            .byte $0C
L3CD2       lda #$75
            sta GPRIOR
L3CD7       bit TRIG2
            bpl L3CD7
            sei
            ldx #$03
            clc
            sed
L3CE1       lda KEYDIS,X
L3CE4       adc LTEMP+1,X
            sta LTEMP+1,X
            dex
            bmi L3CF0
            bne L3CE1
            txa
            beq L3CE4
L3CF0       cld
            bit ICCOMZ
            bpl L3D0F
            bvs L3CFD
            ldx #$05
            lda #$02
            bne L3D01
L3CFD       ldx #$15
            lda #$06
L3D01       stx PADDL2
            sta ICBLHZ
L3D06       lda #$FA
            jsr L4105
            dec ICBLHZ
            bne L3D06
L3D0F       cli
            lda CHKSNT
            ora NOCKSM
            bne L3D1A
            inc CHKSNT
            inc NOCKSM
L3D1A       jsr L4071
            jsr L2422
            jsr L0A2B
            lda #$F5
            jsr L0939
            lda L00E0
            sta DOSINI+1
            jsr L09DC
L3D2F       jsr L2363
            plp
            bne L3D3B
            ldx #$D2
            ldy #$E4
            bne L3D3F
L3D3B       ldx #$BF
            ldy #$E4
L3D3F       jsr L3230
            ldx #$0F
            jsr L421A
            bit ICCOMZ
            bpl L3D57
            ldx #$E5
            ldy #$E4
            jsr L3230
            ldx #$12
            jsr L421A
L3D57       ldx #$35
            ldy #$E5
            jsr L31AE
            jsr L241F
            lda #$4D
            sta L1E2C+1
            jmp L31E6
L3D69       ldx #$12
            jsr L318C
            lda #$00
            sta BRKKEY
            sta RTCLOK+1
            jsr L2417
            lda #$06
            sta SWPFLG
            lda #$13
            sta CHBAS
            ldx #$AA
            ldy #$E1
            jsr L20E5
L3D87       lda SWPFLG
            bmi L3DB7
            lda #$07
            sta RAMLO+1
            jsr L20AD
            lda SWPFLG
            sta RAMLO+1
            lda LD419+2
            and #$07
            sec
            sbc #$04
            clc
            adc TRAMSZ
            sta TRAMSZ
            lda #$9D
            sta DOSINI
            lda LD419+2
            and #$07
            sec
            sbc #$04
            clc
            adc WARMST
            sta WARMST
            jsr L20F3
L3DB7       jsr L0758
            lda #$07
            sta RAMLO+1
L3DBE       jsr L20AD
            inc DOSINI
            lda DOSINI
            cmp #$A8
            bcc L3DCD
            lda #$00
            sta BOOT
L3DCD       jsr L20F3
            dec RAMLO+1
            bpl L3DBE
            jsr L0758
            dec SWPFLG
            lda SWPFLG
            cmp #$F0
            bne L3D87
            lda #$80
            sta L3DF2+2
            ldy #$21
L3DE6       ldx #$00
L3DE8       lda LD419+2
            eor LDD06
            and #$03
            ora #$EC
L3DF2       sta L8000,X
            bit L00BF
            inx
            cpx #$A7
            bne L3DE8
            inc L3DF2+2
            dey
            bne L3DE6
            ldx #$03
            tya
L3E05       sta LC1EC,X
            dex
            bpl L3E05
            lda #$40
            sta TEMPCHR
            lda #$0F
            sta CHBAS
L3E14       jsr L4071
            lda LD419+2
            ldx #$07
            .byte $87,$4F
            lsr
            lsr
            lsr
            .byte $87
            lsr L1BAD
            .byte $D4
            sta DSKUTL+1
            lda #$03
            and LD419+2
            sta ABUFPT
            lda #$00
            sta ABUFPT+2
            lda LD419+2
            lsr
            sta ABUFPT+1
L3E39       bit TRIG2
            bpl L3E39
            lda #$F1
L3E40       sta L0090
            jsr L1ED6
            lda TRIG2
            cmp #$76
            bcs L3E6A
            ldx #$07
L3E4E       ldy L7F78,X
            lda L7F70,X
            sta L7F78,X
            lda L7F68,X
            sta L7F70,X
            lda L7F60,X
            sta L7F68,X
            tya
            sta L7F60,X
            dex
            bpl L3E4E
L3E6A       inc TEMPCHR
            bpl L3E14
            jsr L4071
            jsr L0A2B
            lda #$F3
            jsr L0939
L3E79       lda #$17
            sta DOSINI+1
            ldx #$A0
            jsr L2365
            ldx #$D0
            ldy #$E3
            jsr L3230
            ldx #$DF
            ldy #$E3
            jsr L3230
            jsr L241F
            lda #$4D
            sta L1E2C+1
            lda #$05
            sta CHBAS
            lda #$64
            sta SWPFLG
L3EA1       jsr L4071
            lda SWPFLG
            beq L3EAA
            dec SWPFLG
L3EAA       ora CHKSNT
            ora NOCKSM
            bne L3EA1
            ldy BPTR
            dey
            cpy HATABS+34
            bcc L3EC0
            beq L3EC0
            sty HATABS+34
            inc DCOMND
L3EC0       ldy #$04
            jsr L3F15
            bcs L3ED5
            ldy #$00
            jsr L3F15
            bcc L3F0A
            ldx #$06
            ldy #$E4
            clc
            bcc L3EDC
L3ED5       inc DCOMND
            ldx #$EC
            ldy #$E3
L3EDC       php
            jsr L3230
            ldx #$15
            ldy #$E4
            jsr L3230
            jsr L3F28
            plp
            bcc L3EF5
            ldx #$00
            jsr L3F8A
            ldy #$07
            .byte $0C
L3EF5       ldy #$03
            ldx #$03
L3EF9       lda LTEMP+1,X
            sta HATABS+26,Y
            dey
            dex
            bpl L3EF9
            tya
            bpl L3EF5
            ldx #$10
            jsr L3F8A
L3F0A       lda DCOMND
            beq L3F27
            sei
L3F10       dec NGFLAG
            jmp LD075
L3F15       ldx #$00
L3F17       lda LTEMP+1,X
            cmp HATABS+26,Y
            bcc L3F27
            bne L3F27
            iny
            inx
            cpx #$04
            bcc L3F17
            clc
L3F27       rts
L3F28       ldx #$03
L3F2A       lda LE435,X
            sta L4BFF,X
            lda #$26
            sta LE435,X
            dex
            bne L3F2A
            stx ICAX2Z
            jsr L3F40
            jsr L3F40
L3F40       ldx ICAX2Z
            lda L4C00,X
            ldy #$00
L3F47       cmp LE540,Y
            beq L3F51
            iny
            cpy #$1A
            bcc L3F47
L3F51       sty ICAX3Z
L3F53       jsr L1EC5
            lda ICBAHZ
            bne L3F5F
            jsr L1EC5
            lda ICBAHZ
L3F5F       ora ICPTLZ
            clc
            adc ICAX3Z
            bmi L3F6D
            cmp #$1B
            bcc L3F6F
            lda #$00
            .byte $0C
L3F6D       lda #$1A
L3F6F       sta ICAX3Z
            tay
            lda LE540,Y
            ldx ICAX2Z
            sta LE436,X
            ldx #$34
            ldy #$E4
            jsr L3230
            bit ICPTHZ
            bpl L3F53
            inc ICAX2Z
            jmp L0929
L3F8A       ldy #$00
L3F8C       lda LE436,Y
            sta IOCB0+ICAX1,X
            inx
            iny
            cpy #$03
            bcc L3F8C
L3F98       lda L506B,Y
            sta HATABS+34,X
            inx
            iny
            cpy #$0B
            bne L3F98
            rts
L3FA5       ldx #$01
            jsr L3FAC
L3FAA       ldx #$00
L3FAC       dec LE5EB,X
            bpl L4018
            lda LE5ED,X
            sta LE5EB,X
            lda LE5EF,X
            adc #$08
            and #$18
            sta LE5EF,X
            ora LE21E,X
            tay
            cpy #$30
            bcc L3FCB
            ldy #$28
L3FCB       lda LE5F1,X
            sta L3FDC+1
            lda LE5F3,X
L3FD4       sta L3FDC+2
            ldx #$07
L3FD9       lda LE5F5,Y
L3FDC       sta L7800,X
            iny
            dex
            bpl L3FD9
            bit LC110
            bmi L4018
            lda L78A0
            sta L7888
            lda L78A1
            sta L7889
            lda L78A2
            sta L788A
            lda L78A3
            sta L788B
            lda L78A4
            sta L788C
            lda L78A5
            sta L788D
            lda L78A6
            sta L788E
            lda L78A7
            sta L788F
L4018       rts
L4019       lda TEMPCHR
            lsr
            bcs L4070
            lda DSKFMS+1
            cmp #$14
            beq L4028
            cmp #$11
            bne L4070
L4028       lda ICCOMT
            ldy DSKFMS
L402C       ldx #$FC
            .byte $87,$2F
            tya
            .byte $87
            bmi L3FD4
            ora (L00B1,X)
            .byte $2F
            and #$03
            tax
            lda LCF8A,X
            sta (ICAX6Z),Y
            iny
            lda LCF8B,X
            sta (ICAX6Z),Y
            iny
            inc STATUS
            lda LCF8C,X
            sta (ICAX6Z),Y
            ldy #$00
            lda LCF8D,X
            sta (ICAX6Z),Y
            inc STATUS
            lda LCF8C,X
            sta (ICAX6Z),Y
            ldy #$03
            lda LCF8D,X
            sta (ICAX6Z),Y
            inc STATUS
            dey
            lda LCF8A,X
            sta (ICAX6Z),Y
            dey
            lda LCF8B,X
            sta (ICAX6Z),Y
L4070       rts
L4071       jsr L1ECB
L4074       ldy #$00
            lda CHKSNT
            beq L40A5
            ldx NOCKSM
            beq L4089
            sec
            sbc NOCKSM
            beq L40BE
            bcc L409D
            sta CHKSNT
            sty NOCKSM
L4089       dec CHKSNT
            sed
            sec
            ldx #$03
L408F       tya
            adc LTEMP+1,X
            sta LTEMP+1,X
            dex
            bpl L408F
            bcc L40C2
            lda #$99
            bne L40BB
L409D       eor #$FF
            sta NOCKSM
            sty CHKSNT
            bcc L40AB
L40A5       lda NOCKSM
            beq L40F7
            dec NOCKSM
L40AB       sed
            clc
            ldx #$03
L40AF       lda LTEMP+1,X
            sbc #$00
            sta LTEMP+1,X
            dex
            bpl L40AF
            bcs L40C2
            tya
L40BB       jsr L412C
L40BE       sty CHKSNT
            sty NOCKSM
L40C2       cld
            lda #$30
            sta ICBLHZ
            ldx #$00
            stx ICAX2Z
L40CB       lda LTEMP+1,X
            lsr
            lsr
            lsr
            lsr
            beq L40D8
            ldy #$00
            sty ICBLHZ
            .byte $0C
L40D8       lda ICBLHZ
            jsr L40F8
            lda LTEMP+1,X
            and #$0F
            beq L40E9
            ldy #$00
            sty ICBLHZ
            beq L40EF
L40E9       cpx #$03
            beq L40EF
            lda ICBLHZ
L40EF       jsr L40F8
            inx
            cpx #$04
            bne L40CB
L40F7       rts
L40F8       ldy ICAX2Z
            sta L506E,Y
            ora #$80
            sta L5096,Y
            inc ICAX2Z
            rts
L4105       clc
            adc CHKSNT
            sta CHKSNT
            bcc L4134
            sed
            lda XMTDON
            adc #$54
            sta XMTDON
            lda RECVDN
            adc #$02
            sta RECVDN
            lda BUFRFL
            adc #$00
            sta BUFRFL
            lda LTEMP+1
            adc #$00
            sta LTEMP+1
            cld
            inc CHKSNT
            lda #$99
            bcc L4134
L412C       sta LTEMP+1
            sta BUFRFL
            sta RECVDN
            sta XMTDON
L4134       rts
L4135       clc
            adc NOCKSM
            sta NOCKSM
            bcc L415C
            sed
            lda XMTDON
            sbc #$55
            sta XMTDON
            lda RECVDN
            sbc #$02
            sta RECVDN
            lda BUFRFL
            sbc #$00
            sta BUFRFL
            lda LTEMP+1
            sbc #$00
            sta LTEMP+1
            cld
            inc NOCKSM
            lda #$00
            bcc L412C
L415C       rts
L415D       ldx #$14
            lda #$00
L4161       sta L025F,X
            dex
            bpl L4161
            rts
L4168       stx CASINI
            ldx #$0F
            bpl L418A
L416E       stx CASINI
            ldx #$00
            bpl L418A
L4174       stx CASINI
            ldx #$03
            bpl L418A
L417A       stx CASINI
            ldx #$06
            bpl L418A
L4180       stx CASINI
            ldx #$09
            bpl L418A
L4186       stx CASINI
            ldx #$0C
L418A       clc
            sed
            lda #$01
            adc L0261,X
            sta L0261,X
            lda #$00
            adc L0260,X
            sta L0260,X
            lda #$00
            adc L025F,X
            sta L025F,X
            cld
            ldx CASINI
            rts
L41A8       lda OLDADR
            bne L4219
            jsr L2422
            lda #$33
            ldx #$F6
            jsr L0A32
            lda #$FE
            jsr L0939
            ldx #$3D
            ldy #$E4
            jsr L3230
            ldx #$4F
            ldy #$E4
            jsr L3230
            ldx #$00
            jsr L421A
            ldx #$61
            ldy #$E4
            jsr L3230
            ldx #$0C
            jsr L421A
            ldx #$71
            ldy #$E4
            jsr L3230
            ldx #$09
            jsr L421A
            ldx #$80
            ldy #$E4
            jsr L3230
            jsr L424D
            ldx #$0F
            jsr L421A
            lda #$7F
            jsr L32BE
            ldx #$91
            ldy #$E4
            jsr L3230
            ldx #$03
            jsr L421A
            ldx #$AA
            ldy #$E4
L420A       jsr L3230
L420D       ldx #$06
            jsr L421A
L4212       lda #$4D
L4214       sta L1E2C+1
L4217       inc OLDADR
L4219       rts
L421A       lda #$05
            sta ZCHAIN
            lda #$00
            sta ZCHAIN+1
L4222       lda L025F,X
            lsr
            lsr
            lsr
            lsr
            jsr L4240
            dec ZCHAIN
            bne L4232
            inc ZCHAIN+1
L4232       lda L025F,X
            and #$0F
            jsr L4240
            inx
            dec ZCHAIN
            bpl L4222
            rts
L4240       bne L4247
L4242       ldy ZCHAIN+1
            beq L424C
            .byte $0C
L4247       sta ZCHAIN+1
            jsr L32BE
L424C       rts
L424D       ldx #$02
L424F       lda L0268,X
            sta L42A1,X
            lda #$00
            sta FINE,X
            dex
            bpl L424F
            sta L42A4
            lda L0268
            ora L0269
            ora L026A
            bne L427E
            rts
L426C       sta L42A1
            pla
            sta L42A2
            pla
            sta L42A3
            pla
            sta L42A4
            jsr L4168
L427E       sed
            sec
            lda L42A4
            sbc KEYDIS
            pha
            lda L42A3
            sbc VSFLAG
            pha
            lda L42A2
            sbc CHSALT
            pha
            lda L42A1
            sbc #$00
            bcs L426C
            pla
            pla
            pla
            cld
            rts
L42A1       .byte $00
L42A2       .byte $00
L42A3       .byte $00
L42A4       .byte $00
L42A5       jsr L42CD
            lda #$80
            ora DSTAT
            sta LD416+2
            rts
L42B0       ldx #$18
L42B2       sta DMACLT,X
            dex
            bpl L42B2
            sta L00A5
            rts
L42BB       lda L00A4
            beq L42B0
            cmp #$02
            beq L433D
            bcc L4302
            cmp #$11
            beq L42A5
            cmp #$12
            bne L4301
L42CD       ldx #$0F
            .byte $87
            ldy L00A9
            .byte $00
            ldx #$0D
L42D5       sta L00A6,X
            sta L00B4,X
            sta DMACLT,X
            dex
            bpl L42D5
            sta NEWROW
            sta CH
            sta CHBAS
            sta ATACHR
            lda #$8F
            sta LD416+2
            stx NMIEN
            stx NMIRES
            stx LD413
            stx LD413+1
            lda #$81
L42FE       sta LD412
L4301       rts
L4302       dec L00A5
            lda L00A5
            asl
            bne L4322
            lda #$1D
            ldx LD419+2
            cpx #$55
            adc #$00
            cpx #$AA
            adc #$00
            sta CHBAS
            bcc L433D
L431B       lda #$10
            sta ATACHR
            bne L433D
L4322       asl
            cmp #$88
            beq L431B
            cmp #$C0
            beq L431B
            and #$3C
            cmp #$20
            bne L433D
            lda LD419+2
            and #$1F
            adc #$EF
            clc
            adc L00A9
            sta L00A9
L433D       ldx #$07
L433F       lda CHBAS,X
            beq L435C
            ldy NEWROW,X
            bmi L4357
            sta NEWROW,X
            jsr L43E8
            lda #$00
            sta CHBAS,X
            jmp L43DF
L4357       lda #$00
            sta CHBAS,X
L435C       lda L00AC,X
            beq L43DC
            clc
            lda L00A6,X
            adc L00A8,X
            sta L00A6,X
            sta DMACLT,X
            lda L00A7,X
            adc L00A9,X
            sta L00A7,X
            sta CHACTL,X
            ldy L00B9,X
            lda LCB9F,Y
            asl
            bpl L438D
            lda L00B4,X
            adc L00B6,X
            sta L00B4,X
            sta DLISTL,X
            lda L00B5,X
            adc L00B7,X
            sta L00B5,X
            sta DLISTH,X
L438D       lsr NEWCOL,X
            bcc L43A6
            lda LCBAB,Y
            sta VSCROL,X
            lda LCB9F,Y
            ora #$01
            sta HSCROL,X
            lda LCBB7,Y
            sta LD406,X
L43A6       lda L00BA,X
            beq L43B4
            dec L00BA,X
            bne L43B4
            lda LCB9F,Y
            sta HSCROL,X
L43B4       dec L00AA,X
            bne L43DF
            lda L00AB,X
            sta L00AA,X
            lda L00B8,X
            beq L43C8
            sta L00A7,X
            lda #$00
            sta L00A6,X
            beq L43D3
L43C8       sec
            sbc L00A8,X
            sta L00A8,X
            lda #$00
            sbc L00A9,X
            sta L00A9,X
L43D3       dec L00AC,X
            bne L43DF
            lda #$00
            sta HSCROL,X
L43DC       sta NEWROW,X
L43DF       dex
            bmi L43E7
            ldx #$00
            jmp L433F
L43E7       rts
L43E8       and #$1F
            tay
            asl
            asl
            sta L43F1+1
            asl
L43F1       adc #$00
            sta L4420+1
            sta L442B+1
            lda #$00
            adc #$CA
            sta L4420+2
            sta L442B+2
            lda LCB7F,Y
            sta L00B9,X
            tay
            lda #$00
            sta VSCROL,X
            sta LD406,X
            sta HSCROL,X
            lda LCBC3,Y
            sta L00BA,X
            lda #$01
            sta NEWCOL,X
            ldy #$00
L4420       lda L1234,Y
            sta L00A6,X
            iny
            inx
            cpy #$07
            bcc L4420
L442B       lda L1234,Y
            sta L00AD,X
            iny
            inx
            cpy #$0C
            bcc L442B
            txa
            .byte $CB,$0C
            rts
L443A       lda #$BF
            sta CASINI+1
            lda LCA0B
            sta RAMLO
            rts
L4444       lda CASINI+1
            asl
            rol RAMLO
            bcc L4450
            eor #$D7
            sta CASINI+1
            rts
L4450       eor #$00
            sta CASINI+1
            rts
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
L4500       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
L4530       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
L4600       .byte $FF,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$FF,$80,$55,$40,$33,$2A
L4617       .byte $24,$20,$1C,$19,$17,$15,$13,$12,$00,$00,$80,$FF,$AA,$80,$66,$55
            .byte $49,$40,$38,$33,$2E,$2A,$27,$24,$00,$00,$55,$AA,$FF,$C0,$99,$80
            .byte $6D,$60,$55,$4C,$45,$40,$3B,$36,$00,$00,$40,$80,$C0,$FF,$CC,$AA
            .byte $92,$80,$71,$66,$5D,$55,$4E,$49,$00,$00,$33,$66,$99,$CC,$FF,$D5
            .byte $B6,$A0,$8E,$80,$74,$6A,$62,$5B,$00,$00,$2A,$55,$80,$AA,$D5,$FF
            .byte $DB,$C0,$AA,$99,$8B,$80,$76,$6D,$00,$00,$24,$49,$6D,$92,$B6,$DB
            .byte $FF,$E0,$C7,$B3,$A2,$95,$89,$80,$00,$00,$20,$40,$60,$80,$A0,$C0
            .byte $E0,$FF,$E3,$CC,$BA,$AA,$9D,$92,$00,$00,$1C,$38,$55,$71,$8E,$AA
            .byte $C7,$E3,$FF,$E6,$D1,$C0,$B1,$A4,$00,$00,$19,$33,$4C,$66,$80,$99
            .byte $B3,$CC,$E6,$FF,$E8,$D5,$C4,$B6,$00,$00,$17,$2E,$45,$5D,$74,$8B
            .byte $A2,$BA,$D1,$E8,$FF,$EA,$D8,$C9,$00,$00,$15,$2A,$40,$55,$6A,$80
            .byte $95,$AA,$C0,$D5,$EA,$FF,$EC,$DB,$00,$00,$13,$27,$3B,$4E,$62,$76
            .byte $89,$9D,$B1,$C4,$D8,$EC,$FF,$ED,$00,$00,$12,$24,$36,$49,$5B,$6D
            .byte $80,$92,$A4,$B6,$C9,$DB,$ED,$FF,$00,$00,$11,$22,$33,$44,$55,$66
            .byte $77,$88,$99,$AA,$BB,$CC,$DD,$EE,$00,$00,$10,$20,$30,$40,$50,$60
            .byte $70,$80,$90,$A0,$B0,$C0,$D0,$E0,$00,$00,$0F,$1E,$2D,$3C,$4B,$5A
            .byte $69,$78,$87,$96,$A5,$B4,$C3,$D2,$00,$00,$0E,$1C,$2A,$38,$47,$55
            .byte $63,$71,$80,$8E,$9C,$AA,$B8,$C7,$00,$00,$0D,$1A,$28,$35,$43,$50
            .byte $5E,$6B,$79,$86,$94,$A1,$AF,$BC,$00,$00,$0C,$19,$26,$33,$40,$4C
            .byte $59,$66,$73,$80,$8C,$99,$A6,$B3,$00,$00,$0C,$18,$24,$30,$3C,$49
            .byte $55,$61,$6D,$79,$86,$92,$9E,$AA,$00,$00,$0B,$17,$22,$2E,$3A,$45
            .byte $51,$5D,$68,$74,$80,$8B,$97,$A2,$00,$00,$0B,$16,$21,$2C,$37,$42
            .byte $4D,$59,$64,$6F,$7A,$85,$90,$9B,$00,$00,$0A,$15,$20,$2A,$35,$40
            .byte $4A,$55,$60,$6A,$75,$80,$8A,$95,$00,$00,$0A,$14,$1E,$28,$33,$3D
            .byte $47,$51,$5C,$66,$70,$7A,$85,$8F,$00,$00,$09,$13,$1D,$27,$31,$3B
            .byte $44,$4E,$58,$62,$6C,$76,$80,$89,$00
L47B0       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
L47C0       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
L47D0       .byte $00,$10,$20,$30,$40,$50,$60,$70,$80,$90,$A0,$B0,$C0,$D0,$E0,$F0
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
L4800       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
L4900       .byte $00,$00,$00,$00
L4904       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
L4940       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
L494D       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
L495A       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00
L49AB       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00
L4A00       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
L4A0D       .byte $00,$00
L4A0F       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00
L4B00       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
L4BF8       .byte $00,$00,$00,$00,$00,$00,$00
L4BFF       .byte $00
L4C00       .byte $EE,$19,$D0,$4C,$7E,$EA
L4C06       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00
L4C20       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
L4C6F       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
L4CB7       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
L4CD2       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00
L4CE8       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
L4D00       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
L4D2B       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00
L4E00       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
L4F00       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
L5000       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00
L5052       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00
L506B       .byte $00,$00,$00
L506E       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
L507A       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
L5096       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00
L5140       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
L514D       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
L515A       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
L529A       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
L53A6       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00
L53E8       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
L53F8       .byte $00,$00,$00,$00,$00,$00,$00,$00
L5400       .byte $00,$00,$00
L5403       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00
L5418       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00
L5430       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
L54C0       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
L54FE       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00
L5530       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$18
            .byte $00,$00,$3C,$00,$00,$3C,$00,$00,$18,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$2B,$0F,$00,$2B,$05,$00,$2B,$0F,$00,$2B,$05,$00,$04,$0F,$00
            .byte $0B,$05,$00,$0B,$0A,$00,$0B,$2A,$00,$0B,$8A,$00,$0B,$9A,$00,$0B
            .byte $AA,$00,$0B,$FF,$00,$0B,$AC,$00,$0B,$AC,$00,$0B,$AC,$00,$04,$AC
            .byte $00,$3F,$AC,$00,$CE,$AC,$00,$0B,$AC,$00,$0F,$AC,$00,$0C,$AC,$95
            .byte $00,$00,$AC,$00,$00,$BC,$00,$00,$FC,$00,$00,$10,$00,$00,$2C,$00
            .byte $00,$AC,$00,$00,$AC,$00,$00,$AC,$00,$00,$AC,$00,$00,$AC,$00,$00
            .byte $AC,$00,$00,$AC,$00,$00,$AC,$00,$00,$AC,$00,$00,$AC,$00,$00,$AC
            .byte $00,$00,$BC,$00,$00,$FC,$00,$00
L5678       .byte $10,$00,$00,$AC,$00,$02,$BC,$95,$00,$00,$EA,$00,$00,$3F,$00,$00
            .byte $EA,$00,$00,$3F,$00,$00,$EA,$00,$00,$3F,$00,$00,$AA,$00,$02,$AA
            .byte $00,$0A,$BF,$00,$2A,$EA,$00,$AB,$AA,$02,$AE,$A2,$0A,$AE,$A6,$0A
            .byte $AB,$AA,$29,$5A,$EA,$2B,$FA,$EA,$29,$5A,$FF,$39,$5A,$AA,$03,$F2
            .byte $AA,$01,$53,$FF,$01,$50,$00,$95,$00,$03,$F0,$00,$00,$80,$00,$00
            .byte $80,$00,$02,$AA,$00,$02,$AA,$00,$0A,$95,$00,$0A,$BA,$00,$25,$BA
            .byte $00,$2F,$BA,$00,$AF,$BA,$00,$A0,$80,$00,$AA,$AA,$00,$3F,$FF,$00
            .byte $0F,$FF,$00,$00,$55,$00,$00,$15,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$90,$00,$0A,$AA,$00,$2A,$AA,$00,$AF
            .byte $FA,$02,$AE,$BA,$02,$AC,$3A,$03,$AD,$7A,$0B,$AE,$BA,$0A,$EF,$FA
            .byte $2A,$FA,$AB,$2B,$0E,$AE,$9C,$02,$AE,$90,$02,$BA,$A0,$03,$BA,$E8
            .byte $00,$FF,$3A,$08,$15,$0E,$AA,$00,$03,$9A,$00,$00,$DA,$00,$00,$2A
            .byte $00,$00,$00,$00,$00,$00,$00,$8F,$00,$00,$BA,$00,$00,$BA,$00,$02
            .byte $BA,$00,$02,$BA,$00,$0A,$BA,$82,$0A,$BF,$8B,$2A,$AA,$9C,$3E,$AA
            .byte $98,$2B,$AA,$3A,$AC,$FF,$0D,$B0,$55,$01,$C0,$15,$00,$00,$05,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$8D,$00,$02,$AA,$00,$02,$AA,$00,$0A
            .byte $AA,$00,$0A,$55,$00,$0B,$AA,$00,$1B,$9A,$00,$2B,$9A,$00,$1B,$AA
            .byte $00,$2A,$00,$00,$1A,$AA,$00,$2A,$AA,$00,$1A,$95,$00,$0A,$BA,$00
            .byte $0E,$80,$00,$03,$AA,$00,$00,$FF,$00,$00,$15,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$91,$00,$00,$05,$00,$00,$3F,$00,$00
            .byte $05,$00,$02,$AA,$00,$2B,$FF,$00,$2E,$AA,$00,$2E,$AA,$00,$AB,$AA
            .byte $00,$BA,$FF,$00,$AE,$AA,$02,$AF,$AA,$02,$B0,$FF,$0A,$C0,$05,$2B
            .byte $00,$3F,$14,$00,$05,$16,$00,$3F,$0E,$80,$05,$03,$A3,$00,$00,$EC
            .byte $00,$00,$3F,$C0,$00,$0F,$00,$91,$00,$00,$AA,$00,$00,$AA,$00,$00
            .byte $FF,$00,$00,$15,$00,$02,$AA,$00,$0A,$AA,$00,$2A,$AA,$00,$FF,$FF
            .byte $00,$EA,$AA,$00,$E9,$56,$00,$EB,$AE,$00,$78,$02,$00,$9E,$AA,$00
            .byte $A7,$95,$00,$A9,$40,$00,$AB,$C0,$00,$8B,$C0,$00,$9B,$C0,$00,$AF
            .byte $00,$00,$AF,$00,$00,$AF,$00,$95,$00,$AC,$00,$00,$AC,$00,$00,$AC
            .byte $00,$00,$50,$00,$00,$50,$00,$00,$AC,$00,$00,$AC,$00,$00,$AC,$00
            .byte $00,$8C,$00,$00,$9C,$00,$00,$AC,$00,$00,$AC,$00,$00,$AC,$00,$00
            .byte $AC,$00,$00,$8C,$00,$00,$9C,$00,$00,$AC,$00,$00,$AC,$00,$00,$50
            .byte $00,$00,$EC,$00,$03,$BB,$00,$95,$50,$00,$00,$FC,$00,$00,$50,$00
            .byte $00,$AA,$A0,$00,$FF,$E8,$00,$AA,$B8,$00,$AA,$B8,$00,$AA,$E8,$00
            .byte $FF,$AC,$00,$AA,$BA,$80,$AA,$EA,$A8,$FF,$3E,$A5,$50,$03,$E5,$FC
            .byte $00,$3B,$50,$00,$0B,$FC,$00,$0B,$50,$00,$0B,$00,$00,$0B,$00,$00
            .byte $F4,$00,$00,$3C,$00,$03,$F0,$91,$00,$00,$2A,$00,$0A,$AA,$00,$AA
            .byte $AA,$02,$A8,$2A,$0A,$A0,$0A,$2A,$AE,$BA,$2A,$AE,$BA,$EA,$AE,$BA
            .byte $FA,$A9,$6A,$7E,$AA,$AA,$1F,$EA,$AA,$07,$FF,$FF,$01,$7F,$FF,$00
            .byte $15,$55,$00,$00,$EA,$00,$00,$3F,$00,$00,$EA,$00,$00,$3F,$00,$00
            .byte $EA,$00,$00,$3F,$00,$00,$00,$94,$00,$00,$2A,$00,$00,$BF,$00,$02
            .byte $EA,$00,$02,$E0,$00,$0B,$AF,$00,$0B,$9F,$00,$0E,$9F,$00,$2E,$95
            .byte $00,$2B,$AA,$00,$6A,$FF,$02,$6A,$AA,$0A,$6A,$FF,$3A,$6B,$AA,$3E
            .byte $6E,$95,$2E,$2E,$95,$A8,$2B,$A5,$F0,$0A,$EA,$A8,$02,$BF,$2C,$00
            .byte $EA,$3A,$00,$3F,$0B,$00,$00,$94,$00,$00,$EA,$0E,$80,$3F,$02,$D0
            .byte $EA,$03,$54,$3F,$01,$50,$EA,$01,$40,$3F,$01,$00,$AA,$00,$02,$AA
            .byte $00,$0A,$BF,$00,$2A,$EA,$00,$AB,$AA,$02,$AE,$A2,$0A,$AE,$A6,$0A
            .byte $AB,$AA,$29,$5A,$EA,$2B,$FA,$EA,$29,$5A,$FF,$39,$5A,$AA,$03,$F2
            .byte $AA,$01,$53,$FF,$01,$50,$00,$95,$00,$40,$2A,$00,$10,$AA,$00,$06
            .byte $AA,$00,$02,$AA,$00,$0A,$AA,$00,$08,$A2,$00,$19,$A6,$00,$0E,$AA
            .byte $00,$0F,$AA,$00,$03,$EA,$00,$07,$FF,$00,$10,$FF,$00,$40,$3F,$00
            .byte $00,$05,$00,$00,$0F,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$8F,$00,$00,$0F,$00,$00,$AA,$00,$0A
            .byte $BF,$00,$AA,$EA,$00,$AB,$AA,$01,$AB,$AA,$09,$EB,$AF,$0A,$7A,$FA
            .byte $2A,$CE,$AA,$2B,$03,$A2,$AC,$00,$A6,$D0,$00,$AA,$68,$04,$FF,$1A
            .byte $1F,$00,$06,$97,$FF,$01,$97,$77,$00,$DF,$FF,$00,$FC,$00,$03,$F0
            .byte $00,$0F,$C0,$00,$0F,$C0,$00,$8D,$F0,$00,$00,$AA,$00,$00,$FE,$A0
            .byte $00,$AB,$AA,$00,$AA,$EA,$00,$AA,$EA,$40,$FA,$EB,$40,$AF,$AD,$A0
            .byte $AA,$B3,$A0,$8A,$C3,$A0,$9A,$03,$A0,$AA,$03,$80,$FF,$03,$80,$00
            .byte $03,$80,$FF,$FD,$70,$77,$75,$7F,$FF,$FD,$50,$03,$C1,$50,$01,$40
            .byte $60,$03,$C0,$A0,$01,$40,$00,$8D,$00,$00,$AA,$00,$02,$AA,$00,$0A
            .byte $AA,$00,$0A,$AA,$00,$06,$AA,$00,$05,$6A,$00,$09,$5F,$00,$0A,$A0
            .byte $00,$08,$20,$00,$0A,$E0,$00,$0A,$E0,$00,$0A,$E0,$00,$0A,$E0,$00
            .byte $0A,$E0,$00,$0A,$E0,$00,$0A,$E0,$00,$09,$60,$00,$0A,$A0,$00,$0A
            .byte $A0,$00,$09,$60,$00,$09,$60,$95,$00,$0A,$A0,$00,$08,$20,$00,$0A
            .byte $E0,$00,$0A,$E0,$00,$0A,$E0,$00,$0A,$E0,$00,$0A,$E0,$00,$0A,$E0
            .byte $00,$0A,$E0,$00,$09,$60,$00,$0A,$A0,$00,$0A,$A0,$00,$01,$40,$00
            .byte $0A,$A0,$00,$0E,$B0,$00,$2B,$FC,$00,$2A,$BC,$00,$3A,$BC,$00,$0F
            .byte $F0,$00,$00,$00,$00,$00,$00,$93,$00,$00,$2A,$00,$02,$8A,$00,$0A
            .byte $DE,$00,$2A,$DE,$00,$3A,$9A,$00,$0F,$EA,$00,$03,$FF,$00,$00,$3F
            .byte $00,$00,$05,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$89,$00,$00,$2A,$80,$02,$A2,$80,$0A
            .byte $A6,$80,$2A,$FF,$C0,$9B,$D5,$C2,$8F,$55,$CA,$9D,$55,$CE,$8D,$55
            .byte $FE,$9D,$55,$CE,$AD,$55,$CE,$8D,$55,$FE,$9D,$55,$CE,$AD,$55,$4E
            .byte $8D,$50,$0E,$9D,$40,$07,$AD,$00,$01,$F4,$00,$00,$50,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$90,$00,$00,$3F,$00,$00,$3A,$00,$00
            .byte $3A,$00,$00,$3A,$00,$00,$3A,$00,$03,$FA,$00,$03,$AA,$00,$00,$EA
            .byte $00,$00,$3A,$00,$00,$0E,$00,$00,$03,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$0F,$00,$00,$33,$00,$00,$F7,$00,$03,$3C,$00,$03
            .byte $4D,$00,$03,$D3,$00,$00,$37,$8D,$00,$00,$3F,$00,$00,$C0,$00,$03
            .byte $00,$00,$06,$00,$00,$06,$00,$00,$0C,$00,$00,$0C,$00,$00,$0C,$00
            .byte $00,$0C,$78,$00,$0C,$CC,$00,$0C,$CC,$00,$0C,$79,$00,$0C,$03,$00
            .byte $0C,$06,$00,$06,$03,$00,$03,$80,$00,$00,$CF,$00,$00,$C7,$00,$00
            .byte $40,$00,$00,$60,$00,$00,$3F,$15,$00,$00,$0A,$00,$00,$2A,$00,$00
            .byte $AA,$00,$02,$AA,$00,$02,$AA,$00,$03,$FF,$00,$01,$51,$00,$02,$B3
            .byte $00,$02,$B1,$00,$02,$B2,$00,$02,$B2,$00,$02,$B2,$00,$02,$F2,$00
            .byte $02,$C2,$00,$02,$C3,$00,$02,$C1,$00,$02,$C5,$00,$03,$F5,$00,$01
            .byte $50,$00,$06,$54,$00,$06,$54,$95,$00,$00,$10,$00,$00,$10,$00,$00
            .byte $2A,$00,$00,$0F,$00,$00,$AA,$00,$2A,$8A,$02,$AA,$9A,$0A,$AA,$AA
            .byte $00,$00,$00,$2A,$2B,$FF,$2A,$2F,$FF,$A2,$2C,$FB,$66,$3D,$F7,$2A
            .byte $3F,$FF,$05,$55,$55,$05,$7F,$FF,$01,$55,$FF,$00,$15,$55,$00,$00
            .byte $55,$00,$00,$05,$00,$00,$00,$94,$04,$00,$00,$04,$00,$00,$A8,$00
            .byte $00,$F0,$00,$00,$BF,$00,$00,$A3,$FC,$00,$A7,$FF,$C0,$AB,$FF,$F0
            .byte $00,$00,$00,$FF,$FE,$54,$FF,$F6,$54,$EF,$B6,$66,$DF,$56,$55,$FF
            .byte $56,$54,$55,$55,$50,$FD,$55,$50,$F5,$55,$40,$55,$54,$00,$55,$00
            .byte $00,$50,$00,$00,$00,$00,$00,$8D,$00,$2B,$0F,$00,$2B,$05,$00,$AB
            .byte $AA,$02,$BF,$FF,$0A,$C0,$0F,$2B,$00,$05,$AC,$00,$0F,$B0,$15,$55
            .byte $B0,$D5,$FF,$B3,$D7,$FF,$B3,$D7,$FF,$B3,$DF,$FF,$B7,$DF,$AA,$B7
            .byte $DE,$AA,$F7,$FA,$AA,$F7,$FA,$95,$03,$EA,$95,$03,$EA,$AA,$03,$C0
            .byte $00,$00,$FF,$FF,$00,$C3,$0C,$95,$00,$00,$01,$00,$00,$02,$00,$00
            .byte $0A,$00,$00,$2A,$00,$00,$A2,$00,$02,$A6,$00,$03,$AA,$00,$03,$EA
            .byte $00,$00,$FA,$00,$00,$05,$00,$00,$0A,$00,$00,$28,$00,$00,$A3,$00
            .byte $02,$A7,$00,$0A,$A9,$00,$2A,$AA,$00,$15,$55,$00,$3F,$0F,$00,$2F
            .byte $05,$00,$2B,$0F,$00,$2B,$05,$95,$00,$3C,$2A,$00,$3E,$A9,$00,$3A
            .byte $A4,$00,$2A,$A4,$00,$2A,$A4,$00,$AA,$A9,$00,$AA,$AA,$00,$AA,$AA
            .byte $00,$AA,$AA,$00,$19,$65,$00,$19,$65,$00,$19,$65,$00,$AA,$AA,$00
            .byte $19,$65,$00,$19,$65,$00,$19,$65,$00,$AA,$AA,$00,$19,$65,$00,$19
            .byte $65,$00,$19,$65,$00,$AA,$AA,$95,$00,$2E,$EE,$00,$AE,$EE,$00,$AE
            .byte $EE,$00,$AD,$6E,$00,$A5,$5F,$02,$E5,$DF,$02,$E7,$DF,$02,$E7,$1F
            .byte $00,$FD,$7F,$02,$AF,$FF,$02,$AB,$FF,$02,$AA,$AA,$02,$AA,$AA,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$8D,$EE,$EC,$00,$EE,$EE,$00,$EE,$EE
            .byte $00,$EE,$2E,$00,$FF,$3E,$00,$FC,$4E,$00,$FC,$4E,$00,$FF,$3E,$00
            .byte $FF,$3C,$00,$FF,$FA,$00,$FF,$EA,$00,$AA,$AA,$00,$AA,$AA,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$8E,$00,$AA,$AA,$00,$23,$8E,$03,$EF
            .byte $BE,$03,$EF,$BE,$00,$AA,$AA,$00,$AA,$AA,$00,$23,$8E,$03,$EF,$BE
            .byte $03,$EF,$BE,$00,$AA,$AA,$02,$AA,$AA,$02,$23,$8E,$0E,$EF,$BE,$0E
            .byte $EF,$BE,$02,$AA,$AA,$02,$AA,$AA,$02,$23,$8E,$0E,$EF,$BE,$0E,$EF
            .byte $BE,$02,$AA,$AA,$02,$AA,$AA,$95,$05,$55,$55,$05,$55,$55,$05,$55
            .byte $55,$05,$55,$55,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$84,$00,$28,$00,$00,$A5,$00,$02,$A5
            .byte $40,$03,$A9,$55,$03,$AA,$55,$03,$EA,$AA,$00,$FE,$AA,$00,$3F,$FF
            .byte $00,$02,$AA,$00,$0A,$FF,$00,$2A,$EA,$00,$AA,$E2,$02,$AA,$E6,$03
            .byte $AA,$EA,$01,$EA,$FF,$02,$7E,$AA,$0A,$C3,$AA,$2B,$00,$EA,$AC,$01
            .byte $15,$5A,$A9,$40,$3F,$FD,$40,$93,$00,$10,$2A,$00,$04,$A9,$00,$01
            .byte $A4,$00,$02,$A4,$00,$03,$A9,$00,$00,$EA,$00,$00,$3F,$00,$00,$05
            .byte $00,$00,$2A,$00,$00,$05,$00,$00,$2A,$00,$00,$05,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$8C,$10,$00,$2A,$06,$A9,$AF,$07,$F6
            .byte $BA,$10,$0A,$EA,$00,$0E,$BF,$00,$03,$AA,$00,$00,$EA,$00,$00,$15
            .byte $00,$0A,$AA,$00,$2B,$FF,$00,$3A,$EA,$00,$2E,$BF,$00,$AB,$AA,$02
            .byte $BC,$FF,$0A,$C0,$15,$2B,$00,$00,$14,$00,$00,$3A,$00,$50,$0F,$A1
            .byte $00,$00,$F9,$50,$00,$0D,$50,$8F,$00,$00,$AA,$00,$0A,$0A,$00,$28
            .byte $AA,$00,$EA,$AA,$02,$FA,$AA,$0E,$FF,$AA,$0B,$BF,$FF,$3A,$C5,$FF
            .byte $2E,$00,$55,$2B,$00,$2A,$3A,$02,$8A,$2F,$0E,$AA,$2A,$0F,$EA,$3F
            .byte $01,$7F,$2A,$00,$15,$2F,$00,$2A,$3A,$00,$8A,$2B,$03,$AA,$2D,$03
            .byte $EA,$35,$40,$7F,$05,$50,$15,$95,$00,$02,$AA,$00,$0A,$AA,$00,$3A
            .byte $AA,$00,$3E,$AF,$00,$3F,$FF,$00,$07,$F5,$00,$01,$50,$00,$02,$B0
            .byte $00,$02,$B0,$00,$02,$B0,$00,$02,$B0,$00,$02,$B0,$00,$02,$B0,$00
            .byte $02,$B0,$00,$02,$B0,$00,$02,$B0,$00,$02,$B0,$00,$03,$F0,$00,$02
            .byte $A0,$00,$03,$F0,$00,$02,$A0,$95,$00,$02,$B0,$00,$02,$B0,$00,$02
            .byte $B0,$00,$02,$B0,$00,$02,$B0,$00,$02,$B0,$00,$02,$B0,$00,$02,$B0
            .byte $00,$02,$B0,$00,$02,$B0,$00,$02,$B0,$00,$03,$F0,$00,$00,$40,$00
            .byte $01,$50,$00,$01,$50,$00,$07,$74,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$90,$00,$00,$2A,$00,$00,$05,$00,$00
            .byte $2A,$00,$00,$05,$00,$00,$2A,$00,$00,$AA,$00,$02,$80,$00,$02,$95
            .byte $00,$03,$AA,$00,$00,$EA,$00,$00,$3F,$00,$00,$01,$00,$00,$01,$00
            .byte $00,$09,$00,$00,$05,$00,$00,$04,$00,$00,$08,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$92,$01,$42,$AA,$04,$52,$0A,$05,$52
            .byte $AA,$05,$52,$5A,$05,$52,$A8,$05,$5E,$A9,$05,$5E,$AA,$05,$53
L6017       .byte $FF
L6018       .byte $05,$50,$55,$05,$50,$00,$05,$50,$00,$01,$40,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$8C,$00,$00,$00,$00,$00,$00,$00,$50
            .byte $40,$01,$04,$10,$01,$01,$04,$03,$01,$01,$0E,$C0,$41,$0E,$C0,$41
            .byte $03,$00,$41,$00,$00,$4A,$00,$00,$A2,$00,$02,$A6,$00,$03,$AA,$00
            .byte $03,$FA,$00,$00,$FF,$00,$00,$0F,$00,$00,$02,$00,$00,$02,$00,$00
            .byte $02,$00,$00,$00,$00,$00,$00,$93,$00,$00,$11,$00,$00,$AA,$00,$02
            .byte $09,$00,$08,$24,$00,$08,$A4,$00,$2A,$A9,$00,$EA,$80,$00,$7A,$15
            .byte $00,$3A,$51,$00,$FE,$51,$00,$7F,$55,$00,$3F,$55,$00,$0F,$55,$00
            .byte $0F,$45,$00,$03,$05,$00,$00,$0D,$00,$00,$0F,$00,$00,$0F,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$92,$00,$00,$2A,$00,$00,$00,$00,$02
            .byte $AA,$00,$07,$66,$00,$03,$FF,$00,$00,$00,$00,$00,$FF,$00,$02,$AA
            .byte $00,$0A,$FF,$00,$2B,$AA,$00,$AE,$AA,$02,$BA,$FA,$06,$EA,$EE,$06
            .byte $EA,$EB,$26,$BA,$EA,$27,$AE,$EA,$29,$EB,$E5,$2B,$3A,$AE,$2B,$0E
            .byte $AE,$2B,$0E,$A0,$2B,$0E,$AA,$95,$2B,$0E,$AA,$2B,$0E,$AA,$2B,$03
            .byte $AF,$2B,$03,$AE,$2B,$03,$AE,$2B,$00,$AE,$2B,$00,$AE,$2B,$00,$AF
            .byte $05,$00,$AA,$0E,$80,$AA,$03,$A0,$AA,$00,$E8,$9A,$00,$3A,$6A,$00
            .byte $0E,$56,$00,$0B,$56,$00,$3A,$9A,$00,$3F,$AA,$00,$1F,$FA,$00,$07
            .byte $FF,$00,$01,$7F,$00,$00,$00,$90,$00,$00,$0A,$00,$00,$22,$00,$00
            .byte $82,$00,$00,$89,$00,$02,$A4,$00,$13,$A4,$00,$07,$A4,$00,$13,$E9
            .byte $00,$00,$FA,$00,$00,$FF,$00,$00,$3F,$00,$00,$0F,$00,$00,$01,$00
            .byte $00,$06,$00,$00,$01,$00,$00,$06,$00,$00,$01,$0A,$00,$0A,$20,$80
            .byte $22,$8A,$20,$82,$AA,$AF,$8A,$95,$A0,$AA,$AA,$A5,$BA,$EA,$EA,$BF
            .byte $FA,$FA,$F0,$FF,$3F,$C0,$3F,$0F,$00,$0F,$00,$00,$01,$00,$00,$06
            .byte $00,$00,$01,$00,$00,$06,$00,$00,$01,$00,$00,$0A,$00,$10,$28,$00
            .byte $04,$A9,$00,$01,$AA,$00,$00,$88,$00,$00,$D9,$00,$01,$FA,$00,$04
            .byte $FC,$00,$10,$3D,$00,$00,$0F,$95,$00,$00,$AA,$00,$02,$AA,$00,$0F
            .byte $FF,$00,$AA,$AA,$0A,$AA,$AA,$20,$AA,$FA,$82,$AA,$0A,$8A,$2A,$0A
            .byte $AA,$6A,$FA,$EF,$FE,$AA,$F5,$57,$AA,$F7,$77,$EA,$F5,$57,$FF,$37
            .byte $74,$FF,$05,$54,$00,$07,$74,$00,$04,$44,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$91,$00,$00,$03,$00,$00,$03,$00,$00
            .byte $03,$00,$00,$03,$00,$00,$0F,$00,$00,$0F,$00,$00,$0F,$00,$00,$0F
            .byte $00,$00,$0A,$00,$00,$AA,$00,$0A,$AA,$00,$20,$AA,$00,$82,$BE,$00
            .byte $8A,$EE,$02,$0B,$AE,$02,$2E,$AE,$02,$2E,$AE,$02,$BA,$AE,$0A,$BA
            .byte $AE,$0A,$BF,$FE,$0A,$AA,$AA,$95,$0A,$28,$82,$1A,$69,$96,$0A,$AA
            .byte $AA,$0A,$BF,$FE,$0A,$BA,$AE,$02,$BA,$AE,$02,$AE,$AE,$03,$AE,$AE
            .byte $03,$AB,$AE,$00,$EA,$EE,$00,$FA,$BE,$00,$3E,$AA,$00,$0F,$EA,$00
            .byte $03,$FF,$00,$00,$3F,$00,$00,$01,$00,$00,$01,$00,$00,$04,$00,$00
            .byte $10,$00,$00,$00,$00,$00,$00,$93,$00,$EA,$AA,$00,$3F,$FF,$00,$02
            .byte $C0,$00,$02,$C0,$00,$02,$C0,$00,$02,$C0,$00,$02,$C0,$00,$01,$40
            .byte $02,$AA,$AA,$02,$FD,$7F,$02,$C1,$40,$02,$C1,$00,$02,$C0,$00,$02
            .byte $C4,$00,$0A,$AA,$AA,$0B,$FF,$FF,$0B,$AA,$AA,$0B,$AA,$AA,$0B,$AA
            .byte $AA,$0B,$FF,$FF,$0A,$AA,$AA,$95,$00,$00,$0E,$00,$00,$3A,$00,$00
            .byte $3A,$40,$00,$AA,$44,$02,$22,$50,$02,$66,$60,$0B,$AA,$68,$2B,$EA
            .byte $3A,$AC,$3A,$0D,$B0,$3A,$01,$C0,$3F,$00,$00,$AA,$00,$02,$DF,$00
            .byte $0B,$DF,$00,$2F,$DF,$00,$BF,$DF,$02,$BF,$DF,$0A,$AA,$AA,$0E,$AA
            .byte $AA,$03,$AA,$AA,$00,$FF,$FF,$95,$00,$00,$0A,$00,$00,$AA,$00,$02
            .byte $0A,$00,$08,$2A,$00,$08,$AA,$00,$2A,$A0,$00,$28,$85,$00,$29,$91
            .byte $00,$38,$91,$00,$3D,$95,$00,$3F,$D5,$00,$3C,$D5,$00,$3D,$D4,$00
            .byte $3C,$C5,$00,$0D,$05
L632D       .byte $00,$00,$01,$00,$00,$3E,$00,$02,$AF,$00,$0A,$AB,$00,$2B,$AA,$00
            .byte $AE,$EA,$95,$00,$BA,$BA,$00,$AE,$EB,$00,$EB,$AE,$00,$BA,$BA,$00
            .byte $BE,$EA,$00,$B2,$BA,$00,$B2,$AE,$00,$B2,$2B,$00,$B2,$6A,$00,$B2
            .byte $AA,$00,$B3,$FF,$00,$B0,$55,$00,$30,$00,$03,$70,$00,$03,$70,$00
            .byte $03,$70,$00,$00,$C0,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$8C,$00,$2A,$AA,$00,$A8,$0A,$02,$AB,$BA,$02,$AB,$BA,$03
            .byte $A9,$5A,$00,$EA,$AA,$00,$3A,$A0,$00,$0E,$AE,$00,$03,$A5,$00,$00
            .byte $EA,$00,$00,$3A,$02,$20,$0F,$02,$60,$01,$00,$40,$0A,$00,$40,$01
            .byte $02,$AA,$AA,$0A,$BF,$FF,$2A,$EA,$AA,$3A,$BA,$AA,$0E,$AE,$AA,$0B
            .byte $AB,$AA,$95,$0B,$EA,$EA,$0B,$3A,$BF,$0B,$0E,$AA,$0B,$03,$FF,$0B
            .byte $00,$05,$0B,$00,$2A,$0B,$00,$05,$0B,$00,$2A,$0B,$00,$05,$0B,$00
            .byte $2A,$0B,$00,$05,$0B,$00,$2A,$0B,$00,$05,$0B,$00,$00,$0B,$00,$00
            .byte $05,$00,$00,$0A,$85,$00,$0E,$94,$00,$03,$D5,$40,$03,$D5,$40,$00
            .byte $D5,$00,$8D,$00,$02,$AA,$00,$0A,$FF,$00,$2B,$AA,$00,$3A,$EA,$00
            .byte $0E,$BA,$00,$0B,$AF,$00,$0A,$EA,$00,$0A,$FF,$00,$0A,$C0,$00,$0A
            .byte $C0,$00,$0A,$C0,$00,$0A,$C0,$00,$0A,$C0,$00,$0A,$C0,$00,$0A,$C0
            .byte $00,$0A,$C0,$00,$0A,$C0,$00,$0A,$C0,$00,$0A,$C0,$00,$0A,$C0,$00
            .byte $0A,$C0,$95,$00,$0A,$C0,$00,$05,$00,$00,$0B,$00,$00,$0B,$00,$00
            .byte $0B,$00,$00,$0B,$00,$00,$0B,$00,$00,$0B,$00,$00,$0B,$00,$00,$0B
            .byte $00,$00,$0B,$00,$00,$0B,$00,$00,$0B,$00,$00,$0B,$00,$00,$0B,$00
            .byte $00,$0B,$00,$00,$05,$00,$00,$0B,$00,$00,$2B,$C0,$00,$2B,$C0,$00
            .byte $11,$40,$95,$00,$02,$AA,$00,$0A,$AA,$00,$0A,$0A,$00,$0E,$8A,$00
            .byte $0F,$AA,$00,$0F,$EA,$00,$0F,$FF,$00,$0F,$D5,$00,$0F,$55,$00,$0D
            .byte $57,$00,$05,$5E,$00,$01,$7A,$00,$00,$7A,$00,$00,$3A,$00,$00,$3A
            .byte $00,$00,$3A,$00,$AA,$BA,$02,$AA,$BA,$02,$AA,$BA,$03,$AA,$AE,$03
            .byte $EA,$AB,$95,$03,$FA,$AA,$01,$7E,$AA,$0E,$9F,$FF,$0E,$9F,$33,$0E
            .byte $9F,$77,$0E,$9F,$FF,$0E,$9F,$3F,$0E,$BF,$7F,$0E,$BF,$FF,$0E,$BF
            .byte $55,$0E,$BD,$55,$0E,$B5,$57,$0E,$95,$5F,$0E,$95,$7E,$0E,$95,$7E
            .byte $0E,$80,$3A,$0E,$80,$3A,$0E,$80,$3A,$0E,$80,$3A,$0E,$8A,$BA,$0E
            .byte $A2,$AE,$95,$0E,$B6,$2B,$01,$7E,$62,$0E,$BF,$A6,$0E,$BF,$FF,$0E
            .byte $BF,$55,$0E,$BD,$55,$0E,$B5,$F5,$0E,$97,$FD,$0E,$83,$FC,$0E,$83
            .byte $BC,$0E,$83,$AC,$0E,$83,$AC,$0E,$83,$AC,$0E,$83,$AC,$0E,$83,$AC
            .byte $0E,$83,$AC,$0E,$43,$AC,$0D,$43,$AC,$0D,$53,$AC,$01,$55,$AC,$00
            .byte $15,$AC,$95,$00,$03,$AC,$00,$03,$AC,$00,$00,$50,$00,$03,$A0,$00
            .byte $03,$A0,$00,$03,$A0,$00,$03,$A0,$00,$03,$A0,$00,$03,$A0,$00,$03
            .byte $A0,$00,$03,$A0,$00,$03,$A0,$00,$03,$A0,$00,$03,$A0,$00,$03,$A0
            .byte $00,$03,$A0,$00,$03,$A0,$00,$2B,$B8,$00,$3A,$EC,$00,$3E,$BC,$00
            .byte $05,$50,$95,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$40,$00,$00,$19,$00,$00,$7C,$00,$00,$78,$00,$00,$38
            .byte $00,$00,$0D,$00,$00,$20,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$20,$00,$00
            .byte $20,$00,$00,$0C,$80,$00,$9C,$00,$00,$7E,$00,$00,$FE,$00,$00,$7C
            .byte $80,$00,$2C,$00,$00,$06,$00,$00,$40,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$40,$00,$00,$C0,$00,$00,$40,$20,$00
            .byte $0A,$00,$00,$1C,$00,$02,$7E,$00,$00,$7E,$00,$00,$FF,$00,$00,$7E
            .byte $00,$00,$38,$20,$00,$67,$00,$00,$01,$80,$00,$00,$00,$01,$80,$00
            .byte $03,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
L6639       .byte $00,$00,$00,$00,$00,$00,$00,$03,$00,$00,$01,$00,$08,$00,$00,$80
            .byte $00,$06,$00,$00,$0C,$00,$20,$10,$00,$00,$4C,$00,$00,$6E,$00,$00
            .byte $BA,$00,$00,$6C,$00,$00,$30,$04,$00,$01,$00,$01,$00,$C0,$00,$00
            .byte $60,$00,$00,$00,$00,$00,$00,$00,$00,$00,$02,$00,$00,$04,$00,$00
            .byte $08,$00,$00,$00,$00,$00,$00,$01,$00,$40,$00,$02,$00,$00,$06,$00
            .byte $00,$04,$00,$00,$01,$00,$00,$10,$00,$00,$48,$00,$00,$64,$00,$08
            .byte $1A,$00,$00,$64,$00,$00,$30,$01,$00,$00,$00,$00,$00,$00,$00,$04
            .byte $00,$04,$00,$00,$00,$00,$00,$00,$00,$18,$00,$00,$04,$00,$00,$00
            .byte $00,$00,$00,$04,$00,$00,$00,$00,$02,$00,$00,$04,$00,$00,$00,$20
            .byte $00,$00,$00,$00,$08,$00,$00,$00,$00,$00,$40,$00,$00,$24,$00,$00
            .byte $10,$00,$40,$44,$00,$00,$10,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$01,$00,$00,$00,$00,$10,$00,$00,$00,$00,$01
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$04,$00,$00,$00,$00,$00,$00,$00,$00,$82,$00,$00
            .byte $10,$00,$00,$00,$00,$00,$02,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$80
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$F0,$00,$03,$AC,$00,$00,$EB,$00,$03,$9A,$C0,$0E,$AA,$C0,$03
            .byte $9B,$00,$0E,$6B,$00,$0E,$9C,$00,$03,$AB,$00,$03,$AC,$00,$00,$F0
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$C0,$00,$0F,$B0,$00
            .byte $0E,$BF,$00,$0E,$AA,$C0,$03,$AA,$B0,$0E,$9A,$F0,$0E,$A6,$C0,$0E
            .byte $6A,$B0,$3A,$A6,$C0,$3A,$AA,$C0,$0E,$99,$B0,$03,$AA,$B0,$0E,$BB
            .byte $C0,$0E,$CC,$00,$03,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$0F,$00,$30,$3A,$C0,$EC,$39,$B3,$B0
            .byte $3A,$BE,$AC,$3E,$AA,$6C,$0F,$9A,$AC,$0E,$AA,$F0,$0E,$A6,$C0,$0E
            .byte $69,$B0,$FA,$AA,$C0,$EA,$66,$F0,$3E,$AA,$AC,$03,$6A,$AB,$3E,$BB
            .byte $EB,$2A,$CC,$EC,$E6,$C0,$3C,$EB,$00,$00,$3C,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$3F,$00,$3C,$EA,$C0,$EF,$E9,$B3,$AB
            .byte $E6,$BF,$6F,$3A,$FB,$AC,$3F,$FA,$EC,$03,$AA,$F0,$0E,$9A,$C0,$0D
            .byte $A9,$B0,$0E,$AA,$C0,$3A,$9A,$F0,$0E,$A6,$B0,$03,$6A,$AC,$0E,$EA
            .byte $6C,$3A,$BE,$9C,$EA,$B3,$AB,$E6,$B0,$DB,$EA,$C0,$EB,$3B,$00,$3C
            .byte $0C,$00,$0C,$00,$00,$00,$00,$3C,$00,$3C,$FB,$30,$EF,$EA,$C0,$EB
            .byte $3A,$BC,$EC,$0E,$FF,$BC,$03,$FB,$C0,$03,$AA,$F0,$0E,$EE,$C0,$03
            .byte $AB,$00,$0E,$BA,$C0,$CE,$AB,$F0,$0E,$EE,$B0,$03,$AB,$AC,$03,$AB
            .byte $EC,$0E,$FE,$AC,$3A,$B3,$BB,$36,$C0,$EB,$3B,$00,$EB,$0C,$00,$3C
            .byte $00,$00,$0C,$00,$00,$00,$00,$00,$00,$0C,$0F,$30,$3C,$3A,$C0,$3B
            .byte $3A,$B0,$3B,$0F,$FC,$0C,$00,$3B,$00,$03,$EB,$F0,$0E,$EE,$C0,$03
            .byte $AB,$00,$03,$BA,$C0,$00,$EB,$C0,$00,$EE,$CC,$03,$B3,$80,$03,$C3
            .byte $F0,$0E,$C0,$EC,$3A,$C0,$FC,$3A,$C0,$EB,$3B,$00,$FB,$0C,$00,$FF
            .byte $00,$00,$3C,$00,$00,$0C,$00,$00,$00,$00,$03,$30,$0C,$0E,$C0,$3B
            .byte $0E,$C0,$0F,$03,$0C,$0C,$00,$3F,$00,$00,$EB,$03,$03,$EA,$C0,$03
            .byte $BB,$00,$03,$AE,$C0,$00,$EB,$C0,$00,$EE,$C0,$00,$33,$03,$03,$03
            .byte $F0,$0E,$C0,$EC,$0E,$C0,$3C,$3A,$C0,$3B,$0F,$00,$3F,$00,$00,$3C
            .byte $00,$00,$0C,$00,$00,$00,$00,$00,$00,$00,$03,$00,$00,$03,$C0,$0C
            .byte $03
L690A       .byte $00,$0C,$00,$0C,$00,$00,$3C,$00,$00,$EB,$00,$03,$FF,$00,$03,$BB
            .byte $00,$00,$EE,$C0,$00,$EB,$00,$00,$3F,$00,$00,$03,$00,$00,$00,$F0
            .byte $03,$C0,$30,$0E,$C0,$3C,$0E,$C0,$3C,$03,$00,$0C,$00,$00,$0C,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$03,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$3C,$00,$00,$EC,$00,$00,$FF,$00,$00,$FB
            .byte $00,$00,$EF,$00,$00,$3F,$00,$00,$3C,$00,$00,$00,$00,$00,$00,$30
            .byte $03,$00,$30,$0E,$C0,$3C,$03,$00,$0C,$00,$00,$0C,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$0C,$00,$00,$3C,$00,$00,$EF,$00,$00,$EB
            .byte $00,$00,$3F,$00,$00,$3C,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$30,$03,$00,$3C,$00,$00,$0C,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$0C,$00,$00,$3C,$00,$00,$3C
            .byte $00,$00,$3C,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$03,$CF,$00,$07,$FF,$C0,$01,$F3,$80,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$01,$E7,$00
            .byte $07,$FF,$C0,$03,$9F,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$38,$00,$00,$38,$00,$03,$FF,$80,$0F
            .byte $FF,$E0,$3F,$FF,$F8,$00,$00,$00,$00,$FC,$00,$00,$FE,$00,$00,$0E
            .byte $00,$00,$3E,$00,$00,$3C,$00,$00,$00,$00,$00,$38,$00,$00,$38,$00
            .byte $00,$00,$00,$3F,$FF,$F8,$0F,$FF,$E0,$03,$FF,$80,$00,$10,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$60,$00,$00,$70,$00,$00
            .byte $7C,$00,$00,$3E,$00,$00,$1E,$00,$00,$1E,$00,$30,$0F,$00,$38,$07
            .byte $C0,$3C,$03,$E0,$1E,$00,$F0,$1F,$00,$70,$0F,$80,$78,$03,$80,$38
            .byte $01,$C0,$0C,$00,$E0,$00,$00,$F8,$00,$00,$7C,$00,$00,$3E,$00,$00
            .byte $07,$00,$00,$00,$00,$00,$00,$FC,$00,$0E,$FF,$00,$3F,$CF,$80,$7F
            .byte $FF,$80,$D8,$3F,$C0,$DB,$A0,$F0,$D8,$2E,$F0,$7F,$E6,$30,$3F,$F7
            .byte $BC,$0C,$16,$9E,$0D,$D6,$57,$0C,$16,$9E,$3F,$F7,$BC,$7F,$E6,$30
            .byte $D8,$2E,$F0,$DB,$A0,$F0,$D8,$3F,$C0,$7F,$FF,$80,$3F,$CF,$80,$0E
            .byte $FF,$00,$00,$FC,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$01,$81,$80,$03,$81,$C0,$03,$C3,$C0,$03,$C3,$C0,$03,$C3
            .byte $C0,$01,$C1,$80,$01,$C1,$80,$03,$81,$C0,$03,$83,$C0,$03,$C3,$C0
            .byte $03,$C3,$80,$01,$C1,$80,$01,$81,$80,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$01,$83,$00,$03,$EF,$D0,$0F,$FF,$BC,$3E,$03
            .byte $AE,$70,$FD,$AB,$D6,$FE,$AE,$D6,$FE,$BC,$D6,$FE,$B0,$70,$FE,$80
            .byte $3E,$FE,$80,$00,$FE,$00,$00,$F8,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$06,$00,$00,$0E,$00,$00
            .byte $3C,$00,$00,$78,$00,$00,$F8,$00,$00,$F0,$00,$01,$F0,$0C,$03,$E0
            .byte $1C,$0F,$80,$3C,$1F,$00,$78,$1F,$00,$F8,$1E,$01,$F0,$38,$01,$C0
            .byte $30,$03,$80,$00,$07,$00,$00,$1F,$00,$00,$3E,$00,$00,$7C,$00,$00
            .byte $E0,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$40,$00,$00,$41,$80,$00,$41,$80,$00,$C1,$81,$80,$59,$81
            .byte $80,$59,$81,$80,$59,$81,$80,$D9,$81,$80,$59,$99,$80,$59,$99,$80
            .byte $59,$99,$80,$D9,$99,$80,$59,$99,$80,$59,$99,$80,$59,$99,$80,$FF
            .byte $FF,$E0,$44,$44,$40,$FF,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$19,$C0
            .byte $F0,$FF,$FF,$FF,$0F,$E6,$38,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$1F,$1E
            .byte $7C,$FF,$FF,$FF,$3C,$C1,$F8,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$20,$00,$00,$18,$00,$00,$0F
            .byte $00,$00,$0F,$80,$00,$07,$80,$00,$03,$C0,$00,$03,$E0,$00,$01,$F8
            .byte $00,$00,$FE,$00,$00,$1E,$00,$00,$0F,$00,$00,$07,$00,$00,$07,$80
            .byte $00,$07,$80,$00,$01,$E0,$00,$00,$E0,$00,$00,$30,$00,$00,$18,$00
            .byte $00,$08,$00,$00,$00,$00,$00,$00,$00,$20,$00,$00,$18,$00,$00,$1E
            .byte $00,$00,$07,$00,$00,$03,$80,$00,$03,$E0,$00,$01,$F0,$00,$00,$FC
            .byte $00,$00,$7C,$00,$00,$3C,$00,$00,$3E,$00,$00,$1F,$80,$00,$1F,$C0
            .byte $00,$0F,$E0,$00,$07,$E0,$00,$01,$F0,$00,$00,$78,$00,$00,$18,$00
            .byte $00,$0C,$00,$00,$06,$00,$00,$08,$00,$00,$08,$00,$00,$18,$00,$00
            .byte $1C,$00,$00,$3C,$00,$00,$3C,$00,$00,$3C,$00,$00,$38,$00,$00,$78
            .byte $00,$00,$78,$00,$00,$78,$00,$00,$3C,$00,$00,$3C,$00,$00,$3C,$00
            .byte $00,$3E,$00,$00,$1E,$00,$00,$0E,$00,$00,$0C,$00,$00,$1C,$00,$00
            .byte $18,$00,$00,$18,$00,$00,$00,$10,$00,$00,$18,$00,$00,$18,$00,$00
            .byte $38,$00,$00,$3C,$00,$00,$3C,$00,$00,$3C,$00,$00,$3E,$00,$00,$1E
            .byte $00,$00,$1E,$00,$00,$1E,$00,$00,$3E,$00,$00,$3C,$00,$00,$3C,$00
            .byte $00,$3C,$00,$00,$18,$00,$00,$08,$00,$00,$08,$00,$00,$18,$00,$00
            .byte $18,$00,$00,$08,$00,$00,$00,$00,$06,$00,$00,$1C,$00,$00,$78,$00
            .byte $00,$F0,$00,$00,$F0,$00,$01,$F0,$00,$01,$E0,$00,$03,$80,$00,$07
            .byte $00,$00,$1F,$00,$00,$3E,$00,$00,$7E,$00,$00,$F8,$00,$01,$E0,$00
            .byte $07,$C0,$00,$0F,$00,$00,$0E,$00,$00,$1C,$00,$00,$1C,$00,$00,$38
            .byte $00,$00,$20,$00,$00,$00,$00,$00,$06,$00,$00,$1C,$00,$00,$3C,$00
            .byte $00,$F8,$00,$01,$F0,$00,$01,$F0,$00,$03,$E0,$00,$07,$C0,$00,$1F
            .byte $00,$00,$3F,$00,$00,$7E,$00,$00,$7C,$00,$00,$F8,$00,$01,$E0,$00
            .byte $01,$C0,$00,$03,$C0,$00,$0F,$80,$00,$1F,$00,$00,$1E,$00,$00,$38
            .byte $00,$00,$20,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $F1,$80,$0F,$FF,$F8,$FF,$FF,$FF,$33,$CF,$F0,$01,$81,$C0,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$01,$C6,$00
            .byte $1F,$FF,$F0,$FF,$FF,$FF,$0F,$FC,$78,$00,$E0,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $3E,$00,$3F,$FF,$E0,$FF,$FF,$FF,$07,$F9,$FC,$00,$60,$30,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$39,$80
            .byte $07,$FF,$F8,$FF,$FF,$FF,$3E,$7F,$F0,$00,$1E,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$01,$80,$00,$00,$C0,$00,$00,$F0,$00,$00
            .byte $7C,$00,$00,$1E,$00,$C0,$1F,$00,$60,$0F,$00,$38,$07,$80,$3E,$03
            .byte $C0,$1F,$03,$C0,$1F,$83,$F0,$0F,$81,$F8,$07,$C0,$78,$01,$C0,$1C
            .byte $00,$60,$0E,$00,$78,$03,$00,$3E,$00,$00,$3E,$00,$00,$0F,$00,$00
            .byte $03,$00,$00,$01,$80,$00,$01,$80,$00,$00,$C0,$00,$00,$F0,$00,$00
            .byte $78,$00,$00,$3C,$00,$C0,$1E,$00,$70,$1F,$00,$38,$0F,$C0,$3C,$07
            .byte $E0,$1E,$03,$F0,$07,$80,$F8,$03,$C0,$78,$01,$C0,$7C,$01,$E0,$1C
            .byte $01,$E0,$0E,$00,$F0,$03,$00,$7C,$00,$00,$3E,$00,$00,$0E,$00,$00
            .byte $03,$00,$00,$01,$80,$00,$04,$00,$40,$06,$00,$C0,$06,$00,$C0,$0E
            .byte $01,$C0,$0E,$01,$E0,$1F,$01,$E0,$0F,$01,$E0,$0F,$01,$E0,$07,$00
            .byte $F0
L6F1B       .byte $07,$00,$F0,$07,$00,$F0,$0F,$80,$F0,$0F,$80,$F0,$0F,$81,$E0,$1F
            .byte $01,$E0,$1F,$01,$E0,$0E,$01,$E0,$0E,$00,$C0,$06,$00,$C0,$06,$00
            .byte $40,$02,$00,$40,$00,$02,$00,$80,$06,$00,$C0,$06,$00,$C0,$07,$01
            .byte $C0,$07,$01,$E0,$07,$00,$E0,$0F,$00,$E0,$0F,$81,$E0,$07,$81,$E0
            .byte $07,$81,$C0,$07,$81,$C0,$0F,$03,$C0,$0F,$03,$C0,$0E,$03,$E0,$0E
            .byte $01,$E0,$0E,$01,$C0,$06,$01,$C0,$07,$00,$C0,$07,$00,$C0,$06,$00
            .byte $80,$02,$00,$80,$00,$00,$01,$80,$00,$03,$00,$00,$0F,$00,$00,$3E
            .byte $00,$00,$3E,$00,$00,$7C,$03,$00,$F8,$06,$01,$E0,$1C,$03,$C0,$3C
            .byte $03,$C0,$F8,$0F,$C1,$F8,$1F,$83,$F0,$1E,$03,$E0,$38,$07,$C0,$70
            .byte $1F,$80,$C0,$1E,$00,$80,$38,$00,$00,$78,$00,$00,$F0,$00,$00,$C0
            .byte $00,$01,$80,$00,$00,$00,$01,$80,$00,$03,$00,$00,$0F,$00,$00,$3E
            .byte $00,$00,$7C,$00,$00,$7C,$03,$00,$F8,$0E,$03,$E0,$1C,$07,$C0,$7C
            .byte $0F,$80,$F8,$1F,$01,$F8,$1E,$01,$F8,$3E,$03,$E0,$38,$07,$C0,$70
            .byte $07,$80,$C0,$0F,$00,$00,$3E,$00,$00,$7C,$00,$00,$70,$00,$00,$C0
            .byte $00,$01,$80,$00,$00,$00,$00,$00,$00,$38,$7C,$EE,$C6,$00,$00,$00
            .byte $00,$3C,$3C,$0C,$0C,$00,$00,$00,$00,$F8,$FC,$0E,$06,$00,$00,$F8
            .byte $FC,$0E,$06,$0E,$FC,$00,$00,$00,$00,$06,$0E,$1C,$38,$00,$00,$00
            .byte $00,$FE,$FE,$C0,$F8,$00,$00,$0E,$1C,$38,$7C,$EE,$C6,$00,$00,$00
            .byte $00,$FE,$FE,$1C,$1C,$00,$00,$38,$7C,$EE,$C6,$EE,$7C,$00,$00,$00
            .byte $00,$38,$7C,$EE,$C6,$00,$00,$00,$00,$3E,$7E,$E6,$C6,$00,$00,$C0
            .byte $C0,$D8,$DC,$CE,$C6,$00,$00,$00,$00,$3E,$7E,$E0,$C0,$00,$00,$06
            .byte $06,$36,$76,$E6,$C6,$00,$00,$00,$00,$3E,$7E,$E6,$CC,$00,$00,$1E
            .byte $3E,$70,$60,$60,$7E,$00,$00,$00,$00,$3E,$7E,$E6,$C6,$00,$00,$C0
            .byte $C0,$F8,$FC,$CE,$C6,$00,$00,$18,$18,$00,$18,$18,$18,$00,$00,$06
            .byte $06,$00,$06,$06,$06,$00,$00,$C0,$C0,$C6,$CE,$FC,$F8,$00,$00,$18
            .byte $18,$18,$18,$18,$18,$00,$00,$0C,$0C,$0C,$0C,$0C,$0C,$00,$00,$00
            .byte $00,$F8,$FC,$CE,$C6,$00,$00,$00,$00,$38,$7C,$EE,$C6,$00,$00,$00
            .byte $00,$F8,$FC,$CE,$C6,$00,$00,$00,$00,$3E,$7E,$E6,$C6,$00,$00,$00
            .byte $00,$1E,$3E,$70,$60,$00,$00,$00,$00,$60,$60,$70,$38,$00,$00,$60
            .byte $60,$7E,$7E,$60,$60,$00,$00,$00,$00,$C6,$C6,$C6,$C6,$00,$00,$00
            .byte $00,$D8,$DC,$CE,$66,$00,$00,$00,$3C,$42,$9D,$A1,$A1,$00,$00,$00
            .byte $00,$C7,$EF,$3C,$18,$00,$00,$00,$00,$D8,$DC,$CE,$66,$00,$00,$00
            .byte $00,$FE,$FE,$0E,$38,$00,$00,$70,$78,$1C,$0C,$1C,$78,$00,$00,$70
            .byte $70,$70,$70,$70,$70,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$66,$66,$66,$66,$66,$66,$66,$66,$66,$66,$66
            .byte $66,$66,$66,$66,$66,$66,$66,$66,$66,$66,$66,$66,$66,$66,$66,$66
            .byte $66,$66,$66,$66,$66,$66,$66,$66,$66,$66,$66,$66,$66,$00,$00,$70
            .byte $70,$30,$60,$00,$00,$00,$00,$00,$00,$00,$00,$00,$F6,$00,$00,$00
            .byte $00,$70,$70,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$7F,$78,$78
            .byte $78,$78,$78,$7F,$78,$FF,$00,$00,$00,$00,$00,$FF,$00,$55,$55,$55
            .byte $55,$55,$55,$55,$55,$FF,$00,$00,$00,$06,$06,$FF,$06,$55,$55,$55
            .byte $55,$55,$55,$55,$55,$55,$55,$55,$55,$55,$55,$55,$55,$F0,$30,$30
            .byte $30,$30,$30,$F0,$30,$55,$55,$55,$55,$55,$55,$55,$55,$55,$55,$55
            .byte $55,$55,$55,$55,$55,$00,$00,$03,$07,$0E,$0C,$0C,$0C,$00,$00,$0F
            .byte $0F,$0C,$0C,$0D,$0D,$00,$00,$01,$03,$07,$0E,$0C,$0C,$00,$00,$0F
            .byte $0F,$0C,$0C,$0C,$0C,$00,$00,$03,$07,$0E,$0E,$07,$07,$00,$00,$01
            .byte $03,$07,$0E,$0C,$0F,$00,$00,$01,$03,$07,$0E,$0C,$0C,$00,$00,$0C
            .byte $0C,$0C,$0C,$0D,$0D,$00,$00,$00,$00,$3E,$3F,$33,$31,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$0C,$0C,$0C,$0C,$0F,$0F,$00,$00,$01
            .byte $01,$01,$01,$01,$01,$00,$00,$3E,$3F,$33,$31,$31,$31,$00,$00,$0F
            .byte $0F,$0C,$0C,$0C,$0C,$00,$00,$01,$03,$07,$0E,$0C,$0C,$00,$00,$0F
            .byte $0F,$0C,$0C,$0C,$0C,$00,$00,$01,$03,$07,$0E,$0C,$0C,$00,$00,$0F
            .byte $0F,$0C,$0C,$0C,$0C,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$0F
            .byte $0F,$00,$00,$00,$00,$00,$00,$0C,$0C,$0C,$0C,$0C,$0C,$00,$00,$0C
            .byte $0C,$0E,$06,$07,$03,$00,$00,$1D,$3D,$71,$61,$61,$61,$00,$00,$0E
            .byte $0F,$03,$01,$00,$00,$00,$00,$0C,$0C,$0E,$06,$07,$03,$00,$00,$0F
            .byte $0F,$00,$00,$00,$01,$00,$00,$00,$00,$1D,$3D,$71,$61,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$55,$55,$55,$55,$55,$55,$55,$55,$55,$55,$55
            .byte $55,$55,$55,$55,$55,$3C,$1E,$1F,$0F,$83,$C0,$F0,$FE,$00,$00,$00
            .byte $00,$00,$FF,$00,$00,$00,$00,$F0,$F8,$1C,$0C,$0C,$0C,$00,$00,$F0
            .byte $F8,$1C,$1C,$F8,$F8,$00,$00,$FC,$FC,$00,$00,$00,$00,$00,$00,$E0
            .byte $F0,$38,$1C,$0C,$0C,$00,$00,$FC,$FC,$00,$00,$FC,$FC,$00,$00,$FC
            .byte $FC,$00,$00,$00,$FC,$00,$00,$FC,$FC,$00
L7305       .byte $0C,$0C,$0C,$00,$00,$0C,$0C,$0C,$0C,$FC,$FC,$00,$00,$00,$00,$70
            .byte $78,$9C,$8C,$00,$00,$0C,$0C,$0C,$0C,$0C,$0C,$00,$00,$0C,$0C,$1C
            .byte $38,$F0,$F0,$00,$00,$80,$80,$80,$80,$80,$80,$00,$00,$70,$78,$9C
            .byte $8C,$8C,$8C,$00,$00,$F0,$F8,$1C,$0C,$0C,$0C,$00,$00,$E0,$F0,$38
            .byte $1C,$0C,$0C,$00,$00,$E0,$F0,$38,$1C,$0C,$0C,$00,$00,$E0,$F0,$38
            .byte $1C,$0C,$0C,$00,$00,$F0,$F8,$1C,$0C,$0C,$1C,$00,$00,$C0,$C0,$E0
            .byte $70,$38,$1C,$00,$00,$FC,$FC,$C0,$C0,$C0,$C0,$00,$00,$0C,$0C,$0C
            .byte $0C,$0C,$0C,$00,$00,$F0,$F8,$1C,$0C,$1C,$18,$00,$00,$B8,$BC,$8E
            .byte $86,$86,$86,$00,$00,$1C,$3C,$F0,$E0,$C0,$C0,$00,$00,$F0,$F8,$1C
            .byte $0C,$1C,$18,$00,$00,$FC,$FC,$38,$70,$E0,$C0,$00,$00,$00,$00,$B8
            .byte $BC,$8E,$86,$55,$55,$55,$55,$55,$55,$55,$55,$55,$55,$55,$55,$55
            .byte $55,$55,$55,$55,$55,$55,$55,$55,$55,$55,$55,$00,$00,$00,$C0,$F8
            .byte $7F,$00,$00,$55,$55,$55,$55,$55,$55,$55,$55,$00,$00,$00,$03,$0F
            .byte $FE,$00,$00,$3C,$78,$F8,$F0,$C1,$03,$0F,$7F,$3C,$3C,$3C,$3C,$3C
            .byte $3C,$3C,$3C,$55,$55,$55,$55,$55,$55,$55,$55,$55,$55,$55,$55,$55
            .byte $55,$55,$55,$00,$00,$00,$00,$00,$E0,$E7,$0E,$C6,$EE,$7C,$38,$00
            .byte $00,$00,$00,$0C,$0C,$0C,$0C,$00,$00,$00,$00,$0E,$1C
L7412       .byte $38,$70,$FE,$FE,$00,$00,$FC,$0E,$06,$0E,$FC,$F8,$00,$00,$70,$E0
            .byte $FE,$FE,$06,$06,$00,$00,$FC,$0E,$06,$0E,$FC,$F8,$00,$00,$C6,$EE
            .byte $7C,$38,$00,$00,$00,$00,$38,$38,$70,$70,$E0,$E0,$00,$00,$7C,$EE
            .byte $C6,$EE,$7C,$38,$00,$00,$C6,$EE,$7C,$38,$70,$E0,$00,$00,$C6,$E6
            .byte $76,$36,$00,$00,$00,$00,$C6,$CE,$FC,$F8,$00,$00,$00,$00,$C0,$E0
            .byte $7E,$3E,$00,$00,$00,$00,$C6,$E6,$7E,$3E,$00,$00,$00,$00,$D8,$E0
            .byte $7E,$3E,$00,$00,$00,$00,$7E,$60,$60,$60,$00,$00,$00,$00,$E6,$76
            .byte $36,$06,$3E,$3C,$00,$00,$C6,$C6,$C6,$C6,$00,$00,$00,$00,$18,$18
            .byte $18,$18,$00,$00,$00,$00,$06,$06,$06,$0E,$7C,$78,$00,$00,$DC,$CE
            .byte $C6,$C6,$00,$00,$00,$00,$18,$18,$18,$18,$00,$00,$00,$00,$0C,$0C
            .byte $0C,$0C,$00,$00,$00,$00,$C6,$C6,$C6,$C6,$00,$00,$00,$00,$C6,$EE
            .byte $7C,$38,$00,$00,$00,$00,$C6,$CE,$DC,$D8,$C0,$C0,$00,$00,$C6,$E6
            .byte $76,$36,$06,$06,$00,$00,$60,$60,$60,$60,$00,$00,$00,$00,$1C,$0E
            .byte $06,$0E,$7C,$78,$00,$00,$60,$70,$3E,$1E,$00,$00,$00,$00,$C6,$E6
            .byte $7E,$3E,$00,$00,$00,$00,$66,$6E,$3C,$38,$00,$00,$00,$00,$9D,$42
            .byte $3C,$00,$00,$00,$00,$00,$18,$3C,$EF,$C7,$00,$00,$00,$00,$66,$6E
            .byte $3C,$38,$70,$E0,$00,$00,$38,$E0,$FE,$FE,$00,$00,$00,$00,$70,$00
            .byte $70,$70,$00,$00,$00,$00,$70,$00,$70,$70,$00,$00,$00,$00,$00,$00
            .byte $70,$70,$00,$00,$00,$00,$00,$00,$70,$70,$30,$60,$00,$00,$55,$55
            .byte $55,$55,$55,$55,$55,$55,$55,$55,$55,$55,$55,$55,$55,$55,$55,$55
            .byte $55,$55,$55,$55,$55,$55,$55,$55,$55,$55,$55,$55,$55,$55,$55,$55
            .byte $55,$55,$55,$55,$55,$55,$00,$00,$00,$00,$00,$00,$00,$00,$FE,$DE
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$70,$70,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$78,$78,$78,$7C,$7C,$7C,$7C,$00,$FC,$CC
            .byte $0C,$FE,$CE,$CE,$FE,$00,$DE,$F6,$60,$70,$70,$70,$70,$00,$FE,$C6
            .byte $C6,$E7,$E7,$E7,$FF,$00,$FE,$C6,$C6,$E7,$E7,$E7,$FF,$00,$67,$06
            .byte $66,$77,$77,$77,$77,$00,$F0,$30,$30,$38,$38,$38,$FB,$00,$55,$55
            .byte $55,$55,$55,$55,$55,$55,$55,$55,$55,$55,$55,$55,$55,$55,$0F,$0F
            .byte $00,$00,$00,$00,$00,$00,$0C,$0C,$0C,$0C,$03,$03,$00,$00,$0E,$07
            .byte $03,$01,$00,$00,$00,$00,$0C,$0C,$0D,$0D,$00,$00,$00,$00,$0E,$0C
            .byte $0C,$0E,$07,$03,$00,$00,$0F,$0C,$0C,$0C,$0C,$0C,$00,$00,$0E,$07
            .byte $03,$01,$00,$00,$00,$00,$0C,$0C,$0C,$0C,$00,$00,$00,$00,$31,$31
            .byte $31,$31,$00,$00,$00,$00,$00,$00,$01,$01,$00,$00,$00,$00,$0C,$0C
            .byte $0C,$0C,$00,$00,$00,$00,$01,$01,$01,$01,$00,$00,$00,$00,$31,$31
            .byte $31,$31,$00,$00,$00,$00,$0C,$0C,$0C,$0C,$00,$00,$00,$00,$0E,$07
            .byte $03,$01,$00,$00,$00,$00,$0C,$0C,$0D,$0D,$0C,$0C,$00,$00,$0E,$07
            .byte $03,$01,$00,$00,$00,$00,$0D,$0D,$0C,$0C,$00,$00,$00,$00,$00,$00
            .byte $01,$01,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$0C,$0E
            .byte $07,$03,$00,$00,$00,$00,$03,$01,$01,$00,$00,$00,$00,$00,$61,$73
            .byte $7F,$1E,$00,$00,$00,$00,$01,$03,$0F,$0E,$00,$00,$00,$00,$03,$01
            .byte $01,$00,$00,$00,$00,$00,$03,$07,$0F,$0F,$00,$00,$00,$00,$61,$73
            .byte $3F,$1E,$00,$00,$00,$00,$FE,$F0,$C0,$83,$0F,$1F,$1E,$3C,$00,$00
            .byte $FF,$00,$00,$00,$00,$00,$00,$00,$FE,$1F,$03,$00,$00,$00,$55,$55
            .byte $55,$55,$55,$55,$55,$55,$55,$55,$55,$55,$55,$55,$55,$55,$EC,$EC
            .byte $0C,$0C,$0C,$0C,$00,$00,$1C,$0C,$0C,$1C,$F8,$F0,$00,$00,$00,$00
            .byte $FC,$FC,$00,$00,$00,$00,$1C,$38,$F0,$E0,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$FC,$FC,$00,$00,$FC,$00,$00,$00,$00,$00,$00,$00,$0C,$0C
            .byte $FC,$FC,$00,$00,$00,$00,$0C,$0C,$0C,$0C,$0C
L770D       .byte $0C,$00,$00,$8C,$8C,$8C,$8C,$00,$00,$00,$00,$0C,$1C,$F8,$F0,$00
            .byte $00,$00,$00,$38,$1C,$0C,$0C,$0C,$0C,$00,$00,$80,$80,$FC,$FC,$00
            .byte $00,$00,$00,$8C,$8C,$8C,$8C,$00,$00,$00,$00,$0C,$0C,$0C,$0C,$00
            .byte $00,$00,$00,$1C,$38,$F0,$E0,$00,$00,$00,$00,$1C,$38,$F0,$E0,$00
            .byte $00,$00,$00,$1C,$38,$F0,$F0,$18,$00,$00,$00,$F8,$F8,$1C,$0C,$0C
            .byte $0C,$00,$00,$0C,$1C,$F8,$F0,$00,$00,$00,$00,$C0,$C0,$C0,$C0,$00
            .byte $00,$00,$00,$0C,$0C,$FC,$FC,$00,$00,$00,$00,$B8,$B0,$F0,$E0,$00
            .byte $00,$00,$00,$86,$CE,$FC,$78,$00,$00,$00,$00,$E0,$F0,$3C,$1C,$00
            .byte $00,$00,$00,$B8,$B0,$F0,$E0,$60,$C0,$00,$00,$80,$00,$FC,$FC,$00
            .byte $00,$00,$00,$86,$CE,$FC,$78,$00,$00,$00,$00,$00,$00,$7F,$F8,$C0
            .byte $00,$00,$00,$55,$55,$55,$55,$55,$55,$55,$55,$7F,$0F,$03,$C1,$F0
            .byte $F8,$78,$3C
L77C0       .byte $7E,$FF,$E7,$42,$00,$00,$00,$00,$00,$00,$00,$00,$42,$E7,$FF,$7E
            .byte $60,$F0,$E0,$C0,$C0,$E0,$F0,$60,$06,$0F,$07,$03,$03,$07,$0F,$06
            .byte $55,$55,$55,$55,$55,$55,$55,$55,$55,$55,$55,$55,$55,$55,$55,$55
            .byte $55,$55,$55,$55,$55,$55,$55,$55,$1C,$38,$70,$E7,$07,$00,$00,$00
L7800       .byte $00,$00,$00,$00,$00,$00,$00,$00,$3C,$18,$18,$18,$18,$18,$18,$3C
            .byte $00,$00,$81,$FF,$FF,$81,$00,$00,$7E,$C3,$B1,$B1,$B1,$BD,$C3,$7E
            .byte $00,$FF,$FF,$FF,$FF,$FF,$FF,$00,$7E,$7E,$7E,$7E,$7E,$7E,$7E,$7E
            .byte $00,$FC,$FE,$FE,$FE,$FE,$FE,$7E,$00,$3F,$7F,$7F,$7F,$7F,$7F,$7E
            .byte $7E,$7F,$7F,$7F,$7F,$7F,$3F,$00,$7E,$FE,$FE,$FE,$FE,$FE,$FC,$00
            .byte $7E,$FF,$FF,$FF,$FF,$FF,$FF,$00,$7E,$FE,$FE,$FE,$FE,$FE,$FE,$7E
            .byte $00,$FF,$FF,$FF,$FF,$FF,$FF,$7E,$7E,$7F,$7F,$7F,$7F,$7F,$7F,$7E
            .byte $7E,$FF,$FF,$FF,$FF,$FF,$FF,$7E,$00,$7E,$7E,$7E,$7E,$7E,$7E,$00
L7880       .byte $00,$00,$00,$00,$00,$00,$00,$00
L7888       .byte $00
L7889       .byte $24
L788A       .byte $6E
L788B       .byte $46
L788C       .byte $62
L788D       .byte $76
L788E       .byte $24
L788F       .byte $00,$FF,$80,$80,$80,$80,$80,$80,$80,$00,$00,$00,$00,$00,$00,$00
            .byte $00
L78A0       .byte $00
L78A1       .byte $24
L78A2       .byte $76
L78A3       .byte $62
L78A4       .byte $46
L78A5       .byte $6E
L78A6       .byte $24
L78A7       .byte $00,$FF,$81,$81,$81,$81,$81,$81,$FF,$00,$3C,$7E,$7E,$7E,$7E,$3C
            .byte $00,$7E,$FF,$FF,$FF,$FF,$FF,$81,$00,$FF,$81,$81,$99,$99,$81,$81
            .byte $FF,$FF,$99,$99,$FF,$FF,$99,$99,$FF,$00,$01,$03,$07,$0F,$1F,$3F
            .byte $7F,$00,$80,$C0,$E0,$F0,$F8,$FC,$FE,$FF,$81,$81,$81,$81,$81,$81
            .byte $FF,$FF,$81,$81,$81,$81,$81,$81,$FF,$FF,$81,$99,$BD,$BD,$99,$81
            .byte $FF,$00,$00,$00,$00,$00,$00,$00,$00,$18,$18,$38,$F0,$E0,$00,$00
            .byte $00,$00,$00,$08,$03,$01,$00,$00,$00,$00,$00,$00,$07,$0F,$1C,$18
            .byte $18,$00,$00,$00,$80,$C0,$10,$00,$00,$00,$00,$00,$07,$0F,$1C,$18
            .byte $18,$00,$00,$00,$80,$C0,$10,$00,$00
L7930       .byte $18,$18,$38,$F0,$E0,$00,$00,$00,$00,$00,$08,$03,$01,$00,$00,$00
            .byte $99,$99,$99,$99,$99,$99,$99,$99,$99,$99,$99,$99,$99,$99,$99,$99
            .byte $99,$99,$99,$99,$99,$99,$99,$99,$99,$99,$99,$99,$99,$99,$99,$99
            .byte $3C,$FF,$00,$DB,$DB,$00,$FF,$3C,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $18,$18,$18,$18,$18,$18,$18,$18,$00,$00,$00,$FF,$FF,$00,$00,$00
            .byte $18,$18,$18,$18,$18,$18,$18,$18,$00,$00,$00,$FF,$FF,$00,$00,$00
            .byte $00,$00,$03,$0C,$10,$10,$20,$20,$00,$00,$C0,$20,$00,$08,$00,$C0
            .byte $03,$00,$10,$00,$04,$03,$00,$00,$04,$04,$08,$08,$30,$C0,$00,$00
            .byte $0E,$3E,$76,$66,$CE,$FE,$FE,$00,$70,$7C,$6E,$66,$73,$7F,$7F,$00
            .byte $00,$FE,$FE,$CE,$66,$76,$3E,$0E,$00,$7F,$7F,$73,$66,$6E,$7C,$70
            .byte $FF,$80,$80,$80,$80,$80,$80,$80,$FF,$01,$01,$01,$01,$01,$01,$01
            .byte $01,$01,$01,$01,$01,$01,$01,$FF,$80,$80,$80,$80,$80,$80,$80,$FF
            .byte $8C,$00,$40,$01,$37,$3F,$1C,$00,$40,$1F,$00,$FF,$FF,$00,$00,$00
            .byte $03,$FC,$00,$FF,$FF,$00,$00,$00,$31,$03,$06,$8E
L7A1C       .byte $FC,$FC,$38,$00,$00,$1C,$3F,$37,$61,$60,$C0,$8C,$00,$00,$00,$FF
            .byte $FF,$00,$3F,$C0,$00,$00,$00,$FF,$FF,$00,$FC,$01,$00,$38,$FC,$EC
            .byte $80,$02,$00,$31,$C0,$70,$3C,$0F,$83,$87,$0E,$0C,$8C,$CC,$CC,$CC
            .byte $CC,$CC,$CC,$CC,$CC,$CC,$CC,$CC,$CC,$CC,$CC,$8C,$0C,$0E,$87,$83
            .byte $07,$0C,$30,$C0,$03,$0C,$30,$E0,$C1,$E1,$70,$70,$31,$33,$33,$33
            .byte $33,$33,$33,$33,$33,$33,$33,$33,$33,$33,$33,$31,$70,$70,$E1,$C1
            .byte $C0,$30,$08,$01,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
L7A98       .byte $00,$00,$00,$00,$00,$00,$00,$00,$10,$18,$30,$60,$40,$00,$60,$70
            .byte $18,$18,$0C,$0E,$1E,$30,$66,$CE
L7AB0       .byte $70,$60,$00,$40,$60,$30,$18,$18,$CE,$66,$30,$1E,$0E,$0C,$18,$08
            .byte $00,$33,$73,$C1,$00,$00,$00,$00,$01,$03,$06,$8C,$F9,$73,$33,$00
            .byte $00,$CC,$CE,$83,$01,$00,$00,$00,$80,$C0,$60,$30,$9F,$CE,$CC,$00
            .byte $01,$07,$0F,$1E,$1C,$18,$18,$18,$FF,$FF,$FF,$00,$00,$00,$00,$00
            .byte $40,$30,$18,$18,$18,$18,$18,$18,$18,$18,$18,$18,$18,$18,$18,$18
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$18,$18,$18,$18,$18,$08,$00,$08
            .byte $00,$00,$FF,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$FF,$00,$00
            .byte $00,$3C,$7E,$7E,$7E,$7E,$3C,$00,$00,$00,$00,$18,$18,$00,$00,$00
            .byte $00,$00,$18,$3C,$3C,$18,$00,$00,$00,$00,$38,$7C,$7C,$7C,$38,$00
            .byte $20,$20,$20,$20,$20,$20,$20,$20,$20,$20,$20,$20,$20,$20,$20,$23
            .byte $C4,$04,$04,$04,$04,$04,$04,$04,$04,$04,$04,$04,$04,$04,$04,$04
            .byte $00,$00,$07,$08,$10,$20,$20,$26,$00,$00,$E0,$10,$08,$04,$00,$60
            .byte $06,$00,$20,$10,$08,$07,$00,$00,$64,$04,$04,$08,$10,$E0,$00,$00
            .byte $07,$18,$23,$4C,$50,$90,$A0,$A0,$E0,$18,$C4,$32,$08
L7B8D       .byte $01,$04,$00,$00,$20,$80,$10,$4C,$23,$18,$07,$05,$05,$09,$0A,$32
            .byte $C4,$18,$E0,$10,$00,$10,$18,$18,$18,$18,$18,$18,$18,$18,$18,$18
            .byte $08,$00,$08,$01,$03,$07,$0E,$1C,$18,$18,$18,$80,$C0,$E0,$70,$38
            .byte $18,$18,$18,$7F,$C6,$92,$92,$82,$92,$92,$FF,$FF,$70,$73,$73,$70
            .byte $73,$10,$FF,$FF,$86,$93,$93,$87,$93,$93,$FF,$FE,$07,$9F,$9F,$9F
            .byte $99,$99,$FF,$10,$00,$10,$18,$18,$18,$18,$18,$18,$18,$18,$18,$18
            .byte $08,$00,$08,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$55,$40,$40
L7C03       .byte $40,$40,$40,$40,$AA,$55,$00,$00,$00,$00,$00,$00,$AA,$55,$02,$02
            .byte $02,$02,$02,$02,$AA,$55,$40,$40,$40,$40,$40,$40,$40,$55,$00,$00
            .byte $00,$00,$00,$00,$00,$55,$02,$02,$02,$02,$02,$02,$02,$40,$40,$40
            .byte $40,$40,$40,$40,$40,$00,$00,$00,$00,$00,$00,$00,$00,$02,$02,$02
            .byte $02,$02,$02,$02,$02,$40,$40,$40,$40,$40,$40,$40,$AA,$00,$00,$00
            .byte $00,$00,$00,$00,$AA,$02,$02,$02,$02,$02,$02,$02,$AA,$00,$40,$10
            .byte $20,$08,$02,$02,$AA,$56,$42,$42,$7E,$7E,$42,$42,$7E,$7E,$42,$42
            .byte $7E,$7E,$42,$42,$7E,$7E,$42,$42,$7E,$7E,$42,$42,$AA,$55,$7F,$7F
            .byte $7F,$7F,$7F,$7F,$AA,$55,$FF,$FF,$FF,$FF,$FF,$FF,$AA,$55,$FE,$FE
            .byte $FE,$FE,$FE,$FE,$AA,$55,$7F,$7F,$7F,$7F,$7F,$7F,$7F,$55,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$55,$FE,$FE,$FE,$FE,$FE,$FE,$FE,$7F,$7F,$7F
            .byte $7F,$7F,$7F,$7F,$7F,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FE,$FE,$FE
            .byte $FE,$FE,$FE,$FE,$FE,$7F,$7F,$7F,$7F,$7F,$7F,$7F,$AA,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$AA,$FE,$FE,$FE,$FE,$FE,$FE,$FE,$AA,$00,$40,$D0
            .byte $E0,$F8,$FE,$FE,$AA,$AA,$AA,$AA,$AA,$AA,$AA,$AA,$AA
L7CF0       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$3F,$7F,$60,$60,$60,$60,$40,$00,$F0,$F8,$04,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$20,$1F,$0F,$00,$04,$06,$06,$06,$06,$FE,$FC,$00
            .byte $00,$1F,$3F,$70,$60,$60,$60,$61,$00,$FF,$FF,$00,$00,$00,$00,$FF
L7D30       .byte $00,$F8,$FC,$00,$02,$00,$00,$00,$63,$63,$63,$63,$63,$63,$63,$21
            .byte $84,$C6,$C6,$C6,$C6,$C6,$C6,$C6,$01,$40,$00,$00,$60,$3F,$1F,$00
            .byte $FF,$00,$00,$00
L7D54       .byte $00,$FF,$FF,$00,$86,$06,$06,$06,$0E,$FC,$F8,$00,$18,$18,$18,$18
            .byte $18,$18,$18,$18,$18,$18,$18,$18,$18,$08,$00,$08,$10,$00,$10,$18
            .byte $18,$18,$18,$18,$18,$18,$18,$18,$18,$18,$18,$18,$00,$00,$00,$FF
            .byte $FF,$00,$00,$66,$66,$00,$00,$FF,$FF,$00,$00,$00,$66,$00,$00,$00
            .byte $00,$00,$18,$24,$24,$18,$00,$00,$00,$00,$00,$66,$66,$00,$00,$00
            .byte $00,$00,$00,$FF,$FF,$00,$00,$00,$00,$00,$00,$66,$01,$03,$07,$0E
            .byte $1C,$38,$70,$E0,$C0,$80,$00,$00,$00,$00,$00,$00,$00,$00,$00,$0F
            .byte $1F,$38,$70,$E0,$00,$01,$1F,$FF,$F0,$00,$00,$00,$00,$FF,$FF,$00
            .byte $00,$00,$00,$FF,$00,$80,$F8,$FF,$0F,$00,$00,$00,$00,$00,$00,$40
            .byte $18,$0C,$0E,$07,$80,$C0,$E0,$70,$38,$1C,$0E,$07,$03,$01,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$03,$07,$0E
            .byte $1C,$38,$60,$8C,$00,$FF,$FF,$00,$00,$3F,$40,$80,$00,$FF,$FF,$00
            .byte $00,$F8,$02,$00,$00,$C0,$E0,$70,$38,$0C,$00,$31,$8C,$00,$20,$18
            .byte $0E,$07,$03,$00,$80,$40,$3F,$00,$00,$FF,$FF,$00,$01,$02,$FC,$00
            .byte $00,$FF,$FF,$00,$31,$06,$1C,$38,$70,$E0,$C0,$00,$01,$02,$06,$0C
            .byte $1D,$39,$70,$E0,$C3,$C6,$CC,$CC,$CC,$CC,$CC,$CC,$CC,$CC,$CC,$CC
            .byte $CC,$CC,$C6,$C3,$E0,$70,$31,$19,$0C,$04,$00,$01,$80,$00,$20,$30
            .byte $98,$9C,$0E,$07,$C3,$E3,$73,$33,$33,$33,$33,$33,$33,$33,$33,$33
            .byte $33,$73,$E3,$C3,$07,$0E,$9C,$B8,$30,$60,$C0,$80,$00,$00,$00,$FF
            .byte $FF,$00,$00,$FF,$FF,$00,$00,$FF,$FF,$00,$00,$00,$31,$31,$31,$31
            .byte $31,$31,$31,$31,$8C,$8C,$8C,$8C,$8C,$8C,$8C,$8C,$10,$18,$30,$7F
            .byte $7F,$00,$60,$73,$18,$18,$0C,$FE,$FE,$00,$06,$CE,$73,$60,$00,$7F
            .byte $7F,$30,$18,$18,$CE,$06,$00,$FE,$FE,$0C,$18,$08,$00,$33,$73,$F1
            .byte $30,$30,$31,$31,$31,$31,$30,$B0,$F1,$73,$33,$00,$00,$CC,$CE,$8F
            .byte $0D,$0C,$8C,$8C,$8C,$8C,$0C,$0C,$8F,$CE,$CC,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$FF,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$07,$0F,$1C,$18,$18,$00,$00,$00
L7F03       .byte $FF,$FF,$00,$00,$00,$00,$00,$00,$C0,$E0,$08,$00,$00,$00,$7C,$C6
            .byte $FE,$C6,$C6,$E6
L7F17       .byte $00,$00,$FC,$C6,$FC
L7F1C       .byte $C6,$C6,$FC,$00,$00,$FC,$C6,$C6,$C6,$C6,$FC,$00,$00,$FE,$C2,$F0
            .byte $C0,$C2,$FE,$00
L7F30       .byte $00,$CC,$D8,$F0,$D8,$CC,$E6,$00,$00,$C6,$E6,$F6,$DE,$CE,$E6,$00
            .byte $00,$7C,$C6,$C6,$C6,$C6,$7C,$00,$00,$FC,$C6,$FC,$D8,$CC,$E6,$00
            .byte $00,$E7,$C3,$DB,$FF,$E7,$C3,$00,$00,$66,$66,$7E,$18,$18,$18,$00
L7F60       .byte $00,$08,$7D,$FF,$FF,$CF,$04,$00
L7F68       .byte $00,$03,$8F,$FF,$FF,$31,$00,$00
L7F70       .byte $38,$FE,$FF,$FF,$FF,$F9,$60,$00
L7F78       .byte $00,$26,$7F,$FF,$FF,$9C,$00,$00,$00,$00,$00,$00,$00,$28,$AA,$AA
            .byte $AA,$AA,$28,$00,$00,$00,$00,$00,$02,$02,$0A,$0A,$0A,$0A,$02,$02
            .byte $80,$80,$A0,$A0,$A0,$A0,$80,$80,$00,$00,$00
L7FA3       .byte $EA
L7FA4       .byte $BA,$00,$00,$00,$00,$00,$00,$AE,$BA,$00,$00,$00,$00,$A8,$BA,$BE
            .byte $BE,$BA,$A8,$00,$00,$2A,$AE,$BE,$BE,$AE,$2A,$00,$0A,$2A,$AE,$BE
            .byte $BE,$AE,$2A,$0A,$A0,$A8,$BA,$BE,$BE,$BA,$A8,$A0,$AA,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$AA,$00,$AA,$BE,$BE
            .byte $BE,$BE,$AA,$00,$00,$00,$AA,$BE,$BE,$BE,$BE,$BE,$BE,$BE,$BE,$BE
            .byte $BE,$BE,$BE,$BE,$BE,$BE,$BE,$BE,$BE,$AA,$00,$00
L8000       .byte $00,$00,$00,$00,$00,$00,$00,$00,$3C,$18,$18,$18,$18,$18,$18,$3C
            .byte $00,$00,$81,$FF,$FF,$81,$00,$00,$7E,$C3,$B1,$B1,$B1,$BD,$C3,$7E
            .byte $00,$FF,$FF,$FF,$FF,$FF,$FF,$00,$7E,$7E,$7E,$7E,$7E,$7E,$7E,$7E
            .byte $00,$FC,$FE,$FE,$FE,$FE,$FE,$7E,$00,$3F,$7F,$7F,$7F,$7F,$7F,$7E
            .byte $7E,$7F,$7F,$7F,$7F,$7F,$3F,$00,$7E,$FE,$FE,$FE,$FE,$FE,$FC,$00
            .byte $7E,$FF,$FF,$FF,$FF,$FF,$FF,$00,$7E,$FE,$FE,$FE,$FE,$FE,$FE,$7E
            .byte $00,$FF,$FF,$FF,$FF,$FF,$FF,$7E,$7E,$7F,$7F,$7F,$7F,$7F,$7F,$7E
            .byte $7E,$FF,$FF,$FF,$FF,$FF,$FF,$7E,$00,$7E,$7E,$7E,$7E,$7E,$7E,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$1C,$1E,$46,$62,$78,$38,$00
            .byte $FF,$80,$80,$80,$80,$80,$80,$80,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$1C,$1E,$46,$62,$78,$38,$00,$FF,$81,$81,$81,$81,$81,$81,$FF
            .byte $69,$BE,$FF,$FF,$FF,$FF,$3C,$41,$7E,$FF,$FF,$FF,$FF,$FF,$81,$00
            .byte $FF,$81,$81,$99,$99,$81,$81,$FF,$FF,$99,$99,$FF,$FF,$99,$99,$FF
            .byte $00,$01,$03,$07,$0F,$1F,$3F,$7F,$00,$80,$C0,$E0,$F0,$F8,$FC,$FE
            .byte $FF,$81,$81,$81,$81,$81,$81,$FF,$FF,$81,$81,$81,$81,$81,$81,$FF
            .byte $FF,$81,$99,$BD,$BD,$99,$81,$FF,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $09,$09,$09,$59,$19,$F5,$01,$55,$B9,$B9,$B9,$B9,$B9,$A4,$90,$55
            .byte $55,$55,$55,$AA,$BF,$BA,$B9,$B9,$55,$55,$55,$AA,$FE,$A9,$59,$09
            .byte $00,$00,$00,$05,$08,$09,$09,$09,$00,$00,$00,$54,$09,$19,$A9,$A9
            .byte $B9,$B9,$B9,$B9,$F9,$A9,$55,$00,$09,$09,$09,$0A,$0F,$06,$01,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $3C,$FF,$00,$DB,$DB,$00,$FF,$3C,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $09,$09,$09,$09,$09,$09,$09,$09,$00,$00,$00,$55,$00,$FF,$00,$55
            .byte $B9,$B9,$B9,$B9,$B9,$B9,$B9,$B9,$55,$55,$55,$AA,$FF,$AA,$55,$00
            .byte $00,$0A,$15,$10,$10,$10,$10,$10,$00,$A0,$54,$0C,$0C,$0C,$0C,$0C
            .byte $10,$10,$10,$10,$1F,$0F,$00,$00,$0C,$0C,$0C,$0C,$FC,$F0,$00,$00
            .byte $0E,$3E,$76,$66,$CE,$FE,$FE,$00,$70,$7C,$6E,$66,$73,$7F,$7F,$00
            .byte $00,$FE,$FE,$CE,$66,$76,$3E,$0E,$00,$7F,$7F,$73,$66,$6E,$7C,$70
            .byte $FF,$80,$80,$80,$80,$80,$80,$80,$FF,$01,$01,$01,$01,$01,$01,$01
            .byte $01,$01,$01,$01,$01,$01,$01,$FF,$80,$80,$80,$80,$80,$80,$80,$FF
            .byte $1A,$1E,$1A,$16,$2B,$3E,$19,$04,$7F,$5A,$AA,$FF,$AA,$55,$00,$00
            .byte $FE,$AA,$AA,$FF,$AA,$55,$00,$00,$6A,$7A,$6A,$96,$EA,$7E,$1A,$05
            .byte $00,$14,$01,$3C,$03,$14,$11,$1D,$00,$00,$00,$55,$00,$FF,$00,$7F
            .byte $00,$00,$00,$55,$00,$FF,$00,$FD,$00,$14,$41,$3E,$C2,$16,$46,$36
            .byte $00,$00,$40,$60,$58,$9A,$AE,$B9,$79,$79,$B9,$B9,$B9,$B9,$B9,$B9
            .byte $B9,$B9,$B9,$B9,$B9,$B9,$B9,$79,$B4,$A4,$69,$6D,$B9,$E4,$90,$40
            .byte $00,$00,$05,$15,$51,$96,$EA,$3A,$25,$25,$28,$28,$28,$28,$28,$28
            .byte $28,$28,$28,$28,$28,$28,$26,$25,$25,$61,$96,$EA,$7A,$1F,$06,$01
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $B9,$B9,$F9,$F9,$F9,$F9,$B9,$B9,$09,$09,$09,$11,$0D,$31,$0D,$12
            .byte $B9,$B9,$F9,$F9,$F9,$F9,$B9,$B9,$16,$1A,$1A,$25,$39,$2D,$09,$09
            .byte $55,$55,$69,$BE,$FF,$AA,$55,$00,$00,$05,$10,$4F,$37,$E5,$26,$55
            .byte $55,$55,$69,$BE,$FF,$AA,$55,$00,$00,$50,$05,$F9,$05,$68,$A8,$A5
            .byte $AA,$BF,$BA,$B9,$B9,$B9,$B9,$B9,$AA,$FF,$AA,$55,$00,$00,$00,$00
            .byte $A9,$F9,$A9,$59,$09,$09,$09,$09,$B9,$B9,$B9,$B9,$B9,$B9,$B9,$B9
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$09,$09,$09,$09,$09,$09,$09,$09
            .byte $00,$AA,$55,$00,$00,$00,$00,$00,$00,$00,$00,$00,$FF,$FF,$00,$00
            .byte $00,$3C,$7E,$7E,$7E,$7E,$3C,$00,$00,$00,$00,$18,$18,$00,$00,$00
            .byte $00,$00,$18,$3C,$3C,$18,$00,$00,$00,$00,$38,$7C,$7C,$7C,$38,$00
            .byte $10,$10,$10,$10,$10,$10,$10,$10,$10,$10,$10,$10,$10,$10,$10,$10
            .byte $0C,$0C,$0C,$0C,$0C,$0C,$0C,$0C,$0C,$0C,$0C,$0C,$0C,$0C,$0C,$0C
            .byte $00,$00,$0A,$25,$50,$40,$4C,$41,$00,$00,$A0,$54,$03,$03,$33,$07
            .byte $45,$49,$45,$40,$30,$0F,$00,$00,$17,$27,$17,$03,$0C,$F0,$00,$00
            .byte $07,$18,$23,$4C,$50,$90,$A0,$A0,$E0,$18,$C4,$32,$08,$01,$04,$00
            .byte $00,$20,$80,$10,$4C,$23,$18,$07,$05,$05,$09,$0A,$32,$C4,$18,$E0
            .byte $B9,$B9,$B9,$B9,$B9,$B9,$B9,$B9,$09,$09,$09,$09,$09,$09,$09,$09
            .byte $00,$01,$00,$07,$00,$0D,$01,$05,$80,$50,$20,$D4,$B4,$B9,$B9,$B9
            .byte $7F,$C6,$92,$92,$82,$92,$92,$FF,$FF,$70,$73,$73,$70,$73,$10,$FF
            .byte $FF,$86,$93,$93,$87,$93,$93,$FF,$FE,$07,$9F,$9F,$9F,$99,$99,$FF
            .byte $09,$09,$09,$09,$09,$09,$09,$09,$B9,$B9,$B9,$B9,$B9,$B9,$B9,$B9
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $55,$40,$40,$40,$40,$40,$40,$AA,$55,$00,$00,$00,$00,$00,$00,$AA
            .byte $55,$02,$02,$02,$02,$02,$02,$AA,$55,$40,$40,$40,$40,$40,$40,$40
            .byte $55,$00,$00,$00,$00,$00,$00,$00,$55,$02,$02,$02,$02,$02,$02,$02
            .byte $40,$40,$40,$40,$40,$40,$40,$40,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $02,$02,$02,$02,$02,$02,$02,$02,$40,$40,$40,$40,$40,$40,$40,$AA
            .byte $00,$00,$00,$00,$00,$00,$00,$AA,$02,$02,$02,$02,$02,$02,$02,$AA
            .byte $00,$40,$10,$20,$08,$02,$02,$AA,$56,$42,$42,$7E,$7E,$42,$42,$7E
            .byte $7E,$42,$42,$7E,$7E,$42,$42,$7E,$7E,$42,$42,$7E,$7E,$42,$42,$AA
            .byte $55,$7F,$7F,$7F,$7F,$7F,$7F,$AA,$55,$FF,$FF,$FF,$FF,$FF,$FF,$AA
            .byte $55,$FE,$FE,$FE,$FE,$FE,$FE,$AA,$55,$7F,$7F,$7F,$7F,$7F,$7F,$7F
            .byte $55,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$55,$FE,$FE,$FE,$FE,$FE,$FE,$FE
            .byte $7F,$7F,$7F,$7F,$7F,$7F,$7F,$7F,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FE,$FE,$FE,$FE,$FE,$FE,$FE,$FE,$7F,$7F,$7F,$7F,$7F,$7F,$7F,$AA
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$AA,$FE,$FE,$FE,$FE,$FE,$FE,$FE,$AA
            .byte $00,$40,$D0,$E0,$F8,$FE,$FE,$AA,$AA,$AA,$AA,$AA,$AA,$AA,$AA,$AA
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $55,$55,$55,$6A,$7F,$6A,$65,$64,$55,$55,$55,$A9,$FD,$A9,$55,$01
            .byte $64,$64,$64,$6A,$55,$40,$7F,$40,$01,$01,$01,$A9,$55,$01,$FD,$01
            .byte $00,$00,$01,$04,$03,$0C,$01,$05,$00,$00,$55,$00,$FF,$00,$55,$FF
            .byte $00,$00,$40,$10,$C4,$39,$49,$59,$04,$04,$04,$04,$04,$04,$04,$04
            .byte $99,$99,$99,$99,$99,$99,$99,$99,$05,$05,$05,$09,$0E,$07,$01,$00
            .byte $AA,$55,$55,$55,$AA,$FF,$AA,$55,$59,$59,$59,$69,$B9,$E4,$90,$40
            .byte $09,$09,$19,$29,$09,$09,$09,$09,$09,$09,$09,$09,$09,$19,$29,$09
            .byte $B9,$B2,$B6,$BA,$B9,$B9,$B9,$B9,$B9,$B9,$B9,$B9,$B1,$B6,$BA,$BA
            .byte $00,$00,$00,$55,$00,$FF,$00,$D7,$96,$D7,$55,$AA,$FF,$AA,$55,$00
            .byte $14,$96,$55,$55,$55,$55,$7D,$69,$41,$7D,$55,$55,$55,$55,$D7,$14
            .byte $96,$D7,$55,$55,$55,$AA,$FF,$AA,$00,$FF,$00,$55,$55,$55,$D7,$14
            .byte $00,$01,$00,$07,$00,$1D,$01,$75,$05,$D5,$15,$55,$55,$55,$55,$55
            .byte $00,$00,$00,$01,$04,$13,$0C,$71,$00,$15,$40,$3F,$C0,$15,$55,$55
            .byte $00,$55,$00,$FF,$00,$55,$55,$55,$00,$54,$01,$FC,$03,$54,$55,$55
            .byte $00,$00,$00,$40,$10,$C4,$30,$4D,$40,$40,$10,$D0,$04,$74,$41,$5D
            .byte $50,$57,$54,$55,$55,$55,$55,$55,$55,$55,$55,$55,$55,$55,$55,$55
            .byte $00,$01,$04,$13,$0C,$35,$1D,$11,$00,$55,$00,$FF,$3F,$7F,$40,$55
            .byte $00,$55,$00,$FF,$FD,$FE,$02,$56,$00,$40,$10,$C4,$30,$4E,$76,$56
            .byte $1E,$1A,$25,$39,$1E,$07,$01,$10,$55,$55,$6A,$6A,$BF,$FF,$AA,$55
            .byte $56,$56,$AA,$AA,$FE,$FF,$AA,$55,$7A,$6A,$5A,$6E,$B9,$E4,$90,$40
            .byte $00,$00,$05,$15,$94,$95,$96,$95,$95,$95,$A4,$A4,$A4,$A4,$A4,$A4
            .byte $A4,$A4,$A4,$A4,$A4,$A4,$AA,$9A,$95,$94,$95,$E6,$7A,$1F,$06,$01
            .byte $00,$00,$40,$60,$6C,$6D,$6E,$6E,$6E,$6E,$BE,$BE,$BE,$BE,$BE,$BE
            .byte $BE,$BE,$BE,$BE,$BE,$BE,$6E,$6E,$6E,$6E,$6E,$6E,$B9,$E4,$90,$40
            .byte $00,$00,$55,$00,$FF,$00,$55,$55,$55,$55,$55,$55,$AA,$FF,$AA,$55
            .byte $24,$24,$24,$24,$24,$24,$24,$24,$6E,$6E,$6E,$6E,$6E,$6E,$6E,$6E
            .byte $A9,$A9,$55,$40,$7F,$40,$75,$49,$09,$09,$55,$01,$FD,$01,$5D,$52
            .byte $59,$69,$69,$55,$6A,$BF,$BA,$B9,$56,$5A,$5A,$55,$A9,$FD,$A9,$59
            .byte $55,$54,$65,$A6,$E5,$A4,$64,$24,$24,$24,$24,$64,$27,$E5,$26,$55
            .byte $55,$16,$6A,$AE,$AF,$6E,$6E,$6E,$6E,$6E,$6E,$6D,$2D,$6C,$AC,$A5
            .byte $AA,$FF,$80,$95,$95,$95,$95,$95,$AA,$FF,$00,$55,$55,$55,$55,$55
            .byte $A9,$F9,$09,$59,$59,$59,$59,$59,$95,$95,$95,$AA,$BF,$BA,$B9,$B9
            .byte $55,$55,$55,$AA,$FF,$AA,$55,$00,$59,$59,$59,$A9,$F9,$A9,$59,$09
            .byte $00,$7C,$C6,$FE,$C6,$C6,$E6,$00,$00,$FC,$C6,$FC,$C6,$C6,$FC,$00
            .byte $00,$FC,$C6,$C6,$C6,$C6,$FC,$00,$00,$FE,$C2,$F0,$C0,$C2,$FE,$00
            .byte $00,$CC,$D8,$F0,$D8,$CC,$E6,$00,$00,$C6,$E6,$F6,$DE,$CE,$E6,$00
            .byte $00,$7C,$C6,$C6,$C6,$C6,$7C,$00,$00,$FC,$C6,$FC,$D8,$CC,$E6,$00
            .byte $00,$E7,$C3,$DB,$FF,$E7,$C3,$00,$00,$66,$66,$7E,$18,$18,$18,$00
            .byte $00,$08,$7D,$FF,$FF,$CF,$04,$00,$00,$03,$8F,$FF,$FF,$31,$00,$00
            .byte $38,$FE,$FF,$FF,$FF,$F9,$60,$00,$00,$26,$7F,$FF,$FF,$9C,$00,$00
            .byte $00,$00,$00,$00,$00,$28,$AA,$AA,$AA,$AA,$28,$00,$00,$00,$00,$00
            .byte $02,$02,$0A,$0A,$0A,$0A,$02,$02,$80,$80,$A0,$A0,$A0,$A0,$80,$80
            .byte $00,$00,$00,$AE,$AB,$00,$00,$00,$00,$00,$00,$BA,$EA,$00,$00,$00
            .byte $00,$A8,$BA,$BE,$BE,$BA,$A8,$00,$00,$2A,$AE,$BE,$BE,$AE,$2A,$00
            .byte $0A,$2A,$AE,$BE,$BE,$AE,$2A,$0A,$A0,$A8,$BA,$BE,$BE,$BA,$A8,$A0
            .byte $AA,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$AA
            .byte $00,$AA,$BE,$BE,$BE,$BE,$AA,$00,$00,$00,$AA,$BE,$BE,$BE,$BE,$BE
            .byte $BE,$BE,$BE,$BE,$BE,$BE,$BE,$BE,$BE,$BE,$BE,$BE,$BE,$AA,$00,$00
L8800       jsr L1ECB
            ldy #$00
            lda TEMP1+1
            beq L8812
            bpl L8818
            lda #$48
            ldx #$40
            bne L881C
L8812       lda #$A1
            ldx #$3F
            bne L881C
L8818       lda #$46
            ldx #$3E
L881C       sta L8838
            stx LDD06
            sty LDD06+1
            lda #$11
            ldx #$80
L8829       cpx TRIG2
            bne L8829
L882E       cpx TRIG2
            bne L8833
L8833       ldy #$09
L8835       dey
            bne L8835
L8838       lsr SAVADR
            inx
            bne L882E
            ldy #$07
L883F       dey
            bne L883F
            sta LDD0E+1
            rts
L8846       sei
            lda #$35
            sta NGFLAG
            ldx #$08
L884D       lda IOCB2+ICBLL,X
            bmi L885E
            cmp #$30
            bcc L885B
            and #$0F
            sta IOCB2+ICBLL,X
L885B       dex
            bpl L884D
L885E       bit L9E72
            bmi L88A7
            lda #$20
            ldx #$44
            ldy #$44
            sta L0541
            stx L0541+1
            sty L0541+2
            sta L1145
            stx L1145+1
            sty L1145+2
            sta L114F
            stx L114F+1
            sty L114F+2
            lda #$A9
            sta L0431
            lda #$12
            sta L0431+1
            dec L3E79+1
            lda #$43
            dec NGFLAG
            sta L8973
            sta LD172
            inc NGFLAG
            lda #$0C
            sta L3C1F
            lda #$80
            sta L3C22
L88A7       ldx #$00
            ldy #$4C
            stx TEMP2
            sty TEMP3
            ldx #$86
            ldy #$25
            stx HATABS+14
            sty HATABS+15
            jsr L8948
            lda #$00
            tay
L88C1       sta CASINI,Y
            sta L0100,Y
            sta VDSLST,Y
            iny
            bne L88C1
            ldx #$2E
L88CF       sta HPOSP0,X
            dex
            bpl L88CF
            lda #$01
            sta COLBK
            lda #$7F
            sta LDC0D
            lda LDC0D
            inc LDD00
            jsr L8800
            lda #$05
            sta DSTAT
            bit L9E73
            bmi L8945
            lda #$10
            sta FREQ
            lda #$0C
            sta LDBF4
            lda #$0B
            sta LDBF4+1
            ldx #$38
L8901       lda L8A74,X
            sta LE14A,X
            cpx #$09
            bcs L8911
            lda L8AAC,X
            sta LCFDC,X
L8911       dex
            bne L8901
            dec NGFLAG
L8916       lda L8975,X
            sta LC100,X
            lda L8AB5,X
            sta LDE1C+1,X
            cpx #$52
            bcs L892C
            lda L8BB5,X
            sta LDF1C+1,X
L892C       inx
            bne L8916
            inc NGFLAG
            ldy #$08
L8933       lda L8000,X
L8936       sta L7800,X
            inx
            bne L8933
            inc L8933+2
            inc L8936+2
            dey
            bne L8933
L8945       jmp IOCB3+ICDNO
L8948       inc NGFLAG
            ldx LCA09
            beq L896F
            lda #$00
            ldy #$80
L8953       sta L0000,Y
            iny
            bne L8953
            sta TMPLBT
            ldy #$01
            jsr LFFBA
            lda #$02
            ldx #<L8973
            ldy #>L8973
            jsr LFFBD
            lda #$00
            jsr JKEYON
L896F       sei
            dec NGFLAG
            rts
L8973       .byte 'PS'
L8975       .byte $00,$70,$70,$80,$70,$70,$70,$70,$70,$70,$70,$70,$70,$70,$70,$70
            .byte $A0,$A0,$A0,$A0,$10,$10,$00,$70,$10,$10,$70,$70,$10,$10,$10,$00
            .byte $B0,$C0,$C0,$C0,$B0,$C0,$C0,$C0,$00,$00,$00,$00,$B0,$00,$00,$00
            .byte $B0,$B0,$C0,$C0,$B0,$B0,$B0,$B0,$80,$80,$80,$80,$10,$10,$10,$10
            .byte $C0,$C0,$C0,$C0,$B0,$B0,$B0,$B0,$C0,$C0,$C0,$C0,$C0,$C0,$C0,$C0
            .byte $B0,$C0,$90,$70,$C0,$B0,$C0,$C0,$C0,$B0,$C0,$B0,$C0,$C0,$C0,$C0
            .byte $C0,$C0,$B0,$90,$20,$20,$20,$20,$B0,$B0,$B0,$B0,$B0,$B0,$B0,$B0
            .byte $90,$90,$70,$70,$C0,$B0,$B0,$C0,$70,$70,$70,$70,$B0,$C0,$00,$00
            .byte $50,$50,$50,$50,$50,$50,$50,$50,$50,$50,$50,$50,$50,$B0,$B0,$B0
            .byte $50,$50,$50,$50,$50,$50,$50,$50,$50,$50,$50,$50,$50,$50,$00,$00
            .byte $C0,$C0,$B0,$B0,$B0,$B0,$B0,$B0,$B0,$C0,$C0,$C0,$B0,$B0,$C0,$C0
            .byte $B0,$C0,$C0,$B0,$C0,$B0,$B0,$B0,$B0,$B0,$B0,$B0,$B0,$B0,$B0,$B0
            .byte $B0,$B0,$B0,$B0,$C0,$C0,$C0,$C0,$C0,$C0,$C0,$C0,$C0,$C0,$C0,$C0
            .byte $B0,$C0,$C0,$C0,$B0,$B0,$C0,$C0,$C0,$B0,$C0,$C0,$C0,$C0,$C0,$C0
            .byte $C0,$C0,$20,$20,$20,$20,$20,$20,$20,$20,$20,$20,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
L8A74       .byte $00,$F6,$7C,$BB,$80,$41,$09,$CB,$B0,$77,$7B,$00,$53,$2B,$00,$E3
            .byte $74,$0E,$B6,$71,$69,$46,$75,$12,$8F,$22,$51,$69,$89,$D5,$15,$0D
            .byte $65,$51,$09,$5B,$A7,$12,$9A,$72,$41,$79,$2B,$36,$16,$0B,$75,$71
            .byte $09,$CB,$F2,$1B,$0B,$E0,$71,$29
L8AAC       .byte $CB,$0D,$0F,$0F,$0A,$0D,$0F,$0A,$0A
L8AB5       .byte $E9,$00,$24,$31,$FF,$05,$25,$F8,$0F,$FA,$0E,$25,$F5,$00,$00,$30
            .byte $BF,$FF,$05,$74,$F8,$03,$22,$E2,$33,$33,$23,$F8,$04,$F7,$04,$FE
            .byte $0A,$23,$E2,$04,$5E,$D8,$09,$32,$D0,$03,$7D,$00,$00,$7C,$F8,$04
            .byte $F6,$BF,$32,$A4,$A5,$FF,$09,$A6,$7C,$E2,$04,$7B,$D8,$0D,$21,$31
            .byte $31,$20,$D8,$06
L8AF9       .byte $A7,$E3,$FF,$EB,$16,$E2,$E7,$E4,$E9,$E5,$EA,$C5,$16,$A8,$C7,$05
            .byte $B0,$0D,$91,$04,$26,$B0,$05,$A9,$AA,$AA,$FF,$09,$AB,$D8,$0F,$27
            .byte $DC,$03,$00,$FF,$03,$00,$C1,$07,$FE,$0A,$B0,$0F,$08,$08,$D8,$20
            .byte $10,$0C,$38,$0D,$60,$12,$7F,$10,$0A,$A7,$E3,$E9,$E2,$EB,$E3,$E9
            .byte $E5,$E8,$E8,$E6,$38,$14,$10,$09,$7D,$38,$13,$75,$00,$10,$07,$F8
            .byte $03,$88,$04,$F8,$04,$CF,$04,$FE,$0A,$10,$07,$18,$03,$50,$FE,$04
            .byte $26,$10,$06,$27,$F8,$04,$FE,$12,$E6,$03,$01,$06,$D9,$24,$25,$10
            .byte $06,$25,$00,$08,$08,$F0,$05,$25,$14,$E2,$08,$DE,$03,$9C,$05,$20
            .byte $18,$04,$22,$11,$03,$28,$04,$68,$08,$05,$27,$23,$77,$03,$26,$E2
            .byte $08,$10,$05,$68,$05,$66,$10,$05,$7C,$7D,$28,$04,$22,$03,$7C,$32
            .byte $F7,$06,$27,$E2,$08,$10,$05,$7D,$E8,$04,$10,$05,$20,$21,$25,$42
            .byte $F0,$06,$30,$D8,$0A,$E2,$05,$10,$05,$21,$03,$20
L8BB5       .byte $10,$08,$B4,$74,$F0,$08,$24,$20,$08,$03,$25,$E2,$08,$10,$19,$00
            .byte $08,$06,$F2,$09,$38,$08,$20,$07,$F8,$0A,$FA,$0E,$08,$03,$18,$03
            .byte $00,$FE,$04,$10,$06,$0F,$03,$F0,$07,$FA,$0E,$10,$06,$D6,$04,$10
            .byte $06,$00,$30,$06,$D6,$12,$10,$06,$D8,$08,$30,$05,$D8,$18,$10,$03
            .byte $B0,$05,$00,$10,$07,$08,$04,$B0,$15,$10,$17,$FA,$0E,$10,$03,$FE
            .byte $27,$00
            stx L00C3
            sty BFENLO
            lda #$00
            sta L0090
            ldx L00B9
            lda #$60
            sta L00B9
            inc LD01F+1
            jsr L8C28
            lda L00BA
            jsr LED09
            lda L00B9
            jsr LEE13
            jmp LF4D8
L8C28       lda L00B9
            ldy L00B7
            lda #$00
L8C2E       sta L0090
            inc LD01F+1
            lda L00BA
            jsr L8C58
            lda L00B9
            ora #$F0
            jsr LEDB9
            lda L0090
            bmi L8C52
            ldy #$00
L8C45       lda (L00BB),Y
            jsr LEDDD
            iny
            cpy L00B7
            bne L8C45
            jmp LF654
L8C52       inc LD01F+1
            jmp L8C52
L8C58       ora #$20
            inc LD01F+1
            jsr LF0A4
            pha
            bit L0094
            bpl L8C77
            sec
            ror L00A3
            ldy #$00
L8C6A       inc LD01F+1
            iny
            bne L8C6A
            jsr LED40
            lsr L0094
            lsr L00A3
L8C77       pla
            sta L0095
            sei
            inc LD01F+1
            jsr LEE97
            cmp #$3F
            bne L8C90
            ldy #$00
L8C87       inc LD01F+1
            iny
            bne L8C87
            jsr LEE85
L8C90       jmp LED2E
            jmp LED11
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF
L8E17       .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF
L8EE8       .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            sei
            lda #$2F
            sta L0000
            lda #$35
            sta NGFLAG
            ldx L00BA
            cpx #$06
            bcs L9011
            ldx #$00
L9011       lda #$00
            tay
L9014       sta CASINI,Y
            sta L0100,Y
L901A       sta VDSLST,Y
            sta VTIMR4,Y
            iny
            bne L9014
            stx LCA09
            jsr L921D
            lda #$25
            sta CASINI
L902D       ldx #$3F
L902F       cpx TRIG2
            bne L902F
            inx
L9035       cpx TRIG2
            bne L9035
L903A       ldy CASINI
L903C       dey
            bne L903C
            dex
            bne L903A
            lda TRIG2
            sta TEMP1+1
            beq L9059
            cmp #$03
            beq L9061
            cmp #$FD
            bne L902D
            dec L90EE+1
            lda #$48
            bne L905B
L9059       lda #$A1
L905B       sta L90F6
            inc L90DA+1
L9061       lda #$3F
            sta COLPM3
            lda #$00
            sta DOSINI
            sta DOSINI+1
            sta TRAMSZ
            sta CMCMD
            ldx #$00
            ldy #$D8
            stx DOSVEC
            sty DOSVEC+1
            ldx #$18
L907A       lda LB2D0,X
            ldy #$27
L907F       sta (DOSVEC),Y
            dey
            bpl L907F
            clc
            lda DOSVEC
            adc #$28
            sta DOSVEC
            bcc L908F
            inc DOSVEC+1
L908F       dex
            bpl L907A
            ldx #$5F
            ldy #$B2
            stx DOSVEC
            sty DOSVEC+1
            lda #$01
            sta BOOT
            lda #$08
            ldx #$03
L90A2       sta LD8CB+2,X
            sta LD8B5,X
            sta LD937+1,X
            dex
            bpl L90A2
            ldx #$0B
L90B0       sta LD951,X
            sta LD979,X
            sta LD9A1,X
            sta LD9C7+2,X
            sta LD9F1,X
            sta LDA17+2,X
            dex
            bpl L90B0
            jsr L9643
L90C8       inc HOLD1
            lda DOSINI+1
            bne L90FD
L90CE       bit L9C21
            ldy #$92
L90D3       cpy TRIG2
            bne L90D3
            ldx #$FF
L90DA       ldy #$0D
L90DC       dey
            bne L90DC
            nop
L90E0       inx
L90E1       lda LAC00,X
            sta LD021
            lda TRIG2
            and #$07
            eor #$03
L90EE       beq L90E0
            ldy #$06
L90F2       dey
            bne L90F2
            nop
L90F6       lsr SAVADR
            inx
            cpx #$36
            bcc L90E1
L90FD       lda L9B2F
            cmp #$FF
            bne L9123
            cmp DOSINI
            bne L9121
            cmp DOSINI+1
            beq L9123
            sta DOSINI+1
            lda #$20
            sta L91ED
            sta L91A0
            lda #$2C
            sta L91F3
L911B       lda #$12
            sta L00A4
            bne L9123
L9121       inc DOSINI
L9123       lda L9B2E
L9126       cmp #$01
            bne L9154
            lda #$0F
            sta L9183+1
            lda #$0C
            sta L9188+1
            lda #$0B
            sta L918D+1
            ldx #$27
L913B       lda #$09
            sta LDB20,X
            sta LDB48,X
            lda #$08
            sta LDB6F+1,X
            sta LDB98,X
            sta LDBC0,X
            dex
            bpl L913B
            stx L9126+1
L9154       cmp #$02
            bne L9160
            lda #$20
            sta L91EA
            sta L90CE
L9160       lda HOLD1
            bit LB23E
            beq L916C
            inc TEMPCHR
            jsr L0B3C
L916C       lda HOLD1
            and #$3F
            cmp #$22
            beq L9178
            cmp #$30
            bne L917C
L9178       lda #$09
            sta RAMLO+1
L917C       lda #$D1
L917E       cmp TRIG2
            bcs L917E
L9183       lda #$00
            sta LD021
L9188       lda #$00
            sta LD021+1
L918D       lda #$00
            sta LD021+2
L9192       lda #$D0
            sta COLPF0
            jsr L9283
            jsr L92CA
            jsr L92A9
L91A0       bit L42BB
            lda LB256
            ldx #$17
L91A8       ldy LB2B8,X
            sta LD826+2,Y
            dex
            bpl L91A8
            sta LD962
            sta LD987
            sta LD98A+2
            jsr L9309
            ldx #$D4
            ldy #$13
            lda #$F8
L91C3       cmp TRIG2
            bcs L91C3
            stx COLPF0
            sty GRAFM
            ldy #$1B
            lda #$04
L91D2       cmp TRIG2
            bne L91D2
            sty GRAFM
            lda L9E74
            beq L91F9
            ldx #$02
L91E1       lda LB253,X
            sta LD021,X
            dex
            bpl L91E1
L91EA       bit L9339
L91ED       bit L9B30
            jsr L9E75
L91F3       jsr L9646
            jmp L90C8
L91F9       sta GRAFM
            ldx #$18
L91FE       sta DMACLT,X
            dex
            bpl L91FE
            lda #$10
L9206       ldx #$02
L9208       bit TRIG1
            bpl L9208
L920D       bit TRIG1
            bmi L920D
            bit LDBFF+1
            beq L9206
            dex
            bpl L9208
            jmp L8846
L921D       lda #$C4
            sta LFFFA
            sta STACKP
            lda #$1E
            sta LFFFB
            sta TSTAT
            lda #$7F
            sta LDC0D
            sta LDD0C+1
            lda LDC0D
L9238       bit TRIG1
            bpl L9238
L923D       bit TRIG1
            bmi L923D
            lda #$00
            sta GRAFP3
            sta GRAFM
            sta COLPF1
            ldx #$05
L924F       txa
            asl
            tay
            lda #$FC
            sta HPOSP1,Y
            lda #$00
            sta PRIOR,X
            sta LD01F+1,X
            sta LD026+1,X
            dex
            bpl L924F
            stx LDC01+1
            sta LDC03
            lda #$3F
            sta LDD01+1
            lda LDD00
            and #$FC
            ora #$01
            sta LDD00
            lda #$A9
            sta COLPF2
            stx LBFFF
            rts
L9283       lda HOLD1
            and #$03
            cmp #$03
            bne L92A8
            ldx #$02
L928D       clc
            lda LB245,X
            adc #$01
            and #$07
            sta LB245,X
            tay
            lda LB24B,Y
            ldy LB248,X
            sta LD826+2,Y
            sta LD829,Y
            dex
            bpl L928D
L92A8       rts
L92A9       ldx BOOT
            beq L92C9
            ldy LB2A8,X
            lda (DOSVEC),Y
            sta LB253
            iny
            lda (DOSVEC),Y
            sta LB254
            iny
            lda (DOSVEC),Y
            sta LB255
            iny
            lda (DOSVEC),Y
            sta LB256
            dec BOOT
L92C9       rts
L92CA       lda DOSINI+1
            beq L9304
            lda HOLD1
            and #$1F
            bne L9304
            jsr L4444
            cmp #$40
            bcs L9304
            ldx #$0F
            stx BOOT
            and #$03
            tay
            lda LB257,Y
            sta DOSVEC
            lda LB25B,Y
            sta DOSVEC+1
            tya
            and #$01
            tay
            stx CASINI+1,Y
            lda L00A4
            cmp #$12
            beq L9304
            lda LD419+2
            and #$03
            tax
            lda L9305,X
            sta CHBAS
L9304       rts
L9305       ora (RAMLO,X)
            .byte $12,$13
L9309       lda HOLD1
            and #$60
            bne L9323
            lda HOLD1
            lsr
            lsr
            and #$07
            tax
            ldy LB23D,X
            jsr L4444
            and #$07
            ora #$08
            sta LD927+1,Y
L9323       rts
            .byte $32,$64
            stx L00C8,Y
L9328       and #$0F
            tax
            lda L9323,X
            sta CASINI
L9330       rts
L9331       dec CASINI
            bne L9330
            lda #$1F
            bne L936D
L9339       lda CASINI
            bne L9331
            lda L9192+1
            eor #$04
            sta L9192+1
            and #$04
            beq L9330
            ldx #$00
L934B       lda LAB49,X
            sta LAB48,X
            inx
            cpx #$77
            bcc L934B
L9356       ldx #$00
L9358       bpl L9368
L935A       cmp #$FF
            bne L9328
            lda #$27
            sta L9368+1
            lda #$95
            sta L9368+2
L9368       lda L9527
            bmi L935A
L936D       inc L9368+1
            bne L9375
            inc L9368+2
L9375       and #$3F
            tax
            stx L9356+1
            ldy L93AA,X
            beq L9368
            lda #$3E
L9382       sta L9358+1
            lda L945A,Y
            sta LAB6F
            lda L945B,Y
            sta LAB97
            lda L945C,Y
            sta LABBF
            rts
            ldy L93E5,X
            beq L9368
            lda #$47
            bne L9382
            ldy L9420,X
            beq L9368
            lda #$0E
            bne L9382
L93AA       .byte $A9,$37,$3D,$01,$43,$3D,$49,$4F,$55,$58,$5B,$61,$67,$6D,$6D,$70
            .byte $49,$79,$7F,$85,$88,$8E,$91,$67,$97,$9D,$A3,$00,$00,$00,$00,$00
            .byte $A9,$B2,$00,$A9,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$AC,$00
            .byte $01,$07,$0D,$13,$19,$1F,$25,$2B,$31,$34,$AF
L93E5       .byte $A9,$3A,$16,$40,$04,$46,$4C,$52,$58,$00,$5E,$64,$6A,$6D,$3A,$73
            .byte $76,$7C,$82,$22,$8B,$5E,$94,$8E,$9A,$A0,$A6,$00,$00,$00
L9403       .byte $00,$00,$00,$00,$00,$A9,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$04,$0A,$10,$16,$1C,$22,$28,$2E,$16,$04,$00
L9420       .byte $00,$00,$00,$00,$00,$00
L9426       .byte $00,$00,$00,$00,$00,$00,$00,$3A,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$5E,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$A9,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00
L945A       .byte $00
L945B       .byte $01
L945C       .byte $0D,$22,$02,$0E,$23,$03,$0F,$24,$04,$10,$25,$01,$11,$26,$02,$12
            .byte $27,$01,$13,$28,$02,$14,$23,$05,$15,$29,$06,$16,$2A,$07,$17,$2B
            .byte $08,$18,$23,$01,$19,$2C,$08,$1A,$23,$09,$1B,$2D,$0A,$1C,$10,$01
            .byte $1D,$2C,$01,$1E,$2E,$01,$1F,$2F,$02,$0E,$30,$07,$1F,$26,$08,$20
            .byte $27,$07,$0D,$31,$08,$21,$32,$07,$1F,$2F,$08,$36,$42,$01,$37,$22
            .byte $08,$38,$23,$0A,$39,$2F,$0A,$0E,$30,$53,$53,$43,$0A,$0E,$23,$34
            .byte $3A,$44,$0A,$3B,$45,$0A,$0E,$31,$53,$53,$27,$07,$0D,$30,$07,$0D
            .byte $31,$0A,$0E,$46,$02,$12,$53,$07,$3C,$47,$0A,$0E,$48,$07,$3D,$49
            .byte $02,$3E,$30,$01,$3F,$2E,$35,$40,$2D,$08,$41,$10,$0A,$0E,$4A,$0A
            .byte $0E,$4B,$0A,$0E,$4C,$0A,$4D,$2F,$0A,$14,$30,$0A,$4E,$2D,$0A,$12
            .byte $10,$09,$4F,$52,$0A,$50,$27,$53,$53,$53,$53,$53,$51,$53,$51,$51
            .byte $0A,$30,$51,$54,$56,$58,$55,$57,$59,$5A,$5D,$60,$5B,$5E,$61,$5C
            .byte $5F,$62,$63,$66,$69,$64,$67,$6A,$65,$68,$6B
L9527       .byte $23,$23,$50,$41,$52,$41,$44,$52,$4F,$49,$44,$20,$52,$45,$44,$55
            .byte $58,$23,$23,$83
L953B       .byte $23,$23,$23,$23,$23,$23,$23,$23,$23,$44,$45,$53,$49,$47,$4E,$45
            .byte $44,$20,$42,$59,$40,$40,$41,$4E,$44,$52,$45,$57,$20,$42,$52,$41
            .byte $59,$42,$52,$4F,$4F,$4B,$40,$40,$83,$23,$23,$23,$23,$23,$23,$23
            .byte $48,$45,$41,$56,$49,$4C,$59,$20,$4D,$4F,$44,$49,$46,$49,$45,$44
            .byte $20,$42,$59,$40,$54,$4E,$54,$20,$4F,$46,$20,$42,$45,$59,$4F,$4E
            .byte $44,$20,$46,$4F,$52,$43,$45,$40,$81,$23,$23,$23,$23,$23,$23,$23
            .byte $23,$23,$23,$23,$23,$23,$23,$55,$53,$45,$20,$4A,$4F,$59,$53,$54
            .byte $49,$43,$4B,$20,$49,$4E,$20,$50,$4F,$52,$54,$20,$32,$23,$23,$23
            .byte $23,$55,$50,$3A,$20,$4F,$52,$49,$47,$49,$4E,$41,$4C,$20,$47,$52
            .byte $41,$50,$48,$49,$43,$53,$23,$23,$23,$23,$44,$4F,$57,$4E,$3A,$20
            .byte $4D,$45,$54,$41,$4C,$20,$45,$44,$49,$54,$49,$4F,$4E,$23,$23,$23
            .byte $23,$4C,$45,$46,$54,$3A,$20,$4E,$4F,$52,$4D,$41,$4C,$20,$4D,$4F
            .byte $44,$45,$23,$23,$23,$23,$52,$49,$47,$48,$54,$3A,$20,$43,$4F,$4D
            .byte $50,$45,$54,$49,$54,$49,$4F,$4E,$20,$4D,$4F,$44,$45,$23,$23,$23
            .byte $23,$23,$23,$46,$49,$52,$45,$3A,$20,$53,$54,$41,$52,$54,$20,$47
            .byte $41,$4D,$45,$23,$40,$81,$23,$23,$23,$23,$23,$23,$23,$23,$23,$23
            .byte $23,$23,$23,$23,$23,$23,$23,$FF
L9643       .byte $4C,$49,$96
L9646       .byte $4C,$4E,$96,$A9,$11,$85,$90,$60,$A5,$90,$C9,$01,$F0,$1B,$C9,$11
            .byte $D0,$16,$20,$90,$98,$A9,$0F,$8D,$18,$D4,$A9,$01,$85,$F2,$A9,$00
            .byte $8D,$2E,$9B,$A9,$FE,$8D,$2F,$9B,$60,$C6,$F2,$F0,$03,$4C,$2D,$97
            .byte $A9,$05,$85,$F2,$AD,$2F,$9B,$C9,$FE,$90,$34,$F0,$04,$60,$EE,$2E
            .byte $9B,$A2,$00,$AD,$2E,$9B,$0A,$6D,$2E,$9B,$A8,$B9,$60,$99,$8D,$2F
            .byte $9B,$30,$EA,$B9,$60,$99,$84,$A0,$A8,$B9,$A5,$99,$95,$F3,$B9,$C4
            .byte $99,$95,$FC,$A9,$01,$95,$F6,$A4,$A0,$C8,$E8,$E0,$03,$D0,$E4,$A2
            .byte $00,$D6,$F6,$D0,$6D,$86,$9F,$B5,$FC,$85,$A3,$B5,$F3,$85,$A2,$A0
            .byte $00,$B1,$A2,$D0,$10,$C8,$B1,$A2,$29,$07,$95,$F9,$B1,$A2,$4A,$4A
            .byte $4A,$4A,$95,$A4,$C8,$A9,$FF,$D1,$A2,$F0,$A3,$98,$75,$F3,$95,$F3
            .byte $A9,$00,$75,$FC,$95,$FC,$B1,$A2,$4A,$4A,$4A,$4A,$D0,$02,$A9,$10
            .byte $95,$F6,$B5,$F9,$AA,$B1,$A2,$29,$0F,$A8,$B9,$52,$99
L9703       .byte $85,$F0,$B9,$42,$99,$4A,$66,$F0,$CA,$D0,$FA,$A6,$9F,$B4,$A4,$C8
            .byte $94,$91,$BE,$DA,$98,$9D,$E4,$98,$9D,$F0,$98,$A5,$F0,$9D,$E3,$98
            .byte $9D,$EF,$98,$A6,$9F,$E8,$E0,$03,$D0,$8A,$A0,$00,$84,$9F,$B9,$91
            .byte $00,$F0,$0C,$30,$04,$B6,$96,$30,$06,$99,$96,$00,$20,$1D,$98,$A4
            .byte $9F,$A9,$00,$99,$91,$00,$C8,$84,$9F,$C0,$03,$90,$E1,$A2,$00,$86
            .byte $9F,$BD,$C0,$98,$85,$A1,$BD,$C3,$98,$85,$A0,$AA,$B5,$C6,$D0,$03
            .byte $4C,$05,$98,$A4,$A1,$18,$B5,$C0,$75,$C2,$95,$C0,$99,$00,$D4,$B5
            .byte $C1,$75,$C3,$95,$C1,$99,$01,$D4,$18,$B5,$C7,$75,$C9,$95,$C7,$99
            .byte $02,$D4,$B5,$C8,$75,$CA,$95,$C8,$99,$03,$D4,$F0,$04,$C9,$0F,$90
            .byte $11,$38,$B5,$C9,$49,$FF,$69,$00,$95,$C9,$B5,$CA,$49,$FF,$69,$00
            .byte $95,$CA,$A6,$9F,$B4,$99,$F0,$0E,$88,$94,$99,$D0,$09,$B5,$9C,$29
            .byte $FE,$A4,$A1,$99,$04,$D4,$A6,$A0,$D6,$C4,$D0,$53,$B5,$C5,$95,$C4
            .byte $B5,$CB,$C9,$01,$F0,$25,$38,$B5,$C2,$49,$FF,$69,$00,$95,$C2,$B5
            .byte $C3,$49,$FF,$69,$00,$95,$C3,$38,$B5,$C9,$49,$FF,$69,$00,$95,$C9
            .byte $B5,$CA,$49,$FF,$69,$00,$95,$CA,$4C,$F6,$97,$B5,$CC,$95,$C0,$B5
            .byte $CD,$95,$C1,$D6,$C6,$D0,$18,$20,$7B,$98,$B5,$CE,$F0,$04,$A6,$9F
            .byte $95,$91,$A6,$9F,$A9,$00,$95,$96,$E0,$02,$D0,$03,$20,$AA,$98,$A6
            .byte $9F,$E8,$E0,$03,$B0,$03,$4C,$52,$97,$60,$A6,$9F,$BD,$C0,$98,$85
            .byte $A1,$B6,$96
L9826       .byte $BD,$DA,$98,$A0,$00,$18,$69,$E2,$85,$A2,$98,$69,$98,$85,$A3,$B1
            .byte $A2,$AA,$20,$7B,$98,$99,$00,$D4,$99,$01,$D4,$99,$02,$D4,$A9,$08
            .byte $99,$03,$D4,$BD,$C7,$98,$99,$04,$D4,$48,$BD,$CC,$98,$99,$05,$D4
            .byte $BD,$D1,$98,$99,$06,$D4,$BD,$D6,$98,$A6,$9F,$95,$99,$68,$95,$9C
            .byte $09,$01,$99,$04,$D4,$A4,$9F,$BE,$C4,$98,$A0,$0F,$B1,$A2,$95,$BF
            .byte $CA,$88,$D0,$F8,$60,$A4,$A1,$A9,$00,$99,$06,$D4,$99,$05,$D4,$A9
            .byte $08,$99,$04,$D4,$A9,$00,$99,$04,$D4,$60,$29,$0F,$85,$90,$A0,$18
            .byte $A9,$00,$99,$00,$D4,$88,$10,$FA,$85,$91,$85,$92,$85,$93,$85,$96
            .byte $85,$97,$85,$98,$A9,$FF,$8D,$0E,$D4,$8D,$0F,$D4,$A9,$00,$8D,$13
            .byte $D4,$8D,$14,$D4,$A9,$81,$8D,$12,$D4,$60,$00,$07,$0E,$00,$0F,$1E
            .byte $2D,$40,$40,$40,$80,$80,$1D,$7C,$10,$09,$07,$7C,$00,$A2,$00,$00
            .byte $40,$40,$20,$40,$40,$00,$10,$20,$30,$40,$50,$60,$00,$68,$09,$00
            .byte $00,$F0,$01,$01,$00,$08,$10,$00,$00,$68,$09,$00,$01,$B4,$04,$00
            .byte $00,$FF,$00,$01,$80,$07,$04,$00,$00,$B4,$04,$00,$02,$31,$1C,$10
            .byte $00,$03,$06,$FF,$00,$08,$00,$00,$00,$31,$1C,$00,$03,$68,$09,$00
            .byte $00,$40,$00,$01,$00,$00,$00,$00,$00,$68,$09,$00,$04,$C1,$2C,$00
            .byte $00,$20,$00,$01,$00,$00,$00,$00,$00,$C1,$2C,$00,$01,$D1,$12,$08
            .byte $00,$03,$06,$FF,$00,$06,$D0,$00,$00,$D1,$12,$00,$6A,$70,$77,$7E
            .byte $86,$8E,$96,$9F,$A8,$B3,$BD,$C8,$D4,$E1,$EE,$FD,$73,$C7,$7C,$97
            .byte $1E,$18,$8B,$7E,$FA,$06,$AC,$F3,$E6,$85,$00,$0C,$17,$00,$0C,$17
            .byte $01,$0D,$18,$02,$0D,$18,$01,$0D,$18,$02,$0E,$18,$03,$0F,$18,$04
            .byte $10,$18,$03,$0F,$18,$04,$10,$18,$05,$11,$18,$06,$10,$19,$07,$12
            .byte $19,$08,$13,$18,$05,$11,$1A,$06,$10,$18,$07,$12,$18,$08,$0F,$1B
            .byte $09,$14,$1C,$01,$15,$1D,$0A,$16,$1D,$0B,$11,$1E,$FF,$FF,$FF,$E3
            .byte $E7,$EC,$FA,$13,$1A,$2B,$3C,$4B,$5A,$61,$68,$6A,$6E,$7C,$8C,$94
            .byte $98,$9C,$A8,$B3,$C2,$C7,$CD,$D1,$E2,$F4,$02,$1C,$23,$2A,$99,$99
            .byte $99,$99,$9A,$9A,$9A,$9A,$9A,$9A,$9A,$9A,$9A,$9A,$9A,$9A,$9A,$9A
            .byte $9A,$9A,$9A,$9A,$9A,$9A,$9A,$9A,$9A,$9B,$9B,$9B,$9B,$00,$16,$09
            .byte $FF,$69,$29,$49,$49,$FF,$00,$16,$69,$00,$15,$29,$00,$14,$24,$29
            .byte $00,$15,$44,$FF,$00,$16,$29,$00,$15,$29,$00,$16,$29,$00,$14,$24
            .byte $00,$16,$2B,$00,$15,$2B,$00,$16,$2B,$00,$14,$26,$FF,$00,$15,$61
            .byte $21,$41,$41,$FF,$00,$15,$66,$00,$14,$16,$38,$00,$15,$26,$00,$14
            .byte $26,$00,$15,$26,$FF,$00,$15,$66,$00,$14,$16,$38,$00,$15,$26,$00
            .byte $14,$26,$00,$15,$28,$FF,$69,$00,$14,$19,$3B,$00,$15,$29,$00,$14
            .byte $29,$00,$15,$29,$FF,$69,$00,$14,$19,$3B,$00,$15,$29,$00,$14,$29
            .byte $00,$15,$2B,$FF,$2D,$2B,$28,$21,$44,$44,$FF,$00,$15,$66,$26,$46
            .byte $46,$FF,$06,$FF,$00,$54,$09,$FF,$00,$53,$69,$00,$52,$44,$00,$53
            .byte $29,$00,$52,$24,$29,$FF,$49,$28,$00,$53,$29,$00,$52,$26,$00,$53
            .byte $29,$00,$52,$28,$24,$FF,$2D,$21,$2B,$21,$29,$28,$46,$FF,$00,$52
            .byte $01,$FF,$00,$53,$06,$FF,$00,$23,$4D,$2B,$2D,$00,$22,$26,$24,$23
            .byte $24,$FF,$00,$23,$4D,$2B,$2D,$00,$22,$46,$24,$23,$FF,$28,$2B,$2D
            .byte $2B,$1D,$1B,$18,$14,$11,$00,$53,$1B,$19,$14,$FF,$61,$21,$41,$41
            .byte $FF,$61,$21,$44,$29,$28,$FF,$00,$06,$09,$FF,$00,$34,$46,$00,$42
            .byte $29,$29,$00,$35,$46,$00,$42,$19,$19,$1F,$1F,$FF,$00,$34,$46,$00
            .byte $42,$19,$19,$19,$19,$00,$34,$26,$26,$00,$42,$29,$29,$FF,$00,$42
            .byte $19,$19,$13,$13,$00,$34,$46,$26,$26,$26,$26,$FF,$00,$34,$26,$00
            .byte $42,$19,$19,$00,$34,$26,$00,$42,$19,$19,$00,$34,$26,$00,$42,$19
            .byte $19,$00,$34,$26,$26,$FF,$00,$04,$6D,$2D,$4D,$4D,$FF,$00,$04,$69
            .byte $29,$49,$49,$FF,$00,$04,$06,$FF
L9B2E       .byte $00
L9B2F       .byte $00
L9B30       .byte $A2,$63,$BD,$59,$9B,$9D,$E0,$A9,$BD,$BD,$9B,$9D,$44,$AA,$A9,$08
            .byte $9D,$E0,$D9,$9D,$44,$DA,$CA,$10,$E9,$A2,$4F,$A9,$00,$9D,$A8,$AA
            .byte $CA,$10,$FA,$A9,$2C,$8D,$ED,$91,$60,$C3,$C3,$C3,$C3,$C3,$B4,$C3
            .byte $C3,$B4,$C3,$C3,$C3,$C3,$C3,$C3,$C3,$B7,$BB,$B7,$B7,$B7,$B7,$B7
            .byte $C4,$C5,$C6,$B7,$B7,$BC,$B7,$C3,$C3,$BD,$C3,$C3,$BD,$C3,$C3,$C3
            .byte $C3,$C7,$C8,$C7,$C7,$C9,$CA,$CB,$C9,$CA,$CB,$C7,$CC,$C7,$C7,$C8
            .byte $C7,$CD,$BB,$B7,$B7,$B7,$B7,$B7,$CE,$CF,$D0,$B7,$B7,$BC,$D1,$C7
            .byte $C7,$B4,$C7,$C7,$B4,$C7,$C7,$C7,$CC,$D2,$D3,$D2,$D2,$D2,$D3,$D2
            .byte $D2,$D2,$D3,$D2,$D2,$D2,$D3,$D2,$D2,$D2,$D3,$D2,$D2,$D2,$D3,$D2
            .byte $D4,$D5,$D6,$D2,$D2,$D2,$D3,$D2,$D2,$B4,$D3,$D2,$B4,$D2,$D3,$D2
            .byte $D2,$C7,$C7,$C7,$C7,$CC,$C7,$C7,$C7,$C8,$C7,$C7,$C7,$C7,$CC,$C7
            .byte $C7,$C7,$C8,$C7,$C7,$C7,$C7,$CC,$C7,$C7,$C7,$C8,$C7,$C7,$C7,$D7
            .byte $C9,$CA,$CB,$C9,$CA,$CB,$C7,$C7,$C7,$D2,$D2,$D8,$D2,$D2,$D2,$D2
            .byte $D8,$D2,$D2,$D2,$D2,$D8,$D2,$D2,$D2,$D2,$D8,$D2,$D2,$D2,$D2,$D8
            .byte $D2,$D2,$D2,$D2,$D8,$D2,$D2,$D2,$D2,$D8,$D2,$D2,$D2,$D2,$D8,$D2
            .byte $D2
L9C21       .byte $A5,$06,$0A,$AA,$BD,$34,$9C,$8D,$32,$9C,$BD,$35,$9C,$8D,$33,$9C
            .byte $4C,$31,$9C,$44,$9C,$7A,$9C,$96,$9C,$96,$9C,$FC,$9C,$62,$9D,$A2
            .byte $9D,$05,$9E,$A6,$08,$E0,$3F,$B0,$1A,$A5,$07,$18,$69,$60,$85,$07
            .byte $90,$10,$A0,$08,$B9,$71,$9C,$9D,$00,$AC,$CA,$30,$03,$88,$10,$F4
            .byte $E6,$08,$60,$E0,$BC,$90,$F9,$A9,$00,$85,$08,$85,$07,$E6,$06,$60
            .byte $00,$08,$0A,$07,$0D,$03,$0E,$04,$06,$E6,$08,$A5,$08,$C9
L9C7F       .byte $01,$D0,$0A,$20,$14,$9E,$20,$35,$9E,$20,$51,$9E,$60,$C9,$E2,$D0
            .byte $FB,$20,$06,$9E,$4C,$68,$9C,$E6,$08,$A5,$08,$C9,$7F,$B0,$49,$4A
            .byte $90,$10,$AA,$A0,$08,$B9,$F3,$9C,$9D,$00,$AC,$CA,$30,$03,$88,$10
            .byte $F4,$60,$E9,$34,$49,$FF,$18,$69,$01,$AA,$E0,$1D,$F0,$1D,$B0,$04
            .byte $E0,$13,$B0,$16,$A0,$00,$E0,$36,$B0,$06,$B9,$EA,$9C,$9D,$00,$AC
            .byte $E8,$C8,$C0,$09,$F0,$04,$C0,$12,$D0,$EC,$60,$A9,$00,$A0,$09,$9D
            .byte $00,$AC,$E8,$88,$D0,$F9,$F0,$F2,$4C,$68,$9C,$01,$0E,$03,$0E,$04
            .byte $06,$04,$06,$00,$00,$0D,$01,$0D,$03,$05,$03,$05,$01,$E6,$08,$A5
            .byte $08,$C9,$3F,$B0,$49,$4A,$90,$10,$AA,$A0,$08,$B9,$59,$9D,$9D,$00
            .byte $AC,$CA,$30,$03,$88,$10,$F4,$60,$E9,$34,$49,$FF,$18,$69,$01,$AA
            .byte $E0,$1D,$F0,$1D,$B0,$04,$E0,$13,$B0,$16,$A0,$00,$E0,$36,$B0,$06
            .byte $B9,$50,$9D,$9D,$00,$AC,$E8,$C8,$C0,$09,$F0,$04,$C0,$12,$D0,$EC
            .byte $60,$A9,$00,$A0,$09,$9D,$00,$AC,$E8,$88,$D0,$F9,$F0,$F2,$4C,$68
            .byte $9C,$01,$0E,$03,$0E,$04,$06,$04,$06,$00,$00,$0D,$01,$0D,$03,$05
            .byte $03,$05,$01,$E6,$08,$A5,$08,$C9,$01,$D0,$03,$4C,$51,$9E,$C9,$05
            .byte $90,$30,$D0,$03,$4C,$35,$9E,$C9,$09,$90,$27,$D0,$03,$4C,$14,$9E
            .byte $C9,$B0,$90,$1E,$D0,$03,$4C,$11,$9E,$C9,$B4,$90,$15,$D0,$03,$4C
            .byte $32,$9E,$C9,$B8,$90,$0C,$D0,$03,$4C,$4E,$9E,$C9,$E1,$90,$03,$4C
            .byte $68,$9C,$60,$E6,$08,$A5,$08,$C9,$EE,$B0,$1C,$A2,$34,$BD,$00,$AC
            .byte $9D,$01,$AC,$CA,$10,$F7,$A2,$23,$BD,$E1,$9D,$8D,$00,$AC,$CA,$10
            .byte $02,$A2,$23,$8E,$B6,$9D,$60,$A9,$00,$85,$08,$E6,$07,$A5,$07,$C9
            .byte $02,$90,$D8,$20,$06,$9E,$20,$14,$9E,$20,$35,$9E,$20,$51,$9E,$4C
            .byte $68,$9C,$00,$00,$00,$00,$06,$04,$0E,$03,$01,$0A,$08,$02,$00,$00
            .byte $00,$00,$02,$08,$0A,$01,$0D,$03,$05,$05,$00,$00,$00,$00,$05,$0D
            .byte $03,$01,$03,$0E,$04,$06,$60,$A2,$35,$A9,$00,$9D,$00,$AC,$CA,$10
            .byte $FA,$60,$A2,$08,$2C,$A2,$00,$E8,$A0,$01,$BD,$62,$9E,$99,$31,$AC
            .byte $E8,$C8,$C0,$05,$90,$F4,$BD,$62,$9E,$99,$FB,$AB,$E8,$C8,$C0,$08
            .byte $90,$F4,$60,$A2,$08,$2C,$A2,$00,$A0,$01,$E8,$BD,$62,$9E,$99,$08
            .byte $AC,$99,$26,$AC,$E8,$C8,$C0,$03,$F0,$F0,$C0,$07,$D0,$ED,$60,$A2
            .byte $08,$2C,$A2,$00,$A0,$00,$BD,$62,$9E,$99,$17,$AC,$E8,$C8,$C0,$08
            .byte $D0,$F4,$60,$09,$08,$07,$01,$07,$07,$08,$09,$00,$00,$00,$00,$00
            .byte $00,$00,$00
L9E72       .byte $FF
L9E73       .byte $FF
L9E74       .byte $FF
L9E75       ldx L9EC5
            bne L9EA8
            lda LDBFF+1
            eor #$1F
            and #$1F
            beq L9EC4
            ldx #$04
L9E85       lsr
            bcs L9E8B
            dex
            bpl L9E85
L9E8B       bne L9EC4
            cpx L9E74
            beq L9EC4
            stx L9E74
            dex
            bmi L9EC4
            txa
            tay
            lsr
            tax
            ror L9E72,X
            jsr L9EC6
            lda #$37
            sta L9EC5
            rts
L9EA8       dec L9EC5
            txa
            cmp #$33
            bcc L9EB4
            sbc #$2E
            bcs L9EB8
L9EB4       cmp #$03
            bcs L9EC4
L9EB8       tax
            lda LB29E,X
            ldx #$05
L9EBE       sta LD026+1,X
            dex
            bpl L9EBE
L9EC4       rts
L9EC5       .byte $00
L9EC6       lda L9F3C,Y
            ldx #$00
L9ECB       sta HPOSP0,X
            clc
            adc #$18
            inx
            inx
            cpx #$0C
            bcc L9ECB
            lda #$00
            ldx #$17
L9EDB       sta LB000,X
            sta LB040,X
            sta LB080,X
            sta LB0C0,X
            sta LB100,X
            sta LB140,X
            sta LB180,X
            sta LB1C0,X
            dex
            bpl L9EDB
            sta APPMHI
            lda #$B0
            sta APPMHI+1
            lda L9F38,Y
            tay
L9F00       lda LB200,Y
            bmi L9F34
            sty L9F1B+1
            asl
            asl
            asl
            ora #$07
            tax
            ldy #$15
L9F10       lda LB2E9,X
            sta (APPMHI),Y
            dex
            dey
            dey
            dey
L9F19       bpl L9F10
L9F1B       ldy #$00
L9F1D       iny
            inc APPMHI
            lda APPMHI
            and #$3F
            cmp #$03
            bcc L9F00
            eor APPMHI
            adc #$3F
            sta APPMHI
            bcc L9F00
            inc APPMHI+1
            bne L9F00
L9F34       asl
            beq L9F1D
            rts
L9F38       .byte $0C,$00,$2F,$1D
L9F3C       .byte $77,$8B,$83,$73,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
LA006       .byte $FF,$FF,$00,$15,$40,$3F,$C0,$15,$55,$6A,$00,$00,$40,$10,$E4,$24
            .byte $64,$64,$00,$04,$11,$4E,$32,$C6,$16,$66,$00,$00,$00,$40,$40,$40
            .byte $40,$40,$00,$01,$04,$13,$4C,$31,$C5,$19,$00,$00,$40,$90,$90,$90
            .byte $90,$90,$00,$55,$00,$FF,$00,$55,$55,$6A,$00,$40,$10,$E4,$24,$64
            .byte $64,$A4,$00,$55,$00,$FF,$00,$55,$55,$AA,$00,$40,$10,$E4,$24,$64
            .byte $64,$64,$AA,$55,$00,$FF,$00,$00,$00,$00,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$7F,$6A,$65,$64,$64,$64,$64,$64,$64,$64,$64,$64,$64,$64
            .byte $64,$64,$B6,$E6,$56,$06,$06,$06,$06,$06,$40,$40,$40,$40,$40,$40
            .byte $40,$40,$BF,$EA,$55,$00,$15,$6A,$7F,$6A,$64,$64,$64,$64,$A4,$E4
            .byte $90,$40,$BF,$EA,$55,$00,$15,$2A,$3F,$1A,$64,$64,$64,$A4,$E4,$14
            .byte $60,$64,$6D,$79,$65,$51,$55,$A9,$FD,$69,$90,$90,$90,$10,$60,$A4
            .byte $E4,$A4,$7F,$6A,$55,$40,$55,$AA,$FF,$6A,$E4,$A4,$54,$00,$00,$60
            .byte $64,$64,$7F,$6A,$55,$40,$55,$6A,$7F,$6A,$E4,$A4,$54,$00,$80,$50
            .byte $64,$64,$FF,$6A,$15,$05,$06,$06,$06,$06,$64,$64,$64,$B4,$E4,$90
            .byte $40,$40,$7F,$6A,$65,$80,$55,$2A,$7F,$6A,$7F,$6A,$65,$40,$95,$EA
            .byte $7F,$1A,$7F,$6A,$65,$40,$55,$6A,$7F,$6A,$E4,$A4,$54,$00
LA104       .byte $00,$00,$00,$00,$E4,$A4,$54,$00,$40,$90,$90,$90,$64,$40,$55,$95
            .byte $EA,$7F,$1A,$05,$64,$64,$64,$A4,$E4,$A4,$90,$40,$06,$06,$55,$55
            .byte $AA,$FF,$6A,$15,$40,$40,$50,$64,$A4,$E4,$A4,$54,$65,$40,$55,$55
            .byte $AA,$FF,$6A,$15,$00,$00,$50,$64,$A4,$E4,$A4,$54,$05,$40,$55,$95
            .byte $EA,$7F,$1A,$05,$15,$01,$01,$01,$02,$03,$01,$00,$94,$90,$90,$90
            .byte $90,$90,$90,$50,$15,$00,$55,$55,$AA,$FF,$6A,$15,$65,$40,$55,$95
            .byte $EA,$7F,$1A,$05,$06,$06,$06,$06,$0A,$0E,$06,$01,$05,$00,$55,$55
            .byte $AA,$FF,$6A,$15,$65,$64,$64,$64,$A4,$E4,$64,$14,$64,$64,$64,$64
            .byte $A4,$E4,$64,$14,$64,$40,$55,$55,$AA,$FF,$6A,$15,$50,$00,$50,$64
            .byte $A4,$E4,$A4,$54,$00,$00,$00,$00,$55,$AA,$AA,$AA,$00,$40,$10,$E1
            .byte $20,$67,$60,$5D,$00,$55,$00,$FF,$00,$55,$55,$A6,$E4,$A4,$54,$00
            .byte $80,$90,$90,$90,$7F,$6A,$65,$64,$65,$66,$67,$64,$E4,$A4,$54,$00
            .byte $60,$64,$64,$64,$64,$64,$64,$40,$55,$6A,$7F,$6A,$41,$45,$56,$57
            .byte $56,$55,$65,$75,$A0,$D0,$80,$40,$00,$00,$00,$00,$7F,$6A,$65,$64
            .byte $64,$64,$64,$61,$7F,$6A,$65,$40,$55,$69,$7E,$6B,$64,$64,$64,$64
            .byte $A4,$E4,$54,$60,$7F,$6A,$55,$40,$95,$EA,$7F,$1A,$F7,$66,$16,$06
            .byte $06,$06,$06,$06,$E4,$A4,$54,$40,$40,$40,$40,$40,$50,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$40,$55,$95,$EA,$7F,$1A,$05,$69,$6D,$66,$67
            .byte $A5,$E4,$64,$14,$00,$40,$50,$54,$94,$E4,$64,$14,$64,$64,$64,$64
            .byte $A4,$E4,$A4,$54,$45,$55,$59,$6E,$BB,$E6,$50,$00,$A4,$24,$64,$64
            .byte $A4,$E4,$A4,$14,$66,$64,$64,$64,$A4,$E4,$64,$14,$64,$40,$55,$55
            .byte $AA,$FF,$AA,$55,$44,$51,$95,$E6,$7B,$1E,$05,$00,$64,$64,$A4,$E4
            .byte $90,$40,$00,$00,$64,$64,$44,$91,$D5,$19,$6E,$7B,$64,$64,$44,$51
            .byte $95,$E6,$77,$16,$FF,$6A,$15,$05,$16,$5B,$6E,$79,$64,$64,$64,$B4
            .byte $E4,$90,$40,$00,$00,$00,$50,$64,$A4,$E4,$64,$14,$64,$64,$55,$55
            .byte $AA,$FF,$6A,$15,$00,$00,$00,$00,$00,$00,$00,$00,$00,$01,$04,$03
            .byte $0C,$01,$05,$05,$00,$00,$40,$10,$E4,$24,$64,$64,$09,$05,$01,$01
            .byte $01,$01,$01,$11,$A4,$94,$90,$90,$90,$90,$90,$90,$45,$11,$55,$55
            .byte $AA,$FF,$6A,$15,$50,$04,$59,$59,$A9,$F9,$A9,$55,$01,$00,$03,$00
            .byte $02,$00,$01,$01,$54,$01,$FE,$02,$5A,$52,$56,$56,$00,$00,$40,$40
            .byte $40,$40,$40,$40,$02,$01,$15,$41,$15,$55,$55,$55,$5A,$5A,$55,$54
            .byte $55,$55,$55,$55,$40,$40,$40,$10,$44,$59,$59,$59,$59,$59,$59,$59
            .byte $AA,$FB,$69,$14,$56,$56,$56,$56,$AA,$FE,$AA,$55,$59,$59,$59,$59
            .byte $A9,$F9,$69,$15,$00,$05,$00,$0F,$00,$05,$05,$06,$00,$55,$00,$FF
            .byte $00,$55,$55,$AA,$00,$40,$10,$E4,$24,$64,$64,$64,$07,$06,$06,$06
            .byte $06,$06,$06,$16,$FF,$AA,$55,$40,$40,$40,$40,$41,$64,$64,$64,$64
            .byte $64,$64,$64,$64,$46,$16,$56,$9A,$EE,$39,$04,$00,$44,$41,$45,$49
            .byte $4E,$03,$00,$00,$64,$64,$64,$A4,$E4,$90,$40,$00,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FA,$EA,$FF,$FF,$FF,$FF,$FF,$FF,$AA,$AA,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$AA,$AA,$FF,$FF,$FF,$FF,$FF,$FF,$AF,$AB,$FA,$EA,$DA,$DA
            .byte $D6,$D6,$D5,$D5,$AA,$AA,$FE,$FE,$FE,$BE,$AA,$AA,$AA,$AA,$BF,$BF
            .byte $BF,$BE,$AA,$AA,$AF,$AB,$A7,$A7,$97,$97,$57,$57,$EA,$EA,$DA,$D6
            .byte $D5,$D5,$D5,$D4,$FE,$FE,$BE,$AA,$AA,$55,$00,$00,$BF,$BF,$BE,$AA
            .byte $AA,$55,$00,$00,$AB,$AB,$A7,$97,$57,$57,$57,$17,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FE,$D5,$D5,$D0,$C0,$FF,$FF,$FF,$95,$55,$00,$01,$01
            .byte $02,$06,$06,$06,$55,$00,$90,$90,$E0,$E4,$E4,$E4,$57,$57,$07,$03
            .byte $FF,$FF,$FF,$56,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$BF,$D0,$C0,$F0,$FF
            .byte $FF,$FF,$FF,$95,$01,$01,$06,$06,$06,$06,$06,$06,$90,$90,$E0,$E4
            .byte $E4
LA405       .byte $E4,$E4,$E4,$07,$03,$0F,$FF,$FF,$FF,$FF,$56,$FA,$FA,$F6,$F5,$E9
            .byte $AA,$95
LA417       .byte $55,$B5,$9B,$89,$A8,$6A,$55,$5D,$59,$46,$96,$A5,$AA,$AA,$55,$FF
            .byte $AA,$E5,$E6,$5A,$AA,$AA,$55,$FF,$AA,$5E,$E6,$62,$2A,$A9,$55,$75
            .byte $65,$AF,$AF,$9F,$5F,$6B,$AA,$56,$55,$40,$18,$19,$19,$19,$19,$19
            .byte $19,$51,$5D,$55,$40,$00,$00,$00,$00,$00,$FF,$55,$00,$00,$01,$01
            .byte $01,$00,$FF,$55,$00,$00,$90,$90,$90,$45,$75,$55,$01,$00,$00,$00
            .byte $00,$01,$19,$19,$19,$19,$19,$19,$19,$19,$19,$19,$19,$19,$19,$15
            .byte $00,$00,$FF,$FF,$FF,$FF,$55,$55,$AA,$02,$06,$06,$06,$06,$06,$46
            .byte $96,$E0,$E4,$E4,$E4,$E4,$E4,$E5,$E6,$00,$FF,$FF,$FF,$FF,$55,$55
            .byte $AA,$19,$19,$19,$19,$19,$19,$15,$00,$FF,$FF,$FF,$FF,$FE,$FA,$EA
            .byte $AA,$FF,$FF,$FF,$FF,$BF,$AF,$9F,$4F,$FF,$FD,$F6,$DA,$6A,$69,$68
            .byte $6A,$57,$A9,$9A,$86,$A2,$AA,$6A,$2A,$FF,$FF,$7F,$9F,$A7,$A9,$AA
            .byte $AA,$FF,$FF,$FF,$FF,$FF,$FF,$7F,$9F,$BF,$6E,$6E,$6E,$6E,$6E,$6E
            .byte $6E,$6A,$5A,$55,$50,$40,$00,$04,$05,$55,$AA,$AA,$FF,$AA,$55,$65
            .byte $45,$55,$AA,$AA,$FF,$AA,$55,$69,$16,$55,$AA,$AA,$FF,$AA,$55,$55
            .byte $55,$BF,$6E,$6E,$6E,$6E,$6E,$6E,$6E,$A9,$64,$50,$50,$50,$50,$50
            .byte $90,$05,$0A,$0A,$0A,$0A,$0A,$6A,$1A,$FF,$7F,$9F,$A7,$A9,$A9,$A9
            .byte $A9,$A5,$AA,$55,$00,$00,$00,$00,$00,$5A,$AA,$55,$00,$00,$00,$00
            .byte $00,$A9,$A5,$55,$05,$01,$00,$04,$05,$6E,$22,$1A,$0A,$06,$02,$01
            .byte $00,$AA,$AE,$A2,$AA,$AA,$AE,$A2,$55,$AA,$AA,$AE,$E2,$2A,$AE,$A2
            .byte $55,$A5,$AA,$EE,$22,$AA,$EE,$22,$55,$19,$04,$C0,$19,$19,$19,$19
            .byte $45,$55,$AA,$EE,$22,$AA,$EE,$22,$55,$55,$65,$45,$55,$65,$45,$AA
            .byte $55,$16,$16,$41,$55,$55,$55,$AA,$55,$55,$55,$69,$81,$01,$15,$2A
            .byte $55,$5B,$B8
LA57A       .byte $8A,$AB,$A8,$AA,$AA,$55,$A2,$AA,$AA,$AA,$AA,$AA,$AA,$55,$86,$A1
            .byte $A8,$AA,$AA,$AA,$AA,$55,$A9,$A8,$64,$10,$80,$80,$40,$00,$19,$04
            .byte $C0,$6E,$6E,$6E,$6E,$EF,$19,$19,$19,$19,$19,$19,$19,$19,$19,$19
            .byte $19,$19,$19,$19,$19,$19,$19,$04,$83,$B6,$B6,$B6,$B6,$FB,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$AA,$95,$90,$90,$A8,$BC,$90,$90,$AA,$55
            .byte $00,$00,$28,$3C,$00,$00,$AA,$56,$02,$02,$2A,$3E,$02,$02,$A8,$BC
            .byte $90,$90,$A8,$BC,$90,$90,$2A,$3E,$02,$02,$2A,$3E,$02,$02,$19,$19
            .byte $19,$15,$00,$6A,$19,$19,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$55,$AA
            .byte $AA,$AA,$AA,$AA,$AA,$55,$55,$73,$73,$55,$40,$40,$6A,$55,$55,$3B
            .byte $37,$5F,$1F,$1A,$A1,$51,$55,$7D,$ED,$DD,$FD,$AD,$69,$65,$00,$FF
            .byte $FF,$FF,$FF,$00,$55,$AA,$40,$7F,$40,$7F,$40,$6A,$6A
LA627       .byte $5A,$01,$F0,$FF,$3F,$CF,$AA,$AA,$55,$61,$0D,$FD,$ED,$DD,$A9,$A9
            .byte $A5,$AA,$AA,$AA,$AA,$AA,$AA,$AA,$AA,$AA,$A6,$A2,$AA,$AA,$AA,$AA
            .byte $AA,$AA,$A5,$95,$95,$85,$A0,$AA,$AA,$19,$19,$19,$59,$55,$55,$00
            .byte $AA,$AA,$5A,$56,$56,$52,$0A,$AA,$AA,$AA,$6A,$2A,$AA,$AA,$AA,$AA
            .byte $AA,$00,$00,$80,$80,$A0,$A0,$A8,$A8,$05,$50,$55,$55,$55,$55,$55
            .byte $55,$00,$55,$55,$55,$55,$55,$55,$55,$50,$05,$55,$55,$55,$55,$55
            .byte $55,$00,$00,$02,$02,$0A,$0A,$2A,$2A,$AA,$AA,$AA,$AA,$AA,$55,$00
            .byte $00,$AA,$A9,$A8,$AA,$AA,$55,$00,$00,$65,$45,$55,$00,$AA,$55,$00
            .byte $00,$55,$55,$55,$00,$AA,$55,$00,$00,$59,$51,$55,$00,$AA,$55,$00
            .byte $00,$AA,$A9,$A8,$AA,$AA,$AA,$AA,$AA,$AA,$9A,$8A,$AA,$AA,$55,$00
            .byte $00,$FF,$FE,$FC,$F8,$F0,$E0,$C0,$80,$01,$03,$07,$0F,$1F,$3F,$7F
            .byte $FF,$80,$C0,$E0,$F0,$F8,$FC,$FE,$FF,$FF,$7F,$3F,$1F,$0F,$07,$03
            .byte $01,$00,$00,$00,$00,$00,$00,$00,$00,$FF,$FE,$FC,$F8,$F8,$FC,$FE
            .byte $FF,$01,$03,$07,$0F,$0F,$07,$03,$01,$00,$00,$00,$00,$00,$00,$FF
            .byte $FF,$FF,$7F,$3F,$1F,$1F,$3F,$7F,$FF,$03,$39,$39,$03,$27,$33,$39
            .byte $3D,$C7,$93,$39,$39,$39,$01,$39,$39,$03,$39,$3F,$07,$3F,$3F,$3F
            .byte $3F,$81,$27,$E7,$E7,$E7,$E7,$E7,$E7,$C3,$99,$3F,$3F,$21,$39,$99
            .byte $C3,$C7,$93,$39,$39,$39,$39,$93,$C7,$3F,$3F,$3F,$3F,$3F,$3F,$39
            .byte $03,$07,$33,$39,$39,$39,$39,$33,$07,$FF,$FF,$FF,$FF,$FF,$FF,$CF
            .byte $CF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$6C,$6D,$6E,$6F,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$70,$71,$72,$73,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$74,$75
            .byte $76,$77,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$78,$79,$7A
            .byte $7B,$7C,$7D,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$78,$7E,$7F,$80,$81,$7D,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$82,$83,$84,$85,$86,$87,$00,$00,$00,$00,$00
            .byte $00,$6C,$6D,$6E,$6F,$00,$00,$00,$00,$00,$00,$00,$00,$82,$83,$84
            .byte $85,$86,$87,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$88,$89,$8A
            .byte $8B,$8C,$8D,$00,$00,$00,$00,$00,$00,$74,$75,$76,$77,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$88,$89,$8A,$8B,$8C,$8D,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$8E,$8F,$90,$91,$92,$93,$00,$94,$95,$00,$00
            .byte $78,$7E,$7F,$80,$81,$7D,$00,$00,$00,$00,$00,$00,$00,$8E,$8F,$90
            .byte $91,$92,$93,$00,$00,$00,$00,$00,$00,$96,$97,$98,$99,$9A,$9B,$9C
            .byte $9D,$9E,$9F,$00,$A0,$A1,$A2,$00,$82,$83,$84,$85,$86,$87,$00,$00
            .byte $00,$00,$00,$00,$00,$9A,$9B,$A3,$A4,$A5,$9F,$00,$00,$00,$00,$00
            .byte $00,$A6,$A7,$A8,$A9,$AA,$AB,$AC,$AD,$AE,$AB,$AF,$B0,$B1,$B2,$00
            .byte $88,$89,$8A,$8B,$8C,$8D,$00,$00,$00,$00,$00,$00,$00,$B3,$B4,$00
            .byte $00,$B5,$B6,$00,$00,$00,$00,$00,$00,$B7,$B8,$B9,$B9,$B9,$B9,$B9
            .byte $B9,$B9,$B9,$B9,$B9,$BA,$B7,$00,$8E,$8F,$90,$91,$92,$93,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$B4,$00,$00,$B4,$00,$00,$00,$00,$00,$00
            .byte $00,$B7,$BB,$B7,$B7,$B7,$B7,$B7,$B7,$B7,$B7,$B7,$B7,$BC,$B7,$00
            .byte $9A,$9B,$A3,$A4,$A5,$9F,$00,$00,$00,$00,$00,$00,$00,$00,$BD,$00
            .byte $00,$BD,$00,$00,$00,$00,$00,$00,$00,$B7,$BB,$B7,$B7,$B7,$B7,$B7
            .byte $B7,$B7,$B7,$B7,$B7,$BC,$B7,$00,$B3,$B4,$00,$BE,$B5,$B6,$00,$00
            .byte $00,$BF,$BF,$BF,$BF,$BF,$B4,$BF,$BF,$B4,$BF,$BF,$BF,$BF,$BF,$BF
            .byte $BF,$B7,$BB,$B7,$B7,$B7,$B7,$B7,$C0,$C1,$C2,$B7,$B7,$BC,$B7,$BF
            .byte $BF,$B4,$BF,$BF,$B4,$BF,$BF,$BF,$BF,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$D9,$DC,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$D9,$DA,$DB,$DC
            .byte $00,$00,$00,$00,$00
LAA1C       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00
LAA26       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$D9,$DA,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$DE,$DF,$DE,$DD,$DD
            .byte $DD,$DD,$E1,$E2,$E3,$E4,$E5,$E6,$E7,$E8,$E9,$00,$E8,$E5,$E9,$EA
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$DB,$DC,$00,$00,$D9,$DA,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$DB,$DC,$D9
            .byte $DA,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$E0,$E0,$E0,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$DB,$DA,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$0B,$0B,$0B,$0B,$0B,$0B
            .byte $0B,$0B,$0B,$0B,$0B,$0B,$0B,$0B,$0B,$0B,$0B,$0B,$0B,$0B,$0B,$0B
            .byte $0B,$0B,$0B,$0B,$0B,$0B,$0B,$0B,$0B,$0B,$0B,$0B,$0B,$0B,$0B,$0B
            .byte $0B,$0B
LAB48       .byte $53
LAB49       .byte $53,$53,$53,$53,$53,$53,$53,$53,$53,$53,$53,$53,$53,$53,$53,$53
            .byte $53,$53,$53,$53,$53,$53,$53,$53,$53,$53,$53,$53,$53,$53,$53,$53
            .byte $53,$53,$53,$53,$53,$53
LAB6F       .byte $53,$53,$53,$53,$53,$53,$53,$53,$53,$53,$53,$53,$53,$53,$53,$53
            .byte $53,$53,$53,$53,$53,$53,$53,$53,$53,$53,$53,$53,$53,$53,$53,$53
            .byte $53,$53,$53,$53,$53,$53,$53,$53
LAB97       .byte $53,$53,$53,$53,$53,$53,$53,$53,$53,$53,$53,$53,$53,$53,$53,$53
            .byte $53,$53,$53,$53,$53,$53,$53,$53,$53,$53,$53,$53,$53,$53,$53,$53
            .byte $53,$53,$53,$53,$53,$53,$53,$53
LABBF       .byte $53,$33,$33,$33,$33,$33,$33,$33,$33,$33,$33,$33,$33,$33,$33,$33
            .byte $33,$33,$33,$33,$33,$33,$33,$33,$33,$33,$33,$33,$33,$33,$33,$33
            .byte $33,$33,$33,$33,$33,$33,$33,$33,$33,$BF,$BF,$BF,$BF,$BF,$BF,$BF
            .byte $BF,$BF,$BF,$BF,$BF,$BF,$BF,$BF,$BF,$C0,$C1,$C2,$C3,$C4,$C5,$C6
            .byte $C7
LAC00       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
LAC1C       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF
LAD82       .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
LB000       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
LB01D       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00
LB040       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
LB07F       .byte $00
LB080       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
LB0C0       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
LB100       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
LB140       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
LB180       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
LB1C0       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
LB200       .byte $0E,$0F,$12,$0B,$00,$0A,$80,$0C,$0F,$03,$04,$FF,$02,$0F,$0B,$11
            .byte $04,$14,$09,$14,$09,$0F,$0D,$80,$0C,$0F,$03,$04,$FF,$10,$12,$09
            .byte $06,$09,$0D,$00,$0A,$80,$07,$12,$00,$11,$08,$09,$01,$13,$FF,$0C
            .byte $04,$14,$00,$0A,$80,$05,$03,$09,$14,$09,$0F,$0D,$FF
LB23D       .byte $00
LB23E       .byte $01,$02,$03,$05,$09,$0A,$00
LB245       .byte $06,$03,$00
LB248       .byte $2E,$16,$99
LB24B       .byte $0A,$0C,$0F,$09,$09,$0F,$0C,$0A
LB253       .byte $00
LB254       .byte $00
LB255       .byte $00
LB256       .byte $08
LB257       .byte $5F,$6F,$7F,$8F
LB25B       .byte $B2,$B2,$B2,$B2,$00,$00,$00,$08,$00,$00,$0B,$0B,$00,$0B,$0C,$0B
            .byte $0B,$0C,$0F,$09,$00,$00,$00,$08,$00,$00,$09,$0F,$00,$09,$08,$0F
            .byte $09,$08,$07,$09,$00,$00,$00,$08,$00,$00,$02,$0F,$00,$02,$0A,$0F
            .byte $02,$0A,$07,$09,$00,$00,$00,$08,$00,$00,$05,$0B,$00,$05,$03,$0B
            .byte $05,$03,$0D
LB29E       .byte $09,$00,$06,$0E,$0D,$01,$07,$0A,$04,$06
LB2A8       .byte $00,$00,$04,$04,$04,$04,$08,$08,$08,$08,$0C
LB2B3       .byte $0C,$0C,$08,$04,$00
LB2B8       .byte $DC,$DE,$DF,$E0,$E1,$E8,$E9,$EA,$EB,$C2,$B7,$F4,$F9,$CF,$7D,$7E
            .byte $7F,$80,$57,$65,$66,$67,$68,$3F
LB2D0       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$08,$0E,$0E
            .byte $0E,$0E,$0E,$0E,$0E,$0E,$0E,$0E,$0E
LB2E9       .byte $00,$00,$7C,$04,$7C,$64,$7C,$00,$00,$00,$7C,$44,$60,$64,$7C,$00
            .byte $7E,$42,$40,$60,$60,$62,$7E,$00,$04,$04
LB303       .byte $7C,$44,$64,$64,$7C,$00,$00,$00,$7C,$44,$7C,$60,$7C,$00,$7C,$40
            .byte $40,$7C,$60,$60,$7C,$00,$00,$00,$7C,$44,$64,$7C,$04,$7C,$7E,$42
            .byte $40,$66,$62,$62,$7E,$00,$40,$40,$7C,$44,$64,$64,$64,$00,$18,$00
            .byte $10,$10,$18,$18,$18,$00,$10,$10,$18,$18,$18,$18,$18,$00,$00,$00
            .byte $FE,$92,$DA,$DA,$DA,$00,$FF,$89,$89,$C9,$C9,$C9,$C9,$00,$00,$00
            .byte $7C,$44,$64,$64,$64,$00,$7E,$42,$42,$62,$62,$62,$62,$00,$00,$00
            .byte $7C,$44,$64,$64,$7C,$00,$7E,$46,$42,$42,$42,$42,$7E,$00,$00,$00
            .byte $7C,$44,$64,$7C,$40,$40,$00,$00,$3C,$20,$30,$30,$30,$00,$00,$00
            .byte $7C,$40,$7C,$0C,$7C,$00,$10,$10,$3C,$10,$18,$18,$18,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00
LB426       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00
LB78A       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$58,$16,$61,$16,$EE,$08,$DC
LB907       .byte $94,$1A,$38,$0B,$AE,$05,$74,$6A,$2E,$E8,$15
LB912       .byte $D9,$A8,$26,$71,$15,$E0,$0A,$A8,$A0,$55,$B0,$8A,$E6,$55,$5C,$6A
            .byte $31,$4C,$16,$21,$A0,$8E,$D5,$0A,$DA,$64,$0C,$16,$02,$00,$01,$01
            .byte $5C,$90,$0E,$20,$D2,$F9,$F0,$1E,$24,$28,$50,$04,$05,$A1,$D0,$04
            .byte $49,$FF,$25,$A1,$85,$A1,$20,$B4,$F9,$24,$A1,$10,$06,$A9,$40,$85
            .byte $F0,$85,$0F,$20,$07,$0D,$20,$D5,$F9,$0A,$90,$06,$A9,$00,$85,$5C
            .byte $F0,$20,$10,$22,$A2,$0F,$B5,$E0,$29,$E0,$D0,$07,$A9,$FF,$95,$E0
            .byte $20,$08,$12,$CA,$D0,$F0,$E8,$86,$5B,$A9,$01,$8D,$44,$E6,$A5,$5C
            .byte $D0,$02,$E6,$1F,$38,$60,$A5,$50,$29,$03,$D0,$24,$A5,$29,$29,$30
            .byte $F0,$1E,$A6,$E0,$29,$20,$D0,$07,$CA,$10,$0B,$A2,$17,$D0,$07,$E8
            .byte $E0,$18,$90,$02,$A2,$00,$86,$E0,$8E,$20,$02,$86,$0D,$20,$F7,$38
            .byte $18,$60,$A2,$01,$A0,$00,$A5,$A1,$85,$29,$A9,$30,$06,$29,$90,$01
            .byte $8A,$99,$52,$50,$09,$80,$99,$7A,$50,$C8,$E8,$E0,$09,$90,$EB,$60
            .byte $A2,$07,$0C,$A2,$0F,$A9,$01,$85,$29,$BD,$F2,$F9,$20,$50,$0B,$3D
            .byte $02,$FA,$C9,$01,$26,$29,$CA,$90,$F0,$A5,$29,$49,$FF,$85,$29,$60
            .byte $F7,$F7,$FB,$FB,$FD,$FD,$7F,$7F,$FF,$FF,$FF,$FF,$DF,$DF,$FB,$FD
            .byte $08,$01,$08,$01,$08,$01,$08,$01,$00,$00,$00,$00,$08,$01,$04,$20
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
LBB12       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00
LBB1B       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00
LBB30       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
LBD1C       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00
LBF00       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00
LBF09       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00
LBFFF       .byte $00
LC000       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
LC100       .byte $00,$70,$70,$80,$70,$70,$70,$70,$70,$70,$70,$70,$70,$70,$70,$70
LC110       .byte $A0,$A0,$A0,$A0,$10,$10
LC116       .byte $00,$70,$10,$10,$70,$70,$10,$10,$10,$00,$90,$90,$70,$70,$90,$90
            .byte $70,$70,$00,$00,$00,$00
LC12C       .byte $90,$00,$00,$00,$90,$90,$70,$70,$70,$70,$90,$90,$80,$80,$80,$80
            .byte $10,$10,$10,$10,$70,$70,$70,$70,$90,$90,$90,$90,$70,$70,$70,$70
            .byte $90,$90,$90,$90,$70,$90,$70,$90,$90,$90,$70,$70,$90,$90,$70,$70
            .byte $70,$70,$90,$70,$00,$90,$70,$90,$20,$20,$20,$20,$70,$70,$90,$90
            .byte $90,$90,$70,$70,$90,$90,$70,$70,$70,$90,$90,$70,$70,$70,$70,$70
            .byte $90,$70,$00,$00,$50,$50,$50,$50,$50,$50,$50,$50,$50,$50,$50,$50
            .byte $50,$B0,$B0,$B0
LC190       .byte $50,$50,$50,$50,$50,$50,$50,$50,$50,$50,$50,$50,$50,$50,$00,$00
            .byte $70,$70,$90,$90,$90,$90,$90,$90,$70,$70,$70,$70,$90,$90,$70,$70
            .byte $90,$70,$70,$90,$70,$90,$90,$90,$90,$90,$90,$90,$70,$70,$70,$90
            .byte $90,$90,$90,$90,$70,$70,$70,$70,$90,$90,$90,$90,$70,$70,$70,$70
            .byte $90,$70,$90,$70,$90,$90,$70,$70,$90,$90,$70,$70,$70,$70,$70,$70
            .byte $70,$70,$40,$40,$40,$40,$40,$40,$40,$40,$40,$40
LC1EC       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00
LC200       .byte $87,$87,$87,$87,$27,$2E,$2E,$24,$25,$D4,$D6,$26,$34,$68,$69,$36
            .byte $31,$BF,$BF,$33,$30,$30,$30,$30,$31,$BF,$BF,$23,$24,$30,$30,$30
            .byte $30,$30,$75,$27,$20,$BF,$BF,$33,$20,$BF,$BF,$33,$20,$BF,$BF,$23
            .byte $31,$BF,$BF,$23,$30,$30,$30,$30,$20,$BF,$BF,$23,$24,$30,$75,$27
            .byte $C0,$40,$3C,$3F,$3C,$3F,$44,$C4,$C8,$C9,$CA,$CB,$3C,$3F,$3C,$3F
            .byte $00,$67,$66,$00,$3C,$3F,$3C,$3F,$78,$A4,$A7,$A9,$B9,$DC,$DF,$26
            .byte $3C,$3F,$3C,$3F,$3C,$3F,$3C,$3F,$3C,$3F,$3C,$76,$BC,$BE,$BF,$23
            .byte $24,$AC,$AD,$27,$24,$AC,$AD,$27,$34,$68,$69,$36,$00,$00,$00,$00
LC280       .byte $87,$87,$87,$87,$D8,$D2,$D2,$D9,$2E,$D0,$D1,$2E,$62,$38,$3A,$63
            .byte $31,$A0,$A2,$33,$BF,$A0,$A2,$BF,$31,$A0,$A2,$BF,$31,$A0,$A2,$BF
            .byte $BF,$A0,$A2,$33,$BF,$A0,$A2,$33,$BF,$A0,$A2,$33,$BF,$A0,$A2,$BF
            .byte $31,$A0,$A2,$BF,$BF,$A0,$A2,$BF,$BF,$A0,$A2,$BF,$31,$A0,$A2,$33
            .byte $C1,$41,$2F,$3E,$3D,$2F,$45,$C5,$48,$49,$4A,$4B,$3D,$2F,$2F,$3E
            .byte $64,$14,$14,$65,$3D,$00,$00,$3E,$79,$A5,$16,$AA,$BA,$DD,$E0,$2E
            .byte $3D,$6C,$6E,$3E,$3D,$3E,$3D,$3E,$3D,$00,$B6,$B7,$3D,$BD,$BE,$BF
            .byte $B0,$B2,$B3,$B1,$B0,$B4,$B5,$B1,$62,$70,$72,$63,$00,$00,$00,$00
LC300       .byte $87,$87,$87,$87,$DA,$D3,$D3,$DB,$2E,$D0,$D1,$2E,$62,$39,$3B,$63
            .byte $31,$A1,$A3,$33,$BF,$A1,$A3,$BF,$31,$A1,$A3,$BF,$31,$A1,$A3,$BF
            .byte $BF,$A1,$A3,$33,$BF,$A1,$A3,$33,$BF,$A1,$A3,$33,$BF,$A1,$A3,$BF
            .byte $31,$A1,$A3,$BF,$BF,$A1,$A3,$BF,$BF,$A1,$A3,$BF,$31,$A1,$A3,$33
            .byte $C2,$42,$2F,$3F,$3C,$2F,$46,$C6,$3C,$2F,$2F,$3F,$4C,$4D,$4E,$4F
            .byte $65,$14,$14,$64,$3C,$00,$00,$3F,$7A,$A5,$16,$AA,$BA,$DD,$E0,$2E
            .byte $3C,$6D,$6F,$3F,$3C,$3F,$3C,$3F,$3C,$B6,$B7,$BF,$3C,$00,$BD,$BE
            .byte $B0,$B2,$B3,$B1,$B0,$B4,$B5,$B1,$62,$71,$73,$63,$00,$00,$00
LC37F       .byte $00
LC380       .byte $87,$87,$87,$87,$26,$2E,$2E,$25,$24,$D5,$D7,$27,$35,$6A,$6B,$37
            .byte $31,$BF,$BF,$33,$32,$32,$32,$32,$25,$74,$32,$32,$31,$BF,$BF,$22
            .byte $21,$BF,$BF,$33,$32,$32,$32,$26,$21,$BF,$BF,$33,$32,$32,$32,$32
            .byte $31,$BF,$BF,$22,$21,$BF,$BF,$22,$21,$BF,$BF,$22,$25,$74,$32,$26
            .byte $C3,$43,$3D,$3E,$3D,$3E,$47,$C7,$3D,$3E,$3D,$3E,$CC,$CD,$CE,$CF
            .byte $00,$66,$67,$00,$3D,$3E,$3D,$3E,$7B,$A6,$A8,$AB,$BB,$DE,$E1,$27
            .byte $3D,$3E,$3D,$3E,$3D,$3E,$3D,$3E,$B8,$B7,$BF,$22,$3D,$3E,$3D,$77
            .byte $25,$AE,$AF,$26,$25,$AE,$AF,$26,$35,$6A,$6B,$37,$00,$00,$00,$00
            .byte $92,$17,$0F,$15,$1E,$0E,$17,$0C,$0E,$8D,$12,$1C,$19,$18,$1C,$0A
            .byte $15,$9C,$0E,$1B,$1F,$0A,$17,$1D,$C2,$0E,$1C,$1C,$0E,$17,$10,$0E
            .byte $1B,$C2,$0A,$12,$17,$1D,$0E,$17,$0A,$17,$0C,$0E,$8C,$1B,$0E,$54
            .byte $9C,$0E,$17,$1D,$12,$17,$0E,$15,$8B,$0A,$1D,$1D,$15,$0E,$9C,$0E
            .byte $0C,$1E,$1B,$12,$1D,$22,$8C,$18,$42,$42,$0A,$17,$0D,$9B,$18,$0B
            .byte $18,$1D,$8D,$1B,$18,$12,$0D,$8C,$22,$0B,$18,$1B,$10,$8E,$17,$1D
            .byte $1B,$22,$8C,$15,$0A,$1C,$1C,$91,$0E,$12,$10,$11,$1D,$D4,$0E,$12
            .byte $10,$11,$1D,$8D,$1B,$12,$1F,$0E,$8B,$1B,$0A,$12,$17,$8A,$1B,$42
            .byte $0A,$42,$0E,$17,$1D,$9C,$0E,$17,$1C,$18,$1B,$1C,$30,$30,$01,$B0
            .byte $30,$30,$30,$30,$30,$30,$30,$30,$30,$02,$B0,$30,$30,$30,$30,$30
            .byte $30,$30,$30,$30,$03,$97,$18,$1D,$0E,$1C,$AF,$30,$97,$18,$17,$0E
            .byte $8A,$17,$1D,$12,$2E,$10,$1B,$0A,$1F,$8B,$12,$19,$0E,$0D,$0A,$15
            .byte $91,$18,$1F,$0E,$1B,$D4,$11,$0E,$0E,$15,$1C,$9D,$1B,$0A,$0C,$14
            .byte $1C,$9C,$19,$0E,$0C,$1D,$1B,$0A,$15,$9C,$18,$17,$12,$0C,$9E,$15
            .byte $1D,$1B,$0A,$2E,$1C,$18,$17,$12,$0C,$9C,$1E,$0B,$1C,$18,$17,$12
            .byte $0C,$92,$17,$0F,$1B,$0A,$2E,$1B,$0E,$0D,$9B,$0A,$0D,$0A,$1B,$9D
            .byte $1B,$12,$19,$0E,$0D,$0A,$15,$95,$12,$10,$11,$1D,$91,$0E,$0A,$1F
            .byte $22,$8D,$1E,$1D,$22,$9D,$11,$0E,$9D,$11,$12,$1C,$98,$17,$0E,$98
            .byte $0F,$8F,$12,$1B,$1C,$1D,$9D,$18,$9E,$1C,$0E,$9C,$22,$1C,$1D,$0E
            .byte $42,$97,$0E,$1E,$1D,$1B,$18,$17,$12,$0C,$AE,$30,$92,$1C,$9C,$12
            .byte $42,$19,$15,$0E,$9B,$1E,$0B,$0B,$12,$1C,$11,$8C,$18,$42,$42,$18
            .byte $17,$8D,$0E,$1F,$12,$0C,$0E,$92,$17,$C2,$18,$1C,$1D,$9C,$19,$0A
            .byte $0C,$0E,$8C,$1B,$0A,$0F,$1D,$C2,$0A,$12,$17,$1D,$0A,$12,$17,$8A
            .byte $8C,$15,$0E,$0A,$17,$9C,$11,$12,$19,$95,$0A,$1C,$0E,$1B,$30,$1B
            .byte $12,$0F,$15,$0E,$8D,$12,$1C,$1B,$1E,$19,$1D,$18,$1B,$95,$18,$54
            .byte $9E,$1C,$0E,$0D,$99,$1B,$18,$1D,$0E,$0C,$1D,$8A,$1B,$0E,$0A,$1C
            .byte $8F,$1B,$18,$42,$92,$17,$1D,$1B,$1E,$0D,$0E,$1B,$1C,$9C,$15,$18
            .byte $54,$8B,$1E,$1D,$9C,$1E,$1B,$0E,$97,$18,$1D,$8B,$1E,$12,$15,$1D
            .byte $92,$17,$1D,$18,$91,$0E,$0A,$0D,$95,$0A,$1C,$0E,$1B,$1C,$8A,$0C
            .byte $1D,$12,$1F,$12,$1D,$22,$91,$0E,$15,$42,$0E,$1D,$9C,$0E,$15,$0F
            .byte $2E,$19,$18,$54,$0E,$1B,$0E,$0D,$8A,$17,$0D,$8C,$18,$17,$1D,$1B
            .byte $18,$15,$8A,$17,$22,$8F,$18,$1B,$9C,$11,$18,$1B,$1D,$9D,$12,$42
            .byte $0E,$8A,$1B,$0E,$9D,$1E,$1B,$1B,$0E,$1D,$C2,$18,$1E,$17,$1D,$0E
            .byte $0D,$9F,$0E,$1B,$1C,$12,$18,$17,$9E,$17,$12,$1D,$9D,$11,$0A,$1D
            .byte $A2,$18,$1E,$8C,$1E,$1B,$1B,$0E,$17,$1D,$15,$22,$D4,$12,$15,$15
            .byte $99,$1B,$0E,$19,$0A,$1B,$0E,$8B,$18,$0A,$1B,$0D,$CB,$18,$0B,$18
            .byte $2E,$3F,$1B,$0E,$12,$10,$11,$1D,$0E,$1B,$8E,$15,$12,$42,$12,$17
            .byte $0A,$1D,$0E,$8A,$15,$15,$9B,$18,$10,$1E,$0E,$9B,$18,$0B,$18,$1D
            .byte $1C,$99,$0A,$1B,$0A,$0D,$1B,$18,$12,$0D,$C2,$0E,$1D,$0A,$11,$0A
            .byte $54,$14,$91,$0E,$54,$1C,$1D,$1B,$18,$42,$18,$90,$1B,$0A,$0F,$1D
            .byte $10,$18,$15,$0D,$8B,$15,$0A,$0B,$10,$18,$1B,$12,$1E,$1C,$30,$16
            .byte $4F,$9B,$0E,$0D,$30,$3B,$0A,$1B,$0C,$11,$0E,$1D,$1D,$0A,$9B,$0E
            .byte $1D,$1D,$0A,$2E,$0B,$0E,$0A,$1C,$1D,$92,$1D,$1C,$17,$18,$1D,$0A
            .byte $1B,$0D,$0E,$17,$1E,$0F,$0F,$9D,$1B,$0A,$17,$1C,$0F,$0E,$1B,$D4
            .byte $12,$1C,$11,$8D,$0E,$1C,$12,$10,$17,$0E,$0D,$8B,$22,$BD,$1B,$26
            .byte $C6,$0A,$1C,$1D,$0E,$1B,$17,$0A,$14,$8F,$12,$1D,$1D,$0E,$0D,$D4
            .byte $12,$1D,$11,$8C,$0A,$19,$0A,$0B,$15,$0E,$98,$0B,$1C,$0E,$1B,$1F
            .byte $0A,$1D,$12,$18,$17,$8B,$1B,$12,$0D,$10,$0E,$8A,$12,$1B,$15,$18
            .byte $0C,$14,$9B,$0E,$0A,$0C,$1D,$18,$1B,$9B,$0E,$1C,$0E,$0A,$1B,$0C
            .byte $11,$9C,$1D,$18,$1B,$0E,$1C,$9C,$1D,$0A,$1D,$0E,$1B,$18,$18,$42
            .byte $1C,$9B,$0E,$19,$0A,$12,$1B,$1C,$9A,$1E,$0A,$1B,$1D,$0E,$1B,$1C
            .byte $9B,$18,$0B,$18,$2E,$1C,$1D,$18,$1B,$0E,$1C,$9E,$19,$19,$0E,$1B
            .byte $30,$0C,$0A,$1B,$10,$18,$C2,$12,$0D,$30,$0C,$0A,$1B,$10,$18,$9F
            .byte $0E,$11,$12,$0C,$15,$0E,$30,$11,$18,$15,$0D,$9C,$11,$1E,$1D,$1D
            .byte $15,$0E,$30,$0B,$0A,$22,$8E,$17,$10,$12,$17,$0E,$0E,$1B,$12,$17
            .byte $10,$C2,$0A,$12,$17,$1D,$0E,$17,$0A,$17,$0C,$0E,$8C,$1B,$0E,$0A
            .byte $1D,$0E,$0D,$9E,$19,$95,$0A,$1B,$10,$0E,$91,$0E,$0A,$19,$1C,$92
            .byte $1D,$1C,$9C,$0C,$18,$18,$19,$8C,$18,$15,$15,$0E,$0C,$1D,$92,$1D
            .byte $9D,$11,$0E,$17,$92,$17,$1D,$0E,$1B,$17,$0A,$15,$15,$22,$8C,$11
            .byte $0E,$0A,$19,$0E,$1B,$C2,$0A,$12,$17,$15,$22,$9C,$0E,$1B,$1F,$12
            .byte $17,$10,$8D,$1B,$12,$17,$14,$1C,$9D,$1B,$0A,$22,$98,$17,$C8,$1B
            .byte $0C,$11,$0A,$1B,$0D,$C6,$0A,$1B,$1C,$0D,$0E,$17,$BE,$17,$1D,$0E
            .byte $1B,$19,$1B,$12,$1C,$0E,$1C,$C2,$18,$1F,$12,$17,$10,$9C,$42,$0A
            .byte $15,$15,$99,$0A,$0C,$14,$0A,$10,$0E,$1C,$8C,$15,$0A,$42,$19,$95
            .byte $18,$54,$0E,$1B,$8B,$18,$0D,$22,$8E,$0A,$1B,$15,$22,$9D,$22,$19
            .byte $0E,$92,$42,$19,$0E,$0D,$0E,$C2,$18,$1D,$12,$18,$17,$8C,$18,$17
            .byte $0F,$12,$17,$0E,$0D,$8D,$1E,$1B,$12,$17,$10,$8F,$15,$12,$10,$11
            .byte $1D,$C2,$1E,$15,$1D,$12,$19,$15,$0E,$8A,$1B,$42,$1C,$8C,$0A,$1B
            .byte $1B,$22,$98,$1E,$1D,$8E,$0F,$0F,$12,$0C,$12,$0E,$17,$1D,$15,$22
            .byte $8A,$0F,$1D,$0E,$1B,$C3,$1E,$19,$12,$1D,$0E,$1B,$96,$17,$0C,$12
            .byte $0D,$0E,$17,$1D,$9C,$1E,$19,$19,$15,$12,$0E,$0D,$9D,$0E,$0A,$42
            .byte $9D,$11,$0E,$1C,$0E,$8A,$0B,$15,$0E,$8C,$11,$0E,$0C,$14,$1C,$98
            .byte $17,$15,$22,$97,$18,$95,$18,$17,$10,$0E,$1B,$9C,$1D,$0A,$17,$0D
            .byte $0A,$1B,$0D,$9C,$0E,$1B,$1F,$12,$0C,$0E,$C6,$12,$15,$12,$1D,$0A
            .byte $1B,$22,$9F,$0E,$1B,$22,$9B,$0E,$15,$12,$0A,$0B,$15,$0E,$99,$0A
            .byte $1D,$1B,$18,$15,$8A,$1C,$9C,$18,$18,$17,$8D,$0E,$1D,$0E,$0C,$1D
            .byte $0E,$0D,$99,$18,$54,$0E,$1B,$0F,$1E,$15,$9C,$0E,$17,$1C,$18,$1B
            .byte $1C,$8E,$21,$1D,$0E,$1B,$42,$12,$17,$0A,$1D,$18,$1B,$8A,$1B,$0C
            .byte $11,$12,$1F,$0E,$8D,$0A,$1D,$0A,$9C,$18,$42,$0E,$9B,$0E,$0A,$1C
            .byte $18,$17,$9E,$17,$14,$17,$18,$54,$17,$92,$17,$1C,$1D,$12,$15,$1C
            .byte $90,$1B,$0E,$0A,$1D,$8F,$0E,$0A,$1B,$C1,$1E,$42,$0A,$17,$8A,$0D
            .byte $1F,$0E,$1B,$1C,$0A,$1B,$12,$0E,$1C,$90,$1B,$0E,$0E,$17,$A2,$0E
            .byte $15,$15,$18,$54,$8A,$42,$0B,$0E,$1B,$9B,$0E,$0D,$8E,$21,$19,$0E
            .byte $1B,$12,$42,$0E,$17,$1D,$0A,$15,$97,$0E,$54,$8C,$18,$17,$1F,$0E
            .byte $17,$12,$0E,$17,$0C,$0E,$D4,$0A,$1B,$17,$12,$17,$10,$2F,$99,$1B
            .byte $12,$42,$18,$0D,$0E,$95,$18,$17,$10,$D4,$0A,$1C,$C2,$0A,$22,$8C
            .byte $1B,$1E,$1C,$11,$0E,$0D,$8C,$18,$17,$10,$1B,$0A,$1D,$1E,$15,$0A
            .byte $1D,$12,$18,$17,$1C,$25,$8C,$15,$0E,$0A,$1B,$97,$18,$54,$8B,$18
            .byte $17,$1E,$1C,$82,$00,$00,$00,$91,$12,$10,$11,$15,$22,$9C,$18,$19
            .byte $11,$12,$1C,$1D,$12,$0C,$0A,$1D,$0E,$0D,$98,$54,$17,$8A,$17,$98
            .byte $1F,$0E,$1B,$2E,$0D,$1B,$12,$1F,$0E,$17,$8F,$0A,$1C,$1D,$9D,$18
            .byte $18,$15,$0B,$18,$21,$9C,$15,$0A,$1F,$0E,$9B,$0A,$1B,$0E,$C7,$12
            .byte $0C,$11,$18,$15,$1C,$18,$17,$BC,$18,$42,$19,$0A,$17,$22,$9D,$0A
            .byte $17,$14,$C0,$12,$15,$15,$0E,$17,$8A,$1E,$1D,$18,$2E,$0C,$0A,$17
            .byte $17,$18,$17,$8B,$0E,$8F,$0E,$54,$91,$0A,$1F,$0E,$8E,$17,$1D,$0E
            .byte $1B,$0E,$0D,$9E,$17,$12,$1D,$1C,$8E,$1F,$0E,$1B,$8F,$0A,$1B,$9C
            .byte $18,$8A,$54,$0A,$1B,$0D,$0E,$0D,$BF
LCA09       .byte $00
LCA0A       .byte $00
LCA0B       .byte $00,$00,$08,$D0,$FF,$1C,$00,$01,$00,$00,$00,$00,$00,$00,$10,$A0
            .byte $FF,$28,$00,$01,$00,$00,$00,$00,$00,$00,$12,$80,$FF,$09,$24,$02
            .byte $00,$00,$00,$00,$12,$00,$04,$50,$00,$09,$06,$07,$00,$0E,$10,$FF
            .byte $00,$00,$40,$00,$1C,$1D,$1D,$0D,$00,$00,$00,$00,$40,$00,$04,$50
            .byte $00,$20,$20,$07,$00,$08,$05,$00,$04,$00,$14,$60,$FF,$10,$10,$05
            .byte $00,$00,$00,$00,$14,$00,$20,$40,$00,$08,$08,$08,$00,$00,$00,$00
            .byte $20,$00,$2D,$00,$E0,$E0,$00,$01,$00,$00,$00,$00,$00,$00,$0C,$00
            .byte $F1,$10,$00,$01,$00,$00,$00,$00,$00,$00,$28,$00,$E1,$60,$00,$01
            .byte $00,$00,$00,$00,$00,$00,$20,$00,$E8,$60,$00,$01,$00,$00,$00,$00
            .byte $00,$00,$10,$00,$C7,$60,$00,$01,$00,$00,$00,$00,$00,$00,$0C,$00
            .byte $CD,$20,$20,$04,$00,$08,$40,$00,$00,$00,$80,$00,$00,$C0,$00,$01
            .byte $00,$00,$00,$00,$00,$00,$0C,$90,$FF,$0B,$04,$05,$00,$00,$00
LCACA       .byte $00,$00,$00,$08,$C0,$00,$04,$08,$02,$00,$0F,$F0,$FF,$00,$00,$02
            .byte $FA,$FF,$28,$00,$01,$00,$00,$00,$00,$00,$80,$02,$FC,$FF,$40,$00
            .byte $01,$00,$00,$00,$00,$00,$00,$20,$00,$CD,$60,$00,$01,$00,$08,$40
            .byte $00,$00,$00,$10,$E0,$00,$08,$00,$01,$00,$00,$00,$00,$00,$00,$10
            .byte $B0,$FF,$04,$04,$04,$00,$00,$00,$00,$00,$00,$0B,$D0,$FF,$38,$00
            .byte $01,$00,$00,$00,$00,$00,$00,$01,$F5,$00
LCB24       .byte $13
LCB25       .byte $08
LCB26       .byte $0A,$00,$00,$00,$00,$08,$00,$04,$03,$07,$03,$04,$04,$00,$0F,$D0
            .byte $FF,$04,$00,$06,$20,$00,$07,$04,$04,$00,$0E,$00,$00,$06,$00,$30
            .byte $00,$E0,$E0,$00,$01,$00,$00,$00,$00,$00,$00,$34,$00,$D7,$0F,$00
            .byte $01,$00,$00,$00,$00,$00,$00,$20,$00,$E8,$7D,$00,$01,$00,$00,$00
            .byte $00,$00,$00,$10,$00,$C7,$7D,$00,$01,$00,$00,$00,$00,$00,$00,$0C
            .byte $00,$CD,$7D,$00,$01,$00,$08,$40,$00
LCB7F       .byte $00,$05,$05,$05,$0B,$07,$08,$06,$00,$09,$00,$07,$07,$06,$08,$04
            .byte $0A,$02,$05,$05,$08,$0A,$00,$09,$03,$02,$0B,$09,$00,$07,$06,$08
LCB9F       .byte $10,$20,$40,$10,$80,$80,$20,$10,$40,$10,$10,$40
LCBAB       .byte $55,$55,$55,$00,$00,$00,$55,$55,$55,$33,$80,$00
LCBB7       .byte $F8,$F8,$F8,$56,$CF,$FA,$CD,$CD,$CD,$FC,$F0,$F9
LCBC3       .byte $10,$10,$10,$80,$FF,$08,$80,$80,$80,$10,$30,$02
LCBCF       .byte $30,$58,$79,$A4,$C3,$DB,$03,$22,$3F,$5A,$8A,$A6,$C7,$E0,$FE,$23
            .byte $45,$74,$9A,$B5,$DE,$09,$30,$54
LCBE7       .byte $00,$00,$00,$00,$00,$00,$01,$01,$01,$01,$01,$01,$01,$01,$01,$02
            .byte $02,$02,$02,$02,$02,$03,$03,$03,$6B,$00,$6C,$00,$6D,$00,$6E,$00
            .byte $64,$1B,$19,$19,$4F,$32,$32,$32,$0A,$50,$00,$37,$FE,$2A,$51,$33
            .byte $52,$53,$61,$54,$55,$0A,$56,$3D,$57,$58,$FE,$4F,$59,$5A,$5B,$FF
            .byte $63,$00,$64,$00,$65,$00,$20,$20,$89,$55,$1E,$19,$19,$1F,$23,$32
            .byte $34,$35,$01,$0A,$FE,$36,$37,$38,$39,$3A,$3B,$2E,$3C,$3D,$3E,$3F
            .byte $FF,$73,$00,$72,$00,$20,$20,$20,$20,$7A,$3D,$1A,$19,$19,$1F,$32
            .byte $32,$8A,$74,$75,$76,$2E,$3E,$8B,$8C,$8D,$2C,$35,$FE,$8E,$8C,$8F
            .byte $33,$43,$2E,$90,$35,$FE,$91,$33,$92,$DC,$93,$FF,$5B,$00,$5C,$00
            .byte $5D,$00,$20,$20,$9C,$4E,$1A,$31,$19,$1F,$32,$32,$26,$28,$02,$0A
            .byte $FE,$2B,$2C,$29,$2D,$2E,$2F,$29,$1A,$30,$FF,$5B,$00,$5C,$00,$6F
            .byte $00,$20,$20,$A3,$53,$25,$31,$19,$1F,$32,$32,$94,$5C,$2C,$29,$1A
            .byte $02,$0A,$FF,$79,$00,$5A,$00,$20,$20,$20,$20,$78,$2F,$1E,$31,$19
            .byte $1F,$32,$32,$2A,$0A,$33,$43,$95,$56,$96,$97,$FE,$3D,$98,$33,$5B
            .byte $99,$29,$4E,$FE,$4C,$74,$9A,$53,$9B,$9C,$FF,$7A,$00,$7F,$00,$20
            .byte $20,$20,$20,$6B,$17,$1A,$19,$19,$1F,$32,$32,$36,$37,$56,$9D,$9E
            .byte $9F,$FE,$A0,$33,$5B,$99,$29,$A1,$A2,$FF,$81,$00,$80,$00,$20,$20
            .byte $20,$20,$6B,$1F,$1D,$19,$19,$1F,$32,$32,$A3,$A4,$03,$0A,$FE,$8C
            .byte $1D,$A5,$A6,$99,$9E,$3B,$FF,$5B,$00,$7B,$00,$5A,$00,$20,$20,$8D
            .byte $39,$1E,$31,$19,$1F,$32,$32,$48,$04,$0A,$FE,$A7,$2E,$11,$04,$A8
            .byte $A9,$FF,$7A,$00,$7B,$00,$5D,$00,$20,$20,$84,$2A,$1A,$31,$4F,$1F
            .byte $23,$32,$3F,$04,$0A,$FE,$77,$78,$AA,$AB,$2E,$AC,$AD,$81,$2E,$29
            .byte $3F,$AE,$FE,$66,$3B,$4C,$AF,$29,$B0,$B1,$59,$B2,$78,$3D,$B3,$2C
            .byte $B4,$FF,$7A,$00,$8B,$00,$8A,$00,$20,$20,$94,$33,$1A,$31,$19,$1F
            .byte $32,$32,$E9,$04,$0B,$FE,$BA,$5C,$61,$AC,$8E,$E4,$E8,$FF,$73,$00
            .byte $58,$00,$59,$00,$20,$20,$9D,$4A,$1B,$31,$19,$1F,$32,$32,$A3,$05
            .byte $0B,$FE,$B5,$2E,$AC,$AD,$34,$A9,$B6,$B7,$FE,$B8,$B9,$B2,$FF,$66
            .byte $00,$7C,$00,$7D,$00,$7E,$00,$B0,$3E,$1B,$31,$19,$1F,$32,$32,$BA
            .byte $05,$0B,$FE,$B2,$78,$29,$3F,$FF,$8E,$00,$8F,$00,$90,$00,$91,$00
            .byte $AC,$5D,$1B,$31,$19,$1F,$32,$32,$3D,$E2,$E3,$37,$FE,$B5,$2E,$54
            .byte $29,$64,$99,$8E,$E4,$FF,$66,$00,$67,$68,$69,$00,$6A,$00,$C1,$79
            .byte $1B,$31,$40,$1F,$22,$32,$42,$08,$06,$0B,$FE,$43,$2E,$44,$45,$2C
            .byte $29,$3F,$46,$47,$FE,$3D,$48,$49,$4A,$37,$FF,$85,$00,$86,$00,$20
            .byte $20,$20,$20,$78,$1D,$1A,$31,$4F,$1F,$23,$32,$E3,$06,$0B,$FE,$B7
            .byte $E1,$4C,$74,$29,$EB,$EC,$FE,$B4,$59,$DF,$BD,$EA,$FF,$6B,$00,$87
            .byte $00,$20,$20,$20,$20,$6D,$3B,$1E,$31,$4F,$1F,$22,$32,$48,$06,$0B
            .byte $FE,$4F,$59,$4C,$4D,$29,$5A,$FE,$B4,$59,$5B,$99,$3D,$9E,$ED,$A2
            .byte $FE,$DB,$F0,$77,$78,$E5,$EF,$99,$29,$EE,$5C,$FF,$5E,$00,$5F,$62
            .byte $60,$00,$61,$00,$C1,$66,$1B,$31,$41,$21,$24,$32,$27,$28,$07,$0B
            .byte $FE,$41,$33,$4C,$4D,$29,$4E,$FE,$2B,$2C,$29,$2D,$38,$BB,$78,$29
            .byte $BC,$FF,$8C,$00,$8D,$00,$69,$00,$6A,$00,$BB,$8C,$1B,$31,$41,$1F
            .byte $24,$32,$2A,$5C,$33,$29,$2B,$95,$43,$74,$29,$BC,$FF,$92,$00,$93
            .byte $00,$94,$00,$95,$00,$C1,$E3,$1B,$31,$4F,$1F,$32,$32,$BD,$27,$28
            .byte $07,$0B,$FE,$B7,$3D,$F1,$F2,$F7,$F6,$F3,$BB,$FE,$B4,$59,$29,$39
            .byte $C3,$07,$F4,$F5,$4C,$FF,$70,$71,$20,$20,$20,$20,$20,$20,$64,$1C
            .byte $1A,$31,$4F,$1F,$24,$23,$3D,$BD,$BE,$1A,$5D,$33,$77,$4D,$2A,$0B
            .byte $FE,$91,$61,$BF,$29,$3F,$53
LCECE       .byte $65,$47,$C0,$C1,$C0,$C2,$74,$C3,$C4,$FF,$88,$00,$89,$00,$20,$20
            .byte $20,$20,$6E,$22,$1A,$31,$4F,$1F,$24,$32,$A3,$A4,$1A,$08,$0B,$FE
LCEEE       .byte $77,$78,$E5,$E6,$1A,$5D,$FE,$2A,$0B,$33,$BD,$E7,$49,$33,$4B,$BE
            .byte $FF,$74,$00,$75,$76,$77,$00,$78,$00,$A2,$4F,$1D,$31,$C5,$1F,$24
            .byte $32,$2A,$0B,$DA,$73,$46,$C6,$C7,$FE,$56,$C8,$CA,$C9,$91,$CB,$CC
            .byte $CD,$38,$CE,$CF,$FF,$82,$00,$83,$00,$84,$00,$70,$71,$BB,$A2,$1A
            .byte $D8,$4F,$23,$24,$22,$D4,$09,$0C,$FE,$77,$78,$3D,$D5,$A4,$2C,$12
            .byte $FE,$5B,$99,$3D,$08,$0B,$1A,$5D,$56,$D6,$FE,$D7,$29,$00,$37,$DB
            .byte $4B,$54,$3D,$D8,$12,$56,$D9,$FF
LCF56       .byte $20,$34,$48,$5C,$70,$84,$18,$2C,$40,$54
LCF60       .byte $68,$7C,$90,$24,$38,$4C,$60,$80
LCF68       .byte $51
LCF69       .byte $51
LCF6A       .byte $51
LCF6B       .byte $51,$51,$52,$52,$52,$52
LCF71       .byte $52,$52,$52,$53,$53,$53,$53,$53,$40
LCF7A       .byte $68
LCF7B       .byte $90
LCF7C       .byte $B8,$E0,$08,$30,$58,$80,$A8,$D0,$F8,$20,$48,$70,$98,$C0
LCF8A       .byte $67
LCF8B       .byte $64
LCF8C       .byte $65
LCF8D       .byte $66,$67,$64,$65
LCF91       .byte $40
LCF92       .byte $80
LCF93       .byte $01
LCF94       .byte $02
LCF95       .byte $04
LCF96       .byte $08
LCF97       .byte $10,$20,$40,$80,$01,$02
LCF9D       .byte $00,$40,$80,$C0,$00,$40,$80,$C0
LCFA5       .byte $54,$54,$54,$54,$55,$55,$55,$55
LCFAD       .byte $FF,$01,$00,$00,$FF,$01,$FE,$FF,$FE,$02,$01,$02
LCFB9       .byte $FE,$FE,$FF,$01,$02,$02,$FF
LCFC0       .byte $00,$01,$FF,$00,$01
LCFC5       .byte $FF,$FC,$FB,$F0,$FB,$FC,$FF,$F1,$F3,$FE,$F6,$F0,$F6,$FE,$F3,$F1
            .byte $F7,$FA,$F2,$F0
LCFD9       .byte $F2,$FA,$F7
LCFDC       .byte $F1
LCFDD       .byte $05,$07,$08,$02,$05,$07,$08,$02
LCFE5       .byte $15,$17,$18,$12
LCFE9       .byte $01,$02,$03,$04,$05,$06,$07,$08,$09,$0A,$0B,$0C,$0D,$0E,$0F
LCFF8       .byte $0E
LCFF9       .byte $01,$02,$03,$04,$05,$06
LCFFF       .byte $05
            ldx #$33
LD002       lda PUPBT1,X
            sta LB100,X
            dex
            bpl LD002
            ldx #$74
            ldy #$D1
            jsr LE018
            ldx #$00
            ldy #$B1
            stx CHKSUM
            sty BUFRLO
            lda #$80
            sta LD02A+2
LD01F       ldx #$00
LD021       lda LD02A+2
            cmp BUFRLO
LD026       bcs LD035
            lda #$30
LD02A       sta L8000,X
            inx
            bne LD02A
LD030       inc LD02A+2
            bne LD021
LD035       ldy #$00
            lda (CHKSUM),Y
            cmp #$FF
            beq LD074
            sta BFENLO
            iny
            lda (CHKSUM),Y
            sta BUFRHI
            iny
            bne LD04B
LD047       jsr LD05B
            iny
LD04B       lda (CHKSUM),Y
            bpl LD047
            tya
            clc
            adc CHKSUM
            sta CHKSUM
            bcc LD035
            inc BUFRLO
            bne LD021
LD05B       jsr LD068
            cmp #$BA
            bcc LD074
            cmp #$DA
            bcs LD074
            adc #$A0
LD068       sta (BUFRHI,X)
            ora #$80
            inc BFENLO
            sta (BUFRHI,X)
            dec BFENLO
            inc BUFRHI
LD074       rts
LD075       lda #$4F
            .byte $0C
            lda #$3C
            sta LD088+1
            ldx #$00
LD07F       lda LD08B,X
            sta L4C00,X
            inx
            bne LD07F
LD088       jmp L4C00
LD08B       inc COLPF3
            jmp LEA7E
            ldx #$80
LD093       lda L0000,X
            sta LBF00,X
            lda #$00
            sta L0000,X
            inx
            bne LD093
            stx TMPLBT
            stx DCOMND
            lda #$36
            sta NGFLAG
LD0A9       bit TRIG1
            bpl LD0A9
            stx LD030
            stx GRAFM
            stx LD416+2
            stx COLBK
            inx
            stx COLPF3
            lda #$81
            sta LDC0D
            lda LDC0D
            rts
            jsr L4C06
            ldx LCA09
            beq LD0F9
            jsr L4CD2
            lda #$00
            jsr JKEYON
            jmp L4C6F
            jsr L4C06
            ldx LCA09
            beq LD0F9
            ldy #$53
            jsr L4CD2
            ldx #$38
            ldy #$03
            stx L00C1
            sty L00C2
            ldx #$4D
            ldy #$03
            lda #$C1
            jsr LFFD8
            .byte $80
LD0F9       sec
            sei
            ldx #$80
LD0FD       lda LBF00,X
            sta L0000,X
            inx
            bne LD0FD
            lda #$7F
            sta LDC0D
            jsr L1ECB
            ldy #$00
            lda TEMP1+1
            beq LD11C
            bpl LD122
            lda #$48
            ldx #$40
            bne LD126
LD11C       lda #$A1
            ldx #$3F
            bne LD126
LD122       lda #$46
            ldx #$3E
LD126       sta L4CB7
            stx LDD06
            sty LDD06+1
            lda #$11
            ldx #$80
LD133       cpx TRIG2
            bne LD133
LD138       cpx TRIG2
            bne LD13D
LD13D       ldy #$09
LD13F       dey
            bne LD13F
            lsr SAVADR
            inx
            bne LD138
            ldy #$07
LD149       dey
            bne LD149
            sta LDD0E+1
            lda #$01
            sta COLBK
            sta COLPF3
            lda LDC0D
            dec NGFLAG
            rts
            sty L4CE8
            lda #$01
            tay
            jsr LFFBA
            lda #$05
            ldx #$E4
            ldy #$4C
            jmp LFFBD
            rti
            bmi LD1AC
LD172       bvc LD1B3
            .byte $34
            lda (L00FF),Y
            .byte $82
            asl L3E40+1
            bvc LD1C8+1
            pha
            .byte $47,$FF,$82,$2B,$3A
            bmi LD194
            ora APPMHI,X
            asl L1DFF
            bmi LD1BC
            clc
            .byte $0F
            bmi LD1C0
            .byte $4B,$FF
            clc
            .byte $0B
LD194       clc
            rol L1B3D+2
            asl LFD12
            bpl LD1AE
            ora L1B0D+1,X
            .byte $1C,$EB,$03,$17,$FF,$82,$53
            eor L0E11
            bmi LD1AC
LD1AC       .byte $00,$FF
LD1AE       ora (STATUS,X)
            asl ICCOMT,X
            .byte $0F
LD1B3       ora ABUFPT+2,X
            asl L17FE+1
            .byte $0C
            asl L3D2F+1
LD1BC       asl L121F
            .byte $7F
LD1C0       sbc L0C03,Y
            clc
            .byte $17,$1C,$12,$1C
LD1C8       ora L1CFB,X
            .byte $82,$7B,$3A,$17,$E2,$13
            asl
            .byte $17,$FF
            bmi LD1F3
            .byte $1B
            asl
            .byte $17,$1C,$42,$12,$FF
            ora LAD82,X
            eor L1918
            bmi LD201
            sbc DOSINI,X
            clc
            .byte $1B
            asl L0498
            ora IOCB6+ICAX5,X
            ora L0AFE+1
            .byte $22
LD1F3       .byte $2F
            sty POKMSK
            ora L0E1B,Y
            .byte $FF,$1C
            asl L1D17
            .byte $1C
LD1FF       sty ICAX1Z
LD201       .byte $12,$FF
            ora L301C,X
            .byte $54
            asl
            .byte $22
            bmi LD227+1
            .byte $BF
            clc
            sbc (RAMLO+1,X)
            .byte $3B
            asl L0A1B+2
            bmi LD24F+2
            .byte $FF
            asl L121D
            bmi LD24A+1
            .byte $1C,$22,$1C,$FB
            ora L420D+1,X
            sty LMARGN
            .byte $5F,$03
            asl
LD227       bmi LD227+1
            bmi LD23A+2
            asl L4214+1
            asl L271D
            sbc L5430,X
            ora (RTCLOK),Y
            .byte $0C
            ora (L00F9),Y
            .byte $04
LD23A       asl L17FE+1
            bmi LD26D+2
            ora L0A15,Y
            .byte $0C
            asl L0DFF
            sty KEYDEF+1
            clc
            .byte $17
LD24A       ora ICCOMZ,X
LD24C       bmi LD24C
            .byte $0C
LD24F       asl L1D19+2
            asl
            .byte $12,$17,$9E,$03,$FF
            ora L270A,X
            bmi LD274
            asl
            .byte $42
            asl LEB3F
            .byte $03
            lda (RAMLO,X)
            bmi LD27E+1
            .byte $54,$17
            stx ICAX1Z
            .byte $FF,$1B
LD26D       asl L1818+1
            .byte $1B
            ora L0D0D+1,X
LD274       .byte $BF
            bmi LD1FF
            .byte $03
            asl L121A+1
            .byte $17
            bpl LD2AE
LD27E       adc IOCB1+ICPTH,X
            asl L0C17,X
            ora (DOSVEC),Y
            nop
            ora APPMHI+1
            .byte $FF,$12
            asl L0D14+1
            stx LMARGN
            clc
            .byte $1F,$DF
            asl L911B
            .byte $03,$1B
            clc
            .byte $0B
            clc
            ora L2DDE,X
            .byte $1C
            ora RAMLO,X
            ora L181A+1,X
            ora L00D8,X
            .byte $03,$DF,$12
            ora L0526,X
            ora (DOSVEC),Y
LD2AE       ora ABUFPT+1,X
LD2B0       stx L00FF
            .byte $7A
            ora DSKFMS,X
            .byte $0C
            asl
            ora L1812,X
            .byte $74
            ror L0A03,X
            .byte $17
            ora L0528+1
            bmi LD2B0+1
            ora #$18
            .byte $03,$BF
            clc
            .byte $17,$03,$1B,$87,$0B
            eor #$0A
            .byte $1B,$FF
            asl
            ora L181A+1
            .byte $12
            ora L302E+2
            inc L0E4B,X
            ora L211E
            dey
            rol
            and CASINI+1,X
            .byte $CF
            bmi LD2F1
            rol
            .byte $03,$EB,$04,$1C
            rol STATUS
            rol L0AEE,X
LD2F1       .byte $0C
            ora (DSKFMS),Y
            .byte $03
            ora (RTCLOK),Y
            ora IOCB6+ICBAL,Y
            .byte $FF,$1B,$1B,$12
            asl L301C
            asl
            dey
            .byte $BF,$52
            lda (RAMLO),Y
            .byte $17
            clc
            .byte $1B,$42
            asl
            ora L00FF,X
            bmi LD319+1
            .byte $0C
            ora L1F12,X
            .byte $12
            ora LE73D,X
            .byte $04
LD319       lda #$03
            ora L0A11,X
            ora L0666,X
            dey
            .byte $B7,$7A,$5F
            ora ABUFPT
            bmi LD338+2
            .byte $03,$1F,$12,$1C,$BF
            asl PRNBUF+21,X
            .byte $1B
            asl
            .byte $17
            bpl LD344
            rol L00FF
LD338       eor L1210+1
            .byte $1C
            bmi LD34B
            asl
            ora L0AFE+1,X
            bmi LD356
LD344       .byte $1C
            txa
            ora ICHIDZ
LD348       bmi LD348+1
            rti
LD34B       .byte $1B
            asl
            .byte $0F
            ora L180E+2,X
            ora L00FF,X
            ora L4530
LD356       ora L260C+1,X
            bmi LD35C
            .byte $FF
LD35C       ora #$08
            ora STATUS
            rol SDLSTL
            .byte $00
            inc NGFLAG,X
            .byte $00
            txa
            rol
            sta L1002+1
            clc
            eor (RAMLO),Y
            inc L0A0A+1,X
            ora L151D,X
            asl L5530
            ora L00FF
            .byte $1C
            bmi LD398+2
            clc
            bmi LD3B0
            .byte $1B
            asl L12FE+1
            .byte $17,$0F
            clc
            .byte $1B,$0C
            asl LB78A
            .byte $52
            sbc L3002+1,Y
            asl
            .byte $5C,$03
            clc
            .byte $1B
            ora L30FF,X
LD398       ora L4212,X
            asl L3026
            eor L117F
            .byte $03
            ora (APPMHI),Y
            ora CRITIC,X
            asl L301C+1
            .byte $FF
            bmi LD3BD
            asl
            .byte $1C
            txa
            .byte $7A
LD3B0       .byte $42
            asl L1BFE+1
            bpl LD3C3+1
            ora L5430
LD3B9       .byte $12
            ora L11FE,X
LD3BD       bmi LD3D1+2
            .byte $17
            clc
            .byte $54,$17
LD3C3       and LFB06
            ora DOSVEC,X
            .byte $22
            clc
            asl IOCB1+ICAX3,X
            asl
            ora L8AF9,X
LD3D1       ldx L153A
            ora L00C2,X
            ora L00A2
            .byte $03
            ora L30FF,Y
            .byte $1C,$0C
            clc
            .byte $1B
            asl L8C2E+1
            sta L172A,X
            .byte $04
            dec L5403,X
            clc
            .byte $1B
            rts
            .byte $03
            ora L0EFE+1
            .byte $0F
            asl L0C17
            asl L261C
            .byte $EB
            sty L1251+1
            .byte $27
LD3FD       .byte $03
            bmi LD3B9
            .byte $04
            asl L7F17
            ldx CASINI+1
LD406       .byte $22
            bmi LD424+1
            asl L1918+1,X
            ora L00FF,X
            .byte $22
            bmi LD440+1
            asl
LD412       .byte $17
LD413       ora L1930
LD416       inc L5418,X
LD419       asl L1C1B
            sty LA57A
            .byte $03
LD420       .byte $FB,$1B
            bmi LD45E+2
LD424       asl RAMLO
            eor CASINI+1,X
            .byte $42
            clc
            .byte $BF
            ora IOCB6+ICBLL,X
            asl L421A+1
            .byte $12,$17
            asl
            .byte $FF
            ora ICPTLZ,X
            stx L4904
            ora ABUFPT+2,X
            bpl LD3FD
            bmi LD420
LD440       ora RTCLOK+1
            clc
            .byte $22,$1C
            ora LFA12,X
            .byte $0C,$14
            bmi LD45E
            .byte $17
            rol
            .byte $03
            ora L043C+1,Y
            .byte $FB,$02
            stx L4D2B
            .byte $54
            asl L0D02+1,X
            .byte $12,$FF,$1C
LD45E       ora L0E1B,X
            .byte $1C,$1C
            bmi LD470
            .byte $FB
            asl L0C0A
            clc
            .byte $17,$8F,$03
            ora (DOSVEC),Y
            .byte $EF
LD470       .byte $1F
            asl LF130
            .byte $03
            asl L8E17
            .byte $52,$7F
            ror RAMLO
            bmi LD499
            clc
            .byte $0B
            clc
            ora L7930,X
            .byte $73,$03
            asl L0F15
            .byte $27,$82,$03
            ora (CASINI+1),Y
            asl
            .byte $FF,$17
            bmi LD4C3
            asl L1017+2,X
            .byte $1B
            asl
            .byte $FF
LD499       ora L0D0D+1
            stx L3A7B
            ora LF30D
            .byte $12
            ora L1812,X
            txa
            .byte $03,$9F,$03,$0F
            clc
            sbc DSKUTL+1
            .byte $42
            asl
            .byte $F2,$04
            cli
            .byte $03,$0B
            php
            .byte $03
            bmi LD537+1
            .byte $22,$03
            bcc LD4E7
            .byte $1B
            asl L180B+1
            .byte $1F,$FF
LD4C3       asl L0E1B
            ora L3026
            jmp LFE12
            .byte $42,$12
            ora DOSVEC,X
            .byte $1B
            bmi LD515
            txa
            .byte $03,$FB
            asl
            bpl LD4E7
            .byte $1C
            bmi LD513
            .byte $03
            asl LFF90
            .byte $52,$0C
            asl
            ora L0B09+1,Y
            .byte $12
LD4E7       ora L00FD,X
            .byte $12
            ora L2621+1,X
            bmi LD53C
            .byte $89,$04
            ora (L00FF),Y
            asl L4214+1
            asl L301C+1
            asl
            ora L00BF,X
            .byte $1C
            eor L1E01+2,Y
LD500       .byte $1C
            asl L901A+2
            .byte $7A,$F3,$1C
            ora (RTCLOK),Y
            ora L0502,Y
            adc L1C03+2
            bmi LD500
            .byte $42
            asl
LD513       .byte $22
            pha
LD515       .byte $03
            bmi LD530
            .byte $0B
            ora L0AFB,X
            .byte $12,$17
            asl L4A0D
            .byte $03,$22,$92,$DF
            asl
            eor #$31
            ora APPMHI+1
            .byte $12,$1B
            asl L7F30
            .byte $13
LD530       .byte $04
            ora DOSVEC,X
            .byte $22,$92
            rol
            .byte $1C
LD537       ora L18A4,X
LD53A       sty CASINI+1
LD53C       bmi LD5A9
            .byte $03,$1A,$03
            ora (L009F),Y
            asl L00B3
            .byte $04
            sta L121C
            .byte $03
            cmp #$04
            .byte $13,$03,$92,$52,$2B,$03
            asl L17FE+1
            asl L101A+1
            .byte $22
            bmi LD589+2
            .byte $0C,$FB
            ora DSKFMS,X
            asl
            .byte $14
            bmi LD598
            .byte $03
            bmi LD596
            sbc L1B19,X
            clc
            ora L0C0E,X
            .byte $2F
            asl DSKFMS
            sbc L920D+2,X
            .byte $7A
            asl
            .byte $0C,$0C,$47,$03,$12,$D7,$17
            bpl LD5A4+1
            .byte $04
            bmi LD53A+1
            ora ICAX4Z
            .byte $1C
LD584       bmi LD584+1
            .byte $0C
            clc
            .byte $42
LD589       ora L1D1E,Y
            asl LDF1B
            bmi LD59B
            lsr L9403
            rol
            .byte $0B
LD596       clc
            .byte $42
LD598       sbc DOSVEC+1,X
            asl
LD59B       .byte $1B
            ora IOCB2+ICAX4
            .byte $0B,$B2,$03
            asl
            .byte $EF
LD5A4       bmi LD5BE+1
            clc
            .byte $1B,$03
LD5A9       .byte $0F
            asl L3014+1,X
            .byte $F7
            bmi LD5CB
            asl
            ora IOCB7+ICBAH
            .byte $12,$0C
            sty L00BF,X
            .byte $52
            cmp (RAMLO,X)
            ora (DSKFMS),Y
            .byte $1C
LD5BE       ora L9426,X
            .byte $FF,$7A,$0C
            clc
            .byte $17,$1C
            clc
            ora APPMHI,X
            .byte $FF
LD5CB       rol STATUS
            .byte $3A
            bmi LD5EC
            .byte $42
            asl
            ora L00FE,X
            ora ICAX5Z,X
            .byte $1C,$0C
            asl
            ora APPMHI,X
            .byte $44,$04,$9F,$17
            sty L9703
            .byte $04
            stx CASINI+1,Y
            .byte $3C
            ora APPMHI,X
            .byte $9F
            asl
            adc (CASINI+1),Y
LD5EC       .byte $8B,$04,$0F,$1B
            asl L1012
            .byte $9F
            ora (L0092),Y
            .byte $04,$E2,$04,$1B
            clc
            .byte $0B
            clc
            ora L1CBF,X
            ora CASINI+1
            stx ICAX1Z,Y
            .byte $0B
            asl L420A
            .byte $6B,$E2,$03
            clc
            .byte $42
            asl CASINI+1,X
            asl L07C6
            bmi LD61D+1
            .byte $BF,$1C
            pla
            .byte $03
            clc
            .byte $12
            ora L261C
LD61D       stx L00BF,Y
            .byte $53
            php
            .byte $04
            ora (APPMHI),Y
            ora CRITIC,X
            asl LFF1D
            bmi LD63D
LD62B       .byte $1C
            bmi LD65D+1
            .byte $0F,$12
            ora L1DBF,X
            .byte $5C,$03,$54,$12
            ora L3011,X
            ora L54FE,X
LD63D       .byte $12,$17
            stx KEYDEF+1,Y
            .byte $54
            ora (DELTAC+1),Y
            .byte $03
            sbc L0D30,X
            asl L140B+1
            bmi LD62B
            .byte $04
            asl
            .byte $FF,$1F
            asl
            .byte $12
            ora DOSVEC,X
            .byte $0B
            ora APPMHI,X
            .byte $BF,$27
            lda L3002+1,Y
LD65D       bmi LD6B2+1
            asl L1514+1
            .byte $FF
            stx L00AA,Y
            eor L3017+1
            ora L1E0A,Y
            .byte $FF,$1C
            asl L302E+1
            ora L0E1B,Y
            .byte $1C,$FF,$1C
            bmi LD692+1
            asl L2E16+1,X
            .byte $1C
            ora L18FF,X
            ora L9826,Y
            .byte $02
LD683       ora L1C0C+2
            inc L1B1D,X
            clc
            .byte $22,$12,$17
            bpl LD6D9+1
            .byte $04,$BF,$42
LD692       ldx DOSVEC
            bmi LD6C5+1
            ora DOSVEC,X
            .byte $1C
            asl L1BFE
            .byte $1C
            tya
            .byte $2B,$3A
            ora BUFADR,X
            eor #$05
            .byte $27
            ora (RAMLO,X)
            .byte $44
            php
            clc
            cpx #$03
            .byte $27,$04,$1C
            ora (RTCLOK),Y
            .byte $FB
LD6B2       ora L271B+1,Y
            tya
            .byte $52,$D4
            asl STATUS
            .byte $42,$EB
            clc
            asl L6017,X
            .byte $04
            bmi LD683
            .byte $03
            asl
LD6C5       bmi LD6C5+1
            ora L1B1D+1,X
            .byte $1B
            asl L261C+1
            bmi LD74A+1
            .byte $3C,$03,$1C
            asl L7A98
            bvs LD6DB
            asl
LD6D9       bmi LD6D9+1
LD6DB       .byte $1C,$12
            ora L300E
            asl L0E15
            sbc (ABUFPT+3),Y
            asl
            ora LBB12,X
            .byte $03
            asl LB907
            .byte $04
            rol L00FF
            txs
            .byte $02
            clc
            .byte $1B
            bmi LD6FF+2
            .byte $22
LD6F7       bmi LD6F7+1
            ora L0A1B,X
            .byte $17,$1C,$0F
LD6FF       asl LBB1B
            .byte $1B,$6B,$04,$0C
            clc
            .byte $17
            rts
            .byte $03
            ora STATUS,X
            .byte $DF
            ora L6018,X
            ora ICPTLZ
            txs
            rol
            .byte $12,$17,$F7,$0C
            ora ABUFPT+2,X
            ora L064D+2
            clc
            .byte $1C
            asl L953B
            .byte $04,$34,$03,$1B
            asl
            bpl LD736
            .byte $03
            bmi LD737
            .byte $FF
            asl L0A0A+2
            .byte $42
            asl L529A
            asl
            .byte $FF
LD736       .byte $1B
LD737       asl L152E+2
            clc
            .byte $54
            bmi LD757
            .byte $F7
            clc
            .byte $54
            asl L6F1B
            .byte $04
            asl
            .byte $17
            ora L30F1
LD74A       ora (DOSVEC),Y
            .byte $1F
            rol L7C03,X
            .byte $03
            inc CASINI+1
            txs
            .byte $CF,$7B,$4B
LD757       rol TRAMSZ,X
            dec RAMLO,X
            bmi LD778
            asl L9F19
            .byte $1B,$5C,$03,$44
            asl DSKFMS
            .byte $17
            rol L0C1C
LD76A       .byte $FF,$1B
            asl L170E
            txs
            ldy L1B3D+2
            sbc L4217+1,X
            bmi LD791
LD778       asl
            asl IOCB5+ICBLH,X
            .byte $42,$BF
            clc
            .byte $47,$03
            clc
            .byte $17
            ora ICCOMZ,X
            .byte $2F,$9C,$FF
            rol
            ora (ICCOMZ),Y
            ora L1B0D+1,Y
            rol LFA0A
LD791       .byte $0C
            ora L1F12,X
            asl L0520+1
            bmi LD76A
            .byte $03
            cmp L1C52+2,X
            lda L1803+1,Y
            .byte $1B
            ora L0493,X
            .byte $9C,$FF,$52,$1B
            asl L220C
            .byte $0C
            ora APPMHI,X
            inc L1B2F+1,X
            asl
            ora L260C+2,X
            .byte $9C,$03,$07
            inc ICCOMZ
            .byte $42,$0B,$34,$03
            ora (CASINI+1),Y
            clc
            .byte $54,$23,$04,$9F
            asl
            .byte $03,$03,$97,$03
            rol L120B+2
            bpl LD7E2
            inc ABUFPT+1,X
            .byte $9E,$03,$3C,$14,$07,$12,$1C,$37,$03,$FF,$22
            bmi LD80F
            .byte $13
            clc
            .byte $22
LD7E2       .byte $1C
            ora L12EF,X
            .byte $0C,$14
            adc L1504
            .byte $22,$27
LD7ED       bmi LD7ED+1
            asl
            .byte $1C,$9E
            rol
            asl
            ora L0A1B+2,X
            .byte $3F
            inc LE103
            .byte $03
            ora L181A+1
            .byte $12
            ora LBD1C
            .byte $27,$D2,$03
            clc
            ora DOSVEC,X
            ora L04AF+2,X
            ora L11EE+1,X
LD80F       asl L4242
LD812       .byte $03,$9E,$53
            lsr DSKFMS
            .byte $FB,$1C
            ora L1830,X
            .byte $0F,$EF,$04
            bmi LD82D+1
            sbc L1F0E,X
            .byte $12,$0C
LD826       asl L632D
LD829       .byte $04,$1C,$FB
            clc
LD82D       asl L0C1A+1,X
            asl L050C+1
            .byte $9E,$7A
            sbc L1E16+1,X
            .byte $42,$0B
            asl L421A+1
            asl APPMHI+1
            dec RTCLOK
            .byte $1B,$CF,$03,$7A
            ora ADRESS
            ora ICCOMT
            bmi LD8C7
            .byte $03,$DA,$9E
            ldx CKEY
LD851       asl LFF30
            ora ICAX5Z
            .byte $C2,$04,$FF
            ora L1B0A,X
            ora L271B+1,X
LD85F       ldy #$02
            .byte $FF,$0F
            clc
            ora BUFADR,X
            clc
            .byte $54,$1C,$2F,$DF
            ldy #$2A
            ldy #$04
            .byte $0B,$1B,$12
            ora L9F10
            asl L05B7+1
LD878       eor L0C03
            asl
            .byte $17,$17
            clc
            cmp L301C+1,X
            .byte $93,$03
            asl
            .byte $0C
            ora (SHFAMT),Y
            .byte $04
            ldy #$EF
            .byte $52,$0C
            ora (L00EC),Y
            .byte $03
            asl L1514+1
            asl L0DFF
            bmi LD8B5
            clc
            .byte $54
            asl
            .byte $1B
            ora L1CFD
            bmi LD8B1+1
            clc
            ora DOSINI+1,X
            .byte $4F,$04
            bmi LD85F
            .byte $0C
            asl LA006
            .byte $7A
            lsr RAMLO,X
            .byte $12,$42
LD8B1       ora L18FB,Y
            .byte $1B
LD8B5       ora L1709+1,X
            eor CASINI+1
            .byte $17
            asl L27BE
            .byte $43
            ora DOSINI
            ora DOSVEC,X
            .byte $1C,$1C
            rol CMCMD,X
LD8C7       .byte $FF
            ldy #$AE
            .byte $1B
LD8CB       asl L2E16+1,X
            .byte $1C
            ora L18DF,X
            ora L0D6F+1,Y
            ldx #$2A
            .byte $1C
            ora (ABUFPT+2),Y
            inc L1D1C+1
            ora COLCRS+1,X
            .byte $03
            asl
            .byte $17
            ora L047E+1
            .byte $7F
            lda (RAMLO+1,X)
            bmi LD902
            asl L301C+1,X
            .byte $0F
            clc
            .byte $BF,$1B
            ldy CASINI+1,X
            ora ICCOMZ,X
            bmi LD900+1
            ldx #$52
            .byte $1F,$BB
            asl L0085
            .byte $03
            asl CASINI+1,X
LD900       bmi LD91C+1
LD902       clc
            .byte $0B
            clc
            sbc L271D,X
            bmi LD914
            .byte $1C
            bmi LD91B
            .byte $03
LD90E       bmi LD90E+1
            asl
            ora L0E1D,X
LD914       .byte $42
            ora L1C1D,Y
            .byte $DF
            ldx #$7A
LD91B       inx
LD91C       ora ICPTLZ
            bmi LD936
            ora LDB30,X
            ora L380D+1
            .byte $03
LD927       asl L7A1C
            ora ABUFPT
            ora L1BFE+1,X
            asl L1017
            ora L3011,X
            asl
LD936       .byte $FF
LD937       ora ABUFPT,X
            clc
            rol L00A2
            ldx L150B+1
            sbc L2E1B,X
            ora (DSKFMS),Y
            .byte $42
            asl IOCB3+ICBLH
            .byte $1A,$FF
            asl L1D11+1,X
            .byte $1C
            bmi LD960
            asl
LD951       .byte $42
            sbc L270E,X
            ldy CASINI
            .byte $3A
            ora PRNBUF+30,X
            ora L00F7,X
            bmi LD97C
            .byte $12
LD960       .byte $42,$C7
LD962       .byte $03,$13
            clc
            .byte $22,$FF,$1C
            ora L0C12,X
            .byte $14
            bmi LD9AF+1
            clc
            .byte $9F,$1F
            clv
            .byte $03
            stx LA405
            rol
            .byte $0C
            clc
LD979       asl L19BA+1,X
LD97C       eor CASINI+1,X
            .byte $42
            clc
            .byte $1B
            adc RAMLO,X
            asl LFD1B,X
            .byte $1C
LD987       rol L00A4
            .byte $52
LD98A       ora L3218,X
            .byte $04
            asl LB07F,X
            .byte $03
            and L1817
            .byte $1B,$42
            asl
            ora L00FF,X
            and L1830
            ora L1B0D+1,Y
            asl
LD9A1       ora L12FE+1,X
LD9A4       clc
            .byte $17
            rol L00A4
            .byte $7A
            eor LE318
            bmi LD9BD
            .byte $12
LD9AF       rol CASINI+1
            .byte $2F,$04
            iny
            ora STATUS
            asl
            .byte $BF,$0B,$23,$04
            asl
            .byte $17
LD9BD       .byte $22
            bmi LD9F0
LD9C0       bpl LD9C0+1
            .byte $12,$1F
            asl LA417
LD9C7       ldx L070E+1
            .byte $BF
LD9CB       bmi LD9CB+1
            .byte $07
            rol L0C30
            ora (APPMHI),Y
            asl L1CFD
            asl LA627
            .byte $02
            asl
            .byte $CB,$03
            ora (L00FC),Y
            clc
            ora DOSINI+1,X
            .byte $12,$17
            bpl LD9A4
            .byte $03,$8F,$03
            inc L180B+2,X
            .byte $54,$17
            bmi LDA44
LD9F0       .byte $12
LD9F1       eor L7F03,X
            cli
            .byte $04
            clc
            .byte $54
            ldx ICAX2Z
            jmp L1711+1
            .byte $DF,$0C
            asl L041C
            .byte $1C
            asl L0B30
            asl L0AFE+1
            .byte $0C
            clc
            .byte $17,$1C
            bmi LDA62+1
            asl L9C7F
            .byte $04
            ora DSKFMS,X
            .byte $0C
            asl
LDA17       ora L0D0D+1,X
            inc L53A6,X
            asl ABUFPT+1,X
            bmi LDA33
            .byte $1C
            cmp LEF04,X
            .byte $1B
            asl L4A0F
            .byte $04,$17
            asl L0E0C
            sbc L1C1B+1,X
            asl
            .byte $1B
LDA33       .byte $22
            bmi LDA7D+1
            .byte $03,$0C
            inc L0A11,X
            .byte $17
            bpl LDA4B+1
            ldx KEYDEF+1
            .byte $22
            ora L00E3
            .byte $27
LDA44       bmi LDA62+2
            lda LB303,Y
            .byte $03,$13
LDA4B       asl STATUS
            asl L17F6+1
            .byte $1A
            asl LD812,X
            .byte $03,$1C,$22,$1C
            inc L0E1D,X
            .byte $42
            ldx L00AE
            .byte $0F
            php
            .byte $5B
            asl
LDA62       cmp L0A19,X
            .byte $D4,$03
            rol L00A8
            .byte $02
            dec L1803+1
            inc L300E+1,X
            ora RTCLOK,X
            .byte $0F
            ora L2E1B+1,X
            .byte $03,$DF
            ora L7D30
            .byte $04
            clc
LDA7D       ora APPMHI,X
            .byte $1C
            rol L00FF
            tay
            rol
            ora L1853+1,X
            bmi LDA96
            asl
            .byte $BB,$22
            inx
            .byte $03
            bpl LDAA8
            .byte $27
            adc #$03
            bmi LDAA0+1
LDA95       .byte $FF
LDA96       asl
            .byte $17
            bmi LDAB2
            .byte $17
            ora ICCOMZ,X
            bmi LDA95
            .byte $0F
LDAA0       asl L1B0A
            lsr
            .byte $04
            tay
            .byte $52,$92
LDAA8       .byte $04,$F7
            ora (DSKFMS),Y
            .byte $1C
            ora L078C+1,X
            clc
            .byte $0F
LDAB2       ora L0EEF,X
            .byte $17
            bmi LDB1D+1
            .byte $04
            ora L0E1B,Y
            .byte $1F,$EF
            asl L1D17
            .byte $DA
            ora KEYDEF+1
            asl
            ora LBB30,X
            asl
            ldx WARMST
            rol STATUS
            pha
            lda L0D04,X
            asl
            .byte $FF
            ora L3009+1,X
            asl
            .byte $0B
            clc
            asl LFF1D,X
            bmi LDAFB+1
            .byte $17,$12
            ora LAA1C,X
            rol
            .byte $FF,$54
            clc
            .byte $1B,$1C
            ora LAA26,X
            .byte $52,$FF
            ora L1F0E
            .byte $12,$0C
            asl L302E+2
            .byte $FF,$0F,$1B
            clc
            .byte $42
LDAFB       bmi LDB07+1
            asl LFB1B,X
            .byte $17,$12,$17
            bpl LDB34+1
            cld
            .byte $03
LDB07       rol STATUS
            .byte $FF
            eor L0A1B
            .byte $17,$1C,$0F
            asl LDF1B
            tax
            .byte $7A
            eor L0A03
            bmi LDB2D+2
            clc
            .byte $54
LDB1C       .byte $FF
LDB1D       asl L301A+1
LDB20       .byte $0C
            ora DOSVEC,X
            .byte $1C,$1C
            eor IOCB4+ICAX4,X
            ora (LOGCOL),Y
            .byte $03,$22
            clc
LDB2D       asl ACMVAR+2,X
LDB30       asl L1BDE+1,X
            .byte $1B
LDB34       stx CASINI+1
            ldy L1601+1
            .byte $17
LDB3A       bmi LDB3A+1
            asl
            ora L120B+2
            ora L1812,X
            .byte $17,$B7,$27
            bvs LDB4C
LDB48       .byte $1C,$1C
            ldx RAMLO,Y
LDB4C       .byte $0F,$12,$1B
            sbc L300E,X
            .byte $54,$12
            ora L3F10+1,X
            .byte $04
            ldy L2BF7+1
            jmp L4217+1
            asl IOCB0+ICBLH
            .byte $32,$04
            sta LFF03
            ora APPMHI,X
            asl L301C+1
            .byte $54
            asl
            .byte $1C,$BF
LDB6F       bmi LDB1C
            .byte $03
            ora L1C2F+1,X
            asl L170E
            .byte $DF
            ldy L3351+1
            .byte $03
            asl
            bmi LDB96+1
            asl L7D54
LDB83       ora L3007,X
            .byte $1B
            asl L1E19+1
            ldy L1C03,X
            and L05CC+1,X
            .byte $53
            asl L00AC
            .byte $7A
            ora (DSKFMS),Y
LDB96       bne LDB9B
LDB98       .byte $12,$FF,$1C
LDB9B       bmi LDBA5+2
            .byte $1F
            asl
            .byte $12
            ora DOSVEC,X
            sbc L150B,Y
LDBA5       asl L3026+1
            .byte $92,$03
            and L1204,Y
            .byte $3F
            inx
            ora L0093
            .byte $04
            ldx L1302
            clc
            .byte $22,$1C
            inc L121D,X
            .byte $0C,$14
            bmi LDBEF
            .byte $0C
LDBC0       lsr LEE03,X
            .byte $1B
            asl L770D
            .byte $03
            ora BUFADR,X
            bmi LDC2F
            .byte $04,$CF
            ora L690A,Y
            .byte $03,$DB,$04
            rol
            ora (APPMHI),Y
            asl
            .byte $BF
            ora L0557
            clc
            .byte $1B
            bmi LDBED+1
            .byte $17
            asl L42FE+1
            .byte $22
            bmi LDC03
LDBE7       ora L0C0A,Y
LDBEA       asl L26E4+1
LDBED       bmi LDC1F
LDBEF       .byte $2F,$03,$EF
LDBF2       ora L00AE
LDBF4       jmp (L0D04)
            inc DSKUTL+1,X
            asl
            .byte $12,$17
            ldx #$03
            .byte $1C
LDBFF       bmi LDB83+2
LDC01       ora BPTR
LDC03       and PBCTL,X
            .byte $03,$1B
            bpl LDC2C
            bmi LDBF4+2
            .byte $03
LDC0D       clc
            .byte $FB,$1B
            ora L1B0D+1
            ldx L056A+1
            and L7F1C
            and L0C03,Y
            asl L121A+1,X
LDC1F       ora L3022,X
            sbc L150B+1,X
            asl L1B0A
            asl
            ror
            ora ROWCRS
LDC2C       .byte $F7
            ora (RTCLOK),Y
LDC2F       .byte $0C
            ora (COLRSH),Y
            .byte $03
            ldx L49AB
            adc ACMVAR,X
            ora L181A+1
            .byte $12
            ora IOCB3+ICAX1
            clc
            .byte $FB
            bpl LDC5D+2
            asl
            .byte $42,$42
            adc #$03
            .byte $0B,$22
            cmp (L00B0),Y
            .byte $02,$CB,$03,$1B,$5A,$03
            ora #$07
            .byte $72,$04
            ora LC37F,X
            .byte $03,$1C,$0F
LDC5D       asl L261A+1
            bmi LDC9E
            .byte $FF
            clc
            .byte $17
            ora L0C0A,X
            ora L2AB0,X
            sed
            ora (DOSVEC),Y
            .byte $17
            ora L231C
            .byte $04
            bvs LDC78
            ldy LFF03,X
LDC78       .byte $1C
            bmi LDC87
            asl
            .byte $17
            bmi LDC8A
            asl L30EB+2
            asl L231C,X
            .byte $03
            asl
LDC87       bpl LDCE6
            .byte $03
LDC8A       .byte $1C
            dec LB01D,X
            eor (RAMLO),Y
            ora L1409+1,X
            asl IOCB1+ICCOM
            .byte $EF
            bmi LDCB0+1
            .byte $1F
            cpy #$04
            .byte $3F
            asl
LDC9E       .byte $12
            ora L00BF,X
            asl L040F,X
            clc
            bmi LDCB0+1
            .byte $0C
            ora (RTCLOK),Y
            sbc L1F0E,Y
            asl L7AB0
LDCB0       cmp LEF05
            ora DOSINI
            .byte $DF
            asl LAC1C
            asl DOSINI
            clc
            .byte $17,$1C
            clc
            .byte $FF
            ora APPMHI,X
            rol L00B0
            ldx L173A
            ora L1BFE+1
            asl L3052+2
            .byte $3B,$1B
            asl
LDCD0       .byte $22,$FF,$0B,$1B
            clc
            clc
            .byte $14
            rol L00B2
            .byte $02,$FF,$54,$12
            ora L3011,X
            asl
            .byte $17
            clc
            .byte $47,$83,$03
LDCE6       .byte $1B,$5B,$07
            sbc L7305
            .byte $04,$0B
            asl LFF1D,X
            ora L1716+2,X
            .byte $B2
            rol
            clc
            asl L3B1B,X
            lsr RAMLO
            .byte $AF,$03
            rol L00B2
LDD00       .byte $52
LDD01       eor (WARMST,X)
            bmi LDD20
            .byte $FF
LDD06       asl L1E1B+1
            ora ABUFPT+1,X
            .byte $1C
LDD0C       bmi LDD20
LDD0E       lda L4617,X
            asl APPMHI
            .byte $1F,$12,$0C
            cpy L0E02+1
            .byte $FF,$12,$17
            bpl LDCD0
            bcs LDD6B+1
LDD20       clc
            asl L17AF,X
            eor CASINI+1
            ora L06EA+2
            .byte $22
            ldy CASINI,X
            ora L18ED
            .byte $54,$17
            ldx CASINI+1
            ora BUFADR,X
            bne LDD39+1
            .byte $12,$F3
LDD39       ora L0A11+1,X
            ora IOCB0+ICBLH,X
            ldx CMCMD,Y
            rol L00B4
            .byte $FB,$2B
            and L0C17+1,X
            .byte $14
            iny
            .byte $03
            bmi LDDA1
            .byte $FB
            clc
            asl L0D14+1,X
            bmi LDD57+2
            .byte $04,$12,$42
LDD57       inc L1818+1,X
            .byte $1C,$1C,$12,$0B
            ora KEYDEF,X
            ora L00FF
            ldy LMARGN,X
            asl
            bmi LDD76
            .byte $1B
            asl LFA0E
LDD6B       bmi LDD77
            bpl LDD7C+1
            .byte $17
            ora (CASINI+1,X)
            .byte $17
            stx CASINI+1,Y
            .byte $FF
LDD76       .byte $42
LDD77       clc
            .byte $1B
            asl LB426
LDD7C       bcs LDDC9+1
            ldx L081C+1,Y
            .byte $03
            bmi LDDD1
            asl L171B,X
            ldx CASINI+1,Y
            .byte $EF
            ldx CASINI,Y
            eor #$69
            .byte $03,$1C
            bmi LDDA1
            .byte $12,$DF,$1B
            asl L0A37+1
            .byte $13
            clc
            .byte $22,$1C
            ora L12EF,X
            .byte $0C,$14
LDDA1       eor RAMLO,X
            bmi LDDAF
            ldx ICAX1Z,Y
            inc L0E54,X
            bmi LDDDB+1
            .byte $0C
            asl
            .byte $17
LDDAF       .byte $54,$03
            dec L420A,X
            .byte $EF,$03,$0B
            clc
            asl
            .byte $1B,$8B,$03,$FF
            asl
            bmi LDDF0
            ora L181A+1,Y
            ora LFF18,X
            ora L1921+1,X
LDDC9       asl TMPROW
            .byte $1C
            ora L0EFE+1,Y
            .byte $0C
LDDD1       .byte $12,$0F,$12,$0C
            bmi LDDE4
            .byte $7A
            lda CASINI+1,X
            .byte $0C
LDDDB       ora L1812,X
            .byte $1A,$03
            clc
            tay
            asl L00FB
LDDE4       ora DOSVEC,X
            .byte $1C
            asl L081A+1
            .byte $04
            clv
            rol
            sbc L1716,X
LDDF0       .byte $0F
            ora ABUFPT+2,X
            asl L0472
            and L0173,X
            ora ICPTLZ
            clv
            .byte $5A,$7F,$0B,$CF,$03
            ora LF615,Y
            asl
            .byte $22
            clv
LDE07       .byte $82
            cpx LB912
            lda L12D8
            sbc TMPCOL+1,X
            ora L0A11,X
            ora L0A8E+1,X
            rol LOMEM
            .byte $FF,$00,$00
LDE1C       bvc LDE07
            .byte $00
            bit CHKSUM
            .byte $FF
            ora ICBAHZ
            sed
            .byte $0F,$FA
            asl LF525
            .byte $00,$00
            bmi LDE2E
LDE2E       .byte $FF
            ora ENDPT
            sed
            .byte $03,$22
            sbc BUFRHI
            .byte $33,$23
            sed
            .byte $04
            inc L7412,X
            cld
            ora #$32
            .byte $7B
            bne LDE46
            adc.w L0000,X
LDE46       .byte $7C
            sed
            .byte $04,$00,$00,$D3
            ldy L00A5
            .byte $FF
            ora #$A6
            .byte $E2
            ora L00D8
            ora L3121
            .byte $DF
            and (ICHIDZ),Y
            cld
            asl L00A7
            .byte $E3,$EB
            asl L00E2,X
            inc LE4E7,X
            sbc #$E5
            nop
            asl L00A8,X
            cld
            .byte $12
            adc IOCB4+ICBLL
            .byte $33
            rol DELTAC+1
            ora L00A9
            tax
            .byte $FF
            ora #$AB
            rti
            bcs LDE89
            .byte $27,$DC,$03
            eor RAMLO,X
            rts
            ora L0088,X
            .byte $0B
            php
            php
            cld
            jsr L101F
LDE89       ora L1E36+2
            bpl LDE98
            .byte $A7,$E3
            sbc #$E2
            .byte $EB
            sed
            .byte $E3
            sbc #$E8
            inx
LDE98       inc BUFRFL
            .byte $14
            bpl LDEA7
            sec
            .byte $13,$80
            adc POKMSK,X
            .byte $07
            sed
            .byte $03
            dey
            .byte $04
LDEA7       sed
            .byte $04
            sec
            ora DSKFMS,X
            .byte $03
            inc LA104,X
            rol POKMSK
            asl ICPTHZ
            sed
            .byte $04
            inc LE612,X
            .byte $03
            ora (TRAMSZ,X)
            bit L00B2
            and POKMSK
            asl ICBAHZ
            .byte $00
            php
            php
            beq LDECB+1
            and L00E2
            php
            plp
LDECB       dec LE002+1,X
            ora ICHIDZ
            clc
            .byte $04,$22
            ora (CASINI+1),Y
            plp
            .byte $04
            php
            ora L00B0
            .byte $27,$DC
LDEDC       .byte $03,$22
            rol L00E2
            php
            bpl LDEE8
            pla
            ora POKMSK
            ora L00CC
LDEE8       .byte $7C
            adc L0427+1,X
            .byte $22,$03,$7C,$32,$F7
            asl L00E2
            php
            lsr L050F+1
            adc L04E7+1,X
            bpl LDF01
            jsr L2521
            beq LDF06+1
LDF01       .byte $89
            bmi LDEDC
            asl
            .byte $E2
LDF06       ora POKMSK
            ora ICDNOZ
            dey
            .byte $03
            bpl LDF16
            .byte $74
            pla
            beq LDF19+1
            bit ICHIDZ
            .byte $C7,$03
LDF16       and L00E2
            php
LDF19       bpl LDF33+1
LDF1B       php
LDF1C       asl L0000
            .byte $F2
            ora #$38
            php
            jsr LF807
            asl
            .byte $FA
            asl DBYTLO
            clc
            .byte $03,$03
LDF2D       .byte $04,$00
            bpl LDF36+1
            .byte $0F,$03
LDF33       beq LDF3B+1
            .byte $FA
LDF36       asl L0610
            dec RAMLO,X
LDF3B       bpl LDF42+1
            bmi LDF45
            .byte $00
            dec RTCLOK,X
LDF42       bpl LDF4A
            cld
LDF45       php
            bmi LDF4C+1
            cld
            clc
LDF4A       bpl LDF4E+1
LDF4C       bcs LDF53
LDF4E       bpl LDF57
            .byte $00
            php
            .byte $04
LDF53       bcs LDF6A
            bpl LDF6E
LDF57       .byte $FA
            asl TIMER2
            inc.w ICPTHZ,X
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
LDF6A       .byte $00,$00,$00,$00
LDF6E       .byte $00,$00
            bvc LDF2D
            eor L00FF,X
            .byte $27
            cmp L00F5,X
            dec L00FF,X
            .byte $23,$D7,$F7,$DF,$7C
LDF7E       bmi LDF7E+1
            ora L3230+1
            .byte $32,$32,$34,$5B,$FC,$03,$37
            sbc #$0E
            bmi LE00A
            cld
            .byte $0F
            lda (L00B2),Y
            inc LB2B3,X
            ldy L00B3,X
            lda L00B6,X
            .byte $B7
            cmp (APPMHI,X)
            .byte $FB
            bcs LE01A+1
            cli
            sei
            eor L23FF,Y
            .byte $7A,$7B,$00
            sec
            plp
            sei
            .byte $0F
            sbc (SOUNDR),Y
            .byte $00,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF
LE000       asl
            tay
LE002       lda LEA00,Y
            sta CHKSUM
            lda LEA01,Y
LE00A       sta BUFRLO
            cpy #$20
            bcs LE01C
            lda #$4C
            ldx #$00
            ldy #$00
            beq LE025
LE018       stx CHKSUM
LE01A       sty BUFRLO
LE01C       ldy #$00
            lda (CHKSUM),Y
            tax
            iny
            lda (CHKSUM),Y
            iny
LE025       stx LE04A+1
            sta LE04A+2
            ldx #$00
            sec
LE02E       lda (CHKSUM),Y
            rol
            sta BFENHI
            iny
            bne LE046
            inc BUFRLO
            bne LE046
LE03A       inc LE04A+2
LE03D       iny
            bne LE042
            inc BUFRLO
LE042       asl BFENHI
            beq LE02E
LE046       bcc LE052
            lda (CHKSUM),Y
LE04A       sta L0400,X
            inx
            bne LE03D
            beq LE03A
LE052       txa
            adc LE04A+1
            sta LE04A+1
            sta LE087+1
            lda #$00
            tax
            adc LE04A+2
            sta LE04A+2
            sta LE087+2
            lda (CHKSUM),Y
            beq LE094
            adc LE087+1
            sta LE084+1
            lda LE087+2
            adc #$FF
            sta LE084+2
            iny
            bne LE07F
            inc BUFRLO
LE07F       lda (CHKSUM),Y
            sta LE08B+1
LE084       lda L1234,X
LE087       sta L5678,X
            inx
LE08B       cpx #$01
            bne LE084
            txa
            bne LE03D
            beq LE03A
LE094       rts
LE095       stx LE0A7+1
            sta LE0A7+2
            lda #$80
            sta BFENHI
            rts
LE0A0       lda #$01
LE0A2       asl BFENHI
            bne LE0B6
            pha
LE0A7       lda L1234
            inc LE0A7+1
            bne LE0B2
            inc LE0A7+2
LE0B2       rol
            sta BFENHI
            pla
LE0B6       rol
            bcc LE0A2
            rts
LE0BA       sei
            jsr L08C1
            jsr L1ECB
            lda L0090
            sta LD021
            sta LD01F+1
            sta OLDCHR
            dec NGFLAG
            ldx #$1B
            ldy #$DE
            jsr LE018
            inc NGFLAG
            lda #$50
            sta LE0E2+2
            lda #$D8
            sta LE0E8+2
            ldy #$00
LE0E2       ldx L5000,Y
            lda LC100,X
LE0E8       sta AFP,Y
            iny
            bne LE0E2
            inc LE0E2+2
            inc LE0E8+2
            lda LE0E2+2
            and #$03
            bne LE0E2
            jsr L1ECB
            lda #$1B
            sta GRAFM
LE103       lda #$C8
            ora FREQ
            sta COLPF0
            lda #$4F
            sta COLPF2
            lda L0090
            sta LD021
            sta LD01F+1
            sta OLDCHR
            dec NGFLAG
            jsr M0PF
            inc NGFLAG
            lda #$00
            sta TEMPCHR
            lda #$11
            sta L00A4
LE128       jsr L1ECB
            jsr L0B5C
            jsr L42BB
            lda ICPTHZ
            sta DINDEX
            bmi LE13B
            inc TEMPCHR
            bne LE128
LE13B       ldx #$5E
            ldy #$1E
            stx LFFFE
            sty LFFFF
            lda #$F7
            sta COLPM0
LE14A       rts
LE14B       .byte $F6,$0C,$BB,$8B,$C1,$0E,$00,$B0,$F7,$7B,$E0,$8C,$CE,$00,$EF,$74
            .byte $0E,$B6,$71,$6E,$00,$78,$12,$8F,$29,$51,$9E,$00,$DF,$15,$0D,$65
            .byte $51,$0E,$00,$A7,$82,$9A,$72,$41,$9E,$00,$3C,$E6,$0B,$7B,$51,$BE
            .byte $00,$F8,$1B,$0B,$EB,$71,$2E,$00
LE183       .byte $A3
LE184       .byte $A4,$B4,$A4,$A6,$B2,$B2,$B2,$A0,$AA,$B7,$AA,$A0,$AA,$B7,$AA,$AC
            .byte $A0,$AC,$A0,$AC,$B4,$AC,$B4
LE19B       .byte $00,$28,$00,$90
LE19F       .byte $00,$00,$F7,$00,$AC,$00,$AC,$00,$00,$F1,$50,$07,$AC,$00,$AC,$FF
            .byte $FF,$F7,$9D,$07
LE1B3       .byte $00,$00
LE1B5       .byte $00,$FF,$00
LE1B8       .byte $00,$57,$01,$34,$00,$88,$00,$00,$FF,$50
LE1C2       .byte $0A,$03
LE1C4       .byte $05
LE1C5       .byte $07,$34,$28
LE1C8       .byte $28,$35,$A0,$B8
LE1CC       .byte $CE,$E0,$A9,$AB,$AD,$AF,$04,$03,$05,$07
LE1D6       .byte $00,$00
LE1D8       .byte $00,$01,$02,$02,$02,$01,$00,$00
LE1E0       .byte $F4,$00,$0C
LE1E3       .byte $00,$FF,$7F,$55,$3F,$33,$2A,$24,$1F,$1C,$19,$17,$15,$13,$12,$11
            .byte $0F,$0F,$0E,$0D,$0C,$0C,$0B,$0B,$0A,$0A,$09,$09,$09,$08,$08,$08
            .byte $00,$FF,$FE,$FF,$FC,$FF,$FC,$FC,$F8,$FC,$FA,$FD,$FC,$F7,$FC,$FF
            .byte $F0,$FF,$FC,$F7,$F0,$FC,$F2,$FD,$F0,$FA,$EA
LE21E       .byte $00,$20,$40,$60
LE222       .byte $80,$A0,$C0,$E0
LE226       .byte $00,$01,$01,$02,$02,$02,$03,$03,$04,$04,$04,$05,$05,$05,$06,$06
            .byte $06,$07,$07,$07,$08,$08,$08,$09
LE23E       .byte $01,$23,$39,$47,$49,$96,$02,$29,$20,$76,$93,$16,$71,$98,$14,$15
            .byte $29,$11,$42,$51,$21,$34,$83,$99
LE256       .byte $04,$01,$02,$04,$02,$01,$08,$02,$01,$04,$04,$02,$02,$02,$01,$04
            .byte $01,$02,$02,$02,$04,$08,$01,$04
LE26E       .byte $00,$00,$00,$00,$00,$00,$00,$00,$80,$01,$00,$00,$00,$00,$01,$01
            .byte $02,$83,$83,$01,$82,$01,$01,$82
LE286       .byte $00,$90,$8C,$8C,$8C,$90,$8C,$88,$90,$88,$82,$0C,$0C,$0C,$4C,$48
            .byte $50,$4C,$4C,$4C,$48,$48,$48,$44
LE29E       .byte $02,$06,$07,$04,$06,$01,$08,$01,$04,$05,$08,$01,$09,$05,$03,$06
            .byte $08,$02,$09,$04,$03,$09,$08,$03
LE2B6       .byte $6F,$63,$87,$8A,$53,$5D,$4F,$C3,$BF,$6E
LE2C0       .byte $7F,$3F,$3F,$3F,$3F,$1F,$1F,$1F,$1F
LE2C9       .byte $0F
LE2CA       .byte $00,$19,$32,$4B,$64,$7D,$96,$AF,$C8,$FA
LE2D4       .byte $00,$0A,$14,$1E,$28,$32,$3C,$46,$50,$C8
LE2DE       .byte $00,$05,$0A,$19,$00,$05,$0A,$19
LE2E6       .byte $3B,$1B,$12,$0E,$0F,$12,$17,$10
LE2EE       .byte $B0
LE2EF       .byte $40,$0A,$42,$0E,$30,$18,$17,$25,$B0,$40,$0A,$42,$0E,$30,$18,$1F
            .byte $0E,$1B,$B0,$49,$0A,$1E,$1C,$0E,$B0,$3C,$11,$0E,$0E,$1C,$0E,$B0
            .byte $3C,$18,$17,$1D,$12,$17,$1E,$0E,$B0
LE318       .byte $3C,$18,$15,$18,$1E,$1B,$B0,$3B,$15,$14,$2E,$50,$11,$1D,$0E,$B0
            .byte $4F,$18,$15,$1E,$42,$0E,$30
LE32F       .byte $00
LE330       .byte $05,$B0,$4C,$11,$12,$19,$30
LE337       .byte $01,$B0,$46,$18,$0B,$12,$15,$0E,$B0,$50,$0E,$0A,$19,$18,$17,$B0
            .byte $4D,$1B,$0A,$17,$1C,$0F,$0E,$1B,$B0,$3C,$0A,$19,$1D,$1E,$1B,$0E
            .byte $0D,$B0,$3C,$18,$15,$18,$1E,$1B,$24,$30,$09,$09,$B0,$3F,$12,$17
            .byte $12,$1C,$11,$30,$2E,$09,$09,$B0,$3D,$0E,$0A,$0D,$15,$18,$0C,$14
            .byte $B0,$3C,$18,$42,$19,$15,$0E,$1D,$0E,$B0,$4B,$0E,$13,$0E,$0C,$1D
            .byte $0E,$0D,$B0,$3B,$1E,$1B,$17,$1D,$30,$48,$1E,$1D,$B0,$45,$12,$0F
            .byte $1D,$B0,$3C,$18,$17,$1C,$18,$15,$0E,$B0,$09,$02,$30,$4E,$17,$12
            .byte $1D,$30,$1D,$22,$19,$0E,$30
LE3AE       .byte $00
LE3AF       .byte $00,$00,$FF,$0C,$0C,$3A,$15,$0E,$1B,$1D,$30,$2F,$FF,$10,$0C,$4C
            .byte $11,$12,$19,$30,$30,$2F,$FF,$15,$0C,$3D,$0E,$0C,$14,$30,$30,$2F
            .byte $FF,$0A,$0D,$4D,$1B,$0A,$17,$1C,$42,$12,$1C,$1C,$12,$18,$17,$FF
            .byte $16,$0E,$4D,$0E,$1B,$42,$12,$17,$0A,$1D,$0E,$0D,$FF,$0A,$06,$3A
            .byte $15,$15,$30,$4D,$12,$42,$0E,$30,$41,$12,$10,$11,$0E,$1C,$1D,$30
            .byte $4C,$0C,$18,$1B,$0E,$25,$FF,$0A,$0D,$40,$1B,$0E,$0A,$1D,$30,$4C
            .byte $0C,$18,$1B,$0E,$25,$FF,$16,$01,$49,$15,$0E,$0A,$1C,$0E,$30,$0E
            .byte $17,$1D,$0E,$1B,$30,$22,$18,$1E,$1B,$30,$12,$17,$12,$1D,$12,$0A
            .byte $15,$1C,$30,$2E,$FF,$16
LE435       .byte $1F
LE436       .byte $30,$30,$30,$30,$30,$30,$FF,$09,$0C,$40,$0A,$42,$0E,$30,$1C,$1D
            .byte $0A,$1D,$12,$1C,$1D,$12,$0C,$1C,$FF,$0C,$09,$4C,$11,$12,$19,$1C
            .byte $30,$0E,$17,$1D,$0E,$1B,$0E,$0D,$2F,$30,$FF,$0E,$0B,$4C,$11,$18
            .byte $1D,$1C,$30,$0F,$12,$1B,$0E,$0D,$2F,$30,$FF,$10,$0C,$3D,$1B,$18
            .byte $12,$0D,$30,$11,$12,$1D,$1C,$2F,$30,$FF,$12,$0A,$41,$12,$1D,$30
            .byte $0A,$0C,$0C,$1E,$1B,$0A,$0C,$22,$2F,$30,$FF,$14,$02,$4C,$1E,$0C
            .byte $0C,$0E,$1C,$1C,$0F,$1E,$15,$30,$1D,$1B,$0A,$17,$1C,$0F,$0E,$1B
            .byte $1C,$2F,$30,$FF,$16,$06,$3D,$1B,$18,$12,$0D,$1C,$30,$0D,$0E,$1C
            .byte $1D,$1B,$18,$22,$0E,$0D,$2F,$30,$FF,$13,$09,$3A,$0C,$0C,$1E,$1B
            .byte $0A,$0C,$22,$30,$0B,$18,$17,$1E,$1C,$2F,$30,$FF,$13,$09,$49,$0A
            .byte $0C,$12,$0F,$12,$1C,$1D,$30,$0B,$18,$17,$1E,$1C,$2F,$30,$FF,$15
            .byte $09
LE4E7       .byte $3A,$15,$0E,$1B,$1D,$30,$0B,$18,$17,$1E,$1C,$2F,$30,$FF,$01,$26
LE4F7       .byte $00
LE4F8       .byte $00,$30,$42,$FF
LE4FC       .byte $00
LE4FD       .byte $00
LE4FE       .byte $00,$30,$14,$10,$FF
LE503       .byte $00
LE504       .byte $00
LE505       .byte $00,$FF,$2A,$33,$29,$5D,$5E,$5F,$60,$54,$FE,$62,$2E,$63,$64
LE514       .byte $00,$2E,$65,$66,$67,$68,$FE,$FF,$2A,$33,$29,$5D,$5E,$5F,$60,$54
            .byte $FE,$FF,$2A,$33,$29,$5D,$5E,$5F,$72,$2E,$54,$FE,$62,$2E,$71,$FE
            .byte $FF,$DD,$3F,$33,$DF,$DE,$2C,$66,$0A,$50,$FE,$FF
LE540       .byte $3A,$3B,$3C,$3D,$3E,$3F,$40,$41,$16,$43,$44,$45,$46,$47,$48,$49
            .byte $4A,$4B,$4C,$4D,$4E,$4F,$50,$51,$52,$53,$30
LE55B       .byte $7C,$3C,$FC,$FE,$1C,$FE,$38,$FE,$7C,$7C
LE565       .byte $FE,$7C,$FE,$FC,$38,$FE,$38,$FE,$FE,$FE
LE56F       .byte $00,$1C,$0E,$00,$70,$E0,$70,$00,$C6,$E6
LE579       .byte $C6,$1C,$7E,$3C,$FE,$FC,$70,$1C,$38,$66
LE583       .byte $C6,$1C,$FC,$7E,$FE,$FE,$CC,$1C,$38,$1C
LE58D       .byte $C6,$1C,$00,$0E,$00,$00,$CE,$38,$C6,$1C
LE597       .byte $FE,$1C,$FE,$FE,$1C,$FE,$FE,$38,$FE,$38
LE5A1       .byte $7C,$1C,$FE,$FC,$1C,$FC,$7C,$70,$7C,$38
LE5AB       .byte $02,$03,$03,$03,$03,$03,$03,$03
LE5B3       .byte $7F,$3F,$9F,$CF,$E7,$F3,$F9,$FC
LE5BB       .byte $09,$0C,$0E,$0F,$0F,$0F,$0F,$0F
LE5C3       .byte $FF,$FF,$3F,$8F,$E3,$F8,$FE,$FF
LE5CB       .byte $E0,$E0,$E0,$E0,$E0,$E0,$60,$20
LE5D3       .byte $23,$38,$3E,$3F,$3F,$3F,$3F,$3F
LE5DB       .byte $FF,$FF,$3F,$8F,$E3,$F8,$FE,$FF
LE5E3       .byte $F8,$F8,$F8,$F8,$F8,$F8,$38,$88
LE5EB       .byte $00,$00
LE5ED       .byte $02,$03
LE5EF       .byte $00,$00
LE5F1       .byte $A0,$60
LE5F3       .byte $78,$79
LE5F5       .byte $00,$38,$78,$62,$46,$1E,$1C,$00,$00,$24,$76,$62,$46,$6E,$24,$00
            .byte $00,$1C,$0E,$66,$66,$70,$38,$00,$00,$3C,$6E,$04,$20
LE612       .byte $76,$3C,$00,$00,$00,$00,$00,$00,$00,$00,$00,$3C,$FF,$00,$DB,$DB
            .byte $00,$FF
LE624       .byte $3C,$BA
LE626       .byte $AE,$AE,$BA,$AB,$EA,$EA,$AB,$BA,$AE
LE62F       .byte $00
LE630       .byte $00,$00,$00,$00,$00,$00,$00
LE637       .byte $00,$00,$00,$00,$00,$00,$00,$00
LE63F       .byte $00,$00,$00,$00,$00
LE644       .byte $00,$00,$00,$00,$00,$00,$00
LE64B       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
LE656       .byte $00
LE657       .byte $00,$00,$00,$00,$00,$00,$00,$00
LE65F       .byte $17,$17,$17,$17,$16,$16,$15,$15,$14,$13,$12,$11,$10,$0F,$0D,$0B
            .byte $07,$03,$00,$00,$00
LE674       .byte $13,$13,$13,$13,$12,$12,$11,$11,$10,$0F,$0E,$0D,$0C,$0A,$08,$04
            .byte $00,$00,$00,$00,$00
LE689       .byte $03,$05,$01,$04,$01,$08,$01,$00,$00,$03,$00,$00,$00,$01
LE697       .byte $0D,$0A,$12,$0C,$12,$02,$13,$14,$14,$0E,$14,$14,$14,$11
LE6A5       .byte $04,$05,$02,$06,$02,$00,$01,$09,$01,$00,$00,$03,$00,$00
LE6B3       .byte $CF,$EB,$01,$27,$41,$67,$6D,$95,$BF,$E9,$07,$31,$5B,$85
LE6C1       .byte $E6,$E6,$E7,$E7,$E7,$E7,$E7,$E7,$E7,$E7,$E8,$E8,$E8,$E8,$09,$04
            .byte $07,$08,$06,$0A,$05,$0C,$05,$0C,$04,$0E,$04,$0E,$04,$0E,$04,$0E
            .byte $05,$0C,$05,$0C,$06,$0A,$07,$08,$09,$04,$06,$0A,$05,$0C,$05,$0C
            .byte $05,$0C,$05,$0C,$05,$0C,$05,$0C,$05,$0C,$05,$0C,$05,$0C,$06,$0A
            .byte $09,$01,$08,$03,$07,$06,$06,$08,$04,$0A,$03,$0B,$02,$0D,$02,$0F
            .byte $02,$10,$03,$10,$03,$10,$04,$10,$06,$0E,$07,$0E,$08,$0C,$08,$0B
            .byte $09,$08,$0A,$06,$0D,$02,$07,$09,$06,$0B,$06,$0B,$06,$0B,$06,$0B
            .byte $07,$09,$07,$09,$06,$0B,$06,$0B,$06,$0B,$06,$0A
LE73D       .byte $07,$09,$07,$09,$0D,$01,$0C,$03,$0A,$06,$09,$09,$08,$0B,$08,$0C
            .byte $07,$0E,$06,$0F,$04,$11,$03,$11,$03,$11,$03,$10,$02,$0F,$02,$0E
            .byte $03,$0C,$04,$0B,$06,$08,$07,$06,$08,$02,$03,$12,$00,$17,$02,$12
            .byte $02,$00,$03,$01,$03,$04,$04,$04,$05,$03,$06,$04,$06,$05,$07,$06
            .byte $08,$06,$0A,$04,$0A,$05,$0B,$05,$0B,$06,$0C,$06,$0D,$05,$0F,$04
            .byte $11,$03,$13,$01,$14,$01,$15,$01,$0B,$01,$0B,$01,$0B,$01,$0A,$03
            .byte $0A,$03,$0A,$03,$0A,$03,$0A,$04,$09,$05,$09,$05,$09,$05,$0A,$04
            .byte $0A,$03,$0A,$03,$0A,$04,$0B,$03,$0C,$02,$0C,$01,$0B,$02,$0B,$01
            .byte $0B,$01,$15,$01,$13,$02,$11,$04,$10,$04,$0F,$04,$0F,$04,$0E,$04
            .byte $0D,$04,$0B,$04,$0A,$05,$09,$05,$09,$05,$08,$04,$07,$03,$05,$04
            .byte $04,$05,$04,$04,$03,$04,$03,$03,$02,$02,$02,$00,$08,$08,$02,$12
            .byte $00,$17,$00,$17,$00,$17,$00,$17,$00,$17,$00,$17,$00,$17,$00,$17
            .byte $00,$17,$00,$17,$00,$17,$02,$12,$08,$06,$07,$01,$06,$03,$04,$07
            .byte $03,$0A,$01,$0D,$00,$0F,$01,$0E,$02,$0F,$02,$10,$03,$10,$03,$11
            .byte $04,$10,$05,$10,$07,$0E,$07,$0F,$08,$0F,$09,$0D,$0A,$0A,$0C,$07
            .byte $0E,$03,$0F,$01,$05,$0C,$05,$0C,$05,$0C,$04,$0D,$04,$0E,$03,$0F
            .byte $04,$0E,$04,$0E,$05,$0E,$05,$0E,$05,$0E,$04,$0F,$04,$0F,$04,$0E
            .byte $03,$0F,$03,$0F,$04,$0E,$04,$0D,$05,$0C,$05,$0C,$06,$0B,$0F,$01
            .byte $0E,$03,$0C,$07,$0A,$0A,$09,$0D,$09,$0E,$08,$0E,$06,$0F,$05,$10
            .byte $04,$10,$03,$11,$03,$11,$02,$10,$02,$0F,$01,$0F,$00,$0F,$00,$0E
            .byte $01,$0C,$03,$08,$05,$04,$07,$01,$0A,$02,$06,$0A,$04,$0E,$03,$10
            .byte $02,$12,$01,$14,$01,$14,$00,$16,$00,$16,$00,$16,$00,$16,$01,$14
            .byte $01,$14,$02,$12,$03,$10,$04,$0E,$06,$0A,$0A,$02
LE8A9       .byte $07,$0F,$07,$0F,$06,$0E,$06,$0E,$05,$0D,$05,$0D,$04,$0C,$04,$0C
            .byte $03,$0B,$03,$0B
LE8BD       .byte $00,$00,$01,$02,$00,$00,$01,$02
LE8C5       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
LE8D4       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
LE8E3       .byte $0A,$03,$05,$07,$0A,$03,$05,$07,$0A,$03,$05,$07,$0A,$03,$05
LE8F2       .byte $00
LE8F3       .byte $00,$C8,$2E,$19,$30,$C3,$30,$A7,$41
LE8FC       .byte $D5
LE8FD       .byte $2E,$FA,$2E,$1C,$2F,$37,$2F,$47,$2F
LE906       .byte $00,$14,$3C,$00,$68,$51,$B2,$00,$FF,$12,$F0,$F0,$EB,$13,$D8,$03
            .byte $FD,$D4,$0E,$F7,$F2,$FA,$FA,$F3,$DF,$15,$FE,$F4,$F6,$6E,$D8,$0D
            .byte $FB,$FB,$D8,$0D,$F7,$F5,$FE,$8B,$04,$9F,$F6,$D8,$03,$FF,$0D,$F8
            .byte $FA,$FA,$F9,$F5,$73,$FF,$0D,$FE,$00,$F7,$D8,$00,$10,$BA,$00,$00
            .byte $98,$FF,$FB,$04,$FC,$0B,$F1,$F1,$EF,$0F,$DF,$04,$00,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
            .byte $FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF,$FF
LEA00       .byte $5C
LEA01       .byte $EB,$A3,$EB,$39,$EC,$70,$EC,$39,$ED,$FE,$ED,$DA,$EE,$C7,$EF,$78
            .byte $F0,$12,$F1,$7A,$F1,$46,$F2,$08,$F3,$C2,$F3,$21,$F4,$F9,$F4,$F3
            .byte $F5,$26,$F6
LEA24       .byte $CE
LEA25       .byte $F6,$E1,$F6,$05,$F7,$13,$F7,$40,$F7,$6C,$F7,$9D,$F7,$D9,$F7,$FE
            .byte $F7,$24,$F8,$39,$F8,$6B,$F8,$98,$F8,$C0,$F8,$D5,$F8,$00,$F9
LEA44       .byte $0A,$0D,$00,$02,$03,$01,$05,$07,$11,$07,$11,$13,$0F,$0D,$07,$08
LEA54       .byte $13,$12,$11,$14,$12,$13,$14,$0A,$0C,$05,$13,$07,$09,$10,$0E,$11
LEA64       .byte $08,$07,$09,$07,$08,$09,$08,$08,$09,$06,$0C,$0B,$06,$07,$08,$07
LEA74       .byte $0C,$0E,$0B,$0D,$0C,$0B,$0C,$0D,$0C,$12
LEA7E       .byte $08,$0A,$11,$0E,$0D,$0E
LEA84       .byte $00,$0F,$0D,$0F,$1B,$1B,$07,$07,$0B,$20,$1A,$10,$0C,$08,$25,$1F
            .byte $17,$13,$0F,$0E,$0E,$15,$0F,$2D,$2D,$0D,$23,$08,$26,$26,$1A,$00
LEAA4       .byte $00,$24,$25,$27,$27,$27,$24,$24,$24,$29,$24,$27,$27,$27,$2A,$27
            .byte $25,$25,$25,$25,$25,$25,$24,$22,$22,$24,$27,$2D,$2E,$2E,$2D,$00
LEAC4       .byte $00,$00,$02,$03,$0E,$0F,$0A,$0B,$0C,$20,$01,$04,$05,$06,$21,$04
            .byte $04,$05,$06,$07,$08,$09,$09,$2E,$2F,$01,$03,$06,$0A,$0B,$0D
LEAE3       .byte $00
LEAE4       .byte $FF
LEAE5       .byte $00,$00,$00,$00,$00,$01,$01,$01,$02,$02,$02,$02,$02,$03,$03,$04
            .byte $04,$04,$04,$04,$04,$05,$05,$05,$06,$06,$07,$07,$07,$07,$FF
LEB04       .byte $00,$01,$01,$02,$02,$03,$04,$05,$06,$07,$03,$06,$09,$09,$08,$09
LEB14       .byte $05,$0F,$05,$03,$10,$12,$14,$16,$16,$16,$09,$09,$09,$12,$00,$00
LEB24       .byte $01,$01,$01,$01,$01,$01,$01,$01,$01,$01,$03,$03,$03,$02,$01,$01
LEB34       .byte $11,$0F,$08,$0D,$12,$12,$12,$0E,$0C,$08,$0D
LEB3F       .byte $0D,$0D,$04,$1A,$18
LEB44       .byte $05,$09,$14,$1C,$1A,$17,$0F,$15
LEB4C       .byte $00,$03,$00,$01,$02,$07,$01,$04
LEB54       .byte $0A,$09,$05,$02,$06,$03,$02,$07,$B5,$00,$FF,$CD,$07,$04,$FF,$03
            .byte $06,$2C,$3A,$05,$FA,$03,$15,$15,$15,$05,$01,$3A,$08,$80,$04,$95
            .byte $09,$01,$79,$07,$0B,$07,$76,$04,$06,$36,$3A,$05,$F3,$16,$15,$15
            .byte $15,$31,$04,$BC,$37,$05,$12,$31,$C0,$03,$BD,$03,$03,$05,$80,$37
            .byte $C1,$04,$3B,$04,$09,$00,$3C,$36,$45,$04,$C4,$38,$CA,$EC,$00,$B5
            .byte $00,$FF,$03,$07,$04,$FF,$06,$0C,$F8,$0A,$06,$35,$E8,$04,$FC,$28
            .byte $05,$15,$FF,$06,$01,$F8,$09,$13,$AD,$05,$C0,$2C,$08,$80,$07,$0B
            .byte $15,$76,$05,$02,$F0,$0B,$15,$15,$0D,$6D,$07,$78,$2F,$76,$06,$B0
            .byte $03,$26,$BD,$03,$6B,$03,$10,$B8,$05,$34,$29,$05,$03,$36,$08,$4E
            .byte $35,$04,$03,$2D,$07,$34,$2A,$0D,$04,$09,$01,$03,$94,$18,$7C,$04
            .byte $77,$06,$11,$38,$05,$09,$3C,$20,$0A,$05,$74,$06,$03,$0B,$16,$12
            .byte $80,$07,$13,$48,$04,$EA,$04,$0C,$C4,$21,$08,$06,$4B,$09,$8A,$06
            .byte $11,$16,$4C,$29,$01,$04,$A1,$08,$80,$07,$0A,$78,$07,$88,$28,$9C
            .byte $28,$20,$05,$06,$60,$AE,$3A,$05,$03,$1F,$03,$2E,$2D,$20,$13,$2E
            .byte $4D,$B4,$CA,$00,$00,$B5,$00,$FF,$CB,$07,$04,$FF,$13,$06,$1E,$2A
            .byte $05,$AB,$15,$FF,$06,$1E,$FE,$03,$10,$F4,$05,$14,$15,$D0,$14,$05
            .byte $C0,$2B,$03,$BF,$07,$F9,$0C,$C0,$2C,$80,$0B,$AC,$16,$80,$33,$08
            .byte $01,$13,$04,$09,$40,$2A,$D6,$00,$00,$01,$F4,$00,$B4,$00,$FF,$05
            .byte $07,$04,$FF,$0A,$0C,$F4,$0B,$F3,$09,$9F,$06,$D8,$06,$FA,$18,$05
            .byte $1C,$1D,$1D,$1D,$A5,$15,$FF,$06,$01,$F8,$07,$F3,$0D,$05,$C0,$1F
            .byte $15,$D6,$15,$1C,$C9,$08,$0D,$74,$0A,$06,$15,$E7,$03,$12,$C0,$27
            .byte $8A,$09,$B4,$03,$10,$F5,$09,$C0,$28,$14,$C0,$0D,$2A,$B5,$0A,$80
            .byte $27,$08,$4C,$03,$0C,$48,$03,$02,$44,$03,$82,$0B,$C0,$33,$3C,$0C
            .byte $80,$0D,$A7,$04,$A8,$04,$0B,$C0,$27,$F6,$03,$15,$15,$01,$03,$06
            .byte $16,$0F,$F5,$08,$2E,$EC,$03,$80,$37,$0D,$30,$04,$06,$15,$07,$2B
            .byte $03,$36,$40,$1E,$DE,$03,$04,$0A,$55,$06,$04,$0B,$40,$0C,$1B,$30
            .byte $05,$26,$03,$80,$1F,$05,$14,$03,$05,$1C,$1D,$92,$1C,$80,$0D,$2D
            .byte $05,$01,$C0,$24,$44
LED09       .byte $07,$1C,$C0,$0D,$90,$11,$80,$2A
LED11       .byte $04,$06,$1D,$0B,$05,$38,$07,$35,$08,$C0,$2B,$52,$06,$05,$01,$F5
            .byte $0F,$05,$EE,$04,$40,$1F,$08,$8C,$0A,$D4,$04,$0A,$F5
LED2E       .byte $0F,$0A,$EE,$04,$09,$40,$1E,$E2,$3A,$00,$00,$B4,$00,$FF,$03,$07
            .byte $04,$FF
LED40       .byte $0D,$06,$EC,$04,$FC,$2C,$D4,$05,$15,$FF,$0D,$05,$C0,$34,$0F,$BC
            .byte $04,$FB,$05,$F8,$15,$0D,$04,$04,$0C,$FE,$03,$74,$2A,$80,$10,$E4
            .byte $15,$15,$01,$FE,$03,$6B,$04,$10,$74,$25,$40,$04,$9C,$07,$06,$05
            .byte $C0,$08,$0D,$04,$0B,$34,$2B,$40,$04,$F9,$05,$1C,$1D,$15,$1D,$77
            .byte $06,$85,$03,$05,$82,$03,$6C,$07,$34,$03,$3A,$22,$C0,$05,$06,$05
            .byte $02,$0A,$04,$FC,$0A,$04,$04,$09,$15,$08,$01,$05,$F7,$03,$3A,$BA
            .byte $08,$BC,$1B,$05,$15,$16,$0B,$04,$18,$0B,$04,$84,$03,$48,$06,$FA
            .byte $08,$F1,$06,$6E,$03,$14,$36,$1B,$80
LEDB9       .byte $0F,$89,$0C,$85,$06,$46,$06,$3A,$09,$09,$40,$23,$0B,$06,$05,$80
            .byte $1E,$7C,$0A,$46,$04,$C4,$1D,$80,$08,$10,$05,$12,$06,$3A,$03,$0A
            .byte $3B,$09,$4A,$1B
LEDDD       .byte $40,$0E,$CC,$0C,$11,$C0,$33,$0F,$17,$04,$00,$FB,$05,$4C,$07,$FB
            .byte $06,$46,$1F,$80,$16,$4C,$24,$28,$0C,$04,$05,$00,$8C,$2F,$DC,$48
            .byte $00,$B7,$00,$FF,$03,$07
LEE03       .byte $04,$FF,$05,$0C,$04,$06,$37,$F2,$04,$FC,$32,$05,$15,$FF,$05,$01
LEE13       .byte $15,$0D,$1F,$B7,$03,$BC,$33,$C0,$07,$05,$15,$05,$1C,$1C,$C3,$1C
            .byte $08,$78,$03,$70,$04,$74,$2B,$80,$06,$13,$05,$26,$7E,$03,$F8,$05
            .byte $12,$B2,$05,$40,$2F,$11,$16,$80,$03,$77,$37,$04,$07,$0A,$06,$C0
            .byte $33,$07,$04,$04,$ED,$09,$15,$08,$46,$04,$09,$03,$F9,$03,$02,$C0
            .byte $04,$0E,$3A,$05,$38,$25,$40,$04,$45,$05,$FC,$0C,$EF,$08,$9F,$0D
            .byte $B8,$22,$DD,$03,$0C,$0A,$02,$04,$06,$B5,$03,$7B,$03,$04,$0C,$88
            .byte $07,$0B,$2B,$03,$0F,$47,$A7,$0A,$13,$30,$1B,$09,$03,$06,$04,$05
            .byte $15,$05
LEE85       .byte $FF,$1C,$1C,$1C,$05,$1C,$1D,$1D,$15,$CB,$15,$1C,$EE,$05,$80,$0C
            .byte $09,$C0
LEE97       .byte $1E,$14,$15,$88,$14,$C0,$03,$C7,$03,$C8,$06,$1D,$C8,$24,$80,$0A
            .byte $CA,$05,$85,$05,$0C,$05,$08,$08,$50,$1B,$40,$13,$08,$06,$03,$0A
            .byte $91,$02,$0C,$04,$36,$04,$1C,$3E,$03,$C0,$2E,$8C,$07,$1D,$C0,$1D
            .byte $1D,$48,$2B,$C0,$11,$0C,$04,$C8,$2B,$1E,$08,$43,$04,$00,$8C,$30
            .byte $D6,$48,$00,$B4,$00,$FF,$03,$07,$04,$FF,$03,$06,$F6,$04,$FC,$36
            .byte $FD,$05,$16,$10,$15,$15,$0D,$BC,$3A,$05,$FF,$12,$15,$15,$15,$05
            .byte $10,$10,$10,$E2,$08,$04,$0C,$75,$04,$FC,$05,$70,$2B
LEF04       .byte $05
LEF05       .byte $C1,$03,$C5,$15,$01,$FB,$04,$FA,$07,$F0,$04,$05,$C0,$2E,$07,$FF
            .byte $0A,$04,$02,$04,$06,$15,$08,$06,$E0,$15,$15,$0F,$FD,$05,$30,$04
            .byte $78,$27,$80,$04,$7C,$05,$E0,$05,$15,$03,$74,$05,$76,$0C,$B8,$04
            .byte $34,$1F,$40,$03,$CD,$07,$09,$C0,$06,$79,$03,$04,$0C,$01,$03,$0C
            .byte $E1,$04,$0E,$02,$82,$05,$28,$05,$BC,$1F,$40,$03,$05,$95,$03,$80
            .byte $06,$75,$05,$10,$82,$03,$14,$6F,$05,$16,$EC,$08,$04,$0B,$74,$06
            .byte $16,$05,$40,$1C,$89,$04,$C8,$04,$02,$D4,$03,$35,$08,$0D,$C8,$05
            .byte $38,$08,$3C,$04,$8A,$09,$40,$1C,$0A,$07,$80,$03,$12,$C0,$06,$05
            .byte $48,$05,$45,$01,$03,$14,$C4,$1F,$C0,$0F,$16,$03,$02,$74,$03,$02
            .byte $80,$09,$0C,$05,$FD,$04,$C4,$1F,$80,$0F,$8A,$07,$3A,$04,$C8,$22
            .byte $7F,$40,$0A,$08,$04,$04,$06,$11,$16,$11,$C7,$0D,$04,$84,$03,$88
            .byte $2A,$40,$0A,$15,$15,$03,$00,$18,$23,$36,$18,$82,$03,$C4,$39,$CA
            .byte $40,$00,$B5,$00,$FF,$46,$07,$04,$FF,$05,$06,$B1,$38,$05,$B1,$15
            .byte $FF,$04,$13,$0D,$B9,$06,$B6,$32,$C0,$06,$16,$DE,$05,$14,$B8,$05
            .byte $1D,$1C,$14,$05,$27,$2E,$78,$87,$04,$0C,$02,$04,$09,$79,$05,$74
            .byte $04,$76,$06,$3C,$30,$29,$40,$04,$01,$15,$15,$03,$C0,$09,$2F,$05
            .byte $9A,$16,$30,$04,$36,$25,$05,$12,$CD,$04,$07,$02,$09,$0C,$C0,$06
            .byte $BA,$05,$BC,$25,$80,$04,$05,$15,$01,$07,$82,$04,$1A,$73,$03,$6D
            .byte $06,$69,$04,$13,$05,$C0,$27,$18,$01,$06,$93,$13,$B5,$03,$80,$0D
            .byte $09,$3C,$20,$17,$04,$0B,$15,$C8,$08,$06,$02,$09,$40,$05,$11,$C4
            .byte $25,$40,$08,$4D,$03,$F6,$08,$04,$04,$02,$01,$03,$04,$09,$0A,$05
            .byte $03,$C4,$27,$40,$08,$82,$09,$46,$05,$C6,$28,$80,$0A,$15,$05,$C3
            .byte $16,$07,$8A,$05,$86,$2D,$22,$06,$47,$03,$04,$04,$80,$0A,$CA,$30
            .byte $D2,$83,$00,$B5,$00,$FF,$46,$07,$04,$FF,$05,$06,$B1,$38,$05,$A3
            .byte $15,$FF,$05,$0D,$B9,$06,$B8,$32,$C0,$07,$05,$14,$67,$B8,$05,$1E
            .byte $05,$29,$30,$87,$04,$0C,$02,$04,$87,$09,$79,$06,$78,$07,$32
LF0A4       .byte $2B,$40,$04,$01,$15,$15,$83,$03,$C0,$07,$F5,$03,$32,$07,$36,$27
            .byte $40,$04,$05,$15,$BB,$07,$01,$07,$04,$0E,$02,$70,$03,$02,$0B,$41
            .byte $2B,$03,$08,$78,$26,$C0,$06,$89,$06,$B5,$05,$FA,$0A,$15,$DE,$13
            .byte $05,$40,$26,$08,$04,$0A,$06,$C0,$0D,$2E,$3A,$04,$6D,$04,$09,$40
            .byte $26,$15,$14,$10,$8E,$07,$65,$80,$06,$16,$0D,$C4,$26,$40,$05,$12
            .byte $4E,$0C,$15,$02,$C6,$04,$C4,$29,$80,$09,$C0,$0A,$0E,$23,$40,$0F
            .byte $16,$88,$0B,$20,$86,$2D,$20,$06,$08,$C8,$37,$D0,$81,$00,$B5,$00
            .byte $FF,$8D,$07,$04,$FF,$05,$06,$6A,$38,$05,$B6,$15,$FF,$04,$13,$0D
            .byte $BA,$38,$05,$03,$BF,$04,$ED,$07,$09,$14,$B8,$03
LF130       .byte $14,$05,$80,$38,$01,$4D,$BA,$05,$15,$7A,$04,$36,$2F,$0D,$02,$01
            .byte $04,$0A,$80,$06,$39,$05,$33,$03,$7C,$30,$80,$06,$C6,$04,$79,$04
            .byte $BC,$04,$6A,$C0,$3B,$05,$1E,$C0,$3E,$0D,$34,$03,$09,$40,$36,$8A
            .byte $14,$01,$03,$04,$30,$40,$05,$16,$01,$03,$11,$86,$05,$20,$84,$31
            .byte $16,$04,$08,$C7,$05,$C6,$34,$CE,$B8,$00,$B5,$00,$FF,$05,$07,$04
            .byte $FF,$0C,$0C,$F2,$12,$06,$3A,$D8,$06,$FA,$18,$05,$16,$15,$FF,$0B
            .byte $05,$F3,$0C,$6B,$F2,$05,$1C,$05,$C0,$1F,$12,$C0,$1C,$1D,$1D,$BB
            .byte $1D,$C0,$1F,$0D,$04,$06,$7F,$0B,$01,$15,$84,$15,$2F,$08,$E9,$03
            .byte $72,$05,$80,$20,$03,$CC,$03,$C0,$0C,$F4,$05,$16,$14,$15,$FE,$03
            .byte $16,$35,$03,$C0,$24,$F8,$0D,$04,$0A,$02,$02,$7D,$09,$0E,$03,$B1
            .byte $08,$01,$75,$03,$A6,$05,$80,$20,$09,$0D,$C0,$0E,$40,$25,$02,$04
            .byte $08,$7F,$0C,$04,$02,$02,$04,$04,$0E,$02,$02,$AA,$0A,$01,$06,$04
            .byte $F8,$03,$0E,$EA,$04,$0B,$80,$23,$3D,$FC,$09,$0E,$05,$1C,$1C,$1C
            .byte $1D,$32,$06,$16,$00,$40,$27,$C0,$0E,$AE,$09,$35,$05,$C0,$2D,$03
            .byte $08,$72,$0B,$80,$24,$44,$58,$04,$14,$8E,$09,$F2,$09,$C0,$29,$0F
            .byte $FC,$07,$40,$06,$45,$F2,$08,$1C,$80,$29,$4E,$0E,$01,$07,$1D,$C0
            .byte $40,$1D,$FB,$05,$15,$11,$15,$03,$40,$1F,$08,$04,$50,$FF,$0C,$0A
            .byte $F2,$12,$09,$01,$18,$00,$B4,$00,$FF,$05,$07,$04,$FF,$1F,$06,$D8
            .byte $06,$FA,$18,$D7,$05,$15,$FF,$1F,$05,$C0,$61,$07,$02,$02,$DC,$04
            .byte $0C,$39,$04,$02,$02,$0C,$F5,$03,$2F,$04,$2D,$F1,$05,$F0,$03,$06
            .byte $80,$22,$03,$11,$3D,$04,$05,$9E,$16,$53,$06,$32,$05,$14,$16,$05
            .byte $14,$FA,$03,$3F,$F0,$06,$40,$1F,$0D,$04,$04,$09,$15,$15,$93,$1D
            .byte $39,$07,$32,$07,$14,$2A,$08,$C0,$22,$05,$16,$83,$18,$97,$07,$C0
            .byte $0B,$F8,$0C,$40,$20,$87,$09,$14,$0F,$06,$40,$05,$FA,$03,$38,$07
            .byte $F0,$06,$C0,$21,$1C,$1D,$80,$0C,$31,$F3,$04,$08,$04,$1C,$1C,$80
            .byte $29,$C0,$10,$B0,$03,$05,$04,$AB,$05,$40,$26,$8E,$05,$39,$0A,$01
            .byte $03,$1C,$71,$08,$80,$26,$14,$C0,$0F,$47,$04,$C0,$2D,$0F,$17,$06
            .byte $14,$F8,$03,$F2,$07,$11,$38,$06,$F0,$06,$80,$26,$01,$F9,$0D,$F8
            .byte $09,$C0,$3F,$16,$ED,$05,$11,$03,$40,$1F,$08,$04,$FF,$05,$0A,$20
            .byte $F9,$0D,$F8,$0C,$09,$01,$18,$00,$B4,$00,$FF,$09,$07
LF30D       .byte $04,$FF,$04,$0C,$FA,$05,$FB,$0D,$9A,$06,$DC,$0A,$F6,$1C,$05,$15
            .byte $FF,$04,$01,$FA,$05,$5D,$FB,$0D,$05,$C0,$31,$1A,$17,$1B,$C0,$35
            .byte $07,$EF,$02,$04,$0B,$C2,$03,$04,$09,$19,$08,$8C,$04,$BE,$04,$B8
            .byte $06,$80,$28,$03,$15,$3D,$03,$FD,$04,$FC,$10,$19,$19,$19,$10,$05
            .byte $B8,$09,$40,$2A,$39,$C0,$07,$C1,$03,$19,$19,$05,$B8,$09,$C0,$2B
            .byte $16,$47,$C0,$09,$16,$C0,$03,$B8,$08,$80,$29,$0D,$04,$02,$82,$0B
            .byte $80,$0B,$B8,$08,$40,$38,$80,$31,$BD,$07,$14,$01,$03,$BF,$14,$C0
            .byte $3A,$08,$0C,$04,$02,$04,$0C,$98,$09,$B8,$0A,$40,$2A,$14,$11,$3A
            .byte $03,$07,$05,$B8,$0C,$7A,$40,$29,$08,$04,$04,$0B,$C0,$03,$08,$80
            .byte $03,$88,$09,$B8,$0A,$40,$2A,$84,$03,$01,$FA,$05,$FB,$06,$B8,$07
            .byte $04,$C0,$39,$B5,$07,$40,$27,$43,$03,$FE,$03,$0A,$FA,$05,$FB,$0D
            .byte $80,$09,$01,$1C,$00,$B4,$00,$FF,$11,$07,$04,$FF,$07,$06,$E4,$12
            .byte $EE,$24,$D7,$05,$15,$FF,$07,$05,$C0,$79,$1A,$17,$1B,$7D,$80,$3C
            .byte $07,$09,$19,$08,$06,$40,$3B,$05,$E7,$10,$19,$10,$3A,$03,$C0,$3A
            .byte $19,$19,$19,$7C,$C0,$BC,$08,$0C,$02,$0C,$09,$40,$3B,$3F,$03,$35
            .byte $01,$03,$C0,$3B,$08,$02,$81,$03,$13,$40,$38,$16,$1B,$84,$03,$FD
            .byte $04,$40,$37,$08,$04,$FF,$05,$06,$03,$20,$40,$37,$B9,$09,$09,$C0
            .byte $3D,$03,$27,$00,$B5,$00,$FF,$0D,$07,$04,$FF,$0D,$0C,$F1,$05,$06
            .byte $1A,$DC,$0E,$F2,$18,$C4,$04,$0B,$15,$FF,$0D,$05,$FA,$06,$39,$A2
            .byte $0D,$B2,$19,$05,$16,$10,$CF,$07,$C0,$18,$05,$85,$03,$B2,$0C,$C0
            .byte $11,$8F,$09,$7B,$09,$01,$80,$0E,$0D,$61,$22,$03,$02,$0E,$2E,$18
            .byte $53,$04,$CF,$06,$40,$0C,$0F,$4B,$80,$0C,$16,$2C,$07,$7C,$15,$08
            .byte $22,$03,$0A,$04,$96,$0C,$1D,$04,$01,$07,$16,$31,$03,$1D,$1C,$40
            .byte $0B,$19,$6C,$08,$3C,$19,$3A,$06,$0D,$02,$57,$03,$FD,$04,$0B,$F8
            .byte $15,$15,$1C,$1C,$1E,$C0,$0E,$80,$19,$3A,$07,$AE,$12,$3A,$09,$03
            .byte $31,$03,$13,$0F,$1D,$80,$0F,$A9,$0D,$59,$03,$09,$80,$2A,$15,$C0
            .byte $10,$44,$19,$07,$73,$06,$03,$0C,$04,$0A,$A1,$04,$01,$06,$15,$16
            .byte $20,$0F,$05,$12,$0A,$08,$29,$04,$84,$19,$06,$04,$46,$07,$FA,$08
            .byte $40,$0F,$03,$0F,$12,$1D,$46,$10,$8F,$06,$BB
LF4D8       .byte $09,$B1,$06,$C0,$28,$C2,$16,$11,$C0,$10,$40,$0F,$78,$1E,$22,$04
            .byte $0B,$C0,$3B,$28,$BC,$08,$FD,$0B,$0A,$F1,$05,$09,$C0,$2A,$D6,$32
            .byte $00,$B7,$00,$FF,$11,$07,$04,$FF,$0D,$0C,$04,$06,$1B,$DC,$12,$EE
            .byte $14,$C8,$08,$0B,$15,$FF,$0D,$01,$15,$97,$05,$A6,$0B,$B4,$1B,$05
            .byte $C8,$03,$1D,$15,$15,$B6,$16,$F8,$04,$0F,$15,$7A,$04
LF525       .byte $0C,$02,$B1,$05,$61,$C0,$09,$05,$03,$B4,$0A,$C0,$16,$8F,$06,$FA
            .byte $07,$10,$5D,$EE,$05,$05,$C0,$0B,$15,$15,$0D,$32,$1A,$05,$86,$11
            .byte $47,$06,$B8,$06,$72,$04,$C0,$13,$05,$14,$7E,$19,$B5,$08,$01,$06
            .byte $04,$0A,$46,$03,$06,$BA,$04,$14,$53,$2E,$04,$13,$80,$0E,$01,$3E
            .byte $1C,$24,$0A,$0D,$02,$E1,$0A,$04,$0C,$05,$03,$80,$2C,$34,$0D,$82
            .byte $04,$03,$5C,$36,$04,$16,$40,$0E,$0D,$04,$09,$C0,$27,$02,$03,$B6
            .byte $0D,$31,$05,$02,$0B,$40,$09,$18,$16,$42,$1B,$9A,$07,$01,$06,$FA
            .byte $05,$0B,$14,$87,$04,$01,$40,$04,$37,$3E,$03,$0E,$08,$08,$04,$82
            .byte $1C,$05,$15,$1D,$1E,$D0,$06,$0A,$03,$86,$04,$0A,$02,$0E,$02,$B3
            .byte $06,$3C,$0E,$24,$0C,$03,$1C,$1C,$1C,$15,$BC,$07,$86,$06,$0E,$3A
            .byte $04,$80,$0C,$0C,$22,$7F,$08,$05,$15,$0F,$FE,$03,$1E,$C0,$2E,$22
            .byte $03,$01,$05,$06,$16,$15,$11,$86,$06,$49,$80,$06,$13,$80,$27,$B8
            .byte $0C,$0A,$07,$05,$FA,$06,$04,$80,$09,$C0,$2E,$92,$2E,$00,$00,$C0
            .byte $F5,$00,$3F,$3F,$00,$FF,$0D,$1F,$FF,$06,$0F,$E8,$0F,$0F,$2F,$E7
            .byte $03,$17,$F0,$03,$E0,$17,$D0,$0F,$02,$E0,$3E,$80,$0E,$70,$16,$8D
LF615       .byte $06,$F0,$24,$30,$10,$3F,$1B,$0B,$40,$22,$06,$3F,$23,$04,$07,$04
            .byte $00,$40,$51,$BB,$00,$FF,$55,$8D,$81,$8C,$A7,$0B,$80,$8D,$34,$F2
            .byte $0C,$8E,$0C,$8E,$81,$FF,$05,$82,$CE,$04,$F5,$03,$13,$F1,$05,$F3
            .byte $04,$D0,$0E,$80,$E7,$06,$D4,$05,$8F,$80,$79,$D8,$08,$81,$8D
LF654       .byte $81,$8F,$C3,$07,$3B,$04,$9D,$5A,$FF,$04,$8E,$FA,$03,$8D,$84,$FF
            .byte $06,$85,$C9,$08,$1B,$C3,$09,$D6,$05,$D8,$09,$8E,$87,$FF,$07,$84
            .byte $85,$8B,$8F,$82,$0A,$6F,$09,$D8,$0A,$8A,$FF,$09,$81,$8E,$00,$82
            .byte $0A,$B0,$12,$88,$07,$81,$04,$D8,$0C,$60,$10,$88,$09,$80,$03,$C1
            .byte $8E,$80,$0D,$03,$3A,$03,$3C,$07,$13,$06,$1F,$06,$82,$41,$88,$0B
            .byte $8A,$D8,$04,$DC,$0B,$D8,$09,$C5,$04,$88,$08,$85,$A3,$83,$88,$04
            .byte $8F,$DA,$0E,$CC,$0B,$10,$08,$88,$89,$DD,$8A,$8A,$DA,$1A,$00,$00
            .byte $8F,$10,$07,$8B,$00,$DC,$1B,$E5,$72,$00,$01,$20,$81,$14,$92,$02
            .byte $56,$A4,$72,$DD,$03,$DB,$87,$0B,$F3,$00,$72,$52,$60,$56,$1A,$60
            .byte $43,$46,$A2,$7B,$0C,$45,$70,$88,$A2,$11,$41,$E1,$69,$A2,$55,$31
            .byte $A8,$AA,$D0,$55,$52,$A2,$A8,$C0,$61,$48,$08,$A8,$A8,$0C,$D4,$10
            .byte $00,$22,$9C,$A1,$06,$51,$22,$C2,$C6,$5D,$49,$0B,$CB,$03,$00,$39
            .byte $13,$A1,$13,$84,$A2,$73,$85,$19,$49,$0B,$8E,$53,$7B,$61,$AE,$54
            .byte $15,$E1,$A8,$BC,$F5,$05,$5E,$8A,$AA,$14,$55,$A0,$AC,$7A,$15,$8E
            .byte $A8,$30,$D2,$46,$6D,$80,$CC,$34,$19,$9E,$08,$00,$C4,$EE,$27,$50
            .byte $D4,$FD,$80,$98,$E1,$13,$EC,$86,$73,$C5,$4E,$52,$C0,$F6,$11,$14
            .byte $42,$22,$E8,$14,$73,$20,$B3,$50,$17,$4E,$02,$81,$C0,$50,$12,$0A
            .byte $86,$51,$51,$60,$B3,$BC,$04,$00,$8B,$3E,$64,$B5,$44,$AA,$81,$91
            .byte $41,$92,$65,$4A,$61,$85,$4C,$1A,$09,$8D,$50,$32,$8A,$A6,$6C,$14
            .byte $56,$0A,$8A,$25,$11,$74,$22,$FB,$84,$DE,$69,$0A,$27,$01,$52,$A0
            .byte $2B,$B4,$55,$73,$20,$CE,$F0,$10,$00,$96,$CF,$05,$D8,$14,$B3,$A8
            .byte $9D,$61,$53,$8E,$A0,$29,$94,$45,$D0,$23,$53,$0D,$61,$E0,$2D,$C2
            .byte $45,$8B,$82,$B1,$25,$16,$96,$A2,$95,$C5,$52,$82,$0A,$87,$21,$5A
            .byte $E2,$AB,$39,$45,$65,$09,$C5,$10,$D8,$18,$2B,$02,$D0,$63,$74,$9C
            .byte $4D,$61,$9D,$E0,$20,$C0,$E9,$C4,$A8,$44,$8A,$08,$98,$44,$13,$16
            .byte $28,$65,$41,$4D,$B0,$1B,$AF,$51,$76,$00,$A8,$B9,$05,$1B,$82,$AF
            .byte $70,$15,$DC,$A2,$BA,$90,$64,$78,$08,$00,$57,$92,$A5,$12,$68,$2A
            .byte $63,$85,$4C
LF807       .byte $5A,$81,$B5,$01,$6E,$02,$AD,$AC,$05,$E7,$48,$BB,$10,$94,$6C,$12
            .byte $AB,$D0,$57,$80,$8A,$EB,$41,$56,$E0,$31,$3C,$04,$00,$04,$1F,$41
            .byte $53,$D4,$A0,$F3,$C0,$51,$58,$C2,$75,$01,$4C,$D0,$0B,$38,$55,$65
            .byte $A0,$00,$5A,$10,$42,$12,$A8,$8A,$57,$04,$4A,$42,$09,$A5,$01,$3D
            .byte $22,$6F,$55,$45,$EE,$02,$A7,$C0,$54,$EA,$22,$B5,$45,$55,$49,$2A
            .byte $A2,$11,$59,$4A,$70,$AC,$46,$11,$20,$C1,$23,$18,$6A,$07,$34,$C1
            .byte $66,$5A,$08,$00,$49,$10,$60,$62,$F1,$02,$40,$B0,$0A,$D0,$89,$63
            .byte $41,$1C,$82,$0D,$EC,$15,$B9,$21,$A7,$A1,$54,$C6,$82,$B1,$04,$56
            .byte $41,$2C,$7B,$05,$8C,$8A,$32,$91,$06,$6D,$02,$4B,$A1,$09,$86,$02
            .byte $00,$38,$18,$25,$13,$E8,$0A,$70,$D4,$4F,$52,$19,$9C,$15,$39,$04
            .byte $AD,$74,$55,$A1,$A0,$BD,$D1,$57,$94,$82,$97,$44,$52,$12,$0A,$DD
            .byte $15,$59,$48,$31,$74,$16,$21,$80,$80,$64,$35,$90,$26,$44,$82,$CD
            .byte $81,$13,$A0,$2A,$62,$11,$5B,$22,$8A,$E0,$0D,$64,$20,$80,$66,$97
            .byte $41,$13,$0C,$42,$6A,$04,$CF,$A9,$0B,$D7,$05,$78,$42,$2F,$3C,$15
            .byte $0B,$09,$A4,$E1,$15,$5E,$2A,$AA,$50,$62,$B8,$0C,$42,$41,$25,$B0
            .byte $09,$C0,$88,$18,$2A,$57,$45,$80,$80,$58,$16,$61,$16,$EE,$08,$DC
            .byte $94,$1A,$38,$0B,$AE,$05,$74,$6A,$2E,$E8,$15,$D9,$A8,$26,$71,$15
            .byte $E0,$0A,$A8,$A0,$55,$B0,$8A,$E6,$55,$5C,$6A,$31,$4C,$16,$21,$A0
            .byte $8E,$D5,$0A,$DA,$64,$0C,$16,$02,$00
LF930       .byte $01
LF931       .byte $01
LF932       .byte $5C
LF933       .byte $90
LF934       .byte $0E
LF935       jsr LF9D2
            beq LF958
            bit ICBLLZ
            bvc LF942
            ora L00A1
            bne LF946
LF942       eor #$FF
            and L00A1
LF946       sta L00A1
            jsr LF9B4
            bit L00A1
            bpl LF955
            lda #$40
            sta L00F0
            sta APPMHI+1
LF955       jsr L0D07
LF958       jsr LF9D5
            asl
            bcc LF964
            lda #$00
            sta OLDCOL+1
            beq LF984
LF964       bpl LF988
            ldx #$0F
LF968       lda L00E0,X
            and #$E0
            bne LF975
            lda #$FF
            sta L00E0,X
            jsr L1208
LF975       dex
            bne LF968
            inx
            stx OLDCOL
            lda #$01
            sta LE644
            lda OLDCOL+1
            bne LF986
LF984       inc ABUFPT+3
LF986       sec
            rts
LF988       lda TEMPCHR
            and #$03
            bne LF9B2
            lda ICBLHZ
            and #$30
            beq LF9B2
            ldx L00E0
            and #$20
            bne LF9A1
            dex
            bpl LF9A8
            ldx #$17
            bne LF9A8
LF9A1       inx
            cpx #$18
            bcc LF9A8
            ldx #$00
LF9A8       stx L00E0
            stx CDTMV5
            stx DOSINI+1
            jsr L38F7
LF9B2       clc
            rts
LF9B4       ldx #$01
            ldy #$00
            lda L00A1
            sta ICBLHZ
LF9BC       lda #$30
            asl ICBLHZ
            bcc LF9C3
            txa
LF9C3       sta L5052,Y
            ora #$80
            sta L507A,Y
            iny
            inx
            cpx #$09
            bcc LF9BC
            rts
LF9D2       ldx #$07
            .byte $0C
LF9D5       ldx #$0F
            lda #$01
            sta ICBLHZ
LF9DB       lda LF9F2,X
            jsr L0B50
            and LFA02,X
            cmp #$01
            rol ICBLHZ
            dex
            bcc LF9DB
            lda ICBLHZ
            eor #$FF
            sta ICBLHZ
            rts
LF9F2       .byte $F7,$F7,$FB,$FB,$FD,$FD,$7F,$7F,$FF,$FF,$FF,$FF,$DF,$DF,$FB,$FD
LFA02       .byte $08,$01,$08,$01,$08,$01,$08,$01
LFA0A       .byte $00,$00,$00,$00
LFA0E       .byte $08,$01,$04,$20
LFA12       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00
LFB06       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00
LFB1B       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00
LFD12       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00
LFD1B       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00
LFE12       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
LFF00       .byte $00,$00,$00
LFF03       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00
LFF18       .byte $00,$00,$00,$00,$00
LFF1D       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00
LFF30       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
LFF90       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00
LFFBA       .byte $00,$00,$00
LFFBD       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
LFFD8       .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00
;
         

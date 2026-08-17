;
        icl 'unit001.inc'
;
; Start of code
;

            org $2000
;
L2000       jsr L2128
            ldx #$30
            jsr L210D
            ldy #$0F
            ldx #$3B
            jsr L20EE
            bmi L2000
L2011       lda #$00
            sta INVFLG
            lda #$40
            sta SHFLOC
            lda #$FF
            sta CH
L2020       ldy #$15
            ldx #$39
            jsr L20EE
            bmi L2011
            cmp #$20
            bne L2047
            lda DAUX1
            cmp #$71
            bcs L2000
            ldy L00B1
            lda L2600,Y
            beq L2000
            jsr L2115
            lda #$00
            sta L00B0
            jsr L220D
L2045       bpl L2011
L2047       cmp #$44
            beq L205E
            ldx TRAMSZ
            beq L2053
            cmp #$42
            beq L205E
L2053       sec
            sbc #$30
            beq L2011
            cmp L00B0
            beq L205E
            bcs L2011
L205E       sta L00B1
            ldx #$30
            jsr L210D
            ldx #$40
            jsr L210D
            lda L00B1
            cmp #$42
            bne L2071
            rts
L2071       cmp #$44
            bne L2078
L2075       jmp (DOSVEC)
L2078       lda L00B1
            asl
            tax
            lda L00BE,X
            sta DAUX1
            lda L00BF,X
            sta DAUX2
            jsr DSKINV
            bmi L2075
            ldx #$24
            ldy #$1D
            lda L2600
            ora L2601
            beq L20A7
            ldx L00B1
            dec L00B7,X
L209B       bne L20A3
            ldx #$23
            ldy #$23
            bne L20A7
L20A3       ldx #$25
            ldy #$07
L20A7       stx L20AF+2
            sty L20B2+2
            ldx #$00
L20AF       lda L2500,X
L20B2       sta L0700,X
            inx
            bne L20AF
            ldx #$0F
L20BA       lda DDEVIC,X
            sta L00C0,X
            lda #$00
            sta L00B0,X
            dex
            bpl L20BA
            cpy #$07
            beq L20D0
            sty L20CD+2
L20CD       jmp L2300
L20D0       lda #$01
            sta BOOT
            lda #$00
            sta COLDST
            lda #$00
            sta DOSINI
            lda #$07
            sta DOSINI+1
            jmp WARMSV
L20E4       stx L228F
            sty L2290
            ldy #$1B
            ldx #$49
L20EE       jsr L20F7
            jsr CIOV
            rts
L20F5       dex
            dey
L20F7       lda L2277,Y
            sta IOCB0,X
            txa
            and #$0F
            cmp #$08
            bne L2106
            dex
            dex
L2106       cmp #$02
            bne L20F5
            dex
            dex
            rts
L210D       lda #$0C
            sta IOCB0+ICCOM,X
            jmp CIOV
L2115       lda #$00
            sta ROWCRS
            ldx #$32
            ldy #$22
            jsr L20E4
            ldx #$43
            ldy #$22
            jsr L20E4
            rts
L2128       ldx #$40
            jsr L210D
            ldy #$07
            ldx #$4B
            jsr L20EE
            bpl L2137
            rts
L2137       lda #$00
            sta COLOR4
            lda #$B6
            sta COLOR0
            lda #$0A
            sta COLOR1
            jsr L2115
            lda #$00
            ldx #$1F
L214D       sta L00B0,X
            dex
            bpl L214D
            ldx #$0B
L2154       lda L2293,X
            sta DDEVIC,X
            dex
            bpl L2154
L215D       jsr DSKINV
            bpl L2170
            lda #$0A
            sta ROWCRS
            ldx #$56
            ldy #$22
            jsr L20E4
L216D       jmp L2226
L2170       lda #$00
            sta L00B1
L2174       ldy L00B1
            lda L2600,Y
            beq L216D
            and #$7F
            beq L21A0
            lda L260D,Y
            ldx #$00
            cmp #$42
            beq L219E
            cmp #$20
            beq L219E
            cmp #$4F
            beq L21A3
            cmp #$52
            beq L21A4
            cmp #$43
            beq L21A4
            cmp #$45
            beq L21A4
            bne L2207
L219E       lda TRAMSZ
L21A0       beq L2207
            inx
L21A3       inx
L21A4       txa
            inc L00B0
            ldx L00B0
            sta L00B7,X
            pha
            lda #$20
            ldx #$20
L21B0       sta L0400,X
            dex
            bpl L21B0
            lda L00B0
            ora #$10
            sta L0402
            lda L00B0
            asl
            tax
            lda L2603,Y
            sta L00BE,X
            lda L2604,Y
            sta L00BF,X
            lda #$05
            sta L21D2+1
            ldx #$04
L21D2       lda L2605,Y
            cmp #$20
            beq L21E4
            sta L0400,X
            inc L21D2+1
            inx
            cpx #$0C
            bcc L21D2
L21E4       pla
            beq L21FB
            ldy #$00
            cmp #$01
            beq L21EF
            ldy #$07
L21EF       lda L2263,Y
            bmi L21FB
            sta L0400,X
            iny
            inx
            bpl L21EF
L21FB       lda #$9B
            sta L0400,X
            ldx #$00
            ldy #$04
            jsr L20E4
L2207       lda L00B0
            cmp #$08
            bcs L2226
L220D       lda L00B1
            clc
            adc #$10
            sta L00B1
            bmi L2219
            jmp L2174
L2219       inc DAUX1
            lda DAUX1
            cmp #$71
            bcs L2226
            jmp L215D
L2226       lda #$0B
            sta ROWCRS
            ldx #$44
            ldy #$22
            jsr L20E4
            rts
            adc L2020,X
            jsr L4147
            eor L2045
            lsr FMSZPG+6
            jmp L5345
            jsr LE1B1
            .byte $9B
            jsr L4320
            pha
            .byte $4F,$4F,$53
            eor ICHIDZ
            eor (ICHIDZ,X)
            .byte $23
            jsr L209B
            jsr L2020
            jsr L4920
            .byte $2F,$4F
            jsr L5245
            .byte $52,$4F,$52
            and (L009B,X)
L2263       jsr L4F28
            .byte $42
            lsr
            and #$9B
            jsr L4228
            eor (RMARGN,X)
            and #$9B
            .byte $4B,$3A,$9B,$53,$3A,$9B
L2277       .byte $03,$00,$74,$22,$00,$00
            php
            .byte $02,$03,$00
            adc (ICCOMZ),Y
            .byte $00,$00,$04,$00,$07,$00,$00,$00,$00,$00
            ora #$00
L228F       .byte $00
L2290       .byte $04,$80,$00
L2293       and (NGFLAG),Y
            .byte $52,$00,$00
            rol L0000
            .byte $00,$00,$00
            adc #$01
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00
L2300       lda L2600
            sta L2329+1
            lda L2601
            sta L2322+1
            lda L267F
            and #$7F
            sta L2359+1
            ldx #$FF
L2316       jsr L2358
            sta L233C+1
            jsr L2358
            sta L233C+2
L2322       cmp #$FF
            bne L232D
            lda L233C+1
L2329       cmp #$FF
            beq L2316
L232D       jsr L2358
            sta L2349+1
            jsr L2358
            sta L2342+1
L2339       jsr L2358
L233C       sta L0400
            lda L233C+2
L2342       cmp #$00
            bne L234D
            lda L233C+1
L2349       cmp #$00
            beq L2316
L234D       inc L233C+1
            bne L2339
            inc L233C+2
            jmp L2339
L2358       inx
L2359       cpx #$7D
            bcs L2361
            lda L2600,X
            rts
L2361       lda L267D
            and #$03
            sta DAUX2
            lda L267E
            sta DAUX1
            ora DAUX2
            beq L2385
            jsr DSKINV
            bmi L2385
            lda L267F
            and #$7F
            sta L2359+1
            ldx #$FF
            bne L2358
L2385       pla
            pla
            jmp L2000
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00
            ldx #$7F
            lda #$00
L2404       sta LOMEM,X
            dex
            bpl L2404
            lda DAUX1
            sta L1D2A
            lda DAUX2
            sta L1D31
            lda #$69
            sta DAUX1
            lda #$01
            sta DAUX2
L241F       jsr DSKINV
            bmi L2445
            ldy #$00
L2426       lda L2603,Y
            cmp #$00
            bne L2434
            lda L2604,Y
            cmp #$00
            beq L2448
L2434       tya
            clc
            adc #$10
            tay
            bpl L2426
            inc DAUX1
            lda DAUX1
            cmp #$71
            bcc L241F
L2445       jmp L2000
L2448       ldx #$00
L244A       lda L2605,Y
            cmp #$20
            beq L2455
            sta L1DD3,X
            inx
L2455       iny
            tya
            and #$0F
            cmp #$08
            bne L2465
            lda #$2E
            sta L1DD3,X
            inx
            bne L244A
L2465       cmp #$0B
            bcc L244A
            lda #$9B
            sta L1DD3,X
            ldx IOCB0
            lda HATABS+1,X
            sta L00D4
            sta L1DB0
            lda HATABS+2,X
            sta L00D5
            sta L1DB5
            ldy #$0F
L2483       lda (L00D4),Y
            sta L1DBD,Y
            dey
            bpl L2483
            lda #$9F
            sta L1DC1
            lda #$1D
            sta L1DC2
            lda #$BD
            sta HATABS+1,X
            lda #$1D
            sta HATABS+2,X
            rts
            ldx #$FF
            inx
            stx L1DA1
            lda L1DCD,X
            bpl L24BA
            pha
            ldx IOCB0
            lda #$00
            sta HATABS+1,X
            lda #$00
            sta HATABS+2,X
            pla
L24BA       ldy #$01
            rts
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $52
            eor DRKMSK,X
            .byte $22,$44,$3A,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00
L2500       ldx #$7F
L2502       lda L2600,X
            sta L0800,X
            lda #$00
            sta LOMEM,X
            dex
            bpl L2502
            sta WARMST
            lda L0800
            sta L076C
            lda L0801
            sta L0765
            lda L0802
            sta L07E7
            lda L0803
            sta L07E8
            lda #$B1
            sta RUNAD
            lda #$07
            sta RUNAD+1
L2533       lda #$00
            tax
L2536       sta L0400,X
            inx
            bne L2536
L253C       inc L0738
            lda L0738
            cmp #$07
            beq L253C
            cmp #$08
            beq L253C
            cmp MEMTOP+1
            bcc L2533
            jsr L07D5
            jmp L075B
L2555       jsr L07E9
L2558       jsr L07A4
            sta L0789
            jsr L07A4
            sta L078A
            cmp #$FF
            bne L256F
            lda L0789
            cmp #$FF
            beq L2558
L256F       jsr L07A4
            sta L0796
            jsr L07A4
            sta L078F
            lda #$B1
            sta INITAD
            lda #$07
            sta INITAD+1
L2585       jsr L07A4
            sta L0400
            lda L078A
            cmp #$00
            bne L2599
            lda L0789
            cmp #$00
            beq L2555
L2599       inc L0789
            bne L2585
            inc L078A
            jmp L0785
            ldx #$FF
L25A6       inx
            stx L07A5
            cpx #$7D
            bcs L25B2
            lda L0800,X
            rts
L25B2       lda L087D
            and #$03
            sta DAUX2
            lda L087E
            sta DAUX1
            ora DAUX2
            beq L25E1
            ldx #$05
L25C7       lda L07EF,X
            sta DDEVIC,X
            dex
            bpl L25C7
            jsr DSKINV
            bmi L25E1
            lda L087F
            and #$7F
            sta L07AB
            ldx #$FF
            bne L25A6
L25E1       pla
            pla
            jsr L07EC
            jmp L07B1
            jmp (INITAD)
            jmp (RUNAD)
            and (NGFLAG),Y
            .byte $52,$00,$00
            php
            .byte $00,$00,$00,$00,$00,$00,$00,$00,$00,$00,$00

            org $02E2
;
            .word L2000

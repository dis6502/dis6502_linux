; 
            icl 'C:\jac\system\Windows\Programming\Tools\dis6502\tst\suite\disassembly\unit001\out\unit001.inc'
;
; Start of code
;
            org 8192
;
L2000       jsr L2128
            ldx #48
            jsr L210D
            ldy #15
            ldx #59
            jsr L20EE
            bmi L2000
L2011       lda #0
            sta INVFLG
            lda #64
            sta SHFLOC
            lda #255
            sta CH
L2020       ldy #21
            ldx #57
            jsr L20EE
            bmi L2011
            cmp #32
            bne L2047
            lda DAUX1
            cmp #113
            bcs L2000
            ldy L00B1
            lda L2600, Y
            beq L2000
            jsr L2115
            lda #0
            sta L00B0
            jsr L220D
L2045       bpl L2011
L2047       cmp #68
            beq L205E
            ldx ICPTL
            beq L2053
            cmp #66
            beq L205E
L2053       sec
            sbc #48
            beq L2011
            cmp L00B0
            beq L205E
            bcs L2011
L205E       sta L00B1
            ldx #48
            jsr L210D
            ldx #64
            jsr L210D
            lda L00B1
            cmp #66
            bne L2071
            rts
L2071       cmp #68
            bne L2078
L2075       jmp (ICAX1)
L2078       lda L00B1
            asl
            tax
            lda L00BE,X
            sta DAUX1
            lda L00BF,X
            sta DAUX2
            jsr DSKINV
            bmi L2075
            ldx #36
            ldy #29
            lda L2600
            ora L2601
            beq L20A7
            ldx L00B1
            dec L00B7,X
L209B       bne L20A3
            ldx #35
            ldy #35
            bne L20A7
L20A3       ldx #37
            ldy #7
L20A7       stx L20AF+2
            sty L20B2+2
            ldx #0
L20AF       lda L2500,X
L20B2       sta L0700,X
            inx
            bne L20AF
            ldx #15
L20BA       lda DDEVIC,X
            sta L00C0,X
            lda #0
            sta L00B0,X
            dex
            bpl L20BA
            cpy #7
            beq L20D0
            sty L20CD+2
L20CD       jmp L2300
L20D0       lda #1
            sta ICBLH
            lda #0
            sta COLDST
            lda #0
            sta ICAX3
            lda #7
            sta ICAX4
            jmp WARMSV
L20E4       stx L228F
            sty L2290
            ldy #27
            ldx #73
L20EE       jsr L20F7
            jsr CIOV
            rts
L20F5       dex
            dey
L20F7       lda L2277, Y
            sta IOCB0,X
            txa
            and #15
            cmp #8
            bne L2106
            dex
            dex
L2106       cmp #2
            bne L20F5
            dex
            dex
            rts
L210D       lda #12
            sta IOCB0+ICCOM,X
            jmp CIOV
L2115       lda #0
            sta ROWCRS
            ldx #50
            ldy #34
            jsr L20E4
            ldx #67
            ldy #34
            jsr L20E4
            rts
L2128       ldx #64
            jsr L210D
            ldy #7
            ldx #75
            jsr L20EE
            bpl L2137
            rts
L2137       lda #0
            sta COLOR4
            lda #182
            sta COLOR0
            lda #10
            sta COLOR1
            jsr L2115
            lda #0
            ldx #31
L214D       sta L00B0,X
            dex
            bpl L214D
            ldx #11
L2154       lda L2293,X
            sta DDEVIC,X
            dex
            bpl L2154
L215D       jsr DSKINV
            bpl L2170
            lda #10
            sta ROWCRS
            ldx #86
            ldy #34
            jsr L20E4
L216D       jmp L2226
L2170       lda #0
            sta L00B1
L2174       ldy L00B1
            lda L2600, Y
            beq L216D
            and #127
            beq L21A0
            lda L260D, Y
            ldx #0
            cmp #66
            beq L219E
            cmp #32
            beq L219E
            cmp #79
            beq L21A3
            cmp #82
            beq L21A4
            cmp #67
            beq L21A4
            cmp #69
            beq L21A4
            bne L2207
L219E       lda ICPTL
L21A0       beq L2207
            inx
L21A3       inx
L21A4       txa
            inc L00B0
            ldx L00B0
            sta L00B7,X
            pha
            lda #32
            ldx #32
L21B0       sta L0400,X
            dex
            bpl L21B0
            lda L00B0
            ora #16
            sta L0402
            lda L00B0
            asl
            tax
            lda L2603, Y
            sta L00BE,X
            lda L2604, Y
            sta L00BF,X
            lda #5
            sta L21D2+1
            ldx #4
L21D2       lda L2605, Y
            cmp #32
            beq L21E4
            sta L0400,X
            inc L21D2+1
            inx
            cpx #12
            bcc L21D2
L21E4       pla
            beq L21FB
            ldy #0
            cmp #1
            beq L21EF
            ldy #7
L21EF       lda L2263, Y
            bmi L21FB
            sta L0400,X
            iny
            inx
            bpl L21EF
L21FB       lda #155
            sta L0400,X
            ldx #0
            ldy #4
            jsr L20E4
L2207       lda L00B0
            cmp #8
            bcs L2226
L220D       lda L00B1
            clc
            adc #16
            sta L00B1
            bmi L2219
            jmp L2174
L2219       inc DAUX1
            lda DAUX1
            cmp #113
            bcs L2226
            jmp L215D
L2226       lda #11
            sta ROWCRS
            ldx #68
            ldy #34
            jsr L20E4
            rts
            adc L2020,X
            jsr L4147
            eor L2045
            lsr FMSZPG+6
            jmp L5345
            jsr LE1B1
            .byte 155
            jsr L4320
            pha
            .byte 79,79,83
            eor AVSCR
            eor (AVSCR,X)
            .byte 35
            jsr L209B
            jsr L2020
            jsr L4920
            .byte 47,79
            jsr L5245
            .byte 82,79,82
            and (L009B,X)
L2263       jsr L4F28
            .byte 66
            lsr
            and #155
            jsr L4228
            eor (RMARGN,X)
            and #155
            .byte 75,58,155,83,58,155
L2277       .byte 3,0,116,34,0,0
            php
            .byte 2,3,0
            adc (ICCOMZ),Y
            .byte 0,0,4,0,7,0,0,0,0,0
            ora #0
L228F       .byte 0
L2290       .byte 4,128,0
L2293       and (ICDNO),Y
            .byte 82,0,0
            rol L0000
            .byte 0,0,0
            adc #1
            .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
            .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
            .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
            .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
            .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
            .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
            .byte 0
L2300       lda L2600
            sta L2329+1
            lda L2601
            sta L2322+1
            lda L267F
            and #127
            sta L2359+1
            ldx #255
L2316       jsr L2358
            sta L233C+1
            jsr L2358
            sta L233C+2
L2322       cmp #255
            bne L232D
            lda L233C+1
L2329       cmp #255
            beq L2316
L232D       jsr L2358
            sta L2349+1
            jsr L2358
            sta L2342+1
L2339       jsr L2358
L233C       sta L0400
            lda L233C+2
L2342       cmp #0
            bne L234D
            lda L233C+1
L2349       cmp #0
            beq L2316
L234D       inc L233C+1
            bne L2339
            inc L233C+2
            jmp L2339
L2358       inx
L2359       cpx #125
            bcs L2361
            lda L2600,X
            rts
L2361       lda L267D
            and #3
            sta DAUX2
            lda L267E
            sta DAUX1
            ora DAUX2
            beq L2385
            jsr DSKINV
            bmi L2385
            lda L267F
            and #127
            sta L2359+1
            ldx #255
            bne L2358
L2385       pla
            pla
            jmp L2000
            .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
            .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
            .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
            .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
            .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
            .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
            .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
            .byte 0,0,0,0,0,0
            ldx #127
            lda #0
L2404       sta ADLI,X
            dex
            bpl L2404
            lda DAUX1
            sta L1D2A
            lda DAUX2
            sta L1D31
            lda #105
            sta DAUX1
            lda #1
            sta DAUX2
L241F       jsr DSKINV
            bmi L2445
            ldy #0
L2426       lda L2603, Y
            cmp #0
            bne L2434
            lda L2604, Y
            cmp #0
            beq L2448
L2434       tya
            clc
            adc #16
            tay
            bpl L2426
            inc DAUX1
            lda DAUX1
            cmp #113
            bcc L241F
L2445       jmp L2000
L2448       ldx #0
L244A       lda L2605, Y
            cmp #32
            beq L2455
            sta L1DD3,X
            inx
L2455       iny
            tya
            and #15
            cmp #8
            bne L2465
            lda #46
            sta L1DD3,X
            inx
            bne L244A
L2465       cmp #11
            bcc L244A
            lda #155
            sta L1DD3,X
            ldx IOCB0
            lda HATABS+1,X
            sta L00D4
            sta L1DB0
            lda HATABS+2,X
            sta L00D5
            sta L1DB5
            ldy #15
L2483       lda (L00D4),Y
            sta L1DBD, Y
            dey
            bpl L2483
            lda #159
            sta L1DC1
            lda #29
            sta L1DC2
            lda #189
            sta HATABS+1,X
            lda #29
            sta HATABS+2,X
            rts
            ldx #255
            inx
            stx L1DA1
            lda L1DCD,X
            bpl L24BA
            pha
            ldx IOCB0
            lda #0
            sta HATABS+1,X
            lda #0
            sta HATABS+2,X
            pla
L24BA       ldy #1
            rts
            .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
            .byte 82
            eor DRKMSK,X
            .byte 34,68,58,0,0,0,0,0,0,0,0,0,0,0,0,0
            .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
            .byte 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
L2500       ldx #127
L2502       lda L2600,X
            sta L0800,X
            lda #0
            sta ADLI,X
            dex
            bpl L2502
            sta ICBLL
            lda L0800
            sta L076C
            lda L0801
            sta L0765
            lda L0802
            sta L07E7
            lda L0803
            sta L07E8
            lda #177
            sta RUNAD
            lda #7
            sta RUNAD+1
L2533       lda #0
            tax
L2536       sta L0400,X
            inx
            bne L2536
L253C       inc L0738
            lda L0738
            cmp #7
            beq L253C
            cmp #8
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
            cmp #255
            bne L256F
            lda L0789
            cmp #255
            beq L2558
L256F       jsr L07A4
            sta L0796
            jsr L07A4
            sta L078F
            lda #177
            sta INITAD
            lda #7
            sta INITAD+1
L2585       jsr L07A4
            sta L0400
            lda L078A
            cmp #0
            bne L2599
            lda L0789
            cmp #0
            beq L2555
L2599       inc L0789
            bne L2585
            inc L078A
            jmp L0785
            ldx #255
L25A6       inx
            stx L07A5
            cpx #125
            bcs L25B2
            lda L0800,X
            rts
L25B2       lda L087D
            and #3
            sta DAUX2
            lda L087E
            sta DAUX1
            ora DAUX2
            beq L25E1
            ldx #5
L25C7       lda L07EF,X
            sta DDEVIC,X
            dex
            bpl L25C7
            jsr DSKINV
            bmi L25E1
            lda L087F
            and #127
            sta L07AB
            ldx #255
            bne L25A6
L25E1       pla
            pla
            jsr L07EC
            jmp L07B1
            jmp (INITAD)
            jmp (RUNAD)
            and (ICDNO),Y
            .byte 82,0,0
            php
            .byte 0,0,0,0,0,0,0,0,0,0,0
;
            org 738
;
            .word L2000
;
         

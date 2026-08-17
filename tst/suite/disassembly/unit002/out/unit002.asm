;
        icl 'unit002.inc'
;
; Start of code
;

            org $2000
;
L2000       lda #$41
            lda #$30
            lda L1234
            sta L5678
            jsr L2016
L200D       lda VCOUNT
            sta COLBK
            jmp L200D
L2016       ldx #$00
            sta L202B,X
            sta L2035,X
            sta L203F,X
            sta L2044,X
            sta L2049+1,X
            sta L204F,X
            rts
L202B       ora (CASINI,X)
            .byte $03,$04
            ora TRAMSZ
            .byte $07
            php
            ora #$0A
L2035       sta (L0082,X)
            .byte $83
            sty L0085
            stx L0087
            dey
            .byte $89
            txa
L203F       eor (RMARGN,X)
            .byte $43
            eor #$49
L2044       .byte $33,$23,$32
            and ICBAHZ
L2049       rol LD3C1
            .byte $C3
            cmp #$C9
L204F       .byte $B3,$A3,$B2
            lda L00A5
            .byte $AE

            org $3000
;
L3000       lda L1234
            sta L5678
            rts

            org $02E2
;
            .word L3000

            org $4000
;
L4000       lda L1234
            sta L5678
            rts

            org $02E2
;
            .word L4000

            org $02E0
;
            .word L2000

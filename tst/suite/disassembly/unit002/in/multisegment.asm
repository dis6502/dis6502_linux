;	Multi-Segment Test Field

	org $2000
	
	lda #'A'
	lda #'0'
	lda $1234
	sta $5678
	jsr read
loop	lda $d40b
	sta $d01a
	jmp loop
	
read	ldx #0
	sta decimal,x
	sta hexinv,x
	sta ascii,x
	sta screen,x
	sta iascii,x
	sta iscreen,x
	rts
	
decimal	.byte 1,2,3,4,5,6,7,8,9,10
hexinv	.byte $81,$82,$83,$84,$85,$86,$87,$88,$89,$8A
ascii	.by 'ASCII'
screen	.sb 'SCREEN'
iascii	.by +$80 'ASCII'
iscreen	.sb +$80 'SCREEN'

	org $3000
	
	lda $1234
	sta $5678
	rts
	
	ini $3000
	
	org $4000

	lda $1234
	sta $5678
	rts

	ini $4000

	run $2000
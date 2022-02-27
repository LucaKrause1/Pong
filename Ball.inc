;* Datei: Ball.inc
; -----------------------------------------------------------------------------
;   Realisiert den Ball und seine Bewegung
; -----------------------------------------------------------------------------

     
		switch VarSection
;Variablen

moveX		ds.b 	1						;Ballaenderung in X-Richtung
moveY		ds.b	1						;Ballaenderung in Y-Richtung
BallX		ds.b 	1							;Ballposition in X-Richtung
BallY		ds.b	1							;Ballposition in Y-Richtung
Geschwindigkeit ds.w	1				;Geschwindigkeit des Balls, 
												;je kleiner, desto schneller

		switch RomSection
;Konstanten

StartGeschwindigkeit dc.w 2000
startX 				dc.b	64
starty 				dc.b	32
startXMove		dc.b	1
startYMove		dc.b	1
lastX					dc.b	127
lastY					dc.b	63




; -----------------------------------------------------------------------------
;	Setzt den Ball auf die Startposition und Bewegung jedes Spiels
; -----------------------------------------------------------------------------
initBall
			ldaa startX
			ldab starty
			staa BallX
			stab BallY
			
			ldaa startXMove
			ldab startYMove
			staa moveX
			stab moveY
			
			ldaa StartGeschwindigkeit
			staa Geschwindigkeit
			
			rts 									;Ende
			
; -----------------------------------------------------------------------------
;	Initialisiert den Interrupt-Timer fuer den Ball
; -----------------------------------------------------------------------------
BallTimer
			ldd TOC2
			addd StartGeschwindigkeit		;Timer einstellen
			std TOC2
			
			bset TMSK1, #%01000000  
			rts										;Ende
			
; -----------------------------------------------------------------------------
;	Bewegt den Ball und sorgt fuer ein Abprallen an den Y-Grenzen
; -----------------------------------------------------------------------------
moveBall
		;TODO: Abprallen und vorbei an Ball!!!
			ldaa BallX
			adda moveX
			staa BallX
			
			;ueberpruefen, ob beim naechsten Move die X-Grenzen/Schlaeger uebertreten werden
			cmpa bat1X
			bne moveBallIndexOkay1
			
			jsr initBall
			bra moveBallIndexOkay4
			
moveBallIndexOkay1
			cmpa bat2X
			bne moveBallIndexOkay2
			
			jsr initBall
			bra moveBallIndexOkay4
			
moveBallIndexOkay2

			
			
;ueberpruefen, ob beim naechsten Move die Y-Grenzen uebertreten werden
			ldaa BallY
			adda moveY
			staa BallY
			
			
			cmpa lastY
			blt moveBallIndexOkay3
			
			ldaa #-1
			staa moveY
			
moveBallIndexOkay3
			cmpa #0
			blt moveBallIndexOkay4
			
			ldaa #1
			staa moveY
			
moveBallIndexOkay4
			
			rts										;Ende

; -----------------------------------------------------------------------------
;	Interrupt-Service-Routine
;	Timer 2
;	aktualisiert den Ball
; -----------------------------------------------------------------------------
isrOC2
			ldaa BallX
			ldab BallY
			jsr deleteBall
			
			jsr loadBall
														;Timer einstellen
			ldd TOC2								;Frequenz einstellen
			addd Geschwindigkeit				;diesen Wert eingeben -> (2000) / 2.000.000 = 1000 Hz
														;-> nach 1000 us wieder interrupten
			std TOC2
			
			;Flag zuruecksetzen
			bset TFLG1,  #%10000000	
			
			rti											;Ende Interrupt



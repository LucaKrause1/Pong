;* Datei: Ball.inc
; -----------------------------------------------------------------------------
;   Realisiert den Ball und seine Bewegung
; -----------------------------------------------------------------------------

     
		switch VarSection
;Variablen

ballPos ds.b 2									;Ball-Positionen: [0] = X, [1] = Y
ballChange ds.b 2								;Positionsaenderung nach dem naechsten Tick
Geschwindigkeit ds.w 1						;Geschwindigkeit des Balls, 
														;je kleiner, desto schneller

		switch RomSection
;Konstanten

StartGeschwindigkeit dc.w 2000

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
;	Interrupt-Service-Routine
;	Timer 2
;	aktualisiert den Ball
; -----------------------------------------------------------------------------
isrOC2
			ldx #ballPos
			ldaa 0,x
			ldab 1,x
			jsr deleteBall
			
			jsr loadBall
														;Timer einstellen
			ldd TOC2								;Frequenz einstellen
			addd #2000							;diesen Wert eingeben -> (2000) / 2.000.000 = 1000 Hz
														;-> nach 1000 us wieder interrupten
			std TOC2
			
			;Flag zuruecksetzen
			bset TFLG1,  #%10000000	
			
			rti											;Ende Interrupt



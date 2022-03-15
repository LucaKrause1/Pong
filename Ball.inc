;* Datei: Ball.inc
; -----------------------------------------------------------------------------
;   Realisiert den Ball und seine Bewegung
; 	Author: 			Luca Manuel Krause
; -----------------------------------------------------------------------------

     
		switch VarSection
;Variablen
moveX		ds.b 	1							;Ballaenderung in X-Richtung
moveY		ds.b	1							;Ballaenderung in Y-Richtung
BallX		ds.b 	1							;Ballposition in X-Richtung
BallY		ds.b	1							;Ballposition in Y-Richtung
Geschwindigkeit ds.w	1						;Geschwindigkeit des Balls, 
												;je kleiner, desto schneller
timerSeconds	ds.b 1							;wird runtergezaelt, 
												;damit der Ball laenger sichtbar ist

		switch RomSection
;Konstanten
StartGeschwindigkeit dc.w 65000
maxGeschwindigkeit dc.w		20000
startX 				dc.b	64
starty 				dc.b	32
startXMove		dc.b	1
startYMove		dc.b	1
lastX					dc.b	127
lastY					dc.b	63
timerSecondsReset		dc.b 	1


startNewGame

; -----------------------------------------------------------------------------
;	Setzt den Ball fuer jedes Spiel auf die Startposition
;	nutzt A,B,X,Y
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
			
			ldaa BallX
			ldab BallY
			jsr loadBall
			
			rts 									;Ende
			
; -----------------------------------------------------------------------------
;	Initialisiert den Interrupt-Timer fuer den Ball
;	Nutzt A,B
; -----------------------------------------------------------------------------
initBallTimer
			ldd TOC2
			addd StartGeschwindigkeit		;Timer einstellen
			std TOC2
			
			bset TMSK1, #%01000000  
			rts										;Ende
			

; -----------------------------------------------------------------------------
;	Interrupt-Service-Routine
;	Timer 2
;	Dieser Timer aktualisiert den Ball auf dem Spielfeld
;	nutzt A,B
; -----------------------------------------------------------------------------
isrOC2
			dec timerSeconds			;Soll der Ball schon neu gesetzt werden?
			bne NotReloadingBall
			
LoadingBall
			ldaa BallX
			ldab BallY
			jsr deleteBall
			
			jsr moveBall
			
			ldaa BallX
			ldab BallY
			jsr loadBall
			
			ldaa timerSecondsReset
			staa timerSeconds
			
NotReloadingBall
			ldd Geschwindigkeit							;Geschwindigkeit des Balls erhoehen
			cmpa maxGeschwindigkeit
			bmi NoSpeedCorrection
			subd #10
			std Geschwindigkeit
			
NoSpeedCorrection
														;Timer einstellen
			ldd TOC2									;Frequenz einstellen
			addd Geschwindigkeit						;diesen Wert eingeben -> (2000) / 2.000.000 = 1000 Hz
														;-> nach 1000 us wieder interrupten
			std TOC2
			
			;Flag zuruecksetzen
			bset TFLG1,  #%10000000
			
			rti											;Ende Interrupt
			
; -----------------------------------------------------------------------------
;	Bewegt den Ball und sorgt fuer ein Abprallen an den Y-Grenzen
;	nutzt A,B,
; -----------------------------------------------------------------------------
moveBall
			;Bewegen
			ldaa BallX
			adda moveX
			staa BallX
			
			ldaa BallY
			adda moveY
			staa BallY
			
			;obere Display-Grenze
			ldaa BallY
			cmpa #0
			
			bne DidntHitUpperBorder
			
			ldaa #1
			staa moveY
			
DidntHitUpperBorder	
			;untere Display-Grenze
			ldaa BallY
			cmpa lastY
			
			bne DidntHitLowerBorder
			
			ldaa #-1
			staa moveY

DidntHitLowerBorder

XUeberpruefung
			ldaa bat1X
			inca
			cmpa BallX
			beq Left			;Ist der Ball vor der Zeile des linken Schlaegers?
NotLeft		
			ldaa bat2X
			deca
			cmpa BallX
			beq Right			;Ist der Ball vor der Zeile des rechten Schlaegers?
NotRight
			bra EndMove
Left
			ldaa BallY
			cmpa BatPos1
			blo MovingPunkteB
			cmpa BatPos1End
			bhi MovingPunkteB
			
			ldaa #1
			staa moveX
			bra EndMove
Right
			ldaa BallY
			cmpa BatPos2
			blo MovingPunkteA
			cmpa BatPos2End
			bhi MovingPunkteA
			
			ldaa #-1
			staa moveX
			bra EndMove
			
MovingPunkteA
			ldd PunkteA
			addd #1
			std PunkteA
			bra startingNewGame
MovingPunkteB
			ldd PunkteB
			addd #1
			std PunkteB
			bra startingNewGame
			
startingNewGame
			jsr pointsToString
			jsr PointsToSegments
			
			ldaa BallX
			ldab BallY
			jsr deleteBall
			jsr initBall
			
EndMove			
			rts						;Ende




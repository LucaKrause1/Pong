;* Datei: Ball.inc
; -----------------------------------------------------------------------------
;   Realisiert den Ball und seine Bewegung
; 	Author: 			Luca Manuel Krause
;	Datum:				18.01.2022
; -----------------------------------------------------------------------------     
		switch VarSection
;Variablen
BallX							ds.b 	1				;Ballposition in X-Richtung
BallY							ds.b	1				;Ballposition in Y-Richtung
moveX						ds.b 	1				;Ballaenderung in X-Richtung
moveY						ds.b	1				;Ballaenderung in Y-Richtung
speed 						ds.w	1				;Geschwindigkeit des Balls, 
														;je kleiner, desto schneller
														;realisiert als Einheit 
														;fuer den Timer des Balls
timerSeconds				ds.b 1				;wird runtergezaehlt, 
														;damit der Ball laenger sichtbar ist

		switch RomSection
;Konstanten
StartSpeed				dc.w	65000
maxSpeed 				dc.w	20000
startX 						dc.b	64
startY 						dc.b	32
startXMove				dc.b	1
startYMove				dc.b	1
lastX							dc.b	127
lastY							dc.b	63
timerSecondsReset		dc.b 	1				;Grundschnelligkeit in Zyklen des Balls,
														; um die 2-Byte der Timer-Grenze zu umgehen.
														;TODO: auf 2 setzen

; -----------------------------------------------------------------------------
;	Initialisiert den Ball fuer jedes Spiel
;	nutzt A,B
;	keine Parameter
; -----------------------------------------------------------------------------
initBall
			psha
			pshb
			
			ldaa startX
			ldab startY
			staa BallX
			stab BallY
			
			ldaa startXMove
			ldab startYMove
			staa moveX
			stab moveY
			
			ldaa StartSpeed
			staa speed
			
			ldaa timerSecondsReset
			staa timerSeconds
			
			ldaa BallX
			ldab BallY
			jsr loadBall
			
			pulb
			pula
			rts 										;Ende
			
; -----------------------------------------------------------------------------
;	Initialisiert den Interrupt-Timer fuer den Ball
;	Nutzt D
;	keine Parameter
; -----------------------------------------------------------------------------
initBallTimer
			psha
			pshb
			
			ldd TOC2
			addd StartSpeed					;Timer einstellen
			std TOC2
			
			bset TMSK1, #%01000000  
			
			pulb
			pula
			rts										;Ende

; -----------------------------------------------------------------------------
;	Interrupt-Service-Routine
;	Timer 2
;	Dieser Timer aktualisiert den Ball nach 
;	einer vorbestimmten Zeit auf dem Spielfeld.
;	nutzt A,B
;	keine Parameter, da es sich um eine Interrupt Service Routine handelt.
; -----------------------------------------------------------------------------
isrOC2			
			dec timerSeconds					;Soll der Ball schon neu gesetzt werden?
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
			ldd speed								;Geschwindigkeit des Balls erhoehen
			cmpa maxSpeed
			bmi NoSpeedCorrection
			subd #10
			std speed
			
NoSpeedCorrection
														;Timer einstellen
			ldd TOC2								;Frequenz einstellen
			addd speed							;diesen Wert eingeben -> (2000) / 2.000.000 = 1000 Hz
														;-> nach 1000 us wieder interrupten
			std TOC2
			
			;Flag zuruecksetzen
			bset TFLG1,  #%10000000
			
			rti											;Ende Interrupt
			
; -----------------------------------------------------------------------------
;	Bewegt den Ball und sorgt fuer ein Abprallen an den Y-Grenzen
;	nutzt A,B
; -----------------------------------------------------------------------------
moveBall
			psha
			pshb

;Erst ueberpruefen, dann bewegen!
;ueberpruefen
			
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

XTest
			ldaa bat1X
			inca
			cmpa BallX
			beq Left								;Ist der Ball vor der Spalte des linken Schlaegers?
NotLeft		
			ldaa bat2X
			deca
			cmpa BallX
			beq Right								;Ist der Ball vor der Spalte des rechten Schlaegers?
NotRight
			bra EndMove
Left
			ldaa BallY
			cmpa BatPos1
			blo MovingPointsB
			cmpa BatPos1End
			bhi MovingPointsB
			
			ldaa #1
			staa moveX
			bra EndMove
Right
			ldaa BallY
			cmpa BatPos2
			blo MovingPointsA
			cmpa BatPos2End
			bhi MovingPointsA
			
			ldaa #-1
			staa moveX
			bra EndMove
			
MovingPointsA
			ldd PointsA
			addd #1
			std PointsA
			bra PlayerGotPoints
MovingPointsB
			ldd PointsB
			addd #1
			std PointsB
			bra PlayerGotPoints
			
PlayerGotPoints
			jsr pointsToString
			jsr PointsToSegments
			
			ldaa BallX
			ldab BallY
			jsr deleteBall
			jsr initBall
			
			bra EndOfMoving
			
EndMove
;bewegen
			;Bewegen
			ldaa BallX
			adda moveX
			staa BallX
			
			ldaa BallY
			adda moveY
			staa BallY
			
EndOfMoving
			;TODO: hinzufuegen, dass der Ball an den Raendern des Schlaegers anders abprallt?
			pulb
			pula
			rts											;Ende




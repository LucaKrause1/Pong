;* Datei: Ball.inc
; -----------------------------------------------------------------------------
;   Realisiert den Ball und seine Bewegung
; 	Author: 			Luca Manuel Krause
;	Datum:				18.01.2022
; -----------------------------------------------------------------------------     
		switch VarSection
;Variablen
BallX			ds.b 	1			;Ballposition in X-Richtung
BallY			ds.b	1			;Ballposition in Y-Richtung
moveX			ds.b 	1			;Ballaenderung in X-Richtung
moveY			ds.b	1			;Ballaenderung in Y-Richtung
BallTimer 		ds.w 	1			;Anzahl an 7-Segment-Interrupts fuer die Ball-Bewegung

BatStart		ds.b	1			;wo faengt ein Schlager an
BatMid			ds.b	1			;Start des Mittelbereichs eines Schlaegers
BatEnd			ds.b	1			;Start des Endbereichs eines Schlaegers


		switch RomSection
;Konstanten
BatSteps		dc.b	3			;Schrittweite der Bereiche eines Schlaegers
startX 			dc.b	64
startY 			dc.b	32
startXMove		dc.b	1
startYMove		dc.b	1
firstY			dc.b	1
lastY			dc.b	62
BallSpeed		dc.w	40			;Geschwindigkeit des Balls, 
									;je kleiner, desto schneller
									;realisiert als Einheit 
									;fuer den Timer des Balls

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
			
			ldaa BallX
			ldab BallY
			jsr loadBall
			
			pulb
			pula
			rts 										;Ende
			
; -----------------------------------------------------------------------------
;	Laedt den Ball neu, falls eine bestimmte Zeit vergangen ist
;	nutzt A,B
;	keine Parameter
; -----------------------------------------------------------------------------
reloadBall
			ldd BallTimer
			cmpd BallSpeed
			blo LoadingBallEnd
			ldd #0
			std BallTimer
			
LoadingBall
			ldaa BallX
			ldab BallY
			jsr deleteBall
			
			jsr moveBall
			
			ldaa BallX
			ldab BallY
			jsr loadBall
			
LoadingBallEnd
			rts											;Ende
			
; -----------------------------------------------------------------------------
;	Berechnet, den Abschlagwinkel vom uebergebenen Schlaeger
;	Parameter: 
;		A = Ballposition
;		B = Schlaegerposition
;	nutzt A,B
; -----------------------------------------------------------------------------
hitBat
			psha
			pshb
			
			stab BatStart
			addb BatSteps
			stab BatMid
			addb BatSteps
			stab BatEnd

			cmpa BatEnd
			bge EndBat
			cmpa BatMid
			bge MidBat
			
StartBat
			ldaa #-1
			staa moveY
			bra hitBatEnd
MidBat
			ldaa #0
			staa moveY
			bra hitBatEnd
EndBat
			ldaa #1
			staa moveY
			bra hitBatEnd
			
hitBatEnd

			pulb
			pula
			rts											;Ende

			
; -----------------------------------------------------------------------------
;	Bewegt den Ball und sorgt fuer ein Abprallen
;		an den Y-Grenzen und den Schlaegern
;	nutzt A,B
; -----------------------------------------------------------------------------
moveBall
			psha
			pshb

;Raender ueberpruefen
XTest
			ldaa bat1X
			inca
			inca
			cmpa BallX
			beq Left								;Ist der Ball vor der Spalte des linken Schlaegers?
NotLeft		
			ldaa bat2X
			deca
			deca
			cmpa BallX
			beq Right								;Ist der Ball vor der Spalte des rechten Schlaegers?
NotRight
			bra EndControl
Left
			ldaa BallY
			cmpa BatPos1
			blo MovingPointsB
			cmpa BatPos1End
			bhi MovingPointsB
			
			ldab BatPos1							;BallY schon in A vorhanden
			jsr hitBat								;Abschlagwinkel errechnen
			
			ldaa #1
			staa moveX
			
			bra EndControl
Right
			ldaa BallY
			cmpa BatPos2
			blo MovingPointsA
			cmpa BatPos2End
			bhi MovingPointsA
			
			ldab BatPos2							;BallY schon in A vorhanden
			jsr hitBat								;Abschlagwinkel errechnen
			
			ldaa #-1
			staa moveX
			
			bra EndControl
			
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
			
			bra EndMove
			
EndControl
			;obere Display-Grenze
			ldaa BallY
			cmpa firstY
			
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

;Bewegen
StartMove
			ldaa BallX
			adda moveX
			staa BallX
			
			ldaa BallY
			adda moveY
			staa BallY
			
EndMove
			pulb
			pula
			rts											;Ende




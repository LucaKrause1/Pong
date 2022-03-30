;* Datei: sieben_Segment.inc
; -----------------------------------------------------------------------------
;   Diese Datei befasst sich mit der Darstellung der Pukte 
;	der beiden Spieler auf der Sieben-Segment-Anzeige
; 	Author: 			Luca Manuel Krause
;	Datum:				18.01.2022
; -----------------------------------------------------------------------------

		switch VarSection
;Variablen
PointsA 			ds.w	1							;Punkte des ersten Spielers
PointsB 			ds.w 	1							;Punkte des zweiten Spielers
PointsAasString 	ds.b 	4							;Punkte des ersten Spielers
PointsBasString 	ds.b 	4							;Punkte des zweiten Spielers
segment 			ds.b 	1							;welches Segment als naechstes gezeigt wird

		switch RomSection
;Konstanten
											;Hexadezimale Darstellung der Zahlen
Numbers			dc.b				$3F,$06,$5B,$4F,$66,$6D,$7D,$07,$7F,$6F
											;Bit-Darstellung der Zahlen
											;$#00111111,$#00000110,$#01011011,
											;$#01001111,$#01100110,$#01101101,
											;$#01111101,$#00000111,$#01111111,
											;$#01101111

; -----------------------------------------------------------------------------
;	wandelt die Punkte der Spieler in Strings um 
;	nutzt A,B,Y
; -----------------------------------------------------------------------------
pointsToString
			psha
			pshb
			pshy
			
			ldd PointsA
			ldy #PointsAasString
			jsr Conversion16BitValue
						
			ldd PointsB
			ldy #PointsBasString
			jsr Conversion16BitValue
			
			puly
			pulb
			pula
			rts											;Ende
			
; -----------------------------------------------------------------------------
;	wandelt die Punkte der Spieler aus Dezimal in 7-Segment-Zahlen um
;	nutzt A,B,X,Y
; -----------------------------------------------------------------------------
pointsToSegments
			psha
			pshb
			pshx
			pshy
			
			ldaa #8
			ldx #PointsAasString
			
pointsToSegmentsLoop
			ldab 0,x
			ldy #Numbers
			aby
			ldab 0,y
			stab 0,x
			
			inx
			deca
			bne pointsToSegmentsLoop
			
			puly
			pulx
			pulb
			pula
			rts											;Ende
			

; -----------------------------------------------------------------------------
;	Initialisiert die Verbindung mit der 7-Segment-Anzeige
;	nutzt A
; -----------------------------------------------------------------------------
initSegments
			psha
			
			ldaa PORTA
			anda #%10111111
			staa PORTA
			
			pula
			rts											;Ende

; -----------------------------------------------------------------------------
;	Initialisiert den Timer fuer die 7-Segment-Anzeige
;	nutzt A
; -----------------------------------------------------------------------------
initSegmentTimer   
			psha          
			
			ldaa #%10000000       
			staa TMSK1
			       
			pula          
			rts											;Ende

; -----------------------------------------------------------------------------
;	Interrupt-Service-Routine
;	Timer 1
;	Dieser Timer aktualisiert die 7-Segment-Anzeige
;	nutzt A,B,Y
; -----------------------------------------------------------------------------
isrOC1		
			ldab segment								;Segment weiterstellen
			incb
			andb #%00000111					;nur die letzen 3 Bit sind interessant
			stab segment
			
			ldy #PointsAasString
			aby											;Segment-Index raufaddieren
			
			ldaa 0,y
			
			stab PIO_C								;0 = links, 7 = rechts
			staa PIO_A
			
			ldd BallTimer					;Timer fuer den Ball weitersetzen
			addd #1
			std BallTimer
															;Timer einstellen
			ldd TOC1									;Frequenz einstellen
			addd #2000								;diesen Wert eingeben -> (2000) / 2.000.000 = 1000 Hz
															;-> nach 1000 us wieder interrupten
			std TOC1
			
			;Flag zuruecksetzen
			bset TFLG1,  #%10000000	
			
			rti												;Ende Interrupt

;* Datei: sieben_Segment.inc
; -----------------------------------------------------------------------------
;   TODO: kurze Beschreibung
; -----------------------------------------------------------------------------

     
		switch VarSection
;Variablen
PunkteA ds.w 1									;Punkte des ersten Spielers
PunkteB ds.w 1									;Punkte des zweiten Spielers
PunkteAasString ds.b 4							;Punkte des ersten Spielers
PunkteBasString ds.b 4							;Punkte des zweiten Spielers
segment ds.b 1									;welches Segment als naechstes gezeigt wird

		switch RomSection
;Konstanten
Numbers	dc.b				$3F,$06,$5B,$4F,$66,$6D,$7D,$07,$7F,$6F
							;$#00111111,$#00000110,$#01011011,
							;$#01001111,$#01100110,$#01101101,
							;$#01111101,$#00000111,$#01111111,
							;$#01101111

; -----------------------------------------------------------------------------
;	wandelt die Punkte der Spieler in Strings um 
; -----------------------------------------------------------------------------
pointsToString
			ldd PunkteA
			ldy #PunkteAasString
			jsr Umwandlung16BitWert
						
			ldd PunkteB
			ldy #PunkteBasString
			jsr Umwandlung16BitWert
			
			rts										;Ende
			
; -----------------------------------------------------------------------------
;	wandelt die Punkte der Spieler aus Dezimal in 7-Segment-Zahlen um
; -----------------------------------------------------------------------------
pointsToSegments
			ldaa #8
			ldx #PunkteAasString
			
pointsToSegmentsLoop
			ldab 0,x
			ldy #Numbers
			aby
			ldab 0,y
			stab 0,x
			
			inx
			deca
			bne pointsToSegmentsLoop
			
			rts										;Ende
			

; -----------------------------------------------------------------------------
;	Initialisiert die Verbindung mit der 7-Segment-Anzeige
;	nutzt A
; -----------------------------------------------------------------------------
initSegments
			ldaa PORTA
			anda #%10111111
			staa PORTA
			
			rts				;Ende

; -----------------------------------------------------------------------------
;	Initialisiert den Timer fuer die 7-Segment-Anzeige
;	nutzt A
; -----------------------------------------------------------------------------
initSegmentTimer   
			psha          
			
			ldaa #%10000000       
			staa TMSK1
			       
			pula          
			rts				;Ende

; -----------------------------------------------------------------------------
;	Interrupt-Service-Routine
;	Timer 1
;	aktualisiert die 7-Segment-Anzeige		
; -----------------------------------------------------------------------------
isrOC1			
			ldy #PunkteAasString
			ldab segment
			aby									;Segment-Index raufaddieren
			
			ldaa 0,y
			
			staa PIO_A					
			stab PIO_C							;0 = links, 7 = rechts
			
			ldab segment						;Segment weiterstellen
			incb
			andb #%00000111						;nur die letzen 3 Bit sind interessant
			stab segment
			
												;Timer einstellen
			ldd TOC1							;Frequenz einstellen
			addd #2000							;diesen Wert eingeben -> (2000) / 2.000.000 = 1000 Hz
														;-> nach 1000 us wieder interrupten
			std TOC1
			
			;Flag zuruecksetzen
			bset TFLG1,  #%10000000	
			
			rti				;Ende Interrupt

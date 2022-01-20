;* Datei: sieben_Segment.inc
; -----------------------------------------------------------------------------
;   TODO: kurze Beschreibung
; -----------------------------------------------------------------------------

     
		switch VarSection
;Variablen
PunkteA ds.b 1									;Punkte des ersten Spielers
PunkteB ds.b 1									;Punkte des zweiten Spielers
PunkteAasString ds.b 4						;Punkte des ersten Spielers
PunkteBasString ds.b 4						;Punkte des zweiten Spielers
segment ds.b 1									;welches Segment als naechstes gezeigt wird

		switch RomSection
;Konstanten

; -----------------------------------------------------------------------------
;	wandelt die Punkte der Spieler in Strings um 
; -----------------------------------------------------------------------------
pointsToString
			;TODO
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
			;TODO: Wert ausgeben: noch schauen, was man braucht!!!
			;Zahlen kodieren in Konstante
			;PunkteAasString+segment = Zeichen
			staa PIO_A					
			stab PIO_C							;0 = links, 7 = rechts
			
			
			
			ldab segment							;Segment weiterstellen
			incb
			andb #%00000111				;nur die letzen 3 Bit sind interessant
			stab segment
			
														;Timer einstellen
			ldd TOC1								;Frequenz einstellen
			addd #2000							;diesen Wert eingeben -> (2000) / 2.000.000 = 1000 Hz
														;-> nach 1000 us wieder interrupten
			std TOC1
			
			;Flag zuruecksetzen
			ldaa #%10000000					
			staa TFLG1
			
			rti				;Ende Interrupt

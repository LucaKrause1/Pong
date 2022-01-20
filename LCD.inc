;* Datei: LCD.inc
; -----------------------------------------------------------------------------
;   TODO: kurze Beschreibung
; -----------------------------------------------------------------------------

		switch VarSection
;Variablen
batX ds.b 1										;X-Position des Schlaegers
Schlaeger1 ds.b 8								;Alle Positionen des Schlaegers 1
Schlaeger2 ds.b 8								;Alle Positionen des Schlaegers 2
LCDcurrentPage ds.b 1

		switch RomSection
;Konstanten
pages dc.b 8
columns dc.b 128

; -----------------------------------------------------------------------------
;   Setzt die Schlaeger im Speicher zurueck
;	-> gesamte Y-Richtung
;	-> erstmal Schlaeger = Breite 1
;	nutzt A,B,X
; -----------------------------------------------------------------------------
deleteBats
		ldab pages
		addb pages								;Zaehler initialisieren
								
		ldaa #0
		ldx #Schlaeger1						;auf den Anfang der Schlaeger zeigen
deleteBatsLoop
		staa 0,x
		inx
		decb
		bne deleteBatsLoop
		
		rts											;Ende

; -----------------------------------------------------------------------------
;   Setzt einen Schlaeger auf dem Feld zurueck
;	-> gesamte Y-Richtung
;	-> erstmal Schlaeger = Breite 1
;	Column inVariable batY
;	nutzt A,B,X
; -----------------------------------------------------------------------------
resetBat
		ldaa #0										;erste Page zuerst ansprechen
		staa LCDcurrentPage
		
		ldab #8										;Schleifendurchlaeufe, da 8 Zeilen
resetBatLoop
		ldaa batX									;Spalte einstellen
		jsr LCDcolumn
		
		ldaa LCDcurrentPage					;Zeile einstellen
		jsr LCDpage
		inca
		staa LCDcurrentPage
		
		ldaa 0,x									;Daten veroeffentlichen
		inx
		jsr LCDdata
		
		decb											;Schleife weiterlaufen lassen?
		bne resetBatLoop
		
		rts											;Ende

; -----------------------------------------------------------------------------
;   Setzt die Schlaeger auf dem Feld zurueck
;	-> gesamte Y-Richtung
;	-> erstmal Schlaeger = Breite 1
; -----------------------------------------------------------------------------
resetBats
		ldaa #1										;Eine Spalte frei nach links
		staa batX
		jsr resetBat
		
		ldaa #126									;Eine Spalte frei nach rechts
		staa batX
		jsr resetBat
		
		rts											;Ende										

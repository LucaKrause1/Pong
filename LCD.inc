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
secondPos  ds.b 1

		switch RomSection
;Konstanten
pages dc.b 8
columns dc.b 128
pagesInBits dc.b 64


deleteBall
		rts											;Ende
		
loadBall
		rts											;Ende

; -----------------------------------------------------------------------------
;   Setzt die Schlaeger im Speicher zurueck
;	-> gesamte Y-Richtung
;	-> erstmal Schlaeger = Breite 1
;	nutzt A,B,X
; -----------------------------------------------------------------------------
deleteBats
		ldab pages
		addb pages								;Zaehler initialisieren fuer beide Schlager
								
		ldaa #0
		ldx #Schlaeger1						;auf den Anfang der Schlaeger zeigen
														;wo ist Schlaeger2? -> direkt im Speicher dahinter
deleteBatsLoop
		staa 0,x
		inx
		decb
		bne deleteBatsLoop
		
		rts											;Ende


; -----------------------------------------------------------------------------
;   Setzt die Schlaeger im Speicher
;	-> gesamte Y-Richtung
;	-> erstmal Schlaeger = Breite 1
;	nutzt A,B,X
;	A = Position Schlaeger 1
;	B = Position Schlaeger 2
; -----------------------------------------------------------------------------
setBats
		stab secondPos
		ldab 
setBatsLoop1
		
		
		rts

; -----------------------------------------------------------------------------
;   Setzt einen Schlaeger auf dem Feld zurueck
;	-> gesamte Y-Richtung
;	-> erstmal Schlaeger = Breite 1
;	Column in Variable batX
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
		;TODO: wo kommt das X her?
		ldaa 0,x									;Daten veroeffentlichen
		inx
		jsr LCDdata
		
		decb											;Gehoert die naechste Zeile noch zum Spielfeld?
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

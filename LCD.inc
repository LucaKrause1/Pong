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
position ds.b 1
leftBits ds.b 1

bitNum ds.b 1
startBit ds.b 1
endBit ds.b 1									;Bit nach dem letzten zulässigen Bit

		switch RomSection
;Konstanten
pages dc.b 8
columns dc.b 128
pagesInBits dc.b 64
pageLength dc.b 8
batSize dc.b 8


deleteBall
		rts											;Ende
		
loadBall
		rts											;Ende

		
; -----------------------------------------------------------------------------
;   Setzt die Schlaeger im Speicher
;	-> gesamte Y-Richtung
;	-> erstmal Schlaeger = Breite 1
;	nutzt A,B,X,Y
;	B = Position Schlaeger
;	X = Adresse des Schlaegers
;	X ist immer der Speicherort des naechsten Bytes
; -----------------------------------------------------------------------------
setBat
		ldaa #0
		staa bitNum
		stab startBit
		addb batSize
		stab endBit
		ldaa startBit
		ldab endBit
		
		ldy #0		;Number of pages in Y
		ldab pages
		aby
		
BytesSetBatLoop		
		ldaa #0
		ldab pageLength
BitsSetBatLoop
		decb
		bmi EndSetBatLoops
		pshb			;Zaehler sichern
		lsra
		inc bitNum
		
		ldab bitNum
		
		cmpb startBit
		blo EndSetBatLoop ;Branch, wenn die bitnum < startbit
		ldab bitNum		;nicht sicher, ob das Ergebnis in B landet, da es wie eine Subtraktion implementiert ist.
		cmpb endBit
		bhi EndSetBatLoop	;Branch, wenn bitnum >= endbit
		
		adda #128			;Bit ist Teil des Schlaegers

EndSetBatLoop
		pulb			;Zaehler zurueckholen
		bra BitsSetBatLoop
		
EndSetBatLoops				;Page fertig
		staa 0,x
		inx
		
		;jsr debug
		;jsr zeilenumbruch
		dey
		beq EndeSetBat
		
		bra BytesSetBatLoop

		
EndeSetBat
		
		rts											;Ende



; -----------------------------------------------------------------------------
;   Setzt die Schlaeger im Speicher
;	-> gesamte Y-Richtung
;	-> erstmal Schlaeger = Breite 8
;	nutzt A,B,X
;	A = Position Schlaeger 1
;	B = Position Schlaeger 2
; -----------------------------------------------------------------------------
setBats
		
		
		ldx #Schlaeger1
		;jsr debug
		;jsr zeilenumbruch
		ldab BatPos1
		jsr setBat
		ldaa #1
		jsr setBatLCD
		
		
		
		ldx #Schlaeger2
		ldab BatPos2
		;jsr debug
		;jsr zeilenumbruch
		jsr setBat
		ldaa #126
		jsr setBatLCD
		
		rts					;Ende

; -----------------------------------------------------------------------------
; X = Speicherfeld eines Schlaegers
;	A = Spalte
;	gibt bisher 8 Pages untereinander aus!!!!!!!
setBatLCD
		staa batX
		ldaa #0										;erste Page zuerst ansprechen
		staa LCDcurrentPage
		
		ldab #8										;Schleifendurchlaeufe, da 8 Zeilen
		
setBatLCDLoop
		ldaa batX									;Spalte einstellen
		
		jsr LCDcolumn
		
		ldaa LCDcurrentPage							;Zeile einstellen
		jsr LCDpage
		
		ldaa LCDcurrentPage
		inca
		staa LCDcurrentPage
		
		ldaa 0,x									;Daten veroeffentlichen
		inx
		jsr LCDdata
		
		decb										;Gehoert die naechste Zeile noch zum Spielfeld?
		bne setBatLCDLoop
		
		rts					;Ende

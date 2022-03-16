;* Datei: LCD.inc
; -----------------------------------------------------------------------------
;	Darstellung von Elementen auf dem Spielfeld
; 	Author: 			Luca Manuel Krause
;	Datum:				18.01.2022
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

x_pos ds.b 1
y_pos ds.b 1

		switch RomSection
;Konstanten
pages dc.b 8
columns dc.b 128
pagesInBits dc.b 64
pageLength dc.b 8
batSize dc.b 9
bat1X	dc.b	1
bat2X	dc.b	126


; -----------------------------------------------------------------------------
;   Loescht den Ball auf dem Display
;	A = X-Position
;	B = Y-Position
; -----------------------------------------------------------------------------
deleteBall
		;TODO: interrupts unterbinden, sodass sie nicht mehr die Baelle nur teilweise loeschen
		;staa x_pos
		stab y_pos
		;sei
		;ldaa x_pos
		;ldab y_pos
		jsr LCDcolumn
		
		ldaa y_pos
		lsra
		lsra
		lsra
		jsr LCDpage
		
		ldaa #0
		jsr LCDdata
		;cli
		
		rts											;Ende
		
; -----------------------------------------------------------------------------
;   Setzt den Ball auf dem Display
;	A = X-Position
;	B = Y-Position
; -----------------------------------------------------------------------------
loadBall
		stab y_pos
		;sei
		jsr LCDcolumn
		
		ldaa y_pos
		lsra
		lsra
		lsra
		jsr LCDpage
		
		ldab y_pos
		andb #%00000111			;Divisionsrest /8 fuer das Bit in der Page
		incb
		
		ldaa #1
loadBallLoop
		decb
		beq loadBallEnd
		
		lsla
		bra loadBallLoop
		
loadBallEnd
		;Daten in A
		jsr LCDdata
		;cli
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
		incb
		stab startBit
		addb batSize
		decb
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
;	Position in Hexa: A = 0 - 37, B = 0 - 37 TODO: Zahlen ueberpruefen!!!
; -----------------------------------------------------------------------------
setBats
		ldx #Schlaeger1
		ldab BatPos1
		jsr setBat
		ldx #Schlaeger1
		ldaa bat1X
		jsr setBatLCD
		
		ldaa BatPos1
		adda batSize
		deca
		staa BatPos1End
		
		ldx #Schlaeger2
		ldab BatPos2
		jsr setBat
		ldaa bat2X
		ldx #Schlaeger2
		jsr setBatLCD
		
		ldaa BatPos2
		adda batSize
		deca
		staa BatPos2End
		
		rts					;Ende

; -----------------------------------------------------------------------------
; X = Speicherfeld eines Schlaegers
;	A = Spalte
;	gibt bisher 8 Pages untereinander aus!!!!!!!
setBatLCD
		staa batX
		
		sei											;TODO: Interrupts unterbinden
		
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
		
		cli											;TODO: Interrupts erlauben
		
		rts					;Ende

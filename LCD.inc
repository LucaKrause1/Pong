;* Datei: LCD.inc
; -----------------------------------------------------------------------------
;	Darstellung von Elementen auf dem Spielfeld
; 	Author: 			Luca Manuel Krause
;	Datum:				18.01.2022
; -----------------------------------------------------------------------------

		switch VarSection
;Variablen
batX 					ds.b 	1				;X-Position eines Schlaegers
Bat1 					ds.b 	8				;Alle Positionen des Schlaegers 1
Bat2 					ds.b 	8				;Alle Positionen des Schlaegers 2
LCDcurrentPage 	ds.b 	1
position 				ds.b 	1
leftBits 					ds.b 	1

bitNum 					ds.b 	1
startBit 					ds.b 	1
endBit 					ds.b 	1				;Bit nach dem letzten zulässigen Bit

x_pos 					ds.b 	1
y_pos 					ds.b 	1
pageNow				ds.b	1
dataByteLCD			ds.b	1


		switch RomSection
;Konstanten
pages 					dc.b 	8
columns 				dc.b 	128
pagesInBits 			dc.b 	64
pageLength 			dc.b 	8
batSize 					dc.b 	9
bat1X					dc.b	1
bat2X					dc.b	126

; -----------------------------------------------------------------------------
;   Loescht den Ball auf dem Display
;	Parameter:
;		A = X-Position
;		B = Y-Position
;	nutzt A,B
; -----------------------------------------------------------------------------
deleteBall
		;TODO: Interruptsperre einbauen, um immer in der richtigen Position zu sein
		stab y_pos							;Zwischenspeichern, da B ueberschrieben wird
		jsr LCDcolumn						;Spalte uebertragen
		
		ldaa y_pos								
		lsra
		lsra
		lsra
		jsr LCDpage							;Zeile uebertragen
		
		ldaa #0									;Daten uebertragen: Null fuers Loeschen des Balls
		jsr LCDdata
		
		rts										;Ende
		
; -----------------------------------------------------------------------------
;   Setzt den Ball auf dem Display
;	Parameter:
;		A = X-Position
;		B = Y-Position
;	nutzt A,B
; -----------------------------------------------------------------------------
loadBall
		;stab y_pos
		;;sei
		;jsr LCDcolumn
		
		;ldaa y_pos
		;lsra										;Page herausfinden
		;lsra
		;lsra
		;jsr LCDpage
		
		;ldab y_pos
		;andb #%00000111				;Divisionsrest /8 fuer das Bit in der Page
		;incb
		
		;ldaa #1
;loadBallLoop
		;decb
		;beq loadBallEnd
		
		;lsla
		;bra loadBallLoop
		
;loadBallEnd
		;;Daten in A
		;jsr LCDdata
		;cli
		
		;TODO: Interruptsperre einbauen, um immer in der richtigen Position zu sein
		;TODO: alle Werte erst speichern und gemeinsam uebertragen
		;TODO: am Ende uebertragen
		;TODO: Fall mit Interruptsperre:
		staa x_pos
		stab y_pos
		lsrb										;Page herausfinden
		lsrb
		lsrb
		stab pageNow
		
		ldab y_pos
		andb #%00000111				;Divisionsrest /8 fuer das Bit in der Page
		incb
		
		ldaa #1
loadBallLoop
		decb
		beq loadBallEnd
		
		lsla
		bra loadBallLoop
		
loadBallEnd
		staa dataByteLCD
		
		;cli
		ldaa x_pos
		jsr LCDcolumn
		ldaa pageNow
		jsr LCDpage
		ldaa dataByteLCD
		jsr LCDdata
		;sei
		
		rts										;Ende

; -----------------------------------------------------------------------------
;   Setzt einen Schlaeger im Speicher
;	-> gesamte Y-Richtung
;	nutzt A,B,X,Y
;	B = Position Schlaeger
;	X = Adresse des Schlaegers
;	X ist immer der Speicherort des naechsten Bytes
;	nutzt A,B,X,Y
; -----------------------------------------------------------------------------
setBat
		psha
		pshy
		
		ldaa #0
		staa bitNum
		incb
		stab startBit
		addb batSize
		decb
		stab endBit
		ldaa startBit
		ldab endBit
		
		ldy #0									;Number of pages in Y
		ldab pages
		aby
		
BytesSetBatLoop		
		ldaa #0
		ldab pageLength
BitsSetBatLoop
		decb
		bmi EndSetBatLoops
		pshb										;Zaehler sichern
		lsra
		inc bitNum
		
		ldab bitNum
		
		cmpb startBit
		blo EndSetBatLoop 				;Branch, wenn die bitnum < startbit
		ldab bitNum							;nicht sicher, ob das Ergebnis in B landet, da es wie eine Subtraktion implementiert ist.
		cmpb endBit
		bhi EndSetBatLoop					;Branch, wenn bitnum >= endbit
		
		adda #128							;Bit ist Teil des Schlaegers

EndSetBatLoop
		pulb										;Zaehler zurueckholen
		bra BitsSetBatLoop
		
EndSetBatLoops								;Page fertig
		staa 0,x
		inx

		dey
		beq EndeSetBat
		
		bra BytesSetBatLoop

EndeSetBat

		puly
		pula
		rts										;Ende


; -----------------------------------------------------------------------------
;   Setzt beide Schlaeger im Speicher anhand 
;	der transformierten Potentiometer-Position
;	-> gesamte Y-Richtung
;	nutzt A,B,X
; -----------------------------------------------------------------------------
setBats
		psha
		pshb
		pshx
		ldx #Bat1							;Ersten Schlaeger setzen
		ldab BatPos1
		jsr setBat
		ldx #Bat1
		ldaa bat1X
		jsr setBatLCD
		
		ldaa BatPos1
		adda batSize
		deca
		staa BatPos1End
		
		ldx #Bat2							;Zweiten Schlaeger setzen
		ldab BatPos2
		jsr setBat
		ldaa bat2X
		ldx #Bat2
		jsr setBatLCD
		
		ldaa BatPos2
		adda batSize
		deca
		staa BatPos2End
		
		pulx
		pulb
		pula
		rts										;Ende

; -----------------------------------------------------------------------------
;	Setzt beide Schlaeger auf dem Display
;	Parameter:
; 		X = Speicherfeld eines Schlaegers
;		A = Spalte
;	nutzt A,B,X
;	gibt 8 Pages untereinander aus!!!!!!!
; -----------------------------------------------------------------------------
setBatLCD
		pshb
		pshx
		staa batX				
		
		ldaa #0									;erste Page zuerst ansprechen
		staa LCDcurrentPage
		
		ldab #8									;Schleifendurchlaeufe, da 8 Zeilen
setBatLCDLoop
		sei											;TODO: Interrupts unterbinden
		ldaa batX								;Spalte einstellen
		jsr LCDcolumn						
		
		ldaa LCDcurrentPage				;Zeile einstellen
		jsr LCDpage
		
		ldaa LCDcurrentPage
		inca
		staa LCDcurrentPage
		
		ldaa 0,x								;Daten veroeffentlichen
		inx
		jsr LCDdata
		cli											;TODO: Interrupts erlauben
		
		decb										;Gehoert die naechste Zeile noch zum Spielfeld?
		bne setBatLCDLoop
		
		pulx
		pulb
		rts										;Ende

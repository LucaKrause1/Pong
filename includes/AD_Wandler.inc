;* Datei: AD_Wandler.inc
; -----------------------------------------------------------------------------
;	Empfang der Werte von den Potentiometern ueber den AD-Wandler
; 	Author: 			Luca Manuel Krause
;	Datum:				18.01.2022
; -----------------------------------------------------------------------------
 
		switch VarSection
;Variablen

		switch RomSection
;Konstanten
kanal 	dc.b 	4 							;4 = Poti links, 5 = Poti rechts

; -----------------------------------------------------------------------------
;   Schaltet den A/D-Wandler ein
;	keine Parameter
; -----------------------------------------------------------------------------
initADC
		bset OPTION, #%10000000
		rts									;Ende
		
; -----------------------------------------------------------------------------
;   Startet den AD_Wandler und wartet die Umwandlung ab
;	nutzt A,X
;	Ergebnisse:
; 	gibt die Werte 0 .. 255 zurueck
;		A = Schlaeger 1
;		B = Schlaeger 2
; -----------------------------------------------------------------------------
startADC
		ldaa kanal							;ausgewaehlten Kanal laden
		oraa #16							;das Mult Bit setzen
		
		staa ADCTL						;Wert an AD-Wandler uebergeben
		
Warte
		ldaa ADCTL
		bpl Warte							;Endlosschleife, bis MSB gesetzt
		
		ldx #ADR1
		ldaa 0,x							;1. Register laden
		ldab 1,x							;2. Register laden
		
												;Ergebnisse in A und B
		rts									;Ende

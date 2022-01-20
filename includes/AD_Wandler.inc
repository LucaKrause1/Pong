;* Datei: AD_Wandler.inc
     
		switch VarSection
;Variablen
kanal ds.b 1

		switch RomSection
;Konstanten

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
;		A = Schlaeger 1
;		B = Schlaeger 2
; -----------------------------------------------------------------------------
startADC
		staa kanal							;ausgewaehlten Kanal speichern
		oraa #16							;das Mult Bit setzen
		
		staa ADCTL						;Wert an AD-Wandler uebergeben
		
Warte
		ldaa ADCTL						;TODO: swi einbauen?
		bpl Warte							;Endlosschleife, bis MSB gesetzt
		
		ldX #ADR1
		ldaa 0,x							;1. Register laden
		ldab 1,x							;2. Register laden
		
												;Ergebnisse in A und B
		rts									;Ende

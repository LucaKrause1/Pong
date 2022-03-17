;* Datei: DezimaleUmwandlung.inc
; -----------------------------------------------------------------------------
;   Diese Datei ist hauptsaechlich dazu da, 16-Bit-Werte in einem 4-Bit langen String zu speichern.
; 	Author: 			Luca Manuel Krause
;	Datum:				18.01.2022
; -----------------------------------------------------------------------------

		switch VarSection
;Variablen
conversion16Bit		 			ds.b 	2			;Zwischengespeicherter Wert der Umwandlung
counter16conversion			ds.b 	1			;Zaehler, wie oft der Rest geholt werden soll
fourSegmentConversion		ds.w 1			;max. 4 Stellen ausgeben

		switch RomSection
;Konstanten
countConversion	 			dc.b 	4

; -----------------------------------------------------------------------------
;  Wandelt den uebergebenen Wert in Dezimalschreibweise als String
;	D = Wert
;	Y = Speicherort String
;	nutzt D,X,Y
; -----------------------------------------------------------------------------
Conversion16BitValue
		pshx
		pshy
		psha
		pshb
		
		std conversion16Bit
		sty fourSegmentConversion
		
		ldab countConversion				;Am Ende des Strings anfangen
		aby										
		
		ldaa countConversion				;anzahlAusgabe Zeichen ausgeben
		staa counter16conversion
		
		ldd conversion16Bit
													;TODO: fertig???
DivConversion
		dey										;y heruntersetzen
		
		ldx #10
		idiv										;Division durchfuehren
		
		stab 0,y
		
		dec counter16conversion		;Counter dekrementieren
		xgdx										;Div-Ergebnis holen
		bne DivConversion
		
EndStringConversion
		pulb
		pula
		puly
		pulx
		
		rts										;Ende
	

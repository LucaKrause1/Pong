;* Datei: DezimaleUmwandlung.inc
; -----------------------------------------------------------------------------
;   Diese Datei ist hauptsaechlich dazu da, 16-Bit-Werte in einem 4-Bit langen String zu speichern.
; 	Author: 			Luca Manuel Krause
;	Datum:				18.01.2022
; -----------------------------------------------------------------------------

		switch VarSection
;Variablen
UmwandlungWert16 ds.b 2				;Zwischengespeicherter Wert der Umwandlung
counter16Umwandlung ds.b 1			;Zaehler, wie oft der Rest geholt werden soll
vierstellenUmwandlung ds.w 1			;max. 4 Stellen ausgeben

		switch RomSection
;Konstanten
anzahlUmwandlung dc.b 4

; -----------------------------------------------------------------------------
;  Wandelt den uebergebenen Wert in Dezimalschreibweise als String
;	D = Wert
;	Y = Speicherort String
;	nutzt D,X,Y
; -----------------------------------------------------------------------------
Umwandlung16BitWert
		pshx
		pshy
		psha
		pshb
		
		std UmwandlungWert16
		sty vierstellenUmwandlung
		
		ldab anzahlUmwandlung			;Am Ende des Strings anfangen
		aby										
		
		ldaa anzahlUmwandlung			;anzahlAusgabe Zeichen ausgeben
		staa counter16Umwandlung
		
		ldd UmwandlungWert16
																																										;TODO: fertig???
DivUmwandlung
		dey										;y heruntersetzen
		
		ldx #10
		idiv										;Division durchfuehren
		
		stab 0,y
		
		dec counter16Umwandlung		;Counter dekrementieren
		xgdx										;Div-Ergebnis holen
		bne DivUmwandlung
		
EndeStringUmwandlung
		pulb
		pula
		puly
		pulx
		
		rts										;Ende
	

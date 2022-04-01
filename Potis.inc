;* Datei: Potis.inc
; -----------------------------------------------------------------------------
;   Wandelt die Potentiometer-Werte auf ihr erlaubtes Maximum
; 	Author: 			Luca Manuel Krause
;	Datum:				18.01.2022
; -----------------------------------------------------------------------------

		switch VarSection
;Variablen

BatPos1 		ds.b	1				;Position des ersten Schlaegers
BatPos2 		ds.b	1				;Position des zweiten Schlaegers
BatPos1End 	ds.b	1
BatPos2End 	ds.b	1

mapCoordI 			ds.b 	1
mapCoordJ 			ds.b 	1

		switch RomSection
;Konstanten
maxY 			dc.b 	56

; -----------------------------------------------------------------------------
;	transformiert die Koordinaten eines Schlaegers von 255 auf max 55
;	55 = 0x37
;	(Schlaeger immer im Bild)
;	Schlaegerposition in A
;	nutzt A
;	Ergebnisposition in A
; -----------------------------------------------------------------------------
mapCoord
		lsra
		lsra
		staa mapCoordI
		lsra
		lsra
		lsra
		staa mapCoordJ
		ldaa mapCoordI
		suba mapCoordJ
		
		cmpa maxY
		
		bne mapCoordEnd
		ldaa maxY
		deca
		
mapCoordEnd
		rts								;Ende

; -----------------------------------------------------------------------------
;	transformiert die Koordinaten beider Schlaeger auf max 55
;	55 = 0x37
;	(Schlaeger immer im Bild)
;	Schlaeger1 = BatPos1
; 	Schlaeger2 = BatPos2
; -----------------------------------------------------------------------------
transformCoords
		psha
		pshb
		
		ldaa BatPos1
		ldab #0
		jsr mapCoord		
		staa BatPos1
		
		ldaa BatPos2
		ldab #0
		jsr mapCoord
		staa BatPos2
		
		pulb
		pula
		rts								;Ende

;* Datei: Potis.inc
; -----------------------------------------------------------------------------
;   Wandelt die Potentiometer-Werte auf ihr erlaubtes Maximum
; 	Author: 			Luca Manuel Krause
;	Datum:				18.01.2022
; -----------------------------------------------------------------------------

		switch VarSection
;Variablen

BatPos1 ds.b	1						;Position des ersten Schlaegers
BatPos2 ds.b	1						;Position des zweiten Schlaegers
BatPos1End ds.b	1
BatPos2End ds.b	1

i ds.b 1
j ds.b 1

		switch RomSection
;Konstanten

maxY dc.b 56

; -----------------------------------------------------------------------------
;	transformiert die Koordinaten eines Schlaegers von 255 auf max 55 
;	(Schlaeger immer im Bild)
;	Koordinaten in A
;	nutzt A
;	Ergebnis in A
; -----------------------------------------------------------------------------
mapCoord
		lsra
		lsra
		staa i
		lsra
		lsra
		lsra
		staa j
		ldaa i
		suba j
		
		cmpa maxY
		
		bne mapCoordEnd
		ldaa maxY
		deca
		
mapCoordEnd
		rts								;Ende

; -----------------------------------------------------------------------------
;	transformiert die Koordinaten beider Schlaeger auf max 51 
;	(Schlaeger immer im Bild)
;	Schlaeger1 = BatPos1
; 	Schlaeger2 = BatPos2
; -----------------------------------------------------------------------------
transformCoords
		ldaa BatPos1
		ldab #0
		jsr mapCoord		
		staa BatPos1
		
		ldaa BatPos2
		ldab #0
		jsr mapCoord
		staa BatPos2
		
		rts								;Ende

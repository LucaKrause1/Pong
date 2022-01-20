;* Datei: Potis.inc
; -----------------------------------------------------------------------------
;   TODO: kurze Beschreibung
; -----------------------------------------------------------------------------

     
		switch VarSection
;Variablen

BatPos1 ds.b	1						;Position des ersten Schlaegers
BatPos2 ds.b	1						;Position des zweiten Schlaegers
											;Poti-Maximalwert = 255
div8 ds.b 1							;Potiposition /8
div16 ds.b 1							;Potiposition /16
div64 ds.b 1							;Potiposition /64


		switch RomSection
;Konstanten
Schlaegerbreite dc.b 11

; -----------------------------------------------------------------------------
;	transformiert die Koordinaten eines Schlaegers auf max 51 
;	(Schlaeger immer im Bild)
;	Koordinaten in A
;	nutzt A
;	Ergebnis in A
; -----------------------------------------------------------------------------
mapCoord
		lsra								;/2
		lsra								;/4
		lsra								;/8
		staa div8
		lsra								;/16
		staa div16
		lsra								;/32
		lsra								;/64
		staa div64
		
		;div64 schon im Akku A
		adda div16
		adda div8
		
		rts								;Ende

; -----------------------------------------------------------------------------
;	transformiert die Koordinaten beider Schlaeger auf max 51 
;	(Schlaeger immer im Bild)
;	Schlaeger1 = BatPos1
; 	Schlaeger2 = BatPos2
; -----------------------------------------------------------------------------
transformCoords
		ldaa BatPos1
		jsr mapCoord		
		staa BatPos1
		ldaa BatPos2
		jsr mapCoord
		staa BatPos2
		
		rts								;Ende

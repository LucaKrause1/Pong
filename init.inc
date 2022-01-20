;* Datei: init.inc
; -----------------------------------------------------------------------------
;   TODO: kurze Beschreibung
; -----------------------------------------------------------------------------

     
		switch VarSection
;Variablen

		switch RomSection
;Konstanten


initAll
			
			jsr initSPI 				;SPI-Schnittstelle
			jsr initLCD 				;LCD-Display
											
											;Potis
			jsr initADC				;AD-Wandler
			
			jsr initSegments		;7-Segment Anzeigen
			jsr initSegmentTimer
			
			rts							;Ende

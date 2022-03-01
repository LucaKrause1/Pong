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
			jsr LoeschLCD
											
											;Potis
			jsr initADC				;AD-Wandler
			
			;Punkte
			ldd #0		;Beispiel-Punkte
			std PunkteA
			std PunkteB	
			jsr pointsToString
			jsr pointsToSegments
			jsr initSegments		;7-Segment Anzeigen
			jsr initSegmentTimer
			
			
			
			
			cli
			
			rts							;Ende

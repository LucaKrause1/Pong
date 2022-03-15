;* Datei: init.inc
; -----------------------------------------------------------------------------
;   kümmert sich um alle Initialisierungen vor dem Ersten Spiel
; -----------------------------------------------------------------------------

     
		switch VarSection
;Variablen

		switch RomSection
;Konstanten

; -----------------------------------------------------------------------------
;   Initialisiert alle nötigen Schnittstellen und Punkte vor dem Spiel
; -----------------------------------------------------------------------------
initAll
			jsr initSPI 				;SPI-Schnittstelle
			
			jsr initLCD 				;LCD-Display
			jsr LoeschLCD
											
										;Potis
			jsr initADC					;AD-Wandler
			
			;Punkte
			ldd #0						;Start-Punkte
			std PunkteA
			std PunkteB	
			jsr pointsToString
			jsr pointsToSegments
			jsr initSegments			;7-Segment Anzeigen
			jsr initSegmentTimer
			
			cli
			
			rts							;Ende

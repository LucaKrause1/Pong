;* Datei: init.inc
; -----------------------------------------------------------------------------
;   kümmert sich um alle Initialisierungen vor dem Ersten Spiel
; 	Author: 			Luca Manuel Krause
;	Datum:				18.01.2022
; -----------------------------------------------------------------------------
     
		switch VarSection
;Variablen

		switch RomSection
;Konstanten

; -----------------------------------------------------------------------------
;   Initialisiert alle Variablen mit Nullen vor jedem Spielstart
;	nutzt A,X
; -----------------------------------------------------------------------------
delete
			psha
			pshx
			
			
			ldx #dummy					;Adresse der ersten Variable
			ldaa #0
deleteLoop
			staa 0,x						;loeschen
			inx
			cpx #varEnd					;alle erwischt?
			bne deleteLoop				;sonst weiter
			
			pulx
			pula
			rts								;Ende

; -----------------------------------------------------------------------------
;   Initialisiert alle nötigen Schnittstellen und Punkte vor dem Spiel
;	nutzt A,B
; -----------------------------------------------------------------------------
initAll
			psha
			pshb
			
			jsr delete						;Variablen initialisieren

			jsr initSPI 					;SPI-Schnittstelle
			
			jsr initLCD 					;LCD-Display
			jsr LoeschLCD
											
												;Potis
			jsr initADC					;AD-Wandler
			
			;Punkte
			ldd #0							;Start-Punkte
			std PointsA
			std PointsB	
			jsr pointsToString
			jsr pointsToSegments
			jsr initSegments			;7-Segment Anzeigen
			jsr initSegmentTimer
			
			cli
			
			pulb
			pula
			rts								;Ende

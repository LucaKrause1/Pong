; -----------------------------------------------------------------------------
;   Projekt. Microcontroller
;	Author: 			Luca Manuel Krause
;	Startdatum: 		18.01.2022
;	Enddatum:			TODO
;	Name des Projekts: 	Pong
;	Zusatz: realisiert auf einem Microcontroller, an dem ein LCD-Bildschirm, 
;			zwei 7-Segmentanzeigen aus jeweils 4 Zeichen bestehend, 
;			Potentiometern und einige Knoepfe angeschlossen sind.
; -----------------------------------------------------------------------------

; -----------------------------------------------------------------------------
;   Register- und Makrodefinitionen
; -----------------------------------------------------------------------------

			include trainer11Register.inc

; -----------------------------------------------------------------------------
;   Definition von Variablen im RAM
; -----------------------------------------------------------------------------

VarSection	Section
			org $0040
			
ballPos ds.b 2				;Ball-Positionen: [0] = X, [1] = Y


; -----------------------------------------------------------------------------
;   Beginn des Programmcodes im schreibgeschuetzten Teil des Speichers
;	-> Konstanten
; -----------------------------------------------------------------------------
RomSection	Section
			org $C000
        
; -----------------------------------------------------------------------------
;   Hauptprogramm
; -----------------------------------------------------------------------------

Start	
			lds #$3FFF		;Stack-Pointer initialisieren

			jsr initAll
						
Ende
			bra *


; -----------------------------------------------------------------------------
;	Include-Dateien
; -----------------------------------------------------------------------------
			include LCD.inc
			include sieben_Segment.inc
			include Potis.inc
			include init.inc
			
					
			include includes\LCDfont.inc		;TODO: an richtiger Stelle?
			include includes\LCD_communication.inc
			
			include includes\AD_Wandler.inc
			include includes\loeschvariable.inc
			
			include includes\DezimaleUmwandlung.inc
			
; -----------------------------------------------------------------------------
;   Vektortabelle
; -----------------------------------------------------------------------------

Error		bra	*			; hier waere Fehlerbehandlung sinnvoll

;*** Vektoren ***
Vector		section
			org	$FFD6

VecSCI			dc.w Error				; $FFD6 SCI Serial System
VecSPI			dc.w Error				; $FFD8 SPI Serial Transfer Complete
VecPAI			dc.w Error				; $FFDA Pulse Accumulator Input Edge
VecPAO			dc.w Error				; $FFDC Pulse Accumulator Overflow
VecTOF			dc.w Error				; $FFDE Timer Overflow
VecTOC5		dc.w Error				; $FFE0 Timer Output Compare 5
VecTOC4		dc.w Error				; $FFE2 Timer Output Compare 4
VecTOC3		dc.w Error				; $FFE4 Timer Output Compare 3
VecTOC2		dc.w Error				; $FFE6 Timer Output Compare 2
VecTOC1		dc.w isrOC1			; $FFE8 Timer Output Compare 1
VecTIC3		dc.w Error				; $FFEA Timer Input Capture 3
VecTIC2		dc.w Error				; $FFEC Timer Input Capture 2
VecTIC1		dc.w Error				; $FFEE Timer Input Capture 1
VecRTI			dc.w Error				; $FFF0 Real Time Interrupt
VecIRQ			dc.w Error				; $FFF2 External Interrupt Request
VecXIRQ		dc.w Error				; $FFF4 Non-Maskable Interrupt
VecSWI			dc.w Error				; $FFF6 Software Interrupt
VecIOT			dc.w Error				; $FFF8 Illegal Opcode Trap
VecCOP			dc.w Error				; $FFFA Computer Operate Properly 
VecCLK			dc.w Error				; $FFFC Clock Monitor Reset
VecRESET		dc.w Start				; $FFFE Reset

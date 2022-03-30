;* Datei: LCD_communication.inc
; -----------------------------------------------------------------------------
;	Kommunikation mit dem LCD-Display und initiale Subroutinen,
;	 die mit dem LCD-Display zusammenhaengen
; 	Author: 			Luca Manuel Krause
;	Datum:				18.01.2022
; -----------------------------------------------------------------------------

		switch VarSection
;Variablen
row 		ds.b 	1							;Fuer das Loschen des LCD-Displays
page 	ds.b 	1							;In welcher Zeile sind wir?

		switch RomSection
;Konstanten

; -----------------------------------------------------------------------------
;   Setzt das gesamte Display zurueck
;	nutzt D
; -----------------------------------------------------------------------------
LoeschLCD
		psha
		pshb
		
		ldaa #9
		
RepeatRows
		staa row

		ldaa #0								;wird an das Display uebertragen
		ldab #128							;Initialisiere den Spaltenzaehler mit 128 
												;->entspricht der maximalen Spaltenzahl des Displays
RepeatForRow
		ldaa #0								;wird an das Display uebertragen
		jsr LCDdata						;leeres Byte setzen				
		decb									;setze die noch zu leerenden Spalten runter
		bne RepeatForRow				;wiederhole, solange B nicht Null ist
												
EndeRow
		jsr nextRow
		ldaa row
		deca
		bne RepeatRows
		
		pulb
		pula
		rts									;Ende
		
; -----------------------------------------------------------------------------
;  Springt im Display in die naechste Page (an dern Anfang der Zeile)
;	nutzt A
; -----------------------------------------------------------------------------
nextRow
		psha
		
		ldaa #0								;Spalte auf 0 setzen
		jsr LCDcolumn

		ldaa page							;Zeile einen weiter setzen, oder auf 0 springen,
		inca									;	falls am Ende angekommen
		anda #%00000111
		
		staa page
		jsr LCDpage						;page uebertragen
		
		pula
		rts									;Ende
		
; -----------------------------------------------------------------------------
;	Setzt den Page-Cursor des LCD-Displays auf den uebergebenen Wert
;	A = Wert -> erlaubte Werte sind 0 - 7
;	nutzt A
; -----------------------------------------------------------------------------
LCDpage
		adda #%10110000			;Kommando hinzufuegen
		jsr LCDcommand
		rts									;Ende
	
; -----------------------------------------------------------------------------
;	Setzt den Spalten-Cursor des LCD-Displays auf den uebergebenen Wert
;	A = Wert -> erlaubte Werte sind 0 - 127
;	nutzt A
; -----------------------------------------------------------------------------
LCDcolumn								
		psha
		anda #%11110000
		lsra
		lsra
		lsra
		lsra
		adda #%00010000			;Kommando hinzufuegen
		jsr LCDcommand
		pula
		anda #%00001111
		adda #%00000000			;Kommando hinzufuegen
		jsr LCDcommand
		
		rts									;Ende

; -----------------------------------------------------------------------------
;   Initialisierung des Displays
;	keine Parameter
; -----------------------------------------------------------------------------
initLCD
		psha              
		ldaa #$A2             
		 bsr  LCDcommand              
		 ldaa #$A0              
		 bsr  LCDcommand              
		 ldaa #$C8              
		 bsr  LCDcommand              
		 ldaa #$24              
		 bsr  LCDcommand              
		 ldaa #$81              
		 bsr  LCDcommand              
		 ldaa #$2F              
		 bsr  LCDcommand              
		 ldaa #$2F              
		 bsr  LCDcommand              
		 ldaa #$AF              
		 bsr  LCDcommand              
		 bclr PORTA,%01000000 	; PA6 = DIMM              
		 pula   								; Hintergrundbeleuchtung an              
		 rts									;Ende

; -----------------------------------------------------------------------------
*       SPI-Unterprogramme
; -----------------------------------------------------------------------------
initSPI  
		psha          
		ldaa #%00000011          
		staa PORTD          
		ldaa #%00111010          
		staa DDRD          				; TXD,MOSI,SCK,-SS = out          
		ldaa #%01010010     		; 125kHz,Mode=0          
		staa SPCR     					; enable,no int          
		pula          
		rts									;Ende

; -----------------------------------------------------------------------------
;	Uebertraegt Bytes an das LCD-Display
;	nutzt A
; -----------------------------------------------------------------------------
SPItransfer   
		staa  SPDR         				; Byte senden 
SPIwait2      
		tst   SPSR        					; Warten mal anders              
		bpl   SPIwait2 					; auf MSB = SPIF = 1              
		ldaa  SPDR         				; Antwortbyte holen              
		rts									;Ende

; -----------------------------------------------------------------------------
;	Sendet ein Kommandobyte aus A an das Display
;	nutzt A
; -----------------------------------------------------------------------------
LCDcommand
		bclr  PORTD,%00100000 	; PD5, LCD_A0=0              
		bra LCDsend
		
; -----------------------------------------------------------------------------
;	Sendet ein Datenbyte aus A an das Display
;	nutzt A
; -----------------------------------------------------------------------------
LCDdata 
		bset  PORTD,%00100000 	; PD5, LCD_A0=1         
		bra LCDsend

; -----------------------------------------------------------------------------
;	Sendet ein Byte 
;	A = Byte
;	nutzt A,B
; -----------------------------------------------------------------------------
LCDsend
		pshb           
		
		sei   					;Interrupts unterbinden, da sonst die 
								;7-Segment-Anzeige PIO_C ueberschreibt
		ldab  PIO_C              
		andb  #%00111111   		; SPI_SEL = 0 = LCD          
		stab  PIO_C              
		jsr   SPItransfer              
		orab  #%11000000  			; SPI_SEL = 3 = EEPROM              
		stab  PIO_C
		cli
		      
		pulb              
		rts									;Ende


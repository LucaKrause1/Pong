;* Datei: LCD_communication.inc

		switch VarSection
;Variablen
row ds.b 1								;
col ds.b 1								;
column ds.b 1						;Anzahl Spalten
page ds.b 1							;Anzahl Zeilen

		switch RomSection
;Konstanten

; -----------------------------------------------------------------------------
;   Setzt das gesamte Display zurueck
;	nutzt D
; -----------------------------------------------------------------------------
LoeschLCD
		ldaa #8
		
RepeatRows
		staa row

		ldaa #0							;wird an das Display uebertragen
		ldab #128						;Initialisiere den Spaltenzaehler mit 128 
											;	->entspricht der maximalenSpaltenzahl des Displays
RepeatForRow
		jsr LCDdata					;leeres Byte setzen				
		decb								;setze die noch zu leerenden Spalten runter
		bne RepeatForRow			;wiederhole, solange B nicht Null ist
											;TODO: vielleicht muss man B auch speichern
EndeRow
		jsr nextRow
		ldaa row
		deca
		bne RepeatRows
		
		rts								;Ende
		
; -----------------------------------------------------------------------------
;  Springt im Display in die naechste Page (an dern Anfang der Zeile)
;	nutzt A
; -----------------------------------------------------------------------------
nextRow	
		ldaa #0							;Spalte auf 0 setzen
		staa column
		jsr LCDcolumn

		ldaa page						;Zeile einen weiter setzen, oder auf 0 springen,
		inca								;	falls am Ende angekommen
		cmpa #8
		bne EndeNextRow			;if page 8 then page = 0
		
		ldaa #0							;page zuruecksetzen
		
EndeNextRow
		staa page
		jsr LCDpage
		
		rts								;Ende
	
;TODO: delete?
getCharLCD
		brclr scsr, #$20, *			; Wartet auf RDRF=1        
		ldaa scdr          				; Zeichen lesen        
		rts								;Ende

; -----------------------------------------------------------------------------
;	Gibt ein Zeichen auf dem Display aus.
;	Das Zeichen wird an der Stelle ausgegeben,
;		an welcher der Cursor im Display ist.
;	A = Zeichen
;	nutzt A,B,Y
; -----------------------------------------------------------------------------
putCharLCD
		suba #$20					;Erstes Zeichen abziehen
		ldab #8
		mul								;Addition zu Adresse in D
		
		addd #chr_20				;Adresse der 8 Bytes in D
		xgdy								;Adresse nun in D
		
		ldab 0,y						;Anzahl in B
SchleifeBytes
		iny
		ldaa 0,y						;Zeichen-Byte in A laden
		jsr LCDdata					;Zeichen-Byte uebertragen
		decb
		bne SchleifeBytes
		
		ldaa #0
		jsr LCDdata					;leeres Byte setzen
		
		rts								;Ende
		
; -----------------------------------------------------------------------------
;	Setzt den Page-Cursor des LCD-Displays auf den uebergebenen Wert
;	A = Wert -> erlaubte Werte sind 0 - 7
;	nutzt A
; -----------------------------------------------------------------------------
LCDpage
		adda #%10110000		;Kommando hinzufuegen
		jsr LCDcommand
		rts								;Ende
		
; -----------------------------------------------------------------------------
;	Setzt den Page-Cursor des LCD-Displays auf den uebergebenen Wert
;	A = Wert -> erlaubte Werte sind 0 - 127
;	nutzt A
; -----------------------------------------------------------------------------
LCDcolumn							;rol-befehle, um nicht nur auf Spalte 0 zu kommen
		psha
		anda #%11110000
		lsra
		lsra
		lsra
		lsra
		adda #%00010000		;Kommando hinzufuegen
		jsr LCDcommand
		pula
		anda #%00001111
		adda #%00000000		;Kommando hinzufuegen
		jsr LCDcommand
		
		rts								;Ende

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
		 rts
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
		rts
		
SPItransfer   
		staa  SPDR         				; Byte senden 
SPIwait2      
		tst   SPSR        					; Warten mal anders              
		bpl   SPIwait2 					; auf MSB = SPIF = 1              
		ldaa  SPDR         				; Antwortbyte holen              
		rts

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
		ldab  PIO_C              
		andb  #%00111111   		; SPI_SEL = 0 = LCD              
		stab  PIO_C              
		jsr   SPItransfer              
		orab  #%11000000  			; SPI_SEL = 3 = EEPROM              
		stab  PIO_C
		      
		pulb              
		rts


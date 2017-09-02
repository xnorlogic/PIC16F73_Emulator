; PIC16F73 Configuration Bit Settings
; Assembly source line config statements
#include "p16f73.inc"
    
; CONFIG
; __config 0xFFED
 __CONFIG _FOSC_XT & _WDTE_ON & _PWRTE_OFF & _CP_ON & _BOREN_ON
 
;Reset vector
RES_VECT    CODE	0x0000    
    GOTO    START    
    
;Interrupt vector    
INTERRUPT   CODE	0x0004
    GOTO    START1
    
;Program START 
TestRut	    CODE	0x0005
START
    BSF	    0x03, 5
    CLRF    TRISB
    BCF	    0x03, 5
    CLRF    PORTB
    CLRW
    NOP
    GOTO START1
    
;Main Program  
MAIN_PROG_1 CODE	0x0010                 
START1
    CLRW
    MOVLW 0x01
    MOVWF PORTB
    
LOOP
    ADDWF PORTB, f
    NOP
    GOTO LOOP
    
    END

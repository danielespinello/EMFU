/*******************************************************************************************************************//**
 * @file f051-appfw.ld
 * Linker script FW Applicazione sensori basati su STM32F051.
 *
 * @author Maurizio Nervo
 * @version $Id$
 * @revision{ 1.0.0 }
 * @copyright{ (C) 2013 CTS cashpro S.p.A. }
 **********************************************************************************************************************/
OUTPUT_FORMAT ("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")

/* Internal Memory Map */
MEMORY
{
	app_head 	(r)		: ORIGIN = 0x08002000, LENGTH = 32				/* Informazioni Header FW e dati ausiliari			*/
	fw			(rx)	: ORIGIN = 0x08002020, LENGTH = 32k				/* Codice FW APP (Warning <= 52k manual setting - 4K E2P virtual)	*/
	app_tail 	(r)		: ORIGIN = 0x08002020 + LENGTH(fw), LENGTH = 8	/* Informazioni Tail FW e dati ausiliari			*/	
	eeprom		(rx)	: ORIGIN = 0x0800f000, LENGTH = 4k				/* Emulazione EEPROM								*/
	vtram		(rwx)	: ORIGIN = 0x20000000, LENGTH = 0xc0			/* Interrupt Vector Table in RAM					*/
	ram			(rwx)	: ORIGIN = 0x200000c0, LENGTH = 8k-0xc0			/* RAM applicazione									*/
}

/* Puntatori inizio e fine FW applicazione */
_sapp = ORIGIN(fw);
_eapp = ORIGIN(app_tail) + LENGTH(app_tail);

/* Puntatori a Stack Pointer e Reset Vector del FW applicazione */
_app_sp = ORIGIN(fw);
_app_pc = ORIGIN(fw) + 4;

/* Puntatori inizio e fine Vector Table in RAM */
_svtram = ORIGIN(vtram);
_evtram = ORIGIN(vtram) + LENGTH(vtram);

/* Puntatori inizio e fine RAM */
_sram = ORIGIN(ram);
_eram = ORIGIN(ram) + LENGTH(ram);

/* Section Definitions */ 
SECTIONS 
{ 
    .text : 
    { 
        KEEP(*(.isr_vector .isr_vector.*)) 
        *(.text .text.* .gnu.linkonce.t.*) 	      
        *(.glue_7t) *(.glue_7)		                
        *(.rodata .rodata* .gnu.linkonce.r.*)		    	                  
    } > fw
    
    .ARM.extab : 
    {
        *(.ARM.extab* .gnu.linkonce.armextab.*)
    } > fw
    
    __exidx_start = .;
    .ARM.exidx :
    {
        *(.ARM.exidx* .gnu.linkonce.armexidx.*)
    } > fw
    __exidx_end = .;
    
    . = ALIGN(4); 
    _etext = .;
    _sidata = .; 
    		
    .data : AT (_etext) 
    { 
        _sdata = .; 
        *(.data .data.*) 
        . = ALIGN(4); 
        _edata = . ;
    } > ram

    /* Firmware information */ 
    .app_head : 
    { 
        _sapp_head = .; 
        *(.app_head .app_head.*) 
    } > app_head
	
    /* Firmware information */ 
    .app_tail : 
    { 
        _sapp_tail = .; 
        *(.app_tail .app_tail.*) 
    } > app_tail
	
    /* .bss section which is used for uninitialized data */ 
    .bss (NOLOAD) : 
    { 
        _sbss = . ; 
        *(.bss .bss.*) 
        *(COMMON) 
        . = ALIGN(4); 
        _ebss = . ; 
    } > ram
    
    /* stack section */
    .co_stack (NOLOAD):
    {
        . = ALIGN(8);
        *(.co_stack .co_stack.*)
    } > ram
    
    . = ALIGN(4); 
    _end = . ; 
} 

/*****************************************************************************
* linker command file for OMAP-L138 test code.
*
* © Copyright 2009, Logic Product Development, Inc. All Rights Reserved.
******************************************************************************/

-l rts32e.lib
-l ..\..\..\..\bsl\lib\evmomapl138_bsl.lib
-l ..\..\..\..\bsl\lib\evmomapl138_csl.lib

-stack  0x2000                             /* SOFTWARE STACK SIZE           */
-heap   0x1000                             /* HEAP AREA SIZE                */
-e _Entry

/* SPECIFY THE SYSTEM MEMORY MAP */

MEMORY
{
		ONCHIP_RAM    	: org = 0x80000000   len = 0x20000        /* L3 RAM */
}

/* SPECIFY THE SECTIONS ALLOCATION INTO MEMORY */

SECTIONS
{
    .init 	 : {

                 evmomapl138_csl.lib<evmomapl138_init.obj>(.text)
               } load > 0x80000000

    .text    : load > ONCHIP_RAM              /* CODE                       */
    .data    : load > ONCHIP_RAM
    .bss     : load > ONCHIP_RAM              /* GLOBAL & STATIC VARS       */
                    RUN_START(bss_start),
					RUN_END(bss_end)
    .const   : load > ONCHIP_RAM              /* SOFTWARE SYSTEM STACK      */
    .cinit   : load > ONCHIP_RAM              /* SOFTWARE SYSTEM STACK      */
    .sysmemt : load > ONCHIP_RAM              /* SOFTWARE SYSTEM STACK      */
    .stack   : load > 0x8001E000              /* SOFTWARE SYSTEM STACK      */
}


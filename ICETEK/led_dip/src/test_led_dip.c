//-----------------------------------------------------------------------------
// \file    test_led_dip_pb.c
// \brief   implementation of OMAP-L138 leds/dip switch/pushbutton test.
//
//-----------------------------------------------------------------------------

#include "stdio.h"
#include "types.h"
#include "evmomapl138.h"
#include "evmomapl138_timer.h"
#include "evmomapl138_led.h"
#include "evmomapl138_dip.h"
#include "test_led_dip.h"

//-----------------------------------------------------------------------------
// Private Defines and Macros
//-----------------------------------------------------------------------------

#define DIP_MASK_LED_1     (0x0000000F)
#define DIP_MASK_LED_2     (0x000000F0)

//-----------------------------------------------------------------------------
// Static Variable Declarations
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Private Function Prototypes
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Public Function Definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// \brief   tests the leds and dip switches. lights each led.
//          prints message to stdout for dip changes.
//          lights leds based on dip switches.
//
// \param   none.
//
// \return  uint32_t
//-----------------------------------------------------------------------------
uint32_t TEST_ledDip(void)
{
   uint32_t rtn;
   uint32_t i;
   uint8_t tmp_val;
   uint8_t dip;
   uint32_t last_dip_state = 0;

   printf("------------------------------------------------------------\r\n");
   printf("              OMAP-L138 LED and Dip Switch Test\r\n\r\n");

   printf("Test Description\r\n");
   printf("----------------\r\n");
   printf("this code will begin by flashing the leds, then it will sit in\r\n");
   printf("a forever loop checking the dip switches. when a change in the \r\n");
   printf("dip switches is detected, a message will be printed to stdout \r\n");
   printf("and an led will be toggled.\r\n");
   printf("------------------------------------------------------------\r\n\r\n");

   //-------------------------------------
   // initialize the required bsl modules.
   //-------------------------------------
   printf("Initialize the Required BSL Modules\r\n");
   printf("-----------------------------------\r\n\r\n");
   
   EVMOMAPL138_pinmuxConfig(8, 0xFFFFFFFF, 0x88888888);
   GPIO_setDir(3, 7, 0);
   GPIO_setDir(3, 6, 0);
   GPIO_setDir(3, 5, 0);
   GPIO_setDir(3, 4, 0);
   GPIO_setDir(3, 3, 1);
   GPIO_setDir(3, 2, 1);
   GPIO_setDir(3, 1, 1);
   GPIO_setDir(3, 0, 1);


   //--------------
   // execute test.
   //--------------
   printf("\r\nExecute Test\r\n");
   printf("------------\r\n\r\n");
   
   printf("flash led patterns\r\n\r\n");
   

   // flash leds for a few seconds.
   for (i = 0; i < 3; i++)
   {
    GPIO_setOutput(3, 7, 0);
    GPIO_setOutput(3, 6, 0);
    GPIO_setOutput(3, 5, 0);
    GPIO_setOutput(3, 4, 0);
    USTIMER_delay(DELAY_1_SEC);
   
    GPIO_setOutput(3, 7, 1);
    GPIO_setOutput(3, 6, 1);
    GPIO_setOutput(3, 5, 1);
    GPIO_setOutput(3, 4, 1);
    USTIMER_delay(DELAY_1_SEC);
   }
   
   printf("loop forever printing dips switch status and setting leds based on dips\r\n\r\n");
   while (1)
   {
      // check the dip switches and print status changes.
	  GPIO_getInput(3,3,&dip);
	  GPIO_setOutput(3, 7, dip);     
	  GPIO_getInput(3,2,&dip);
	  GPIO_setOutput(3, 6, dip);     
	  GPIO_getInput(3,1,&dip);
	  GPIO_setOutput(3, 5, dip);     
	  GPIO_getInput(3,0,&dip);
	  GPIO_setOutput(3, 4, dip);     
   }

   // currently can't reach this due to the forever loop.
//    return (rtn);
}

//-----------------------------------------------------------------------------
// Private Function Definitions
//-----------------------------------------------------------------------------


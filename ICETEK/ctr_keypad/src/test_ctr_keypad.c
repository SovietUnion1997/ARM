//-----------------------------------------------------------------------------
// \file    test_nor.c
// \brief   implementation of OMAP-L138 nor test.
//
//-----------------------------------------------------------------------------

#include "stdio.h"
#include "types.h"
#include "evmomapl138.h"
#include "evmomapl138_int.h"
#include "evmomapl138_int_gpio.h"
#include "evmomapl138_timer.h"
#include "evmomapl138_ctr.h"
#include "evmomapl138_ctr_keypad.h"
#include "test_ctr_keypad.h"

//-----------------------------------------------------------------------------
// Private Defines and Macros
//-----------------------------------------------------------------------------
#define TEST_CTR_int_period (2000)


//-----------------------------------------------------------------------------
// Static Variable Declarations
//-----------------------------------------------------------------------------
static uint8_t key_code = 0xFF;

//-----------------------------------------------------------------------------
// Private Function Prototypes
//-----------------------------------------------------------------------------
static void TEST_ctr_keypad_isr(void);

//-----------------------------------------------------------------------------
// Public Function Definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// external Function Definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// \brief   tests the nor.
//
// \param   none.
//
// \return  uint32_t
//-----------------------------------------------------------------------------
uint32_t TEST_ctr_keypad(void)
{
   uint32_t rtn;

   printf("------------------------------------------------------------\r\n");
   printf("                    OMAP-L138 CTR Test\r\n\r\n");

   printf("Test Description\r\n");
   printf("----------------\r\n");
   printf("This code will configure the EMIFA to interface with CTR \r\n");
   printf("key pad on the CTR board. Once configured, the code willenable keypad. \r\n");
   printf("------------------------------------------------------------\r\n\r\n");

   //-------------------------------------
   // initialize the required bsl modules.
   //-------------------------------------
   printf("Initialize the Required BSL Modules\r\n");
   printf("-----------------------------------\r\n\r\n");
   
   rtn = ICETEK_CTR_init();
   if (rtn != ERR_NO_ERROR)
   {
      printf("error initializing ctr: %u\r\n", rtn);
      return (rtn);
   }
   
   rtn = ICETEK_CTR_KEYPAD_init();
   if (rtn != ERR_NO_ERROR)
   {
      printf("error initializing keypad: %u\r\n", rtn);
      return (rtn);
   }
   
   //--------------
   // execute test.
   //--------------
   printf("\r\nExecute Test\r\n");
   printf("------------\r\n\r\n");

   EVMOMAPL138_int_enable();
   USTIMER_delay(1000);

   GPIO_int_enable(0, 12, GPIO_INT_ON_BOTH_EDGE, 2, TEST_ctr_keypad_isr);

   ICETEK_CTR_KEYPAD_enable(true);

   // turn on and turn off the lights.
   while(1)
   {
		if(key_code == KEYCODE_9)
			break;
   }

   printf("\r\n");
   
   ICETEK_CTR_KEYPAD_enable(false);

   GPIO_int_disable(0, 12, GPIO_INT_ON_BOTH_EDGE);

   EVMOMAPL138_int_disable();

   ICETEK_CTR_close();

   return (ERR_NO_ERROR);
}



void TEST_ctr_keypad_isr(void)
{
	key_code = ICETEK_CTR_KEYPAD_getkey();
}

//-----------------------------------------------------------------------------
// Private Function Definitions
//-----------------------------------------------------------------------------


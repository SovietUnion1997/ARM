//-----------------------------------------------------------------------------
// \file    test_nor.c
// \brief   implementation of OMAP-L138 nor test.
//
//-----------------------------------------------------------------------------

#include "stdio.h"
#include "types.h"
#include "evmomapl138.h"
#include "evmomapl138_int.h"
#include "evmomapl138_int_timer.h"
#include "evmomapl138_timer.h"
#include "evmomapl138_ctr.h"
#include "evmomapl138_ctr_dcmotor.h"
#include "evmomapl138_ctr_keypad.h"
#include "test_ctr_dcmotor.h"

//-----------------------------------------------------------------------------
// Private Defines and Macros
//-----------------------------------------------------------------------------
#define TEST_CTR_int_period (2000)

// address that should be beyond anything used by u-boot.

//-----------------------------------------------------------------------------
// Static Variable Declarations
//-----------------------------------------------------------------------------
static uint32_t TEST_CTR_duty = 50;

//-----------------------------------------------------------------------------
// Private Function Prototypes
//-----------------------------------------------------------------------------

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
uint32_t TEST_ctr_dcmotor(void)
{
   uint32_t rtn;
   uint32_t i;

   printf("------------------------------------------------------------\r\n");
   printf("                    OMAP-L138 CTR Test\r\n\r\n");

   printf("Test Description\r\n");
   printf("----------------\r\n");
   printf("This code will configure the EMIFA to interface with CTR \r\n");
   printf("DC motor on the CTR board. Once configured, the code will run the DC motor. \r\n");
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
   
   rtn = ICETEK_CTR_DCMOTOR_init();
   if (rtn != ERR_NO_ERROR)
   {
      printf("error initializing DC motor: %u\r\n", rtn);
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

   USTIMER_delay(1000);

   ICETEK_CTR_DCMOTOR_direct(DCMOTOR_DIRECTION_CLOCK);

   ICETEK_CTR_DCMOTOR_enable(true);

   // turn on and turn off the lights.
   for(i = 0 ; ; i++)
   {
		if(i%100 == TEST_CTR_duty)
		{
			ICETEK_CTR_DCMOTOR_drive(DCMOTOR_DRIVE_HIGHT);
		}

		if(i%100 == 0)
		{
			ICETEK_CTR_DCMOTOR_drive(DCMOTOR_DRIVE_LOW);
		}
		
	    if(ICETEK_CTR_KEYPAD_getkey() == KEYCODE_9)
		 break;

		USTIMER_delay(TEST_CTR_int_period);
   }

   printf("\r\n");
   
   ICETEK_CTR_DCMOTOR_enable(false);

   ICETEK_CTR_close();

   return (ERR_NO_ERROR);
}


//-----------------------------------------------------------------------------
// Private Function Definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// \file    test_nor.c
// \brief   implementation of OMAP-L138 nor test.
//
//-----------------------------------------------------------------------------

#include "stdio.h"
#include "types.h"
#include "evmomapl138.h"
#include "evmomapl138_timer.h"
#include "evmomapl138_ctr.h"
#include "evmomapl138_ctr_traffic.h"
#include "evmomapl138_ctr_keypad.h"
#include "test_ctr_traffic.h"

//-----------------------------------------------------------------------------
// Private Defines and Macros
//-----------------------------------------------------------------------------
// address that should be beyond anything used by u-boot.

//-----------------------------------------------------------------------------
// Static Variable Declarations
//-----------------------------------------------------------------------------
static const uint8_t _direction[] 	= {	
								ICETEK_CTR_TRAFFIC_EAST,
							  	ICETEK_CTR_TRAFFIC_SOUTH,
							  	ICETEK_CTR_TRAFFIC_WEST,
							  	ICETEK_CTR_TRAFFIC_NORTH
							 	};
static const uint8_t _status[] 	= {
								ICETEK_CTR_TRAFFIC_STATUS_RED,
								ICETEK_CTR_TRAFFIC_STATUS_YELLOW,
								ICETEK_CTR_TRAFFIC_STATUS_GREEN
								};
//-----------------------------------------------------------------------------
// Private Function Prototypes
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// Public Function Definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// \brief   tests the nor.
//
// \param   none.
//
// \return  uint32_t
//-----------------------------------------------------------------------------
uint32_t TEST_ctr_traffic(void)
{
   uint32_t rtn,toggle = 1;
   uint32_t i, direct,color;

   printf("------------------------------------------------------------\r\n");
   printf("                    OMAP-L138 CTR Test\r\n\r\n");

   printf("Test Description\r\n");
   printf("----------------\r\n");
   printf("This code will configure the EMIFA to interface with CTR \r\n");
   printf("taffic lights on the CTR board. Once configured, the code will turn on  \r\n");
   printf("and turn off the lights. \r\n");
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
   
   rtn = ICETEK_CTR_TRAFFIC_init();
   if (rtn != ERR_NO_ERROR)
   {
      printf("error initializing traffic lights: %u\r\n", rtn);
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
   
   // turn on and turn off the lights.
   for(i = 0 ;  ; i++)
   {
	   for (direct = 0; direct < ICETEK_CTR_TRAFFIC_LIGHT_GROUPS; direct++)
	   {
		   for (color = 0; color < ICETEK_CTR_TRAFFIC_STATUS_NUM; color++)
		   {
			  if(toggle)
		      	ICETEK_CTR_TRAFFIC_turnon(_direction[direct], _status[color]);
			  else
		      	ICETEK_CTR_TRAFFIC_turnoff(_direction[direct], _status[color]);
		      
		      USTIMER_delay(DELAY_QUARTER_SEC);

			  if(ICETEK_CTR_KEYPAD_getkey() == KEYCODE_9)
		 		  goto end;
		  	}
	   }

	   toggle = 1 - toggle;


   }

end:

   printf("\r\n");
   
   ICETEK_CTR_close();

   return (ERR_NO_ERROR);
}

//-----------------------------------------------------------------------------
// Private Function Definitions
//-----------------------------------------------------------------------------


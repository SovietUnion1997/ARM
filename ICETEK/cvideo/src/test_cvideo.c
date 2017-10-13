//------------------------------------------------------------------------------
// \file    test_cvideo.c
// \brief   implementation of OMAP-L138 composite video test.
//
//-----------------------------------------------------------------------------

#include "stdio.h"
#include "types.h"
#include "evmomapl138.h"
#include "evmomapl138_timer.h"
#include "evmomapl138_gpio.h"
#include "evmomapl138_i2c.h"
#include "evmomapl138_led.h"
#include "evmomapl138_vpif.h"
#include "evmomapl138_videodec.h"
#include "test_cvideo.h"

//-----------------------------------------------------------------------------
// Private Defines and Macros
//-----------------------------------------------------------------------------

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
// \brief   tests the composite video.
//
// \param   none.
//
// \return  uint32_t
//-----------------------------------------------------------------------------
uint32_t TEST_compVideo(void)
{
   uint32_t rtn = ERR_NO_ERROR;

   printf("------------------------------------------------------------\r\n");
   printf("           OMAP-L138 Composite Video Test\r\n\r\n");

   printf("Additional Equipment\r\n");
   printf("--------------------\r\n");
   printf("- video output device (DVD player)\r\n");
   printf("- video input device (TV)\r\n");
   printf("- 2 composite video cables\r\n\r\n");

   printf("Test Description\r\n");
   printf("----------------\r\n");
   printf("connect a video output device (DVD player, etc) to\r\n");
   printf("connector J4 and a video input device (TV, etc) to connector J5.\r\n");
   printf("start the video output device and execute the code. The video\r\n");
   printf("from the output device will be transmitted to the input device.\r\n");
   printf("------------------------------------------------------------\r\n\r\n");

   //------------------------------------
   // initialize the required bsl modules
   //------------------------------------
   printf("Initialize the Required BSL Modules\r\n");
   printf("-----------------------------------\r\n\r\n");
   
   rtn = CVIDEODEC_init();
   if (rtn != ERR_NO_ERROR)
   {
      printf("error initializing composite decoder!\r\n");
      return (rtn);
   }
   
   rtn = VPIF_initReceive(VIDEO_CONN_COMPOSITE);
   if (rtn != ERR_NO_ERROR)
   {
      printf("error initializing vpif receive composite video!\r\n");
      return (rtn);
   }
   
   rtn = VPIF_initTransmit(VIDEO_CONN_COMPOSITE);
   if (rtn != ERR_NO_ERROR)
   {
      printf("error initializing vpif transmit composite video!\r\n");
      return (rtn);
   }
   
   rtn = VIDEOENC_init(VIDEO_CONN_COMPOSITE);
   if (rtn != ERR_NO_ERROR)
   {
      printf("error initializing composite encoder!\r\n");
      return (rtn);
   }

   EVMOMAPL138_pinmuxConfig(8, 0xFFFFFFFF, 0x88888888);
   GPIO_setDir(3, 7, 0);
   GPIO_setDir(3, 6, 0);
   GPIO_setDir(3, 5, 0);
   GPIO_setDir(3, 4, 0);

   //--------------
   // execute test.
   //--------------
   printf("\r\nExecute Test\r\n");
   printf("------------\r\n\r\n");
   
   printf("toggle leds to show we are alive\r\n");
   while (1)
   {
	   // turn led 0 on and wait 1 second.
       GPIO_setOutput(3, 4, 1);
	   USTIMER_delay(DELAY_HALF_SEC);

	   // turn led 1 on and wait 1 second.
       GPIO_setOutput(3, 5, 1);
	   USTIMER_delay(DELAY_HALF_SEC);

	   // turn led 2 on and wait 1 second.
       GPIO_setOutput(3, 6, 1);
	   USTIMER_delay(DELAY_HALF_SEC);

	   // turn led 3 on and wait 1 second.
       GPIO_setOutput(3, 7, 1);
	   USTIMER_delay(DELAY_HALF_SEC);

	   // turn led 3 off and wait 1 second.
	   GPIO_setOutput(3, 7, 0);
	   USTIMER_delay(DELAY_HALF_SEC);

	   // turn led 2 off and wait 1 second.
       GPIO_setOutput(3, 6, 0);
	   USTIMER_delay(DELAY_HALF_SEC);

	   // turn led 1 off and wait 1 second.
       GPIO_setOutput(3, 5, 0);
	   USTIMER_delay(DELAY_HALF_SEC);

	   // turn led 0 off and wait 1 second.
       GPIO_setOutput(3, 4, 0);
	   USTIMER_delay(DELAY_HALF_SEC);

   }

   // currently can't reach this due to the forever loop.
//    return (rtn);
}

//-----------------------------------------------------------------------------
// Private Function Definitions
//-----------------------------------------------------------------------------


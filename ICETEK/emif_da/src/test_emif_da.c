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
#include "evmomapl138_emifda.h"
#include "test_emif_da.h"

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
// external Function Definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// \brief   tests the nor.
//
// \param   none.
//
// \return  uint32_t
//-----------------------------------------------------------------------------
uint32_t TEST_emif_da(void)
{
   uint32_t rtn;
   uint32_t i;
   uint8_t  data;

   printf("------------------------------------------------------------\r\n");
   printf("                    OMAP-L138 Emif DA Test\r\n\r\n");

   printf("Test Description\r\n");
   printf("----------------\r\n");
   printf("This code will configure the EMIFA to interface with DA. \r\n");
   printf("Once configured, the code will enable da. \r\n");
   printf("------------------------------------------------------------\r\n\r\n");

   //-------------------------------------
   // initialize the required bsl modules.
   //-------------------------------------
   printf("Initialize the Required BSL Modules\r\n");
   printf("-----------------------------------\r\n\r\n");
   
   rtn = ICETEK_EMIFADA_init();
   if (rtn != ERR_NO_ERROR)
   {
      printf("error initializing da: %u\r\n", rtn);
      return (rtn);
   }
   
   
   //--------------
   // execute test.
   //--------------
   printf("\r\nExecute Test\r\n");
   printf("------------\r\n\r\n");

   // output the data.
   for(i = 0; i < 30000 ; i++)
   {
		data = i & 0xff;
		ICETEK_EMIFADA_write(ICETEK_EMIFA_CHANNEL_A, &data, sizeof(data));
		ICETEK_EMIFADA_write(ICETEK_EMIFA_CHANNEL_B, &data, sizeof(data));

		USTIMER_delay(DELAY_10TH_SEC/100);
   }

   printf("\r\n");
   
   return (ERR_NO_ERROR);
}

//-----------------------------------------------------------------------------
// Private Function Definitions
//-----------------------------------------------------------------------------


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
#include "evmomapl138_ctr_buzzer.h"
#include "evmomapl138_ctr_keypad.h"
#include "test_ctr_buzzer.h"

//-----------------------------------------------------------------------------
// Private Defines and Macros
//-----------------------------------------------------------------------------
#define TEST_CTR_int_period (2000)

// address that should be beyond anything used by u-boot.

//-----------------------------------------------------------------------------
// Static Variable Declarations
//-----------------------------------------------------------------------------
uint8_t TEST_CTR_toggle = 0;


#define nMusicNumber 52
static uint32_t music[nMusicNumber][2]=
{
	{101,1920},{121,1440},{114,480},{101,1920},{151,1920},
	{151,480},{135,480},{121,480},{101,480},{114,960},{121,960},{135,3840},
	{121,480},{114,480},{101,480},{91,480},{101,960},{101,960},
	{76,1920},{101,1920},{114,960},{121,960},{135,1440},{151,480},{151,1920},{0,1920},
	{101,1920},{121,1440},{114,480},{101,1920},{151,1920},
	{151,480},{135,480},{121,480},{101,480},{114,960},{121,960},{135,3840},
	{121,480},{114,480},{101,480},{91,480},{101,960},{101,960},
	{76,1920},{101,1920},{114,960},{121,960},{135,1440},{151,480},{151,1920},{0,1920}
};

//-----------------------------------------------------------------------------
// Private Function Prototypes
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Public Function Definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// external Function Definitions
//-----------------------------------------------------------------------------
void delay(uint32_t delay);
//-----------------------------------------------------------------------------
// \brief   tests the nor.
//
// \param   none.
//
// \return  uint32_t
//-----------------------------------------------------------------------------
uint32_t TEST_ctr_buzzer(void)
{
   uint32_t rtn;
   uint32_t i,j,delay,times;
   uint32_t stop = false;

   printf("------------------------------------------------------------\r\n");
   printf("                    OMAP-L138 CTR Test\r\n\r\n");

   printf("Test Description\r\n");
   printf("----------------\r\n");
   printf("This code will configure the EMIFA to interface with CTR \r\n");
   printf("buzzer on the CTR board. Once configured, the code will run the buzzer. \r\n");
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
   
   rtn = ICETEK_CTR_BUZZER_init();
   if (rtn != ERR_NO_ERROR)
   {
      printf("error initializing buzzer: %u\r\n", rtn);
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

    // 设置合适的音长和音高
   for ( i=0 ; i < nMusicNumber ; i++ )
   {
      music[i][0] *= 2;
      music[i][1] *= 1;
   }

   USTIMER_delay(1000);

   ICETEK_CTR_BUZZER_enable(true);

   i = 0;
   while(!stop)
   {
		delay = music[i%nMusicNumber][0];
		times = music[i++%nMusicNumber][1];
		for(j = 0 ; j < times ; j++)
		{
			if(TEST_CTR_toggle)
				ICETEK_CTR_BUZZER_drive(BUZZER_DRIVE_HIGHT);
			else
				ICETEK_CTR_BUZZER_drive(BUZZER_DRIVE_LOW);

			if(delay)
			{
				TEST_CTR_toggle ^= 1;
			}

		    if(ICETEK_CTR_KEYPAD_getkey() == KEYCODE_9)
			{
				stop = true;
			   break;
			}

			USTIMER_delay(delay);
		}
   }

   printf("\r\n");
   
   ICETEK_CTR_BUZZER_enable(false);

   ICETEK_CTR_close();

   return (ERR_NO_ERROR);
}

//-----------------------------------------------------------------------------
// Private Function Definitions
//-----------------------------------------------------------------------------


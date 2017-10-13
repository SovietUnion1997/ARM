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
#include "evmomapl138_ctr_lcd.h"
#include "evmomapl138_ctr_keypad.h"
#include "test_ctr_lcd.h"

//-----------------------------------------------------------------------------
// Private Defines and Macros
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static Variable Declarations
//-----------------------------------------------------------------------------
static uint8_t ledkey[10][8]=
{
	{0x00,0x00,0x7C,0x82,0x82,0x82,0x7C,0x00},	//0
	{0x00,0x00,0x00,0x84,0xFE,0x80,0x00,0x00},	//1
	{0x00,0x00,0x84,0xC2,0xA2,0x92,0x8C,0x00},	//2
	{0x00,0x00,0x44,0x92,0x92,0x92,0x6C,0x00},
	{0x00,0x00,0x30,0x28,0x24,0xFE,0x20,0x00},
	{0x00,0x00,0x4E,0x92,0x92,0x92,0x62,0x00},
	{0x00,0x00,0x7C,0x92,0x92,0x92,0x64,0x00},
	{0x00,0x00,0x02,0xC2,0x32,0x0A,0x06,0x00},
	{0x00,0x00,0x6C,0x92,0x92,0x92,0x6C,0x00},
	{0x00,0x00,0x4C,0x92,0x92,0x92,0x7C,0x00}
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

//-----------------------------------------------------------------------------
// \brief   tests the nor.
//
// \param   none.
//
// \return  uint32_t
//-----------------------------------------------------------------------------
uint32_t TEST_ctr_lcd(void)
{
   uint32_t rtn;
   uint32_t i;

   printf("------------------------------------------------------------\r\n");
   printf("                    OMAP-L138 CTR Test\r\n\r\n");

   printf("Test Description\r\n");
   printf("----------------\r\n");
   printf("This code will configure the EMIFA to interface with CTR \r\n");
   printf("lcd on the CTR board. Once configured, the code will enable lcd. \r\n");
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
   
   rtn = ICETEK_CTR_LCD_init();
   if (rtn != ERR_NO_ERROR)
   {
      printf("error initializing lcd: %u\r\n", rtn);
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

   ICETEK_CTR_LCD_enable(true);

   // diaplay th eled key array on lcd.
   for(i=0;;i++)
   {
		ICETEK_CTR_LCD_put_bmp(0, 0, ledkey[i % 10], 8, 8);

		ICETEK_CTR_LCD_update(0, 0, 8, 8);

		if(ICETEK_CTR_KEYPAD_getkey() == KEYCODE_9)
			break;

		USTIMER_delay(DELAY_HALF_SEC);
   }

   printf("\r\n");
   
   ICETEK_CTR_LCD_enable(false);

   ICETEK_CTR_close();

   return (ERR_NO_ERROR);
}


//-----------------------------------------------------------------------------
// Private Function Definitions
//-----------------------------------------------------------------------------


//------------------------------------------------------------------------------
// \file    test_touch.c
// \brief   implementation of OMAP-L138 touch test.
//
//-----------------------------------------------------------------------------
#include "stdio.h"
#include "types.h"
#include "evmomapl138.h"
#include "evmomapl138_gpio.h"
#include "evmomapl138_int.h"
#include "evmomapl138_int_timer.h"
#include "evmomapl138_timer.h"
#include "evmomapl138_led.h"
#include "evmomapl138_pmic.h"
#include "evmomapl138_i2c.h"
#include "evmomapl138_lcd_raster.h"
#include "evmomapl138_spiad.h"
#include "test_spi_ad.h"

//-----------------------------------------------------------------------------
// Private Defines and Macros
//-----------------------------------------------------------------------------
#define TEST_AD_int_period (100)


//-----------------------------------------------------------------------------
// Static Variable Declarations
//-----------------------------------------------------------------------------
static uint32_t TEST_AD_int_count = 0;
static uint8_t  TEST_AD_inp[256];
static uint8_t  Test_AD_channel = SPI_AD_CHANNEL_SIG0;

//-----------------------------------------------------------------------------
// Private Function Prototypes
//-----------------------------------------------------------------------------
static void TEST_spi_ad_isr(void);

//-----------------------------------------------------------------------------
// external Function Definitions
//-----------------------------------------------------------------------------
extern void intcVectorTable(void);

//-----------------------------------------------------------------------------
// Public Function Definitions
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// \brief   Test the Touch Screen feature of the PMIC
//
// \param   none.
//
// \return  none.
//-----------------------------------------------------------------------------
uint32_t TEST_spi_ad(void)
{
	uint32_t rtn;
	uint32_t i;


   printf("------------------------------------------------------------\r\n");
   printf("                OMAP-L138 SPI AD Test\r\n\r\n");
   printf("Test Description\r\n");
   printf("----------------\r\n");
   printf("This code will configure the spi to interface with a \r\n");
   printf("ad kit . \r\n");
   printf("Once configured the ad will be enable and the code \r\n");
   printf("will loop reading ad . \r\n");
   printf("------------------------------------------------------------\r\n\r\n");

	//-------------------------------------
	// initialize the required bsl modules.
	//-------------------------------------
   printf("Initialize the Required BSL Modules\r\n");
   printf("-----------------------------------\r\n\r\n");
   
	rtn = SPIAD_init();
	if (rtn != ERR_NO_ERROR)
	{
		printf("error initializing spi!\r\n");
		return (rtn);
	}

    USTIMER_delay(1000);
   
    memset(TEST_AD_inp, 0, sizeof(TEST_AD_inp));

    // output the data.
    for(i = 0; i < 256 * 20 ; i++)
    {
		SPIAD_read(Test_AD_channel, &TEST_AD_inp[TEST_AD_int_count++%256]);
		USTIMER_delay(TEST_AD_int_period);
    }

	return (rtn); //ÈíÖÐ¶ÏÎ»ÖÃ
}


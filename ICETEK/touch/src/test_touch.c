//------------------------------------------------------------------------------
// \file    test_touch.c
// \brief   implementation of OMAP-L138 touch test.
//
//-----------------------------------------------------------------------------
#include "stdio.h"
#include "types.h"
#include "evmomapl138.h"
#include "evmomapl138_gpio.h"
#include "evmomapl138_timer.h"
#include "evmomapl138_led.h"
#include "evmomapl138_pmic.h"
#include "evmomapl138_i2c.h"
#include "evmomapl138_lcd_raster.h"
#include "evmomapl138_touch.h"
#include "test_touch.h"

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
// \brief   Test the Touch Screen feature of the PMIC
//
// \param   none.
//
// \return  none.
//-----------------------------------------------------------------------------
uint32_t TEST_touch(void)
{
	uint32_t rtn;
	uint8_t reg1, reg2, reg3, reg4, temp;

	uint16_t xPos, yPos, pPos;
	uint16_t xPoses[5], yPoses[5], pPoses[5];

	ulong_t xDispPos, yDispPos;

   printf("------------------------------------------------------------\r\n");
   printf("                OMAP-L138 LCD Touch Test\r\n\r\n");

   printf("Additional Equipment\r\n");
   printf("--------------------\r\n");
   printf("- Logic Display Kit\r\n\r\n");

   printf("Test Description\r\n");
   printf("----------------\r\n");
   printf("This code will configure the LCDC to interface with a \r\n");
   printf("Logic Display Kit . \r\n");
   printf("Once configured the LCD will be wiped clean and the code \r\n");
   printf("will loop reading touch axes and updating a crosshairs drawn on the LCD. \r\n");
   printf("------------------------------------------------------------\r\n\r\n");

	//-------------------------------------
	// initialize the required bsl modules.
	//-------------------------------------
   printf("Initialize the Required BSL Modules\r\n");
   printf("-----------------------------------\r\n\r\n");
   
	rtn = TOUCH_init();
	if (rtn != ERR_NO_ERROR)
	{
		printf("error initializing touch!\r\n");
		return (rtn);
	}

#ifdef USE_SPI_TOUCH_CONTROLER
	//--------------
   // execute test.
   //--------------
   printf("\r\nExecute Test\r\n");
   printf("------------\r\n\r\n");

	printf("--- loop forever with LCD running...");
	while (1)
	{
		do
		{
			temp = TOUCH_getTouchStatus();
		}while (temp);
		
		//delay to remove jitter
		USTIMER_delay(1000);

		// Read Axis X
		TOUCH_readAxis(TSC_XPOS, &xPoses[0]);
		TOUCH_readAxis(TSC_XPOS, &xPoses[1]);
		TOUCH_readAxis(TSC_XPOS, &xPoses[2]);
		TOUCH_readAxis(TSC_XPOS, &xPoses[3]);
		TOUCH_readAxis(TSC_XPOS, &xPoses[4]);

		// Read Axis Y
		TOUCH_readAxis(TSC_YPOS, &yPoses[0]);
		TOUCH_readAxis(TSC_YPOS, &yPoses[1]);
		TOUCH_readAxis(TSC_YPOS, &yPoses[2]);
		TOUCH_readAxis(TSC_YPOS, &yPoses[3]);
		TOUCH_readAxis(TSC_YPOS, &yPoses[4]);
		
		// get average to remove deviation
		xPos = (xPoses[0] + xPoses[1] + xPoses[2] + xPoses[3] + xPoses[4])/5;
		yPos = (yPoses[0] + yPoses[1] + yPoses[2] + yPoses[3] + yPoses[4])/5;

		printf("\txPos: 0x%04X  yPos: 0x%04X \r\n", xPos, yPos);

		//calculate the pixel values
		xDispPos = (xPos * DEFAULT_XSCALE) + DEFAULT_XOFFSET;
		yDispPos = (yPos * DEFAULT_YSCALE) + DEFAULT_YOFFSET;

		//The y-axis is reversed so correct it

		//Wipe the LCD screen
		RASTER_clear(COLOR_WHITE);

		// Plot the pos
		TOUCH_plotCursor(xDispPos, yDispPos);
	}

#else

	//Clear the interrupt one last time
	TOUCH_readAxis(TSC_PRESSURE, &reg1, &reg2); // For reasons Idont know, this line is needed clear the first interrupt.
	PMIC_readRegister(INT, &temp);
	
	//--------------
   // execute test.
   //--------------
   printf("\r\nExecute Test\r\n");
   printf("------------\r\n\r\n");

	printf("--- loop forever with LCD running...");
	while (1)
	{

		// Read the touch pressure
		TOUCH_readAxis(TSC_PRESSURE, &reg1, &reg2);

		pPos = reg1 + (((uint16_t)reg2) << 8);
		//printf("\tPressure: 0x%04X \r\n", yPos);

		if (pPos < TOUCH_HOLD_LIMIT)
		{
			// Put the PMIC touch controler back in standby mode
			PMIC_writeRegister(TSCMODE, TSC_STANDBY);

			// Wait for a touch interrupt.
			do
			{
				PMIC_readRegister(INT, &temp);
			}while ((temp & PMIC_INT_TSC) == 0);

			// Clear the interrupt
			PMIC_readRegister(INT, &temp);
		}

		//USTIMER_delay(DELAY_1_SEC/10);

		// Read Axis X
		TOUCH_readAxis(TSC_XPOS, &reg1, &reg2);

		// Read Axis Y
		TOUCH_readAxis(TSC_YPOS, &reg3, &reg4);

		// combine the register values into 16bit words
		xPos = reg1 + (((uint16_t)reg2) << 8);
		yPos = reg3 + (((uint16_t)reg4) << 8);

		//calculate the pixel values
		xDispPos = (xPos * DEFAULT_XSCALE) + DEFAULT_XOFFSET;
		yDispPos = (yPos * DEFAULT_YSCALE) + DEFAULT_YOFFSET;

		//The y-axis is reversed so correct it
		yDispPos = LCD_HEIGHT - yDispPos;

		//Wipe the LCD screen
		RASTER_clear(COLOR_WHITE);

		// Plot the pos
		TOUCH_plotCursor(xDispPos, yDispPos);

	}
#endif
	//return 0;
}

//-----------------------------------------------------------------------------
// Private Function Definitions
//-----------------------------------------------------------------------------


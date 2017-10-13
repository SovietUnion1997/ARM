//------------------------------------------------------------------------------
// \file    test_audio.c
// \brief   implementation of OMAP-L138 audio test.
//
//-----------------------------------------------------------------------------

#include "stdio.h"
#include "types.h"
#include "evmomapl138.h"
#include "evmomapl138_mcBsp.h"
#include "evmomapl138_aic23.h"
#include "test_audio.h"

//-----------------------------------------------------------------------------
// Private Defines and Macros
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Static Variable Declarations
//-----------------------------------------------------------------------------

static int16_t sinetable[48] = {
    0x0000, 0x10b4, 0x2120, 0x30fb, 0x3fff, 0x4dea, 0x5a81, 0x658b,
    0x6ed8, 0x763f, 0x7ba1, 0x7ee5, 0x7ffd, 0x7ee5, 0x7ba1, 0x76ef,
    0x6ed8, 0x658b, 0x5a81, 0x4dea, 0x3fff, 0x30fb, 0x2120, 0x10b4,
    0x0000, 0xef4c, 0xdee0, 0xcf06, 0xc002, 0xb216, 0xa57f, 0x9a75,
    0x9128, 0x89c1, 0x845f, 0x811b, 0x8002, 0x811b, 0x845f, 0x89c1,
    0x9128, 0x9a76, 0xa57f, 0xb216, 0xc002, 0xcf06, 0xdee0, 0xef4c
};

//-----------------------------------------------------------------------------
// Private Function Prototypes
//-----------------------------------------------------------------------------
static uint32_t testAudioLineOut(void);
static uint32_t testAudioLineIn(void);
static void shutdownAudio(void);

//-----------------------------------------------------------------------------
// Public Function Definitions
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// \brief   tests the AIC23 CODEC
//
// \param   none.
//
// \return  uint32_t
//-----------------------------------------------------------------------------
uint32_t TEST_audio(void)
{
	uint32_t rtn = ERR_NO_ERROR;

   printf("--------------------------------------------------------------------\r\n");
   printf("                     Audio CODEC test\r\n\r\n");
   
   printf("Additional Equipment\r\n");
   printf("--------------------\r\n");
   printf("- 3.5mm pass through cable\r\n\r\n");
   printf("- 3.5mm headphones\r\n\r\n");
   
   printf("Test Description\r\n");
   printf("----------------\r\n");
   printf("The test will begin by playing a 5 second tone though the line\r\n");
   printf("out port. The 3.5mm pass through cable will not be needed for\r\n");
   printf("this portion of the test. After playing the tone, audio from the\n\r");
   printf("line in port will be played through the line out port for 15 seconds.\r\n");
   printf("--------------------------------------------------------------------\r\n\r\n");

	//------------------------------------
	// initialize the required bsl modules
	//------------------------------------
   printf("Initialize the Required BSL Modules\r\n");
   printf("-----------------------------------\r\n\r\n");
   
	rtn = MCBSP_init();
	if (rtn != ERR_NO_ERROR)
	{
		printf("error initializing mcbsp!\r\n");
		return (rtn);
	}

	rtn = AIC23_init();
	if (rtn != ERR_NO_ERROR)
	{
		printf("error initializing aic23!\r\n");
		return (rtn);
	}

	//-----------------------
	// execute line out test.
	//-----------------------
   printf("\r\nExecute Test\r\n");
   printf("------------\r\n\r\n");
	
   printf("--- test audio line out (put on your headphones...) ---\r\n");
   rtn = testAudioLineOut();
   if (rtn != ERR_NO_ERROR)
   {
      printf("\tline out test failed\r\n\r\n");
      return (rtn);
   }
   else
   {
      printf("\tline out test passed\r\n\r\n");
   }

	//------------------------------------
	// initialize the required bsl modules
	//------------------------------------
	rtn = MCBSP_init();
	if (rtn != ERR_NO_ERROR)
	{
		printf("error initializing mcbsp!\r\n");
		return (rtn);
	}

	rtn = AIC23_init();
	if (rtn != ERR_NO_ERROR)
	{
		printf("error initializing aic23!\r\n");
		return (rtn);
	}

	//-----------------------
	// execute line in test.
	//-----------------------
	printf("--- test audio line in (put on your headphones and plug in source to line in...) ---\r\n");

	rtn = testAudioLineIn();
   if (rtn != ERR_NO_ERROR)
   {
      printf("\tline in test failed\r\n\r\n");
      return (rtn);
   }
   else
   {
      printf("\tline in test passed\r\n\r\n");
   }

	// kill codec and mcasp.
   shutdownAudio();

	return (rtn);
}

//-----------------------------------------------------------------------------
// Private Function Definitions
//-----------------------------------------------------------------------------

uint32_t testAudioLineOut(void)
{
   uint32_t rtn = ERR_NO_ERROR;
   int16_t msec, sec;
   int16_t sample;
   // transmit beep.
   for (sec = 0; sec < 5; sec++)
   {
      for (msec = 0; msec < 1000; msec++)
      {
         for (sample = 0; sample < 48; sample++)
         {
            // wait for xmit ready and send a sample to the left channel.
		    // send a sample to the left channel
		    while (!CHKBIT(MCBSP1->SPCR, 0x00020000))
		       ;
		    MCBSP1->DXR = sinetable[sample];

		    // send a sample to the right channel
		    while (!CHKBIT(MCBSP1->SPCR, 0x00020000))
		       ;
		    MCBSP1->DXR = sinetable[sample];
          }
      }
   }

   return (rtn);
}

uint32_t testAudioLineIn(void)
{
   uint32_t rtn = ERR_NO_ERROR;
   int16_t msec, sec, sample;
   int32_t dat;

   MCBSP1->DXR = 0;

  // loop audio
   for (sec = 0; /* sec < 5 */; sec++)
   {
      for (msec = 0; msec < 1000; msec++)
      {
         for (sample = 0; sample < 48; sample++)
         {
            // wait for xmit ready and send a sample to the left channel.
		    // send a sample to the left channel
		    while (!CHKBIT(MCBSP1->SPCR, 0x00000002))
		       ;
			dat = MCBSP1->DDR;
		    MCBSP1->DXR = dat;

		    // send a sample to the right channel
		    while (!CHKBIT(MCBSP1->SPCR, 0x00000002))
		       ;
			dat = MCBSP1->DDR;
		    MCBSP1->DXR = dat;
          }
      }
   }

   return (rtn);
}

void shutdownAudio(void)
{
   // close codec.
   AIC23_writeRegister(0x0F, 0);

   MCBSP1->SPCR = 0;
}


//-----------------------------------------------------------------------------
// \file    main.c
// \brief   implementation of main() to test bsl drivers.
//
//-----------------------------------------------------------------------------
#include "stdio.h"
#include "types.h"
#include "evmomapl138.h"
#include "evmomapl138_timer.h"
#include "evmomapl138_i2c.h"
#include "test_sata.h"


//-----------------------------------------------------------------------------
// Private Defines and Macros
//-----------------------------------------------------------------------------
// uncomment this define if running without gel initialization.
// #define NO_GEL    (1)

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
// \brief   entry point for bsl test code.
//
// \param   none.
//
// \return  none.
//-----------------------------------------------------------------------------
int main(void)
{
   uint32_t results = 0;

   printf("\r\n\r\n********** OMAP-L138 TEST BEGIN **********\r\n\r\n");
   
#if NO_GEL
   EVMOMAPL138_init();
   EVMOMAPL138_initRAM();
#endif
   
   // init the us timer and i2c for all to use.
   USTIMER_init();
   I2C_init(I2C0, I2C_CLK_400K);

   // TEST SATA
   //---------
   results = TEST_sata();
   printf("\r\n\r\n");

   if (results != ERR_NO_ERROR)
      printf("\r\n********** OMAP-L138 TEST FAILED **********\r\n");
   else
      printf("\r\n********** OMAP-L138 TEST PASSED **********\r\n");
}

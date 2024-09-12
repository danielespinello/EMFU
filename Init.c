/*** INIT.C ***********************************************
 *
 *********************************************************/
#include "env_def.h"
#include "config.h"
#include "svc_def.h"
#include "sys_var.h"
/*** Types ***********************************************/
/*** Variables *******************************************/
SYS_VAR;
/*** Functions *******************************************/
STATIC void prog_run(void);
/*** main *************************************************
 * Param:
 * Return:
 *********************************************************/
int main(void)
{
	/* Mandatory:
	 * Riloca la Interrupt Vector Table all'inizio della RAM e la mappa nella memoria di sistema.
	 * Questo blocco deve essere all'inizio del main, prima di eseguire qualunque altra operazione.
	 */
	/* VT_RELOCATION START */
	extern dword _sapp, _svtram, _evtram;
	MEMCPY(&_svtram, &_sapp, (int)&_evtram - (int)&_svtram);
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_SYSCFG, DISABLE);
	SYSCFG_MemoryRemapConfig(SYSCFG_MemoryRemap_SRAM);
	/* VT_RELOCATION END */
	prog_run();
	return(0);
}
/*** prog_run ********************************************
 * Param:
 * Return:
 *********************************************************/
SECTION(".app_tail")  const dword app_tail_tbl[2] = {0, APP_CHECK_VAL}; // crc, product id
extern const dword g_pfnVectors[];
SECTION(".app_head") const dword app_head_tbl[8] = {
			(dword)app_tail_tbl + sizeof(app_tail_tbl), (dword)app_head_tbl,
			(dword)g_pfnVectors, (dword)g_pfnVectors,
			APP_VERSION, 0xFFFFFFFF,
			0xFFFFFFFF, 0xFFFFFFFF
		};
void prog_run(void)
{
	volatile dword dw;

	dw = app_head_tbl[0];			// Mandatory
	dw = app_tail_tbl[0];			// Mandatory

	prog_main();
									// Return to BOOTLOADER
	RTC_WriteBackupRegister(RTC_BKP_DR0, 1);
	NVIC_SystemReset();
	for (dw = 0; ; dw--)
		;
}

/*** PROG.C ***********************************************
 *
 *********************************************************/
#include "env_def.h"
#include "config.h"
#include "app_def.h"
#include "svc_def.h"
#include "sys_var.h"
/*** Types ***********************************************/
/*** Variables *******************************************/
SYS_VAR_EXT;
/*** Functions *******************************************/
/*** prog_main ********************************************
 * Param:
 * Return:
 *********************************************************/
void prog_main(void)
{
	STRUCT_SET(sys, 0);
	svc_hw_install();
	svc_hw_tic_install(TRUE);
	svc_e2p_install(TRUE);
	svc_adc_install(TRUE);
	svc_rsa_install(TRUE);
	svc_rsa_ena(TRUE);
	svc_i2c_install(TRUE);
	svc_i2c_ena(TRUE);
	app_install(TRUE);
	for (; ; svc_hw())
	{
		app_main();
		if (sys.download_reboot_val == DOWNLOAD_REBOOT_VAL)
			break;
	}
}

/*** SVC_ADC.C *********************************************************
*
***********************************************************************/
#include "env_def.h"
#include "config.h"
#include "svc_def.h"
#include "hard_def.h"
#include "sys_var.h"
/*** Definitions ******************************************************/
/*** Constants ********************************************************/
STATIC CONST dword adc_chan_tbl[ADC_N] = ADC_CHAN_TBL;
/** Types and prototypes **********************************************/
/*** Variables ********************************************************/
SYS_VAR_EXT;
SVC_VAR_EXT;
/*** Functions ********************************************************/
/*** svc_adc_install **************************************************
* Parameters :	void
* Return :		void
**********************************************************************/
bool svc_adc_install(bool flg)
{
	bool res = TRUE;

	STRUCT_SET(SADC, 0);
	HW_ADC_INS(adc_chan_tbl);
	HW_ADC_DMA_INS(SADC.data_tbl);
	HW_ADC_ENA;
	return(res);
}
/*** svc_adc_ena ******************************************************
* Parameters :	bool
* Return :		Bool result
* Warning:		Interrupt handler called
**********************************************************************/
bool svc_adc_ena(bool flg)
{
	bool res = TRUE;

	if (flg)
	{
		HW_ADC_DONE_ACK;
		HW_ADC_START;
	}
	return(res);
}
/*** svc_adc_done_tst *************************************************
* Parameters :	void
* Return :		Bool result
* Warning:		Interrupt handler called
**********************************************************************/
bool svc_adc_done_tst(void)
{
	bool res = TRUE;

	res = HW_ADC_DONE_TST;
	if (res)
		HW_ADC_DONE_ACK;
	return(res);
}
/*** svc_adc_rd ******************************************************
* Parameters :	id, val reference
* Return :		Bool result
* Warning:		Interrupt handler called
**********************************************************************/
bool svc_adc_rd(byte id, word *val_ptr)
{
	bool res = TRUE;

	res = (id < SIZEOF(SADC.data_tbl));
	if (res)
		*val_ptr = SADC.data_tbl[id];
	return(res);
}


/*** SVC_RSA.C *********************************************************
*
***********************************************************************/
#include "env_def.h"
#include "config.h"
#include "svc_def.h"
#include "hard_def.h"
#include "sys_var.h"
/*** Definitions ******************************************************/
/*** Constants ********************************************************/
/** Types and prototypes **********************************************/
/*** Variables ********************************************************/
SYS_VAR_EXT;
SVC_VAR_EXT;
/*** Functions ********************************************************/
/*** svc_rsa_install *************************************************
* Parameters :	void
* Return :		void
**********************************************************************/
bool svc_rsa_install(bool flg)
{
	bool res = TRUE;

	STRUCT_SET(SRSA, 0);
															// Interrupt controller + USART
	HW_RSA_INT_INS;
	HW_RSA_ASYNC_INS(RSA_RATE, RSA_PARITY, RSA_BIT_N, RSA_STOP_N);
	return(res);
}
/*** svc_rsa_ena *******************************************************
* Parameters:	flg
* Return:		void
***********************************************************************/
bool svc_rsa_ena(bool flg)
{
	bool res = TRUE;

	if (flg)
		HW_RSA_RX_INT_ENA;
	else
		HW_RSA_RX_INT_DIS;
	return(res);
}
/*** HW_RSA_HANDLER ***************************************************
* Parameters :	void
* Return :		void
**********************************************************************/
HW_RSA_HANDLER()
{
	if (HW_RSA_RX_CRT_TST)
	{									// Read one byte from the receive data register
		HW_RSA_RX_CRT(RSA_RX_BUF[SRSA.rx_wr_ind]);
		VAR_INC_ROTATE(SRSA.rx_wr_ind, SIZEOF(RSA_RX_BUF));
	}
	if (HW_RSA_TX_IDLE_TST)
	{									// Write one byte to the transmit data register
		HW_RSA_TX_CRT(RSA_TX_BUF[SRSA.tx_rd_ind]);
		VAR_INC_ROTATE(SRSA.tx_rd_ind, SIZEOF(RSA_TX_BUF));
		if (SRSA.tx_rd_ind == SRSA.tx_wr_ind)
		{							// Disable the Transmit interrupt
			HW_RSA_TX_INT_DIS;
		}
	}
}
/*** svc_rsa_crt_put ***************************************************
* Parameters:	crt
* Return:		bool
***********************************************************************/
bool svc_rsa_crt_put(byte crt)
{
	bool res = FALSE;
	t_rsa_tx_ind ind;

	VAR_ADD_ROTATE(ind, SRSA.tx_wr_ind, 1, SIZEOF(RSA_TX_BUF));
	res = !(ind == SRSA.tx_rd_ind);
	if (res)
	{
		RSA_TX_BUF[SRSA.tx_wr_ind] = crt;
		SRSA.tx_wr_ind = ind;
		HW_RSA_TX_INT_ENA;
	}
	return(res);
}
/*** svc_rsa_crt_get ***************************************************
* Parameters:	crt_ptr
* Return:		bool
***********************************************************************/
bool svc_rsa_crt_get(byte *crt_ptr)
{
	bool res = FALSE;

	res = !(SRSA.rx_rd_ind == SRSA.rx_wr_ind);
	if (res)
	{
		*crt_ptr = RSA_RX_BUF[SRSA.rx_rd_ind];
		VAR_ADD_ROTATE(SRSA.rx_rd_ind, SRSA.rx_rd_ind, 1, SIZEOF(RSA_RX_BUF));
	}
	return(res);
}
/*** svc_rsa_tx_free_eva ***********************************************
* Parameters:	void
* Return:		cnt
***********************************************************************/
word svc_rsa_tx_free_eva(void)
{
	return(BUF_CIRC_FREE_EVA(SRSA.tx_wr_ind, SRSA.tx_rd_ind, SIZEOF(RSA_TX_BUF)));
}
/*** svc_rsa_tx_empty_tst ***********************************************
* Parameters:	void
* Return:		cnt
***********************************************************************/
bool svc_rsa_tx_empty_tst(void)
{
	return(SRSA.tx_wr_ind == SRSA.tx_rd_ind);
}

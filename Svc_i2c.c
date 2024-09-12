/*** SVC_I2C.C *********************************************************
* I2C Slave
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
/*** svc_i2c__install *************************************************
* Parameters :	flg
* Return :		void
**********************************************************************/
bool svc_i2c_install(bool flg)
{
	bool res = TRUE;

	STRUCT_SET(SI2C, 0);
	HW_I2C_INT_INS;
	HW_I2C_INS;
	return(res);
}
/*** svc_i2c_ena *******************************************************
* Parameters:	flg
* Return:		void
***********************************************************************/
bool svc_i2c_ena(bool flg)
{
	bool res = TRUE;

	if (flg)
		HW_I2C_INT_ENA;
	else
		HW_I2C_INT_DIS;
	return(res);
}
/*** svc_i2c_crt_put ***************************************************
* Parameters:	crt
* Return:		bool
***********************************************************************/
bool svc_i2c_crt_put(byte crt)
{
	bool res = FALSE;
	t_i2c_tx_ind ind;

	VAR_ADD_ROTATE(ind, SI2C.tx_wr_ind, 1, SIZEOF(I2C_TX_BUF));
	res = !(ind == SI2C.tx_rd_ind);
	if (res)
	{
		I2C_TX_BUF[SI2C.tx_wr_ind] = crt;
		SI2C.tx_wr_ind = ind;
	}
	return(res);
}
/*** svc_i2c_tx_empty_tst **********************************************
* Parameters:	void
* Return:		bool
***********************************************************************/
bool svc_i2c_tx_empty_tst(void)
{
	return(SI2C.tx_rd_ind == SI2C.tx_wr_ind && !(SI2C.st == I2C_ST_RUN_TX));
}
/*** svc_i2c_crt_get ***************************************************
* Parameters:	crt_ptr (if null return status empty or not)
* Return:		bool
***********************************************************************/
bool svc_i2c_crt_get(byte *crt_ptr)
{
	bool res = FALSE;

	res = !(SI2C.rx_rd_ind == SI2C.rx_wr_ind);
	if (res)
	{
		if (crt_ptr)
		{
		*crt_ptr = I2C_RX_BUF[SI2C.rx_rd_ind];
		VAR_ADD_ROTATE(SI2C.rx_rd_ind, SI2C.rx_rd_ind, 1, SIZEOF(I2C_RX_BUF));
	}
	}
	return(res);
}
/*** HW_I2C_EVE_HANDLER ***********************************************
* Parameters :	void
* Return :		void
**********************************************************************/
HW_I2C_EVE_HANDLER()
{
	if (HW_I2C_RX_ADR_TST)
	{
		if (I2C_GetTransferDirection(I2C2) == I2C_Direction_Transmitter)
		{									// Master to slave (write)
			SI2C.st = I2C_ST_RUN_RX;
			HW_I2C_RX_INT_ENA;
		}
		else
		{									// Master from slave (read)
			HW_I2C_TX_INT_ENA;
			SI2C.st = I2C_ST_RUN_TX;
		}
		HW_I2C_RX_ADR_ACK;
    }
	if (HW_I2C_RX_CRT_TST)
	{
		if (SI2C.st == I2C_ST_RUN_RX)
		{
			HW_I2C_RX_CRT(I2C_RX_BUF[SI2C.rx_wr_ind]);
			VAR_INC_ROTATE(SI2C.rx_wr_ind, SIZEOF(I2C_RX_BUF));
		}
	}
	if (HW_I2C_TX_IDLE_TST)
	{
		if (SI2C.st == I2C_ST_RUN_TX)
		{
		if (!(SI2C.tx_rd_ind == SI2C.tx_wr_ind))
		{
			HW_I2C_TX_CRT(I2C_TX_BUF[SI2C.tx_rd_ind]);
			HW_I2C_TX_INT_ENA;
			VAR_INC_ROTATE(SI2C.tx_rd_ind, SIZEOF(I2C_TX_BUF));
		}
		else
			{
			HW_I2C_TX_INT_DIS;
	}
		}
	}
	if (HW_I2C_RX_STOP_TST)
	{
		if (SI2C.st == I2C_ST_RUN_RX ||(SI2C.tx_rd_ind == SI2C.tx_wr_ind))
		SI2C.st = I2C_ST_VOID;
		HW_I2C_RX_STOP_ACK;
    }
}

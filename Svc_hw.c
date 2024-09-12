/*** SVC_HW.C **********************************************************
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
SVC_VAR;
/*** Functions ********************************************************/
/*** svc_hw_install ***************************************************
* Parameters :	void
* Return :		void
**********************************************************************/
void svc_hw_install(void)
{
	STRUCT_SET(SVC, 0);
	HW_CPU_CLK_RD(&SHW.RCC_Clocks);
	HW_CPU_CLK_INS;
	HW_IO_INS;
}
/*** svc_hw ***********************************************************
* Parameters :	void
* Return :		void
**********************************************************************/
void svc_hw(void)
{
	svc_prot();
}
/*** svc_hw_int_ena **************************************************
* Parameters :	bool flg
* Return :		void
**********************************************************************/
void svc_hw_int_ena(bool flg)
{
	if (flg)
		HW_INT_ENA;
	else
		HW_INT_DIS;
}
/*** svc_hw_stp_init **************************************************
* Parameters :	void
* Return :		void
**********************************************************************/
void svc_hw_stp_init(void)
{
	STRUCT_SET(sys.stp, 0);
	sys.stp.ver_id = STP_VER_ID;
	sys.stp.ir.k = IR_PWM_K_CENT_DEF;
	sys.stp.us.gain = US_GAIN_DEF;
	sys.stp.us.dist_min_mm = US_DIST_MIN_MM_DEF;
	sys.stp.us.dist_max_mm = US_DIST_MAX_MM_DEF;
	sys.stp.us.echo_lev_mv = US_ECHO_LEV_MV_DEF;
	sys.stp.us.hyst_lev_mv = US_HYST_LEV_MV_DEF;
}
/*** svc_hw_delay_ms **************************************************
* Parameters :	delay
* Return :		void
**********************************************************************/
void svc_hw_delay_ms(volatile dword delay)
{
	for (SHW.TimingDelay = delay; (SHW.TimingDelay != 0);)
		;
}
/*** svc_hw_delay_us **************************************************
* Parameters :	delay
* Return :		void
**********************************************************************/
void svc_hw_delay_us(dword delay)
{
	dword val;

	val = HW_TIC_VAL;
	for (delay *= (SHW.RCC_Clocks.HCLK_Frequency / 1000000); (abs(val - HW_TIC_VAL) < delay);)
		;
}
/*** svc_hw_tic_install ***********************************************
* Parameters :	bool
* Return :		void
**********************************************************************/
bool svc_hw_tic_install(bool flg)
{
	bool res = TRUE;

	HW_TIC_INS(TIC_FREQ, SHW.RCC_Clocks.HCLK_Frequency);
	return(res);
}
/*** svc_hw_tic_set **************************************************
* Parameters :	id, timeout
* Return :		void
**********************************************************************/
void svc_hw_tic_set(byte id, dword timeout)
{
	sys.tic_tbl[id] = timeout;
}
/*** svc_hw_tic_tst **************************************************
* Parameters :	id
* Return :		void
**********************************************************************/
bool svc_hw_tic_tst(byte id)
{
	return(sys.tic_tbl[id] == 0);
}
/*** TIC HANDLER *****************************************************
* Parameters :	void
* Return :		void
**********************************************************************/
HW_TIC_HANDLER()
{
	byte i;

	if (!(SHW.TimingDelay == 0x00))
		SHW.TimingDelay--;
	for (i = 0; i < TIC_N; i++)
	{
		if (!(sys.tic_tbl[i] == 0x00))
			sys.tic_tbl[i]--;
	}
}
/*** svc_hw_ir_pwm_ins ************************************************
* Parameters :	flg = TRUE to install
* Return :		bool
**********************************************************************/
bool svc_hw_ir_pwm_ins(bool flg)
{
	bool res = TRUE;

	if (flg)
	{
		HW_TIM_IR_PWM_INS;
		HW_TIM_IR_PWM_CLR;
		HW_TIM_IR_PWM_OUT_INS(IR_PWM_DUTY_MIN);
		HW_TIM_IR_PWM_CLR;
		HW_TIM_IR_PWM_ENA;
	}
	return(res);
}
/*** svc_hw_ir_pwm_upd ***********************************************
* Parameters :	duty_n
* Return :		bool
**********************************************************************/
bool svc_hw_ir_pwm_upd(byte duty_n)
{
	bool res = TRUE;

	if (duty_n == 0)
		HW_TIM_IR_PWM_OUT_DIS, duty_n = IR_PWM_DUTY_MIN;
	else
		HW_TIM_IR_PWM_OUT_ENA;
	HW_TIM_IR_PWM_UPD(duty_n);
	return(res);
}
/*** svc_hw_ir_calib **************************************************
* Parameters :	flg = TRUE to start, FALSE to continue, duty_val_ptr
* Return :		TRUE if done and result in duty_val (0xFF if FAILED)
* Warning: 		Do not activate interrupt handler's using HW_... of this function
**********************************************************************/
bool svc_hw_ir_calib(byte ind, bool flg, byte *duty_val_ptr)
{
	bool res = FALSE;
	bool act_flg = FALSE;
	byte duty_val, val, inc, tout;

	if (flg)
	{
		HW_IR_ON(ind);
		HW_IR_TX_ON;
		SHW.ir.st = 0, SHW.ir.cnt = 0, SHW.ir.rep_n = 0, val = 0;
		tout = IR_CALIB_STABLE_C_MS;
		act_flg = TRUE;
	}
	else if (SHW.ir.st == 0 || SHW.ir.st == 2)
	{
		if (svc_hw_tic_tst(TIC_IR_CALIB_ID))
		{
			act_flg = TRUE;
			inc = IR_CALIB_DUTY_INC_A, tout = IR_CALIB_STABLE_A_MS;
			if (!(SHW.ir.st == 0))
				inc = IR_CALIB_DUTY_INC_B, tout = IR_CALIB_STABLE_B_MS;
			if (!HW_IR_RX_TST)
			{
				DEBUG_PRINTF('A');
				SHW.ir.cnt += inc, val = SHW.ir.cnt;
				if (SHW.ir.cnt == 0)
					SHW.ir.st = 4, duty_val = 0xFF, val = 0;				// Failed OVERFLOW
			}
			else
			{
				DEBUG_PRINTF('B');
				HW_DEBUG_B_TOGGLE;
				if (SHW.ir.cnt > 0)
				{
					if (SHW.ir.st == 0)
					{
						SHW.ir.cnt -= inc, val = 0;
						SHW.ir.st = 1;
						tout = IR_CALIB_STABLE_C_MS;
					}
					else														// OK
					{
						duty_val = SHW.ir.cnt, val = SHW.ir.cnt;
						SHW.ir.st = 3;
					}
				}
				else
				{
					duty_val = 0x00, val = 0;									// Failed UNDERFLOW
					SHW.ir.st = 4;
				}
			}
		}
	}
	else if (SHW.ir.st == 1)											// Discharge pause
	{
		if (svc_hw_tic_tst(TIC_IR_CALIB_ID))
		{
			SHW.ir.st = 2;
			val = SHW.ir.cnt;
			tout = IR_CALIB_STABLE_B_MS;
			act_flg = TRUE;
		}
	}
	if (act_flg)
	{
		HW_DEBUG_A_TOGGLE;
		svc_hw_ir_pwm_upd(val);
		svc_hw_tic_set(TIC_IR_CALIB_ID, tout);
	}
	if (SHW.ir.st >= 3)
	{
		res = TRUE, *duty_val_ptr = duty_val;
		svc_hw_ir_pwm_upd(0);
		HW_IR_OFF;
		HW_IR_TX_OFF;
		HW_DEBUG_A_OFF;
		HW_DEBUG_B_OFF;
	}
	return(res);
}
/*** svc_hw_ir_adc_ins_ena ********************************************
* Parameters :	bool flg
* Return :		bool
**********************************************************************/
bool svc_hw_ir_adc_ins_ena(bool flg)
{
	bool res = FALSE;

	if (flg)
	{
		HW_TIM_ADC_INS(IR_LOG_ADC_FREQ_HZ);
		HW_TIM_ADC_INT_INS;
		HW_TIM_ADC_INT_PEND_CLR;
		HW_TIM_ADC_CLR;
		HW_TIM_ADC_INT_ENA;
		HW_TIM_ADC_ENA;
	}
	else
	{
		HW_TIM_ADC_INT_DIS;
		HW_TIM_ADC_DIS;
		HW_TIM_ADC_INT_UNINS;
		HW_TIM_ADC_UNINS;
	}
	return(res);
}
/*** HW_TIM_ADC_HANDLER ***********************************************
* Parameters :	void
* Return :		void
* Warning: 		Do not use handler's HW_... at background level simultaneously
**********************************************************************/
HW_TIM_ADC_HANDLER()
{
	word val;

	if (HW_TIM_ADC_INT_TST)
	{
		HW_TIM_ADC_INT_ACK;
		if (svc_adc_done_tst())
		{
			if (svc_adc_rd(ADC_FREE_OUT_AN_ID, &val))
			{
				if (BUF_CIRC_FREE_EVA(SYS.log_adc_wr_ind, SYS.log_adc_rd_ind, SIZEOF(SYS.log_adc_tbl)) > 0)
				{
					SYS.log_adc_tbl[SYS.log_adc_wr_ind] = val;
					VAR_INC_ROTATE(SYS.log_adc_wr_ind, SIZEOF(SYS.log_adc_tbl));
				}
				svc_adc_ena(TRUE);
			}
		}
	}
}
/*** svc_hw_ir ********************************************************
* Parameters :	flg, enable mask, repeat cycle n, pwm_duty_val_ptr
* Return :		TRUE if done
**********************************************************************/
bool svc_hw_ir(bool flg, byte ena_msk, byte rep_n, byte *pwm_duty_val_ptr)
{
	bool res = FALSE;
	byte i;

	if (flg)
	{
		for (i = 0, SHW.ir.ind_n = 0; i < IR_N; i++)
		{
			SHW.ir.pwm_duty_val_tbl[i] = *(pwm_duty_val_ptr + i);
			if ((ena_msk & (0x01 << i)) > 0)
			{
				SHW.ir.ind_tbl[SHW.ir.ind_n] = i;
				SHW.ir.res_on_cnt_tbl[SHW.ir.ind_n] = 0;
				SHW.ir.res_off_cnt_tbl[SHW.ir.ind_n] = 0;
				SHW.ir.ind_n++;
			}
		}
		SHW.ir.ind_cnt = 0;
		SHW.ir.st = 0, SHW.ir.cnt = 0, SHW.ir.rep_n = rep_n;
	}
	else
		res = (SHW.ir.cnt == IR_CNT_AUTO_STOP);
	return(res);
}
/*** svc_hw_ir_ins_ena ************************************************
* Parameters :	flg
* Return :		bool
**********************************************************************/
bool svc_hw_ir_ins_ena(bool flg)
{
	bool res = TRUE;

	if (flg)
	{
		//HW_TIM_IR_PWM_RELOAD_DIS;		/* _FDL_ */
		HW_TIM_IR_INS(IR_TIM_US_TO_HZ(IR_TIM_START_US));
		HW_TIM_IR_INT_INS;
		HW_TIM_IR_INT_PEND_CLR;
		HW_TIM_IR_CLR;
		HW_TIM_IR_INT_ENA;
		HW_TIM_IR_ENA;
	}
	else
	{
		HW_TIM_IR_INT_DIS;
		HW_TIM_IR_DIS;
		HW_TIM_IR_INT_UNINS;
		HW_TIM_IR_UNINS;
		HW_IR_TX_OFF;
		HW_IR_OFF;
		//HW_TIM_IR_PWM_RELOAD_ENA;		/* _FDL_ */
		svc_hw_ir_pwm_upd(0);
	}
	return(res);
}
/*** HW_TIM_IR_HANDLER ************************************************
* Parameters :	void
* Return :		void
* Warning: 		Do not use handler's HW_... at background level simultaneously
**********************************************************************/
HW_TIM_IR_HANDLER()
{
	byte ind;

	if (HW_TIM_IR_INT_TST)
	{
		HW_TIM_IR_UPD(sys.ir.freq_upd_tbl[SHW.ir.cnt]);
		HW_TIM_IR_INT_ACK;
		ind = SHW.ir.ind_tbl[SHW.ir.ind_cnt];
		if (SHW.ir.cnt == 0)
		{
			svc_hw_ir_pwm_upd(SHW.ir.pwm_duty_val_tbl[ind]);
			SHW.ir.cnt = 1;
		}
		else if (SHW.ir.cnt == 1)
		{
			HW_IR_ON(ind);
			SHW.ir.cnt = 2;
		}
		else if (SHW.ir.cnt == 2)
		{
			HW_IR_TX_ON;
			SHW.ir.cnt = 3;
		}
		else if (SHW.ir.cnt == 3)
		{
			if (HW_IR_RX_TST)
			{
				if (SHW.ir.res_on_cnt_tbl[SHW.ir.ind_cnt] < 0xFFFF)
					SHW.ir.res_on_cnt_tbl[SHW.ir.ind_cnt]++;
			}
			else
			{
				if (SHW.ir.res_off_cnt_tbl[SHW.ir.ind_cnt] < 0xFFFF)
					SHW.ir.res_off_cnt_tbl[SHW.ir.ind_cnt]++;
			}
			HW_IR_TX_OFF;
			SHW.ir.cnt = 4;
		}
		else if (SHW.ir.cnt == 4)
		{
			HW_IR_OFF;
			svc_hw_ir_pwm_upd(0);
			SHW.ir.cnt = 0;
			if (++SHW.ir.ind_cnt >= SHW.ir.ind_n)
			{
				SHW.ir.ind_cnt = 0;
				if (SHW.ir.rep_n > 0)					// rep_n == 0 then continuous
				{
					if (--SHW.ir.rep_n == 0)
					{
						HW_TIM_IR_INT_DIS;				// Automatically stopped
						SHW.ir.cnt = IR_CNT_AUTO_STOP;	// Signal stopped automatically
					}
				}
			}
		}
	}
}
/*** svc_hw_ir_res_rd *************************************************
* Parameters :	id, result pointer
* Return :		bool
**********************************************************************/
bool svc_hw_ir_res_rd(byte id, word *res_on_cnt_ptr, word *res_off_cnt_ptr)
{
	bool res = FALSE;
	byte i;

	for (i = 0; !res && i < SHW.ir.ind_n; i++)
		res = (id == SHW.ir.ind_tbl[i]);
	if (res)
		*res_on_cnt_ptr = SHW.ir.res_on_cnt_tbl[i - 1], *res_off_cnt_ptr = SHW.ir.res_off_cnt_tbl[i - 1];;
	return(res);
}
/*** svc_hw_ir_pwm_duty_wr ********************************************
* Parameters :	id, val
* Return :		bool
**********************************************************************/
bool svc_hw_ir_pwm_duty_wr(byte id, byte val)
{
	bool res = FALSE;

	res = (id < IR_N);
	if (res)
		SHW.ir.pwm_duty_val_tbl[id] = val;
	return(res);
}
/*** svc_hw_ir_pwm_duty_rd ********************************************
* Parameters :	void
* Return :		void
**********************************************************************/
bool svc_hw_ir_pwm_duty_rd(byte id, byte *val_ptr)
{
	bool res = FALSE;

	res = (id < IR_N);
	if (res)
		*val_ptr = SHW.ir.pwm_duty_val_tbl[id];
	return(res);
}
/*** svc_hw_us ********************************************************
* Parameters :	flg
* Return :		TRUE if done
**********************************************************************/
bool svc_hw_us(bool flg)
{
	bool res = FALSE;

	if (flg)
	{
		SHW.us.st = 0, SHW.us.cnt = 0;
	}
	else
	{
		res = (SHW.us.cnt == US_CNT_AUTO_STOP);
	}
	return(res);
}
/*** svc_hw_us_ins_ena ************************************************
* Parameters :	flg
* Return :		bool
**********************************************************************/
bool svc_hw_us_ins_ena(bool flg)
{
	bool res = TRUE;

	if (flg)
	{
		HW_DAC_INS;
		HW_COMP_INS;
		HW_US_AMP_SET(sys.us.amp_val);
		HW_TIM_US_PWM_INS;
		HW_TIM_US_PWM_CLR;
		HW_TIM_US_PWM_OUT_INS(US_PWM_DUTY_MIN);
		HW_TIM_US_PWM_CLR;
		HW_TIM_US_PWM_ENA;
		HW_TIM_US_PWM_INT_INS;
		HW_TIM_US_PWM_INT_PEND_CLR;
		HW_TIM_US_PWM_INT_ENA;
	}
	else
	{
		HW_TIM_US_PWM_INT_DIS;
		HW_TIM_US_PWM_DIS;
		HW_TIM_US_PWM_INT_UNINS;
		HW_TIM_US_PWM_UNINS;
	}
	return(res);
}
/*** HW_TIM_US_PWM_HANDLER *******************************************
* Parameters :	void
* Return :		void
* Warning: 		Do not use handler's HW_... at background level simultaneously
**********************************************************************/
HW_TIM_US_PWM_HANDLER()
{
	word val;

	if (HW_TIM_US_PWM_INT_TST)
	{
		HW_TIM_US_PWM_INT_ACK;
		if (SHW.us.cnt == 0)
			HW_US_POW_ENA;
		else if (SHW.us.cnt < sys.us.pow_stable_period_n)
			;
		else if (SHW.us.cnt == sys.us.pow_stable_period_n)
		{
			HW_TIM_US_PWM_OUT_ENA;
			HW_TIM_US_PWM_UPD(US_PWM_DUTY);
		}
		else if (SHW.us.cnt == (sys.us.pow_stable_period_n + sys.us.burst_period_n))
		{
			HW_TIM_US_PWM_OUT_DIS;
			HW_TIM_US_PWM_UPD(US_PWM_DUTY_MIN);
			HW_US_POW_DIS;
			HW_DAC_ENA;
			HW_DAC_VAL(sys.us.dac_val);
			HW_COMP_ENA;
		}
		else if (SHW.us.cnt < (sys.us.pow_stable_period_n + sys.us.burst_period_n + sys.us.dist_min_period_n))
			svc_adc_ena(TRUE);
		else if (SHW.us.cnt < (sys.us.pow_stable_period_n + sys.us.burst_period_n + sys.us.dist_max_period_n))
		{
			if (svc_adc_done_tst())
			{
				HW_DEBUG_A_ON;
				if (svc_adc_rd(ADC_ECHO_LEV_ID, &val))
				{
					SYS.log_adc_tbl[SYS.log_adc_wr_ind] = val;
					VAR_INC_ROTATE(SYS.log_adc_wr_ind, SIZEOF(SYS.log_adc_tbl));
					svc_adc_ena(TRUE);
				}
				HW_DEBUG_A_OFF;
			}
		}
		else
		{
			svc_adc_ena(FALSE);
			HW_TIM_US_PWM_INT_DIS;
			HW_TIM_US_PWM_DIS;
			HW_TIM_US_PWM_INT_UNINS;
			HW_TIM_US_PWM_UNINS;
			HW_DAC_VAL(0);
			HW_DAC_DIS;
			HW_COMP_DIS;
			SHW.us.cnt = US_CNT_AUTO_STOP - 1;
		}
		SHW.us.cnt++;
	}
}

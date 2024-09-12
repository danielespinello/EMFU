/*** APP.C ************************************************
 *
 *********************************************************/
#include "env_def.h"
#include "config.h"
#include "app_def.h"
#include "svc_def.h"
#include "sys_var.h"
/*** Definitions *****************************************/
/*** Constants *******************************************/
STATIC CONST byte ir_calib_cor_mul_tbl[] = IR_CALIB_COR_MUL_TBL;
STATIC CONST byte ir_calib_cor_div_tbl[] = IR_CALIB_COR_DIV_TBL;
STATIC CONST dword ir_freq_upd_tbl[IR_TIM_N] =	// Timing
{
	IR_TIM_US_TO_HZ(IR_TIM_A_US),
	IR_TIM_US_TO_HZ(IR_TIM_B_US),
	IR_TIM_US_TO_HZ(IR_TIM_C_US),
	IR_TIM_US_TO_HZ(IR_TIM_D_US),
	IR_TIM_US_TO_HZ(IR_TIM_E_US)
};
CONST word us_gain_tbl[] = US_GAIN_TBL;
CONST byte us_gain_msk_tbl[] = US_GAIN_MSK_TBL;
/** Types and prototypes *********************************/
/*** Variables *******************************************/
SYS_VAR_EXT;
APP_VAR;
/*** Functions *******************************************/
STATIC void var_init(void);
STATIC bool prot_proc(void);
STATIC bool exe_proc(void);
STATIC void debug_proc(void);
STATIC bool ir_exe(void);
STATIC bool ir_calib_exe(byte ind, bool *valid_flg_ptr, byte *val_ptr);
STATIC bool us_exe(void);
STATIC bool e2p_write(bool flg);
STATIC void word_to_quad_byte_nibble(byte *dptr, byte dsize, word val);
STATIC word quad_byte_nibble_to_word(byte *sptr, byte ssize);
STATIC byte us_gain_to_val_conv(word val);
STATIC byte us_dist_mm_to_period_n_conv(word val);
STATIC word us_period_n_to_dist_mm_conv(byte period_n);
STATIC word us_period_n_to_us_conv(byte period_n);
STATIC bool us_dist_eva(byte *period_n_ptr);
/*** app_main ********************************************
* Param:
* Return:
*********************************************************/
bool app_install(bool flg)
{
	if (flg)
	{
		STRUCT_SET(app, 0);
		svc_prot_install(TRUE);
	}
	return(TRUE);
}
/*** app_main ********************************************
* Param:
* Return:
*********************************************************/
void app_main(void)
{
	bool res;

	if (app.st == APP_ST_VOID)
	{
		sys.st = STATUS_PRESET;
		res = svc_e2p_read((word *)&sys.stp, (sizeof(sys.stp) / 2));
		if (!res || !(sys.stp.ver_id == STP_VER_ID))
		{
			sys.st = STATUS_STP_CHKSUM_ERR;
			svc_hw_stp_init();								// Setup init and write
			if (!e2p_write(FALSE))							// Parameter FALSE power on indicator
				sys.st = STATUS_STP_WR_FAULT;
		}
		var_init();
		svc_hw_tic_set(TIC_TOUT_ID, 1000);
		svc_hw_ir_pwm_ins(TRUE);
		app.st = APP_ST_IDLE;
	}
	else if (app.st == APP_ST_IDLE)
	{
		if (svc_hw_tic_tst(TIC_TOUT_ID))
		{
			svc_hw_tic_set(TIC_TOUT_ID, 1000);
			LED_TOGGLE;
			#if (DBG_MORENO)
			app.exe.us.st = 0, app.exe.us.cnt = 0;
			us_exe();
			#endif
		}
		if (prot_proc())							// TRUE on sys.stp change
		{
			if (!e2p_write(TRUE))					// Parameter TRUE run-time indicator
				sys.st = STATUS_STP_WR_FAULT;
		}
		if (exe_proc())								// TRUE on sys.stp change
		{
			if (!e2p_write(TRUE))					// Parameter TRUE run-time indicator
				sys.st = STATUS_STP_WR_FAULT;
		}
		if (sys.debug_ena_bit)
		{
			debug_proc();
		}
	}
}
/*** var_init ********************************************
* Param:	void
* Return:	void
*********************************************************/
void var_init(void)
{
	byte i;
									// WARNING: Do not use MEMCPY
	for (i = 0; i < SIZEOF(sys.ir.freq_upd_tbl); i++)
		sys.ir.freq_upd_tbl[i] = ir_freq_upd_tbl[i];
	sys.us.pow_stable_period_n = US_POW_STABLE_PERIOD_N;
	sys.us.burst_period_n = US_BURST_PERIOD_N;
	sys.us.dist_min_period_n = us_dist_mm_to_period_n_conv(sys.stp.us.dist_min_mm);
	sys.us.dist_max_period_n = us_dist_mm_to_period_n_conv(sys.stp.us.dist_max_mm);
	sys.us.amp_val = us_gain_to_val_conv(sys.stp.us.gain);
	sys.us.dac_val = US_DAC_VAL_DEF;
	sys.pwm_ir_a_mul = IR_PWM_A_CENT_DEF;
	app.exe.us.echo_val = MV_TO_ADC(sys.stp.us.echo_lev_mv);
	app.exe.us.hyst_val = MV_TO_ADC(sys.stp.us.hyst_lev_mv);
}
/*** us_gain_to_val_conv *********************************
* Param:	word gain
* Return:	Gain mask
*********************************************************/
byte us_gain_to_val_conv(word val)
{
	byte res = 0;
	byte i;

	for (i = 0; i < SIZEOF(us_gain_tbl); i++)
	{
		if (val == us_gain_tbl[i])
			break;
		if (i && val < us_gain_tbl[i])
		{
			i -= ((val - us_gain_tbl[i - 1]) < (us_gain_tbl[i] - val));
			break;
		}
	}
	if (i == SIZEOF(us_gain_tbl))
		i--;
	res = us_gain_msk_tbl[i];
	return(res);
}
/*** us_dist_mm_to_period_n_conv *************************
* Param:	word val
* Return:	period_n
*********************************************************/
byte us_dist_mm_to_period_n_conv(word val)
{
	byte res = 0;

	res = (byte)(((dword)val * 2UL * 1000UL + (SOUND_SPEED * US_CTRL_PERIOD_US / 2)) / (dword)(SOUND_SPEED * US_CTRL_PERIOD_US));
	return(res);
}
/*** us_period_n_to_dist_mm_conv *************************
* Param:	word val
* Return:	Gain mask
*********************************************************/
word us_period_n_to_dist_mm_conv(byte period_n)
{
	word res = 0;

	res = (word)(((dword)period_n * US_CTRL_PERIOD_US * (dword)SOUND_SPEED + 1000UL) / 2000UL);
	return(res);
}
/*** us_period_n_to_us_conv *****************************
* Param:	word val
* Return:	Gain mask
*********************************************************/
word us_period_n_to_us_conv(byte period_n)
{
	word res = 0;

	res = (word)(((dword)period_n * US_CTRL_PERIOD_US + 1UL) / 2UL);
	return(res);
}
/*** prot_proc *******************************************
* Param:	void
* Return:	bool TRUE on sys.stp written
*********************************************************/
bool prot_proc(void)
{
	bool res = FALSE;
	byte b, i, len;
	byte ir_limit;
	word val;
	dword dw;
	byte buf[PROT_DATA_SIZE + 8];

	if (svc_prot_cmd_get(buf, sizeof(buf), &len))
	{
		if (buf[0] == PROT_STATUS_CMD)
		{
			buf[0] = sys.st;
			svc_prot_cmd_put(buf, 1);
		}
		else if (buf[0] == PROT_SET_CMD)
		{
			if (buf[1] == PROT_SET_PAR_IR_MAN_START || buf[1] == PROT_SET_PAR_IR_AUTO)
			{
				if (app.exe.ir.ena_msk)
				{
					app.exe.ir.run_bit = TRUE;
					app.exe.ir.st = 0, app.exe.ir.cnt = 0;
					app.exe.ir.rep_n = IR_RUN_AUTO_REP_N * (buf[1] == PROT_SET_PAR_IR_AUTO);
					for (i = 0; i < IR_N; i++)
					{
						app.exe.ir.res_on_cnt_tbl[i] = 0, app.exe.ir.res_off_cnt_tbl[i] = 0;
#ifdef REWORK_TERZI
						app.exe.ir.duty_val_tbl[i] = (sys.stp.ir.k * sys.stp.ir.pwm_tbl[i].adj_empty_val) / 100;
#else
						app.exe.ir.duty_val_tbl[i] = (sys.stp.ir.k * sys.stp.ir.pwm_tbl[i].adj_empty_val + 50) / 100;
#endif
						app.exe.ir.saturation_tbl[i] = FALSE;
#ifdef REWORK_TERZI
						ir_limit = (sys.pwm_ir_a_mul * sys.stp.ir.pwm_tbl[i].calib_empty_val) / 100;
						if (app.exe.ir.duty_val_tbl[i] > ir_limit)
							app.exe.ir.saturation_tbl[i] = TRUE;
#else
						if (app.exe.ir.duty_val_tbl[i] > sys.stp.ir.pwm_tbl[i].calib_full_val)
						{
							app.exe.ir.duty_val_tbl[i] = sys.stp.ir.pwm_tbl[i].calib_full_val;
							app.exe.ir.saturation_tbl[i] = TRUE;
						}
#endif
					}
				}
			}
			else if (buf[1] == PROT_SET_PAR_IR_MAN_STOP)
			{
				if (app.exe.ir.run_bit)
					app.exe.ir.stop_req_bit = TRUE;
			}
			else if (buf[1] == PROT_SET_PAR_IR_CALIB_EMPTY)
			{
				if (IR_MSK_MUTEX_TST(app.exe.ir.ena_msk))
				{
					app.exe.ir.calib_empty_run_bit = TRUE;
					app.exe.ir.st = 0, app.exe.ir.cnt = 0;
				}
			}
			else if (buf[1] == PROT_SET_PAR_IR_CALIB_FULL)
			{
				if (IR_MSK_MUTEX_TST(app.exe.ir.ena_msk))
				{
					app.exe.ir.calib_full_run_bit = TRUE;
					app.exe.ir.st = 0, app.exe.ir.cnt = 0;
				}
			}
			else if (buf[1] == PROT_SET_PAR_IR_ADJ_EMPTY)
			{
				if (IR_MSK_MUTEX_TST(app.exe.ir.ena_msk))
				{
					app.exe.ir.adj_empty_run_bit = TRUE;
					app.exe.ir.st = 0, app.exe.ir.cnt = 0;
				}
			}
			else if(buf[1] == PROT_SET_PAR_US_AUTO)
			{
				app.exe.us.run_bit = TRUE;
				app.exe.us.st = 0, app.exe.us.cnt = 0;
				app.exe.us.meas_cnt = 0, app.exe.us.meas_dist_period_n_acc = 0, app.exe.us.meas_valid_cnt = 0;
			}
			else if (buf[1] == PROT_SET_PAR_DLL_EXE)
			{
				sys.download_reboot_val = DOWNLOAD_REBOOT_VAL;
			}
			else if (buf[1] == PROT_SET_PAR_DEBUG_ON)
			{
				sys.debug_ena_bit = TRUE;
			}
			else if (buf[1] == PROT_SET_PAR_DEBUG_OFF)
			{
				sys.debug_ena_bit = FALSE;
			}
		}
		else if (buf[0] == PROT_WR_CMD)
		{
			if (buf[1] == PROT_WR_PAR_IR_ENA_MSK)
				app.exe.ir.ena_msk = (buf[5] & ((0x01 << IR_N) - 1));
			else if (buf[1] == PROT_WR_PAR_IR_ADJ_A || buf[1] == PROT_WR_PAR_IR_ADJ_B || buf[1] == PROT_WR_PAR_IR_ADJ_C)
			{
				val = quad_byte_nibble_to_word(buf + 2, PROT_DATA_SIZE);
				i = IR_A_ID;
				if (!(buf[1] == PROT_WR_PAR_IR_ADJ_A))
					i = (buf[1] == PROT_WR_PAR_IR_ADJ_B) ? IR_B_ID : IR_C_ID;
				res = !(val == sys.stp.ir.pwm_tbl[i].adj_empty_val);
				if (res)
				sys.stp.ir.pwm_tbl[i].adj_empty_val = val;
			}
			else if (buf[1] == PROT_WR_PAR_IR_K)
			{
				val = quad_byte_nibble_to_word(buf + 2, PROT_DATA_SIZE);
				res = !(val == sys.stp.ir.k);
				if (res)
				sys.stp.ir.k = val;
			}
			else if (buf[1] >= PROT_WR_PAR_IR_TA && buf[1] <= PROT_WR_PAR_IR_TE)
			{
				b = buf[1] - PROT_WR_PAR_IR_TA;
				val = quad_byte_nibble_to_word(buf + 2, PROT_DATA_SIZE);
				dw = IR_TIM_US_TO_HZ(val);
				sys.ir.freq_upd_tbl[b] = dw;
			}
			else if (buf[1] == PROT_WR_PAR_US_GAIN)
			{
				val = quad_byte_nibble_to_word(buf + 2, PROT_DATA_SIZE);
				res = !(val == sys.stp.us.gain);
				if (res)
					sys.stp.us.gain = val;
				sys.us.amp_val = us_gain_to_val_conv(sys.stp.us.gain);
			}
			else if (buf[1] == PROT_WR_PAR_US_DIST_MIN_MM)
			{
				val = quad_byte_nibble_to_word(buf + 2, PROT_DATA_SIZE);
				res = !(val == sys.stp.us.dist_min_mm);
				if (res)
					sys.stp.us.dist_min_mm = val;
				sys.us.dist_min_period_n = us_dist_mm_to_period_n_conv(sys.stp.us.dist_min_mm);
			}
			else if (buf[1] == PROT_WR_PAR_US_DIST_MAX_MM)
			{
				val = quad_byte_nibble_to_word(buf + 2, PROT_DATA_SIZE);
				res = !(val == sys.stp.us.dist_max_mm);
				if (res)
					sys.stp.us.dist_max_mm = val;
				sys.us.dist_max_period_n = us_dist_mm_to_period_n_conv(sys.stp.us.dist_max_mm);
			}
			else if (buf[1] == PROT_WR_PAR_US_ECHO_LEV)
			{
				val = quad_byte_nibble_to_word(buf + 2, PROT_DATA_SIZE);
				res = !(val == sys.stp.us.echo_lev_mv);
				if (res)
					sys.stp.us.echo_lev_mv = val;
				app.exe.us.echo_val = MV_TO_ADC(val);
			}
			else if (buf[1] == PROT_WR_PAR_US_HYST_LEV)
			{
				val = quad_byte_nibble_to_word(buf + 2, PROT_DATA_SIZE);
				res = !(val == sys.stp.us.hyst_lev_mv);
				if (res)
					sys.stp.us.hyst_lev_mv = val;
				app.exe.us.hyst_val = MV_TO_ADC(val);
			}
		}
		else if (buf[0] == PROT_RD_CMD)
		{
			if (buf[1] == PROT_RD_PAR_IR_MAN || buf[1] == PROT_RD_PAR_IR_AUTO)
			{
				buf[0] = 0;
				for (i = 0; i < IR_N; i++)
				{
					b = 0;
					if (app.exe.ir.saturation_tbl[i])
						b = STATUS_IR_SATURATION;
					else if (app.exe.ir.res_on_cnt_tbl[i] == 0 && app.exe.ir.res_off_cnt_tbl[i] > 0)
						b = STATUS_IR_FULL;
					else if (app.exe.ir.res_on_cnt_tbl[i] > 0 && app.exe.ir.res_off_cnt_tbl[i] == 0)
						b = STATUS_IR_EMPTY;
					buf[1 + i] = b;
				}
				svc_prot_cmd_put(buf, PROT_DATA_SIZE);
			}
			else if (buf[1] == PROT_RD_PAR_IR_CALIB_EMPTY)
			{
				buf[0] = 0;
				buf[1] = sys.stp.ir.pwm_tbl[0].calib_empty_val;
				buf[2] = sys.stp.ir.pwm_tbl[1].calib_empty_val;
				buf[3] = sys.stp.ir.pwm_tbl[2].calib_empty_val;
				svc_prot_cmd_put(buf, PROT_DATA_SIZE);
			}
			else if (buf[1] == PROT_RD_PAR_IR_CALIB_FULL)
			{
				buf[0] = 0;
				buf[1] = sys.stp.ir.pwm_tbl[0].calib_full_val;
				buf[2] = sys.stp.ir.pwm_tbl[1].calib_full_val;
				buf[3] = sys.stp.ir.pwm_tbl[2].calib_full_val;
				svc_prot_cmd_put(buf, PROT_DATA_SIZE);
			}
			else if (buf[1] == PROT_RD_PAR_IR_ADJ_EMPTY)
			{
				buf[0] = 0;
				buf[1] = sys.stp.ir.pwm_tbl[0].adj_empty_val;
				buf[2] = sys.stp.ir.pwm_tbl[1].adj_empty_val;
				buf[3] = sys.stp.ir.pwm_tbl[2].adj_empty_val;
				svc_prot_cmd_put(buf, PROT_DATA_SIZE);
			}
			else if (buf[1] == PROT_RD_PAR_IR_K)
			{
				word_to_quad_byte_nibble(buf, PROT_DATA_SIZE, sys.stp.ir.k);
				svc_prot_cmd_put(buf, PROT_DATA_SIZE);
			}
			else if (buf[1] == PROT_RD_PAR_US_GAIN)
			{
				word_to_quad_byte_nibble(buf, PROT_DATA_SIZE, sys.stp.us.gain);
				svc_prot_cmd_put(buf, PROT_DATA_SIZE);
			}
			else if (buf[1] == PROT_RD_PAR_US_DIST_MIN_MM)
			{
				word_to_quad_byte_nibble(buf, PROT_DATA_SIZE, sys.stp.us.dist_min_mm);
				svc_prot_cmd_put(buf, PROT_DATA_SIZE);
			}
			else if (buf[1] == PROT_RD_PAR_US_DIST_MAX_MM)
			{
				word_to_quad_byte_nibble(buf, PROT_DATA_SIZE, sys.stp.us.dist_max_mm);
				svc_prot_cmd_put(buf, PROT_DATA_SIZE);
			}
			else if (buf[1] == PROT_RD_PAR_US_ECHO_LEV)
			{
				word_to_quad_byte_nibble(buf, PROT_DATA_SIZE, sys.stp.us.echo_lev_mv);
				svc_prot_cmd_put(buf, PROT_DATA_SIZE);
			}
			else if (buf[1] == PROT_RD_PAR_US_HYST_LEV)
			{
				word_to_quad_byte_nibble(buf, PROT_DATA_SIZE, sys.stp.us.hyst_lev_mv);
				svc_prot_cmd_put(buf, PROT_DATA_SIZE);
			}
			else if (buf[1] == PROT_RD_PAR_US_AUTO)
			{
				word_to_quad_byte_nibble(buf, PROT_DATA_SIZE, (app.exe.us.dist_period_n > 0));
				svc_prot_cmd_put(buf, PROT_DATA_SIZE);
			}
			else if (buf[1] == PROT_RD_PAR_US_DIST_US)
			{
				val = us_period_n_to_us_conv(app.exe.us.dist_period_n);
				word_to_quad_byte_nibble(buf, PROT_DATA_SIZE, val);
				svc_prot_cmd_put(buf, PROT_DATA_SIZE);
			}
			else if (buf[1] == PROT_RD_PAR_US_DIST_MM)
			{
				val = us_period_n_to_dist_mm_conv(app.exe.us.dist_period_n);
				word_to_quad_byte_nibble(buf, PROT_DATA_SIZE, val);
				svc_prot_cmd_put(buf, PROT_DATA_SIZE);
			}
			else if (buf[1] == PROT_RD_PAR_VER)
			{
				buf[0] = (byte)(APP_VERSION / 0x1000000);
				buf[1] = (byte)(APP_VERSION / 0x10000);
				buf[2] = (byte)(APP_VERSION / 0x100);
				buf[3] = (byte)(APP_VERSION);
				svc_prot_cmd_put(buf, PROT_DATA_SIZE);
			}
			else if (buf[1] == PROT_RD_PAR_LOG)
			{
				for (; !(sys.log_adc_rd_ind == sys.log_adc_wr_ind); )
				{
					val = sys.log_adc_tbl[sys.log_adc_rd_ind];
					for (; !PROT_CRT_PUT((byte)(val / 0x100)); )
						;
					for (; !PROT_CRT_PUT((byte)val); )
						;
					VAR_INC_ROTATE(sys.log_adc_rd_ind, SIZEOF(sys.log_adc_tbl));
				}
			}
			else if (buf[1] == PROT_RD_PAR_SYS)
			{
				for (i = 0; i < IR_N; i++)
				{
					svc_hw_ir_pwm_duty_rd(i, &b);
					for (; !PROT_CRT_PUT(b); )
						;
				}
			}
		}
	}
	return(res);
}
/*** word_to_quad_byte_nibble ****************************
* Param:	dptr, size, value
* Return:	void
*********************************************************/
void word_to_quad_byte_nibble(byte *dptr, byte dsize, word val)
{
	if (dsize >= 4)
	{
		dptr[0] = (byte)(val / 0x1000);
		dptr[1] = (byte)((val & 0xFFF) / 0x100);
		dptr[2] = (byte)((val & 0xFF) / 0x10);
		dptr[3] = (byte)(val % 0x10);
	}
}
/*** quad_byte_nibble_to_word ****************************
* Param:	dptr, size, value
* Return:	word
*********************************************************/
word quad_byte_nibble_to_word(byte *sptr, byte ssize)
{
	word val = 0;

	if (ssize >= 4)
		val = (sptr[0] & 0x0F) * 0x1000 + (sptr[1] & 0x0F) * 0x100 + (sptr[2] & 0x0F) * 0x10 + (sptr[3] & 0x0F);
	return(val);
}
/*** exe_proc ********************************************
* Param:	void
* Return:	bool TRUE on sys.stp write
*********************************************************/
bool exe_proc(void)
{
	bool res = FALSE;
	byte i, ind, val;

	if (app.exe.ir.run_bit)
	{
		if (!ir_exe())											// FALSE on completion with valid_flg and value
		{
			app.exe.ir.run_bit = FALSE;
			for (i = 0; i < IR_N; i++)
				svc_hw_ir_res_rd(i, app.exe.ir.res_on_cnt_tbl + i, app.exe.ir.res_off_cnt_tbl + i);
		}
	}
	else if (app.exe.ir.calib_empty_run_bit)
	{
		IR_MSK_MUTEX_TO_IND(ind, app.exe.ir.ena_msk);
		if (!ir_calib_exe(ind, &res, &val))				// FALSE on completion with valid_flg and value
		{
			app.exe.ir.calib_empty_run_bit = FALSE;
			if (res)
				sys.stp.ir.pwm_tbl[ind].calib_empty_val = val, sys.stp.ir.pwm_tbl[ind].adj_empty_val = val;
		}
	}
	else if (app.exe.ir.calib_full_run_bit)
	{
		IR_MSK_MUTEX_TO_IND(ind, app.exe.ir.ena_msk);
		if (!ir_calib_exe(ind, &res, &val))				// FALSE on completion with valid_flg and value
		{
			app.exe.ir.calib_full_run_bit = FALSE;
			if (res)
				sys.stp.ir.pwm_tbl[ind].calib_full_val = val;
		}
	}
	else if (app.exe.ir.adj_empty_run_bit)
	{
		IR_MSK_MUTEX_TO_IND(ind, app.exe.ir.ena_msk);
		if (!ir_calib_exe(ind, &res, &val))				// FALSE on completion with valid_flg and value
		{
			app.exe.ir.adj_empty_run_bit = FALSE;
			if (res)
				sys.stp.ir.pwm_tbl[ind].adj_empty_val = val;
		}
	}
	else if (app.exe.us.run_bit)
	{
		if (!us_exe())											// FALSE on completion with valid_flg and value
		{
			app.exe.us.run_bit = FALSE;
		}
	}
	return(res);
}
/*** ir_exe **********************************************
* Param:
* Return:	bool (FALSE to leave)
*********************************************************/
bool ir_exe(void)
{
	bool res = FALSE;

	if (app.exe.ir.st == 0)
	{												// par: TRUE to start return TRUE if done
		res = svc_hw_ir((app.exe.ir.cnt == 0), app.exe.ir.ena_msk, app.exe.ir.rep_n, app.exe.ir.duty_val_tbl);
		if (app.exe.ir.cnt == 0)
		{
			sys.log_adc_rd_ind = sys.log_adc_wr_ind = 0;
			svc_hw_ir_adc_ins_ena(TRUE);			// 1/2 Must be first
			svc_adc_ena(TRUE);						// 2/2
			svc_hw_ir_ins_ena(TRUE);
			sys.st = STATUS_BUSY, app.exe.ir.cnt = 1;
		}
		if (!res)
			res = app.exe.ir.stop_req_bit;		// Manual stop
	}
	if (res)										// Stop
	{
		svc_adc_ena(FALSE);
		svc_hw_ir_adc_ins_ena(FALSE);
		svc_hw_ir_ins_ena(FALSE);
		sys.st = STATUS_READY;
		app.exe.ir.stop_req_bit = FALSE;
	}
	return(!res);
}
/*** ir_calib_exe ****************************************
* Param:	LED index, valid_flg, val_ptr
* Return:	bool (FALSE to leave)
*********************************************************/
bool ir_calib_exe(byte ind, bool *valid_flg_ptr, byte *val_ptr)
{
	bool res = FALSE;
	bool valid_flg = FALSE;
	bool flg;
	byte duty_val;

	if (app.exe.ir.st == 0)
	{
		res = svc_hw_ir_calib(ind, (app.exe.ir.cnt == 0), &duty_val);	// par: TRUE to start return TRUE if done
		if (app.exe.ir.cnt == 0)
		{
			sys.log_adc_rd_ind = sys.log_adc_wr_ind = 0;
			svc_hw_ir_adc_ins_ena(TRUE);								// 1/2 Must be first
			svc_adc_ena(TRUE);											// 2/2
			app.exe.ir.cnt = 1;
			sys.st = STATUS_BUSY;
		}
		if (res)
		{
			flg = (!(duty_val == 0 || duty_val == 0xFF));
			if (flg)
			{
				svc_hw_ir_pwm_duty_wr(ind, duty_val);					// Raw value during calibration
				valid_flg = TRUE;
				sys.st = STATUS_READY;
				DEBUG_PRINTF(duty_val / 100 + '0');
				DEBUG_PRINTF((duty_val % 100) / 10 + '0');
				DEBUG_PRINTF((duty_val % 10) + '0');
			}
			sys.st = (flg) ? STATUS_READY : STATUS_CALIB_KO;
		}
		if (res)
		{
			svc_adc_ena(FALSE);
			svc_hw_ir_adc_ins_ena(FALSE);
		}
	}
	if (valid_flg)
		*val_ptr = duty_val;
	*valid_flg_ptr = valid_flg;
	return(!res);
}
/*** us_exe **********************************************
* Param:
* Return:	bool (FALSE to leave)
*********************************************************/
bool us_exe(void)
{
	bool res = FALSE;
	byte period_n;

	if (app.exe.us.st == 0)							// Start single measure
		{
		svc_hw_tic_set(TIC_US_MEAS_ID, US_MEAS_PERIOD_MS);
		svc_hw_us(TRUE);							// ... start mode
			sys.log_adc_rd_ind = sys.log_adc_wr_ind = 0;
			svc_hw_us_ins_ena(TRUE);
		sys.st = STATUS_BUSY;
		app.exe.us.st = 1;
	}
	else if (app.exe.us.st == 1)					// Wait for measure conmpletion
	{
		res = svc_hw_us(FALSE);						// ... run mode ... returns TRUE if done
		if (res)
		{
			svc_hw_us_ins_ena(FALSE);
			period_n = 0;
			if (us_dist_eva(&period_n))
				app.exe.us.meas_dist_period_n_acc += period_n, app.exe.us.meas_valid_cnt++;
			app.exe.us.st = 2;
			res = FALSE;
		}
	}
	else if (app.exe.us.st == 2)					// Wait for period completion
	{
		if (svc_hw_tic_tst(TIC_US_MEAS_ID))
		{
			app.exe.us.meas_cnt++;
			if (app.exe.us.meas_cnt < US_MEAS_N)
				app.exe.us.st = 0;
			else
			{
				if (US_MEAS_N > 0 && app.exe.us.meas_valid_cnt >=  US_MEAS_N)
					app.exe.us.dist_period_n = (app.exe.us.meas_dist_period_n_acc + app.exe.us.meas_cnt / 2) / app.exe.us.meas_valid_cnt;
				else
					app.exe.us.dist_period_n = 0;
				sys.st = STATUS_READY;
				res = TRUE;
			}
		}
	}
	return(!res);
}
/*** us_dist_eva *****************************************
* Param:	period_n_ptr
* Return:	bool
*********************************************************/
bool us_dist_eva(byte *period_n_ptr)
{
	bool res = FALSE;
	byte st, i, period_n;
	word val;

	for (st = 0, i = 0; !(sys.log_adc_rd_ind == sys.log_adc_wr_ind); i++)
	{
		val = sys.log_adc_tbl[sys.log_adc_rd_ind];
		VAR_INC_ROTATE(sys.log_adc_rd_ind, SIZEOF(sys.log_adc_tbl));
		if (st == 0)
		{
			if (val > (app.exe.us.echo_val + app.exe.us.hyst_val))
				st = 1, res = TRUE, period_n = i + 1 + sys.us.dist_min_period_n;
		}
	}
	if (res)
		*period_n_ptr = period_n;
	return(res);
}
/*** e2p_write *******************************************
* Param:	bool TRUE run-time, FALSE power on
* Return:	bool
*********************************************************/
bool e2p_write(bool flg)
{
	bool res = FALSE;

	for (; !PROT_EMPTY_TST(); )
		;
	PROT_ENA(FALSE);
	svc_hw_int_ena(FALSE);
	res = svc_e2p_write((word *)&sys.stp, (sizeof(sys.stp) / 2));
	svc_hw_int_ena(TRUE);
	PROT_ENA(TRUE);
	return(res);
}
/*** debug_proc ******************************************
* Param:	void
* Return:	void
*********************************************************/
void debug_proc(void)
{
}

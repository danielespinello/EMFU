/*** CONFIG.H *********************************************************
*  Description : Configurations
*  Version :    	0.0
***********************************************************************/
/*** General macros ***************************************************/
#define REWORK_TERZI
#define	APP_CHECK_VAL				(0x12345677)			// Product ID
#ifdef REWORK_TERZI
	#define APP_VERSION					(0x4C323130)			// Version ID L2.. ogni byte < 0x80
#else
	#define APP_VERSION					(0x4C323039)			// Version ID L2.. ogni byte < 0x80
#endif
#define HW_LEV						(0)						// 0 proto 1 production
#define STP_VER_ID					(0)						// STP Version
#define DEBUG_TIMING				(1)
#define I2C_ENA						(1)
#define DBG_MORENO					(0)
#if (I2C_ENA)
#define PROT_ENA(x)					svc_i2c_ena(x)
#define PROT_CRT_PUT(x)				svc_i2c_crt_put(x)
#define PROT_CRT_GET(x_ref)			svc_i2c_crt_get(x_ref)
#define PROT_EMPTY_TST()			svc_i2c_tx_empty_tst()
#else
#define PROT_ENA(x)					svc_rsa_ena(x)
#define PROT_CRT_PUT(x)				svc_rsa_crt_put(x)
#define PROT_CRT_GET(x_ref)			svc_rsa_crt_get(x_ref)
#define PROT_EMPTY_TST()			svc_rsa_tx_empty_tst()
#endif
#define DEBUG_PRINTF(x)				// for (; !PROT_CRT_PUT(x); )
#define DEBUG_BUF_SIZE				(0)
#define LED_ON						GPIOA->BSRR = GPIO_Pin_12
#define LED_OFF						GPIOA->BRR = GPIO_Pin_12
#define LED_TOGGLE					GPIOA->BSRR = (GPIO_Pin_12 << (dword)(16UL * ((GPIOA->ODR & GPIO_Pin_12) > 0)))
#if (DEBUG_TIMING)
#define HW_DEBUG_A_ON				GPIOA->BSRR = GPIO_Pin_8
#define HW_DEBUG_A_OFF				GPIOA->BRR = GPIO_Pin_8
#define HW_DEBUG_A_TOGGLE			GPIOA->BSRR = (GPIO_Pin_8 << (dword)(16UL * ((GPIOA->ODR & GPIO_Pin_8) > 0)))
#define HW_DEBUG_B_ON				GPIOA->BSRR = GPIO_Pin_11
#define HW_DEBUG_B_OFF				GPIOA->BRR = GPIO_Pin_11
#define HW_DEBUG_B_TOGGLE			GPIOA->BSRR = (GPIO_Pin_11 << 16UL) | (((dword)~(dword)(GPIOA->ODR & GPIO_Pin_11)) & GPIO_Pin_11)
#else	// DEBUG_TIMING
#define HW_DEBUG_A_ON
#define HW_DEBUG_A_OFF
#define HW_DEBUG_A_TOGGLE
#define HW_DEBUG_B_ON
#define HW_DEBUG_B_OFF
#define HW_DEBUG_B_TOGGLE
#endif	// DEBUG_TIMING
/*** Configuration parameters *****************************************/
// Calibration IR
#define IR_CALIB_COR_MUL_TBL	{1, 1, 1}					// Tabella moltiplicatori correzione per ogni LED
#define IR_CALIB_COR_DIV_TBL	{1, 1, 1}					// Tabella divisori correzione per ogni LED
#define IR_CALIB_MAX_LEV		(0xFF)						// Soglia massima raggiungibile
/*
duty_val = (duty_val >= IR_CALIB_MAX_LEV)
		 ? 0xFF	: (duty_val * ir_calib_cor_mul_tbl[0] / ir_calib_cor_div_tbl[0]);
svc_hw_ir_pwm_upd(duty_val);
*/
//
#define IR_CALIB_DUTY_INC_A		(256 / 16)					// IR led duty increment (sub multiple of 256)
#define IR_CALIB_STABLE_A_MS	(6)							// IR led stable MS during phase A (Incremento ampio)
#define IR_CALIB_STABLE_B_MS	(4)							// IR led stable MS during phase B (incremento fine)
#define IR_CALIB_STABLE_C_MS	(10)						// IR led stable MS during phase C (Recovery ... discharge)
#define IR_CALIB_DUTY_INC_B		(1)							// IR led duty increment (Must be 1)
#define LOG_ADC_N				(2048)						// Log adc size
#define IR_LOG_ADC_FREQ_HZ		(10000)						// Freq in HZ (IR) Fixed
/*** Installation  parameters *****************************************/
// ADC 10 bit
#define MV_TO_ADC(mv)			(((dword)(mv) * 1023UL + 1650UL) / 3300UL)
#define ADC_TO_MV(val)			(((dword)(val) * 3300UL + 512UL) / 1023UL)
// DAC 12 bit
#define MV_TO_DAC(mv)			(((dword)(mv) * 4095UL + 1650UL) / 3300UL)
#define DAC_TO_MV(val)			(((dword)(val) * 3300UL + 2048UL) / 4095UL)
//
#define	TIC_FREQ					(1000)
enum
{
	TIC_LED_ID = 0,
	TIC_TOUT_ID,
	TIC_IR_CALIB_ID,
	TIC_US_MEAS_ID,
	TIC_N
};
// IR leds
enum
{
	IR_A_ID = 0,
	IR_B_ID = 1,
	IR_C_ID = 2,
	IR_N
};
#define IR_MSK_MUTEX_TST(x)				((x) == 0x01 || (x) == 0x02 || (x) == 0x04)
#define IR_MSK_MUTEX_TO_IND(ind, x)		ind = (x) - 1 - (x) / 4
// Timing
#define IR_TIM_START_US					(100)				// Startup
#define IR_TIM_A_US						(6000)				// PWM stable
#define IR_TIM_B_US						(100)				// VSEL to VBURST
#define IR_TIM_C_US						(2000)				// VBURST
#define IR_TIM_D_US						(100)				// VBURST to VSEL
#define IR_TIM_E_US						(500)				// Recovery
#define IR_TIM_N						(5)
#define IR_TIM_US_TO_HZ(t)				((1000000UL + (dword)((dword)(t) / 2UL)) / (dword)(t))
/*** prog.c ***********************************************************/
void prog_main(void);
/*** app.c ************************************************************/
/*** svc_i2c.c ********************************************************/
#define I2C_SLA_GRP_1  				(1)					// Slave generico
#define I2C_SLA_GRP_2  				(0)					// Indirizzo A2..A0
// Small buffers
typedef byte t_i2c_tx_ind;
typedef byte t_i2c_rx_ind;
#define I2C_TX_BUF_SIZE				(64)
#define I2C_RX_BUF_SIZE				(64)
/*** svc_rsa.c ********************************************************/
#define RSA_RATE					(57600)
#define RSA_PARITY					'N'
#define RSA_BIT_N					'8'
#define RSA_STOP_N					'1'
// Small buffers
typedef byte t_rsa_tx_ind;
typedef byte t_rsa_rx_ind;
#define RSA_TX_BUF_SIZE				(64)
#define RSA_RX_BUF_SIZE				(64)
/*** svc_hw.c *********************************************************/
enum
{
	ADC_AGND_ID = 0,				// ADC_0
	ADC_ECHO_ID = 1,				// ADC_1
	ADC_ECHO_LEV_ID = 2,			// ADC_2
	ADC_FREE_OUT_AN_ID = 3,			// ADC_3
#if (DEBUG_TIMING)
	ADC_N,
#endif	// DEBUG_TIMING
	ADC_HW_LEV_A = 4,				// ADC_8 In jumper resistivo per FW spec.
	ADC_HW_LEV_B = 5,				// ADC_11 In jumper resistivo per FW spec.
#if (DEBUG_TIMING)
#else //DEBUG_TIMING
	ADC_N
#endif	// DEBUG_TIMING
};
#define IR_CTRL_TIC_FREQ			(4800000)	// Frequenza micro tic ctrl LED 1/2
#define IR_CTRL_PERIOD				(480)		// 480 = 100usec circa alias 10KHz 2/2
#define IR_PWM_DUTY_MIN				(0x01)		// Must be different from zero
//
#define SOUND_SPEED					(346)		// Sound speed m/s
#define US_CTRL_TIC_FREQ			(9600000)	// Frequenza micro tic ctrl 1/3
#define US_CTRL_PERIOD				(240)		// 240 = 25usec alias 40KHz 2/3
#define US_CTRL_PERIOD_US			(25)		// 25us 3/3
#define US_LOG_ADC_FREQ_HZ			(1000000UL / US_CTRL_PERIOD_US)
#define US_PWM_DUTY_MIN				(0x01)		// Must be different from zero
#define US_POW_STABLE_PERIOD_N		(8)			// Power stable pause (in PWM cycles 200usec @ 40KHz) 1 / 4
#define US_BURST_PERIOD_N			(8)			// Burst PWM cycles ( 200usec @ 40KHz) 2 / 4
#define US_DIST_MIN_PERIOD_N		(32)		// Distance min (in PWM cycles 800usec @ 40KHz) 3 / 4
#define US_DIST_MAX_PERIOD_N		(32 + 22)	// Distance max (in PWM cycles 550usec @ 40KHz) 4 / 4
#define US_PWM_DUTY					(128)		// Duty cycle in n/256
#define US_GAIN_DEF					(32)		// Gain
#define US_DIST_MIN_MM_DEF			(68)		// Dist MIN mm
#define US_DIST_MAX_MM_DEF			(1000)		// Dist MAX mm
#define US_ECHO_LEV_MV_DEF			(1500)		// Echo lev mV
#define US_HYST_LEV_MV_DEF			(0)			// Hysteresis delta lev mV
#define US_DAC_VAL_DEF				(0)			// DAC value
// Repeated measures
#define US_MEAS_N					(1)			// Measures number (>= 1)
#define US_MEAS_PERIOD_MS			(10)		// Measure period
#if (HW_LEV == 0)
// Ampli gain model 1 and 2
#define US_GAIN_TBL \
{ \
	0, 1, 2, 4, 5, 8, 10, 16, 20, 32, \
	40, 50, 64, 80, 100, 128, 160, 200, 320, 400, \
	640, 800, 1280, 1600, 3200, 6400 \
}
#define US_GAIN_MSK_TBL \
{ \
	0x00, 0x09, 0x0A, 0x0B, 0x19, 0x0C, 0x1A, 0x0D, 0x1B, 0x0E, \
	0x1C, 0x31, 0x0F, 0x1D, 0x32, 0x17, 0x1E, 0x33, 0x1F, 0x34, \
	0x27, 0x35, 0x2F, 0x36, 0x37, 0x3F \
}
#else
// Ampli gain model 1 and 3
#define US_GAIN_TBL \
{ \
	0, 1, 2, 3, 4, 5, 6, 7, 8, 10, \
	12, 14, 15, 20, 25, 30, 35, 40, 50, 60, \
	70,	80, 100, 120, 140, 150, 200, 250, 300, 350, \
	400, 500, 600, 700 \
}
#define US_GAIN_MSK_TBL \
{ \
	0x00, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x14, 0x15, \
	0x16, 0x17, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x24, 0x25, 0x26, \
	0x27, 0x2C, 0x2D, 0x2E, 0x2F, 0x33, 0x34, 0x35, 0x36, 0x37, \
	0x3C, 0x3D, 0x3E, 0x3F \
}
#endif
/*** svc_prot.c *******************************************************/
#define PROT_BUF_SIZE				(64)
#define PROT_CMD_SIZE				(1)
#define PROT_PAR_SIZE				(1)
#define PROT_DATA_SIZE				(4)
//
#define STATUS_PRESET				(0x01)
#define STATUS_STP_WR_FAULT			(0x15)
#define STATUS_STP_CHKSUM_ERR		(0x16)
#define STATUS_PRESET_KO			(0x20)
#define STATUS_READY				(0x40)
#define STATUS_BUSY					(0x41)
#define STATUS_BUSY_CALIB			(0x42)
#define STATUS_CALIB_KO				(0x43)
//
#define STATUS_IR_EMPTY				(0x05)		// Box empty
#define STATUS_IR_FULL				(0x40)		// Box full
#define STATUS_IR_SATURATION		(0x04)		// Box IR saturation
#define IR_PWM_K_CENT_DEF			(160)		// K in cent units
#define IR_PWM_A_CENT_DEF			(220)		// K in cent units
#define IR_PWM_K_CENT_MIN			(100)		// K in cent units
#define IR_PWM_K_CENT_MAX			(200)		// K in cent units
#define IR_RUN_AUTO_REP_N			(1)			// Number of repeat cycles in automatic mode
// Comandi da CM
#define PROT_STATUS_CMD				(0xF0)		// Status command
#define PROT_SW_RESET_CMD			(0x80)		// Software reset
#define PROT_WR_CMD					(0x90)		// Write parameter + PAR + 4 bytes (1 nibble per byte)
#define PROT_WR_PAR_IR_ADJ_A		(0x01)		// ... 1 / 3 IR_A PWM DUTY (byte from 2 last nibbles)
#define PROT_WR_PAR_IR_ADJ_B		(0x02)		// ... 2 / 3 IR_B PWM DUTY (byte from 2 last nibbles)
#define PROT_WR_PAR_IR_ADJ_C		(0x03)		// ... 3 / 3 IR_C PWM DUTY (byte from 2 last nibbles)
#define PROT_WR_PAR_IR_K			(0x05)		// ... K parameter (formula: PWM = (K) * (C),  PWM = Min(PWM, B)
#define PROT_WR_PAR_IR_TA			(0x06)		// ... 1 / 5 IR TIMING_A (word from 4 last nibbles)
#define PROT_WR_PAR_IR_TB			(0x07)		// ... 2 / 5 IR TIMING_B (word from 4 last nibbles)
#define PROT_WR_PAR_IR_TC			(0x08)		// ... 3 / 5 IR TIMING_C (word from 4 last nibbles)
#define PROT_WR_PAR_IR_TD			(0x09)		// ... 4 / 5 IR TIMING_D (word from 4 last nibbles)
#define PROT_WR_PAR_IR_TE			(0x0A)		// ... 5 / 5 IR TIMING_E (word from 4 last nibbles)
#define PROT_WR_PAR_IR_ENA_MSK		(0x0F)		// ... Select LED bit msk 0 to 7 (0 off, 7 all) last nibble
#define PROT_WR_PAR_US_GAIN			(0x21)		// ... write ampli gain
#define PROT_WR_PAR_US_DIST_MIN_MM	(0x22)		// ... write distance min in mm
#define PROT_WR_PAR_US_DIST_MAX_MM	(0x23)		// ... write distance max in mm
#define PROT_WR_PAR_US_ECHO_LEV		(0x24)		// ... write echo threshold
#define PROT_WR_PAR_US_HYST_LEV		(0x25)		// ... write hysteresis
#define PROT_RD_CMD					(0xE0)		// Read parameter
#define PROT_RD_PAR_IR_MAN			(0x01)		// ... Manual started
#define PROT_RD_PAR_IR_CALIB_EMPTY	(0x02)		// ... Calibration empty result (A)
#define PROT_RD_PAR_IR_CALIB_FULL	(0x03)		// ... Calibration full result (B)
#define PROT_RD_PAR_IR_ADJ_EMPTY	(0x04)		// ... Adjust empty result (C)
#define PROT_RD_PAR_IR_K			(0x05)		// ... K parameter (formula: PWM = (K) * (C),  PWM = Min(PWM, B)
#define PROT_RD_PAR_IR_AUTO			(0x06)		// ... Automatic status
#define PROT_RD_PAR_VER				(0x0F)		// ... Version
#define PROT_RD_PAR_SYS				(0x10)		// ... read sys
#define PROT_RD_PAR_LOG				(0x11)		// ... read log sys
#define PROT_RD_PAR_US_AUTO			(0x20)		// ... read auto result
#define PROT_RD_PAR_US_GAIN			(0x21)		// ... read ampli gain
#define PROT_RD_PAR_US_DIST_MIN_MM	(0x22)		// ... read distance min in mm
#define PROT_RD_PAR_US_DIST_MAX_MM	(0x23)		// ... read distance max in mm
#define PROT_RD_PAR_US_ECHO_LEV		(0x24)		// ... read echo threshold
#define PROT_RD_PAR_US_HYST_LEV		(0x25)		// ... read hysteresis
#define PROT_RD_PAR_US_DIST_US		(0x26)		// ... read distance in us
#define PROT_RD_PAR_US_DIST_MM		(0x27)		// ... read distance in mm
#define PROT_SET_CMD				(0xD0)		// Set
#define PROT_SET_PAR_IR_MAN_START	(0x01)		// ... IR Led run duty cycle start
#define PROT_SET_PAR_IR_MAN_STOP	(0x02)		// ... IR Led run duty cycle stop
#define PROT_SET_PAR_IR_CALIB_EMPTY	(0x03)		// ... IR Led calibrate box empty (FACTORY)
#define PROT_SET_PAR_IR_CALIB_FULL  (0x04)		// ... IR Led calibrate box full (FACTORY)
#define PROT_SET_PAR_IR_ADJ_EMPTY	(0x05)		// ... IR Led adjust box empty (RUN time)
#define PROT_SET_PAR_IR_AUTO		(0x06)		// ... IR Led cycle
#define PROT_SET_PAR_US_AUTO		(0x20)		// ... US cycle
#define PROT_SET_PAR_DLL_EXE 		(0x50)		// ... Reboot
#define PROT_SET_PAR_DEBUG_ON 		(0x7E)		// ... Abilita gli eventi di debug
#define PROT_SET_PAR_DEBUG_OFF 		(0x7F)		// ... Disabilita gli eventi di debug
/*** util.c **********************************************************/
#include "Util.h"


/*** SYS_VAR.H ********************************************************
*
***********************************************************************/
typedef struct									// Stp IR PWM
{
	byte		calib_empty_val;				// ... Empty at factory (A)
	byte		calib_full_val;					// ... Full at factory (B)
	byte		adj_empty_val;					// ... Adjusted (C)
	byte		free_tbl[16 - 3];				// ... Padding
}t_stp_ir_pwm;									// Size 16
//
typedef struct									// Stp IR
{
	t_stp_ir_pwm pwm_tbl[IR_N];
	byte		k;								// ... k to obtain current pwm = k * C (<= B)
	byte		free_tbl[64 - 49];				// ... padding
}t_stp_ir;										// Size 64
//
typedef struct									// Stp US
{
	word		gain;							// ... Ampli gain
	word		dist_min_mm;					// ... Distance min
	word		dist_max_mm;					// ... Distance max
	word		echo_lev_mv;					// ... Echo threshold
	word		hyst_lev_mv;					// ... Hysteresis threshold
	byte 		free_tbl[32 - 10];				// ... Free
}t_stp_us;										// Size 64
//
typedef struct ALIGN(2)							// STP
{
	byte		ver_id;							// ... Version id
	t_stp_ir	ir;								// ... Infrared
	t_stp_us	us;								// ... Ultra sound
	byte		free[128 - 1 - 64 - 32];		// ... Padding
}t_stp;
//
#define DOWNLOAD_REBOOT_VAL		(0xAA)
//
typedef struct
{
	dword		freq_upd_tbl[IR_TIM_N];
}t_sys_ir;
//
typedef struct
{
	byte		amp_val;						// ... Amplifier gain
	byte		pow_stable_period_n;			// ... Power stable pause
	byte		burst_period_n;					// ... Burst PWM cycles
	byte		dist_min_period_n;				// ... Distance min.
	byte		dist_max_period_n;				// ... Distance max.
	word		dac_val;						// ... Dac value
}t_sys_us;
//
typedef struct
{
	dword		debug;
	byte		debug_ena_bit : 1;
	byte		download_reboot_val;
	byte		st;
	byte		mod_adr;
	t_sys_ir	ir;
	t_sys_us	us;
volatile dword	tic_tbl[TIC_N];
volatile word	log_adc_tbl[LOG_ADC_N];
volatile word	log_adc_wr_ind;
	word		log_adc_rd_ind;
	byte		rsa_tx_buf[RSA_TX_BUF_SIZE];
volatile byte	rsa_rx_buf[RSA_RX_BUF_SIZE];
	byte		i2c_tx_buf[I2C_TX_BUF_SIZE];
volatile byte	i2c_rx_buf[I2C_RX_BUF_SIZE];
	byte		prot_buf[PROT_BUF_SIZE];
	t_stp		stp;
	byte		pwm_ir_a_mul;
}t_sys;
enum
{
	DEBUG_ADC_AGND_ID = 0,
	DEBUG_ADC_ECHO_ID = 1,
	DEBUG_ADC_FREE_OUT_AN_ID = 2
};
#define DEBUG_MIX_BUILD(w, id, val)	((w) = (val) | (id) << 12)
#define DEBUG_SEP_EVA(id, val, w)	((id) = (w) >> 12, (val) = (w) & 0x0FFF)
// Remap
#define SYS_VAR			t_sys sys
#define SYS_VAR_EXT		extern t_sys sys
#define SYS				sys
// Buffers
#define RSA_TX_BUF	sys.rsa_tx_buf
#define RSA_RX_BUF	sys.rsa_rx_buf
#define I2C_TX_BUF	sys.i2c_tx_buf
#define I2C_RX_BUF	sys.i2c_rx_buf
#define PROT_BUF	sys.prot_buf


/*** SVC_HW.H **********************************************************
*
***********************************************************************/
/*** Types and prototypes *********************************************/
typedef struct										// IR
{
	byte				st;							// ... status
volatile byte			cnt;						// ... cnt
	byte				ind_tbl[IR_N];				// ... index tbl
volatile word			res_on_cnt_tbl[IR_N];		// ... result on tbl
volatile word			res_off_cnt_tbl[IR_N];		// ... result off tbl
	byte				pwm_duty_val_tbl[IR_N];		// ... physical value
	byte				ind_n;
	byte				ind_cnt;
volatile byte			rep_n;						// ... repeater (0 for continuous)
}t_shw_ir;
//
typedef struct										// US
{
	byte				st;							// ... status
volatile byte			cnt;						// ... cnt
}t_shw_us;
//
typedef struct
{
	volatile dword		TimingDelay;
	RCC_ClocksTypeDef	RCC_Clocks;
	t_shw_ir			ir;
	t_shw_us			us;
}t_shw;
/*** Functions ********************************************************/
void svc_hw_install(void);
void svc_hw(void);
void svc_hw_int_ena(bool flg);
void svc_hw_stp_init(void);
void svc_hw_delay_ms(volatile dword delay);
void svc_hw_delay_us(dword delay);
bool svc_hw_tic_install(bool flg);
void svc_hw_tic_set(byte id, dword timeout);
bool svc_hw_tic_tst(byte id);
bool svc_adc_done_tst(void);
// IR
#define IR_CNT_AUTO_STOP		(99)		// Must be > 4
bool svc_hw_ir(bool flg, byte ena_msk, byte rep_n, byte *pwm_duty_val_ptr);
bool svc_hw_ir_res_rd(byte id, word *res_on_cnt_ptr, word *res_off_cnt_ptr);
bool svc_hw_ir_pwm_duty_wr(byte id, byte val);
bool svc_hw_ir_pwm_duty_rd(byte id, byte *val_ptr);
bool svc_hw_ir_calib(byte ind, bool flg, byte *duty_val_ptr);
bool svc_hw_ir_pwm_ins(bool flg);
bool svc_hw_ir_pwm_upd(byte duty_n);
bool svc_hw_ir_ins_ena(bool flg);
bool svc_hw_ir_adc_ins_ena(bool flg);
//
#define US_CNT_AUTO_STOP		(255)		// Must be 255
bool svc_hw_us(bool flg);
bool svc_hw_us_ins_ena(bool flg);

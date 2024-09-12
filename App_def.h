/*** APP_DEF.H ********************************************************
*
***********************************************************************/
/*** APP **************************************************************/
typedef struct										// Infra Red IR
{
	byte			run_bit : 1;					// ... Run
	byte			stop_req_bit : 1;				// ... Stop request
	byte			calib_empty_run_bit : 1;		// ... Calibration mode empty
	byte			calib_full_run_bit : 1;			// ... Calibration mode full
	byte			adj_empty_run_bit : 1;			// ... Adjust mode empty
	byte			ena_msk;						// ... Enable msk (0x1, 0x2, 0x4 = Enable IR-1, IR-2, IR-3)
	byte			st;								// ... status
	byte			cnt;							// ... count
	byte			rep_n;							// ... repeater
	byte			duty_val_tbl[IR_N];				// ... current duty
	bool 			saturation_tbl[IR_N];			// ... saturation
	word			res_on_cnt_tbl[IR_N];			// ... result on tbl
	word			res_off_cnt_tbl[IR_N];			// ... result off tbl
}t_app_exe_ir;
//
typedef struct										// Ultra sound US
{
	byte			run_bit : 1;					// ... Run
	byte			st;								// ... Status
	byte			cnt;							// ... Count
	byte			meas_cnt;						// ... Count measures
	byte			meas_valid_cnt;					// ... Count measures different from 0
	word			meas_dist_period_n_acc;			// ... Accumulate measures
	word			echo_val;						// ... Echo val
	word			hyst_val;						// ... Echo val
	byte			dist_period_n;					// ... distance period n
}t_app_exe_us;
//
typedef struct										// Exe IR
{
	t_app_exe_ir	ir;
	t_app_exe_us	us;
}t_app_exe;
//
typedef struct										// Exe US
{
	byte			st;
	t_app_exe		exe;
}t_app;
//
enum
{
	APP_ST_VOID = 0,
	APP_ST_IDLE
};
#define APP_VAR t_app app
#define APP_VAR_EXT extern t_app app
/*** Functions ********************************************************/
bool app_install(bool flg);
void app_main(void);

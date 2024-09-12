/*** SVC_I2C.H *********************************************************
* I2C Slave
***********************************************************************/
/*** Types and prototypes *********************************************/
typedef struct								// I2C
{
volatile byte			st;					// ... Status
volatile t_i2c_tx_ind	tx_rd_ind;
	t_i2c_tx_ind	tx_wr_ind;
	t_i2c_rx_ind	rx_rd_ind;
volatile t_i2c_rx_ind	rx_wr_ind;
}t_si2c;
enum										// Status
{
	I2C_ST_VOID = 0,
	I2C_ST_RUN_RX,							// ... Addressed Master write slave
	I2C_ST_RUN_TX,							// ... Addressed Master read slave
	I2C_ST_ERR								// ... Error
};
/*** Functions ********************************************************/
bool svc_i2c_install(bool flg);
bool svc_i2c_ena(bool flg);
bool svc_i2c_crt_get(byte *crt_ptr);
bool svc_i2c_crt_put(byte crt);
bool svc_i2c_tx_empty_tst(void);

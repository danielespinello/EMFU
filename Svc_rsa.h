/*** SVC_RSA.H *********************************************************
*
***********************************************************************/
/*** Types and prototypes *********************************************/
typedef struct
{
volatile t_rsa_tx_ind	tx_rd_ind;
	t_rsa_tx_ind	tx_wr_ind;
	t_rsa_rx_ind	rx_rd_ind;
volatile t_rsa_rx_ind	rx_wr_ind;
}t_srsa;
/*** Functions ********************************************************/
bool svc_rsa_install(bool flg);
bool svc_rsa_ena(bool flg);
bool svc_rsa_crt_put(byte crt);
bool svc_rsa_crt_get(byte *crt_ptr);
word svc_rsa_tx_free_eva(void);
bool svc_rsa_tx_empty_tst(void);

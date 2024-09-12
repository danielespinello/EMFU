/*** SVC_ADC.H *********************************************************
*
***********************************************************************/
/*** Types and prototypes *********************************************/
typedef struct
{
volatile word	data_tbl[ADC_N];
}t_sadc;
/*** Functions ********************************************************/
bool svc_adc_install(bool flg);
bool svc_adc_ena(bool flg);
bool svc_adc_rd(byte id, word *val_ptr);

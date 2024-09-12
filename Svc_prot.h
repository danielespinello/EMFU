/*** SVC_PROT.H ********************************************************
*
***********************************************************************/
/*** Types and prototypes *********************************************/
typedef struct
{
	byte	st;
	byte	buf_len;
	byte	buf_ind;
}t_sprot;
enum
{
	SPROT_ST_VOID = 0,
	SPROT_ST_IDLE,
	SPROT_ST_EXE,
};
/*** Functions ********************************************************/
bool svc_prot_install(bool flg);
void svc_prot(void);
bool svc_prot_cmd_get(byte *dptr, byte dsize, byte *dlen_ptr);
bool svc_prot_cmd_put(byte *sptr, byte slen);

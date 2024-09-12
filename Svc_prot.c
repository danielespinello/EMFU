/*** SVC_PROT.C ********************************************************
*
***********************************************************************/
#include "env_def.h"
#include "config.h"
#include "svc_def.h"
#include "sys_var.h"
/*** Defines **********************************************************/
/*** Constants ********************************************************/
/*** Types and prototypes *********************************************/
/*** Variables ********************************************************/
SYS_VAR_EXT;
SVC_VAR_EXT;
/*** Functions ********************************************************/
STATIC bool pars_cmd_wait(void);
/*** svc_prot_install *************************************************
* Parameters:	flg
* Return:		bool
***********************************************************************/
bool svc_prot_install(bool flg)
{
	STRUCT_SET(SPROT, 0);
	return(TRUE);
}
/*** svc_prot **********************************************************
* Parameters:	void
* Return:		void
***********************************************************************/
void svc_prot(void)
{
	if (SPROT.st == SPROT_ST_VOID)
	{
		SPROT.st = SPROT_ST_IDLE;
	}
	else if (SPROT.st == SPROT_ST_IDLE)
	{
		if (pars_cmd_wait())
		{
			SPROT.st = SPROT_ST_EXE;
		}
	}
}
/*** svc_prot_cmd_get *************************************************
* Parameters:	dptr, dsize, dlen_ptr
* Return:		bool
***********************************************************************/
bool svc_prot_cmd_get(byte *dptr, byte dsize, byte *dlen_ptr)
{
	bool res = FALSE;

	res = (SPROT.st == SPROT_ST_EXE);
	if (res)
	{
		SPROT.st = SPROT_ST_IDLE;
		res = (dsize >= SPROT.buf_len);
	}
	if (res)
	{
		MEMCPY(dptr, PROT_BUF, SPROT.buf_len);
		*dlen_ptr = SPROT.buf_len;
	}
	return(res);
}
/*** svc_prot_cmd_put *************************************************
* Parameters:	str, slen
* Return:		bool
***********************************************************************/
bool svc_prot_cmd_put(byte *sptr, byte slen)
{
	bool res = FALSE;
	byte i;

	res = (SPROT.st == SPROT_ST_IDLE);
	if (res)
	{
		for (i = 0; i < slen;)
		{
			if (PROT_CRT_PUT(*sptr))
			{
				sptr++;
				i++;
			}
		}
	}
	return(res);
}
/*** Riceve un comando valido completo (no check sui dati) *************
* Parameters:	void
* Return:		bool
***********************************************************************/
bool pars_cmd_wait(void)
{
	bool res = FALSE;
	byte crt, adr, cmd;

	for (; PROT_CRT_GET(&crt); )
	{
		if (crt & 0x80)
		{
			adr = crt & 0x0F, cmd = crt & 0xF0;
			if (adr == sys.mod_adr)
			{
				if (cmd == PROT_STATUS_CMD || cmd == PROT_SW_RESET_CMD || cmd == PROT_SET_CMD ||
					cmd == PROT_WR_CMD || cmd == PROT_RD_CMD)
				{
					SPROT.buf_len = 0;
					SPROT.buf_ind = 0;
					PROT_BUF[SPROT.buf_ind++] = cmd;
					if (cmd == PROT_SET_CMD || cmd == PROT_RD_CMD)
						SPROT.buf_len = PROT_CMD_SIZE + PROT_PAR_SIZE;
					else if (cmd == PROT_WR_CMD)
						SPROT.buf_len = PROT_CMD_SIZE + PROT_PAR_SIZE + PROT_DATA_SIZE;
					else
						res = TRUE;
				}
			}
		}
		else if (SPROT.buf_len)
		{
			PROT_BUF[SPROT.buf_ind++] = crt;
			if (SPROT.buf_ind == SPROT.buf_len)
				res = TRUE;
		}
	}
	if (res)
	{
		SPROT.buf_len = SPROT.buf_ind;
	}
	return(res);
}

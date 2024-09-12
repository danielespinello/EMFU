/*** SVC_E2P.C *********************************************************
*
***********************************************************************/
#include "env_def.h"
#include "config.h"
#include "svc_def.h"
#include "hard_def.h"
#include "sys_var.h"
/*** Definitions ******************************************************/
/*** Constants ********************************************************/
/** Types and prototypes **********************************************/
/*** Variables ********************************************************/
SVC_VAR_EXT;
/*** Functions ********************************************************/
/*** svc_e2p_install **************************************************
* Parameters :	void
* Return :		void
**********************************************************************/
bool svc_e2p_install(bool flg)
{
	bool res = TRUE;

	STRUCT_SET(SE2P, 0);
	return(res);
}
/*** svc_e2p_read ****************************************************
* Parameters :	dptr, dsize
* Return :		bool
**********************************************************************/
bool svc_e2p_read(word *dptr, word dsize)
{
	bool res = FALSE;
	word i;
	dword *p;
	dword chk;

	res = (dsize <= (E2P_DATA_SIZE / 2));
	if (res)
	{												// Verify integrity
		for (p = (dword *)E2P_BASE_ADR, chk = 0x55555555; p < (dword *)(E2P_BASE_ADR + E2P_DATA_SIZE); p++)
			chk += *p;
		res = (chk == *(dword *)p);
		if (res)
			for (i = 0; i < dsize; i++)
				*dptr++ = *((word *)E2P_BASE_ADR + i);
	}
	return(res);
}
/*** svc_e2p_write ***************************************************
* Parameters :	sptr, slen
* Return :		bool
**********************************************************************/
bool svc_e2p_write(word *sptr, word slen)
{
	bool res = FALSE;
	dword adr;
	dword chk;
	dword *p;

	res = (slen <= E2P_DATA_SIZE / 2);
	if (res)
		res = svc_e2p_erase();
	if (res)
	{
		HW_FLA_UNLOCK;
		for (adr = E2P_BASE_ADR; res && adr < (E2P_BASE_ADR + slen * 2); adr += sizeof(word), sptr++)
			res = HW_FLA_WORD_WRITE_TST(adr, *sptr);
		HW_FLA_LOCK;
	}
	if (res)
	{
		for (p = (dword *)E2P_BASE_ADR, chk = 0x55555555; p < (dword *)(E2P_BASE_ADR + E2P_DATA_SIZE); p++)
			chk += *p;
		HW_FLA_UNLOCK;
		res = HW_FLA_DWORD_WRITE_TST(E2P_BASE_ADR + E2P_DATA_SIZE, chk);
		HW_FLA_LOCK;
	}
	return(res);
}
/*** svc_e2p_erase ***************************************************
* Parameters :	void
* Return :		bool
**********************************************************************/
bool svc_e2p_erase(void)
{
	dword adr;
	bool res = TRUE;

	HW_FLA_UNLOCK;
	for (adr = E2P_BASE_ADR; res && adr < (E2P_BASE_ADR + E2P_SIZE); adr += E2P_FLASH_PAGE_SIZE)
	{
		if (!HW_FLA_PAGE_ERASE_TST(adr))
			res = FALSE;
	}
	HW_FLA_LOCK;
	return(res);
}

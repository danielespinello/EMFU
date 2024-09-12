/*** UTIL.C ***********************************************
 *
 *********************************************************/
#include "env_def.h"
#include "config.h"
/*** dword to ascii decimal ******************************************
* Parameters :	dptr, ui
* Return :		lunghezza della stringa
* Example :		ui = 12345 : util_ui_to_adecstr="12345", len = 5
**********************************************************************/
byte util_ui_to_adecstr(byte *dptr, dword ui)
{
	byte i, len;
	dword dw;

	for (dw = ui, len = 1, dw /= 10; dw; len++)
		dw /= 10;
	for (i = len; i; i--, ui /= 10)
		dptr[i - 1] = (byte)((ui % 10) + '0');
	return(len);
}
/*** util_bcd_lsb_str_to_ui *******************************************
* Parameters :	sptr, slen
* Return :		val
* Example :		sptr="0x0F, 0x0E, 0x0D, 0x0C", slen = 4 -> val = 0xFEDC
**********************************************************************/
dword util_bcd_lsb_str_to_ui(byte *sptr, byte slen)
{
	dword val;
	byte shift;

	for (val = 0, shift = 0; slen; slen--, shift += 4)
	{
		val |= (dword)(*(sptr + slen - 1) & 0x0F) << shift;
	}
	return(val);
}
/*** util_ui_to_bcd_lsb_str *******************************************
* Parameters :	dptr, dlen, val
* Example :		val = 0xFEDC, dlen = 4 -> dptr="0x0F, 0x0E, 0x0D, 0x0C"
**********************************************************************/
void util_ui_to_bcd_lsb_str(byte *dptr, byte dlen, dword val)
{
	byte shift;

	for (shift = 0; dlen; dlen--, shift += 4)
	{
		*(dptr + dlen - 1) = (byte)(val >> shift) & 0x0F;
	}
}

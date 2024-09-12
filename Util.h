/*** UTIL.H ************************************************************
*
***********************************************************************/
/*** Types and prototypes *********************************************/
/*** Functions ********************************************************/
byte util_ui_to_adecstr(byte *dptr, dword ui);
dword util_bcd_lsb_str_to_ui(byte *sptr, byte slen);
void util_ui_to_bcd_lsb_str(byte *dptr, byte dlen, dword val);

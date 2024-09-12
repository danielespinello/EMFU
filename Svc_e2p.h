/*** SVC_E2P.H *********************************************************
*
***********************************************************************/
/*** Types and prototypes *********************************************/
#define E2P_BASE_ADR			(0x0800F000)			/**< Base Address dello spazio per emulazione E2P in flash */
#define E2P_SIZE				(0x1000)				/**< Dimensione della E2P in flash */
#define E2P_FLASH_PAGE_SIZE		(0x400)					/**< Dimensione di una pagina di flash */
#define E2P_DATA_SIZE			(E2P_SIZE - 4)
typedef struct
{
	byte			debug;
}t_se2p;
/*** Functions ********************************************************/
bool svc_e2p_install(bool flg);
bool svc_e2p_read(word *dptr, word dsize);
bool svc_e2p_write(word *sptr, word slen);
bool svc_e2p_erase(void);

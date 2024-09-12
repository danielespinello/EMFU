/*** SVC_DEF.H ********************************************************
*
***********************************************************************/
/*** svc_hw.c ********************************************************/
#include "Svc_hw.h"
/*** svc_adc.c *******************************************************/
#include "Svc_adc.h"
/*** svc_e2p.c *******************************************************/
#include "Svc_e2p.h"
/*** svc_rsa.c *******************************************************/
#include "Svc_rsa.h"
/*** svc_i2cs.c ******************************************************/
#include "Svc_i2c.h"
/*** svc_prot.c ******************************************************/
#include "Svc_prot.h"
/*** SVC *************************************************************/
typedef struct
{
	t_shw			hw;
	t_sadc			adc;
	t_se2p			e2p;
	t_srsa			rsa;
	t_si2c			i2c;
	t_sprot			prot;
}t_svc;
#define SVC_VAR t_svc svc
#define SVC_VAR_EXT extern t_svc svc
// Remap
#define	SVC			svc
#define	SHW			svc.hw
#define	SADC		svc.adc
#define	SE2P		svc.e2p
#define	SRSA		svc.rsa
#define	SI2C		svc.i2c
#define	SPROT		svc.prot


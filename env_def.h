/*** ENV_DEF.H ********************************************************
* Definizioni di ambiente
***********************************************************************/
#include <stdint.h>
/***********************************************************************************************************************
 * Definizione tipi dati e puntatori dipendenti dall'implementazione
 **********************************************************************************************************************/
// Datatype booleano
#ifndef FALSE
	typedef enum 					en_BOOL_t {FALSE, TRUE} bool, BOOL, *lpbool, *LPBOOL;
#endif

// NULL pointer
#ifndef NULL
	#define NULL					((void *)0)
#endif

// void
typedef void						VOID, *lpvoid, *LPVOID;
typedef const void					*lpcvoid, *LPCVOID;

// 8 bit
typedef char						int8, CHAR, TCHAR, *lpchar, *lpint8, *LPCHAR, *LPTCHAR, *LPSTR, *LPTSTR;
typedef const char					*LPCSTR, *LPTCSTR;
typedef unsigned char				byte, uchar, uint8, BYTE, UCHAR, *lpbyte, *lpuchar, *lpuint8, *LPBYTE, *LPUCHAR;
typedef const unsigned char			*lpcbyte, *LPCBYTE, *LPCUCHAR;

// 16 bit
typedef short						int16, SHORT, *lpshort, *LPSHORT, *lpint16;
typedef const short					*lpcshort, *LPCSHORT;
typedef unsigned short				word, ushort, uint16, WORD, USHORT, *lpword, *lpushort, *lpuint16, *LPWORD, *LPUSHORT;
typedef const unsigned short		*lpcword, *LPCWORD, *LPCUSHORT;

// 32 bit
typedef int							int32, ptr, INT, LONG, PTR, *lpint, *lplong, *lpint32, *lpptr, *LPINT, *LPLONG, *LPPTR;
typedef const int					*lpcint, *lpclong, *lpcptr, *LPCINT, *LPCLONG, *LPCPTR;
typedef unsigned int				dword, uint, ulong, uint32, DWORD, UINT, ULONG, *lpdword, *lpuint, *lpulong, \
										*lpuint32, *LPDWORD, *LPUINT, *LPULONG;
typedef const unsigned int			*lpcdword, *lpcuint, *lpculong, *LPCDWORD, *LPCUINT, *LPCULONG;

// 64 bit
typedef long long					int64, guid, LLONG, GUID, *lpint64, *lpguid, *LPLLONG, *LPGUID;
typedef const long long				*lpcint64, *lpcguid, *LPCLLONG, *LPCGUID;
typedef unsigned long long			qword, uint64, QWORD, ULLONG, *lpqword, *lpuint64, *LPQWORD, *LPULLONG;
typedef const unsigned long long	*lpcqword, *LPCQWORD, *LPCULLONG;

// sizeof per membri di strutture.
#define sizeofm(t, m)				sizeof(((t *)0)->m)
#define SIZEOF(a)               	(sizeof(a)/sizeof(a[0]))
//
/***********************************************************************************************************************
 * Costanti di uso comune
 **********************************************************************************************************************/
// Definizioni dei bit 0..31
#define B0						(1UL << 0)
#define B1						(1UL << 1)
#define B2						(1UL << 2)
#define B3						(1UL << 3)
#define B4						(1UL << 4)
#define B5						(1UL << 5)
#define B6						(1UL << 6)
#define B7						(1UL << 7)
#define B8						(1UL << 8)
#define B9						(1UL << 9)
#define B10						(1UL << 10)
#define B11						(1UL << 11)
#define B12						(1UL << 12)
#define B13						(1UL << 13)
#define B14						(1UL << 14)
#define B15						(1UL << 15)
#define B16						(1UL << 16)
#define B17						(1UL << 17)
#define B18						(1UL << 18)
#define B19						(1UL << 19)
#define B20						(1UL << 20)
#define B21						(1UL << 21)
#define B22						(1UL << 22)
#define B23						(1UL << 23)
#define B24						(1UL << 24)
#define B25						(1UL << 25)
#define B26						(1UL << 26)
#define B27						(1UL << 27)
#define B28						(1UL << 28)
#define B29						(1UL << 29)
#define B30						(1UL << 30)
#define B31						(1UL << 31)
// Magic Numbers
#define MAGIC_MSB				0xaa55aa55UL		/**< Magic number, MSB=1 */
#define MAGIC_LSB				0x55aa55aaUL		/**< Magic number, LSB=1 */
#define MAGIC_MSBW				0xaa55U				/**< Magic number, word, MSB=1 */
#define MAGIC_LSBW				0x55aaU				/**< Magic number, word, LSB=1 */
#define MAGIC_MSBB				0xa5				/**< Magic number, byte, MSB=1 */
#define MAGIC_LSBB				0x5a				/**< Magic number, byte, LSB=1 */

/***********************************************************************************************************************
 * Macro di uso comune
 **********************************************************************************************************************/
// Attributi di compilazione
#define NORETURN				__attribute__ ((__noreturn__))
#define NOINLINE				__attribute__ ((__noinline__))
#define ALWAYS_INLINE			__attribute__ ((__always_inline__))
#define SECTION(x)				__attribute__ ((__section__ (x)))
#define ALIGN(x)				__attribute__ ((__aligned__ (x)))
#define PACK					__attribute__ ((__packed__))
#define CTOR					__attribute__ ((__constructor__))
#define DTOR					__attribute__ ((__destructor__))
#define DEPRECATED				__attribute__ ((__deprecated__))
// Impostazione e lettura di singoli bit
#define BSET(val, bit)			((val) |= (bit))					/**< Setta il bit specificato (usare B*) */
#define BCLR(val, bit)			((val) &= ~(bit))					/**< Azzera il bit specificato (usare B*) */
#define BMASK(val, bit)			((val) & (bit))						/**< Maschera i bit non specificati (usare B*) */
// Composizione e scomposizione parole dati
#define MKBYTE(nh, nl)			(((BYTE)(nh) << 4) | (BYTE)(nl))	/**< Costruisce un byte, dati High e Low nibble */
#define MKWORD(bh, bl)			(((WORD)(bh) << 8) | (BYTE)(bl))	/**< Costruisce una word, dati High e Low byte */
#define MKDWORD(wh, wl)			(((DWORD)(wh) << 16) | (WORD)(wl))	/**< Costruisce una dword, date High e Low word */
#define HIWORD(x)				((DWORD)(x) >> 16)					/**< Restituisce la MSW di una double word */
#define LOWORD(x)				((WORD)(x))							/**< Restituisce la LSW di una double word */
#define HIBYTE(x)				((WORD)(x) >> 8)					/**< Restituisce il MSB di una word */
#define LOBYTE(x)				((BYTE)(x))							/**< Restituisce il LSB di una word */
#define HINIBBLE(x)				((BYTE)(x) >> 4)					/**< Restituisce il High nibble di un byte */
#define LONIBBLE(x)				((BYTE)(x) & 0x0f)					/**< Restituisce il Low nibble di un byte */
// Determinazione del segno
#define sign(n)					((n) >= 0 ? 1 : -1)					/**< Restituisce il segno del parametro (+1/-1) */
//
#define STATIC static
#define CONST const
#define	ASCII_09_TST(x)				((x) >= '0' && (x) <= '9')
#define ASCII_AF_TST(x)				((x) >= 'A' && (x) <= 'F')
#define ASCII_AZ_TST(x)				((x) >= 'A' && (x) <= 'Z')
#define ASCII_az_TST(x)				((x) >= 'a' && (x) <= 'z')
#define SIZEOF(a)               	(sizeof(a)/sizeof(a[0]))
#define SIZEOF_STR(a)           	(sizeof(a)-1)
#define MEMSET(dest, crt, len) 		memset((char *)dest,crt,len)
#define MEMCPY(dest, src, len) 		memcpy((char *)(dest),(CONST char *)(src),len)
#define MEMMOV(dest, src, len) 		memmove((char *)(dest),(char *)(src),len)
#define MEMCMP_EQ(src1, src2, len) 	(memcmp((char *)src1,(char *)src2,len) == 0)
#define MEMCMP(src1, src2, len) 	(memcmp((char *)src1,(char *)src2,len))
#define STRUCT_SET(objd, crt)		MEMSET(&objd,crt,sizeof(objd))
#define STRUCT_CMP(obj1, obj2)		(MEMCMP_EQ((byte *)&obj1,(byte *)&obj2, sizeof(obj1)) == 0)
#define MEMCPY_CONST(dest, src, len) memcpy((byte *)(dest),(CONST byte *)(src),len)
#define ASCII_HEX_TO_BIN(x)     	((x) - (((x) > '9') ? '7' : '0'))
//
#define BUF_CIRC_FREE_EVA(wr_ind, rd_ind, size) \
			(((rd_ind) + (size) - (wr_ind) -1) % (size))
//
#define VAR_INC_ROTATE(b, n)		((b) = ((b) + 1) % (n))
#define VAR_ADD_ROTATE(dest,src,val,size)	((dest) = ((src) + (val)) % (size))
#define VAR_SUB_ROTATE(dest,src,val,size)	((dest)=((src)+(size)-(val))%(size))
#define MIN(a,b)					(((a)<(b)) ? (a) : (b))
#define MAX(a,b)					(((a)>(b)) ? (a) : (b))
//
typedef void (*t_fx_ptr_void_void)(void);					/* function pointer, no parameter, return void */
// At the end
#include "cmsis_boot/stm32f0xx.h"
#include "stm_lib/inc/stm32f0xx_rcc.h"
#include "stm_lib/inc/stm32f0xx_gpio.h"
#include "stm_lib/inc/stm32f0xx_syscfg.h"
#include "stm_lib/inc/stm32f0xx_flash.h"
#include "stm_lib/inc/stm32f0xx_spi.h"
#include "stm_lib/inc/stm32f0xx_misc.h"
#include "stm_lib/inc/stm32f0xx_exti.h"
#include "stm_lib/inc/stm32f0xx_tim.h"
#include "stm_lib/inc/stm32f0xx_adc.h"
#include "stm_lib/inc/stm32f0xx_dma.h"
#include "stm_lib/inc/stm32f0xx_usart.h"
#include "stm_lib/inc/stm32f0xx_rtc.h"
#include "stm_lib/inc/stm32f0xx_i2c.h"
#include "stm_lib/inc/stm32f0xx_dac.h"
#include "stm_lib/inc/stm32f0xx_comp.h"
#include "stdlib.h"
#include "string.h"

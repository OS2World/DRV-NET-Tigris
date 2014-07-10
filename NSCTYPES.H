/*
**      nsctypes.h      - Local Standard Data Types
**      
**      $Archive:   /nsclib/tech/include/vcs/nsctypes.h_v  $     
**      $Author:   croussel  $
**      $Revision:   1.14  $     $Date:   04/18/95 10:03:12  $
**
**      $Log:   /nsclib/tech/include/vcs/nsctypes.h_v  $
**	
**	   Rev 1.14   04/18/95 10:03:12   croussel
**	GNU doesn't support the cdecl keyword, so force it to NULL
**	
**	   Rev 1.13   03/20/95 19:18:04   lenglish
**	ifdefed stuff for win95/chicago.
**	no _FAR_ for chicago/win95.
**	no definitions of LOBYTE, HIBYTE, LOWORD, HIWORD: they conflict with 
**	 prior definitions in the system .h files.
**	
**	
**	   Rev 1.12   20 Dec 1994 00:44:54   croussel
**	define MSDOS for borland
**	
**	   Rev 1.11   11/07/94 14:29:54   croussel
**	NULL is now just 0, removed the (void *) typecast, as this was
**	generating a near point to DS:0 in borland, which is NOT a null
**	far pointer.
**	
**	   Rev 1.10   11/01/94 17:11:42   croussel
**	added _FAR_ definition for __BORLANDC__
**	
**	   Rev 1.9   20 Oct 1994 16:33:16   croussel
**	removed #define global
**	
**	   Rev 1.8   10/07/94 15:34:12   rdunlap
**	add #define of _FAR_ for ODI32
**	
**	   Rev 1.7   08/23/94 17:15:40   nsclib
**	removed control-Z at end of file
**	
**	   Rev 1.6   08/23/94 12:49:22   rdunlap
**	added defines for _FAR_ for MSDOS and SCO_UNIX
**	changed defines for "far" for sparc and NDIS_NT to be spelled as _FAR_
**	
**	   Rev 1.5   18 Aug 1994 10:39:00   khazzah
**	
**	Added macros HIBYTE, LOBYTE, HIWORD, LOWORD, MAKEUINT16, MAKEUINT32, ARRAYS
**      
**         Rev 1.4   08/04/94 16:37:58   rdunlap
**      delete #define local static (for h2inc/masm compatibility)
**      
**         Rev 1.3   07/21/94 10:54:18   croussel
**      added changes requested by Eric
**      
**         Rev 1.2   07/14/94 15:33:56   croussel
**      removed // comments
**      
**         Rev 1.1   06/13/94 14:27:32   croussel
**      added FieldOffset macro
**      
**         Rev 1.0   05/26/94 16:48:10   croussel
**      Initial revision.
**
*/
#ifndef  _nsctypes_h_
#define  _nsctypes_h_   1

typedef unsigned long   uint32;
typedef unsigned short  uint16;
typedef unsigned char   uchar;

typedef long            int32;
typedef short           int16;
typedef int             bool;

/* added below */
typedef void far   *PVOID;
typedef void       *PNVOID;
typedef uint32 far *Puint32;
typedef uint16 far *Puint16;
typedef uchar far  *Puchar;


#ifdef	__BORLANDC__
#define	MSDOS	1
#endif
#ifdef  sparc
#define _FAR_     
#define	cdecl
#endif
#ifdef  NDIS_NT
#define _FAR_     
#endif
#ifdef  SCO_UNIX
#define _FAR_     
#endif
#ifdef  MSDOS
#define _FAR_   __far
#endif
#ifdef	ODI32
#define	_FAR_
#endif
#ifdef  CHICAGO
#define _FAR_
#endif 

#ifndef FALSE
#define FALSE           0
#endif
#ifndef TRUE
#define TRUE            (!FALSE)        
#endif

#ifndef NULL
#define NULL            0
#endif

#ifndef min
#define min(a,b)        ((a)<(b)?(a):(b))
#endif

#ifndef max
#define max(a,b)        ((a)>(b)?(a):(b))
#endif

/* macro useful for converting structure field names to
** byte offsets */

#define FieldOffset(s,x)   ((int)&(((s *)0)->x))

#ifndef CHICAGO
#define LOBYTE(w)       ((uchar)(w))
#define HIBYTE(w)       ((uchar)((uint16)(w) >> 8))
#define LOWORD(l)       ((uint16)(uint32)(l))
#define HIWORD(l)       ((uint16)(((uint32)(l)) >> 16))
#endif //CHICAGO

#define MAKEUINT16( msb, lsb ) (((uint16)msb << 8) + lsb)
#define MAKEUINT32(hi, lo) ((uint32)(((uint16)(lo)) | (((uint32)((uint16)(hi))) << 16)))

#define ARRAY_SIZE(a)   (sizeof(a)/sizeof(a[0]))

#endif

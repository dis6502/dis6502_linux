/**************************************************************************

    UTILS.H - useful functions for WinG Sample Apps

 **************************************************************************/
/***************************************************************************

    (C) Copyright 1994 Microsoft Corp.  All rights reserved.

    You have a royalty-free right to use, modify, reproduce and 
    distribute the Sample Files (and/or any modified version) in 
    any way you find useful, provided that you agree that 
    Microsoft has no warranty obligations or liability for any 
    Sample Application Files which are modified. 

 **************************************************************************/

#include <Windows.h>

/***************************************************************************
    Functions for handling Device Independent Bitmaps and clearing the 
    System Palette.                                                    
 **************************************************************************/

#ifndef SAMPLES_UTILS_H  
#define SAMPLES_UTILS_H

typedef     LPBITMAPINFOHEADER PDIB;
typedef     HANDLE             HDIB;

/***************************************************************************
   External function declarations
 **************************************************************************/

void        ClearSystemPalette(void);

BOOL        DibSetUsage(PDIB pdib, HPALETTE hpal,UINT wUsage);
PDIB        DibCreate(int bits, int dx, int dy);
PDIB        DibNew(int bits, int dx, int dy);
void        DibFree(PDIB pdib);
void        DibMapToPalette(PDIB pdib, HPALETTE hpal);  

/****************************************************************************
   DIB macros.
 ***************************************************************************/

#define WIDTHBYTES(i)           ((unsigned)((i+31)&(~31))/8)  /* ULONG aligned ! */

#define DibWidth(lpbi)          (UINT)(((LPBITMAPINFOHEADER)(lpbi))->biWidth)
#define DibHeight(lpbi)         (UINT)(((LPBITMAPINFOHEADER)(lpbi))->biHeight)
#define DibBitCount(lpbi)       (UINT)(((LPBITMAPINFOHEADER)(lpbi))->biBitCount)
#define DibCompression(lpbi)    (DWORD)(((LPBITMAPINFOHEADER)(lpbi))->biCompression)

#define DibWidthBytesN(lpbi, n) (UINT)WIDTHBYTES((UINT)(lpbi)->biWidth * (UINT)(n))
#define DibWidthBytes(lpbi)     DibWidthBytesN(lpbi, (lpbi)->biBitCount)

#define DibSizeImage(lpbi)      ((lpbi)->biSizeImage == 0 \
                                    ? ((DWORD)(UINT)DibWidthBytes(lpbi) * (DWORD)(UINT)(lpbi)->biHeight) \
                                    : (lpbi)->biSizeImage)

#define DibSize(lpbi)           ((lpbi)->biSize + (lpbi)->biSizeImage + (int)(lpbi)->biClrUsed * sizeof(RGBQUAD))
#define DibPaletteSize(lpbi)    (DibNumColors(lpbi) * sizeof(RGBQUAD))

#define DibFlipY(lpbi, y)       ((int)(lpbi)->biHeight-1-(y))

//HACK for NT BI_BITFIELDS DIBs
#ifdef WIN32
    #define DibPtr(lpbi)            ((lpbi)->biCompression == BI_BITFIELDS \
                                       ? (LPVOID)(DibColors(lpbi) + 3) \
                                       : (LPVOID)(DibColors(lpbi) + (UINT)(lpbi)->biClrUsed))
#else
    #define DibPtr(lpbi)            (LPVOID)(DibColors(lpbi) + (UINT)(lpbi)->biClrUsed)
#endif

#define DibColors(lpbi)         ((RGBQUAD FAR *)((LPBYTE)(lpbi) + (int)(lpbi)->biSize))

#define DibNumColors(lpbi)      ((lpbi)->biClrUsed == 0 && (lpbi)->biBitCount <= 8 \
                                    ? (int)(1 << (int)(lpbi)->biBitCount)          \
                                    : (int)(lpbi)->biClrUsed)

#define DibInfo(pDIB)     ((BITMAPINFO FAR *)(pDIB))

/***************************************************************************/

#ifndef BI_BITFIELDS
  #define BI_BITFIELDS 3
#endif  

#endif

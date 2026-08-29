/**************************************************************************

    UTILS.C - useful functions for WinG Sample Apps

 **************************************************************************/
 /**************************************************************************

     (C) Copyright 1994 Microsoft Corp.  All rights reserved.

     You have a royalty-free right to use, modify, reproduce and
     distribute the Sample Files (and/or any modified version) in
     any way you find useful, provided that you agree that
     Microsoft has no warranty obligations or liability for any
     Sample Application Files which are modified.

  **************************************************************************/

  /*----------------------------------------------------------------------------*\
  |   Functions for handling Device Independent Bitmaps and clearing the         |
  |   System Palette.                                                            |
  \*----------------------------------------------------------------------------*/


#include <Windows.h>
#include <windowsx.h> // For GlobalFreePtr, GlobalReAllocPtr, hmemcpy
#include <memory.h>
#include "utils.h"

#if defined(WIN32) || defined(_WIN32)
#define _huge
#endif

#define BFT_ICON   0x4349   /* 'IC' */
#define BFT_BITMAP 0x4d42   /* 'BM' */
#define BFT_CURSOR 0x5450   /* 'PT' */

  /* flags for _lseek */
#define  SEEK_CUR 1
#define  SEEK_END 2
#define  SEEK_SET 0


/*
 *  Clear the System Palette so that we can ensure an identity palette
 *  mapping for fast performance.
 */

void ClearSystemPalette(void)
{
    //*** A dummy palette setup
    struct
    {
        WORD Version;
        WORD NumberOfEntries;
        PALETTEENTRY aEntries[256];
    } Palette =
    {
      0x300,
      256
    };

    HPALETTE ScreenPalette = 0;
    HDC ScreenDC;
    int Counter;


    //*** Reset everything in the system palette to black
    for (Counter = 0; Counter < 256; Counter++)
    {
        Palette.aEntries[Counter].peRed = 0;
        Palette.aEntries[Counter].peGreen = 0;
        Palette.aEntries[Counter].peBlue = 0;
        Palette.aEntries[Counter].peFlags = PC_NOCOLLAPSE;
    }

    //*** Create, select, realize, deselect, and delete the palette
    ScreenDC = GetDC(NULL);
    ScreenPalette = CreatePalette((LOGPALETTE*)&Palette);

    if (ScreenPalette)
    {
        ScreenPalette = SelectPalette(ScreenDC, ScreenPalette, FALSE);
        RealizePalette(ScreenDC);
        ScreenPalette = SelectPalette(ScreenDC, ScreenPalette, FALSE);
        DeleteObject(ScreenPalette);
    }

    ReleaseDC(NULL, ScreenDC);

    return;
}

/*
 *  DibSetUsage(hdib,hpal,wUsage)
 *
 *  Modifies the color table of the passed DIB for use with the wUsage
 *  parameter specifed.
 *
 *  if wUsage is DIB_PAL_COLORS the DIB color table is set to 0-256
 *  if wUsage is DIB_RGB_COLORS the DIB color table is set to the RGB values
 *      in the passed palette
 */

BOOL DibSetUsage(PDIB pdib, HPALETTE hpal, UINT wUsage)
{
    PALETTEENTRY       ape[256];
    RGBQUAD FAR* pRgb;
    WORD FAR* pw;
    int                nColors;
    int                n;

    if (hpal == NULL)
        hpal = (HPALETTE)GetStockObject(DEFAULT_PALETTE);

    if (!pdib)
        return FALSE;

    nColors = DibNumColors(pdib);

    if (nColors == 3 && DibCompression(pdib) == BI_BITFIELDS)
        nColors = 0;

    if (nColors > 0)
    {
        pRgb = DibColors(pdib);

        switch (wUsage)
        {
            //
            // Set the DIB color table to palette indexes
            //
        case DIB_PAL_COLORS:
            for (pw = (WORD FAR*)pRgb, n = 0; n < nColors; n++, pw++)
                *pw = n;
            break;

            //
            // Set the DIB color table to RGBQUADS
            //
        default:
        case DIB_RGB_COLORS:
            nColors = min(nColors, 256);

            GetPaletteEntries(hpal, 0, nColors, ape);

            for (n = 0; n < nColors; n++)
            {
                pRgb[n].rgbRed = ape[n].peRed;
                pRgb[n].rgbGreen = ape[n].peGreen;
                pRgb[n].rgbBlue = ape[n].peBlue;
                pRgb[n].rgbReserved = 0;
            }
            break;
        }
    }
    return TRUE;
}

/*
 *  DibCreate(bits, dx, dy)
 *
 *  Creates a new packed DIB with the given dimensions and the
 *  given number of bits per pixel
 */

PDIB DibCreate(int bits, int dx, int dy)
{
    LPBITMAPINFOHEADER lpbi;
    DWORD       dwSizeImage;
    int         i;
    DWORD FAR* pdw;

    dwSizeImage = dy * (DWORD)((dx * bits / 8 + 3) & ~3);

    HANDLE hpi = GlobalAlloc(GHND, sizeof(BITMAPINFOHEADER) + dwSizeImage + 1024);
    if (hpi == NULL) {
        return NULL;
    }
    lpbi = (PDIB)GlobalLock(hpi);

    if (lpbi == NULL) {
        return NULL;
    }

    lpbi->biSize = sizeof(BITMAPINFOHEADER);
    lpbi->biWidth = dx;
    lpbi->biHeight = dy;
    lpbi->biPlanes = 1;
    lpbi->biBitCount = bits;
    lpbi->biCompression = BI_RGB;
    lpbi->biSizeImage = dwSizeImage;
    lpbi->biXPelsPerMeter = 0;
    lpbi->biYPelsPerMeter = 0;
    lpbi->biClrUsed = 0;
    lpbi->biClrImportant = 0;

    if (bits == 4)
        lpbi->biClrUsed = 16;

    else if (bits == 8)
        lpbi->biClrUsed = 256;

    pdw = (DWORD FAR*)((LPBYTE)lpbi + (int)lpbi->biSize);

    for (i = 0; i < (int)lpbi->biClrUsed / 16; i++)
    {
        *pdw++ = 0x00000000;    // 0000  black
        *pdw++ = 0x00800000;    // 0001  dark red
        *pdw++ = 0x00008000;    // 0010  dark green
        *pdw++ = 0x00808000;    // 0011  mustard
        *pdw++ = 0x00000080;    // 0100  dark blue
        *pdw++ = 0x00800080;    // 0101  purple
        *pdw++ = 0x00008080;    // 0110  dark turquoise
        *pdw++ = 0x00C0C0C0;    // 1000  gray
        *pdw++ = 0x00808080;    // 0111  dark gray
        *pdw++ = 0x00FF0000;    // 1001  red
        *pdw++ = 0x0000FF00;    // 1010  green
        *pdw++ = 0x00FFFF00;    // 1011  yellow
        *pdw++ = 0x000000FF;    // 1100  blue
        *pdw++ = 0x00FF00FF;    // 1101  pink (magenta)
        *pdw++ = 0x0000FFFF;    // 1110  cyan
        *pdw++ = 0x00FFFFFF;    // 1111  white
    }

    return (PDIB)lpbi;
}

/*
 *   Create a DIB containing..
 *
 *   BITMAP INFO    bi
 *   palette data
 *   bits....
 */

PDIB DibNew(int bits, int dx, int dy)
{
    DWORD               dwLen;
    DWORD               dwBits;
    PDIB                pdib;
    LPVOID              p;

    pdib = DibCreate(bits, dx, dy);

    if (!pdib)
        return NULL;

    /* How much memory do we need to hold the DIB */

    dwBits = pdib->biSizeImage;
    dwLen = pdib->biSize + DibPaletteSize(pdib) + dwBits;

    /* Can we get more memory? */

    p = GlobalReAllocPtr(pdib, dwLen, 0);

    if (!p)
    {
        GlobalFreePtr(pdib);
        pdib = NULL;
    }
    else
    {
        pdib = (PDIB)p;
        memset((LPBYTE)pdib + (UINT)pdib->biSize + DibPaletteSize(pdib), 0, dwBits);
    }

    return pdib;
}

static void DibFree(PDIB pdib) {
    GlobalFreePtr(pdib);
}

static void xlatClut8(BYTE FAR* pb, DWORD dwSize, BYTE FAR* xlat)
{
    DWORD dw;

#ifdef __cplusplus
    for (dw = 0; dw < dwSize; dw++, ((BYTE _huge*&)pb)++)
#else
    for (dw = 0; dw < dwSize; dw++, ((BYTE _huge*)pb)++)
#endif
        * pb = xlat[*pb];
}

static void xlatClut4(BYTE FAR* pb, DWORD dwSize, BYTE FAR* xlat)
{
    DWORD dw;

#ifdef __cplusplus
    for (dw = 0; dw < dwSize; dw++, ((BYTE _huge*&)pb)++)
#else
    for (dw = 0; dw < dwSize; dw++, ((BYTE _huge*)pb)++)
#endif
        * pb = (BYTE)(xlat[*pb & 0x0F] | (xlat[(*pb >> 4) & 0x0F] << 4));
}

#define RLE_ESCAPE  0
#define RLE_EOL     0
#define RLE_EOF     1
#define RLE_JMP     2

static void xlatRle8(BYTE FAR* pb, DWORD dwSize, BYTE FAR* xlat)
{
    BYTE    cnt;
    BYTE    b;
    BYTE _huge* prle = pb;

    for (;;)
    {
        cnt = *prle++;
        b = *prle;

        if (cnt == RLE_ESCAPE)
        {
            prle++;

            switch (b)
            {
            case RLE_EOF:
                return;

            case RLE_EOL:
                break;

            case RLE_JMP:
                prle++;     // skip dX
                prle++;     // skip dY
                break;

            default:
                cnt = b;
                for (b = 0; b < cnt; b++, prle++)
                    *prle = xlat[*prle];

                if (cnt & 1)
                    prle++;

                break;
            }
        }
        else
        {
            *prle++ = xlat[b];
        }
    }
}

static void xlatRle4(BYTE FAR* pb, DWORD dwSize, BYTE FAR* xlat)
{
}

static void hmemmove(BYTE _huge* d, BYTE _huge* s, LONG len)
{
    d += len - 1;
    s += len - 1;

    while (len--)
        *d-- = *s--;
}

/*
 *  DibMapToPalette(pdib, hpal)
 *
 *  Map the colors of the DIB, using GetNearestPaletteIndex, to
 *  the colors of the given palette.
 */

void DibMapToPalette(PDIB pdib, HPALETTE hpal)
{
    LPBITMAPINFOHEADER  lpbi;
    PALETTEENTRY        pe;
    int                 n;
    int                 nDibColors;
    int                 nPalColors = 0;
    BYTE FAR* lpBits;
    RGBQUAD FAR* lpRgb;
    BYTE                xlat[256]{};
    DWORD               SizeImage;

    if (!hpal || !pdib)
        return;

    lpbi = (LPBITMAPINFOHEADER)pdib;
    lpRgb = DibColors(pdib);

    GetObject(hpal, sizeof(int), (LPSTR)&nPalColors);
    nDibColors = DibNumColors(pdib);

    if ((SizeImage = lpbi->biSizeImage) == 0)
        SizeImage = DibSizeImage(lpbi);

    //
    //  build a xlat table. from the current DIB colors to the given
    //  palette.
    //
    for (n = 0; n < nDibColors; n++)
        xlat[n] = (BYTE)GetNearestPaletteIndex(hpal, RGB(lpRgb[n].rgbRed, lpRgb[n].rgbGreen, lpRgb[n].rgbBlue));

    lpBits = (LPBYTE)DibPtr(lpbi);
    lpbi->biClrUsed = nPalColors;

    //
    // re-size the DIB
    //
    if (nPalColors > nDibColors)
    {
        GlobalReAllocPtr(lpbi, lpbi->biSize + nPalColors * sizeof(RGBQUAD) + SizeImage, 0);
        hmemmove((BYTE _huge*)DibPtr(lpbi), (BYTE _huge*)lpBits, SizeImage);
        lpBits = (LPBYTE)DibPtr(lpbi);
    }
    else if (nPalColors < nDibColors)
    {
        hmemcpy(DibPtr(lpbi), lpBits, SizeImage);
        GlobalReAllocPtr(lpbi, lpbi->biSize + nPalColors * sizeof(RGBQUAD) + SizeImage, 0);
        lpBits = (LPBYTE)DibPtr(lpbi);
    }

    //
    // translate the DIB bits
    //
    switch (lpbi->biCompression)
    {
    case BI_RLE8:
        xlatRle8(lpBits, SizeImage, xlat);
        break;

    case BI_RLE4:
        xlatRle4(lpBits, SizeImage, xlat);
        break;

    case BI_RGB:
        if (lpbi->biBitCount == 8)
            xlatClut8(lpBits, SizeImage, xlat);
        else
            xlatClut4(lpBits, SizeImage, xlat);
        break;
    }

    //
    //  Now copy the RGBs in the logical palette to the dib color table
    //
    for (n = 0; n < nPalColors; n++)
    {
        GetPaletteEntries(hpal, n, 1, &pe);

        lpRgb[n].rgbRed = pe.peRed;
        lpRgb[n].rgbGreen = pe.peGreen;
        lpRgb[n].rgbBlue = pe.peBlue;
        lpRgb[n].rgbReserved = (BYTE)0;
    }

    return;
}

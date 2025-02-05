//-----------------------------------------------------------------------------
// Copyright (c) 2013 GarageGames, LLC
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//-----------------------------------------------------------------------------

#include "graphics/gColor.h"
#include "console/console.h"
#include "console/consoleTypes.h"

#include "gColor_ScriptBinding.h"

#ifndef _STRINGUNIT_H_
#include "string/stringUnit.h"
#endif

#ifndef _HASHTABLE_H
#include "collection/hashTable.h"
#endif

//-----------------------------------------------------------------------------

typedef HashMap<StringTableEntry, ColorF> typeNameToColorFHash;
typedef HashMap<StringTableEntry, ColorI> typeNameToColorIHash;
typedef HashMap<ColorF, StringTableEntry> typeColorFToNameHash;
typedef HashMap<ColorI, StringTableEntry> typeColorIToNameHash;

static typeNameToColorFHash    mNameToColorF;
static typeNameToColorIHash    mNameToColorI;
static typeColorFToNameHash    mColorFToName;
static typeColorIToNameHash    mColorIToName;

#define DEFAULT_UNKNOWN_STOCK_COLOR_NAME    "White"

//-----------------------------------------------------------------------------

StockColorItem StockColorTable[] =
{
    StockColorItem( "InvisibleBlack", 0, 0, 0, 0 ),
    StockColorItem( "TransparentWhite", 255, 255, 255, 0 ),
    StockColorItem( "AliceBlue", 240, 248, 255 ),
    StockColorItem( "AntiqueWhite", 250, 235, 215 ),
    StockColorItem( "Aqua", 0, 255, 255 ),
    StockColorItem( "Aquamarine", 127, 255, 212 ),
    StockColorItem( "Azure", 240, 255, 255 ),
    StockColorItem( "Beige", 245, 245, 220 ),
    StockColorItem( "Bisque", 255, 228, 196 ),
    StockColorItem( "Black", 0, 0, 0, 255 ),
    StockColorItem( "BlanchedAlmond", 255, 235, 205, 255 ),
    StockColorItem( "Blue", 0, 0, 255 ),
    StockColorItem( "BlueViolet", 138, 43, 226 ),
    StockColorItem( "Brown", 165, 42, 42, 255 ),
    StockColorItem( "BurlyWood", 222, 184, 135 ),
    StockColorItem( "CadetBlue", 95, 158, 160 ),
    StockColorItem( "Chartreuse", 127, 255, 0 ),
    StockColorItem( "Chocolate", 210, 105, 30 ),
    StockColorItem( "Coral", 255, 127, 80 ),
    StockColorItem( "CornflowerBlue", 100, 149, 237 ),
    StockColorItem( "Cornsilk", 255, 248, 220 ),
    StockColorItem( "Crimson", 220, 20, 60 ),
    StockColorItem( "Cyan", 0, 255, 255 ),
    StockColorItem( "DarkBlue", 0, 0, 139 ),
    StockColorItem( "DarkCyan", 0, 139, 139 ),
    StockColorItem( "DarkGoldenrod", 184, 134, 11 ),
    StockColorItem( "DarkGray", 169, 169, 169),
    StockColorItem( "DarkGreen", 0, 100, 0 ),
    StockColorItem( "DarkKhaki", 189, 183, 107 ),
    StockColorItem( "DarkMagenta", 139, 0, 139 ),
    StockColorItem( "DarkOliveGreen", 85, 107, 47 ),
    StockColorItem( "DarkOrange", 255, 140, 0 ),
    StockColorItem( "DarkOrchid", 153, 50, 204 ),
    StockColorItem( "DarkRed", 139, 0, 0 ),
    StockColorItem( "DarkSalmon", 233, 150, 122 ),
    StockColorItem( "DarkSeaGreen", 143, 188, 139 ),
    StockColorItem( "DarkSlateBlue", 72, 61, 139 ),
    StockColorItem( "DarkSlateGray", 47, 79, 79 ),
    StockColorItem( "DarkTurquoise", 0, 206, 209 ),
    StockColorItem( "DarkViolet", 148, 0, 211 ),
    StockColorItem( "DeepPink", 255, 20, 147 ),
    StockColorItem( "DeepSkyBlue", 0, 191, 255 ),
    StockColorItem( "DimGray", 105, 105, 105 ),
    StockColorItem( "DodgerBlue", 30, 144, 255 ),
    StockColorItem( "Firebrick", 178, 34, 34 ),
    StockColorItem( "FloralWhite", 255, 250, 240 ),
    StockColorItem( "ForestGreen", 34, 139, 34 ),
    StockColorItem( "Fuchsia", 255, 0, 255 ),
    StockColorItem( "Gainsboro", 220, 220, 220 ),
    StockColorItem( "GhostWhite", 248, 248, 255 ),
    StockColorItem( "Gold", 255, 215, 0 ),
    StockColorItem( "Goldenrod", 218, 165, 32 ),
    StockColorItem( "Gray", 128, 128, 128 ),
    StockColorItem( "Green", 0, 128, 0 ),
    StockColorItem( "GreenYellow", 173, 255, 47 ),
    StockColorItem( "Honeydew", 240, 255, 24 ),
    StockColorItem( "HotPink", 255, 105, 180 ),
    StockColorItem( "IndianRed", 205, 92, 92 ),
    StockColorItem( "Indigo", 75, 0, 130 ),
    StockColorItem( "Ivory", 255, 255, 240 ),
    StockColorItem( "Khaki", 240, 230, 140 ),
    StockColorItem( "Lavender", 230, 230, 250 ),
    StockColorItem( "LavenderBlush", 255, 240, 245 ),
    StockColorItem( "LawnGreen", 124, 252, 0 ),
    StockColorItem( "LemonChiffon", 255, 250, 205 ),
    StockColorItem( "LightBlue", 173, 216, 230 ),
    StockColorItem( "LightCoral", 240, 128, 128 ),
    StockColorItem( "LightCyan", 224, 255, 255),
    StockColorItem( "LightGoldenrodYellow", 250, 250, 210 ),
    StockColorItem( "LightGray", 211, 211, 211),
    StockColorItem( "LightGreen", 144, 238, 144 ),
    StockColorItem( "LightPink", 255, 182, 193 ),
    StockColorItem( "LightSalmon", 255, 160, 122 ),
    StockColorItem( "LightSeaGreen", 32, 178, 170 ),
    StockColorItem( "LightSkyBlue",135, 206, 250 ),
    StockColorItem( "LightSlateGray", 119, 136, 153 ),
    StockColorItem( "LightSteelBlue", 176, 196, 222 ),
    StockColorItem( "LightYellow", 255, 255, 224 ),
    StockColorItem( "Lime", 0, 255, 0 ),
    StockColorItem( "LimeGreen", 50, 205, 50 ),
    StockColorItem( "Linen", 250, 240, 230 ),
    StockColorItem( "Magenta", 255, 0, 255 ),
    StockColorItem( "Maroon", 128, 0, 0 ),
    StockColorItem( "MediumAquamarine", 102, 205, 170 ),
    StockColorItem( "MediumBlue", 0, 0, 205 ),
    StockColorItem( "MediumOrchid", 186, 85, 211 ),
    StockColorItem( "MediumPurple", 147, 112, 219 ),
    StockColorItem( "MediumSeaGreen", 60, 179, 113 ),
    StockColorItem( "MediumSlateBlue", 123, 104, 238 ),
    StockColorItem( "MediumSpringGreen", 0, 250, 154 ),
    StockColorItem( "MediumTurquoise", 72, 209, 204 ),
    StockColorItem( "MediumVioletRed", 199, 21, 133 ),
    StockColorItem( "MidnightBlue", 25, 25, 112 ),
    StockColorItem( "MintCream", 245, 255, 250 ),
    StockColorItem( "MistyRose", 255, 228, 225 ),
    StockColorItem( "Moccasin", 255, 228, 181 ),
    StockColorItem( "NavajoWhite", 255, 222, 173 ),
    StockColorItem( "Navy", 0, 0, 128 ),
    StockColorItem( "OldLace", 253, 245, 230 ),
    StockColorItem( "Olive", 128, 128, 0 ),
    StockColorItem( "OliveDrab", 107, 142, 35 ),
    StockColorItem( "Orange", 255, 165, 0 ),
    StockColorItem( "OrangeRed", 255, 69, 0 ),
    StockColorItem( "Orchid", 218, 112, 214 ),
    StockColorItem( "PaleGoldenrod", 238, 232, 170 ),
    StockColorItem( "PaleGreen", 152, 251, 152 ),
    StockColorItem( "PaleTurquoise", 175, 238, 238 ),
    StockColorItem( "PaleVioletRed", 219, 112, 147 ),
    StockColorItem( "PapayaWhip", 255, 239, 213 ),
    StockColorItem( "PeachPuff", 255, 218, 185 ),
    StockColorItem( "Peru", 205, 133, 63 ),
    StockColorItem( "Pink", 255, 192, 203 ),
    StockColorItem( "Plum", 221, 160, 221 ),
    StockColorItem( "PowderBlue", 176, 224, 230 ),
    StockColorItem( "Purple", 128, 0, 128 ),
    StockColorItem( "Red", 255, 0, 0 ),
    StockColorItem( "RosyBrown", 188, 143, 143 ),
    StockColorItem( "RoyalBlue", 65, 105, 225 ),
    StockColorItem( "SaddleBrown", 139, 69, 19 ),
    StockColorItem( "Salmon", 250, 128, 114 ),
    StockColorItem( "SandyBrown", 244, 164, 96 ),
    StockColorItem( "SeaGreen", 46, 139, 87 ),
    StockColorItem( "SeaShell", 255, 245, 238 ),
    StockColorItem( "Sienna", 160, 82, 45 ),
    StockColorItem( "Silver", 192, 192, 192 ),
    StockColorItem( "SkyBlue", 135, 206, 235 ),
    StockColorItem( "SlateBlue", 106, 90, 205 ),
    StockColorItem( "SlateGray", 112, 128, 144 ),
    StockColorItem( "Snow", 255, 250, 250 ),
    StockColorItem( "SpringGreen", 0, 255, 127 ),
    StockColorItem( "SteelBlue", 70, 130, 180 ),
    StockColorItem( "Tan", 210, 180, 140 ),
    StockColorItem( "Teal", 0, 128, 128 ),
    StockColorItem( "Thistle", 216, 191, 216 ),
    StockColorItem( "Tomato", 255, 99, 71 ),
    StockColorItem( "Turquoise", 64, 224, 208 ),
    StockColorItem( "Violet", 238, 130, 238 ),
    StockColorItem( "Wheat", 245, 222, 179 ),
    StockColorItem( "White", 255, 255, 255 ),
    StockColorItem( "WhiteSmoke", 245, 245, 245 ),
    StockColorItem( "Yellow", 255, 255, 0 ),
    StockColorItem( "YellowGreen", 154, 205, 50 )
};

//-----------------------------------------------------------------------------

static bool stockColorsCreated = false;

void StockColor::create( void )
{
    // Finish if already created.
    if ( stockColorsCreated )
        return;

    // Fetch stock color count.
    const S32 stockColorCount = sizeof(StockColorTable) / sizeof(StockColorItem);

    // Insert all stock colors.
    for( S32 index = 0; index < stockColorCount; ++index )
    {
        // Fetch stock color item.
        StockColorItem& stockColor = StockColorTable[index];

        // Fetch stock color item.
        StringTableEntry colorName = StringTable->insert( stockColor.mColorName );

        // Insert stock color mappings.
        mNameToColorF.insert( colorName, stockColor.mColorF );
        mNameToColorI.insert( colorName, stockColor.mColorI );
        mColorFToName.insert( stockColor.mColorF, colorName );
        mColorIToName.insert( stockColor.mColorI, colorName );
    }

    // Flag as created.
    stockColorsCreated = true;
}

//-----------------------------------------------------------------------------

void StockColor::destroy( void )
{
    // Finish if not created.
    if ( !stockColorsCreated )
        return;

    // Clear stock color mappings.
    mNameToColorF.clear();
    mNameToColorI.clear();
    mColorFToName.clear();
    mColorIToName.clear();

    // Flag as not created.
    stockColorsCreated = false;
}

//-----------------------------------------------------------------------------

bool StockColor::isColor( const char* pStockColorName )
{
    // Sanity!
    AssertFatal( pStockColorName != NULL, "Cannot fetch a NULL stock color name." );

    // Fetch color name.
    StringTableEntry colorName = StringTable->insert( pStockColorName );

    // Find if color name exists or not.
    return mNameToColorF.find( colorName ) != mNameToColorF.end();
}

//-----------------------------------------------------------------------------

const ColorF& StockColor::colorF( const char* pStockColorName )
{
    // Sanity!
    AssertFatal( pStockColorName != NULL, "Cannot fetch a NULL stock color name." );

    // Fetch color name.
    StringTableEntry colorName = StringTable->insert( pStockColorName );

    // Find stock color.
    typeNameToColorFHash::iterator colorItr = mNameToColorF.find( colorName );

    // Return color if found.
    if ( colorItr != mNameToColorF.end() )
        return colorItr->value;

    // Warn.
    Con::warnf( "Could not find stock color name '%s'.", pStockColorName );

    // Return default stock color.
    return mNameToColorF.find( DEFAULT_UNKNOWN_STOCK_COLOR_NAME )->value;          
}

//-----------------------------------------------------------------------------

const ColorI& StockColor::colorI( const char* pStockColorName )
{
    // Sanity!
    AssertFatal( pStockColorName != NULL, "Cannot fetch a NULL stock color name." );

    // Fetch color name.
    StringTableEntry colorName = StringTable->insert( pStockColorName );

    // Find stock color.
    typeNameToColorIHash::iterator colorItr = mNameToColorI.find( colorName );

    // Return color if found.
    if ( colorItr != mNameToColorI.end() )
        return colorItr->value;

    // Warn.
    Con::warnf( "Could not find stock color name '%s'.", colorName );

    // Return default stock color.
    return mNameToColorI.find( DEFAULT_UNKNOWN_STOCK_COLOR_NAME )->value; 
}

//-----------------------------------------------------------------------------

StringTableEntry StockColor::name( const ColorF& color )
{
    // Find stock color name.
    typeColorFToNameHash::iterator colorNameItr = mColorFToName.find( color );

    // Return name if found.
    if ( colorNameItr != mColorFToName.end() )
        return colorNameItr->value;

    // Return empty string.
    return StringTable->EmptyString;
}

//-----------------------------------------------------------------------------

StringTableEntry StockColor::name( const ColorI& color )
{
    // Find stock color name.
    typeColorIToNameHash::iterator colorNameItr = mColorIToName.find( color );

    // Return name if found.
    if ( colorNameItr != mColorIToName.end() )
        return colorNameItr->value;

    // Return empty string.
    return StringTable->EmptyString;
}

//-----------------------------------------------------------------------------

S32 StockColor::getCount( void )
{
    return sizeof(StockColorTable) / sizeof(StockColorItem);
}

//-----------------------------------------------------------------------------

const StockColorItem* StockColor::getColorItem( const S32 index )
{
    // Fetch stock color count.
    const S32 stockColorCount = StockColor::getCount();

    // Is the stock color index in range?
    if ( index < 0 || index >= stockColorCount )
    {
        // No, so warn.
        Con::warnf("StockColor::getName() - Specified color index '%d' is out of range.  Range is 0 to %d.", index, stockColorCount-1 );
        return NULL;
    }

    // Return color name.
    return &(StockColorTable[index]);
}

//-----------------------------------------------------------------------------

ColorF::ColorF( const char* pStockColorName )
{
    // Set stock color.
    *this = StockColor::colorF( pStockColorName );
}

//-----------------------------------------------------------------------------

void ColorF::set( const char* pStockColorName )
{
    // Set stock color.
    *this = StockColor::colorF( pStockColorName );
}

//-----------------------------------------------------------------------------

const ColorF& ColorF::StockColor( const char* pStockColorName )
{
    return StockColor::colorF( pStockColorName );
}

//-----------------------------------------------------------------------------

StringTableEntry ColorF::StockColor( void )
{
    // Return stock color name.
    return StockColor::name( *this );
}

//-----------------------------------------------------------------------------

ColorI::ColorI( const char* pStockColorName )
{
    // Set stock color.
    *this = StockColor::colorI( pStockColorName );
}

//-----------------------------------------------------------------------------

void ColorI::set( const char* pStockColorName )
{
    // Set stock color.
    *this = StockColor::colorI( pStockColorName );
}

//-----------------------------------------------------------------------------

const ColorI& ColorI::StockColor( const char* pStockColorName )
{
    return StockColor::colorI( pStockColorName );
}

//-----------------------------------------------------------------------------

StringTableEntry ColorI::StockColor( void )
{
    // Return stock color name.
    return StockColor::name( *this );
}

//-----------------------------------------------------------------------------

ConsoleType( ColorF, TypeColorF, sizeof(ColorF), "" )

//-----------------------------------------------------------------------------

ConsoleGetType( TypeColorF )
{
    // Fetch color.
    const ColorF* color = (ColorF*)dptr;

    // Fetch stock color name.
    StringTableEntry colorName = StockColor::name( *color );

    // Write as color name if was found.
    if ( colorName != StringTable->EmptyString ) 
        return colorName;

    // Format as color components.
    char* returnBuffer = Con::getReturnBuffer(256);
    dSprintf(returnBuffer, 256, "%g %g %g %g", color->red, color->green, color->blue, color->alpha);
    return(returnBuffer);
}

//-----------------------------------------------------------------------------

ConsoleSetType( TypeColorF )
{
    ColorF* color = (ColorF*)dptr;

    if(argc == 1)
    {
        // Is only a single argument passed?
        if ( StringUnit::getUnitCount( argv[0], " " ) == 1 )
        {
            // Is this a stock color name?
            if ( !StockColor::isColor(argv[0]) )
            {
                // No, so warn.
                Con::warnf( "TypeColorF() - Invalid single argument of '%s' could not be interpreted as a stock color name.  Using default.", argv[0] );
            }

            // Set stock color (if it's invalid we'll get the default.
            color->set( argv[0] );

            return;
        }

        color->set(0, 0, 0, 1);
        F32 r,g,b,a;
        const S32 args = dSscanf(argv[0], "%g %g %g %g", &r, &g, &b, &a);
        color->red = r;
        color->green = g;
        color->blue = b;
        if (args == 4)
            color->alpha = a;
    }
    else if(argc == 3)
    {
        color->red    = dAtof(argv[0]);
        color->green  = dAtof(argv[1]);
        color->blue   = dAtof(argv[2]);
        color->alpha  = 1.f;
    }
    else if(argc == 4)
    {
        color->red    = dAtof(argv[0]);
        color->green  = dAtof(argv[1]);
        color->blue   = dAtof(argv[2]);
        color->alpha  = dAtof(argv[3]);
    }
    else
        Con::printf("Color must be set as { r, g, b [,a] }, { r g b [b] } or { stockColorName }");
}

//-----------------------------------------------------------------------------

ConsoleType( ColorI, TypeColorI, sizeof(ColorI), "" )

//-----------------------------------------------------------------------------

ConsoleGetType( TypeColorI )
{
    // Fetch color.
    ColorI* color = (ColorI*)dptr;

    // Fetch stock color name.
    StringTableEntry colorName = StockColor::name( *color );

    // Write as color name if was found.
    if ( colorName != StringTable->EmptyString ) 
        return colorName;

    // Format as color components.
    char* returnBuffer = Con::getReturnBuffer(256);
    dSprintf(returnBuffer, 256, "%d %d %d %d", color->red, color->green, color->blue, color->alpha);
    return returnBuffer;
}

//-----------------------------------------------------------------------------

ConsoleSetType( TypeColorI )
{
    ColorI* color = (ColorI*)dptr;
    if(argc == 1)
    {
        // Is only a single argument passed?
        if ( StringUnit::getUnitCount( argv[0], " " ) == 1 )
        {
            // Is this a stock color name?
            if ( !StockColor::isColor(argv[0]) )
            {
                // No, so warn.
                Con::warnf( "TypeColorF() - Invalid single argument of '%s' could not be interpreted as a stock color name.  Using default.", argv[0] );
            }

            // Set stock color (if it's invalid we'll get the default.
            color->set( argv[0] );

            return;
        }

        color->set(0, 0, 0, 255);
        S32 r,g,b,a;
        const S32 args = dSscanf(argv[0], "%d %d %d %d", &r, &g, &b, &a);
        color->red = r;
        color->green = g;
        color->blue = b;
        if (args == 4)
            color->alpha = a;
    }
    else if(argc == 3)
    {
        color->red    = dAtoi(argv[0]);
        color->green  = dAtoi(argv[1]);
        color->blue   = dAtoi(argv[2]);
        color->alpha  = 255;
    }
    else if(argc == 4)
    {
        color->red    = dAtoi(argv[0]);
        color->green  = dAtoi(argv[1]);
        color->blue   = dAtoi(argv[2]);
        color->alpha  = dAtoi(argv[3]);
    }
    else
        Con::printf("Color must be set as { r, g, b [,a] }, { r g b [b] }  or { stockColorName }");
}


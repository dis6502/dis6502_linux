#pragma once

#include "Syntax.h"
#include "PlatformCompat.h"

#include "tinyxml2.h"	// tinyxml2::...
#include <gsl/gsl>

#include "OutputStream.h"
#include "Byte.h"
#include "Word.h"


/*
 * Decisions:
 * - XML content is save as UTF-8
 * - XML filenames are specified as UTF-8
 * - ZIP elements?: NO, use additional ".gz" suffix later consistently for files
 * - Split configuration attributes into nested sections/elements in file?: NO, have grouping in code & UI only, allowes for moving attribute around
 * - Name of attributes in file: Cleanup attribute names and code and use them without type prefix then as they are.
 */

class XML {

#define GetBoolAttribute(ATTR, FIELD)               { auto szValue =  element.Attribute( #ATTR ); \
	                                                  if (szValue) { \
                                                        if (_stricmp(szValue,"true")==0) { FIELD=true; } \
                                                        if (_stricmp(szValue,"false")==0) { FIELD=false; } \
                                                      } \
                                                    }
#define GetByteAttribute(ATTR, FIELD)               { XML::ByteFromUTF8String(b##ATTR, element.Attribute( #ATTR )); }
#define GetByteArrayAttribute(ATTR, FIELD, SIZE)    { XML::ByteArrayFromUTF8String(FIELD, SIZE, element.Attribute( #ATTR )); }
#define GetIntAttribute(ATTR, FIELD)                { int nValue = FIELD; element.QueryIntAttribute( #ATTR, &nValue); FIELD = nValue; }
#define GetSizeAttribute(ATTR, FIELD)               { XML::SizeFromUTF8String(FIELD, element.Attribute( #ATTR )); }
#define GetStdStringAttribute(ATTR, FIELD)          { auto szValue =  element.Attribute( #ATTR ); \
                                                      if (szValue) { FIELD = XML::StringFromUTF8String(szValue); } \
                                                    }
#define GetWordAttribute(ATTR, FIELD)               { XML::WordFromUTF8String(FIELD, element.Attribute( #ATTR )); }

#define SetBoolAttribute(ATTR, FIELD)               { element.SetAttribute(#ATTR, (const char*)(FIELD?"true":"false")); }
#define SetByteAttribute(ATTR, FIELD)               { element.SetAttribute(#ATTR, (byte)(FIELD)); }
#define SetByteAttributeHex(ATTR, FIELD)            { element.SetAttribute(#ATTR, XML::ByteToUTF8HexString(FIELD, true).c_str()); }
#define SetByteArrayAttributeHex(ATTR, FIELD, SIZE) { element.SetAttribute(#ATTR,XML::ByteArrayToUTF8HexString(FIELD, SIZE, true).c_str()); }
#define SetIntAttribute(ATTR, FIELD)                { element.SetAttribute(#ATTR, (int)(FIELD)); }
#define SetSizeAttributeHex(ATTR, FIELD)            { element.SetAttribute(#ATTR,  XML::SizeToUTF8HexString(FIELD, true).c_str()); }
#define SetStdStringAttribute(ATTR, FIELD)          { element.SetAttribute(#ATTR, XML::StringToUTF8String(FIELD).c_str()); }
#define SetWordAttribute(ATTR, FIELD)               { element.SetAttribute(#ATTR, (word)(FIELD)); }
#define SetWordAttributeHex(ATTR, FIELD)            { element.SetAttribute(#ATTR,  XML::WordToUTF8HexString(static_cast<word>(FIELD), true).c_str()); }



public:
    typedef tinyxml2::XMLError     Error;
    typedef tinyxml2::XMLDocument  Document;
    typedef tinyxml2::XMLElement   Element;
    typedef tinyxml2::XMLAttribute Attribute;

    class Serializable {
    public:
        Serializable() = default;
        Serializable(const Serializable& a) = delete;			// copy constructor
        Serializable& operator=(const Serializable&) = delete;  // copy assignment
        Serializable(Serializable&&) = delete;                  // move constructor
        Serializable& operator=(Serializable&&) = delete;       // move assignment
        virtual ~Serializable() = default;  // Required, so sub-class destructor is called

        virtual void SerializeTo(XML::Element& element) const = 0;
        virtual void DeserializeFrom(const XML::Element& element) = 0;
    };

    static gsl::not_null<Element*> AddChildElement(Element& element, wstring_view lementName);
    static Error Load(Serializable& serializeable, wstring_view elementName, wstring_view filePath);
    static Error Save(const Serializable& serializeable, wstring_view elementName, wstring_view filePath); // Throws IOException
    static Error Save(const Serializable& serializeable, wstring_view elementName, OutputStream& outputStream); // Throws IOException

    static wstring StringFromUTF8String(string_view stringValue);
    static bool StringFromUTF8String(wstring& value, string_view stringValue);
    static bool ByteFromUTF8String(byte& value, string_view stringValue);
    static bool WordFromUTF8String(word& value, string_view stringValue);
    static bool SizeFromUTF8String(size_t& value, string_view stringValue);

    static bool ByteArrayFromUTF8String(byte*& valueArray, size_t& size, string_view stringValue);
    
    static string StringToUTF8String(wstring_view value);
    static string ByteToUTF8HexString(byte value, bool withPrefix);
    static string WordToUTF8HexString(word value, bool withPrefix);
    static string SizeToUTF8HexString(size_t value, bool withPrefix);
    static string ByteArrayToUTF8HexString(const byte* valueArray, size_t size, bool withPrefix);

};

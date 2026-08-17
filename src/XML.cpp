#include "DatatypeUtility.h"
#include "Strings.h"
#include "XML.h"

gsl::not_null<XML::Element*> XML::AddChildElement(Element& element, wstring_view elementName) {
    auto childElement = element.GetDocument()->NewElement(StringToUTF8String(elementName).c_str());
    element.InsertEndChild(childElement);

    return childElement;
}

XML::Error XML::Load(Serializable& serializeable, wstring_view elementName, wstring_view filePath) {
    Document document;
    Error result = document.LoadFile(StringToUTF8String(filePath).c_str());

    if (result == Error::XML_SUCCESS) {
        const gsl::not_null<tinyxml2::XMLElement*> element = document.RootElement();

        if (String::Equals(StringFromUTF8String(element->Name()), elementName)) {
            serializeable.DeserializeFrom(*element);
        }
        else {
            result = Error::XML_ERROR_MISMATCHED_ELEMENT;
        }
    }

    return result;
}

XML::Error XML::Save(const Serializable& serializeable, wstring_view elementName, wstring_view filePath) {
    Document document;
    document.SetBOM(true); // BOM is handled by the output stream

    const gsl::not_null<tinyxml2::XMLElement*> element = document.NewElement(StringToUTF8String(elementName).c_str());
    serializeable.SerializeTo(*element);
    document.InsertFirstChild(element);

    const auto result = document.SaveFile(StringToUTF8String(filePath).c_str());

    return result;
}

XML::Error XML::Save(const Serializable& serializeable, wstring_view elementName, OutputStream& outputStream) {
    Document document;
    document.SetBOM(false); // BOM is handled by the output stream with UTF-8 encoding already

    const gsl::not_null<tinyxml2::XMLElement*> element = document.NewElement(StringToUTF8String(elementName).c_str());
    serializeable.SerializeTo(*element);
    document.InsertFirstChild(element);

    // Clear any error from the last save, otherwise it will get reported for *this* call.
    document.ClearError();
    tinyxml2::XMLPrinter stream;
    document.Print(&stream);
    auto utf8String = string(stream.CStr());
    auto wstring = String::utf8_to_wstring(utf8String);
    outputStream.WriteString(wstring);
    return document.ErrorID();
}

wstring XML::StringFromUTF8String(string_view stringValue) {
    return String::utf8_to_wstring(stringValue);
}

bool XML::StringFromUTF8String(wstring& value, string_view stringValue) {
    value = StringFromUTF8String(stringValue);
    return true;
}

bool XML::ByteFromUTF8String(byte& value, string_view stringValue) {
    return DatatypeUtility::ByteFromString(value, String::utf8_to_wstring(stringValue));

}

bool XML::WordFromUTF8String(word& value, string_view stringValue) {
    return DatatypeUtility::WordFromString(value, String::utf8_to_wstring(stringValue));
}

bool XML::SizeFromUTF8String(size_t& value, string_view stringValue) {
    return DatatypeUtility::SizeFromString(value, String::utf8_to_wstring(stringValue));
}

bool XML::ByteArrayFromUTF8String(byte*& valueArray, size_t& size, string_view stringValue) {
    return DatatypeUtility::ByteArrayFromHexString(valueArray, size, String::utf8_to_wstring(stringValue));
}


string XML::StringToUTF8String(wstring_view value) {
    return String::wstring_to_utf8(value);
}

string XML::ByteToUTF8HexString(byte value, bool withPrefix) {
    return StringToUTF8String(DatatypeUtility::ByteToHexString(value, withPrefix));
}

string XML::WordToUTF8HexString(word value, bool withPrefix) {
    return StringToUTF8String(DatatypeUtility::WordToHexString(value, withPrefix));
}

string XML::SizeToUTF8HexString(size_t value, bool withPrefix) {
    return StringToUTF8String(DatatypeUtility::SizeToHexString(value, withPrefix));
}
string XML::ByteArrayToUTF8HexString(const byte* valueArray, size_t size, bool withPrefix) {
    return StringToUTF8String(DatatypeUtility::ByteArrayToHexString(valueArray, size, withPrefix));
}

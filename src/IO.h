#pragma once

#include "Syntax.h"
#include "String.h"

class IOException : public std::exception {
public:
    IOException(const wstringstream& stream) {
        this->text = stream.str();
    };

    IOException(const wstring_view text) {
        this->text = text;
    }

    IOException(const wchar_t* text) {
        this->text = text;
    }

    IOException(const wstring& text) {
        this->text = text;
    }

    // Note: the original Win32 build passed the message to
    // std::exception's (const char*) constructor, which is an MSVC-only
    // extension - libstdc++'s std::exception is default-construct-only,
    // so what() must be overridden explicitly here instead. This also
    // fixes what was otherwise dead code: every constructor above passed
    // an empty string to that base constructor, so on any implementation
    // GetText() (which calls what()) would have returned "" rather than
    // the actual message.
    const char* what() const noexcept override {
        narrowText = String::wstring_to_utf8(text);
        return narrowText.c_str();
    }

    wstring GetText() const;

private:
    wstring text;
    mutable string narrowText;
};

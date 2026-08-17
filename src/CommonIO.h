#pragma once

#include "Strings.h"
#include "Syntax.h"

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

    const char* what() const noexcept override {
        narrowText = String::wstring_to_utf8(text);
        return narrowText.c_str();
    }

    wstring GetText() const;

private:
    wstring text;
    mutable string narrowText;
};

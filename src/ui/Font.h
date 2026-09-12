#pragma once

#include <map>
#include <memory>
#include <Windows.h>


class Font
{

public:
    static constexpr HFONT NULL_HFONT = 0;

    Font(HFONT hFont);
    ~Font();

    static Font* GetInstance(HFONT hFont);


    HFONT hFont;

private:
    static std::map <HFONT, const std::unique_ptr<Font>> instances;

};



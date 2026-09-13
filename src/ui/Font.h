#pragma once

#include <map>
#include <memory>
#include <Windows.h>

class Font
{

public:

    Font();
    ~Font();

    static Font* GetInstance(HFONT hFont);

private:
    friend class Control;
    friend class DC;
    friend class Window;

    static constexpr HFONT NULL_HFONT = 0;
    static std::map <HFONT, const std::unique_ptr<Font>> instances;

    HFONT hFont;
};



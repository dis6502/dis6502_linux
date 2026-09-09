#include "Application.h"
#include "CommonIO.h"
#include "Debug.h"
#include "Resource.h"
#include "StringUtility.h"
#include "Syntax.h"
#include "Text.h"
#include <exception>
#include <string>


Application* g_Application = nullptr;

Application::Application() {}


Application::~Application() {
    settingsSections.clear();
}

wstring Application::GetText(Text::TextID textID) const {
    return std::to_wstring(textID);
}

void Application::SendLogMessage(const wchar_t* szText) {
    Debug::Log(szText);
}

void Application::SendInfoMessageWithID(Text::TextID textID, wstring_view v0, wstring_view v1, wstring_view v2, wstring_view v3, wstring_view v4, wstring_view v5, wstring_view v6, wstring_view v7, wstring_view v8, wstring_view v9) {
    auto text = Text::Format(textID, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);
    SendLogMessage(text.c_str());
}

void Application::SendErrorMessageWithID(Text::TextID textID, wstring_view v0, wstring_view v1, wstring_view v2, wstring_view v3, wstring_view v4, wstring_view v5, wstring_view v6, wstring_view v7, wstring_view v8, wstring_view v9) {
    auto text = Text::Format(textID, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);
    SendLogMessage(text.c_str());
}

void Application::ThrowErrorMessageWithID(Text::TextID textID, wstring_view v0, wstring_view v1, wstring_view v2, wstring_view v3, wstring_view v4, wstring_view v5, wstring_view v6, wstring_view v7, wstring_view v8, wstring_view v9) const {

    auto text = Text::Format(textID, v0, v1, v2, v3, v4, v5, v6, v7, v8, v9);
    throw IOException(text);
}
void Application::SendErrorMessageWithException(const std::exception& ex) {
    SendErrorMessageWithID(IDS_ERR_EXCEPTION, String::utf8_to_wstring(ex.what()));
}

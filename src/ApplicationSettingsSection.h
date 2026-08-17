#pragma once

#include "Syntax.h"

class Application;


class ApplicationSettingsSection {
public:
	ApplicationSettingsSection(wstring_view settingsFilePath, wstring_view name);

	void GetString(wstring_view keyName, wstring& value, wstring_view defaultValue = L"") const;
	void WriteString(wstring_view keyName, wstring_view value) const;
	void GetUnsignedInt(wstring_view keyName, unsigned int& value, unsigned int defaultValue = 0) const;


private:

	wstring settingsFilePath;
	wstring name;

};

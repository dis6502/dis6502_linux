#pragma once

#include "Syntax.h"

class Profile;
class ComputerSystemTypeInfo;
class Window;
class ApplicationSettingsSection;


class ProfileLogic {
public:
	ProfileLogic() = default;
	~ProfileLogic() = default;

	/*
	** Load profile from disk.
	*/
	void LoadDefaultProfile(Profile& profile, const ComputerSystemTypeInfo& computerSystemTypeInfo);
    bool LoadAndSetDefaultProfile(Profile& profile, const ComputerSystemTypeInfo& computerSystemTypeInfo, wstring_view filePath);
	bool Load(Profile& profile, wstring_view filePath);

	/*
	** Save profile to disk.
	*/
    void Save(const Profile& profile, wstring_view filePath);
};

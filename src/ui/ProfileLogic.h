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
	bool Load(Profile& profile, const ComputerSystemTypeInfo& computerSystemTypeInfo, const Window& parentWindow);
	bool Load(Profile& profile, wstring_view filePath);

	/*
	** Save profile to disk.
	*/
	void Save(const Profile& profile, const Window& parentWindow);
};

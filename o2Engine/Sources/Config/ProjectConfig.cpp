#include "ProjectConfig.h"

#include "EngineSettings.h"
#include "Utils/Debug.h"

namespace o2
{
	DECLARE_SINGLETON(ProjectConfig);

	ProjectConfig::ProjectConfig():
		mPlatform(GetEnginePlatform())
	{
		InitializeProperties();
		SetProjectPath(::GetProjectPath());
	}

	ProjectConfig::~ProjectConfig()
	{}

	String ProjectConfig::GetProjectName() const
	{
		return mProjectName;
	}

	void ProjectConfig::SetProjectName(const String& name)
	{
		mProjectName = name;
	}

	Platform ProjectConfig::GetPlatform() const
	{
		return mPlatform;
	}

	void ProjectConfig::SetPlatform(Platform platform)
	{
		mPlatform;
	}

	String ProjectConfig::GetProjectPath() const
	{
		return mProjectPath;
	}

	void ProjectConfig::SetProjectPath(const String& path)
	{
		String cfgFilePath = path + GetProjectSettingFileLocalPath();

		DataNode data;
		if (!data.LoadFromFile(cfgFilePath))
		{
			o2Debug.LogError("Failed to load Project Config. Path: %s. Initializing default values.", cfgFilePath);
			InitializeDefault(cfgFilePath);
			return;
		}
		else Deserialize(data);

		mProjectPath = path;
	}

	void ProjectConfig::InitializeDefault(const String& configFilePath)
	{
		mProjectName = "Unnamed";
		DataNode data = Serialize();
		data.SaveToFile(configFilePath);
	}

	void ProjectConfig::InitializeProperties()
	{
		INITIALIZE_PROPERTY(ProjectConfig, projectName, SetProjectName, GetProjectName);
		INITIALIZE_PROPERTY(ProjectConfig, currentPlatform, SetPlatform, GetPlatform);
		INITIALIZE_GETTER(ProjectConfig, projectPath, GetProjectPath);
	}
}

DECLARE_CLASS(o2::ProjectConfig);

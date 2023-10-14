#include "GameSwiftSdk.h"

#include "GameSwiftSdkSettings.h"
#include "MultipleLoginsBlocker.h"
#include "Utilities.h"
#include "Developer/Settings/Public/ISettingsModule.h"
#include "Developer/Settings/Public/ISettingsSection.h"

#define LOCTEXT_NAMESPACE "FGameSwiftSdkModule"

void FGameSwiftSdkModule::StartupModule()
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		TSharedPtr<ISettingsSection> SettingsSection = SettingsModule->RegisterSettings(
			"Project",
			"Plugins",
			"GameSwiftSdk",
			FText::FromString("GameSwiftSdk"),
			FText::FromString("UE plugin making it easy to connect with GameSwift ecosystem via GameSwift ID."),
			GetMutableDefault<UGameSwiftSdkSettings>());
	}

	if (FUtilities::IsMultipleLoginBlockingEnabled())
	{
		float Rate = FUtilities::GetMultipleLoginBlockingRate();
		if (Rate >= 10.0f)
		{
			FMultipleLoginsBlocker::InitializeBlocking(Rate);
		}
	}
}

void FGameSwiftSdkModule::ShutdownModule()
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "Plugins", "GameSwiftSdk");
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FGameSwiftSdkModule, GameSwiftSdk)

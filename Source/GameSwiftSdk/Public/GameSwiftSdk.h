#pragma once

#include "CoreMinimal.h"

class FGameSwiftSdkModule : public IModuleInterface
{
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};

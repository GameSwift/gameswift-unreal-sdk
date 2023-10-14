#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GameSwiftSdkSettings.generated.h"

UCLASS(config = Engine, defaultconfig)
class UGameSwiftSdkSettings : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(config, EditAnywhere, Category = "GameSwiftSdk")
	FString ClientId;

	UPROPERTY(config, EditAnywhere, Category = "GameSwiftSdk")
	FString ClientRedirectUri;

	UPROPERTY(config, EditAnywhere, Category = "GameSwiftSdk")
	FString ClientAuthenticationSecret;

	UPROPERTY(config, EditAnywhere, Category = "GameSwiftSdk")
	bool MultipleLoginsBlockerEnabled;

	UPROPERTY(config, EditAnywhere, Category = "GameSwiftSdk")
	float MultipleLoginsBlockerHeartbeatRate;
};

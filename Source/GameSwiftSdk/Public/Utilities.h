#pragma once
#include "GameSwiftSdkSettings.h"

class FUtilities
{
public:
	static void ReadCommandLineArgument(const char* ArgumentName, FString& ArgumentValue);
	static FString GetEncodedQueryString(const FString& UrlBase, const TMap<FString, FString>& Parameters);

	static FString GetClientSecret();
	static FString GetClientId();
	static FString GetRedirectUri();
	static bool IsMultipleLoginBlockingEnabled();
	static float GetMultipleLoginBlockingRate();
	static FString GetGameSwiftIdUri(FString EndpointAddress);
	static FString GetJsonString(TMap<FString, FString> Map);

private:
	static UGameSwiftSdkSettings* GetSdkSettings();
};

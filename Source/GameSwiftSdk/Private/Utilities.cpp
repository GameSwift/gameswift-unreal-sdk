#include "Utilities.h"
#include "GenericPlatformHttp.h"

void FUtilities::ReadCommandLineArgument(const char* ArgumentName, FString& ArgumentValue)
{
	FString CommandLineArgs = FCommandLine::Get();

	TArray<FString> ArgArray;
	CommandLineArgs.ParseIntoArray(ArgArray, TEXT(" "), true);

	for (int32 Index = 0; Index < ArgArray.Num() - 1; ++Index)
	{
		if (ArgArray[Index] == ArgumentName)
		{
			ArgumentValue = ArgArray[Index + 1];
			break;
		}
	}
}

FString FUtilities::GetEncodedQueryString(const FString& UrlBase, const TMap<FString, FString>& Parameters)
{
	FString QueryString;

	for (const TPair<FString, FString>& Param : Parameters)
	{
		if (QueryString.IsEmpty() == false)
		{
			QueryString += TEXT("&");
		}

		FString Key = FGenericPlatformHttp::UrlEncode(Param.Key);
		FString Value = FGenericPlatformHttp::UrlEncode(Param.Value);
		QueryString += Key + TEXT("=") + Value;
	}

	return UrlBase + QueryString;
}

FString FUtilities::GetClientSecret()
{
	return GetSdkSettings()->ClientAuthenticationSecret;
}

FString FUtilities::GetClientId()
{
	return GetSdkSettings()->ClientId;
	// return TEXT("7ac441a0-537d-4843-ba54-9aab05c7e8bf");
}

FString FUtilities::GetRedirectUri()
{
	return GetSdkSettings()->ClientRedirectUri;
	// return TEXT("https://id.gameswift.io/api/oauth/callback");
}

bool FUtilities::IsMultipleLoginBlockingEnabled()
{
	return GetSdkSettings()->MultipleLoginsBlockerEnabled;
}

float FUtilities::GetMultipleLoginBlockingRate()
{
	return GetSdkSettings()->MultipleLoginsBlockerHeartbeatRate;
}

UGameSwiftSdkSettings* FUtilities::GetSdkSettings()
{
	return GetMutableDefault<UGameSwiftSdkSettings>();
}

FString FUtilities::GetGameSwiftIdUri(FString EndpointAddress)
{
	return "https://id.gameswift.io/api/1/" + EndpointAddress;
}

FString FUtilities::GetJsonString(TMap<FString, FString> Map)
{
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

	for (const auto& KeyValue : Map)
	{
		const FString& Key = KeyValue.Key;
		const FString& Value = KeyValue.Value;

		JsonObject->SetStringField(Key, Value);
	}

	FString JsonString;
	TSharedRef<TJsonWriter<>> JsonWriter = TJsonWriterFactory<>::Create(&JsonString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JsonWriter);

	return JsonString;
}

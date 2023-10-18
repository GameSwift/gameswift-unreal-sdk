#pragma once

#include "CoreMinimal.h"
#include "Http.h"

struct FOAuthUserInfoResponse;
struct FBaseSdkFailResponse;
struct FBaseSuccessResponse;

class FRequestHandler
{
public:
#if ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION <= 24
	static TSharedRef<IHttpRequest> SendRequest(
#else
	static TSharedRef<IHttpRequest, ESPMode::ThreadSafe> SendRequest(
#endif
		FString FullUrl, FString Method, const FString callBody, const FString accessToken,
		bool ExpectsWwwResponse = false);

	static bool DecodeRequest(FHttpResponsePtr HttpResponse, bool bSucceeded, FJsonObject& ResultObject,
	                          FBaseSdkFailResponse& OutError);
	static bool DecodeRequestArray(FHttpResponsePtr HttpResponse, bool bSucceeded, TArray<FJsonObject>& ResultObject,
	                               FBaseSdkFailResponse& OutError);
	static FBaseSdkFailResponse DecodeError(TSharedPtr<FJsonObject> JsonObject);
};

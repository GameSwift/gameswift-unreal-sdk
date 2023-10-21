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
	using HttpRequestRefType = TSharedRef<IHttpRequest>;
#else
	using HttpRequestRefType = TSharedRef<IHttpRequest, ESPMode::ThreadSafe>;
#endif
	static HttpRequestRefType SendRequest(
		const FString& FullUrl,
		const FString& Method,
		const FString& callBody,
		const FString& accessToken,
		bool ExpectsWwwResponse = false
	);

	static bool DecodeRequest(FHttpResponsePtr HttpResponse, bool bSucceeded, FJsonObject& ResultObject,
	                          FBaseSdkFailResponse& OutError);
	static bool DecodeRequestArray(FHttpResponsePtr HttpResponse, bool bSucceeded, TArray<FJsonObject>& ResultObject,
	                               FBaseSdkFailResponse& OutError);
	static FBaseSdkFailResponse DecodeError(TSharedPtr<FJsonObject> JsonObject);
};

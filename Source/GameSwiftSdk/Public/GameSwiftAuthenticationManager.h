#pragma once
#include "CoreMinimal.h"
#include "Interfaces/IHttpRequest.h"
#include "BackendResponses.h"
#include "GameSwiftAuthenticationManager.generated.h"


UCLASS()
class GAMESWIFTSDK_API UGameSwiftAuthenticationManager : public UObject
{
	GENERATED_BODY()

public:
	static bool IsAuthorized();
	static FString AuthorizedAccessToken;

	UFUNCTION(BlueprintCallable, Category = "GameSwiftSdk | Authentication")
	static void AuthorizeAndReadUserInfoFromLauncher(FGetUserInfoDelegate SuccessDelegate,
	                                                 FBaseSdkFailDelegate ErrorDelegate);

	UFUNCTION(BlueprintCallable, Category = "GameSwiftSdk | Authentication")
	static void LoginAndAuthorize(FString EmailOrNickname, FString Password, FGetUserInfoDelegate SuccessDelegate,
	                              FBaseSdkFailDelegate ErrorDelegate);

	UFUNCTION(BlueprintCallable, Category = "GameSwiftSdk | Authentication")
	static void GetUserInfo(FGetUserInfoDelegate SuccessDelegate,
	                        FBaseSdkFailDelegate ErrorDelegate);

private:
	static void Authorize(FString AccessToken, FGetUserInfoDelegate SuccessDelegate,
	                      FBaseSdkFailDelegate ErrorDelegate);
	static void RetrieveOauthToken(const FString& Code, FGetUserInfoDelegate SuccessDelegate,
	                               FBaseSdkFailDelegate ErrorDelegate);
	static void OnAuthorized(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded,
	                         FGetUserInfoDelegate SuccessDelegate,
	                         FBaseSdkFailDelegate ErrorDelegate);

	static void OnTokenRetrieved(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded,
	                             FGetUserInfoDelegate SuccessDelegate,
	                             FBaseSdkFailDelegate ErrorDelegate);


	static void OnOauthUserInformationRetrieved(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse,
	                                            bool bSucceeded,
	                                            FGetUserInfoDelegate SuccessDelegate,
	                                            FBaseSdkFailDelegate ErrorDelegate);


	static void OnLogin(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded,
	                    FGetUserInfoDelegate SuccessDelegate,
	                    FBaseSdkFailDelegate ErrorDelegate);
};

#pragma once
#include "BackendResponses.h"
#include "Interfaces/IHttpRequest.h"
#include "GameSwiftWalletManager.generated.h"


UCLASS()
class GAMESWIFTSDK_API UGameSwiftWalletManager : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "GameSwiftSdk | Wallet")
	static void GetWallets(FGetWalletsDelegate SuccessDelegate, FBaseSdkFailDelegate ErrorDelegate);

	UFUNCTION(BlueprintCallable, Category = "GameSwiftSdk | Wallet")
	static void GetWalletBalance(FText WalletId, FGetWalletBalanceDelegate SuccessDelegate,
	                             FBaseSdkFailDelegate ErrorDelegate);

private:
	static void OnWalletsRetrieved(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded,
	                               FGetWalletsDelegate SuccessDelegate,
	                               FBaseSdkFailDelegate ErrorDelegate);
	static void OnWalletBalanceRetrieved(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded,
	                                     FGetWalletBalanceDelegate SuccessDelegate,
	                                     FBaseSdkFailDelegate ErrorDelegate);
};

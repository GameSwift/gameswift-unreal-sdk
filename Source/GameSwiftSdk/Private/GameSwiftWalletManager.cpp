#include "GameSwiftWalletManager.h"

#include "GameSwiftAuthenticationManager.h"
#include "RequestHandler.h"
#include "Utilities.h"

void UGameSwiftWalletManager::GetWallets(FGetWalletsDelegate SuccessDelegate, FBaseSdkFailDelegate ErrorDelegate)
{
	auto HttpRequest = FRequestHandler::SendRequest(FUtilities::GetGameSwiftIdUri("wallet"), TEXT("GET"), "",
	                                                UGameSwiftAuthenticationManager::AuthorizedAccessToken);
	HttpRequest->OnProcessRequestComplete().BindStatic(&UGameSwiftWalletManager::OnWalletsRetrieved, SuccessDelegate,
	                                                   ErrorDelegate);
	HttpRequest->ProcessRequest();
}

void UGameSwiftWalletManager::OnWalletsRetrieved(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse,
                                                 bool bSucceeded, FGetWalletsDelegate SuccessDelegate,
                                                 FBaseSdkFailDelegate ErrorDelegate)
{
	TArray<FJsonObject> ResultObject;
	FBaseSdkFailResponse errorResult;

	if (FRequestHandler::DecodeRequestArray(HttpResponse, bSucceeded, ResultObject, errorResult))
	{
		FWalletsResponse outResult;
		if (FWalletsResponse::Parse(ResultObject, outResult) == true)
		{
			SuccessDelegate.ExecuteIfBound(outResult);
		}
		else
		{
			FBaseSdkFailResponse ParseError = FBaseSdkFailResponse();
			ParseError.ErrorMessage = "Cannot parse wallets";
			ErrorDelegate.ExecuteIfBound(ParseError);
		}
	}
	else
	{
		ErrorDelegate.ExecuteIfBound(errorResult);
	}
}

void UGameSwiftWalletManager::GetWalletBalance(FText WalletId, FGetWalletBalanceDelegate SuccessDelegate,
                                               FBaseSdkFailDelegate ErrorDelegate)
{
	FString UriSuffix = "wallet/" + WalletId.ToString() + "/balance";
	auto HttpRequest = FRequestHandler::SendRequest(FUtilities::GetGameSwiftIdUri(UriSuffix), TEXT("GET"), "",
	                                                UGameSwiftAuthenticationManager::AuthorizedAccessToken);
	HttpRequest->OnProcessRequestComplete().BindStatic(&UGameSwiftWalletManager::OnWalletBalanceRetrieved,
	                                                   SuccessDelegate,
	                                                   ErrorDelegate);
	HttpRequest->ProcessRequest();
}


void UGameSwiftWalletManager::OnWalletBalanceRetrieved(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse,
                                                       bool bSucceeded, FGetWalletBalanceDelegate SuccessDelegate,
                                                       FBaseSdkFailDelegate ErrorDelegate)
{
	FJsonObject ResultObject;
	FBaseSdkFailResponse errorResult;

	if (FRequestHandler::DecodeRequest(HttpResponse, bSucceeded, ResultObject, errorResult))
	{
		FWalletBalanceResponse outResult;
		if (FWalletBalanceResponse::Parse(ResultObject, outResult))
		{
			SuccessDelegate.ExecuteIfBound(outResult);
		}
	}
	else
	{
		ErrorDelegate.ExecuteIfBound(errorResult);
	}
}

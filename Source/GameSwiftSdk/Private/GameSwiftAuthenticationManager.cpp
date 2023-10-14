#include "GameSwiftAuthenticationManager.h"
#include "BackendResponses.h"
#include "Utilities.h"
#include "HttpModule.h"
#include "RequestHandler.h"

FString UGameSwiftAuthenticationManager::AuthorizedAccessToken = FString();

bool UGameSwiftAuthenticationManager::IsAuthorized()
{
	return AuthorizedAccessToken.IsEmpty() == false;
}

void UGameSwiftAuthenticationManager::AuthorizeAndReadUserInfoFromLauncher(FGetUserInfoDelegate SuccessDelegate,
                                                                           FBaseSdkFailDelegate ErrorDelegate)
{
	FString CmdAccessToken;
	FUtilities::ReadCommandLineArgument("-access_token", CmdAccessToken);

	if (CmdAccessToken.IsEmpty())
	{
		FBaseSdkFailResponse Response = FBaseSdkFailResponse();
		Response.ErrorCode = 0;
		Response.ErrorMessage = "Cannot read -access_token cmd arg";
		ErrorDelegate.ExecuteIfBound(Response);
	}
	else
	{
		Authorize(CmdAccessToken, SuccessDelegate, ErrorDelegate);
	}
}

void UGameSwiftAuthenticationManager::Authorize(FString AccessToken, FGetUserInfoDelegate SuccessDelegate,
                                                FBaseSdkFailDelegate ErrorDelegate)
{
	TMap<FString, FString> Dictionary = {
		{"response_type", "code"},
		{"client_id", FUtilities::GetClientId()},
		{"redirect_uri", FUtilities::GetRedirectUri()},
		{"code_challenge_method", "S256"},
		{"state", "empty"},
	};


	FString AuthorizeUrl = FUtilities::GetEncodedQueryString("oauth/authorize?", Dictionary);


	auto HttpRequest = FRequestHandler::SendRequest(FUtilities::GetGameSwiftIdUri(AuthorizeUrl), "GET", "",
	                                                AccessToken);
	HttpRequest->OnProcessRequestComplete().BindStatic(&UGameSwiftAuthenticationManager::OnAuthorized,
	                                                   SuccessDelegate,
	                                                   ErrorDelegate);
	HttpRequest->ProcessRequest();
}

void UGameSwiftAuthenticationManager::OnAuthorized(FHttpRequestPtr _, FHttpResponsePtr HttpResponse,
                                                   bool bSucceeded, FGetUserInfoDelegate SuccessDelegate,
                                                   FBaseSdkFailDelegate ErrorDelegate)
{
	FJsonObject ResultObject;
	FBaseSdkFailResponse errorResult;

	if (FRequestHandler::DecodeRequest(HttpResponse, bSucceeded, ResultObject, errorResult))
	{
		FAuthorizeResponse outResult;
		if (FAuthorizeResponse::Parse(ResultObject, outResult))
		{
			RetrieveOauthToken(outResult.Code, SuccessDelegate, ErrorDelegate);
		}
	}
	else
	{
		ErrorDelegate.ExecuteIfBound(errorResult);
	}
}

void UGameSwiftAuthenticationManager::RetrieveOauthToken(const FString& Code, FGetUserInfoDelegate SuccessDelegate,
                                                         FBaseSdkFailDelegate ErrorDelegate)
{
	TMap<FString, FString> Dictionary = {
		{"client_id", FUtilities::GetClientId()},
		{"grant_type", "authorization_code"},
		{"code", Code},
		{"redirect_uri", FUtilities::GetRedirectUri()},
	};

	FString QueryString = FUtilities::GetEncodedQueryString("", Dictionary);

	auto HttpRequest = FRequestHandler::SendRequest(FUtilities::GetGameSwiftIdUri("oauth/token"), "POST",
	                                                QueryString, "", true);
	HttpRequest->OnProcessRequestComplete().BindStatic(&UGameSwiftAuthenticationManager::OnTokenRetrieved,
	                                                   SuccessDelegate,
	                                                   ErrorDelegate);
	HttpRequest->ProcessRequest();
}


void UGameSwiftAuthenticationManager::OnTokenRetrieved(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse,
                                                       bool bSucceeded,
                                                       FGetUserInfoDelegate SuccessDelegate,
                                                       FBaseSdkFailDelegate ErrorDelegate)
{
	FJsonObject ResultObject;
	FBaseSdkFailResponse errorResult;

	if (FRequestHandler::DecodeRequest(HttpResponse, bSucceeded, ResultObject, errorResult))
	{
		FTokenResponse outResult;
		if (FTokenResponse::Parse(ResultObject, outResult))
		{
			AuthorizedAccessToken = outResult.AccessToken;
			GetUserInfo(SuccessDelegate, ErrorDelegate);
		}
	}
	else
	{
		ErrorDelegate.ExecuteIfBound(errorResult);
	}
}


void UGameSwiftAuthenticationManager::GetUserInfo(FGetUserInfoDelegate SuccessDelegate,
                                                  FBaseSdkFailDelegate ErrorDelegate)
{
	auto HttpRequest = FRequestHandler::SendRequest(FUtilities::GetGameSwiftIdUri("oauth/me"), "GET", "",
	                                                AuthorizedAccessToken);
	HttpRequest->OnProcessRequestComplete().BindStatic(
		&UGameSwiftAuthenticationManager::OnOauthUserInformationRetrieved,
		SuccessDelegate, ErrorDelegate);
	HttpRequest->ProcessRequest();
}

void UGameSwiftAuthenticationManager::OnOauthUserInformationRetrieved(FHttpRequestPtr HttpRequest,
                                                                      FHttpResponsePtr HttpResponse,
                                                                      bool bSucceeded,
                                                                      FGetUserInfoDelegate SuccessDelegate,
                                                                      FBaseSdkFailDelegate ErrorDelegate)
{
	FJsonObject ResultObject;
	FBaseSdkFailResponse errorResult;

	if (FRequestHandler::DecodeRequest(HttpResponse, bSucceeded, ResultObject, errorResult))
	{
		FOAuthUserInfoResponse outResult;
		if (FOAuthUserInfoResponse::Parse(ResultObject, outResult))
		{
			SuccessDelegate.ExecuteIfBound(outResult);
		}
	}
	else
	{
		ErrorDelegate.ExecuteIfBound(errorResult);
	}
}

void UGameSwiftAuthenticationManager::LoginAndAuthorize(FString EmailOrNickname, FString Password,
                                                        FGetUserInfoDelegate SuccessDelegate,
                                                        FBaseSdkFailDelegate ErrorDelegate)
{
	TMap<FString, FString> Dictionary = {
		{"emailOrNickname", EmailOrNickname},
		{"password", Password},
	};
	FString RequestBody = FUtilities::GetJsonString(Dictionary);
	FString FullUrl = FUtilities::GetGameSwiftIdUri("auth/login");
	auto HttpRequest = FRequestHandler::SendRequest(FullUrl, "POST", RequestBody, "");
	HttpRequest->OnProcessRequestComplete().BindStatic(&UGameSwiftAuthenticationManager::OnLogin, SuccessDelegate,
	                                                   ErrorDelegate);
	HttpRequest->ProcessRequest();
}

void UGameSwiftAuthenticationManager::OnLogin(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse,
                                              bool bSucceeded, FGetUserInfoDelegate SuccessDelegate,
                                              FBaseSdkFailDelegate ErrorDelegate)
{
	FJsonObject ResultObject;
	FBaseSdkFailResponse errorResult;

	if (FRequestHandler::DecodeRequest(HttpResponse, bSucceeded, ResultObject, errorResult))
	{
		FLoginResponse outResult;
		if (FLoginResponse::Parse(ResultObject, outResult))
		{
			Authorize(outResult.AccessToken, SuccessDelegate, ErrorDelegate);
		}
	}
	else
	{
		ErrorDelegate.ExecuteIfBound(errorResult);
	}
}

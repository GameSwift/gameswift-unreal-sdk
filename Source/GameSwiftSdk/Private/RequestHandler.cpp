#include "RequestHandler.h"
#include "Serialization/JsonSerializer.h"
#include "BackendResponses.h"
#include "Utilities.h"


#if ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION <= 24
TSharedRef<IHttpRequest> FRequestHandler::SendRequest(
#else
TSharedRef<IHttpRequest, ESPMode::ThreadSafe> FRequestHandler::SendRequest(
#endif
	FString FullUrl, FString Method, const FString callBody, const FString accessToken, bool ExpectsWwwResponse)
{
	auto HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetVerb(Method);
	HttpRequest->SetURL(FullUrl);
	FString ContentType = ExpectsWwwResponse ? TEXT("application/x-www-form-urlencoded") : TEXT("application/json");
	HttpRequest->SetHeader(TEXT("Content-Type"), ContentType);
	HttpRequest->SetHeader(TEXT("gs-authentication"), FUtilities::GetClientSecret());

	if (accessToken != TEXT(""))
	{
		HttpRequest->SetHeader("Authorization", "Bearer " + accessToken);
	}

	HttpRequest->SetContentAsString(callBody);
	return HttpRequest;
}

bool FRequestHandler::DecodeRequest(FHttpResponsePtr HttpResponse, bool bSucceeded, FJsonObject& ResultObject,
                                    FBaseSdkFailResponse& OutError)
{
	if (bSucceeded && HttpResponse.IsValid())
	{
		FString ResponseStr = HttpResponse->GetContentAsString();

		if (EHttpResponseCodes::IsOk(HttpResponse->GetResponseCode()))
		{
			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(ResponseStr);


			if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
			{
				ResultObject = *JsonObject;
				return true;
			}
		}
		else
		{
			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(ResponseStr);

			if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
			{
				OutError = DecodeError(JsonObject);
				return false;
			}
		}
	}

	OutError.ErrorCode = 408;
	OutError.ErrorName = OutError.ErrorMessage = TEXT("Request Timeout or null response");
	return false;
}

bool FRequestHandler::DecodeRequestArray(FHttpResponsePtr HttpResponse, bool bSucceeded,
                                         TArray<FJsonObject>& ResultObject,
                                         FBaseSdkFailResponse& OutError)
{
	if (bSucceeded && HttpResponse.IsValid())
	{
		FString ResponseStr = HttpResponse->GetContentAsString();

		if (EHttpResponseCodes::IsOk(HttpResponse->GetResponseCode()))
		{
			ResponseStr = "{\"temp\":" + ResponseStr + "}";

			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(ResponseStr);
			if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
			{
				for (const TSharedPtr<FJsonValue> ArrayElement : JsonObject->GetArrayField("temp"))
				{
					ResultObject.Add(*ArrayElement->AsObject());
				}
				return true;
			}
		}
		else
		{
			TSharedPtr<FJsonObject> JsonObject;
			TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(ResponseStr);

			if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
			{
				OutError = DecodeError(JsonObject);
				return false;
			}
		}
	}

	OutError.ErrorCode = 408;
	OutError.ErrorName = OutError.ErrorMessage = TEXT("Request Timeout or null response");
	return false;
}

FBaseSdkFailResponse FRequestHandler::DecodeError(TSharedPtr<FJsonObject> JsonObject)
{
	FBaseSdkFailResponse OutError = FBaseSdkFailResponse();

	JsonObject->TryGetNumberField(TEXT("statusCode"), OutError.ErrorCode);
	JsonObject->TryGetStringField(TEXT("message"), OutError.ErrorMessage);
	JsonObject->TryGetStringField(TEXT("error"), OutError.ErrorName);

	return OutError;
}

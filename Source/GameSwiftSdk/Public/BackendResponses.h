#pragma once

#include "CoreMinimal.h"
#include "BackendResponses.generated.h"


USTRUCT(BlueprintType)
struct GAMESWIFTSDK_API FBaseSdkFailResponse
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly)
	int32 ErrorCode;
	UPROPERTY(BlueprintReadOnly)
	FString ErrorName;
	UPROPERTY(BlueprintReadOnly)
	FString ErrorMessage;
};

USTRUCT(BlueprintType)
struct GAMESWIFTSDK_API FOAuthUserInfoResponse
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly)
	FString UserId;
	UPROPERTY(BlueprintReadOnly)
	FString Nickname;
	UPROPERTY(BlueprintReadOnly)
	FString AvatarUrl;
	UPROPERTY(BlueprintReadOnly)
	FString Country;

	static bool Parse(const FJsonObject& JsonObject, FOAuthUserInfoResponse& ParsedResponse);
};


struct GAMESWIFTSDK_API FAuthorizeResponse
{
	FString Code;

	static bool Parse(const FJsonObject& JsonObject, FAuthorizeResponse& ParsedResponse);
};


struct GAMESWIFTSDK_API FTokenResponse
{
	FString AccessToken;
	FString RefreshToken;

	static bool Parse(const FJsonObject& JsonObject, FTokenResponse& ParsedResponse);
};


USTRUCT(BlueprintType)
struct GAMESWIFTSDK_API FChain
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly)
	FString ChainId;


	UPROPERTY(BlueprintReadOnly)
	FString ChainName;
};


USTRUCT(BlueprintType)
struct GAMESWIFTSDK_API FGame
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly)
	FString GameId;
};


USTRUCT(BlueprintType)
struct GAMESWIFTSDK_API FWallet
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly)
	FString Address;

	UPROPERTY(BlueprintReadOnly)
	TArray<FGame> Games;

	UPROPERTY(BlueprintReadOnly)
	FString Name;

	UPROPERTY(BlueprintReadOnly)
	FString WalletId;

	UPROPERTY(BlueprintReadOnly)
	FChain Chain;
};

USTRUCT(BlueprintType)
struct GAMESWIFTSDK_API FWalletsResponse
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly)
	TArray<FWallet> Wallets;

	static bool Parse(const TArray<FJsonObject> JsonObject, FWalletsResponse& ParsedResponse);
};

USTRUCT(BlueprintType)
struct GAMESWIFTSDK_API FNft
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly)
	FString Address;
	
	UPROPERTY(BlueprintReadOnly)
	float Balance;
	
	UPROPERTY(BlueprintReadOnly)
	FString Id;
};

USTRUCT(BlueprintType)
struct GAMESWIFTSDK_API FToken
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly)
	FString Address;
	
	UPROPERTY(BlueprintReadOnly)
	float Balance;
};


USTRUCT(BlueprintType)
struct GAMESWIFTSDK_API FWalletBalanceResponse
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(BlueprintReadOnly)
	TArray<FNft> Nfts;
	
	UPROPERTY(BlueprintReadOnly)
	TArray<FToken> Tokens;

	static bool Parse(const FJsonObject& JsonObject, FWalletBalanceResponse& ParsedResponse);
};

struct GAMESWIFTSDK_API FLoginResponse
{
	FString UserId;
	FString Email;
	FString Nickname;
	FString AccessToken;

	static bool Parse(const FJsonObject& JsonObject, FLoginResponse& ParsedResponse);
};


DECLARE_DYNAMIC_DELEGATE_OneParam(FBaseSdkFailDelegate, const FBaseSdkFailResponse&, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FGetUserInfoDelegate, const FOAuthUserInfoResponse&, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FGetWalletsDelegate, const FWalletsResponse&, Response);

DECLARE_DYNAMIC_DELEGATE_OneParam(FGetWalletBalanceDelegate, const FWalletBalanceResponse&, Response);

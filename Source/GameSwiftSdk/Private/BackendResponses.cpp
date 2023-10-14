#include "BackendResponses.h"

bool FOAuthUserInfoResponse::Parse(const FJsonObject& JsonObject, FOAuthUserInfoResponse& ParsedResponse)
{
	bool SuccessfulRead = true;
	SuccessfulRead &= JsonObject.TryGetStringField("userId", ParsedResponse.UserId);
	SuccessfulRead &= JsonObject.TryGetStringField("nickname", ParsedResponse.Nickname);
	SuccessfulRead &= JsonObject.TryGetStringField("avatarUrl", ParsedResponse.AvatarUrl);
	SuccessfulRead &= JsonObject.TryGetStringField("country", ParsedResponse.Country);
	return SuccessfulRead;
}

bool FAuthorizeResponse::Parse(const FJsonObject& JsonObject, FAuthorizeResponse& ParsedResponse)
{
	bool SuccessfulRead = true;
	SuccessfulRead &= JsonObject.TryGetStringField("code", ParsedResponse.Code);
	return SuccessfulRead;
}

bool FTokenResponse::Parse(const FJsonObject& JsonObject, FTokenResponse& ParsedResponse)
{
	bool SuccessfulRead = true;
	SuccessfulRead &= JsonObject.TryGetStringField("access_token", ParsedResponse.AccessToken);
	SuccessfulRead &= JsonObject.TryGetStringField("refresh_token", ParsedResponse.RefreshToken);
	return SuccessfulRead;
}

bool FWalletsResponse::Parse(const TArray<FJsonObject> JsonObject, FWalletsResponse& ParsedResponse)
{
	bool SuccessfulRead = true;

	for (const auto WalletElement : JsonObject)
	{
		FWallet Wallet = FWallet();
		SuccessfulRead &= WalletElement.TryGetStringField("address", Wallet.Address);
		SuccessfulRead &= WalletElement.TryGetStringField("name", Wallet.Name);
		SuccessfulRead &= WalletElement.TryGetStringField("walletId", Wallet.WalletId);

		TArray<FGame> Games;
		TArray<TSharedPtr<FJsonValue>> GamesArray = WalletElement.GetArrayField("games");
		for (const TSharedPtr<FJsonValue>& GameValue : GamesArray)
		{
			FGame Game = FGame();
			TSharedPtr<FJsonObject> GameObject = GameValue->AsObject();
			Game.GameId = GameObject->GetStringField("gameId");
			Games.Add(Game);
		}
		Wallet.Games = Games;

		FChain Chain = FChain();
		TSharedPtr<FJsonObject> ChainObject = WalletElement.GetObjectField("chain");
		Chain.ChainId = ChainObject->GetStringField("chainId");
		Chain.ChainName = ChainObject->GetStringField("chainName");
		Wallet.Chain = Chain;

		ParsedResponse.Wallets.Add(Wallet);
	}

	return SuccessfulRead;
}

bool FWalletBalanceResponse::Parse(const FJsonObject& JsonObject, FWalletBalanceResponse& ParsedResponse)
{
	bool SuccessfulRead = true;

	const TArray<TSharedPtr<FJsonValue>>* NftsArray;
	SuccessfulRead &= JsonObject.TryGetArrayField("nfts", NftsArray);
	for (const TSharedPtr<FJsonValue>& NftValue : *NftsArray)
	{
		FNft Nft = FNft();
		TSharedPtr<FJsonObject> GameObject = NftValue->AsObject();
		Nft.Address = GameObject->GetStringField("address");
		Nft.Balance = GameObject->GetNumberField("balance");
		Nft.Id = GameObject->GetStringField("id");
		ParsedResponse.Nfts.Add(Nft);
	}

	const TArray<TSharedPtr<FJsonValue>>* TokensArray;
	SuccessfulRead &= JsonObject.TryGetArrayField("tokens", TokensArray);
	for (const TSharedPtr<FJsonValue>& TokenValue : *TokensArray)
	{
		FToken Token = FToken();
		TSharedPtr<FJsonObject> GameObject = TokenValue->AsObject();
		Token.Address = GameObject->GetStringField("address");
		Token.Balance = GameObject->GetNumberField("balance");
		ParsedResponse.Tokens.Add(Token);
	}

	return SuccessfulRead;
}

bool FLoginResponse::Parse(const FJsonObject& JsonObject, FLoginResponse& ParsedResponse)
{
	bool SuccessfulRead = true;
	SuccessfulRead &= JsonObject.TryGetStringField("userId", ParsedResponse.UserId);
	SuccessfulRead &= JsonObject.TryGetStringField("email", ParsedResponse.Email);
	SuccessfulRead &= JsonObject.TryGetStringField("nickname", ParsedResponse.Nickname);
	SuccessfulRead &= JsonObject.TryGetStringField("accessToken", ParsedResponse.AccessToken);
	return SuccessfulRead;
}

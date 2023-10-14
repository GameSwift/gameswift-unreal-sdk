#include "MultipleLoginsBlocker.h"

#include "GameSwiftAuthenticationManager.h"

FTickerDelegate FMultipleLoginsBlocker::OnHeartbeatDelegate;

void FMultipleLoginsBlocker::InitializeBlocking(const float HeartbeatRate)
{
	OnHeartbeatDelegate.BindStatic(&FMultipleLoginsBlocker::HeartbeatAction);
	
#if ENGINE_MAJOR_VERSION == 5
	FTSTicker::
#else
	FTicker::
#endif
	GetCoreTicker().AddTicker(OnHeartbeatDelegate, HeartbeatRate);
}

bool FMultipleLoginsBlocker::HeartbeatAction(float DeltaTime)
{
	if (UGameSwiftAuthenticationManager::IsAuthorized())
	{
		const FGetUserInfoDelegate SuccessDelegate;
		const FBaseSdkFailDelegate FailDelegate;
		UGameSwiftAuthenticationManager::GetUserInfo(SuccessDelegate, FailDelegate);
	}
	return true;
}

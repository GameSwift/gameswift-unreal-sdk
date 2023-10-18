#pragma once

class FMultipleLoginsBlocker
{
public:
	static void InitializeBlocking(float HeartbeatRate);

private:
	static FTickerDelegate OnHeartbeatDelegate;
	static bool HeartbeatAction(float DeltaTime);
};

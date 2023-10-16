# Overview

GameSwift Unreal SDK is a toolset created to to help you seamlessly
integrate [GameSwift ecosystem](https://platform.gameswift.io/) into your Unreal Engine projects. Key features of
GameSwift Unreal SDK are:

* **Rapid integration**

  Installation and integration process is streamlined and can be completed in under 30 minutes. This means you can focus
  more on building your game and less on complex API integration.

* **Safety**

  Firstly, [GameSwift Launcher](https://launcher.gameswift.io/) is equipped with a robust DRM system that guarantees
  players launch your game without altering any game files.
  Secondly, [Multiple Logins Blocker](#multiple-logins-blocker) ensures that a user account is logged in from exactly
  one device at a time. Both of these features prevent various types of abuses and help you maintain a secure and fair
  gaming environment.

* **Blueprint support**

  All SDK features can be accessed both in C++ code and in the Unreal Engine Blueprint system. This versatility
  ensures that not only programmers but also non-coding individuals can work on GameSwift ecosystem integration.

* **Multiplatform support**

  The wrapper is designed to work seamlessly on multiple platforms, making it adaptable for a variety of game
  development projects. You can bring the best gaming experiences with GameSwift Unreal SDK for Windows, MacOS and
  mobile.

GameSwift Unreal SDK is compatible with Unreal Engine versions 4.24 and beyond. We are committed to keeping
it up-to-date with newer Unreal Engine releases, including Unreal Engine 5 and upcoming versions.

# Installation

1. Add SDK to you project. This can be done in two ways:
    * Add SDK as [git submodule](https://git-scm.com/book/en/v2/Git-Tools-Submodules) to your repository by invoking the
      following command:
      ```bash
      git submodule add https://github.com/GameSwift/gameswift-unreal-sdk YourMainProjectDir/Plugins/GameSwiftUnrealSdk
      ```
    * Download this repository contents
      from [newest release](https://github.com/GameSwift/gameswift-unreal-sdk/releases/) and unpack the plugin to
      the `Plugins/GameSwiftUnrealSdk` folder. If you don't have a `Plugins` folder in your main project directory yet,
      create it.
2. Open Project Settings in the editor and navigate to `Plugins > GameSwiftSdk`. Fill in following
   fields: `ClientId`, `ClientRedirectUri`, `ClientAuthenticationSecret`. These secrets
   are [distributed by GameSwift](#contact-us).

   ![Plugin secrets](https://cdn.gameswift.io/gameswift/images/sdk/unreal_sdk/ue_plugin.png)
3. Add `GameSwiftSdk` to `PublicDependencyModuleNames` in your main `*.Build.cs` file.

# Integration

You can handle GameSwift login in 2 ways: [with launcher](#logging-in-from-launcher)
or [without launcher](#logging-in-without-launcher). You can download GameSwift
launcher [here](https://launcher.gameswift.io/). As long as your game targets Windows or MacOS, we strongly recommend to
use data passed from our launcher. By doing so, you won't need to implement any login screen for your game as launcher
already handles user credentials ina secure way.If you are building for mobile, you will need to create a login screen
and implement connection with GameSwift backend [the other way](#logging-in-without-launcher).

No matter which approach you choose (C++ or Blueprints), be sure that `AuthorizeAndReadUserInfoFromLauncher`
or `LoginAndAuthorize` is called at the application startup.This method wil succeed provided that:

1. User has internet connection.
2. User did not attempt to login simultaneously from multiple devices (applicable only for
   applications with [Multiple Logins Blocker](#multiple-logins-blocker) enabled).
3. User launched the game via launcher instead of your game executable file (provided that you
   used [with launcher](#logging-in-from-launcher) flow).

Otherwise, you should retry calling this method or close the application as it may not be launched the intended way.

## Launcher authentication - C++

Authorizing the user and getting his info can be done by
calling `UGameSwiftAuthenticationManager::AuthorizeAndReadUserInfoFromLauncher`.

```cpp
void ASampleCharacter::ReadUserInfoFromLauncher()
{
	FGetUserInfoDelegate SuccessDelegate;
	SuccessDelegate.BindUFunction(this, "OnSuccess");
	FBaseSdkFailDelegate FailDelegate;
	FailDelegate.BindUFunction(this, "OnError");
	
	UGameSwiftAuthenticationManager::AuthorizeAndReadUserInfoFromLauncher(SuccessDelegate, FailDelegate);
}

void ASampleCharacter::OnSuccess(const FOAuthUserInfoResponse& Response)
{
	// success - you can read user nickname and other data from Response
}

void ASampleCharacter::OnError(const FBaseSdkFailResponse& Response)
{
	// fail - close the application or retry authorization
}
```

## Launcher authentication - Blueprint

`AuthorizeAndReadUserInfoFromLauncher` node is responsible for authorizing the user, who accessed the game through
GameSwift Launcher.

![Launcher login with Blueprints](https://cdn.gameswift.io/gameswift/images/sdk/unreal_sdk/blueprint_launcher.png)

## Non-launcher authentication - C++

With this approach you need to create a custom login widget, which will enable user to input the login data and
authorize.

Authorizing the user and getting his info can be done by calling `UGameSwiftAuthenticationManager::LoginAndAuthorize`.

```cpp
void ASampleCharacter::LoginAndAuthorize()
{
	Fstring EmailOrNickname = "bob.smith@example.com";
	Fstring Password = "Password123";
	FGetUserInfoDelegate SuccessDelegate;
	SuccessDelegate.BindUFunction(this, "OnSuccess");
	FBaseSdkFailDelegate FailDelegate;
	FailDelegate.BindUFunction(this, "OnError");
	
	UGameSwiftAuthenticationManager::LoginAndAuthorize(EmailOrNickname, Password, 
	SuccessDelegate, FailDelegate);
}

void ASampleCharacter::OnSuccess(const FOAuthUserInfoResponse& Response)
{
	// success - you can read user nickname and other data from Response
}

void ASampleCharacter::OnError(const FBaseSdkFailResponse& Response)
{
	// fail - close the application or retry authorization
}
```

## Non-launcher authentication - Blueprint

`GetUserCredentials` node in the sample below is your implementation of retrieving login data from your custom login
widget. Then, you can pass retrieved data to `LoginAndAuthorize` node.

![Non launcher login with Blueprints](https://cdn.gameswift.io/gameswift/images/sdk/unreal_sdk/blueprint_nolauncher.png)

## Multiple Logins Blocker

Multiple Logins Blocker is an optional, powerful feature of GameSwift Unreal SDK that ensures a user's account can
only be logged in on one device at a time. This component's setup is done in Project Settings
under `Plugins > GameSwiftSdk`.

If enabled, this component works in background of your game, sending periodic heartbeats to GameSwift server at time
intervals specified by you. When server receives these heartbeats, it recognizes that the user is already logged in and
will block any additional login attempts from this user for a configurable duration. If you want to configure the lock
duration, feel free to [contact us](#contact-us).

# Contact Us

If you have any questions, concerns, or feedback, please don't hesitate to reach out to GameSwift team. We're here to
assist you and can be contacted via email at [piotr.sobiecki@gameswift.io](mailto:piotr.sobiecki@gameswift.io).

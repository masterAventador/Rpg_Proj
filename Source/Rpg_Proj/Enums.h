#pragma once

UENUM(Blueprintable)
enum class EAssassinationType : uint8
{
	KickBall UMETA(DisplayName="KickBall"),
	LockThroat UMETA(DisplayName="LockThroat"),

	MAX UMETA(DisplayName="DefaultMax")
};
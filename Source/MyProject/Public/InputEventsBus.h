// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "InputEventsBus.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnKeyPressed, const FKey&, Key);

UCLASS()
class MYPROJECT_API UInputEventsBus : public UObject
{
	GENERATED_BODY()
public:
	void HandleKeyDown(const FKey& Key) const;

	UPROPERTY(BlueprintAssignable, Category = "Input events")
	FOnKeyPressed OnKeyPressed;
};

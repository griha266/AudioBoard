// Fill out your copyright notice in the Description page of Project Settings.


#include "InputEventsBus.h"

void UInputEventsBus::HandleKeyDown(const FKey& Key) const
{
	OnKeyPressed.Broadcast(Key);
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Runtime/UMG/Public/Components/SpinBox.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Runtime/UMG/Public/Components/EditableText.h"
#include "PuzzleProjInteregerEditableText.generated.h"

// This class does not need to be modified.
UCLASS(Blueprintable, BlueprintType)
class UPuzzleProjInteregerEditableText : public UEditableText
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input Data")
		int32 IntegerValue;

public:

	UPuzzleProjInteregerEditableText();

	//~ Begin UWidget Interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
	//// End of UWidget

	UFUNCTION(BlueprintCallable, Category = "User Input")
		void MyHandleOnTextChanged(const FText& InText);

	UFUNCTION(BlueprintCallable, Category = "User Input")
		bool ValidateIntegerValue(FText& InText);

	UFUNCTION(BlueprintCallable, Category = "User Input")
		int32 GetIntFromFloat(const float& FloatValue);

	UFUNCTION(BlueprintCallable, Category = "User Input")
		void SetValue(int32 value);
};


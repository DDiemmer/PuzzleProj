// Fill out your copyright notice in the Description page of Project Settings.

#include "PuzzleProjInteregerEditableText.h"
#include "Runtime/Slate/Public/SlateFwd.h"
#include "Runtime/Slate/Public/Widgets/Input/SEditableText.h"

UPuzzleProjInteregerEditableText::UPuzzleProjInteregerEditableText()
	:
	UEditableText(),
	//set your default value to whatever you like
	IntegerValue(0)
{
	//update the default Text value to reflect your integer value
	Text = FText::AsNumber(IntegerValue);

}

TSharedRef<SWidget> UPuzzleProjInteregerEditableText::RebuildWidget()
{
	//this is a duplication of the UEditableText RebuildWidget method, but
	//with our MyHandleOnTextChanged method bound to OnValueChanged
	MyEditableText = SNew(SEditableText)
		.Style(&WidgetStyle)
		.MinDesiredWidth(MinimumDesiredWidth)
		.IsCaretMovedWhenGainFocus(IsCaretMovedWhenGainFocus)
		.SelectAllTextWhenFocused(SelectAllTextWhenFocused)
		.RevertTextOnEscape(RevertTextOnEscape)
		.ClearKeyboardFocusOnCommit(ClearKeyboardFocusOnCommit)
		.SelectAllTextOnCommit(SelectAllTextOnCommit)
		//here we replace HandleOnValueChanged with our MyHandleOnTextChanged method
		.OnTextChanged(BIND_UOBJECT_DELEGATE(FOnTextChanged, MyHandleOnTextChanged))//<-- 
		.OnTextCommitted(BIND_UOBJECT_DELEGATE(FOnTextCommitted, HandleOnTextCommitted))
		.VirtualKeyboardType(EVirtualKeyboardType::AsKeyboardType(KeyboardType.GetValue()));
	

	return BuildDesignTimeWidget(MyEditableText.ToSharedRef());

}

void UPuzzleProjInteregerEditableText::MyHandleOnTextChanged(const FText& InText)
{
	//the native HandleOnTextChanged takes a constant, but we 
	//may be changing this value so we'll convert it to something editable
	FText NewText(InText);
	//We use ValidateIntegerValue to determine if the value has actually changed and is a valid integer
	if (ValidateIntegerValue(NewText)) {
		//important to tie back into the OnTextChanged event with the validated value
		HandleOnTextChanged(NewText);
		//natively HandleOnTextChanged just broadcasts the OnTextChanged event; we could just
		//do that here, but I always prefer to go back to the native method if possible so
		//any future engine updates will seemlessly integrate (hopefully)

	}

}

//this verifies the passed in text can be interpreted as an integer and cleans the value
bool UPuzzleProjInteregerEditableText::ValidateIntegerValue(FText& InText) {
	//checking to make sure the value is different prevents an infinite loop via SetText
	if (!InText.EqualTo(Text)) {
		if (InText.IsNumeric()) {
			//convert the value to a float so we can round properly if the user entered a decimal
			float FloatValue = FCString::Atof(*InText.ToString());
			//set our integer value
			IntegerValue = GetIntFromFloat(FloatValue);
			//you can use FMath::CeilToInt or FMath::Floor if you prefer
			//IntegerValue = FMath::CeilToInt(FloatValue);
			//IntegerValue = FMath::Floor(FloatValue);

			//then set our new integer back to the text value
			InText = FText::AsNumber(IntegerValue);

			FString convertedString = InText.ToString();

			int32 indexOfChange = convertedString.Find(",");
			if (indexOfChange > 0) {
				convertedString = convertedString.Replace(TEXT(","),TEXT(""));
				InText = FText::FromString(convertedString);
			}
			indexOfChange = convertedString.Find(".");
			if (indexOfChange > 0 )
			{
				convertedString = convertedString.Replace(TEXT("."), TEXT(""));
				InText = FText::FromString(convertedString);
			}
			if (convertedString.Len() > 4) {
				InText = FText::FromString("9999");
			}
			//then we have to set the value back to the SEditableText object
			SetText(InText);
			return true;

		}
		else if (InText.IsEmpty()) {
			//handle the special case of deleting the last digit.
			//here I just set everything to 0; you could set it to a stored default, or whatever
			//I find this to be nicer behavior than refusing to delete the last digit;
			//unfortunately, if the last digit is 0 it still appears to be non-responsive
			IntegerValue = 0;
			SetText(FText::AsNumber(IntegerValue));
			return true;
		}
		else {
			//if we haven't been given a good integer value, set the text value back to our current
			//stored integer value; you can, of course, modify this behavior if you desire
			SetText(FText::AsNumber(IntegerValue));

		}

	}
	return false;

}

//converts referenced float to integer value with proper rounding rules
int32 UPuzzleProjInteregerEditableText::GetIntFromFloat(const float& FloatValue) {
	if (FloatValue >= 0.0f) {
		return (int)(FloatValue + 0.5f);

	}
	return (int)(FloatValue - 0.5f);
}

void UPuzzleProjInteregerEditableText::SetValue(int32 InteregerValue)
{
	FString IntAsString = FString::FromInt(InteregerValue);
	FText StringAsText = FText::FromString(IntAsString);
	UPuzzleProjInteregerEditableText::ValidateIntegerValue(StringAsText);

		//SetText(FText::AsNumber(InteregerValue));
}


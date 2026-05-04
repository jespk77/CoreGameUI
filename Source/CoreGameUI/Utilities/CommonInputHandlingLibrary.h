#pragma once

#include "Components/SlateWrapperTypes.h"
#include "CommonInputHandlingLibrary.generated.h"

UCLASS()
class COREGAMEUI_API UCommonInputHandlingLibrary : public UBlueprintFunctionLibrary {
	GENERATED_BODY()

public:
	UFUNCTION(Category = "Input Handling", BlueprintCallable)
	static FEventReply ConvertBoolToReply(const bool handled) { return FEventReply(handled); }
	static FReply ConvertBoolToNativeReply(const bool handled) { return handled ? FReply::Handled() : FReply::Unhandled(); }

	UFUNCTION(Category = "Input Handling", BlueprintCallable)
	static FEventReply BlockNonNumericCharactersHandler(const FGeometry& geometry, const FCharacterEvent& event) {
		const TCHAR character = event.GetCharacter();
		if (FChar::IsDigit(character) || 
			character == LITERAL(TCHAR, '\b') || 
			character == LITERAL(TCHAR, '.') ||
			character == LITERAL(TCHAR, ','))
			return ConvertBoolToReply(false);
		else return ConvertBoolToReply(true);
	}
	static FReply NativeBlockNonNumericCharactersHandler(const FGeometry& geometry, const FCharacterEvent& event) {
		return BlockNonNumericCharactersHandler(geometry, event).NativeReply;
	}
};

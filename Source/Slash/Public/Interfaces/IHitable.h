#pragma once

#include <CoreMinimal.h>
#include <UObject/Interface.h>
#include <IHitable.generated.h>


UINTERFACE(MinimalAPI)
class UHitable : public UInterface {
	GENERATED_BODY()
};

class SLASH_API IHitable {
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintNativeEvent)
	void Hit(const FVector& impactPoint);
};

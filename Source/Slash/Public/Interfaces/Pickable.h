#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Pickable.generated.h"

UINTERFACE(MinimalAPI)
class UPickable : public UInterface {
	GENERATED_BODY()
};

class SLASH_API IPickable {
	GENERATED_BODY()
public:

	virtual void SetOverlappingItem(class AItems* item);
	virtual void AddSouls(class ASoul* souls);
	virtual void AddGold(class ACollectable* collectable);
};

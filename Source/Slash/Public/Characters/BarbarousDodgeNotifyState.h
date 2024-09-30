#pragma once

#include <CoreMinimal.h>
#include <Animation/AnimNotifies/AnimNotifyState.h>
#include <BarbarousDodgeNotifyState.generated.h>

class ABarbarousPlayer;

UCLASS()
class SLASH_API UBarbarousDodgeNotifyState : public UAnimNotifyState {
	GENERATED_BODY()

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

private:
	UPROPERTY()
	ABarbarousPlayer* m_barbarousPlayer;
};

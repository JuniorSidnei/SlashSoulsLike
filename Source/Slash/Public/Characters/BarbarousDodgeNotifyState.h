#pragma once

#include <CoreMinimal.h>
#include <Animation/AnimNotifies/AnimNotifyState.h>
#include <BarbarousDodgeNotifyState.generated.h>

UCLASS()
class SLASH_API UBarbarousDodgeNotifyState : public UAnimNotifyState {
	GENERATED_BODY()

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

private:
	UPROPERTY()
	class ABarbarousPlayer* m_barbarousPlayer;
};

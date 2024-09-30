#include <Characters/BarbarousDodgeNotifyState.h>
#include <Characters/BarbarousPlayer.h>


void UBarbarousDodgeNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) {
	m_barbarousPlayer = Cast<ABarbarousPlayer>(MeshComp->GetOwner());
}

void UBarbarousDodgeNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) {
	if(m_barbarousPlayer == nullptr) { return; }
	
	m_barbarousPlayer->SetIsDodging(false);
}

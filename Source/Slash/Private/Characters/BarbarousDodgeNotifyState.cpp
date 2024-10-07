#include <Characters/BarbarousDodgeNotifyState.h>
#include <Characters/BarbarousPlayer.h>


void UBarbarousDodgeNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) { }

void UBarbarousDodgeNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) {
	if(m_barbarousPlayer == nullptr) {
		m_barbarousPlayer = Cast<ABarbarousPlayer>(MeshComp->GetOwner());
	}
	
	m_barbarousPlayer->SetIsDodging(false);
}

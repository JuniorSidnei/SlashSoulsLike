#pragma once

#include <CoreMinimal.h>
#include <GameFramework/Character.h>
#include <InputActionValue.h>
#include <BarbarousPlayer.generated.h>

class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class SLASH_API ABarbarousPlayer : public ACharacter {
	GENERATED_BODY()

public:
	ABarbarousPlayer();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool GetDodge() const;
	void SetIsDodging(bool value);

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category = Input)
	UInputMappingContext* BarbarousInputMappingContext;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* MovementActionInput;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* CameraLookActionInput;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* DodgeActionInput;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* InteractActionInput;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* AttackActionInput;
	
	void Move(const FInputActionValue& value);
	void CameraLook(const FInputActionValue& value);
	void Dodge();

	
	
private:
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* m_cameraBoom;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* m_viewCamera;

	bool m_isDodging;
};

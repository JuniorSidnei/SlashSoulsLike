#pragma once

UENUM(BlueprintType)
enum class ECharacterState : uint8 {
	Unequipped,
	EquippedOneHandWeapon,
	EquipTwoHandWeapon,
};

UENUM(BlueprintType)
enum class EAction : uint8 {
	Unoccupied,
	Dodging,
	Death,
	Healing,
	Attacking,
	HitReaction
};
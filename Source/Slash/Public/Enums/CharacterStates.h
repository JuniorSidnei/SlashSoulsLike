#pragma once

UENUM(BlueprintType)
enum class ECharacterState {
	Unequipped,
	EquippedOneHandWeapon,
	EquipTwoHandWeapon,
	Dodging,
	Death,
	Healing,
	Attacking
};
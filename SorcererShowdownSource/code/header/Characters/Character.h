#pragma once
#include "code/header/CharacterCreator/AI/CharacterAI.h"
#include "code/header/CursedTools/CursedTool.h"
struct Battlefield;

class Character { 
protected:
	static unsigned int global_id_counter;
	unsigned int unique_id = 0;

	std::string name = "";
	std::string color = "";
	std::string character_type = "Character";

	double health = 1.0;
	double max_health = 1.0;
	double previous_health = 1.0;
	
	std::unique_ptr<CursedTool> cursed_tool = nullptr;
	std::vector<std::unique_ptr<CursedTool>> inventory_curse;

	std::unique_ptr<CharacterBrain> brain = nullptr;

	int stun_duration = 3;

	double attack_damage = 20.0;

	bool is_player = false;
	bool is_stunned = false;
	bool is_invulnerable = false;
public:
	Character(double hp);

	virtual ~Character();

	virtual std::unique_ptr<Character> Clone() const;
	
	CursedTool* GetTool() const;
	const std::vector<std::unique_ptr<CursedTool>>& GetCursedTools() const;

	void SetVulnerability(bool);

	void SetHealth(double h);
	void SetCharacterName(std::string);
	void SetCharacterColor(std::string);

	bool IsThePlayer() const;
	void SetAsPlayer(bool);

	virtual void OnCharacterTurn(Battlefield&);

	void Taunt(Character* target) const;

	virtual void Attack(Character*);

	void Regen(double h);
	void Damage(double h);
	void DamageBypass(double h);
	void DamageBypassReinforcement(double h);
	void DamageBypassAll(double h);
	void UpdatePreviousHP();
	void SetStunState(bool s);
	void ClearStunTime();

	bool HPMoreThanMax(double) const;

	virtual double GetDamageReinforcement() const;

	double GetCharacterHealth() const;
	double GetCharacterMaxHealth() const;
	double GetCharacterPreviousHealth() const;

	bool IsCharacterStunned() const;

	void CursedToolChoice(size_t);
	void EquipToolByName(std::string_view weaponname);

	virtual void TickCharacterSpecialty();

	int GetID() const;
	std::string GetNameWithID() const;

	static void ResetGlobalID();
	static void IncrementGlobalID();
	static void DecrementGlobalID();
	static void SetGlobalID(unsigned int i);

	virtual bool IsaCurseUser() const;
	virtual bool IsaCursedSpirit() const;
	virtual bool IsaSorcerer() const;
	virtual bool IsPhysicallyGifted() const;
	virtual bool IsShikigami() const;

	std::string GetSimpleName() const;
	std::string GetName() const;
	std::string GetType() const;

	virtual bool CanBeHit() const;

	void AssignID();
	virtual bool CanBeAssignedID() const;
	double GetBaseAttackDamage() const;
	void AddToolToInventory(std::unique_ptr<CursedTool>);

	void SetBaseDamage(double);
	void SetEquippedTool(std::unique_ptr<CursedTool> tool);
	void SetBrain(std::unique_ptr<CharacterBrain>);
};
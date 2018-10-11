#include "stdafx.h"

void Abilities::load() {
	abilities_slk = slk::SLK("Units/AbilityData.slk");

	abilities_slk.merge(ini::INI("Units/HumanAbilityFunc.txt"));
	abilities_slk.merge(ini::INI("Units/OrcAbilityFunc.txt"));
	abilities_slk.merge(ini::INI("Units/UndeadAbilityFunc.txt"));
	abilities_slk.merge(ini::INI("Units/NightelfAbilityFunc.txt"));
	abilities_slk.merge(ini::INI("Units/CampaignAbilityFunc.txt"));
	abilities_slk.merge(ini::INI("Units/NeutralAbilityFunc.txt"));
	abilities_slk.merge(ini::INI("Units/ItemAbilityFunc.txt"));
	abilities_slk.merge(ini::INI("Units/CommonAbilityFunc.txt"));

	abilities_meta_slk = slk::SLK("Units/AbilityMetaData.slk");
	abilities_meta_slk.substitute(world_edit_strings, "WorldEditStrings");
}
#include "stdafx.h"


WidgetData::WidgetData(std::string id,std::string display_name, std::string editor_suffix, std::string parent_id, std::string icon) {
	raw_id = id;
	parent_raw_id = parent_id;
	suffix = editor_suffix;
	icon_path = icon;
	name = display_name;
	item = new QTreeWidgetItem();

	item->setText(0, QString::fromStdString(name + suffix));
	if (icon_path != "") {
		const auto q_icon = texture_to_icon(icon_path);
		item->setIcon(0, q_icon);
	}

	if (parent_raw_id != "") {
		is_custom = true;
		item->setTextColor(0, QColor(255, 0, 0));
	}
}
void WidgetData::display_raw(bool is_raw) {
	std::string display = (is_raw == true) ? raw_id : name + suffix;
	item->setText(0,QString::fromStdString(display));
}

int WidgetCounter::next(std::vector<int> count) {
	std::sort(count.begin(), count.end());
	int value = 0;
	for (int idx = 0; idx < count.size(); idx++) {
		if (count.size() > 1 && count[idx + 1] - count[idx] != 1) {
			value = count[idx]++;
		} else if (count.size() > 1) {
			value = count[idx + 1]++;
		} else if (count.size() == 1) {
			value = 1;
		} else {
			value = 0;
		}
	}
	return value;
}

template <class T>
constexpr bool ObjectEditorExtension::append(T widget) {
	if constexpr (std::is_same<UnitWidget, T>::value) {
		units.push_back(widget); 
		widgets_by_id.emplace(widget.raw_id, std::make_shared<UnitWidget>(widget));
		widgets_by_item.emplace(widget.item, std::make_shared<UnitWidget>(widget));
	}else if constexpr (std::is_same<ItemWidget, T>::value) {
		items.push_back(widget);
		widgets_by_id.emplace(widget.raw_id, std::make_shared<ItemWidget>(widget));
		widgets_by_item.emplace(widget.item, std::make_shared<ItemWidget>(widget));
	} else if constexpr (std::is_same<DestructibleWidget, T>::value) {
		destructibles.push_back(widget);
		widgets_by_id.emplace(widget.raw_id, std::make_shared<DestructibleWidget>(widget));
		widgets_by_item.emplace(widget.item, std::make_shared<DestructibleWidget>(widget));
	} else if constexpr (std::is_same<DoodadWidget, T>::value) {
		doodads.push_back(widget);
		widgets_by_id.emplace(widget.raw_id, std::make_shared<DoodadWidget>(widget));
		widgets_by_item.emplace(widget.item, std::make_shared<DoodadWidget>(widget));
	} else if constexpr (std::is_same<AbilityWidget, T>::value) {
		abilities.push_back(widget);
		widgets_by_id.emplace(widget.raw_id, std::make_shared<AbilityWidget>(widget));
		widgets_by_item.emplace(widget.item, std::make_shared<AbilityWidget>(widget));
	} else if constexpr (std::is_same<UpgradeWidget, T>::value) {
		upgrades.push_back(widget);
		widgets_by_id.emplace(widget.raw_id, std::make_shared<UpgradeWidget>(widget));
		widgets_by_item.emplace(widget.item, std::make_shared<UpgradeWidget>(widget));
	} else if constexpr (std::is_same<EffectWidget, T>::value) {
		effects.push_back(widget);
		widgets_by_id.emplace(widget.raw_id, std::make_shared<EffectWidget>(widget));
		widgets_by_item.emplace(widget.item, std::make_shared<EffectWidget>(widget));
	} else {
		return false;
	}
	return true;
}


void ObjectEditorExtension::load() {
	for (int row = 1; row < map.units.units_slk.rows; row++) {
		std::string unit_id = map.units.units_slk.table_data[row][0];
		std::string unit_name = map.units.units_slk.data("Name", unit_id);
		std::string suffix = map.units.units_slk.data("EditorSuffix", unit_id);
		std::string path = map.units.units_slk.data("Art", unit_id);

		UnitWidget u(unit_id, unit_name, suffix, "", path);
		u.is_building = (map.units.units_slk.data("isbldg", unit_id) == "1") ? true : false;
		u.is_special = (map.units.units_slk.data("special", unit_id) == "1") ? true : false;
		u.is_melee = (map.units.units_slk.data("campaign", unit_id) == "0") ? true : false;
		u.is_hero = (std::isupper(unit_id.at(0))) ? true : false;

		map.objects.append(u);
	
	}

	for (int row = 1; row < map.units.items_slk.rows; row++) {
		std::string item_id = map.units.items_slk.table_data[row][0];
		std::string item_name = map.units.items_slk.data("Name", item_id);
		std::string path = map.units.items_slk.data("Art", item_id);

		ItemWidget i(item_id, item_name, "", "", path);
		map.objects.append(i);
	}

	for (int row = 1; row < map.doodads.doodads_slk.rows; row++) {
		std::string doodad_id = map.doodads.doodads_slk.table_data[row][0];
		std::string doodad_name = map.doodads.doodads_slk.data("Name", doodad_id);
		std::string suffix = map.doodads.doodads_slk.data("EditorSuffix", doodad_id);
		DoodadWidget d(doodad_id, doodad_name, suffix, "", "");
		map.objects.append(d);
	}

	for (int row = 1; row < map.doodads.destructibles_slk.rows; row++) {
		std::string dest_id = map.doodads.destructibles_slk.table_data[row][0];
		std::string dest_name = map.doodads.destructibles_slk.data("Name", dest_id);
		std::string suffix = map.doodads.destructibles_slk.data("EditorSuffix", dest_id);
		DestructibleWidget d(dest_id, dest_name, suffix, "", "");
		map.objects.append(d);

	}

	for (int row = 1; row < map.abilities.abilities_slk.rows; row++) {
		std::string ability_id = map.abilities.abilities_slk.table_data[row][0];
		std::string ability_name = map.abilities.abilities_slk.data("comments", ability_id);
		std::string suffix = map.abilities.abilities_slk.data("EditorSuffix", ability_id);
		std::string path = map.abilities.abilities_slk.data("Art", ability_id);
		if (split(path, '\\').size() != 3) {
			path = "";
		}
		AbilityWidget a(ability_id, ability_name, suffix, "", path);
		a.is_hero = (map.abilities.abilities_slk.data("hero", ability_id) == "1")? true : false;
		map.objects.append(a);

	}

}

void ObjectEditorExtension::save() const {

}

std::string WidgetData::generate_path() {
	return "";
}

std::string UnitWidget::generate_path() {
	std::string melee = (is_melee) ? "Melee" : "Campaign";
	std::string type;
	
	if (is_hero && !is_special) {
		type = "Heroes";
	} else if (is_building) {
		type = "Buildings";
	} else if (is_special) {
		type = "Special";
	} else {
		type = "Units";
	}
	return race() + "\\" + melee + "\\" + type;
}

std::string UnitWidget::race() const {
	std::string race = map.units.units_slk.data("race", raw_id);
	
	if (race == "orc") {
		return "Orc";
	} else if (race == "human") {
		return "Human";
	} else if (race == "undead") {
		return "Undead";
	} else if (race == "nightelf") {
		return "Night Elf";
	} else if (race == "creeps") {
		return "Neutral Hostile";
	} else if (race == "naga") {
		return "Naga";
	}
	return "Neutral Passive";
}

std::string ItemWidget::generate_path() {
	return map.units.items_slk.data("class", raw_id);
}
std::string DestructibleWidget::generate_path() {
	std::string type = map.doodads.destructibles_slk.data("category", raw_id);
	std::vector<std::string> pair = world_edit_data.whole_data("DestructibleCategories", type);
	auto icon = texture_to_icon(pair.at(1) + ".blp");
	item->setIcon(0, icon);
	return pair.at(0);
}
std::string DoodadWidget::generate_path() {
	std::string type = map.doodads.doodads_slk.data("category", raw_id);
	std::vector<std::string> pair = world_edit_data.whole_data("DoodadCategories", type);
	auto icon = texture_to_icon(pair.at(1) + ".blp");
	item->setIcon(0, icon);
	return pair.at(0);
}

std::string AbilityWidget::generate_path() {
	std::string race = map.abilities.abilities_slk.data("race", raw_id);
	std::string hero = (is_hero) ? "Heroes" : "Units";
	return race + "\\" + hero;
}
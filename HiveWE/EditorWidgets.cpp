#include "stdafx.h"


WidgetData::WidgetData(std::string id, std::string editor_suffix, std::string parent_id, std::string icon) {
	raw_id = id;
	parent_raw_id = parent_id;
	suffix = editor_suffix;
	icon_path = icon;
	item = new QTreeWidgetItem();

	item->setText(0, QString::fromStdString(raw_id + suffix));
	const auto q_icon = texture_to_icon(icon_path);
	item->setIcon(0, q_icon);
	map.units.units_slk.data("Art", raw_id);

	if (parent_raw_id != "") {
		is_edited = true;
		item->setTextColor(0, QColor(255, 0, 0));
	}
}


template <class T>
constexpr bool ObjectEditorExtension::append(T widget) {
	if constexpr (std::is_same<UnitWidget, T>::value) {
		units.push_back(widget);
		widgets_by_id.emplace(widget.raw_id, std::make_shared<UnitWidget>(widget));
		widgets_by_item.emplace(widget.item, std::make_shared<UnitWidget>(widget));
	} else if (std::is_same<ItemWidget, T>::value) {
		items.push_back(widget);
		widgets_by_id.emplace(widget.raw_id, std::make_shared<ItemWidget>(widget));
		widgets_by_item.emplace(widget.item, std::make_shared<ItemWidget>(widget));
	} else if (std::is_same<DestructibleWidget, T>::value) {
		destructibles.push_back(widget);
		widgets_by_id.emplace(widget.raw_id, std::make_shared<DestructibleWidget>(widget));
		widgets_by_item.emplace(widget.item, std::make_shared<DestructibleWidget>(widget));
	} else if (std::is_same<DoodadWidget, T>::value) {
		doodads.push_back(widget);
		widgets_by_id.emplace(widget.raw_id, std::make_shared<DoodadWidget>(widget));
		widgets_by_item.emplace(widget.item, std::make_shared<DoodadWidget>(widget));
	} else if (std::is_same<AbilityWidget, T>::value) {
		abilities.push_back(widget);
		widgets_by_id.emplace(widget.raw_id, std::make_shared<AbilityWidget>(widget));
		widgets_by_item.emplace(widget.item, std::make_shared<AbilityWidget>(widget));
	} else if (std::is_same<UpgradeWidget, T>::value) {
		upgrades.push_back(widget);
		widgets_by_id.emplace(widget.raw_id, std::make_shared<UpgradeWidget>(widget));
		widgets_by_item.emplace(widget.item, std::make_shared<UpgradeWidget>(widget));
	} else if (std::is_same<EffectWidget, T>::value) {
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
		//std::string race = match_race(map.units.units_slk.data("race", unit_id));
		std::string path = map.units.units_slk.data("Art", unit_id);

		if (path != "" && path.substr(path.size() - 4, 4) == ".blp") {
			UnitWidget u(unit_id, suffix, "", path);
			u.is_building = (map.units.units_slk.data("isbldg", unit_id) == "0") ? true : false;
			u.is_special = (map.units.units_slk.data("special", unit_id) == "0") ? true : false;
			u.is_melee = (map.units.units_slk.data("campaign", unit_id) == "0") ? true : false;

			map.objects.append(u);
		}
	}

	for (int row = 1; row < map.units.items_slk.rows; row++) {
		std::string item_id = map.units.items_slk.table_data[row][0];
		std::string item_nanme = map.units.items_slk.data("Name", item_id);
		std::string path = map.units.items_slk.data("Art", item_id);

		if (path != "" && path.substr(path.size() - 4, 4) == ".blp") {
			ItemWidget i(item_id, "", "", path);

			map.objects.append(i);
		}
	}

	for (int row = 1; row < map.doodads.doodads_slk.rows; row++) {
		std::string doodad_id = map.doodads.doodads_slk.table_data[row][0];
		std::string doodad_name = map.doodads.doodads_slk.data("Name", doodad_id);
		std::string path = map.doodads.doodads_slk.data("Art", doodad_id);

		if (path != "" && path.substr(path.size() - 4, 4) == ".blp") {
			DoodadWidget d(doodad_id, "", "", path);

			map.objects.append(d);
		}
	}

	for (int row = 1; row < map.doodads.destructibles_slk.rows; row++) {
		std::string dest_id = map.doodads.destructibles_slk.table_data[row][0];
		std::string dest_name = map.doodads.destructibles_slk.data("Name", dest_id);
		std::string path = map.doodads.destructibles_slk.data("Art", dest_name);

		if (path != "" && path.substr(path.size() - 4, 4) == ".blp") {
			DestructibleWidget d(dest_id, "", "", path);

			map.objects.append(d);
		}
	}

}

void ObjectEditorExtension::save() const {

}
#include <stdafx.h>

WidgetProperty::WidgetProperty(std::string id,std::string field_name, std::string display_name) {
	raw_id = id;
	name = display_name;
	field = field_name;

	item = new QTreeWidgetItem();
	item->setText(0,QString::fromStdString(name));
}
void WidgetProperty::display_raw(bool is_raw) {
	std::string display = (is_raw) ? field : name;
	item->setText(0, QString::fromStdString(display));
}

void Properties::load() {
	for (int row = 1; row < map.units.units_meta_slk.rows; row++) {
		std::string raw_id = map.units.units_meta_slk.table_data[row][0];
		std::string field = map.units.units_meta_slk.data("field", raw_id);
		std::string name = map.units.units_meta_slk.data("displayName", raw_id);
		
		WidgetProperty p(raw_id,field, name);
		if ( raw_id.at(0) == 'u' ) {
			unit_properties.push_back(p);
		} else {
			item_properties.push_back(p);
		}
	}

	for (int row = 1; row < map.doodads.doodads_meta_slk.rows; row++) {
		std::string raw_id = map.doodads.doodads_meta_slk.table_data[row][0];
		std::string field = map.doodads.doodads_meta_slk.data("field", raw_id);
		std::string name = map.doodads.doodads_meta_slk.data("displayName", raw_id);

		WidgetProperty p(raw_id, field, name);
		doodad_properties.push_back(p);
	}

	for (int row = 1; row < map.doodads.destructibles_meta_slk.rows; row++) {
		std::string raw_id = map.doodads.destructibles_meta_slk.table_data[row][0];
		std::string field = map.doodads.destructibles_meta_slk.data("field", raw_id);
		std::string name = map.doodads.destructibles_meta_slk.data("displayName", raw_id);

		WidgetProperty p(raw_id, field, name);
		destructible_properties.push_back(p);
	}

	for (int row = 1; row < map.abilities.abilities_meta_slk.rows; row++) {
		std::string raw_id = map.abilities.abilities_meta_slk.table_data[row][0];
		std::string field = map.abilities.abilities_meta_slk.data("field", raw_id);
		std::string name = map.abilities.abilities_meta_slk.data("displayName", raw_id);

		WidgetProperty p(raw_id, field, name);
		ability_properties.push_back(p);
	}
}

void Properties::initialize_dialogs() {

}
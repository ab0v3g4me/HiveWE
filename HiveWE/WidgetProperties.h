class WidgetProperty {
public:
	std::string raw_id;
	std::string field;
	std::string name;

	QTreeWidgetItem* item;

	WidgetProperty(std::string id,std::string field_name, std::string display_name);
	void display_raw(bool is_raw);
};

class Properties {
public:
	std::vector<WidgetProperty> unit_properties;
	std::vector<WidgetProperty> item_properties;
	std::vector<WidgetProperty> doodad_properties;
	std::vector<WidgetProperty> destructible_properties;

	void load();
};
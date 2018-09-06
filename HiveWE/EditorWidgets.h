class WidgetData {
public: 
	std::string raw_id;
	std::string suffix;

	std::string parent_raw_id;
	std::string icon_path;
	bool is_edited = false;

	QTreeWidgetItem* item;

	explicit WidgetData(std::string id, std::string editor_suffix, std::string parent_id, std::string icon);
	void display_raw_values();

};

class UnitWidget : public WidgetData {
public:
	bool is_special = false;
	bool is_building = false;
	bool is_hero = false;
	bool is_melee = false;

	using WidgetData::WidgetData;
};

class ItemWidget : public WidgetData {
	using WidgetData::WidgetData;
};

class DestructibleWidget : public WidgetData {
	using WidgetData::WidgetData;
};
class DoodadWidget : public WidgetData {
	using WidgetData::WidgetData;
};
class AbilityWidget : public WidgetData {
	using WidgetData::WidgetData;
};
class UpgradeWidget : public WidgetData {
	using WidgetData::WidgetData;
};
class EffectWidget : public WidgetData {
	using WidgetData::WidgetData;
};


class ObjectEditorExtension {
public: 
	std::vector<UnitWidget*> units;
	std::vector<ItemWidget*> items;
	std::vector<DestructibleWidget*> destructibles;
	std::vector<DoodadWidget*> doodads;
	std::vector<AbilityWidget*> abilities;
	std::vector<UpgradeWidget*> upgrades;
	std::vector<EffectWidget*> effects;

	std::unordered_map<std::string, std::shared_ptr<WidgetData>> widgets_by_id;
	std::unordered_map<QTreeWidgetItem*, std::shared_ptr<WidgetData>> widgets_by_item;


	template <class T>
	constexpr bool append(T widget);
	
	void load();
	void save() const;
};

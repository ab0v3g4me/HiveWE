class WidgetCounter {
public:
	std::vector<int> unit_count;
	std::vector<int> item_count;

	int next(std::vector<int> count);
};



class WidgetData {
public: 
	std::string raw_id;
	std::string name;
	std::string suffix;

	std::string parent_raw_id;
	std::string icon_path;
	bool is_custom = false;

	QTreeWidgetItem* item;

	explicit WidgetData(std::string id,std::string name,std::string editor_suffix, std::string parent_id, std::string icon);
	void display_raw(bool is_raw);
	virtual ~WidgetData() {};
	virtual std::string generate_path();
};

class UnitWidget : public WidgetData {
public:
	bool is_special = false;
	bool is_building = false;
	bool is_hero = false;
	bool is_melee = false;
	std::string generate_path();
	std::string race() const;
	using WidgetData::WidgetData;
};

class ItemWidget : public WidgetData {
public:
	std::string generate_path();
	using WidgetData::WidgetData;
};

class DestructibleWidget : public WidgetData {
public:
	std::string generate_path();
	using WidgetData::WidgetData;
};
class DoodadWidget : public WidgetData {
public:
	std::string generate_path();
	using WidgetData::WidgetData;
};
class AbilityWidget : public WidgetData {
public:
	bool is_hero = false;
	std::string generate_path();
	using WidgetData::WidgetData;
};
class UpgradeWidget : public WidgetData {
	//std::string generate_path();
	using WidgetData::WidgetData;
};
class EffectWidget : public WidgetData {
	//std::string generate_path();
	using WidgetData::WidgetData;
};


class ObjectEditorExtension {
public: 
	std::vector<UnitWidget> units;
	std::vector<ItemWidget> items;
	std::vector<DestructibleWidget> destructibles;
	std::vector<DoodadWidget> doodads;
	std::vector<AbilityWidget> abilities;
	std::vector<UpgradeWidget> upgrades;
	std::vector<EffectWidget> effects;

	std::unordered_map<std::string, std::shared_ptr<WidgetData>> widgets_by_id;
	std::unordered_map<QTreeWidgetItem*, std::shared_ptr<WidgetData>> widgets_by_item;

	Properties properties;
	WidgetCounter count_system;

	template <class T>
	constexpr bool append(T widget);
	
	void load();
	void save() const;
};

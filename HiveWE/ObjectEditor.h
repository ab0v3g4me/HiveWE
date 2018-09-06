#pragma once

#include "ui_ObjectEditor.h"

namespace Ui {
class ObjectEditor;
}

class ObjectEditor : public QMainWindow
{
	Q_OBJECT

    void setup();

	void load_units();
	void load_unit_data(QTreeWidgetItem* itm);
	void load_unit_field_strings();

	void load_items();
	void load_item_data(QTreeWidgetItem* itm);
	void load_item_field_strings();

	void run_dialog_type(QTreeWidgetItem * item);

	//void change_int(QTreeWidgetItem * item,ObjectField field,int distance);
	//void change_bool(QTreeWidgetItem * item, ObjectField field, int distance);
	//void change_real(QTreeWidgetItem * item, ObjectField field, int distance);


	void switch_raw_names();
	void display_unit_names();
	void display_item_names();

	void create_custom_unit(QTreeWidgetItem* item);
	void custom_menu_popup(const QPoint& pos);

	std::string match_race(const std::string race) const;

	int selected_unit_index = 0;
	bool show_as_raw = true;

	std::unordered_map<std::string,QTreeWidgetItem*> standard_unit_folder_hierarchy;
	std::unordered_map<std::string, QTreeWidgetItem*> custom_unit_folder_hierarchy;

	std::unordered_map<std::string, QTreeWidgetItem*> standard_item_folder_hierarchy;
	std::unordered_map<std::string, QTreeWidgetItem*> custom_item_folder_hierarchy;

	//std::unordered_map<QTreeWidgetItem*, UnitField> oe_units;
	//std::unordered_map<QTreeWidgetItem*, ItemField> oe_items;

	QColor changed_field_color = QColor(200,0,0);
	QColor default_color = QColor(0, 0, 0);
	
    QIcon folder_icon;

//	ObjectData * obj_data = new ObjectData();
	QSet<int> pressed_keys;
protected:
    bool eventFilter(QObject* obj,QEvent* event) override;

public:
    explicit ObjectEditor(QWidget *parent = 0);
    ~ObjectEditor();

private:
	static bool loaded;
    Ui::ObjectEditor *ui;
};

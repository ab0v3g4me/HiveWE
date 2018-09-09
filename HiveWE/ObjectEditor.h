#pragma once

#include "ui_ObjectEditor.h"

namespace Ui {
class ObjectEditor;
}

class ObjectEditor : public QMainWindow
{
	Q_OBJECT

    void setup();

	void load();

	void load_properties();

	void display_transition();

	void load_widget_data(QTreeWidgetItem* item);

	void create_folder(WidgetData* widget);

	QTreeWidgetItem* units_root_item;
	QTreeWidgetItem* standard_units;
	QTreeWidgetItem* custom_units;
	
	QTreeWidgetItem* items_root_item;
	QTreeWidgetItem* standard_items;
	QTreeWidgetItem* custom_items;

	QTreeWidgetItem* doodads_root_item;
	QTreeWidgetItem* standard_doodads;
	QTreeWidgetItem* custom_doodads;

	QTreeWidgetItem* destructibles_root_item;
	QTreeWidgetItem* standard_destructibles;
	QTreeWidgetItem* custom_destructibles;

	std::unordered_map<std::string, QTreeWidgetItem*> folder_hierarchy;


	void custom_menu_popup(const QPoint& pos);


	QColor changed_field_color = QColor(200, 0, 0);
	QColor default_color = QColor(0, 0, 0);
	
    QIcon folder_icon;

	QSet<int> pressed_keys;
protected:
    bool eventFilter(QObject* obj,QEvent* event) override;
	void closeEvent(QCloseEvent *event) override;
public:
    explicit ObjectEditor(QWidget *parent = 0);
    ~ObjectEditor();

private:
	static bool show_as_raw;
    Ui::ObjectEditor *ui;
};

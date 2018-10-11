#include "stdafx.h"

bool ObjectEditor::show_as_raw = false;

ObjectEditor::ObjectEditor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ObjectEditor)
{
    ui->setupUi(this);
	this->installEventFilter(this);
	ui->tabWidget->setCurrentIndex(0);
	
    QFileIconProvider icons;
    folder_icon = icons.icon(QFileIconProvider::Folder);

	map.objects.load();
	map.objects.properties.load();

	setup();
	load();
	load_properties();

	show();
}
void ObjectEditor::closeEvent(QCloseEvent *event) {
	hide();
	event->ignore();
}

ObjectEditor::~ObjectEditor()
{
    delete ui;
}

void ObjectEditor::setup(){
	units_root_item = ui->units->invisibleRootItem();
	items_root_item = ui->items->invisibleRootItem();
	doodads_root_item = ui->doodads->invisibleRootItem();
	destructibles_root_item = ui->destructibles->invisibleRootItem();
	abilities_root_item = ui->abilities->invisibleRootItem();

	standard_units = new QTreeWidgetItem(units_root_item);
	standard_units->setText(0, QString::fromStdString("Standard Units"));
	standard_units->setText(1, QString::fromStdString("D"));
	standard_units->setIcon(0, folder_icon);

	custom_units = new QTreeWidgetItem(units_root_item);
	custom_units->setText(0, QString::fromStdString("Custom Units"));
	custom_units->setText(1, QString::fromStdString("D"));
	custom_units->setIcon(0, folder_icon);

	standard_items = new QTreeWidgetItem(items_root_item);
	standard_items->setText(0, QString::fromStdString("Standard Items"));
	standard_items->setText(1, QString::fromStdString("D"));
	standard_items->setIcon(0, folder_icon);

	custom_items = new QTreeWidgetItem(items_root_item);
	custom_items->setText(0, QString::fromStdString("Custom Units"));
	custom_items->setText(1, QString::fromStdString("D"));
	custom_items->setIcon(0, folder_icon);

	standard_destructibles = new QTreeWidgetItem(destructibles_root_item);
	standard_destructibles->setText(0, QString::fromStdString("Standard Destructibles"));
	standard_destructibles->setText(1, QString::fromStdString("D"));
	standard_destructibles->setIcon(0, folder_icon);

	custom_destructibles = new QTreeWidgetItem(destructibles_root_item);
	custom_destructibles->setText(0, QString::fromStdString("Custom Destructibles"));
	custom_destructibles->setText(1, QString::fromStdString("D"));
	custom_destructibles->setIcon(0, folder_icon);

	standard_doodads = new QTreeWidgetItem(doodads_root_item);
	standard_doodads->setText(0, QString::fromStdString("Standard Doodads"));
	standard_doodads->setText(1, QString::fromStdString("D"));
	standard_doodads->setIcon(0, folder_icon);

	custom_doodads = new QTreeWidgetItem(doodads_root_item);
	custom_doodads->setText(0, QString::fromStdString("Custom Doodads"));
	custom_doodads->setText(1, QString::fromStdString("D"));
	custom_doodads->setIcon(0, folder_icon);

	standard_abilities = new QTreeWidgetItem(abilities_root_item);
	standard_abilities->setText(0, QString::fromStdString("Standard Abilities"));
	standard_abilities->setText(1, QString::fromStdString("D"));
	standard_abilities->setIcon(0, folder_icon);

	custom_abilities = new QTreeWidgetItem(abilities_root_item);
	custom_abilities->setText(0, QString::fromStdString("Custom Abilities"));
	custom_abilities->setText(1, QString::fromStdString("D"));
	custom_abilities->setIcon(0, folder_icon);
    //==========================//
    // Units Tab Initialization //
    //==========================// 
    

    ui->units->header()->hide();

    ui->unit_search->installEventFilter(this);
    ui->unit_stats_search->installEventFilter(this);
    ui->unit_stats->header()->setSectionResizeMode(0, QHeaderView::Fixed);
    ui->unit_stats->header()->setSectionResizeMode(1, QHeaderView::Stretch);
	ui->units->setContextMenuPolicy(Qt::CustomContextMenu);

	connect(ui->unit_search, &QLineEdit::textChanged, [&](const QString &text){  });
	connect(ui->unit_stats_search, &QLineEdit::textChanged, [&](const QString &text) { });

	connect(ui->units, &QTreeWidget::customContextMenuRequested, this, &ObjectEditor::custom_menu_popup);
	connect(ui->units, &QTreeWidget::itemClicked, [&](QTreeWidgetItem * item, int column) { load_widget_data(item); });

    //==========================//
    // Items Tab Initialization //
    //==========================//
    ui->items->header()->hide();
    ui->item_search->installEventFilter(this);
    ui->item_stats_search->installEventFilter(this);
    ui->item_stats->header()->setSectionResizeMode(0, QHeaderView::Fixed);
    ui->item_stats->header()->setSectionResizeMode(1, QHeaderView::Stretch);


	connect(ui->items, &QTreeWidget::itemClicked, [&](QTreeWidgetItem * item, int column) { load_widget_data(item); });
    //==================================//
    // Destructibles Tab Initialization //
    //==================================//


    ui->destructibles->header()->hide();
    ui->destructibles_search->installEventFilter(this);
    ui->destructibles_stats_search->installEventFilter(this);
    ui->destructibles_stats->header()->setSectionResizeMode(0, QHeaderView::Fixed);
    ui->destructibles_stats->header()->setSectionResizeMode(1, QHeaderView::Stretch);

	connect(ui->destructibles, &QTreeWidget::itemClicked, [&](QTreeWidgetItem * item, int column) { load_widget_data(item); });
	//==================================//
	//    Doodads Tab Initialization    //
	//==================================//

	ui->doodads->header()->hide();
	ui->doodads_search->installEventFilter(this);
	ui->doodads_stats_search->installEventFilter(this);
	ui->doodads_stats->header()->setSectionResizeMode(0, QHeaderView::Fixed);
	ui->doodads_stats->header()->setSectionResizeMode(1, QHeaderView::Stretch);

	connect(ui->doodads, &QTreeWidget::itemClicked, [&](QTreeWidgetItem * item, int column) { load_widget_data(item); });
	//==================================//
	//   Abilities Tab Initialization   //
	//==================================//
	ui->abilities->header()->hide();
	ui->abilities_search->installEventFilter(this);
	ui->abilities_stats_search->installEventFilter(this);
	ui->abilities_stats->header()->setSectionResizeMode(0, QHeaderView::Fixed);
	ui->abilities_stats->header()->setSectionResizeMode(1, QHeaderView::Stretch);

	connect(ui->abilities, &QTreeWidget::itemClicked, [&](QTreeWidgetItem * item, int column) { load_widget_data(item); });

}

void ObjectEditor::create_folder(WidgetData* widget) {
	QTreeWidgetItem* root_item;
	std::string path = "";
	
	if (auto u = dynamic_cast<UnitWidget*>(widget); u) {
		path = u->generate_path();
		root_item = (!u->is_custom) ? units_root_item->child(0) : units_root_item->child(1);
	} else if (auto i = dynamic_cast<ItemWidget*>(widget); i) {
		path = i->generate_path();
		root_item = (!i->is_custom) ? items_root_item->child(0) : items_root_item->child(1);
	} else if (auto d = dynamic_cast<DoodadWidget*>(widget); d) {
		path = d->generate_path();
		root_item = (!d->is_custom) ? doodads_root_item->child(0) : doodads_root_item->child(1);
	} else if (auto d = dynamic_cast<DestructibleWidget*>(widget); d) {
		path = d->generate_path();
		root_item = (!d->is_custom) ? destructibles_root_item->child(0) : destructibles_root_item->child(1);
	} else if (auto a = dynamic_cast<AbilityWidget*>(widget); a) {
		path = a->generate_path();
		root_item = (!a->is_custom) ? abilities_root_item->child(0) : abilities_root_item->child(1);
	}

	if (folder_hierarchy.find(path) == folder_hierarchy.end()) {
		QTreeWidgetItem* last_item = root_item;
		for (auto&& folder : split(path, '\\')) {
			for (int index = 0; index < last_item->childCount(); index++) {
				if (auto c = last_item->child(index); c->text(0).toStdString() == folder) {
					last_item = c;
					goto outer_loop;
				}
			}
			root_item = new QTreeWidgetItem(last_item);
			root_item->setText(0, QString::fromStdString(folder));
			root_item->setText(1, QString::fromStdString("D"));
			root_item->setIcon(0, folder_icon);
			last_item = root_item;
		outer_loop: {};
		}
		last_item->addChild(widget->item);
		folder_hierarchy.emplace(path, last_item);
	} else {
		folder_hierarchy.at(path)->addChild(widget->item);
	}
}


void ObjectEditor::custom_menu_popup(const QPoint& pos) {
	QTreeWidgetItem* item = ui->units->itemAt(pos);

	QMenu* menu = new QMenu(this);

	QAction* create_as_custom = new QAction("Copy as a Custom Widget...");
	QAction* new_custom_unit = new QAction("New Custom Widget...");
	QAction* delete_custom_unit = new QAction("Delete Custom Widget...");
	QAction* create_custom_race = new QAction("Create Custom Race...");
	if (item->text(1) == "D") {
		menu->addAction(create_custom_race);
	} else {
		menu->addAction(create_as_custom);
		menu->addAction(new_custom_unit);
		menu->addAction(delete_custom_unit);
	}

	menu->exec(QCursor::pos());
}


bool ObjectEditor::eventFilter(QObject* obj, QEvent* event){
	if (event->type() == QKeyEvent::KeyPress) {
		QKeyEvent* k_event = dynamic_cast<QKeyEvent*>(event);
		QLineEdit * search_bar = qobject_cast<QLineEdit*>(obj);
		pressed_keys += k_event->key();
		if (pressed_keys.contains(Qt::Key_Escape) && search_bar != NULL) {
			search_bar->clear();
		} else if ( pressed_keys.contains(Qt::Key_Control) && pressed_keys.contains(Qt::Key_D) ) {
			show_as_raw = !show_as_raw;
			display_transition();
		}
	} else if (event->type() == QKeyEvent::KeyRelease) {
		QKeyEvent* k_event = dynamic_cast<QKeyEvent*>(event);
		pressed_keys -= k_event->key();
	}

    return false;
}

void ObjectEditor::display_transition() {
	for (auto&& u : map.objects.units) {
		u.display_raw(show_as_raw);
	}
	for (auto&& i : map.objects.items) {
		i.display_raw(show_as_raw);
	}
	for (auto&& d : map.objects.doodads) {
		d.display_raw(show_as_raw);
	}
	for (auto&& d : map.objects.destructibles) {
		d.display_raw(show_as_raw);
	}
	for (auto&& a : map.objects.abilities) {
		a.display_raw(show_as_raw);
	}
}

void ObjectEditor::load() {
	for (auto&& u : map.objects.units) {
		create_folder(&u);
	}
	for (auto&& i : map.objects.items) {
		create_folder(&i);
	}
	for (auto&& d : map.objects.doodads) {
		create_folder(&d);
	}
	for (auto&& d : map.objects.destructibles) {
		create_folder(&d);
	}
	for (auto&& a : map.objects.abilities) {
		create_folder(&a);
	}
}

void ObjectEditor::load_widget_data(QTreeWidgetItem* item) {
	if (item->text(1) != "") {
		return;
	} 
	auto widget = map.objects.widgets_by_item.at(item);

	if (auto u = std::dynamic_pointer_cast<UnitWidget>(widget); u) {
		for (auto&& p: map.objects.properties.unit_properties) {
			std::string value = map.units.units_slk.data(p.field, u->raw_id);
			p.item->setText(1, QString::fromStdString(value));
		}
	} else if (auto i = std::dynamic_pointer_cast<ItemWidget>(widget); i) {
		for (auto&& p : map.objects.properties.item_properties) {
			std::string value = map.units.items_slk.data(p.field, i->raw_id);
			p.item->setText(1, QString::fromStdString(value));
		}
	} else if (auto d = std::dynamic_pointer_cast<DoodadWidget>(widget); d) {
		for (auto&& p :  map.objects.properties.doodad_properties) {
			std::string value = map.doodads.doodads_slk.data(p.field, d->raw_id);
			p.item->setText(1, QString::fromStdString(value));
		}
	} else if (auto d = std::dynamic_pointer_cast<DestructibleWidget>(widget); d) {
		for (auto&& p : map.objects.properties.destructible_properties) {
			std::string value = map.doodads.destructibles_slk.data(p.field, d->raw_id);
			p.item->setText(1, QString::fromStdString(value));
		}
	} else if (auto a = std::dynamic_pointer_cast<AbilityWidget>(widget); a) {
		for (auto&& p : map.objects.properties.ability_properties) {
			std::string value = map.abilities.abilities_slk.data(p.field, a->raw_id);
			p.item->setText(1, QString::fromStdString(value));
		}
	}
}

void ObjectEditor::load_properties() {
	for (auto&& p : map.objects.properties.unit_properties) {
		ui->unit_stats->addTopLevelItem(p.item);
	}
	for (auto&& p : map.objects.properties.item_properties) {
		ui->item_stats->addTopLevelItem(p.item);
	}
	for (auto&& p : map.objects.properties.doodad_properties) {
		ui->doodads_stats->addTopLevelItem(p.item);
	}
	for (auto&& p : map.objects.properties.destructible_properties) {
		ui->destructibles_stats->addTopLevelItem(p.item);
	}
	for (auto&& p : map.objects.properties.ability_properties) {
		ui->abilities_stats->addTopLevelItem(p.item);
	}
}

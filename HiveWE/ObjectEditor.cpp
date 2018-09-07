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

	setup();
	map.objects.load();
	load();
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

    connect(ui->tabWidget, &QTabWidget::currentChanged, [&](int index) { 
		
	});

	units_root_item = ui->units->invisibleRootItem();
	items_root_item = ui->items->invisibleRootItem();
	doodads_root_item = ui->doodads->invisibleRootItem();
	destructibles_root_item = ui->destructibles->invisibleRootItem();

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

	//connect(ui->units, &QTreeWidget::customContextMenuRequested, this, &ObjectEditor::custom_menu_popup);
	connect(ui->units, &QTreeWidget::itemClicked, [&](QTreeWidgetItem * item, int column) { load_widget_data(item); });

    //==========================//
    // Items Tab Initialization //
    //==========================//

    //items_proxy_model->setSourceModel(items_model);
    //items_proxy_model->setRecursiveFilteringEnabled(true);
    //items_proxy_model->setFilterCaseSensitivity(Qt::CaseSensitive);

    ui->items->header()->hide();
    ui->item_search->installEventFilter(this);
    ui->item_stats_search->installEventFilter(this);
    ui->item_stats->header()->setSectionResizeMode(0, QHeaderView::Fixed);
    ui->item_stats->header()->setSectionResizeMode(1, QHeaderView::Stretch);


	connect(ui->items, &QTreeWidget::itemClicked, [&](QTreeWidgetItem * item, int column) { load_widget_data(item); });
    //connect(ui->item_search, &QLineEdit::textChanged, [&](const QString &text){ items_proxy_model->setFilterRegExp(text);});
	///connect(ui->item_stats_search, &QLineEdit::textChanged, [&](const QString &text) { items_proxy_model->setFilterRegExp(text); });

    //==================================//
    // Destructibles Tab Initialization //
    //==================================//


    ui->destructibles->header()->hide();
    ui->destructibles_search->installEventFilter(this);
    ui->destructibles_stats_search->installEventFilter(this);
    ui->destructibles_stats->header()->setSectionResizeMode(0, QHeaderView::Fixed);
    ui->destructibles_stats->header()->setSectionResizeMode(1, QHeaderView::Stretch);

	connect(ui->destructibles, &QTreeWidget::itemClicked, [&](QTreeWidgetItem * item, int column) { load_widget_data(item); });
}

void ObjectEditor::custom_menu_popup(const QPoint& pos) {
	QTreeWidgetItem* item = ui->units->itemAt(pos);
	if (item->childCount() > 0) {
		return;
	}

	QMenu* menu = new QMenu(this);
	QAction* create_as_custom = new QAction("Copy as a Custom Widget...");
	QAction* new_custom_unit = new QAction("New Custom Widget...");
	QAction* delete_custom_unit = new QAction("Delete Custom Widget...");
	
//	connect(new_custom_unit, &QAction::triggered, [&]() { create_custom_unit(item); });
	
	menu->addAction(create_as_custom);
	menu->addAction(new_custom_unit);
	menu->addAction(delete_custom_unit);

	menu->exec(QCursor::pos());
}


bool ObjectEditor::eventFilter(QObject* obj, QEvent* event){
	if (event->type() == QKeyEvent::KeyPress) {
		QKeyEvent* k_event = dynamic_cast<QKeyEvent*>(event);
		QLineEdit * search_bar = qobject_cast<QLineEdit*>(obj);
		pressed_keys += k_event->key();
		if (pressed_keys.contains(Qt::Key_Escape) && search_bar != NULL) {
			search_bar->clear();
		}
		else if ( pressed_keys.contains(Qt::Key_Control) && pressed_keys.contains(Qt::Key_D) ) {
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
}

void ObjectEditor::load() {
	for (auto&& u : map.objects.units) {
		units_root_item->addChild(u.item);
	}
	for (auto&& i : map.objects.items) {
		items_root_item->addChild(i.item);
	}
	for (auto&& d : map.objects.doodads) {
		doodads_root_item->addChild(d.item);
	}
	for (auto&& d : map.objects.destructibles) {
		destructibles_root_item->addChild(d.item);
	}
}

void ObjectEditor::load_widget_data(QTreeWidgetItem* itm) {
	auto widget = map.objects.widgets_by_item.at(itm);
	std::cout << "Selected: " + widget->raw_id << std::endl;
	
}


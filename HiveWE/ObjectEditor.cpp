#include "stdafx.h"

bool ObjectEditor::loaded = false;

ObjectEditor::ObjectEditor(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ObjectEditor)
{
    ui->setupUi(this);
	this->installEventFilter(this);
	ui->tabWidget->setCurrentIndex(0);

	setAttribute(Qt::WA_DeleteOnClose);

    QFileIconProvider icons;
    folder_icon = icons.icon(QFileIconProvider::Folder);

	if ( loaded == false ) {
		map.objects.load();
		loaded = true;
	}

	setup();
	show();
}

ObjectEditor::~ObjectEditor()
{
    delete ui;
}

void ObjectEditor::setup(){

    connect(ui->tabWidget, &QTabWidget::currentChanged, [&](int index) { 
		
	});

    //==========================//
    // Units Tab Initialization //
    //==========================// 
    

    ui->units->header()->hide();

    ui->unit_search->installEventFilter(this);
    ui->unit_stats_search->installEventFilter(this);
    ui->unit_stats->header()->setSectionResizeMode(0, QHeaderView::Fixed);
    ui->unit_stats->header()->setSectionResizeMode(1, QHeaderView::Stretch);
	ui->units->setContextMenuPolicy(Qt::CustomContextMenu);

	connect(ui->unit_stats, &QTreeWidget::itemDoubleClicked, [&](QTreeWidgetItem * item, int column) { run_dialog_type(item); });
    connect(ui->unit_search, &QLineEdit::textChanged, [&](const QString &text){  });
	connect(ui->unit_stats_search, &QLineEdit::textChanged, [&](const QString &text) { });

	connect(ui->units, &QTreeWidget::customContextMenuRequested, this, &ObjectEditor::custom_menu_popup);
	connect(ui->units, &QTreeWidget::itemClicked, [&](QTreeWidgetItem * item, int column) { load_unit_data(item); });

	load_units();
	load_unit_field_strings();
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


	connect(ui->items, &QTreeWidget::itemClicked, [&](QTreeWidgetItem * item, int column) { load_item_data(item); });
    //connect(ui->item_search, &QLineEdit::textChanged, [&](const QString &text){ items_proxy_model->setFilterRegExp(text);});
	///connect(ui->item_stats_search, &QLineEdit::textChanged, [&](const QString &text) { items_proxy_model->setFilterRegExp(text); });

	load_items();
	load_item_field_strings();
    //==================================//
    // Destructibles Tab Initialization //
    //==================================//


    ui->destructibles->header()->hide();
    ui->destructibles_search->installEventFilter(this);
    ui->destructibles_stats_search->installEventFilter(this);
    ui->destructibles_stats->header()->setSectionResizeMode(0, QHeaderView::Fixed);
    ui->destructibles_stats->header()->setSectionResizeMode(1, QHeaderView::Stretch);


}

void ObjectEditor::custom_menu_popup(const QPoint& pos) {
	QTreeWidgetItem* item = ui->units->itemAt(pos);
	if (item->childCount() > 0) {
		return;
	}

	QMenu* menu = new QMenu(this);
	QAction* create_as_custom = new QAction("Copy as a Custom Unit...");
	QAction* new_custom_unit = new QAction("New Custom Unit...");
	QAction* delete_custom_unit = new QAction("Delete Custom Unit...");
	
	connect(new_custom_unit, &QAction::triggered, [&]() { create_custom_unit(item); });
	
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
			switch_raw_names();
		}
	} else if (event->type() == QKeyEvent::KeyRelease) {
		QKeyEvent* k_event = dynamic_cast<QKeyEvent*>(event);
		pressed_keys -= k_event->key();
	}

    return false;
}


void ObjectEditor::load_unit_data(QTreeWidgetItem* itm) {}


void ObjectEditor::load_units() {}


void ObjectEditor::load_unit_field_strings() {}


void ObjectEditor::switch_raw_names() {}

void ObjectEditor::display_unit_names() {}


void ObjectEditor::run_dialog_type(QTreeWidgetItem * item) {}

/*void ObjectEditor::change_int(QTreeWidgetItem * item,ObjectField field, int distance) {
	QDialog* int_dialog = new QDialog();
	QSpinBox* spin_box = new QSpinBox();
	QHBoxLayout* horizontal_layout = new QHBoxLayout;
	QVBoxLayout* main_layout = new QVBoxLayout;
	QDialogButtonBox* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
	QLabel* label = new QLabel();
	label->setText(QString::fromStdString(field.display_name));

	int value = std::stoi(item->text(1).toStdString());

	int max_value = std::stol(map.units.units_meta_slk.data("maxVal", field.raw_id)); 
	int min_value = 0; 
	if (auto val = map.units.units_meta_slk.data("minVal", field.raw_id); val != "") {
		min_value = std::stol(val);
	};

	spin_box->setMaximum(max_value);
	spin_box->setMinimum(min_value);
	spin_box->setSingleStep(1);
	spin_box->setValue(value);

	connect(buttons, &QDialogButtonBox::rejected, int_dialog, &QDialog::reject);
	
	connect(buttons, &QDialogButtonBox::accepted, [&]() {
		item->setText(1, spin_box->text());
		if ( value != spin_box->value()) {
			//map.objects.units.at(selected_unit_index).fields.at(distance) = true;
			item->setTextColor(0, changed_field_color);
			item->setTextColor(1, changed_field_color);
		}
		int_dialog->close();

	});

	horizontal_layout->addWidget(label);
	horizontal_layout->addWidget(spin_box);
	main_layout->addLayout(horizontal_layout);
	main_layout->addWidget(buttons);

	int_dialog->setLayout(main_layout);
	int_dialog->setWindowTitle(QString("Edit Unit Value - Integer"));
	int_dialog->exec();
}

void ObjectEditor::change_bool(QTreeWidgetItem * item, ObjectField field, int distance) {
	QDialog* int_dialog = new QDialog();
	QCheckBox* check_box = new QCheckBox();
	QVBoxLayout* main_layout = new QVBoxLayout;
	QDialogButtonBox* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
	
	bool value = std::stoi(item->text(1).toStdString());
	check_box->setText(QString::fromStdString(field.display_name));
	check_box->setChecked(value);
	connect(buttons, &QDialogButtonBox::rejected, int_dialog, &QDialog::reject);

	connect(buttons, &QDialogButtonBox::accepted, [&]() {
		item->setText(1, QString((check_box->isChecked()) ? "1" : "0"));
		if (value != check_box->isChecked()) {
			item->setTextColor(0, changed_field_color);
			item->setTextColor(1, changed_field_color);
		}
		int_dialog->close();

	});

	main_layout->addWidget(check_box);
	main_layout->addWidget(buttons);

	int_dialog->setLayout(main_layout);
	int_dialog->setWindowTitle(QString("Edit Unit Value - Boolean"));
	int_dialog->exec();
}


void ObjectEditor::change_real(QTreeWidgetItem * item, ObjectField field, int distance) {
	QDialog* int_dialog = new QDialog();
	QDoubleSpinBox * spin_box = new QDoubleSpinBox();
	QHBoxLayout* horizontal_layout = new QHBoxLayout;
	QVBoxLayout* main_layout = new QVBoxLayout;
	QDialogButtonBox* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
	QLabel* label = new QLabel();
	label->setText(QString::fromStdString(field.display_name));

	double value = std::stod(item->text(1).toStdString());

	double max_value = std::stod(map.units.units_meta_slk.data("maxVal", field.raw_id));
	double min_value = 0.0;
	if (auto val = map.units.units_meta_slk.data("minVal", field.raw_id); val != "") {
		min_value = std::stod(val);
	};
	
	spin_box->setMaximum(max_value);
	spin_box->setMinimum(min_value);
	spin_box->setSingleStep(0.5);
	spin_box->setValue(value);

	connect(buttons, &QDialogButtonBox::rejected, int_dialog, &QDialog::reject);

	connect(buttons, &QDialogButtonBox::accepted, [&]() {
		item->setText(1, spin_box->text());
		if (value != spin_box->value()) {
			//map.objects.units.at(selected_unit_index).fields.at(distance) = true;
			item->setTextColor(0, changed_field_color);
			item->setTextColor(1, changed_field_color);
		}
		int_dialog->close();

	});

	horizontal_layout->addWidget(label);
	horizontal_layout->addWidget(spin_box);
	main_layout->addLayout(horizontal_layout);
	main_layout->addWidget(buttons);

	int_dialog->setLayout(main_layout);
	int_dialog->setWindowTitle(QString("Edit Unit Value - Real"));
	int_dialog->exec();
}


void ObjectEditor::load_items() {
}



void ObjectEditor::load_item_field_strings() {
}

void ObjectEditor::load_item_data(QTreeWidgetItem * itm) {
}

void ObjectEditor::display_item_names() {
}

void ObjectEditor::create_custom_unit(QTreeWidgetItem* item) {
	QDialog* dialog = new QDialog(this);
	QVBoxLayout* horizontal_layout = new QVBoxLayout();
	QDialogButtonBox* buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

	QHBoxLayout* horizontal_layout_1 = new QHBoxLayout();
	QHBoxLayout* horizontal_layout_2 = new QHBoxLayout();
	QHBoxLayout* horizontal_layout_3 = new QHBoxLayout();

	QLabel* name_label = new QLabel();
	QLabel* suffix_label = new QLabel();
	QLabel* raw_id_label = new QLabel();
	name_label->setText("Name:");
	suffix_label->setText("Suffix:");
	raw_id_label->setText("Raw ID:");

	QLineEdit* name_input = new QLineEdit();
	QLineEdit* suffix_input = new QLineEdit();
	QLineEdit* raw_id_input = new QLineEdit();

	UnitField unit = oe_units.at(item);
	name_input->setText(QString::fromStdString(unit.name));
	suffix_input->setText(QString::fromStdString(unit.suffix));
	raw_id_input->setText(QString::fromStdString(unit.raw_id));
	raw_id_input->setMaxLength(4);

	connect(name_input, &QLineEdit::textChanged, [&](const QString &text) {
		buttons->button(QDialogButtonBox::Ok)->setDisabled(text.isEmpty() || raw_id_input->text().isEmpty());
	});

	connect(raw_id_input, &QLineEdit::textChanged, [&](const QString &text) {
		buttons->button(QDialogButtonBox::Ok)->setDisabled(text.isEmpty() || name_input->text().isEmpty());
	});


	connect(buttons, &QDialogButtonBox::rejected, dialog, &QDialog::reject);

	connect(buttons, &QDialogButtonBox::accepted, [&]() {
		QTreeWidgetItem* new_custom_unit_item = new QTreeWidgetItem();
		QString display_name = (show_as_raw) ? raw_id_input->text() :name_input->text() + " " + suffix_input->text();
		new_custom_unit_item->setText(0, display_name);
		new_custom_unit_item->setIcon(0,item->icon(0));

		unit.name = name_input->text().toStdString();
		unit.suffix = suffix_input->text().toStdString();
		unit.raw_id = raw_id_input->text().toStdString();
		

		oe_units.emplace(new_custom_unit_item, unit);
		ui->units->addTopLevelItem(new_custom_unit_item);

		dialog->close();
	});

	horizontal_layout_1->addWidget(name_label);
	horizontal_layout_1->addWidget(name_input);
	horizontal_layout_2->addWidget(suffix_label);
	horizontal_layout_2->addWidget(suffix_input);
	horizontal_layout_3->addWidget(raw_id_label);
	horizontal_layout_3->addWidget(raw_id_input);

	horizontal_layout->addLayout(horizontal_layout_1);
	horizontal_layout->addLayout(horizontal_layout_2);
	horizontal_layout->addLayout(horizontal_layout_3);
	horizontal_layout->addWidget(buttons);
	dialog->setLayout(horizontal_layout);
	dialog->exec();
}*/
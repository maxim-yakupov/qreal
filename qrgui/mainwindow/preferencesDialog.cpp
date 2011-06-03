#include "preferencesDialog.h"
#include "ui_preferencesDialog.h"

#include <QSettings>
#include <QFileDialog>
PreferencesDialog::PreferencesDialog(QAction * const showGridAction, QAction * const showAlignmentAction
		,QAction * const activateGridAction, QAction * const activateAlignmentAction, QWidget *parent)
	: QDialog(parent), ui(new Ui::PreferencesDialog), mShowGridAction(showGridAction), mShowAlignmentAction(showAlignmentAction)
	, mActivateGridAction(activateGridAction), mActivateAlignmentAction(activateAlignmentAction)
{
	ui->setupUi(this);
	initPreferences();

	connect(ui->linuxButton, SIGNAL(clicked()), this, SLOT(systemChoosingButtonClicked()));
	connect(ui->windowsButton, SIGNAL(clicked()), this, SLOT(systemChoosingButtonClicked()));
	connect(ui->otherButton, SIGNAL(clicked()), this, SLOT(systemChoosingButtonClicked()));
	connect(ui->gridWidthSlider, SIGNAL(sliderMoved(int)), this, SLOT(widthGridSliderMoved(int)));
	connect(ui->indexGridSlider, SIGNAL(sliderMoved(int)), this, SLOT(indexGridSliderMoved(int)));
	connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(cancel()));
	connect(ui->applyButton, SIGNAL(clicked()), this, SLOT(applyChanges()));
	connect(ui->okButton, SIGNAL(clicked()), this, SLOT(saveAndClose()));
	connect(ui->imagesPathBrowseButton, SIGNAL(clicked()), this, SLOT(browseImagesPath()));

	// fixing #105
	ui->indexGridSlider->setVisible(false);
	ui->label_20->setVisible(false);

	mLastIconsetPath = ui->imagesPathEdit->text();
}

void PreferencesDialog::widthGridSliderMoved(int value)
{
	QSettings settings("SPbSU", "QReal");
	settings.setValue("GridWidth", value);
	emit gridChanged();
}

void PreferencesDialog::indexGridSliderMoved(int value)
{
	QSettings settings("SPbSU", "QReal");
	settings.setValue("IndexGrid", value);
	emit gridChanged();
}

PreferencesDialog::~PreferencesDialog()
{
	delete ui;
}

void PreferencesDialog::initPreferences()
{
	QSettings settings("SPbSU", "QReal");
	ui->embeddedLinkerIndentSlider->setValue(settings.value("EmbeddedLinkerIndent", 8).toInt());
	ui->embeddedLinkerSizeSlider->setValue(settings.value("EmbeddedLinkerSize", 6).toInt());
	ui->gridWidthSlider->setValue(settings.value("GridWidth", 10).toInt());
	ui->indexGridSlider->setValue(settings.value("IndexGrid", 50).toInt());
	ui->zoomFactorSlider->setValue(settings.value("zoomFactor", 2).toInt());
	mWithGrid = ui->gridWidthSlider->value();
	mIndexGrid = ui->indexGridSlider->value();

	ui->chooseDiagramsToSaveCheckBox->setChecked(settings.value("ChooseDiagramsToSave", true).toBool());
	ui->diagramCreateCheckBox->setChecked(settings.value("DiagramCreateSuggestion", true).toBool());
	ui->paletteTabCheckBox->setChecked(settings.value("PaletteTabSwitching", true).toBool());
	ui->saveExitCheckBox->setChecked(settings.value("SaveExitSuggestion", true).toBool());
	ui->showGridCheckBox->setChecked(settings.value("ShowGrid", true).toBool());
	ui->showAlignmentCheckBox->setChecked(settings.value("ShowAlignment", true).toBool());
	ui->activateGridCheckBox->setChecked(settings.value("ActivateGrid", true).toBool());
	ui->activateAlignmentCheckBox->setChecked(settings.value("ActivateAlignment", true).toBool());
	ui->antialiasingCheckBox->setChecked(settings.value("Antialiasing", true).toBool());
	ui->splashScreenCheckBox->setChecked(settings.value("Splashscreen", true).toBool());
	ui->openGLCheckBox->setChecked(settings.value("OpenGL", true).toBool());

	ui->imagesPathEdit->setText(settings.value("pathToImages", "./images/iconset1/").toString());

	ui->windowsButton->setChecked(settings.value("windowsButton", false).toBool());
	ui->linuxButton->setChecked(settings.value("linuxButton", false).toBool());
	ui->otherButton->setChecked(settings.value("otherButton", false).toBool());

	ui->pathToQmake->setText(settings.value("pathToQmake", "").toString());
	ui->pathToMake->setText(settings.value("pathToMake", "").toString());
	ui->pluginExtension->setText(settings.value("pluginExtension", "").toString());
	ui->prefix->setText(settings.value("prefix", "").toString());

	ui->timeoutLineEdit->setText(settings.value("debuggerTimeout", 750).toString());
	ui->colorComboBox->addItems(QColor::colorNames());
	QString curColor = settings.value("debugColor", "red").toString();
	int curColorIndex = ui->colorComboBox->findText(curColor);
	ui->colorComboBox->setCurrentIndex(curColorIndex);
	settings.value("debugColor", ui->colorComboBox->currentText());
}

void PreferencesDialog::applyChanges()
{
	QSettings settings("SPbSU", "QReal");
	settings.setValue("EmbeddedLinkerIndent", ui->embeddedLinkerIndentSlider->value());
	settings.setValue("EmbeddedLinkerSize", ui->embeddedLinkerSizeSlider->value());
	settings.setValue("GridWidth", ui->gridWidthSlider->value());
	settings.setValue("IndexGrid", ui->indexGridSlider->value());
	settings.setValue("zoomFactor", ui->zoomFactorSlider->value());
	mWithGrid = ui->gridWidthSlider->value();
	mIndexGrid = ui->indexGridSlider->value();

	settings.setValue("ChooseDiagramsToSave", ui->chooseDiagramsToSaveCheckBox->isChecked());
	settings.setValue("DiagramCreateSuggestion", ui->diagramCreateCheckBox->isChecked());
	settings.setValue("PaletteTabSwitching", ui->paletteTabCheckBox->isChecked());
	settings.setValue("SaveExitSuggestion", ui->saveExitCheckBox->isChecked());
	settings.setValue("Splashscreen", ui->splashScreenCheckBox->isChecked());
	settings.setValue("ShowGrid", ui->showGridCheckBox->isChecked());
	settings.setValue("ShowAlignment", ui->showAlignmentCheckBox->isChecked());
	settings.setValue("ActivateGrid", ui->activateGridCheckBox->isChecked());
	settings.setValue("ActivateAlignment", ui->activateAlignmentCheckBox->isChecked());
	settings.setValue("Antialiasing", ui->antialiasingCheckBox->isChecked());
	settings.setValue("OpenGL", ui->openGLCheckBox->isChecked());

	settings.setValue("pathToImages", ui->imagesPathEdit->text());

	settings.setValue("windowsButton", ui->windowsButton->isChecked());
	settings.setValue("linuxButton", ui->linuxButton->isChecked());
	settings.setValue("otherButton", ui->otherButton->isChecked());

	settings.setValue("pathToQmake", ui->pathToQmake->text());
	settings.setValue("pathToMake", ui->pathToMake->text());
	settings.setValue("pluginExtension", ui->pluginExtension->text());
	settings.setValue("prefix", ui->prefix->text());

	settings.setValue("debuggerTimeout", ui->timeoutLineEdit->text());
	settings.setValue("debugColor", ui->colorComboBox->currentText());

	mShowGridAction->setChecked(ui->showGridCheckBox->isChecked());
	mShowAlignmentAction->setChecked(ui->showAlignmentCheckBox->isChecked());
	mActivateGridAction->setChecked(ui->activateGridCheckBox->isChecked());
	mActivateAlignmentAction->setChecked(ui->activateAlignmentCheckBox->isChecked());

	if (mLastIconsetPath != ui->imagesPathEdit->text())
		emit iconsetChanged();
}

void PreferencesDialog::changeEvent(QEvent *e)
{
	QDialog::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		ui->retranslateUi(this);
		break;
	default:
		break;
	}
}

void PreferencesDialog::saveAndClose()
{
	applyChanges();
	close();
}

void PreferencesDialog::cancel()
{
	ui->gridWidthSlider->setValue(mWithGrid);
	ui->indexGridSlider->setValue(mIndexGrid);
	QSettings settings("SPbSU", "QReal");
	settings.setValue("GridWidth", mWithGrid);
	settings.setValue("IndexGrid", mIndexGrid);
	close();
}

void PreferencesDialog::systemChoosingButtonClicked()
{
	if(ui->linuxButton->isChecked())
		initCompilersSettings("qmake", "make", "so", "lib");
	if(ui->windowsButton->isChecked())
		initCompilersSettings("qmake", "mingw32-make", "dll", "");
	if(ui->otherButton->isChecked())
		ui->compilerSettingsWidget->setEnabled(true);
}

void PreferencesDialog::initCompilersSettings(const QString &pathToQmake,
		const QString &pathToMake, const QString &pluginExtension, const QString &prefix)
{
	ui->pathToQmake->setText(pathToQmake);
	ui->pathToMake->setText(pathToMake);
	ui->pluginExtension->setText(pluginExtension);
	ui->prefix->setText(prefix);
	ui->compilerSettingsWidget->setEnabled(false);
}

void PreferencesDialog::browseImagesPath()
{
	QString path = QFileDialog::getExistingDirectory(this, "Open Directory");
	ui->imagesPathEdit->setText(path.replace("\\", "/"));
}

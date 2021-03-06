#pragma once

#include <QtWidgets/QDialog>
#include <QtWidgets/QListWidgetItem>

#include "dialogs/metamodelingOnFly/restorePropertiesDialog.h"
#include "pluginManager/interpreterEditorManager.h"

namespace Ui {
class EditPropertiesDialog;
}

namespace qReal {

/// Window where you can edit the attributes of the selected property.
class EditPropertiesDialog : public QDialog
{
	Q_OBJECT

public:
	/// Constructor.
	/// @param interpreterEditorManager Editor manager.
	/// @param id Id of an element for which we edit properties.
	/// @param parent Parent widget, who is responsible for deletion of this dialog.
	explicit EditPropertiesDialog(EditorManagerInterface &interpreterEditorManager, Id const &id, QWidget *parent);

	/// Destructor.
	~EditPropertiesDialog();

	/// Selects a property for editing.
	/// @param propertyItem Item in a list of properties which we edit.
	/// @param propertyName Name of a property which we edit.
	/// @param propertyDisplayedName Displayed name of this property.
	void changeProperty(QListWidgetItem *propertyItem, QString const &propertyName
				, QString const &propertyDisplayedName);

private slots:
	void okButtonClicked();
	void messageBoxCancel();
	void updateProperties();
	void acceptPropertyModifications();

private:
	enum Mode
	{
		addNew
		, editExisting
	};

	void initDefaultValues();

	Ui::EditPropertiesDialog *mUi;
	EditorManagerInterface &mInterperterEditorManager;
	RestorePropertiesDialog *mRestorePropertiesDialog;
	Id mId;
	QString mPropertyName;
	QListWidgetItem *mPropertyItem;
	Mode mMode;
};
}

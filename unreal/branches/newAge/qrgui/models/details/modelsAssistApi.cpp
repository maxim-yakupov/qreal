#include "modelsAssistApi.h"
//#include <QtCore/QUuid>

using namespace qReal;
using namespace models;
using namespace details;
using namespace modelsImplementation;

ModelsAssistApi::ModelsAssistApi(AbstractModel &model, EditorManager const &editorManager)
	: mModel(model), mEditorManager(editorManager)
{
}

EditorManager const &ModelsAssistApi::editorManager() const
{
	return mEditorManager;
}

void ModelsAssistApi::setProperty(qReal::Id const &elem, QVariant const &newValue, int const role)
{
	mModel.setData(mModel.indexById(elem), newValue, role);
}

QVariant ModelsAssistApi::property(qReal::Id const &elem, int const role) const
{
	return mModel.data(mModel.indexById(elem), role);
}
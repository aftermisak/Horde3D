// *************************************************************************************************
//
// QPropertyEditor v 0.3
//   
// --------------------------------------
// Copyright (C) 2007 Volker Wiendl
// Acknowledgements to Roman alias banal from qt-apps.org for the Enum enhancement
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
//
// *************************************************************************************************

#include "QPropertyEditorWidget.h"
#include "QPropertyModel.h"
#include "QVariantDelegate.h"
#include "Property.h"

QPropertyEditorWidget::QPropertyEditorWidget(QWidget* parent /*= 0*/) : QTreeView(parent)
{
	m_model = new QPropertyModel(this);		
	setModel(m_model);
	setItemDelegate(new QVariantDelegate(this));
}


QPropertyEditorWidget::~QPropertyEditorWidget()
{
}

void QPropertyEditorWidget::addObject(QObject* propertyObject)
{
	m_model->addItem(propertyObject);
	expandToDepth(0);
}

void QPropertyEditorWidget::setObject(QObject* propertyObject)
{
	m_model->clear();
	if (propertyObject)
		addObject(propertyObject);
}

void QPropertyEditorWidget::updateObject(QObject* propertyObject)
{    
    m_model->updateItem(propertyObject);
}

void QPropertyEditorWidget::registerCustomPropertyCB(UserTypeCB callback)
{
	m_model->registerCustomPropertyCB(callback);
}

void QPropertyEditorWidget::unregisterCustomPropertyCB(UserTypeCB callback)
{
	m_model->unregisterCustomPropertyCB(callback);
}

bool QPropertyEditorWidget::showsProperty(QObject *propertyObject)
{
    if( !propertyObject )
        return m_model->rowCount() == 0;
    QModelIndex index = m_model->index(0, 0);
    QModelIndexList items = m_model->match( index, QPropertyModel::MetaObjectRule, QVariant::fromValue<void*>( (void*) propertyObject), 1, Qt::MatchRecursive | Qt::MatchExactly );
    return !items.empty();
}

void QPropertyEditorWidget::dataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight, const QVector<int> &roles)
{
    // For some reason currently dataChanged is not called for all changed properties.
    // String editor works so far, Spinboxes not,... TODO find out why
#if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
    QTreeView::dataChanged(topLeft, bottomRight, roles );
#else
    QTreeView::dataChanged(topLeft, bottomRight );
#endif
    int rowStart = topLeft.row();
    int rowEnd = bottomRight.row();
    QObject* propertyObject = 0;
    Property* p = 0;
    QModelIndex index;
    for( int i = rowStart; i <= rowEnd; ++i )
    {
        index = m_model->index(i, 0, topLeft.parent() );
        if( (p = static_cast<Property*>(index.internalPointer())) )
        {
            if( p->propertyObject() != propertyObject )
            {
                propertyObject = p->propertyObject();
                emit objectChanged(propertyObject);
            }
        }
    }
}


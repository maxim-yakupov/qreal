#include <QtWidgets/QGraphicsSceneMouseEvent>
#include <QtWidgets/QStyleOptionGraphicsItem>

#include "wallItem.h"

#include "../../../../../qrkernel/settingsManager.h"
#include "d2ModelScene.h"

using namespace qReal::interpreters::robots;
using namespace details::d2Model;
using namespace graphicsUtils;

WallItem::WallItem(QPointF const &begin, QPointF const &end)
	: LineItem(begin, end)
	, mDragged(false)
	, mImage(QImage(":/icons/2d_wall.png"))
{
	setPrivateData();
	setAcceptDrops(true);
	qreal x1;
	qreal y1;
}

void WallItem::setPrivateData()
{
	setZValue(1);

	mPen.setWidth(10);
	mPen.setStyle(Qt::NoPen);
	mBrush.setStyle(Qt::SolidPattern);
	mBrush.setTextureImage(mImage);
	mSerializeName = "wall";
}

QPointF WallItem::begin()
{
	return QPointF(mX1, mY1) + scenePos();
}

QPointF WallItem::end()
{
	return QPointF(mX2, mY2) + scenePos();
}

void WallItem::drawItem(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	Q_UNUSED(option);
	Q_UNUSED(widget);
	painter->drawPath(shape());
}

void WallItem::drawExtractionForItem(QPainter *painter)
{
	if (!isSelected()) {
		return;
	}

	painter->setPen(QPen(Qt::green));
	mLineImpl.drawExtractionForItem(painter, mX1, mY1, mX2, mY2, drift);
	mLineImpl.drawFieldForResizeItem(painter, resizeDrift, mX1, mY1, mX2, mY2);
}

void WallItem::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
	AbstractItem::mousePressEvent(event);
	mDragged = (flags() & ItemIsMovable) || mOverlappedWithRobot;
	x1 = abs(mX1 - event->scenePos().x());
	y1 = abs(mY1 - event->scenePos().y());
}

void WallItem::mouseMoveEvent(QGraphicsSceneMouseEvent * event)
{
	QPointF const oldPos = pos();
	if (SettingsManager::value("2dShowGrid").toBool())
	{
		QPointF pos = event->scenePos();
		qreal deltaX = (mX1 - mX2);
		qreal deltaY = (mY1 - mY2);
		mX1 = pos.x() - x1;
		mY1 = pos.y() - y1;

		this->reshapeBeginWithGrid(SettingsManager::value("2dGridCellSize").toInt());
		this->setDraggedEndWithGrid(deltaX, deltaY);
	} else {
		QGraphicsItem::mouseMoveEvent(event);
	}
	// Items under cursor cannot be dragged when adding new item,
	// but it mustn`t confuse the case when item is unmovable
	// because overapped with robot
	if (mDragged && ((flags() & ItemIsMovable) || mOverlappedWithRobot)) {
		emit wallDragged(this, realShape(), oldPos);
	}
	event->accept();
}

bool WallItem::isDragged()
{
	return mDragged;
}

void WallItem::mouseReleaseEvent(QGraphicsSceneMouseEvent * event)
{
	QGraphicsItem::mouseReleaseEvent(event);
	mDragged = false;
}

QDomElement WallItem::serialize(QDomDocument &document, QPoint const &topLeftPicture)
{
	QDomElement wallNode = document.createElement(mSerializeName);
	wallNode.setAttribute("begin", QString::number(mX1 + scenePos().x() - topLeftPicture.x())
			+ ":" + QString::number(mY1 + scenePos().y() - topLeftPicture.y()));
	wallNode.setAttribute("end", QString::number(mX2 + scenePos().x() - topLeftPicture.x())
			+ ":" + QString::number(mY2 + scenePos().y() - topLeftPicture.y()));
	return wallNode;
}

void WallItem::deserializePenBrush(QDomElement const &element)
{
	Q_UNUSED(element)
	setPrivateData();
}

void WallItem::onOverlappedWithRobot(bool overlapped)
{
	mOverlappedWithRobot = overlapped;
}

int WallItem::sign(qreal x)
{
	if (x<0)
		return -1;
	else if (x>0)
		return 1;
	else
		return 0;
}


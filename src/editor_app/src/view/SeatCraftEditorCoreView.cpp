#include "SeatCraftEditorCoreView.h"

#include "renderder/QTRendererBackend.h"

#include <SeatCraft/common/common_macro.h>
#include <SeatCraftEditorCore/app/SeatCraftEditorCoreApp.hpp>
#include <SeatCraftEditorCore/renderer/SeatCraftEditorCoreRenderer.hpp>

#include <tgfx/gpu/opengl/qt/QGLWindow.h>
#include <tgfx/platform/Print.h>

#include <QApplication>
#include <QDebug>
#include <QFileInfo>
#include <QQuickWindow>
#include <QSGImageNode>
#include <QThread>

namespace kk::view {

SeatCraftEditorCoreView::SeatCraftEditorCoreView(QQuickItem *parent)
    : QQuickItem(parent)
    , _app(std::make_shared<kk::SeatCraftEditorCoreApp>())
    , _renderer(nullptr) {
    tgfx::PrintLog("%s", __PRETTY_FUNCTION__);

    setFlag(ItemHasContents, true);
    setAcceptedMouseButtons(Qt::LeftButton);
    setAcceptHoverEvents(true);
    setAcceptTouchEvents(true);
    setFocus(true);

    auto backend = std::make_shared<kk::renderer::QTRendererBackend>(this);
    _renderer = std::make_shared<kk::renderer::SeatCraftEditorCoreRenderer>(_app, std::move(backend));

    // 监听宽高变化
    connect(this, &SeatCraftEditorCoreView::widthChanged, this, [this]() {
        if (updateSize()) {
            update();
        }
    });

    connect(this, &SeatCraftEditorCoreView::heightChanged, this, [this]() {
        if (updateSize()) {
            update();
        }
    });
}

SeatCraftEditorCoreView::~SeatCraftEditorCoreView() {
    tgfx::PrintLog("%s", __PRETTY_FUNCTION__);
}

bool SeatCraftEditorCoreView::updateSize() {
    auto sizeChanged = _renderer->updateSize();
    if (sizeChanged) {
        invalidateContent();
    }
    return sizeChanged;
}

void SeatCraftEditorCoreView::invalidateContent() {
    _renderer->invalidateContent();
    update();
}

void SeatCraftEditorCoreView::handlePan(float deltaX, float deltaY) {
    UNUSED_PARAM(deltaX);
    UNUSED_PARAM(deltaY);
    update();
}

void SeatCraftEditorCoreView::handlePinch(float scale, float centerX, float centerY) {
    UNUSED_PARAM(scale);
    UNUSED_PARAM(centerX);
    UNUSED_PARAM(centerY);
    update();
}

void SeatCraftEditorCoreView::mousePressEvent(QMouseEvent *event) {
    event->accept();
    QPointF localPos = event->position();
    //    QPointF scenePos = mapToScene(localPos); // 转换为场景坐标
    //    QPointF globalPos = mapToGlobal(localPos); // 转换为全局坐标
    qDebug() << "mousePressEvent (Local): x" << localPos.x() << "y" << localPos.y();
}

void SeatCraftEditorCoreView::mouseMoveEvent(QMouseEvent *event) {
    event->accept();
    QPointF localPos = event->position();
    qDebug() << "mouseMoveEvent (Local): x" << localPos.x() << "y" << localPos.y();
}

void SeatCraftEditorCoreView::mouseReleaseEvent(QMouseEvent *event) {
    event->accept();
    QPointF localPos = event->position();
    qDebug() << "mouseReleaseEvent (Local): x" << localPos.x() << "y" << localPos.y();
}

void SeatCraftEditorCoreView::wheelEvent(QWheelEvent *event) {
    auto modifiers = event->modifiers();
    auto isZoomMode = (modifiers & Qt::KeyboardModifier::ControlModifier) || (modifiers & Qt::KeyboardModifier::MetaModifier);
    if (isZoomMode) {
        QPointF angleDelta = event->angleDelta();
        auto scaleFactor = std::exp(angleDelta.y() / 300.0f);
        qDebug() << "wheelEvent (Zoom): scale" << scaleFactor;
    }
}

QSGNode *SeatCraftEditorCoreView::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *) {

    do {
        auto backend = _renderer->getRendererBackend();
        if (backend == nullptr) {
            return oldNode;
        }

        auto backendWindow = backend->getWindow();
        if (backendWindow == nullptr) {
            return oldNode;
        }

        if (!_connectSceneGraphInvalidated) {
            onBackendWindowCreated();
        }

        _renderer->draw();

        auto tgfxWindow = std::static_pointer_cast<tgfx::QGLWindow>(backendWindow);
        auto node = static_cast<QSGImageNode *>(oldNode);
        auto texture = tgfxWindow->getQSGTexture();
        if (texture == nullptr) {
            return oldNode;
        }

        if (node == nullptr) {
            node = window()->createImageNode();
        }
        node->setTexture(texture);
        node->markDirty(QSGNode::DirtyMaterial);
        node->setRect(boundingRect());
        return node;
    } while (0);

    return oldNode;
}

void SeatCraftEditorCoreView::onBackendWindowCreated() {
    connect(window(), SIGNAL(sceneGraphInvalidated()), this, SLOT(onSceneGraphInvalidated()), Qt::DirectConnection);
    _connectSceneGraphInvalidated = true;
}

void SeatCraftEditorCoreView::onSceneGraphInvalidated() {
    disconnect(window(), SIGNAL(sceneGraphInvalidated()), this, SLOT(onSceneGraphInvalidated()));
    _connectSceneGraphInvalidated = false;
    _renderer->replaceBackend(nullptr);
}

}  // namespace kk::view

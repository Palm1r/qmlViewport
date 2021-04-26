#include "viewport_item.h"

#include <QGuiApplication>
#include <QQuickWindow>
#include <QDebug>

#include <texture_node.h>

ViewportItem::ViewportItem() {
    setFlag(ItemHasContents, true);

    m_surface = new QOffscreenSurface();
    m_surface->create();

//    connect(this, &QQuickItem::windowChanged, this, &ViewportItem::handleWindowChanged);
}

void ViewportItem::handleWindowChanged(QQuickWindow *win)
{
//    if (win) {
//        connect(window(), &QQuickWindow::beforeSynchronizing, this, &ViewportItem::sync, Qt::DirectConnection);
//    }
}

void ViewportItem::sync()
{
    qDebug() << "sync";

    if (window()) {
//    connect(window(), &QQuickWindow::beforeRendering, m_renderer, &SquircleRenderer::init, Qt::DirectConnection);
//    connect(window(), &QQuickWindow::beforeRenderPassRecording, m_renderer, &SquircleRenderer::paint, Qt::DirectConnection);
    }
}

QSGNode *ViewportItem::updatePaintNode(QSGNode *oldNode,
                                       QQuickItem::UpdatePaintNodeData *) {
    if (width() <= 0 || height() <= 0) {
        delete oldNode;
        return nullptr;
    }

    TextureNode *node = static_cast<TextureNode *>(oldNode);
    const auto size = boundingRect().size().toSize();

    if (!node) {
        node = new TextureNode(this);
    }

    if (node->rect().size().toSize() != size && !size.isEmpty()) {
        node->setRect(boundingRect());
        node->surface = m_surface;
        node->sync();
        node->paint();
        node->setTextureCoordinatesTransform(QSGSimpleTextureNode::NoTransform);
        node->setFiltering(QSGTexture::Linear);
    }
    if (window()) {
        connect(window(), &QQuickWindow::beforeRendering, node, &TextureNode::init, Qt::QueuedConnection);
        connect(window(), &QQuickWindow::beforeRenderPassRecording, node, &TextureNode::paint, Qt::QueuedConnection);
    }

    return node;
}

void ViewportItem::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    QQuickItem::geometryChanged(newGeometry, oldGeometry);

    if (newGeometry.size() != oldGeometry.size())
        update();
}

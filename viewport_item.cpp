#include "viewport_item.h"

#include <QGuiApplication>
#include <texture_node.h>

ViewportItem::ViewportItem() {
    setFlag(ItemHasContents, true);

    m_surface = new QOffscreenSurface();
    m_surface->create();
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
        node->setTextureCoordinatesTransform(QSGSimpleTextureNode::NoTransform);
        node->setFiltering(QSGTexture::Linear);
    }

    update();

    return node;
}

void ViewportItem::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    QQuickItem::geometryChanged(newGeometry, oldGeometry);

    if (newGeometry.size() != oldGeometry.size())
        update();
}

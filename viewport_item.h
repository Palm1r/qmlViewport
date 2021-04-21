#pragma once

#include <QQuickItem>

class QOffscreenSurface;
class ViewportItem : public QQuickItem {
    Q_OBJECT

public:
    ViewportItem();

protected:
    QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData *) override;

    // QQuickItem interface
protected:
    void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry) override;

private:
    QOffscreenSurface *m_surface;

};

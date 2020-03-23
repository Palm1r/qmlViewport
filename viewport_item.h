#pragma once

#include <QQuickItem>

class RenderThread;

class ViewportItem : public QQuickItem {
  Q_OBJECT

public:
  ViewportItem();

signals:

public slots:
  void ready();

protected:
  QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData *);

private:
  RenderThread *_renderThread;

  // QQuickItem interface
protected:
  void geometryChanged(const QRectF &newGeometry,
                       const QRectF &oldGeometry) override;
};

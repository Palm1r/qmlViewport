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
  QSGNode *updatePaintNode(QSGNode *, UpdatePaintNodeData *) override;

private:
  RenderThread *_renderThread;
};

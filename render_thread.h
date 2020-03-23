#pragma once

#include <QOffscreenSurface>
#include <QOpenGLContext>
#include <QOpenGLFramebufferObject>
#include <QThread>

class RenderThread : public QThread {
  Q_OBJECT
public:
  RenderThread(const QSize &size);

  QOffscreenSurface *surface;
  QOpenGLContext *context;

  void bgfxInit();

public slots:
  void renderNext();
  void shutDown();

signals:
  void textureReady(int id, const QSize &size);

private:
  QOpenGLFramebufferObject *_renderFbo;
  QOpenGLFramebufferObject *_displayFbo;

  QSize _size;
  int _texture;
};

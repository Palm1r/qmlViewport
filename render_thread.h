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

  QSize _size;

public slots:
  void renderNext();
  void shutDown();
  void resizeBuffers(QSize size);

signals:
  void textureReady(int id, const QSize &size);

private:
  QOpenGLFramebufferObject *_renderFbo;
  QOpenGLFramebufferObject *_displayFbo;

  QOpenGLFramebufferObjectFormat _multiSampleFormat;
  QOpenGLFramebufferObjectFormat _displayFormat;
};

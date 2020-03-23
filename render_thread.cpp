#include "render_thread.h"

#include <QDebug>
#include <QGuiApplication>
#include <QOpenGLFunctions>

RenderThread::RenderThread(const QSize &size)
    : surface(nullptr), context(nullptr), _renderFbo(nullptr),
      _displayFbo(nullptr), _size(size) {}

void RenderThread::renderNext() {
  context->makeCurrent(surface);

  emit textureReady(_texture, _size);
}

void RenderThread::shutDown() {
  context->makeCurrent(surface);
  delete _renderFbo;
  delete _displayFbo;
  context->doneCurrent();
  delete context;

  surface->deleteLater();

  exit();
  moveToThread(QGuiApplication::instance()->thread());
}

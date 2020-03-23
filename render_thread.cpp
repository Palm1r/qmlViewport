#include "render_thread.h"

#include <QDebug>
#include <QGuiApplication>
#include <QOpenGLFunctions>

RenderThread::RenderThread(const QSize &size)
    : surface(nullptr), context(nullptr), _renderFbo(nullptr),
      _displayFbo(nullptr), _size(size) {
  _multiSampleFormat.setAttachment(
      QOpenGLFramebufferObject::CombinedDepthStencil);
  _multiSampleFormat.setMipmap(true);
  _multiSampleFormat.setSamples(8);

  _displayFormat.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
  _displayFormat.setMipmap(true);
}

void RenderThread::renderNext() {
  context->makeCurrent(surface);

  if (_renderFbo == nullptr || _renderFbo->size() != _size) {
    _renderFbo = new QOpenGLFramebufferObject(_size, _multiSampleFormat);
    _displayFbo = new QOpenGLFramebufferObject(_size, _displayFormat);
  }

  _renderFbo->bind();

  auto oglFunctions = context->functions();

  // render
  oglFunctions->glViewport(0, 0, _size.width(), _size.height());
  oglFunctions->glClearColor(0.8f, 0.0f, 0.0f, 1.0f);
  oglFunctions->glClear(GL_COLOR_BUFFER_BIT);
  oglFunctions->glClear(GL_DEPTH_BUFFER_BIT);

  QOpenGLFramebufferObject::blitFramebuffer(_displayFbo, _renderFbo,
                                            GL_COLOR_BUFFER_BIT, GL_NEAREST);

  oglFunctions->glFlush();

  _renderFbo->bindDefault();

  emit textureReady(_displayFbo->texture(), _size);
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

void RenderThread::resizeBuffers(QSize size) {
  _size = size;
  qDebug() << "new size" << _size;
}

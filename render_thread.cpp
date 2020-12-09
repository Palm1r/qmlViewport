#include "render_thread.h"

#include <QDebug>
#include <QGuiApplication>
#include <QOpenGLFunctions>

RenderThread::RenderThread(const QSize &size)
    : surface(nullptr), context(nullptr), _size(size), _renderFbo(nullptr),
      _displayFbo(nullptr) {
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
        _renderFbo =
                std::make_unique<QOpenGLFramebufferObject>(_size, _multiSampleFormat);
        _displayFbo =
                std::make_unique<QOpenGLFramebufferObject>(_size, _displayFormat);
    }

    _renderFbo->bind();

    auto oglFunctions = context->functions();

    // render
    oglFunctions->glViewport(0, 0, _size.width(), _size.height());
    oglFunctions->glClearColor(0.8f, 0.0f, 0.0f, 1.0f);
    oglFunctions->glClear(GL_COLOR_BUFFER_BIT);
    oglFunctions->glClear(GL_DEPTH_BUFFER_BIT);

    QOpenGLFramebufferObject::blitFramebuffer(_displayFbo.get(), _renderFbo.get(),
                                              GL_COLOR_BUFFER_BIT, GL_NEAREST);

    oglFunctions->glFlush();

    _renderFbo->bindDefault();

    emit textureReady(_displayFbo->texture(), _size);
}

void RenderThread::shutDown() {
    context->makeCurrent(surface);
    _renderFbo.reset();
    _displayFbo.reset();
    context->doneCurrent();
    context.reset();

    surface->deleteLater();

    exit();
    moveToThread(QGuiApplication::instance()->thread());
}

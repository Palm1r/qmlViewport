#pragma once

#include <QOffscreenSurface>
#include <QOpenGLContext>
#include <QOpenGLFramebufferObject>
#include <QThread>
#include <memory.h>

class RenderThread : public QThread {
    Q_OBJECT
public:
    RenderThread(const QSize &size);

    QOffscreenSurface *surface;
    std::shared_ptr<QOpenGLContext> context;

    QSize _size;

public slots:
    void renderNext();
    void shutDown();

signals:
    void textureReady(int id, const QSize &size);

private:
    std::unique_ptr<QOpenGLFramebufferObject> _renderFbo;
    std::unique_ptr<QOpenGLFramebufferObject> _displayFbo;

    QOpenGLFramebufferObjectFormat _multiSampleFormat;
    QOpenGLFramebufferObjectFormat _displayFormat;
};

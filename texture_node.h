#pragma once

#include <QSGSimpleTextureNode>
#include <QSGTextureProvider>
#include <QQuickItem>
#include <QOpenGLFramebufferObject>
#include <QOffscreenSurface>

class TextureNode : public QSGTextureProvider, public QSGSimpleTextureNode
{
    Q_OBJECT
public:
    TextureNode(QQuickItem *item);
    ~TextureNode() override = default;

    QOffscreenSurface *surface;
    QOpenGLContext* context;

    QSGTexture *texture() const override;


public slots:
    void init();
    void paint();
    void sync();

private:
    QQuickItem *m_item;
    QQuickWindow *m_window;
    QSizeF m_size;
    qreal m_dpr;

    std::unique_ptr<QOpenGLFramebufferObject> _renderFbo;
    std::unique_ptr<QOpenGLFramebufferObject> _displayFbo;

    QOpenGLFramebufferObjectFormat _multiSampleFormat;
    QOpenGLFramebufferObjectFormat _displayFormat;
    GLuint m_texture;
};

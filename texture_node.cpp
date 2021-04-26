#include "texture_node.h"

#include <QQuickWindow>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_4_3_Compatibility>
#include <QDebug>

TextureNode::TextureNode(QQuickItem *item) : m_item(item)
{
     m_window = m_item->window();
     m_dpr = m_window->effectiveDevicePixelRatio();
     context = QOpenGLContext::currentContext();

     _multiSampleFormat.setAttachment(
                 QOpenGLFramebufferObject::CombinedDepthStencil);
     _multiSampleFormat.setMipmap(true);
     _multiSampleFormat.setSamples(8);

     _displayFormat.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
     _displayFormat.setMipmap(true);
}

QSGTexture *TextureNode::texture() const
{
    return QSGSimpleTextureNode::texture();
}

void TextureNode::sync()
{
    if (!texture()) {
        const auto newSize = (rect().size() * m_dpr).toSize();
        m_size = newSize;

        _renderFbo =
                std::make_unique<QOpenGLFramebufferObject>(newSize, _multiSampleFormat);
        _displayFbo =
                std::make_unique<QOpenGLFramebufferObject>(newSize, _displayFormat);
    }
}

void TextureNode::init()
{
//    qDebug() << "init";

    context->makeCurrent(surface);

    const auto newSize = (rect().size() * m_dpr).toSize();
    if (newSize != m_size) {
        m_size = newSize;

        qDebug() << "resize";

        _renderFbo =
                std::make_unique<QOpenGLFramebufferObject>(newSize, _multiSampleFormat);
        _displayFbo =
                std::make_unique<QOpenGLFramebufferObject>(newSize, _displayFormat);
    }
}

void TextureNode::paint()
{
//    qDebug() << "paint";
    m_window->beginExternalCommands();

    auto oglFunctions = context->versionFunctions<QOpenGLFunctions_4_3_Compatibility>();
    oglFunctions->initializeOpenGLFunctions();

    _renderFbo->bind();

    // render
    oglFunctions->glViewport(0, 0, m_size.width(), m_size.height());
    oglFunctions->glClearColor(0.8f, 0.0f, 0.0f, 1.0f);
    oglFunctions->glClear(GL_COLOR_BUFFER_BIT);
    oglFunctions->glClear(GL_DEPTH_BUFFER_BIT);

    oglFunctions->glLoadIdentity();
    oglFunctions->glBegin(GL_TRIANGLES);
    oglFunctions->glTranslatef(-1.5f,0.0f,-6.0f);
    oglFunctions->glVertex3f( 0.0f, 1.0f, 0.0f);  // Вверх
    oglFunctions->glVertex3f(-1.0f,-1.0f, 0.0f);  // Слева снизу
    oglFunctions->glVertex3f( 1.0f,-1.0f, 0.0f);  // Справа снизу
    oglFunctions->glEnd();

    QOpenGLFramebufferObject::blitFramebuffer(_displayFbo.get(), _renderFbo.get(),
                                              GL_COLOR_BUFFER_BIT, GL_NEAREST);

    oglFunctions->glFlush();

    _renderFbo->bindDefault();

    m_texture = _displayFbo->texture();

    QSGTexture* qsgtexture{nullptr};

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    qsgtexture = QNativeInterface::QSGOpenGLTexture::fromNative(m_texture, m_window,  m_size.toSize(), QQuickWindow::TextureHasAlphaChannel);
#elif QT_VERSION > QT_VERSION_CHECK(5, 15, 0) && QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    qsgtexture = m_window->createTextureFromNativeObject(QQuickWindow::NativeObjectTexture, &m_texture, 0,
                                                       m_size.toSize(), QQuickWindow::TextureHasAlphaChannel);
#else
    qsgtexture = m_window->createTextureFromId(m_texture,  m_size.toSize());
#endif

    setTexture(qsgtexture);

    m_window->resetOpenGLState();
    m_window->endExternalCommands();
}


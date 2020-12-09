#include "texture_node.h"

TextureNode::TextureNode(QQuickWindow *window)
    : _id(0)
    , _size(0, 0)
    , _texture(nullptr)
    , _window(window)
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    _texture = QNativeInterface::QSGOpenGLTexture::fromNative(_id, _window, QSize(1, 1), QQuickWindow::TextureHasAlphaChannel);
#elif QT_VERSION > QT_VERSION_CHECK(5, 15, 0) && QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    _texture = _window->createTextureFromNativeObject(QQuickWindow::NativeObjectTexture, &_id, 0,
                                                      QSize(1, 1), QQuickWindow::TextureHasAlphaChannel);
#else
    _texture = _window->createTextureFromId(0, QSize(1, 1));
#endif

    setTexture(_texture);
    setFiltering(QSGTexture::Linear);
}

TextureNode::~TextureNode()
{
    delete _texture;
}

void TextureNode::newTexture(int id, const QSize &size) {
    _mutex.lock();
    _id = id;
    _size = size;
    _mutex.unlock();

    emit pendingNewTexture();
}

void TextureNode::prepareNode() {
    _mutex.lock();
    int newId = _id;
    QSize size = _size;
    _id = 0;
    _mutex.unlock();
    if (newId) {
        delete _texture;

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    _texture = QNativeInterface::QSGOpenGLTexture::fromNative(newId, _window, size, QQuickWindow::TextureHasAlphaChannel);
#elif QT_VERSION > QT_VERSION_CHECK(5, 15, 0) && QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    _texture = _window->createTextureFromNativeObject(QQuickWindow::NativeObjectTexture, &newId, 0,
                                                      size, QQuickWindow::TextureHasAlphaChannel);
#else
    _texture = _window->createTextureFromId(newId, size);
#endif
        setTexture(_texture);

        markDirty(DirtyMaterial);
        emit textureInUse();
    }
}

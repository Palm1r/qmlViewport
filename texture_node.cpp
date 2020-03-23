#include "texture_node.h"

TextureNode::TextureNode(QQuickWindow *window)
    : _id(0)
    , _size(0, 0)
    , _texture(nullptr)
    , _window(window)
{
    _texture = _window->createTextureFromId(0, QSize(1, 1));
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

        _texture = _window->createTextureFromId(newId, size);
        setTexture(_texture);

        markDirty(DirtyMaterial);

        emit textureInUse();
    }
}

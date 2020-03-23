#pragma once

#include <qsgsimpletexturenode.h>
#include <QMutex>
#include <QQuickWindow>

class TextureNode : public QObject, public QSGSimpleTextureNode
{
    Q_OBJECT
public:
    TextureNode(QQuickWindow *window);
    ~TextureNode() override;

signals:
    void textureInUse();
    void pendingNewTexture();

public slots:
    void newTexture(int id, const QSize &size);
    void prepareNode();

private:
    int _id;
    QSize _size;
    QMutex _mutex;
    QSGTexture *_texture;
    QQuickWindow *_window;
};

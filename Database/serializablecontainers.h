#ifndef SERIALIZABLECONTAINERS_H
#define SERIALIZABLECONTAINERS_H

#include <QByteArray>
#include <QDataStream>
#include <QString>

class SerializableContainers
{
protected:
    template<typename Container>
    QByteArray serializeContainer(Container&& container)
    {
        QByteArray buffer;
        QDataStream stream(&buffer, QDataStream::WriteOnly);
        for(auto&& element: container)
        {
            stream << element;
        }
        return buffer;
    }

    template<typename Container>
    QByteArray serializeContainer(Container&& container) const
    {
        QByteArray buffer;
        QDataStream stream(&buffer, QDataStream::WriteOnly);
        for(auto&& element: container)
        {
            stream << element;
        }
        return buffer;
    }
    template<typename T, template<class> class Container>
    void deserializeContainer(Container<T>& container, const QByteArray& serializedArray)
    {
        QByteArray arrayVertices = serializedArray;
        QDataStream streamVert(&arrayVertices, QDataStream::ReadOnly);
        while(!streamVert.atEnd())
        {
            T element;
            streamVert >> element;
            container.push_back(element);
        }
    }

    virtual ~SerializableContainers(){}
};

#endif // SERIALIZABLECONTAINERS_H

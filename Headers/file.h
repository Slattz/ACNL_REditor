#ifndef FILE_H
#define FILE_H

#include <QFile>
#include <QFileDialog>
#include <QCryptographicHash>
#include <QByteArray>
#include <QtDebug>
#include <QtGlobal>
#include <QMessageBox>

class File
{
    public:
        File();
        ~File();
        File(const QString &fileName, QIODevice::OpenMode openmode = QFile::ReadOnly, bool warning = true);
        void Close(void);
        bool Write(quint32 offset, quint32 bytes, QDataStream::ByteOrder bo = QDataStream::BigEndian);
        bool Write(quint32 offset, QByteArray bytes, QDataStream::ByteOrder bo = QDataStream::BigEndian);
        bool Append(QByteArray bytes, QDataStream::ByteOrder bo = QDataStream::BigEndian);
        quint32 Read(quint32 offset, QDataStream::ByteOrder bo = QDataStream::BigEndian);
        QByteArray* Read(quint32 offset, int size, QDataStream::ByteOrder bo = QDataStream::BigEndian);
        bool Exists();
        QByteArray getHash();
        QFile *m_file = nullptr;

    private:
        bool m_exists = false;
        qint64 m_size = 0;
        QCryptographicHash m_md5Hash;
        QIODevice::OpenMode m_openmode;

};

#endif // FILE_H

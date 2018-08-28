#include "Headers/file.h"
#include "Headers/mainwindow.h"
#include "ui_mainwindow.h"

File::File() : m_md5Hash(QCryptographicHash::Md5) { }
File::~File() { }

File::File(const QString &filePath, QIODevice::OpenMode openmode)
    : File()
{
    m_file = new QFile(filePath);
    if (m_file->exists() && m_file->open(openmode)) {
        m_exists = true;
        m_size = m_file->size();
        m_md5Hash.addData(m_file);
        qDebug().noquote() << "File Path: " + filePath + "\nFile MD5: " + m_md5Hash.result().toHex();
    }

    else {
        QMessageBox::warning(nullptr, "Failed Opening File", "Failed to open file: " + filePath + "!\nSome features may be disabled as a result.");
        m_exists = false;
        if (m_file)
            delete m_file;
    }
}

bool File::Write(quint32 offset, quint32 bytes, QDataStream::ByteOrder bo) {

    if(!this->Exists() || offset >= m_size)
        return false;

    QDataStream fout(m_file);
    fout.setByteOrder(bo);

    if(!m_file->seek(offset))
        return false;

    fout << bytes;
    if (!m_file->flush())
        return false;

    return true;
}

quint32 File::Read(quint32 offset, QDataStream::ByteOrder bo) {

    quint32 bytes;
    if(!this->Exists() || offset >= m_size)
        return 0xFFFFFFFF;

    QDataStream fin(m_file);
    fin.setByteOrder(bo);

    if(!m_file->seek(offset))
        return 0xFFFFFFFF;

    fin >> bytes;
    return bytes;
}

bool File::Exists() {
    return m_exists;
}

QByteArray File::getHash() {
    return m_md5Hash.result();
}

#include "Headers/file.h"

File::File() : m_md5Hash(QCryptographicHash::Md5) { }
File::~File() {
    this->Close();
    m_file = nullptr;
    m_size = 0;
    m_exists = false;
}

File::File(const QString &filePath, QIODevice::OpenMode openmode, bool warning)
    : File()
{
    m_file = new QFile(filePath);
    if (m_file->open(openmode)) {
        m_exists = true;
        m_openmode = openmode;
        m_size = m_file->size();
        m_md5Hash.addData(m_file);
        qDebug().noquote() << "File Path: " + filePath + "\nFile MD5: " + m_md5Hash.result().toHex();
    }

    else {
        if (warning) QMessageBox::warning(nullptr, "Failed Opening File", "Failed to open file: " + filePath + "!\nSome features may be disabled as a result.");
        m_exists = false;
        if (m_file)
            delete m_file;
    }
}

void File::Close(void) {
    if (!m_exists) return;

    if ((m_openmode & QFile::WriteOnly)) m_file->flush();
    m_file->close();
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

bool File::Write(quint32 offset, QByteArray bytes, QDataStream::ByteOrder bo) {

    if(!this->Exists() || offset >= m_size)
        return false;

    QDataStream fout(m_file);
    fout.setByteOrder(bo);

    if(!m_file->seek(offset))
        return false;

    int amount = fout.writeRawData(bytes.constData(), bytes.size());
    qDebug() << "Amount written: " + QString::number(amount);
    if (!m_file->flush())
        return false;

    return true;
}

bool File::Append(QByteArray bytes, QDataStream::ByteOrder bo) {
    if(!this->Exists() || !(m_openmode & QFile::Append))
        return false;

    QDataStream fout(m_file);
    fout.setByteOrder(bo);

    int amount = fout.writeRawData(bytes.constData(), bytes.size());
    qDebug() << "Amount appended: " + QString::number(amount);
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

QByteArray* File::Read(quint32 offset, int size, QDataStream::ByteOrder bo) {

    QByteArray* bytes = new QByteArray;
    if(!this->Exists() || offset >= m_size)
        return bytes;

    QDataStream fin(m_file);
    fin.setByteOrder(bo);

    if(!m_file->seek(offset))
        return bytes;

    bytes->resize(size);
    fin.readRawData(bytes->data(), size);
    return bytes;
}

bool File::Exists() {
    return m_exists;
}

QByteArray File::getHash() {
    return m_md5Hash.result();
}

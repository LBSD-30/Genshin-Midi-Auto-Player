#pragma once

#include <QDateTime>
#include <QThread>
#include <QMutex>
#include <QMutexLocker>
#include <QWaitCondition>
#include <QElapsedTimer>

#include <atomic>
#include <algorithm>
#include <Windows.h>

// �����¼�����
enum KeyAction {
    PRESS,
    RELEASE
};

// midi�¼��ṹ��
struct MidiKeyEvent {
    qint64 timestamp; // ����ʱ���
    char keyChar;     // �����ַ�
    KeyAction action; // ���»��ͷ�
};

// ����ӳ�������������
char noteToKey(int note, int pitchAdjust);

// ����ӳ���Сд��ĸ��������룩
QMap<char, WORD> createKeyMap();

// �����߳�
class MidiPlayThread : public QThread {

    Q_OBJECT

public:

    explicit MidiPlayThread(const QList<MidiKeyEvent>& events, QObject* parent = nullptr)
        : QThread(parent), m_events(events) {}

    // ����event
    void setEvent(QList<MidiKeyEvent>& events);

    // ��ֹ�߳�
    void stop();

protected:

    void run() override;

private:

    std::atomic<bool> m_stop = false;  // �߳�ֹͣ��־��ȷ�����߳̿ɼ���
    QList<MidiKeyEvent> m_events;

    void sendGameKeyEvent(WORD vkCode, KeyAction action);

};
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

// 键盘事件类型
enum KeyAction {
    PRESS,
    RELEASE
};

// midi事件结构体
struct MidiKeyEvent {
    qint64 timestamp; // 毫秒时间戳
    char keyChar;     // 按键字符
    KeyAction action; // 按下或释放
};

// 按键映射和音符处理函数
char noteToKey(int note, int pitchAdjust);

// 键盘映射表（小写字母到虚拟键码）
QMap<char, WORD> createKeyMap();

// 播放线程
class MidiPlayThread : public QThread {

    Q_OBJECT

public:

    explicit MidiPlayThread(const QList<MidiKeyEvent>& events, QObject* parent = nullptr)
        : QThread(parent), m_events(events) {}

    // 设置event
    void setEvent(QList<MidiKeyEvent>& events);

    // 终止线程
    void stop();

protected:

    void run() override;

private:

    std::atomic<bool> m_stop = false;  // 线程停止标志，确保多线程可见性
    QList<MidiKeyEvent> m_events;

    void sendGameKeyEvent(WORD vkCode, KeyAction action);

};
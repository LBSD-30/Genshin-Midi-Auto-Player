#include "MidiPlay.h"

// ����ӳ�������������
char noteToKey(int note, int pitchAdjust) {

    int realNote = note + pitchAdjust * 12;

    // ֻ����48-107��Χ�ڵ����� (C3-B7)
    if (realNote < 48 || realNote > 107) return '\0';

    // �������ӳ���Сд��ĸ��
    static const char* keyMaps[3] = {
        "zxcvbnm",  // C4-B4 (60-71)
        "asdfghj",  // C5-B5 (72-83)
        "qwertyu"   // C6-B6 (84-95)
    };

    // ֱ�Ӽ���˶�ƫ����
    int octaveOffset = 0;

    // ����Ͱ˶����� (C3-B3)
    if (realNote >= 48 && realNote <= 59) {
        octaveOffset = 1; // ӳ�䵽C4-B4��
    }
    // ����߰˶����� (C7-B7)
    else if (realNote >= 96 && realNote <= 107) {
        octaveOffset = -1; // ӳ�䵽C6-B6��
    }

    // ���������ڰ˶��ڵ�λ��
    int noteInOctave = realNote % 12;

    // ����Ƿ��ڰ׼���Χ��
    if (noteInOctave != 0 && noteInOctave != 2 && noteInOctave != 4 &&
        noteInOctave != 5 && noteInOctave != 7 && noteInOctave != 9 && noteInOctave != 11) {
        return '\0';  // ���Ժڼ�
    }

    // �����λ����
    int keyIndex = -1;
    switch (noteInOctave) {
    case 0:  keyIndex = 0; break;  // C
    case 2:  keyIndex = 1; break;  // D
    case 4:  keyIndex = 2; break;  // E
    case 5:  keyIndex = 3; break;  // F
    case 7:  keyIndex = 4; break;  // G
    case 9:  keyIndex = 5; break;  // A
    case 11: keyIndex = 6; break;  // B
    }

    // ����ʵ�ʰ˶���
    int baseOctave = (realNote - 48) / 12; // 0=C3, 1=C4, 2=C5, 3=C6, 4=C7
    int targetOctave = baseOctave + octaveOffset;

    // ȷ��Ŀ��˶�����Ч��Χ��
    if (targetOctave < 1 || targetOctave > 3) return '\0'; // ֻ����C4-C6��

    // ����ӳ���λ
    return keyMaps[targetOctave - 1][keyIndex]; // �������� (1=C4��, 2=C5��, 3=C6��)
}

// ����ӳ���Сд��ĸ��������룩
QMap<char, WORD> createKeyMap() {
    QMap<char, WORD> map;
    map['z'] = 'Z'; // VK_Z
    map['x'] = 'X'; // VK_X
    map['c'] = 'C'; // VK_C
    map['v'] = 'V'; // VK_V
    map['b'] = 'B'; // VK_B
    map['n'] = 'N'; // VK_N
    map['m'] = 'M'; // VK_M

    map['a'] = 'A'; // VK_A
    map['s'] = 'S'; // VK_S
    map['d'] = 'D'; // VK_D
    map['f'] = 'F'; // VK_F
    map['g'] = 'G'; // VK_G
    map['h'] = 'H'; // VK_H
    map['j'] = 'J'; // VK_J

    map['q'] = 'Q'; // VK_Q
    map['w'] = 'W'; // VK_W
    map['e'] = 'E'; // VK_E
    map['r'] = 'R'; // VK_R
    map['t'] = 'T'; // VK_T
    map['y'] = 'Y'; // VK_Y
    map['u'] = 'U'; // VK_U
    return map;
}

// ����event
void MidiPlayThread::setEvent(QList<MidiKeyEvent>& events) {
    m_events = events;
}

// ��ֹ�߳�
void MidiPlayThread::stop() {
    m_stop = true; 
}

void MidiPlayThread::run() {

    if (m_events.empty()) return;

    // ��ȡ��ʼʱ��
    auto startTime = QDateTime::currentMSecsSinceEpoch();

    int eventIndex = 0;            // �¼���������
    qint64 accumulatedOffset = 0;  // �ۼ���ͣƫ��ʱ��

    // ��������ӳ��
    static QMap<char, WORD> keyMap = createKeyMap();

    // ���Ͱ����¼�
    while (!m_stop && eventIndex < m_events.size()) {

        const auto& event = m_events[eventIndex];

        // ������Ҫ�ȴ���ʱ��
        qint64 now = QDateTime::currentMSecsSinceEpoch();
        qint64 waitTime = event.timestamp - (now - startTime);

        if (waitTime > 0) {
            msleep(static_cast<unsigned long>(waitTime));
        }
        
        // �����������
        if (!keyMap.contains(event.keyChar)) {
            eventIndex++;
            continue;
        }

        WORD vkCode = keyMap[event.keyChar];
        sendGameKeyEvent(vkCode, event.action);
        QThread::msleep(5);

        eventIndex++;

    }

    m_stop = true;

}

void MidiPlayThread::sendGameKeyEvent(WORD vkCode, KeyAction action) {
    INPUT inputs[2] = { 0 };
    int inputCount = 0;

    // �������¼�
    inputs[inputCount].type = INPUT_KEYBOARD;
    inputs[inputCount].ki.wVk = vkCode;

    if (action == PRESS) {
        inputs[inputCount].ki.dwFlags = 0;
    }
    else {
        inputs[inputCount].ki.dwFlags = KEYEVENTF_KEYUP;
    }
    inputCount++;

    // ���ⷢ��ɨ����
    inputs[inputCount].type = INPUT_KEYBOARD;
    inputs[inputCount].ki.wVk = 0; // ���������Ϊ0
    inputs[inputCount].ki.wScan = MapVirtualKey(vkCode, MAPVK_VK_TO_VSC);
    inputs[inputCount].ki.dwFlags = KEYEVENTF_SCANCODE;

    if (action == RELEASE) {
        inputs[inputCount].ki.dwFlags |= KEYEVENTF_KEYUP;
    }
    inputCount++;

    SendInput(inputCount, inputs, sizeof(INPUT));
}
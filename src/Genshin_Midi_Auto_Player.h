#pragma once

#include <QMainWindow>
#include <QElapsedTimer>
#include <QListWidgetItem>

#include <MidiFile.h>

#include "ui_Genshin_Midi_Auto_Player.h"
#include "MidiPlay.h"

class GenshinMidiAutoPlayer : public QMainWindow {

    Q_OBJECT

public:

    GenshinMidiAutoPlayer(QWidget* parent = nullptr);
    ~GenshinMidiAutoPlayer();

protected:

private slots:

    // ˢ���ļ��б�
    void reloadFileList();

    // ж��midi�ļ�
    void unloadMidiFile();

    // ��ȡѡ�е�midi�ļ�
    void readMidiFile();

    // ���ߵ���
    void onPitchLowerButtonClicked();
    void onPitchKeepButtonClicked();
    void onPitchHigherButtonClicked();

    // midi���ſ���
    void onStartButtonClicked();
    void onStopButtonClicked();


private:

    // ���UI����
    Ui::MainWindow ui;

    // ��ǰmidi�ļ�
    smf::MidiFile midiFile;

    /* 
    ���ߵ���
    0Ϊ���Ͱ˶ȣ�1Ϊ���䣬2Ϊ�����˶�
    */
    int pitchAdjust;

    // midi�����¼�
    QList<MidiKeyEvent> keyEvents;

    // midi�����߳�
    MidiPlayThread* playThread = nullptr;

    // ����midi�ļ�·��
    void importMidiFilePath();

    // ��ȡ��ǰѡ���ļ������ƣ�std::string��
    std::string getSelectedFileName(QListWidget* listWidget);

};
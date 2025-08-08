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

    // 刷新文件列表
    void reloadFileList();

    // 卸载midi文件
    void unloadMidiFile();

    // 读取选中的midi文件
    void readMidiFile();

    // 音高调整
    void onPitchLowerButtonClicked();
    void onPitchKeepButtonClicked();
    void onPitchHigherButtonClicked();

    // midi播放控制
    void onStartButtonClicked();
    void onStopButtonClicked();


private:

    // 添加UI对象
    Ui::MainWindow ui;

    // 当前midi文件
    smf::MidiFile midiFile;

    /* 
    音高调整
    0为降低八度，1为不变，2为提升八度
    */
    int pitchAdjust;

    // midi音符事件
    QList<MidiKeyEvent> keyEvents;

    // midi播放线程
    MidiPlayThread* playThread = nullptr;

    // 导入midi文件路径
    void importMidiFilePath();

    // 获取当前选中文件的名称（std::string）
    std::string getSelectedFileName(QListWidget* listWidget);

};
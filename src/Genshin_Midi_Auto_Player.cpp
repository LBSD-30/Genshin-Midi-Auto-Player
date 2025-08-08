#include "Genshin_Midi_Auto_Player.h"

#include <QFileDialog>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTimer>
#include <QMessageBox>
#include <QFileInfo>
#include <QMimeData>
#include <QUrl>
#include <QDir>
#include <QDropEvent>


GenshinMidiAutoPlayer::GenshinMidiAutoPlayer(QWidget* parent)
    : QMainWindow(parent), pitchAdjust(1) {

    // 设置UI
    ui.setupUi(this);
    ui.info->setText("请选择文件");

    this->setWindowTitle("Genshin Midi Auto Player");

    // 按钮初始化
    ui.pitchLowerButton->setChecked(false);
    ui.pitchKeepButton->setChecked(true);
    ui.pitchHigherButton->setChecked(false);
    ui.startButton->setEnabled(false);
    ui.stopButton->setEnabled(false);

    // 启用文件列表排序
    ui.fileList->setSortingEnabled(true);

    // 导入midi文件路径
    importMidiFilePath();

    // 初始化midi播放线程
    playThread = new MidiPlayThread(keyEvents, this);

    // 连接信号
    connect(ui.reloadFileListButton, &QPushButton::clicked,             // 重新识别按钮 
            this, &GenshinMidiAutoPlayer::reloadFileList);
    connect(ui.chooseFileButton, &QPushButton::clicked,                 // 加载按钮
            this, &GenshinMidiAutoPlayer::readMidiFile); 
    connect(ui.fileList, &QListWidget::itemDoubleClicked,               // 双击文件加载
            this, &GenshinMidiAutoPlayer::readMidiFile);
    connect(ui.pitchLowerButton, &QRadioButton::toggled,                // 音高降低按钮
            this, &GenshinMidiAutoPlayer::onPitchLowerButtonClicked);
    connect(ui.pitchKeepButton, &QRadioButton::toggled,                 // 音高保持按钮
            this, &GenshinMidiAutoPlayer::onPitchKeepButtonClicked);
    connect(ui.pitchHigherButton, &QRadioButton::toggled,               // 音高提升按钮
            this, &GenshinMidiAutoPlayer::onPitchHigherButtonClicked);  
    connect(ui.startButton, &QPushButton::clicked,                      // 开始播放按钮
            this, &GenshinMidiAutoPlayer::onStartButtonClicked);
    connect(ui.stopButton, &QPushButton::clicked,                       // 停止播放按钮
            this, &GenshinMidiAutoPlayer::onStopButtonClicked);
    connect(playThread, &MidiPlayThread::playEnded,                     // 播放线程结束也连接到停止播放
            this, &GenshinMidiAutoPlayer::onStopButtonClicked);

}

GenshinMidiAutoPlayer::~GenshinMidiAutoPlayer() {
    playThread->stop();
    playThread->wait();
    delete playThread;
}

// 刷新文件列表
void GenshinMidiAutoPlayer::reloadFileList() {
    importMidiFilePath();
}

// 卸载midi文件
void GenshinMidiAutoPlayer::unloadMidiFile() {
    midiFile.clear();
    ui.info->setText("请选择文件");
    ui.startButton->setEnabled(false);
    ui.stopButton->setEnabled(false);
}

// 读取选中的midi文件
void GenshinMidiAutoPlayer::readMidiFile() {

    // 读取midi文件
    midiFile.clear();
    if (!midiFile.read("playlist/" + getSelectedFileName(ui.fileList))) { 
        ui.info->setText("failed to load midi file.");
        ui.startButton->setEnabled(false);
        ui.stopButton->setEnabled(false);
        return;
    }
    ui.info->setText("successful loaded midi file.");
    ui.startButton->setEnabled(true);
    ui.stopButton->setEnabled(false);

    playThread->terminate();

}

// 音高调整
void GenshinMidiAutoPlayer::onPitchLowerButtonClicked() {
    pitchAdjust = 0;
}
void GenshinMidiAutoPlayer::onPitchKeepButtonClicked() {
    pitchAdjust = 1;
}
void GenshinMidiAutoPlayer::onPitchHigherButtonClicked() {
    pitchAdjust = 2;
}

// 开始播放midi文件
void GenshinMidiAutoPlayer::onStartButtonClicked() {

    ui.info->setText("playing...");
    ui.pitchLowerButton->setEnabled(false);
    ui.pitchKeepButton->setEnabled(false);
    ui.pitchHigherButton->setEnabled(false);

    // 确保时间分析已完成
    midiFile.doTimeAnalysis();
    keyEvents.clear();

    // 遍历所有音轨和事件
    for (int track = 0; track < midiFile.getTrackCount(); ++track) {
        for (int eventIdx = 0; eventIdx < midiFile[track].size(); ++eventIdx) {
            auto& event = midiFile[track][eventIdx];

            // 直接使用isNoteOn和isNoteOff方法判断
            bool isNoteOn = event.isNoteOn();
            bool isNoteOff = event.isNoteOff();
            if (!isNoteOn && !isNoteOff) continue;  // 跳过非音符事件

            // 获取音符值
            int note = event.getKeyNumber();
            char key = noteToKey(note, pitchAdjust);
            if (!key) continue; // 跳过无效音符

            // 计算事件时间（毫秒）
            double seconds = midiFile.getTimeInSeconds(event.tick);
            qint64 timestamp = static_cast<qint64>(seconds * 1000);

            // 创建按键事件
            if (isNoteOn && event.getVelocity() > 0) {
                // Note On 且力度大于0 - 按键按下
                keyEvents.append({ timestamp, key, PRESS });
            }
            else {
                // 包括：Note Off 事件或Note On但力度为0
                keyEvents.append({ timestamp, key, RELEASE });
            }
        }
    }

    // 按时间排序所有事件
    std::sort(keyEvents.begin(), keyEvents.end(), [](const MidiKeyEvent& a, const MidiKeyEvent& b) {
        return a.timestamp < b.timestamp;
    });

    // 创建并启动播放线程
    playThread->setEvent(keyEvents);
    connect(playThread, &QThread::finished, playThread, &QObject::deleteLater);
    playThread->start();

    // 设置按钮有效状态
    ui.startButton->setEnabled(false);
    ui.stopButton->setEnabled(true);
}

void GenshinMidiAutoPlayer::onStopButtonClicked() {

    ui.info->setText("请选择文件");

    // 设置按钮有效状态
    ui.startButton->setEnabled(false);
    ui.stopButton->setEnabled(false);
    ui.pitchLowerButton->setEnabled(true);
    ui.pitchKeepButton->setEnabled(true);
    ui.pitchHigherButton->setEnabled(true);

    keyEvents.clear();

    // 终止线程
    playThread->stop();
    playThread->wait();
    playThread->deleteLater();
    playThread = new MidiPlayThread(keyEvents, this);
}

// 导入midi文件路径
void GenshinMidiAutoPlayer::importMidiFilePath() {

    QDir dir("playlist");

    // 设置过滤器：只获取文件（排除目录），可选是否显示隐藏文件
    QStringList filters;
    filters << "*.mid"; // 指定扩展名".mid"
    dir.setNameFilters(filters);
    dir.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);

    // 获取文件名列表
    QStringList fileNames = dir.entryList();
    ui.fileList->clear();
    for (const QString& fileName : fileNames) {
        // 可选：显示完整路径
        // QString filePath = dir.filePath(fileName);
        ui.fileList->addItem(fileName);
    }

}

// 获取当前选中文件的名称（std::string）
std::string GenshinMidiAutoPlayer::getSelectedFileName(QListWidget* listWidget) {
    if (QListWidgetItem* item = listWidget->currentItem()) {
        return item->text().toStdString(); // QString → std::string
    }
    return ""; // 无选中项时返回空字符串
}

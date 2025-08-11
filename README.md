# Genshin Midi Auto Player

自动播放 midi 文件到《原神》风物之诗琴上的工具

## 下载安装
1. 前往 [Release 页面](https://github.com/LBSD-30/Genshin-Midi-Auto-Player/releases) 下载安装包
2. 运行 `Genshin Midi Auto Player ${APP_VERSION} Setup.exe`
3. 按向导完成安装

## 添加 midi
直接将midi(.mid)文件放置在playlist文件夹下即可，注意midi文件名称不能含有中文，必须是ASCII字符

Release版本的playlist中自带一首senbonzakura.mid，可以直接使用

## 开始使用
> 注意需要使用管理员身份运行程序

程序会自动读取playlist文件夹下的所有midi(.mid)文件并展现在列表中，如果你添加了新的文件可以点击刷新列表使其显示

点击读取文件或双击列表中的文件即可加载，开始播放后保证窗口焦点在《原神》中即可

## 编译指南
详细编译说明请参阅 [BUILD.md](docs/BUILD.md)

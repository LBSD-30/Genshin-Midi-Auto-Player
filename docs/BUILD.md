# Genshin Midi Auto Player 编译指南

本文档提供在 Windows 平台上编译本项目的详细说明，支持两种开发环境：
1. **Visual Studio 2022** (使用 Qt VS Tools 扩展)
2. **Qt Creator**

## 环境要求

### Visual Studio 方案
- Visual Studio 2022 ([下载](https://visualstudio.microsoft.com/))
  - 安装时勾选：
    - "使用 C++ 的桌面开发"
    - "Windows 10 SDK"
- Qt VS Tools 扩展 ([安装指南](https://marketplace.visualstudio.com/items?itemName=TheQtCompany.QtVisualStudioTools-19123))

### Qt Creator 方案
- Qt 6.x MSVC 2019/2022 64位 ([下载](https://www.qt.io/download))
- Qt Creator ([包含在 Qt 安装包中](https://www.qt.io/download))

> **注意**：两种方案都需要安装 **MSVC 2022 构建工具** 和 **Qt 6.x for MSVC**

---

## 使用 Visual Studio + Qt VS Tools 编译

### 步骤 1：克隆仓库

git clone https://github.com/LBSD-30/Genshin-Midi-Auto-Player.git

cd Genshin-Midi-Auto-Player

### 步骤 2：配置 Qt VS Tools
1. 打开 Visual Studio 2022
2. 转到菜单：`扩展` → `Qt VS Tools` → `Qt Options`
3. 添加 Qt 安装路径（例如：`C:\Qt\6.9.1\msvc2022_64`）
4. 设置版本名称（如 "Qt 6.7.0 MSVC2022 64-bit"）

### 步骤 3：打开项目
1. `文件` → `打开` → `项目/解决方案`
2. 选择项目目录中的 `.sln` 文件（如果存在）或 `.pro` 文件

### 步骤 4：配置解决方案
1. 在解决方案资源管理器中右键项目 → `属性`
2. 确认配置：
   - 配置：**Release**
   - 平台：**x64**
3. 检查 Qt 设置：
   - `Qt Project Settings` → 选择正确的 Qt 版本
   - `Qt Modules` → 确保包含 core, gui, widgets

### 步骤 5：编译运行
1. 菜单：`生成` → `生成解决方案` (Ctrl+Shift+B)
2. 输出文件位置：`x64\Release\Genshin_Midi_Auto_Player.exe`

---

## 使用 Qt Creator 编译

### 步骤 1：导入项目
1. 打开 Qt Creator
2. `文件` → `打开文件或项目`
3. 选择项目中的 `.pro` 文件

### 步骤 2：配置 Kits
1. 转到 `项目` 标签页 (左侧)
2. 选择/创建 Kit：
   - 设备类型：Desktop
   - 编译器：MSVC 2022 (x64)
   - Qt 版本：Qt 6.x MSVC2022 64-bit
3. 构建配置：
   - 构建模式：Release
   - 构建目录：`../build-GenshinMidi-Desktop_Qt_6_9_1_MSVC2022_64bit-Release`

### 步骤 3：构建项目
1. 点击左下角 `构建` 按钮 (锤子图标)
2. 或使用菜单：`构建` → `构建项目 "GenshinMidi"`

### 步骤 4：运行程序
1. 点击 `运行` 按钮 (绿色三角)
2. 输出文件位置：`build-*` 目录下的 `release` 文件夹

---

## 生成安装包

### 使用 NSIS 打包
1. 安装 NSIS 3.x ([下载](https://nsis.sourceforge.io/Download))
2. 准备发布文件：

在项目根目录运行

`mkdir distribute`

`xcopy /E /I x64\Release distribute`

3. 编译安装程序：

`cd installer`

`makensis GenshinMidi_Installer.nsi`

4. 安装包输出：`Genshin Midi Auto Player 1.0.0 Setup.exe`

---

> 更多问题请提交到 [Issues](https://github.com/LBSD-30/Genshin-Midi-Auto-Player/issues)

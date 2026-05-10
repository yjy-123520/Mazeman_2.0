#include "mainwindow.h"
#include "gamewidget.h"
#include "mazeeditor.h"
#include "mapselect.h"
#include <QInputDialog>
#include <QDir>
#include <QApplication>
#include <QFile>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), centralWidget(nullptr), layout(nullptr)
{
    setWindowTitle("Mazeman");
    setFixedSize(700, 700);
    setupUI();
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI()
{
    showMainMenu();
}

void MainWindow::showMainMenu()
{
    // 清除旧内容
    if (centralWidget) {
        delete centralWidget;
        centralWidget = nullptr;
    }

    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    layout = new QVBoxLayout(centralWidget);
    layout->setAlignment(Qt::AlignCenter);
    layout->setSpacing(20);

    // 标题
    titleLabel = new QLabel("Mazeman", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet(
        "font-size: 80px;"
        "font-weight: bold;"
        "color: #333;"
        "margin-bottom: 50px;"
        );

    // 按钮样式
    QString buttonStyle =
        "QPushButton {"
        "  font-size: 24px;"
        "  padding: 15px 60px;"
        "  background-color: #4a90d9;"
        "  color: white;"
        "  border: none;"
        "  border-radius: 10px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #357abd;"
        "}"
        "QPushButton:pressed {"
        "  background-color: #2a5f9e;"
        "}";

    // Start按钮 - 只能选择地图进入游戏
    startButton = new QPushButton("Start", this);
    startButton->setStyleSheet(buttonStyle);

    // Map按钮 - 只负责编辑、删除、新建地图
    mapButton = new QPushButton("Map", this);
    mapButton->setStyleSheet(buttonStyle);

    // Exit按钮
    exitButton = new QPushButton("Exit", this);
    exitButton->setStyleSheet(
        "QPushButton {"
        "  font-size: 24px;"
        "  padding: 15px 60px;"
        "  background-color: #e74c3c;"
        "  color: white;"
        "  border: none;"
        "  border-radius: 10px;"
        "}"
        "QPushButton:hover {"
        "  background-color: #c0392b;"
        "}"
        );

    layout->addWidget(titleLabel);
    layout->addWidget(startButton);
    layout->addWidget(mapButton);
    layout->addWidget(exitButton);

    connect(startButton, &QPushButton::clicked, this, &MainWindow::onStartClicked);
    connect(mapButton, &QPushButton::clicked, this, &MainWindow::onMapClicked);
    connect(exitButton, &QPushButton::clicked, this, &MainWindow::onExitClicked);
}

void MainWindow::onStartClicked()
{
    // 只显示可选地图，不包含编辑功能
    MapSelect *mapSelect = new MapSelect(this);
    mapSelect->setMode(true);  // 设置为游戏模式

    int result = mapSelect->exec();

    if (result == QDialog::Accepted) {
        std::string mapName = mapSelect->selectedMap();
        if (!mapName.empty()) {
            GameWidget *game = new GameWidget(mapName, this);
            game->exec();
            delete game;
        }
    }

    delete mapSelect;
    showMainMenu();  // 返回主菜单
}

void MainWindow::onMapClicked()
{
    // 地图管理器 - 只负责编辑、删除、新建
    MapSelect *mapSelect = new MapSelect(this);
    mapSelect->setMode(false);  // 设置为编辑模式

    // 连接新建地图信号
    connect(mapSelect, &MapSelect::newMapRequested, this, [this, mapSelect]() {
        // 创建空白编辑器，保存后命名
        MazeEditor *editor = new MazeEditor(this);
        editor->setWindowTitle("新建地图");

        if (editor->exec() == QDialog::Accepted) {
            // 编辑器保存成功后，弹出命名对话框
            bool ok;
            QString mapName = QInputDialog::getText(this, "保存地图",
                                                    "请输入地图名称:", QLineEdit::Normal, "", &ok);

            if (ok && !mapName.isEmpty()) {
                QDir dir(QApplication::applicationDirPath());
                QString filePath = dir.filePath(mapName + ".maze");

                if (!QFile::exists(filePath)) {
                    // 重命名临时文件
                    QString tempPath = dir.filePath("new_map.maze");
                    QFile::rename(tempPath, filePath);
                    mapSelect->refreshMapList();
                } else {
                    QMessageBox::warning(this, "保存失败",
                                         QString("地图 \"%1\" 已存在").arg(mapName));
                }
            }
        }
        delete editor;
    });

    int result = mapSelect->exec();

    if (result == 2) {  // 编辑模式
        std::string mapName = mapSelect->selectedMap();
        if (!mapName.empty()) {
            MazeEditor *editor = new MazeEditor(this);
            if (editor->loadMap(mapName)) {
                editor->exec();
            }
            delete editor;
        }
    }

    delete mapSelect;
    showMainMenu();  // 返回主菜单
}

void MainWindow::onExitClicked()
{
    close();
}
#include "mapselect.h"
#include <QDir>
#include <QFileInfo>
#include <QApplication>
#include <QMessageBox>
#include <QFile>
#include <QInputDialog>
#include <QFileDialog>

MapSelect::MapSelect(QWidget *parent)
    : QDialog(parent), isPlayMode(true)
{
    setWindowTitle("Mazeman - Map Manager");
    setFixedSize(500, 600);
    setModal(true);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // 标题
    QLabel *titleLabel = new QLabel("地图管理", this);
    titleLabel->setStyleSheet(
        "font-size: 24px;"
        "font-weight: bold;"
        "color: #333333;"
        "margin-bottom: 5px;"
        );
    titleLabel->setAlignment(Qt::AlignCenter);

    // 说明标签
    infoLabel = new QLabel("选择地图进行操作：", this);
    infoLabel->setStyleSheet("font-size: 14px; color: #666666; margin-bottom: 3px;");

    // 地图列表 - 白底黑字
    mapList = new QListWidget(this);
    mapList->setMinimumHeight(250);
    mapList->setStyleSheet(
        "QListWidget {"
        "  font-size: 16px;"
        "  border: 2px solid #cccccc;"
        "  border-radius: 8px;"
        "  padding: 5px;"
        "  background-color: white;"
        "}"
        "QListWidget::item {"
        "  padding: 12px;"
        "  border-bottom: 1px solid #eeeeee;"
        "  color: #000000;"
        "  background-color: white;"
        "}"
        "QListWidget::item:selected {"
        "  background-color: white;"
        "  color: #000000;"
        "  border: 2px solid #87CEEB;"  // 浅蓝色边框
        "  border-radius: 4px;"
        "}"
        "QListWidget::item:hover {"
        "  background-color: #f0f8ff;"  // 鼠标悬停时的浅蓝背景
        "}"
        );

    // 按钮容器
    QWidget *buttonContainer = new QWidget(this);
    QVBoxLayout *buttonLayout = new QVBoxLayout(buttonContainer);
    buttonLayout->setSpacing(10);
    buttonLayout->setAlignment(Qt::AlignCenter);

    // 新建地图按钮（默认显示）
    newMapButton = new QPushButton("新建地图", this);
    newMapButton->setMinimumWidth(200);
    newMapButton->setMinimumHeight(50);
    newMapButton->setStyleSheet(
        "QPushButton {"
        "  font-size: 18px;"
        "  padding: 12px 40px;"
        "  background-color: #4a90d9;"
        "  color: white;"
        "  border: none;"
        "  border-radius: 8px;"
        "  font-weight: bold;"
        "}"
        "QPushButton:hover { background-color: #357abd; }"
        "QPushButton:pressed { background-color: #2a5f9e; }"
        );

    // 游戏开始按钮（选择地图后显示）
    startButton = new QPushButton("开始游戏", this);
    startButton->setMinimumWidth(200);
    startButton->setMinimumHeight(50);
    startButton->setStyleSheet(
        "QPushButton {"
        "  font-size: 18px;"
        "  padding: 12px 40px;"
        "  background-color: #27ae60;"
        "  color: white;"
        "  border: none;"
        "  border-radius: 8px;"
        "  font-weight: bold;"
        "}"
        "QPushButton:hover { background-color: #229954; }"
        "QPushButton:pressed { background-color: #1e8449; }"
        );

    // 操作按钮组（选中地图后显示）
    QHBoxLayout *actionLayout = new QHBoxLayout();
    actionLayout->setSpacing(15);

    // 重命名按钮
    renameButton = new QPushButton("重命名", this);
    renameButton->setMinimumWidth(100);
    renameButton->setMinimumHeight(50);
    renameButton->setStyleSheet(
        "QPushButton {"
        "  font-size: 16px;"
        "  padding: 10px 20px;"
        "  background-color: #f39c12;"
        "  color: white;"
        "  border: none;"
        "  border-radius: 8px;"
        "  font-weight: bold;"
        "}"
        "QPushButton:hover { background-color: #e67e22; }"
        "QPushButton:pressed { background-color: #d35400; }"
        );

    // 修改按钮
    editButton = new QPushButton("修改", this);
    editButton->setMinimumWidth(100);
    editButton->setMinimumHeight(50);
    editButton->setStyleSheet(
        "QPushButton {"
        "  font-size: 16px;"
        "  padding: 10px 20px;"
        "  background-color: #27ae60;"
        "  color: white;"
        "  border: none;"
        "  border-radius: 8px;"
        "  font-weight: bold;"
        "}"
        "QPushButton:hover { background-color: #229954; }"
        "QPushButton:pressed { background-color: #1e8449; }"
        );

    // 删除按钮
    deleteButton = new QPushButton("删除", this);
    deleteButton->setMinimumWidth(100);
    deleteButton->setMinimumHeight(50);
    deleteButton->setStyleSheet(
        "QPushButton {"
        "  font-size: 16px;"
        "  padding: 10px 20px;"
        "  background-color: #e74c3c;"
        "  color: white;"
        "  border: none;"
        "  border-radius: 8px;"
        "  font-weight: bold;"
        "}"
        "QPushButton:hover { background-color: #c0392b; }"
        "QPushButton:pressed { background-color: #a93226; }"
        );

    // 取消按钮
    cancelButton = new QPushButton("取消", this);
    cancelButton->setMinimumWidth(100);
    cancelButton->setMinimumHeight(50);
    cancelButton->setStyleSheet(
        "QPushButton {"
        "  font-size: 16px;"
        "  padding: 10px 20px;"
        "  background-color: #95a5a6;"
        "  color: white;"
        "  border: none;"
        "  border-radius: 8px;"
        "  font-weight: bold;"
        "}"
        "QPushButton:hover { background-color: #7f8c8d; }"
        "QPushButton:pressed { background-color: #6c7a7a; }"
        );

    actionLayout->addStretch();
    actionLayout->addWidget(renameButton);
    actionLayout->addWidget(editButton);
    actionLayout->addWidget(deleteButton);
    actionLayout->addWidget(cancelButton);
    actionLayout->addStretch();

    buttonLayout->addStretch();
    buttonLayout->addWidget(newMapButton);
    buttonLayout->addWidget(startButton);
    buttonLayout->addLayout(actionLayout);
    buttonLayout->addStretch();

    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(infoLabel);
    mainLayout->addWidget(mapList);
    mainLayout->addWidget(buttonContainer);

    // 连接信号
    connect(newMapButton, &QPushButton::clicked, this, &MapSelect::onNewMapClicked);
    connect(startButton, &QPushButton::clicked, this, &MapSelect::onStartClicked);
    connect(renameButton, &QPushButton::clicked, this, &MapSelect::onRenameClicked);
    connect(editButton, &QPushButton::clicked, this, &MapSelect::onEditClicked);
    connect(deleteButton, &QPushButton::clicked, this, &MapSelect::onDeleteClicked);
    connect(cancelButton, &QPushButton::clicked, this, &MapSelect::onCancelClicked);
    connect(mapList, &QListWidget::itemClicked, this, &MapSelect::onItemClicked);

    // 初始化
    refreshMapList();
    showDefaultUI();  // 显示默认界面
}

MapSelect::~MapSelect()
{
}

void MapSelect::showDefaultUI()
{
    if (isPlayMode) {
        // 游戏模式：显示取消按钮
        newMapButton->setVisible(false);
        startButton->setVisible(false);
        renameButton->setVisible(false);
        editButton->setVisible(false);
        deleteButton->setVisible(false);
        cancelButton->setVisible(true);
        infoLabel->setText("点击选择要进入的地图：");
    } else {
        // 编辑模式：显示新建地图按钮
        newMapButton->setVisible(true);
        startButton->setVisible(false);
        renameButton->setVisible(false);
        editButton->setVisible(false);
        deleteButton->setVisible(false);
        cancelButton->setVisible(true);
        infoLabel->setText("点击新建地图或选择一个已有地图：");
    }
}

void MapSelect::showStartButton()
{
    // 游戏模式下选择地图后显示开始按钮
    newMapButton->setVisible(false);
    startButton->setVisible(true);
    renameButton->setVisible(false);
    editButton->setVisible(false);
    deleteButton->setVisible(false);
    cancelButton->setVisible(true);
    infoLabel->setText("已选择地图，点击开始进入游戏：");
}

void MapSelect::showActionButtons()
{
    // 编辑模式下有选中地图时显示操作按钮
    newMapButton->setVisible(false);
    startButton->setVisible(false);
    renameButton->setVisible(true);
    editButton->setVisible(true);
    deleteButton->setVisible(true);
    cancelButton->setVisible(true);
    infoLabel->setText("请选择要执行的操作：");
}

void MapSelect::setMode(bool playMode)
{
    isPlayMode = playMode;
    if (isPlayMode) {
        setWindowTitle("Mazeman - 选择地图");
        showDefaultUI();
    } else {
        setWindowTitle("Mazeman - 地图编辑器");
        showDefaultUI();
    }
}

void MapSelect::refreshMapList()
{
    mapList->clear();
    selectedMapName = "";

    QDir dir(QApplication::applicationDirPath());
    QStringList filters;
    filters << "*.maze";
    QStringList files = dir.entryList(filters, QDir::Files);

    if (files.isEmpty()) {
        QListWidgetItem *item = new QListWidgetItem("没有找到地图文件，请新建地图");
        item->setFlags(item->flags() & ~Qt::ItemIsSelectable);
        item->setForeground(Qt::gray);
        item->setTextAlignment(Qt::AlignCenter);
        mapList->addItem(item);
        newMapButton->setEnabled(true);
    } else {
        for (const QString &file : files) {
            QFileInfo info(file);
            QListWidgetItem *item = new QListWidgetItem(info.baseName());
            item->setForeground(Qt::black);
            item->setBackground(Qt::white);
            item->setData(Qt::UserRole, info.baseName());
            mapList->addItem(item);
        }
        newMapButton->setEnabled(true);
    }

    // 刷新后恢复默认UI
    showDefaultUI();
}

bool MapSelect::deleteMapFile(const QString &mapName)
{
    QDir dir(QApplication::applicationDirPath());
    QString filePath = dir.filePath(mapName + ".maze");

    QFile file(filePath);
    if (file.exists()) {
        return file.remove();
    }
    return false;
}

bool MapSelect::renameMapFile(const QString &oldName, const QString &newName)
{
    QDir dir(QApplication::applicationDirPath());
    QString oldPath = dir.filePath(oldName + ".maze");
    QString newPath = dir.filePath(newName + ".maze");

    // 检查新文件名是否已存在
    if (QFile::exists(newPath)) {
        QMessageBox::warning(this, "重命名失败",
                             QString("文件名 \"%1\" 已存在，请使用其他名称").arg(newName));
        return false;
    }

    QFile file(oldPath);
    if (file.exists()) {
        return file.rename(newPath);
    }
    return false;
}

void MapSelect::onItemClicked(QListWidgetItem *item)
{
    if (item && item->text() != "没有找到地图文件，请新建地图") {
        QString name = item->data(Qt::UserRole).toString();
        if (!name.isEmpty()) {
            selectedMapName = name.toStdString();

            if (isPlayMode) {
                // 游戏模式：选择地图后显示开始按钮
                showStartButton();
            } else {
                // 编辑模式：选择地图后显示操作按钮
                showActionButtons();
            }
        }
    }
}

void MapSelect::onNewMapClicked()
{
    emit newMapRequested();
}

void MapSelect::onStartClicked()
{
    if (selectedMapName.empty()) {
        QMessageBox::warning(this, "提示", "请先选择一个地图");
        return;
    }

    accept();  // 确认选择，开始游戏
}

void MapSelect::onRenameClicked()
{
    if (selectedMapName.empty()) {
        QMessageBox::warning(this, "提示", "请先选择一个地图");
        return;
    }

    QString oldName = QString::fromStdString(selectedMapName);
    bool ok;
    QString newName = QInputDialog::getText(this, "重命名地图",
                                            "请输入新名称:", QLineEdit::Normal, oldName, &ok);

    if (ok && !newName.isEmpty() && newName != oldName) {
        if (renameMapFile(oldName, newName)) {
            QMessageBox::information(this, "重命名成功",
                                     QString("地图 \"%1\" 已重命名为 \"%2\"")
                                         .arg(oldName).arg(newName));
            refreshMapList();
        } else {
            QMessageBox::critical(this, "重命名失败",
                                  QString("无法重命名地图 \"%1\"").arg(oldName));
        }
    }
}

void MapSelect::onEditClicked()
{
    if (selectedMapName.empty()) {
        QMessageBox::warning(this, "提示", "请先选择一个要编辑的地图");
        return;
    }

    done(2);  // 使用2表示编辑模式
}

void MapSelect::onDeleteClicked()
{
    if (selectedMapName.empty()) {
        QMessageBox::warning(this, "提示", "请先选择一个要删除的地图");
        return;
    }

    QString mapName = QString::fromStdString(selectedMapName);

    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "确认删除",
        QString("确定要删除地图 \"%1\" 吗？\n此操作不可撤销！").arg(mapName),
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No
        );

    if (reply == QMessageBox::Yes) {
        if (deleteMapFile(mapName)) {
            QMessageBox::information(this, "删除成功",
                                     QString("地图 \"%1\" 已成功删除").arg(mapName));
            refreshMapList();
        } else {
            QMessageBox::critical(this, "删除失败",
                                  QString("无法删除地图 \"%1\"，文件可能被占用").arg(mapName));
        }
    }
}

void MapSelect::onCancelClicked()
{
    reject();
}

std::string MapSelect::selectedMap() const
{
    return selectedMapName;
}
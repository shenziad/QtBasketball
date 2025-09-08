#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "avatargenerator.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget window;
    QVBoxLayout layout(&window);

    // 测试不同的头像风格
    QStringList testNames = {"勒布朗·詹姆斯", "斯蒂芬·库里", "科比·布莱恩特", "迈克尔·乔丹"};
    
    for (const QString& name : testNames) {
        QLabel* nameLabel = new QLabel(name);
        layout.addWidget(nameLabel);
        
        // 首字母头像
        QLabel* initialsLabel = new QLabel();
        QPixmap initialsAvatar = AvatarGenerator::generateAvatar(name, 64, AvatarGenerator::Initials);
        initialsLabel->setPixmap(initialsAvatar);
        initialsLabel->setFixedSize(64, 64);
        layout.addWidget(initialsLabel);
        
        // 几何图案头像
        QLabel* geometricLabel = new QLabel();
        QPixmap geometricAvatar = AvatarGenerator::generateAvatar(name, 64, AvatarGenerator::GeometricPattern);
        geometricLabel->setPixmap(geometricAvatar);
        geometricLabel->setFixedSize(64, 64);
        layout.addWidget(geometricLabel);
        
        // 色块头像
        QLabel* blocksLabel = new QLabel();
        QPixmap blocksAvatar = AvatarGenerator::generateAvatar(name, 64, AvatarGenerator::ColorBlocks);
        blocksLabel->setPixmap(blocksAvatar);
        blocksLabel->setFixedSize(64, 64);
        layout.addWidget(blocksLabel);
    }

    window.show();
    return app.exec();
}

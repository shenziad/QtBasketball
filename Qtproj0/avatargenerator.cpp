#include "avatargenerator.h"
#include <QApplication>
#include <QFont>
#include <QFontMetrics>
#include <QRandomGenerator>
#include <QPainter>
#include <QCryptographicHash>
#include <QPolygon>
#include <QLinearGradient>
#include <cmath>

QPixmap AvatarGenerator::generateAvatar(const QString& name, int size, AvatarStyle style)
{
    if (name.isEmpty()) {
        // 返回默认头像
        QPixmap pixmap(size, size);
        pixmap.fill(QColor(200, 200, 200));
        return pixmap;
    }

    switch (style) {
        case GeometricPattern:
            return generateGeometricAvatar(name, size);
        case ColorBlocks:
            return generateColorBlocksAvatar(name, size);
        case Initials:
        default:
            return generateInitialsAvatar(name, size);
    }
}

QPixmap AvatarGenerator::generateInitialsAvatar(const QString& name, int size)
{
    QPixmap pixmap(size, size);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);

    // 生成背景色
    uint32_t hash = generateHash(name);
    QColor backgroundColor = generateColor(hash);
    QColor textColor = getContrastColor(backgroundColor);

    // 绘制圆形背景
    painter.setBrush(backgroundColor);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(0, 0, size, size);

    // 设置字体
    QFont font = QApplication::font();
    font.setPointSize(size / 3);
    font.setBold(true);
    painter.setFont(font);
    painter.setPen(textColor);

    // 获取首字母并绘制
    QString initials = getInitials(name);
    QFontMetrics metrics(font);
    QRect textRect = metrics.boundingRect(initials);
    
    int x = (size - textRect.width()) / 2;
    int y = (size + textRect.height()) / 2 - metrics.descent();
    
    painter.drawText(x, y, initials);

    return pixmap;
}

QPixmap AvatarGenerator::generateGeometricAvatar(const QString& name, int size)
{
    QPixmap pixmap(size, size);
    pixmap.fill(Qt::white);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);

    uint32_t hash = generateHash(name);
    
    // 生成基础配色
    QColor color1 = generateColor(hash, 0);
    QColor color2 = generateColor(hash, 1);
    QColor color3 = generateColor(hash, 2);

    // 绘制背景渐变
    QLinearGradient gradient(0, 0, size, size);
    gradient.setColorAt(0, color1);
    gradient.setColorAt(1, color2);
    painter.fillRect(0, 0, size, size, gradient);

    // 根据哈希值绘制几何图案
    painter.setOpacity(0.7);
    
    int pattern = hash % 4;
    switch (pattern) {
        case 0: // 圆形图案
            painter.setBrush(color3);
            painter.setPen(Qt::NoPen);
            for (int i = 0; i < 3; i++) {
                int x = (hash >> (i * 4)) % (size - size/4);
                int y = (hash >> (i * 4 + 8)) % (size - size/4);
                int radius = size/8 + (hash >> (i * 4 + 16)) % (size/8);
                painter.drawEllipse(x, y, radius, radius);
            }
            break;
            
        case 1: // 矩形图案
            painter.setBrush(color3);
            painter.setPen(Qt::NoPen);
            for (int i = 0; i < 4; i++) {
                int x = (hash >> (i * 3)) % (size - size/3);
                int y = (hash >> (i * 3 + 12)) % (size - size/3);
                int w = size/6 + (hash >> (i * 3 + 18)) % (size/6);
                int h = size/6 + (hash >> (i * 3 + 21)) % (size/6);
                painter.drawRect(x, y, w, h);
            }
            break;
            
        case 2: // 三角形图案
            painter.setBrush(color3);
            painter.setPen(Qt::NoPen);
            for (int i = 0; i < 3; i++) {
                QPolygon triangle;
                int centerX = (hash >> (i * 4)) % size;
                int centerY = (hash >> (i * 4 + 8)) % size;
                int radius = size/8 + (hash >> (i * 4 + 16)) % (size/8);
                
                triangle << QPoint(centerX, centerY - radius)
                        << QPoint(centerX - radius, centerY + radius/2)
                        << QPoint(centerX + radius, centerY + radius/2);
                painter.drawPolygon(triangle);
            }
            break;
            
        case 3: // 线条图案
            painter.setPen(QPen(color3, 3));
            for (int i = 0; i < 5; i++) {
                int x1 = (hash >> (i * 3)) % size;
                int y1 = (hash >> (i * 3 + 6)) % size;
                int x2 = (hash >> (i * 3 + 12)) % size;
                int y2 = (hash >> (i * 3 + 18)) % size;
                painter.drawLine(x1, y1, x2, y2);
            }
            break;
    }

    return pixmap;
}

QPixmap AvatarGenerator::generateColorBlocksAvatar(const QString& name, int size)
{
    QPixmap pixmap(size, size);
    pixmap.fill(Qt::white);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);

    uint32_t hash = generateHash(name);
    
    // 创建5x5的网格色块
    int blockSize = size / 5;
    
    for (int row = 0; row < 5; row++) {
        for (int col = 0; col < 5; col++) {
            // 创建对称图案（左右对称）
            int actualCol = col < 2 ? col : (4 - col);
            
            // 根据位置和哈希值决定是否绘制色块
            uint32_t blockHash = hash ^ ((row << 8) | actualCol);
            if (blockHash % 3 == 0) {  // 约1/3的概率绘制色块
                QColor blockColor = generateColor(hash, row + actualCol);
                painter.fillRect(col * blockSize, row * blockSize, 
                               blockSize, blockSize, blockColor);
            }
        }
    }

    return pixmap;
}

uint32_t AvatarGenerator::generateHash(const QString& name)
{
    QByteArray data = name.toUtf8();
    QByteArray hash = QCryptographicHash::hash(data, QCryptographicHash::Md5);
    
    // 取前4字节作为uint32
    uint32_t result = 0;
    for (int i = 0; i < 4 && i < hash.size(); i++) {
        result = (result << 8) | (unsigned char)hash[i];
    }
    return result;
}

QColor AvatarGenerator::generateColor(uint32_t hash, int index)
{
    // 使用HSV颜色空间生成更和谐的颜色
    hash = hash ^ (index * 0x9e3779b9);  // 混合索引
    
    int hue = hash % 360;                    // 色相：0-359
    int saturation = 60 + (hash >> 8) % 40;  // 饱和度：60-100
    int value = 70 + (hash >> 16) % 30;      // 明度：70-100
    
    return QColor::fromHsv(hue, 
                          (saturation * 255) / 100, 
                          (value * 255) / 100);
}

QColor AvatarGenerator::getContrastColor(const QColor& backgroundColor)
{
    // 计算亮度
    double luminance = (0.299 * backgroundColor.red() + 
                       0.587 * backgroundColor.green() + 
                       0.114 * backgroundColor.blue()) / 255.0;
    
    // 根据背景亮度选择白色或黑色文字
    return luminance > 0.5 ? QColor(Qt::black) : QColor(Qt::white);
}

QString AvatarGenerator::getInitials(const QString& name)
{
    if (name.isEmpty()) {
        return "?";
    }

    QStringList words = name.split(' ', Qt::SkipEmptyParts);
    QString initials;
    
    if (words.size() >= 2) {
        // 取前两个词的首字母
        initials = QString(words[0].at(0).toUpper()) + QString(words[1].at(0).toUpper());
    } else if (words.size() == 1) {
        QString word = words[0];
        if (word.length() >= 2) {
            // 取前两个字符
            initials = word.mid(0, 2).toUpper();
        } else {
            // 只有一个字符
            initials = QString(word.at(0).toUpper());
        }
    } else {
        initials = "?";
    }
    
    return initials;
}

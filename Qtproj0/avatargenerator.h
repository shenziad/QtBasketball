#ifndef AVATARGENERATOR_H
#define AVATARGENERATOR_H

#include <QPixmap>
#include <QString>
#include <QColor>
#include <QPainter>
#include <QCryptographicHash>

class AvatarGenerator
{
public:
    // 头像样式枚举
    enum AvatarStyle {
        GeometricPattern,    // 几何图案样式
        ColorBlocks,        // 色块样式
        Initials           // 首字母样式
    };

    // 生成头像
    static QPixmap generateAvatar(const QString& name, 
                                 int size = 64, 
                                 AvatarStyle style = GeometricPattern);

    // 生成首字母头像
    static QPixmap generateInitialsAvatar(const QString& name, int size = 64);
    
    // 生成几何图案头像
    static QPixmap generateGeometricAvatar(const QString& name, int size = 64);
    
    // 生成色块头像
    static QPixmap generateColorBlocksAvatar(const QString& name, int size = 64);

private:
    // 根据名字生成哈希值
    static uint32_t generateHash(const QString& name);
    
    // 根据哈希值生成颜色
    static QColor generateColor(uint32_t hash, int index = 0);
    
    // 生成对比色（用于文字）
    static QColor getContrastColor(const QColor& backgroundColor);
    
    // 获取姓名的首字母
    static QString getInitials(const QString& name);
};

#endif // AVATARGENERATOR_H

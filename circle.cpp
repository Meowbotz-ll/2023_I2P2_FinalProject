#include "Circle.h"

// 判斷兩個圓是否重疊的函數
bool Circle::isOverlap(const Circle *circle_1, const Circle *circle_2)
{
    // 計算兩圓半徑和
    int length = circle_1->r + circle_2->r;

    // 計算兩圓中心的距離的平方
    int distance_square = (circle_1->x - circle_2->x) * (circle_1->x - circle_2->x)
                            + (circle_1->y - circle_2->y) * (circle_1->y - circle_2->y);

    // 判斷是否重疊
    if(length * length >= distance_square)
        return true;

    return false;
}

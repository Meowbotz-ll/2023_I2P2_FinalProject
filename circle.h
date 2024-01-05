#ifndef RECT_H_INCLUDED
#define RECT_H_INCLUDED

class Circle {
public:
    int x, y, r;

    // 構造函數
    Circle(int x, int y, int r) : x(x), y(y), r(r) {}

    // 將參數更改為常量指針
    static bool isOverlap(const Circle *circle_1, const Circle *circle_2);
};



#endif // RECT_H_INCLUDED

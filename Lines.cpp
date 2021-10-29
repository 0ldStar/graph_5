#include <QPainter>
#include <iostream>
#include <QTimer>
#include <QKeyEvent>
#include <QTest>
#include <windows.h>
#include "Lines.h"


Lines::Lines(int width, int height, int count, vector<vector<int>> &arr, vector<vector<int>> &arr2,
             vector<vector<int>> &map)
        : size{0, 0, 0, 0, width, height, count}, points(arr), points2(arr2), map(map) {
    int arrSize = (int) points.size();
    this->count = findMin();
    map.resize(count + 1);
    for (int i = 0; i <= count; ++i) map[i].resize(count + 1);

    /*for (int i = 0; i < arrSize; ++i) {
        for (int j = 0; j < arrSize; ++j) {
            points[i][j] = arr[i][j];
        }
    }*/
    resize(width, height);

}

void Lines::keyPressEvent(QKeyEvent *event) {

    if (event->key() == Qt::Key_Space) {
        count++;
        if (count > findMax()) count = findMin();
        update();
    }
}

void Lines::paintEvent(QPaintEvent *e) {
    Q_UNUSED(e);

    QPainter painter(this);
    drawPolygon(painter, points, 1);
    drawMap(painter);
    drawPolygon(painter, points2, 2);
    drawGrid(painter);
    drawAxes(painter);
}

void Lines::drawPolygon(QPainter &painter, vector<vector<int>> &arr, int PColor) {
    QColor color(0, 0, 0);
    painter.setBrush(QBrush(color));
    painter.setPen(color);
    for (int i = 0; i < arr.size() - 1; ++i) {
        drawCDA(arr[i][0], arr[i + 1][0], arr[i][1], arr[i + 1][1], painter, PColor);
    }
    drawCDA(arr[0][0], arr[arr.size() - 1][0], arr[0][1], arr[arr.size() - 1][1], painter, PColor);
}

void Lines::drawMap(QPainter &painter) {

    fill();
    for (int i = 0; i < size.count; ++i) {
        for (int j = 0; j < size.count; ++j) {
            if (map[i][j] == 0) continue;
            switch (map[i][j]) {
                case 3:
                    painter.setBrush(QBrush(Qt::magenta));
                    painter.setPen(Qt::magenta);
                    break;
                case 4:
                    painter.setBrush(QBrush(Qt::yellow));
                    painter.setPen(Qt::yellow);
                    break;
                case 5:
                    painter.setBrush(QBrush(Qt::red));
                    painter.setPen(Qt::red);
                    break;
                default:
                    painter.setBrush(QBrush(Qt::black));
                    painter.setPen(Qt::black);
                    break;
            }
            drawMediumPixel(i, j, painter);
        }

    }
    mapClean();
}

int Lines::findMin() {
    int min = points[0][0];
    int arrSize = (int) points.size();
    for (int i = 0; i < arrSize; ++i) {
        if (points[i][0] < min) min = points[i][0];
        if (points2[i][0] < min) min = points2[i][0];
    }
    return min + size.count / 2;
}

int Lines::findMax() {
    int max = points[0][0];
    int arrSize = (int) points.size();
    for (int i = 0; i < arrSize; ++i) {
        if (points[i][0] > max) max = points[i][0];
        if (points2[i][0] > max) max = points2[i][0];
    }
    return max + size.count / 2;
}

void Lines::fill() {
    int point;
    bool flag;
    for (int i = 0; i <= count; ++i) {
        flag = false;
        for (int j = 0; j < size.count; ++j) {
            if (map[i][j] == 1) {
                if (map[i][j - 1] == 1) {
                    point = j + 1;
                } else {
                    if (flag) {
                        if (map[i - 1][point] != 0) {
                            for (int k = point; k < j; ++k) {
                                if (map[i][k] == 0)map[i][k] = 3;
                            }
                        } else {
                            point = j + 1;
                        }
                    } else {
                        point = j + 1;
                        flag = !flag;
                    }
                }
            }

        }
    }
    for (int i = 0; i <= count; ++i) {
        flag = false;
        for (int j = 0; j < size.count; ++j) {
            if (map[i][j] == 2) {
                if (map[i][j - 1] == 2) {
                    point = j + 1;
                } else {
                    if (flag) {
                        if (map[i - 1][point] != 0) {
                            for (int k = point; k < j; ++k) {
                                if (map[i][k] == 3) map[i][k] = 5;
                                else if (map[i][k] == 0) map[i][k] = 4;
                            }
                        } else {
                            point = j + 1;
                        }
                    } else {
                        point = j + 1;
                        flag = !flag;
                    }
                }
            }

        }
    }
}


void Lines::mapClean() {
    for (int i = 0; i <= size.count; ++i) {
        for (int j = 0; j <= size.count; ++j) {
            map[i][j] = 0;
        }
    }
}

void Lines::drawGrid(QPainter &painter) const {
    painter.setPen(Qt::black);
    painter.setPen(Qt::DotLine);

    //int bigPixelCount = size.count / points.size();
    for (int i = 0; i < size.count; ++i) {
        painter.drawLine((int) ((float) i / (float) size.count * (float) size.width), 0,
                         (int) ((float) i / (float) size.count * (float) size.width),
                         size.height);
    }
    for (int i = 0; i < size.count; ++i) {
        painter.drawLine(0, (int) ((float) i / (float) size.count * (float) size.height), size.width,
                         (int) ((float) i / (float) size.count * (float) size.height));
    }
}

void Lines::drawAxes(QPainter &painter) const {
    QPen pen;
    pen.setColor("red");
    pen.setWidth(3);
    painter.setPen(pen);
    int dx = (int) ((float) size.width / 2);
    int dy = (int) ((float) size.height / 2);

    painter.drawLine(dx, 0, dx, size.height);
    painter.drawLine(0, dy, size.width, dy);
}

int sign(double x) {
    if (x > 0) return 1;
    else if (x == 0) return 0;
    else
        return -1;
}

void Lines::drawMediumPixel(int x, int y, QPainter &painter) {
    double dx = (float) size.width / (float) size.count;
    double dy = (float) size.height / (float) size.count;
    painter.drawRect(x * dx, y * dy - dy, dx, dy);
}

void Lines::drawCDA(int X1, int X2, int Y1, int Y2, QPainter &painter, int PColor) {
    double dx, dy, l, x, y;
    int _x, _y;
    if (abs(X2 - X1) >= abs(Y2 - Y1)) l = abs(X2 - X1);
    else {
        l = abs(Y2 - Y1);
    }
    dx = (X2 - X1) / l;
    dy = (Y2 - Y1) / l;

    x = X1 + 0.5 * sign(dx);
    y = Y1 + 0.5 * sign(dy);
    if (l == 0) return;
    for (int i = 0; i <= l; ++i) {
        decardToDigital(floor(x), floor(y), _x, _y);
        map[_x][_y] = PColor;
        x = x + dx;
        y = y + dy;
    }
}

void Lines::decardToDigital(int x, int y, int &X, int &Y) const {
    X = (size.count / 2 + x);
    Y = (size.count / 2 - y);
}


#include <QApplication>
#include <QOpenGLWidget>
#include <QTranslator>
#include <vector>
#include "Lines.h"

#define WIDTH 600
#define HEIGHT 600
#define COUNT 60
using namespace std;

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    vector<vector<int>> arr{{18,   -19},
                            {5,   -10},
                            {0,   -7},
                            {-0, 15},
                            {-20, 20}};
    vector<vector<int>> arr2{{10,  20},
                             {10,  -20},
                             {-5,  10},
                             {0,   10},
                             {-10, -20},
                             {-10, 20}};
    vector<vector<int>> map;
    Lines window(WIDTH, HEIGHT, COUNT, arr, arr2, map);
    window.setWindowTitle("Lines");
    window.show();
    return a.exec();
}

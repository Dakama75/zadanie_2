#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <math.h>

using namespace std;
// TIP To <b>Run</b> code, press <shortcut actionId="Run"/> or click the <icon src="AllIcons.Actions.Execute"/> icon in the gutter.

// /**
//  * Odczytuje
//  */
// vector<vector<double>> getPoints(string filePath) {
//
//     int numOfPoints; // Przechowuje liczbe punktów
//     double x, y; // zmienne przechowujące x i y poszczególych punktów
//     vector<vector<double>> points;  // 2 wymiarowy vector, przechowujący współrzędne x i y punktów
//
//     // Read from the text file
//     ifstream file(filePath);
//
//     if (!file.is_open()) {
//         cout << "Nie można utworzyć pliku, upewnij się czy jest w tym samum folderze co .exe";
//         return vector<vector<double>>();
//     }
//
//     file >> numOfPoints;
//     cout << "Number of Points: " << numOfPoints << endl;
//
//
//     while (file >> x >> y) {
//         points.push_back({x,y});
//     }
//
//     // Close the file
//     file.close();
//     return points;
// }


struct Point {
    double x,y;
};


vector<Point> zadanie1Results; // zmienna globalna, przechowuje wynik zadania 1, wykorzystywana w zadaniu 2


/**
 * >0 - lewoskręt;
 * <0 - prawoskręt;
 * =0 - wspólniowe;
 * @param a
 * @param b
 * @param c
 * @return Orinetację trzech punktów
 */
double orientation(const Point& a, const Point& b, const Point& c) {
    return (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
}

/**
 *
 * @param a
 * @param b
 * @return Kwadrat odległośći miedzy punktami
 */
double distanceSquared(const Point& a, const Point& b) {
    return (a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y);
}
/**
 *
 * @param a
 * @param b
 * @return Odległość miedzy punktami
 */
double distance(const Point& a, const Point& b) {
    return sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
}

const double EPS = 1e-9; // stała do pracy na wartościach zmiennoprzecinkowych

bool compareX(const Point& p1, const Point& p2) { return p1.x < p2.x; }
bool compareY(const Point& p1, const Point& p2) { return p1.y < p2.y; }

/**
 * Funkcja z zadaniem 1 o otoczce
 * @param points
 * @param numOfPoints
 * @return
 */
int zadanie1(vector<Point> points, int numOfPoints) {
    int p0 = 0; // zmienna przechowująca punkt z najmnijeszym y

    // Znalezienie punktu o najmnijeszym y, czyli punktu starowego (jeśli jest kilka z tym samym y, to wybierany jest ten z mniejszym x)
    for (int i = 0; i < numOfPoints; i++) {
        if (points[i].y < points[p0].y - EPS ||(fabs(points[i].y - points[p0].y) < EPS && points[i].x < points[p0].x - EPS)) {
            p0 = i;
        }
    }
    swap(points[0], points[p0]);
    Point startPoint = points[0]; // zmienna przechowująca punkt startowy


    // sortowanie po kątcie polarnym
    sort(points.begin() + 1, points.end(), [&](Point a, Point b) {
        double o = orientation(startPoint, a, b);
        if (fabs(o) < EPS) {
            // jeśli wspólniowe, to sortowanie od startpoin
            return distanceSquared(startPoint, a) < distanceSquared(startPoint, b);
        }
        return o > 0;
    });

    vector<Point> stack; //stos przechowujący punkty otoczki
    stack.push_back(points[0]);
    stack.push_back(points[1]);

    for (int i = 2; i < numOfPoints; i++) {
        // dopuki punkty tworza prawoskret, to usuwany jest srodkowy
        while (stack.size() >= 2 && orientation(stack[stack.size()-2], stack.back(), points[i]) < 0) {
            stack.pop_back();
        }
        stack.push_back(points[i]);
    }

    // wypisanie wyniku
    cout << "Otoczka: ";
    for (int i = 0; i < stack.size(); i++) {
        cout << "(" << stack[i].x << ", " << stack[i].y << "), ";
    }

    zadanie1Results = stack;

   return 0;

}

/**
 * Funkcja obliczajaca odległość punktu od prostej przechodząca przez punkty a i b
 * @param p
 * @param a
 * @param b
 * @return
 */
double distanceToLine(const Point& p, const Point& a, const Point& b) {
    double distanceX = b.x - a.x;
    double distanceY = b.y - a.y;
    return fabs(distanceY*p.x - distanceX*p.y + b.x*a.y - b.y*a.x) / sqrt(distanceX*distanceX + distanceY*distanceY);
}


/**
 * Funkcja z zadaniem 2 o prostych
 * */
double zadanie2(vector<Point> points, int numberOfPoints) {
    if (numberOfPoints < 2) {
        return 0.0;
    }

    double bestWidth = numeric_limits<double>::max();

    for (int i = 0; i < numberOfPoints; i++) {
        Point a = points[i];
        Point b = points[(i+1)%numberOfPoints];

        double maxDist = 0.0;

        // sprawdza odległość punktów od prostych
        for (int j = 0; j < numberOfPoints; j++) {
            double distance = distanceToLine(points[j], a, b);
            if (distance > maxDist) {
                maxDist = distance;
            }
        }

        // zachowanie najmniejszej odległości
        if (maxDist < bestWidth) {
            bestWidth = maxDist;
        }
    }
    cout << "Proste: " << bestWidth << endl;
    return bestWidth;
}



int main() {

    string fileName;
    int numOfPoints;
    cout << "Podaj nazwe pliku: ";
    cin >> fileName;

    if (fileName == "'") fileName = "dane.txt";

    double x, y; // zmienne przechowujące x i y poszczególych punktów
    vector<Point> points;  // 2 wymiarowy vector, przechowujący współrzędne x i y punktów

    // Read from the text file
    ifstream file(fileName);

    if (!file.is_open()) {
        cout << "Nie mozna otworzyć pliku, upewnij sie czy jest w tym samym folderze co .exe";
    }

    file >> numOfPoints;


    while (file >> x >> y) {
        points.push_back({x,y});
    }

    //zamknięcie pliku
    file.close();


    zadanie1(points, numOfPoints);

    cout << endl;


    zadanie2(zadanie1Results, zadanie1Results.size());

    return 0;
    // TIP See CLion help at <a href="https://www.jetbrains.com/help/clion/">jetbrains.com/help/clion/</a>. Also, you can try interactive lessons for CLion by selecting 'Help | Learn IDE Features' from the main menu.
}
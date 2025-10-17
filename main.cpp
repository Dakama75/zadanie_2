#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <math.h>

using namespace std;


struct Point {
    double x,y;
};


vector<Point> zadanie1Results; // zmienna globalna, przechowuje wynik zadania 1, wykorzystywana w zadaniu 2
Point minPointA, minPointB;

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

/**
 * Najprostszy sposób na rozwiązanie tego zadania, nawypadek gdyby ta za pomocą divide & conquer by nie działała
 * @param points
 * @param numOfPoints
 * @return
 */
// int zadanie3(vector<Point> points, int numOfPoints) {
//     double minDist = distance(points[0], points[1]);
//     for (int i = 0; i < numOfPoints; i++) {
//         for (int j = i+1; j < numOfPoints; j++) {
//             if (distance(points[i], points[j]) < minDist) {
//                 minDist = distance(points[i], points[j]);
//                 minPointA = points[i];
//                 minPointB = points[j];
//             }
//         }
//     }
//
//     cout << "Najbizsze punkty: (" << minPointA.x << ", " << minPointA.y << "), (" << minPointB.x << ", " << minPointB.y << ") d = " << minDist<< endl;
//     return minDist;
// }


/**
 * zadanie 3 przerobione tak by wykorzystać je w nowym, wydajniejszym rozwiązaniu, zwykłe brutforce znalezienie najmniejszej odległości miezy punktami
 * @param points
 * @param left
 * @param right
 * @return najmniejszą odległość
 */
double zadanie3(const vector<Point>& points, int left, int right) {
    double minDist = distance(points[0], points[1]);
    for (int i = left; i < right; i++) {
        for (int j = i + 1; j <= right; j++) {
            if (distance(points[i], points[j]) < minDist) {

            minDist = min(minDist, distance(points[i], points[j]));
            minPointA = points[i];
            minPointB = points[j];
            }
        }
    }
    return minDist;
}

/**
 * sprawda czy najblizszy punkt nie lezy w pasie podziału zbioru na dwa mniejsze
 * @param points lista punktow, ktore leża w odległości d od lini podziału podziału zbioru
 * @param d
 * @return
 */
double splittedClosestsPoints(vector<Point>& points, double d) {
    sort(points.begin(), points.end(), [](const Point& a, const Point& b) {
        return a.y < b.y;
    });

    double minDist = d;
    for (int i = 0; i < (int)points.size(); ++i) {
        for (int j = i + 1; j < (int)points.size() && (points[j].y - points[i].y) < minDist; ++j) {
            minDist = min(minDist, distance(points[i], points[j]));
        }
    }

    return minDist;
}


/**
 * Rekurencyjna funkcja, która dzieli punkty na dwa mnijesze zbiory i w nich szuka najbliższych punktów
 * @param points
 * @param left
 * @param right
 * @return
 */
double closestRecu(vector<Point>& points, int left, int right) {
    if (right - left <= 3) {
        return zadanie3(points, left, right);
    }
    int mid = (left + right) / 2;
    Point midPoint = points[mid];

    double distanceLeft = closestRecu(points, left, mid);
    double distanceRight = closestRecu(points, mid + 1, right);
    double d = min(distanceLeft, distanceRight); // najmniejsza odległośc miedzy punkatami z lewej i sprawej, wykorzystywana do sprawdzenia czy na lini podziału nie ma bliższych punktów

    // Vector przechowujący punkty w odległości d od lini podziału
    vector<Point> pointsInTheCenter;
    for (int i = left; i <= right; i++) {
        if (fabs(points[i].x - midPoint.x) < d)
            pointsInTheCenter.push_back(points[i]);
    }

    return min(d, splittedClosestsPoints(pointsInTheCenter, d));
}

/**
 * sortuje punkty po x i wywołuje rekurencyjną funkcję closestRecu
 * @param points
 * @param numOfPoints
 * @return najmnijesza odległość między punktami
 */
double zadanie3efficient(vector<Point> points, int numOfPoints) {
    sort(points.begin(), points.end(), [](const Point &a, const Point &b) {
        return a.x < b.x;
    });
    return closestRecu(points, 0, numOfPoints - 1);
}

int main() {

    string fileName;
    int numOfPoints;
    cout << "Podaj nazwe pliku: ";
    cin >> fileName;

    if (fileName == "'") fileName = "dane.txt"; // nie chciało mi się za każdym razem wpisywać "dane.txt" więc dla przyśpieszenia debbugowania stworzyłem tego if

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

    // zadanie3(points, numOfPoints);

    // cout << endl;
    //
    // zadanie3efficient(points, numOfPoints);

    double minDist = zadanie3efficient(points, numOfPoints);
    cout << "Najblizsze punkty: (" << minPointA.x << ", " << minPointA.y << ") i (" << minPointB.x << ", " << minPointB.y << ") d = " << minDist << endl;



    return 0;
}
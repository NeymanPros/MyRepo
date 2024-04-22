#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <array>
#include <vector>
#include <iostream>
#include <fstream>
#include <thread>
#include <cmath>
using namespace cv;

//making the image black and white only
void WaB(Mat &c, const int &contrast) {   
    for (int i = 0; i < c.rows - 1; i++) {
        for (int j = 0; j < c.cols - 1; j++) {
            if (c.at<Vec3b>(i, j)[0] > contrast || c.at<Vec3b>(i, j)[1] > contrast || c.at<Vec3b>(i, j)[2] > contrast) {
                c.at<Vec3b>(i, j) = { 255, 255, 255 };
            }
            else {
                c.at<Vec3b>(i, j) = { 0, 0, 0 };
            }

        }
    }
}

//contours
void neib(Mat &c, Mat &nc, int i, int j) { 
    if ((c.at<Vec3b>(i, j) != c.at<Vec3b>(i - 1, j))     || (c.at<Vec3b>(i, j) != c.at<Vec3b>(i + 1, j)) ||
        (c.at<Vec3b>(i, j) != c.at<Vec3b>(i, j - 1))     || (c.at<Vec3b>(i, j) != c.at<Vec3b>(i, j + 1)) ||
        (c.at<Vec3b>(i, j) != c.at<Vec3b>(i - 1, j - 1)) || (c.at<Vec3b>(i, j) != c.at<Vec3b>(i - 1, j + 1)) ||
        (c.at<Vec3b>(i, j) != c.at<Vec3b>(i + 1, j - 1)) || (c.at<Vec3b>(i, j) != c.at<Vec3b>(i + 1, j + 1))) {

            nc.at<Vec3b>(i, j) = { c.at<Vec3b>(i, j)[0], c.at<Vec3b>(i, j)[1], c.at<Vec3b>(i, j)[2] };
    }
}


//extracting lines from contur
void line(Mat& nc, Mat& c, int& i, int& j, std::vector<int>& dots, unsigned char picture, unsigned char condition) { 
    dots.clear();
    int a = i;
    int b = j;
    int run = 0;
    nc.at<Vec3b>(i, j) = { picture, picture, picture };

    dots.push_back(i);
    dots.push_back(j);

    short count = 0;
    int a1 = a;
    int b1 = b;
    for (int now = 2; now > 0;) {

        if (a > 0 && a < c.rows && b > 0 && b < c.cols) {

            count = 0;

            if (nc.at<Vec3b>(a - 1, b)[1] == condition) {
                count += 1;
                a1 = a - 1;
                b1 = b;
            }
            if (nc.at<Vec3b>(a, b + 1)[1] == condition) {
                count += 1;
                a1 = a;
                b1 = b + 1;
            }
            if (nc.at<Vec3b>(a, b - 1)[1] == condition) {
                count += 1;
                a1 = a;
                b1 = b - 1;
            }
            if (nc.at<Vec3b>(a + 1, b)[1] == condition) {
                count += 1;
                a1 = a + 1;
                b1 = b;
            }


            if (count == 0) {
                if (dots[0] - dots[now - 2] <= 1 && dots[now - 2] - dots[0] <= 1 &&
                    dots[1] - dots[now - 1] <= 1 && dots[now - 1] - dots[1] <= 1){
                    dots.push_back(dots[0]);
                    dots.push_back(dots[1]);
                }
                now = -10;
            }

            else if (count == 1) {
                nc.at<Vec3b>(a, b) = { picture,picture,picture };
                dots.push_back(a1);
                dots.push_back(b1);
                now += 2;
                run = 0;

                a = a1;
                b = b1;
            }

            else if (count == 2) {
                
                if (run > 4) {
                    nc.at<Vec3b>(a, b) = { picture,picture,picture };
                }
                else {
                    run += 1;
                }

                dots.push_back(a1);
                dots.push_back(b1);
                now += 2;

                a = a1;
                b = b1;
            }

            else if (count > 2) {
                

                dots.push_back(a1);
                dots.push_back(b1);
                now += 2;
                
                a = a1;
                b = b1;
            }
        }
        else {
            now = -10;
        }
    }
}

//svg part
std::string start(int width, int height) {
        return "<svg width=\"" + std::to_string(width) + "\" height=\"" + std::to_string(height) + "\" xmlns=\"http://www.w3.org/2000/svg\">\n";
    };


class rect {
protected:
    std::string width = "0";
    std::string height = "0";
    int x = 0;
    int y = 0;
public:
    rect() {};
    rect(int a, int b) {
        x = a;
        y = b;
    }
    rect(std::string w, std::string h) {
        width = w;
        height = h;
        x = 0;
        y = 0;
    }
    rect(int a, int b, int c, int d) {
        x = a;
        y = b;
        width = std::to_string(c);
        height = std::to_string(d);
    }
    ~rect() = default;

    std::string write(int r, int g, int b) {
        return "<rect x=\"" + std::to_string(x) + "\" y=\"" + std::to_string(y) + "\" width=\"" + width + "\" height=\"" + 
            height + " fill=\"rgb(" + std::to_string(r) + " " + std::to_string(g) + " " + std::to_string(b) + ")\"/>\n";
    }
    std::string writeNoXY(int r, int g, int b) {
        return "<rect width=\"" + width + "\" height=\"" + height + 
            "\" fill=\"rgb(" + std::to_string(r) + " " + std::to_string(g) + " " + std::to_string(b) + ")\"/>\n";
    }
};


std::string end() {
    return "</svg>";
}


class path {
private:
    std::string stroke;
    std::vector<int> dotI;
    std::vector<int> dotII;
    std::array <std::string, 3> rgb = { "0", "0", "0" };
    std::string cont;
    std::vector<int> angles;
public:
    path() {};

    path(const std::vector<int>& dots) {
        if(dots.size() > 1){
            dotI = dots;
        }
    };

    ~path() {
        dotI.clear();
        dotII.clear();
        cont.clear();
        angles.clear();
    };

    void NewAngles() {
        angles.clear();

        for (int i = 2; i < dotII.size(); i++)
            angles.push_back(dotII[i] - dotII[i - 2]);
    }

    //no 3 dots in one raw/column
    void simple1() {
        dotII.clear();
        int raw = 0;
        int now = 0;
        
        if (dotI.size() < 2) {
            return;
        }

        dotII.push_back(dotI[0]);
        dotII.push_back(dotI[1]);
        now += 2;

        for (int i = 2; i < dotI.size() - 2; i += 2) {

            if(dotI[i] == dotII[now - 2] || dotI[i + 1] == dotII[now - 1]){
                ;
            }

            else if (dotI[i - 2] != dotI[i + 2] && dotI[i - 1] != dotI[i + 3]) {
                dotII.push_back(dotI[i]);
                dotII.push_back(dotI[i + 1]);
                now += 2;
            }
            else {
                dotII.push_back(dotI[i - 2]);
                dotII.push_back(dotI[i - 1]);
                now += 2;
            }

        }

        dotII.push_back(dotI[dotI.size() - 2]);
        dotII.push_back(dotI[dotI.size() - 1]);

        dotI.clear();

    }

    //no 3 dots one one line
    void simple2() {

        NewAngles();

        for (int i = 2; i < angles.size() - 2; i += 2) {
         
            if ((angles[i] == angles[i + 2] && angles[i] == angles[i - 2]) && (angles[i + 1] == angles[i - 1] && angles[i + 1] == angles[i + 3])) {

                angles.erase(angles.begin() + i, angles.begin() + i + 2);
                dotII.erase(dotII.begin() + i, dotII.begin() + i + 2);
                i -= 2;

            }

        }

        NewAngles();

    }

    //no random dots on curve
    void simple3() {

        for (int i = 2; i + 2 < angles.size(); i += 2) {

            if ((angles[i] * angles[i] + angles[i + 1] * angles[i + 1] <= 8)
                && (angles[i - 2] * angles[i - 2] + angles[i - 1] * angles[i - 1] <= 8)) {

                angles.erase(angles.begin() + i, angles.begin() + i + 2);
                dotII.erase(dotII.begin() + i, dotII.begin() + i + 2);
            }

        }

    }

    void simple() {
        this->simple1();
        this->simple2();
        this->simple3();
        this->simple3();
    }

    //whole image makes of lines
    /*std::string L(const Mat &c) {
        if (dotII.size() > 3) {

            cont.clear();

            cont = "M " + std::to_string(dotII[1]) + " " + std::to_string(dotII[0]) + "\n";

            for (int i = 2; i < dotII.size(); i += 2) {
                    cont += "L " + std::to_string(dotII[i + 1]) + " " + std::to_string(dotII[i]) + "\n";
            }

            dotII.clear();
            return "<path fill-opacity=\"0\" stroke=\"rgb(" + rgb[0] + " " + rgb[1] + " " + rgb[2] + ")\"\nd=\""
                + cont + "\"\n/>\n";
        }
        else {
            return "";
        }
    }*/


    std::string d(const Mat& c) {
        if (dotII.size() > 4) {

            cont.clear();

            NewAngles();
            char cw;
            int rx, ry;
            char line;

            cont = "M " + std::to_string(dotII[1]) + " " + std::to_string(dotII[0]) + "\n";

            for (int i = 2; i < dotII.size() - 2; i += 2) {

                line = true;
                if (angles[i - 1] * angles[i - 1] == 1) {

                    if (angles[i - 1] == angles[i + 1]) {

                        if (angles[i - 2] > angles[i] && (angles[i - 2] > 0) == (angles[i] > 0)) {
                            int k = i;

                            for (k = i; dotII.size() - 2 > k && angles[i - 1] == angles[k + 1] && (angles[i - 2] > 0) == (angles[k] > 0) && angles[k - 2] > angles[k]; k += 2);

                            if (abs(dotII[k] - dotII[i]) > 6) {
                                cw = (angles[i + 1] < 0) + '0';
                                rx = (dotII[k + 1] - dotII[i - 1]) / sin(atan((double)(0.01 + angles[k - 1]) / (0.01 + angles[k - 2])));
                                ry = (dotII[k] - dotII[i - 2]) / sin(atan((double)(0.01 + angles[k - 1]) / (0.01 + angles[k - 2])));
                                i = k;
                                line = false;
                                cont += "A " + std::to_string(rx) + ' ' + std::to_string(ry) + " 0 0 " + cw + ' ' + std::to_string(dotII[k + 1]) + ' ' + std::to_string(dotII[k]) + '\n';
                            }

                        }

                        else if (angles[i - 2] < angles[i] && (angles[i - 2] > 0) == (angles[i] > 0)) {
                            int k = i;

                            for (k = i; dotII.size() - 2 > k && angles[i - 1] == angles[k + 1] && (angles[i - 2] > 0) == (angles[k] > 0) && angles[k - 2] < angles[k]; k += 2);

                            if (abs(dotII[k] - dotII[i]) > 6) {
                                cw = (angles[i + 1] > 0) + '0';
                                rx = (dotII[k + 1] - dotII[i - 1]) / sin(atan((double)(0.01 + angles[k - 1]) / (0.01 + angles[k - 2])));
                                ry = (dotII[k] - dotII[i - 2]) / sin(atan((double)(0.01 + angles[k - 1]) / (0.01 + angles[k - 2])));
                                i = k;
                                line = false;
                                cont += "A " + std::to_string(rx) + ' ' + std::to_string(ry) + " 0 0 " + cw + ' ' + std::to_string(dotII[k + 1]) + ' ' + std::to_string(dotII[k]) + '\n';
                            }

                        }
                    }
                }   

                else if (angles[i - 2] * angles[i - 2] == 1) {

                    if (angles[i - 2] == angles[i]) {
                        if (angles[i - 1] > angles[i + 1] && (angles[i - 1] > 0) == (angles[i + 1] > 0)) {
                            int k = i;

                            for (k = i; dotII.size() - 2 > k && angles[i - 2] == angles[k] && (angles[i - 1] > 0) == (angles[k + 1] > 0) && angles[k - 1] > angles[k + 1]; k += 2);

                            if (abs(dotII[k + 1] - dotII[i + 1]) > 6) {
                                cw = (angles[i] > 0) + '0';
                                rx = (dotII[k + 1] - dotII[i - 1]) / cos(atan((double)(0.01 + angles[k - 1]) / (0.01 + angles[k - 2])));
                                ry = (dotII[k] - dotII[i - 2]) / cos(atan((double)(0.01 + angles[k - 1]) / (0.01 + angles[k - 2])));
                                i = k;
                                line = false;
                                cont += "A " + std::to_string(rx) + ' ' + std::to_string(ry) + " 0 0 " + cw + ' ' + std::to_string(dotII[k + 1]) + ' ' + std::to_string(dotII[k]) + '\n';
                            }

                        }

                        else if (angles[i - 1] < angles[i + 1] && (angles[i - 1] > 0) == (angles[i + 1] > 0)) {
                            int k = i;

                            for (k = i; dotII.size() - 2 > k && angles[i - 2] == angles[k] && (angles[i - 1] > 0) == (angles[k + 1] > 0) && angles[k - 1] < angles[k + 1]; k += 2);

                            if (abs(dotII[k + 1] - dotII[i + 1]) > 6) {
                                cw = (angles[i] < 0) + '0';
                                rx = (dotII[k + 1] - dotII[i - 1]) / cos(atan((double)(0.01 + angles[k - 1]) / (0.01 + angles[k - 2])));
                                ry = (dotII[k] - dotII[i - 2]) / cos(atan((double)(0.01 + angles[k - 1]) / (0.01 + angles[k - 2])));
                                i = k;
                                line = false;
                                cont += "A " + std::to_string(rx) + ' ' + std::to_string(ry) + " 0 0 " + cw + ' ' + std::to_string(dotII[k + 1]) + ' ' + std::to_string(dotII[k]) + '\n';
                            }

                        }
                    }

                }

                if (line)
                    cont += "L " + std::to_string(dotII[i + 1]) + " " + std::to_string(dotII[i]) + "\n";
            }

            cont += "L " + std::to_string(dotII[dotII.size() - 1]) + " " + std::to_string(dotII[dotII.size() - 2]) + "\n";

            dotII.clear();
            return "<path fill-opacity=\"0\" stroke=\"rgb(" + rgb[0] + " " + rgb[1] + " " + rgb[2] + ")\"\nd=\""
                + cont + "\"\n/>\n";
        }

        else if (dotII.size() == 4) {

            if (dotII[0] == dotII[2] && dotII[1] == dotII[3])
                return "";
    
            else {
                cont = "M " + std::to_string(dotII[1]) + " " + std::to_string(dotII[0]) + "\n";
                cont += "L " + std::to_string(dotII[3]) + " " + std::to_string(dotII[2]) + "\n";

                dotII.clear();
                return out();
            }

        }
        else {
            return "";
        }
    }


    std::string out() {
        return "<path fill-opacity=\"0\" stroke=\"rgb(" + rgb[0] + " " + rgb[1] + " " + rgb[2] + ")\"\nd=\""
            + cont + "\"\n/>\n";
    }
};



int main() {
    std::string name;
    std::cout << "\nEnter the path to the picture:\n";
    std::cin >> name;
    std::string result = name;
    Mat c = imread(name, IMREAD_COLOR);

    if (c.empty()) {
        std::cout << "Image wasn't found!\n";
        return 0;
    }

    WaB(c, 128);

    for (int i = result.length() - 1; result[i] != '.'; i--) {
        result.pop_back();
    }
    result += "svg";

    std::ofstream svg(result);

    Mat nc;

    resize(c, nc, Size(), 1, 1);
    
    nc.setTo(Scalar(255, 255, 255));

    for (int i = 1; i < c.rows - 2; i++) {
        for (int j = 1; j < c.cols - 2; j++) {
            neib(c, nc, i, j);
        }
    }

    imshow("Tap c to continue", nc);

    int z = waitKey(10000);

    if (z == '1') {
        imwrite(name, c);
    }
    if (z == 'w' || z == 'b') {

        c = imread(name, IMREAD_COLOR);
        if (z == 'w') {
            WaB(c, 64);
        }
        else if (z == 'b') {
            WaB(c, 192);
        }

        for (int i = 1; i < c.rows - 2; i++) {
            for (int j = 1; j < c.cols - 2; j++) {
                neib(c, nc, i, j);
            }
        }

        imshow("Tap c to continue", nc);
        int z1 = waitKey(10000);
        if (z1 == 'q') {
            return 0;
        }
    }

    if (z == 'q') {
        return 0;
    }

    std::vector<int>dots;

    dots.clear();
    svg << start(c.cols, c.rows);
    rect field("100%", "100%");
    svg << field.writeNoXY(255, 255, 255);

    unsigned char picture = 255;
    unsigned char condition = 0;

    for (int j = 1; j < c.cols - 1; j++) {
        for (int i = 1; i < c.rows - 1; i++) {

            if (nc.at<Vec3b>(i, j)[0] == condition) {
                line(nc, c, i, j, dots, picture, condition);
                path p(dots);
                dots.clear();
                p.simple();
                svg << p.d(c);
            }

        }
    }

    dots.clear();

    svg << end();

}
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <array>
#include <vector>
#include <iostream>
#include <fstream>
using namespace cv;


void WaB(Mat& c, int &white, int &black) {
    for (int i = 0; i < c.rows - 1; i++) {
        for (int j = 0; j < c.cols - 1; j++) {
            if (c.at<Vec3b>(i, j)[0] > 128 || c.at<Vec3b>(i, j)[1] > 128 || c.at<Vec3b>(i, j)[2] > 128) {
                c.at<Vec3b>(i, j) = { 255, 255, 255 };
                white++;
            }
            else {
                c.at<Vec3b>(i, j) = { 0, 0, 0 };
                black++;
            }

        }
    }
}

void neib(Mat& c, Mat& nc, int i, int j) {
    if ((c.at<Vec3b>(i, j) != c.at<Vec3b>(i - 1, j)) || (c.at<Vec3b>(i, j) != c.at<Vec3b>(i + 1, j)) ||
        (c.at<Vec3b>(i, j) != c.at<Vec3b>(i, j - 1)) || (c.at<Vec3b>(i, j) != c.at<Vec3b>(i, j + 1))) {

        nc.at<Vec3b>(i, j) = { c.at<Vec3b>(i, j)[0], c.at<Vec3b>(i, j)[1], c.at<Vec3b>(i, j)[2] };
    }
}

void Valuable_dots(Mat &c, Mat &hc, int &i, int &j) {
    if ((c.at<Vec3b>(i, j) != c.at<Vec3b>(i - 1, j)) + (c.at<Vec3b>(i, j) != c.at<Vec3b>(i, j - 1)) + 
        (c.at<Vec3b>(i, j) != c.at<Vec3b>(i + 1, j)) + (c.at<Vec3b>(i, j) != c.at<Vec3b>(i, j + 1)) >= 2) {

        hc.at<Vec3b>(i, j) = { c.at<Vec3b>(i, j)[0], c.at<Vec3b>(i, j)[1], c.at<Vec3b>(i, j)[2] };
    }
}


void idealize() {

}


void lineB(Mat &nc, Mat &hc, Mat &c, int &i, int &j, std::vector<int> &dots) {
    dots.clear();
    int a = i;
    int b = j;
    bool exit = false;
    bool next = false;
    nc.at<Vec3b>(i, j) = { 0,0,0 };

    if (nc.at<Vec3b>(i, j)[1] == 255) {
        nc.at<Vec3b>(i, j) = { 0,0,0 };
        dots.push_back(i);
        dots.push_back(j);
    }


    for (int count = 0; !exit && count < 3; ) {

        next = false;

        if (nc.at<Vec3b>(a - 1, b - 1)[1] == 255) {
            a -= 1;
            b -= 1;
            next = true;
        }
        else if (nc.at<Vec3b>(a - 1, b)[1] == 255) {
            a -= 1;
            next = true;
        }
        else if (nc.at<Vec3b>(a - 1, b + 1)[1] == 255) {
            a -= 1;
            b += 1;
            next = true;
        }
        else if (nc.at<Vec3b>(a, b - 1)[1] == 255) {
            b -= 1;
            next = true;
        }
        else if (nc.at<Vec3b>(a, b + 1)[1] == 255) {
            b += 1;
            next = true;
        }
        else if (nc.at<Vec3b>(a + 1, b - 1)[1] == 255) {
            a += 1;
            b -= 1;
            next = true;
        }
        else if (nc.at<Vec3b>(a + 1, b)[1] == 255) {
            a += 1;
            next = true;
        }
        else if (nc.at<Vec3b>(a + 1, b + 1)[1] == 255) {
            a += 1;
            b += 1;
            next = true;
        }


        if (next) {
            nc.at<Vec3b>(a, b) = { 0,0,0 };
            if (hc.at<Vec3b>(a, b)[1] == 255) {
                dots.push_back(a);
                dots.push_back(b);
                hc.at<Vec3b>(a, b) = { 0,0,0 };
            }
        }
        else {
            exit = true;
        }
    }
}

void lineW(Mat& nc, Mat& hc, Mat& c, int& i, int& j, std::vector<int>& dots) {
    dots.clear();
    int a = i;
    int b = j;
    bool next = false;
    nc.at<Vec3b>(i, j) = { 255,255,255 };

    if (nc.at<Vec3b>(i, j)[1] == 0) {
        nc.at<Vec3b>(i, j) = { 255,255,255 };
        dots.push_back(i);
        dots.push_back(j);
    }

    for (int count = 0; count < 8;) {

        if (a > 0 && a < c.rows && b > 0 && b < c.cols) {

            next = false;

            if (nc.at<Vec3b>(a - 1, b - 1)[1] == 0) {
                a -= 1;
                b -= 1;
                next = true;
            }
            else if (nc.at<Vec3b>(a - 1, b)[1] == 0) {
                a -= 1;
                next = true;
            }
            else if (nc.at<Vec3b>(a - 1, b + 1)[1] == 0) {
                a -= 1;
                b += 1;
                next = true;
            }
            else if (nc.at<Vec3b>(a, b - 1)[1] == 0) {
                b -= 1;
                next = true;
            }
            else if (nc.at<Vec3b>(a, b + 1)[1] == 0) {
                b += 1;
                next = true;
            }
            else if (nc.at<Vec3b>(a + 1, b - 1)[1] == 0) {
                a += 1;
                b -= 1;
                next = true;
            }
            else if (nc.at<Vec3b>(a + 1, b)[1] == 0) {
                a += 1;
                next = true;
            }
            else if (nc.at<Vec3b>(a + 1, b + 1)[1] == 0) {
                a += 1;
                b += 1;
                next = true;
            }

            if (next) {
                nc.at<Vec3b>(a, b) = { 255,255,255 };
                if (hc.at<Vec3b>(a, b)[1] == 0) {
                    dots.push_back(a);
                    dots.push_back(b);
                    hc.at<Vec3b>(a, b) = { 255,255,255 };
                }
            }
            else { 
                count += 8;

                for (int lloo : dots) {
                    std::cout << lloo << " ";
                }
            }
        }
        else {
            count += 8;
        }
    }
}



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
    int Mx = 0;
    int My = 0;
    std::string stroke;
    std::vector<int> dotI;
    std::array <std::string, 3> rgb = { "0", "0", "0" };
public:
    path() {};

    path(std::vector<int>& dots) {
        Mx = 0;
        My = 0;
        if(dots.size() > 1){
            dotI = dots;
            Mx = dotI[dotI.size() - 1];
            My = dotI[dotI.size() - 2];

            dotI.pop_back();
            dotI.pop_back();
        }
    };

    ~path() {
        dotI.clear();
    };

    std::string L(Mat &c) {
        if (dotI.size() > 0) {
            if (c.at<Vec3b>(dotI[0], dotI[1]) == c.at<Vec3b>(dotI[0] + 1, dotI[1])) {
                rgb[0] = (c.at<Vec3b>(dotI[0], dotI[1] - 1)[1] == 255 ? "0" : "255");
                rgb[1] = (c.at<Vec3b>(dotI[0], dotI[1] - 1)[1] == 255 ? "0" : "255");
                rgb[2] = (c.at<Vec3b>(dotI[0], dotI[1] - 1)[1] == 255 ? "0" : "255");
            }
            else {
                rgb[0] = (c.at<Vec3b>(dotI[0] - 1, dotI[1])[1] == 255 ? "0" : "255");
                rgb[1] = (c.at<Vec3b>(dotI[0] - 1, dotI[1])[1] == 255 ? "0" : "255");
                rgb[2] = (c.at<Vec3b>(dotI[0] - 1, dotI[1])[1] == 255 ? "0" : "255");
            }

            std::string cont = "M ";
            cont = cont + std::to_string(Mx) + " " + std::to_string(My) + "\nL ";

            while (dotI.size() > 0) {
                cont += std::to_string(dotI[dotI.size() - 1]) + " " + std::to_string(dotI[dotI.size() - 2]) + "\n";

                dotI.pop_back();
                dotI.pop_back();
            }

            return "<path fill=\"rgb(" + rgb[0] + " " + rgb[1] + " " + rgb[2] + ")\"\nd=\""
                + cont + " z\"\n/>\n";
        }
        else {
            dotI.clear();
            return "";
        }
    }
};



int main() {
    std::string name = "C:/Users/alexe/source/repos/openCV/rect.png";
    std::string name1, name2, result;
    Mat c = imread(name, IMREAD_COLOR);

    int white = 0, black = 0;

    WaB(c, white, black);

    for (int i = name.length() - 1; name[i] != '.'; i--) {
        name.pop_back();
    }
    name.pop_back();
    name1 = name + "1.png";
    name2 = name + "2.png";
    result = name + ".svg";
    name += ".png";

    std::ofstream newFile(name1);
    newFile.close();
    std::ofstream moreNewFile(name2);
    moreNewFile.close();
    std::ofstream svg(result);

    Mat nc = imread(name1, IMREAD_COLOR);
    Mat hc = imread(name2, IMREAD_COLOR);
    resize(c, nc, Size(), 1, 1);
    resize(c, hc, Size(), 1, 1);
    if (white > black) {
        nc.setTo(Scalar(255, 255, 255));
        hc.setTo(Scalar(255, 255, 255));
    }
    else {
        nc.setTo(Scalar(0, 0, 0));
        hc.setTo(Scalar(0, 0, 0));
    }

    for (int i = 1; i < c.rows - 2; i++) {
        for (int j = 1; j < c.cols - 2; j++) {
            neib(c, nc, i, j);
            Valuable_dots(c, hc, i, j);
        }

    imshow("One", c);
    imshow("Two", nc);
    imshow("Three", hc);

    int z = waitKey(0);
    if (z == '1') {
        imwrite(name, c);
    }
    if (z == '2') {
        imwrite(name1, nc);
        imwrite(name2, hc);
    }

    if (z == 'q') {
        return 0;
    }

    std::vector<int>dots;
    
    dots.clear();
    svg << start(c.cols, c.rows);
    rect field("100%", "100%");
    svg << field.writeNoXY(c.at<Vec3b>(2,2)[0], c.at<Vec3b>(2, 2)[1], c.at<Vec3b>(2, 2)[2]);

    if (black > white) {
        for (int i = 1; i < c.rows - 1; i++) {
            for (int j = 1; j < c.cols - 1; j++) {
                if (nc.at<Vec3b>(i, j)[0] == 255) {
                    lineB(nc, hc, c, i, j, dots);
                    path p(dots);
                    svg << p.L(c);
                }
            }
        }
    }

    else {
        for (int i = 1; i < c.rows - 1; i++) {
            for (int j = 1; j < c.cols - 1; j++) {
                if (nc.at<Vec3b>(i, j)[0] == 255) {
                    path p(dots);
                    lineW(nc, hc, c, i, j, dots);
                    svg << p.L(c);
                }
            }
        }
    }
    dots.clear();

    svg << end();


}

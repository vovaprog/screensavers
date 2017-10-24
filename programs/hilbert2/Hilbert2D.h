#ifndef HILBERT_2D_INCLUDED
#define HILBERT_2D_INCLUDED

#include <Matrix.h>

#include <iostream>
#include <vector>


class Hilbert2D
{
public:
    enum class CalcMode
    {
        Plane, Lines
    };

    ~Hilbert2D()
    {
        deletePlane();
    }

    void calc(int order, CalcMode argMode)
    {
        mode = argMode;

        Matrix<2, 2> rot
        {
            { 0, -1 },
            { 1,  0 }
        };

        Matrix<2, 1> startInc { { 0 }, { 1 } };
        Matrix<2, 1> startPoint { { 0 }, { 0 } };

        if (mode == CalcMode::Plane)
        {
            preparePlane(order);
        }
        else
        {
            prepareLines(startPoint);
        }

        hilbert(startPoint, startInc, rot.transpose(), order);

        lines.push_back(std::make_pair(startPoint.m[0][0], startPoint.m[1][0]));
    }

    void printPlane() const
    {
        for (int i = 0; i < planeSize; ++i)
        {
            for (int j = 0; j < planeSize; ++j)
            {
                std::cout << (plane[i][j] ? '@' : ' ');
            }
            std::cout << std::endl;
        }
    }

    void printLines() const
    {
        for (auto &p : lines)
        {
            std::cout << p.first << " : " << p.second << std::endl;
        }
    }

    const std::vector<std::pair<int, int>>& getLines() const
    {
        return lines;
    }


    static int calcSize(int order)
    {
        int size = 1;

        for (int i = 0; i < order; ++i)
        {
            size = size * 2 + 1;
        }

        return size;
    }

private:

    void drawPoint(const Matrix<2, 1> &p)
    {
        if (mode == CalcMode::Plane)
        {
            plane[p.m[1][0]][p.m[0][0]] = true;
        }
        else
        {
            if (prevPoint1.m[0][0] >= 0 && prevPoint2.m[0][0] >= 0)
            {
                if (prevPoint2 - prevPoint1 != p - prevPoint2)
                {
                    lines.push_back(std::make_pair(prevPoint2.m[0][0], prevPoint2.m[1][0]));
                }
            }

            prevPoint1 = prevPoint2;
            prevPoint2 = p;
        }
    }

    void hilbert(Matrix<2, 1> &p, Matrix<2, 1> inc, const Matrix<2, 2> &rot, int order)
    {
        if (order == 0)
        {
            drawPoint(p);
        }
        else
        {
            hilbert(p, rot * inc, rot.transpose(), order - 1);

            p += inc;
            drawPoint(p);
            p += inc;

            hilbert(p, inc, rot, order - 1);

            Matrix<2, 1> inc2 = rot * inc;

            p += inc2;
            drawPoint(p);
            p += inc2;

            hilbert(p, inc, rot, order - 1);

            inc2 = rot * inc2;

            p += inc2;
            drawPoint(p);
            p += inc2;

            hilbert(p, rot * inc2, rot.transpose(), order - 1);
        }
    }


    void preparePlane(int order)
    {
        deletePlane();

        int size = calcSize(order);

        plane = new bool*[size];
        for (int i = 0; i < size; ++i)
        {
            plane[i] = new bool[size];

            for (int j = 0; j < size; ++j)
            {
                plane[i][j] = false;
            }
        }

        planeSize = size;
    }


    void prepareLines(const Matrix<2, 1> &startPoint)
    {
        prevPoint1.m[0][0] = -1;
        prevPoint2.m[0][0] = -1;

        lines.clear();
        lines.push_back(std::make_pair(startPoint.m[0][0], startPoint.m[1][0]));
    }


    void deletePlane()
    {
        if (plane != nullptr)
        {
            for (int i = 0; i < planeSize; ++i)
            {
                delete[] plane[i];
            }
            delete[] plane;
            plane = nullptr;
            planeSize = 0;
        }
    }


private:

    CalcMode mode;

    bool **plane = nullptr;
    int planeSize = 0;

    Matrix<2, 1> prevPoint1, prevPoint2;

    std::vector<std::pair<int, int>> lines;
};

#endif

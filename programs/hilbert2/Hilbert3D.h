#ifndef HILBERT_3D_INCLUDED
#define HILBERT_3D_INCLUDED

#include <vector>
#include <tuple>

#include <Matrix.h>

class Hilbert3D
{
public:

    void calc(int order)
    {
        Matrix<3, 3> identityMatrix
        {
            { 1, 0, 0 },
            { 0, 1, 0 },
            { 0, 0, 1 }
        };

        Matrix<3, 1> startPoint { { 0 }, { 0 }, { 0 } };

        prepareLines(startPoint);

        hilbert(startPoint, identityMatrix, order, true);

        lines.push_back(std::make_tuple(startPoint.m[0][0], startPoint.m[1][0], startPoint.m[2][0]));
    }


    const std::vector<std::tuple<int, int, int>>& getLines() const
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


    void printLines() const
    {
        for (auto &p : lines)
        {
            std::cout << std::get<0>(p) << " : " << std::get<1>(p) << " : " << std::get<2>(p) << std::endl;
        }
    }


private:

    // rot - current cube matrix.
    // to rotate it in cube local coordinates - multiply it by needed rotation matrix.
    // transpose matrix method changes rotation angle sign.
    // direction specifies in which direction we move in cube.
    void hilbert(Matrix<3, 1> &p, Matrix<3, 3> rot, int order, bool direction)
    {
        if (order == 0)
        {
            drawPoint(p);
        }
        else
        {
            Matrix<3, 1> inc1 = rot * baseInc1;
            Matrix<3, 1> inc2 = rot * baseInc2;
            Matrix<3, 1> inc3 = rot * baseInc3;

            if (direction)
            {
                hilbert(p, rot * rotY, order - 1, !direction);

                p += inc1;
                drawPoint(p);
                p += inc1;

                hilbert(p, rot * rotZ.transpose(), order - 1, !direction);

                p += inc2;
                drawPoint(p);
                p += inc2;

                hilbert(p, rot * rotZ.transpose(), order - 1, !direction);

                p -= inc1;
                drawPoint(p);
                p -= inc1;

                hilbert(p, rot * rotX * rotX, order - 1, direction);

                p += inc3;
                drawPoint(p);
                p += inc3;

                hilbert(p, rot * rotX * rotX, order - 1, direction);

                p += inc1;
                drawPoint(p);
                p += inc1;

                hilbert(p, rot * rotZ, order - 1, !direction);

                p -= inc2;
                drawPoint(p);
                p -= inc2;

                hilbert(p, rot * rotZ, order - 1, !direction);

                p -= inc1;
                drawPoint(p);
                p -= inc1;

                hilbert(p, rot * rotY.transpose(), order - 1, !direction);
            }
            else
            {
                hilbert(p, rot * rotY.transpose(), order - 1, !direction);

                p += inc1;
                drawPoint(p);
                p += inc1;

                hilbert(p, rot * rotZ, order - 1, !direction);

                p += inc2;
                drawPoint(p);
                p += inc2;

                hilbert(p, rot * rotZ, order - 1, !direction);

                p -= inc1;
                drawPoint(p);
                p -= inc1;

                hilbert(p, rot * rotX * rotX, order - 1, direction);

                p -= inc3;
                drawPoint(p);
                p -= inc3;

                hilbert(p, rot * rotX * rotX, order - 1, direction);

                p += inc1;
                drawPoint(p);
                p += inc1;

                hilbert(p, rot * rotZ.transpose(), order - 1, !direction);

                p -= inc2;
                drawPoint(p);
                p -= inc2;

                hilbert(p, rot * rotZ.transpose(), order - 1, !direction);

                p -= inc1;
                drawPoint(p);
                p -= inc1;

                hilbert(p, rot * rotY, order - 1, !direction);
            }
        }
    }

    void drawPoint(const Matrix<3, 1> &p)
    {
        if (prevPoint1.m[0][0] >= 0 && prevPoint2.m[0][0] >= 0)
        {
            if (prevPoint2 - prevPoint1 != p - prevPoint2)
            {
                lines.push_back(std::make_tuple(prevPoint2.m[0][0], prevPoint2.m[1][0], prevPoint2.m[2][0]));
            }
        }

        prevPoint1 = prevPoint2;
        prevPoint2 = p;
    }


    void prepareLines(const Matrix<3, 1> &startPoint)
    {
        prevPoint1.m[0][0] = -1;
        prevPoint2.m[0][0] = -1;

        lines.clear();
        lines.push_back(std::make_tuple(startPoint.m[0][0], startPoint.m[1][0], startPoint.m[2][0]));
    }


private:

    Matrix<3, 1> baseInc1 { { 0 }, { 0 }, { -1 } };
    Matrix<3, 1> baseInc2 { { 0 }, { 1 }, {  0 } };
    Matrix<3, 1> baseInc3 { { 1 }, { 0 }, {  0 } };

    Matrix<3, 3> rotX
    {
        { 1, 0,  0 },
        { 0, 0, -1 },
        { 0, 1,  0 }
    };

    Matrix<3, 3> rotY
    {
        { 0, 0, -1 },
        { 0, 1,  0 },
        { 1, 0,  0 }
    };

    Matrix<3, 3> rotZ
    {
        { 0, -1, 0 },
        { 1,  0, 0 },
        { 0,  0, 1 }
    };

    Matrix<3, 1> prevPoint1, prevPoint2;

    std::vector<std::tuple<int, int, int>> lines;
};

#endif

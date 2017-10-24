#include <Hilbert2D.h>
#include <Hilbert3D.h>

int main()
{
    //Hilbert2D h;
    //h.calc(5, Hilbert2D::CalcMode::Plane);
    //h.printPlane();

    Hilbert3D h;
    h.calc(2);
    h.printLines();

    return 0;
}

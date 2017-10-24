#include <Hilbert2D.h>
#include <Hilbert3D.h>

int main()
{
    Hilbert2D h;
    h.calc(5, Hilbert2D::CalcMode::Plane);
    h.printPlane();

    return 0;
}

package screensavers.fractalflame;

public class FractalFlame {

    public static native void fractaFlameInitLibrary();
    public static native void fractaFlameInit(int Width,int Height);
    public static native void fractalFlameNextFrame(int[] Output);
    public static native void fractaFlameDestroy();

    static {
        System.loadLibrary("FractalFlame");
        fractaFlameInitLibrary();
    }
}

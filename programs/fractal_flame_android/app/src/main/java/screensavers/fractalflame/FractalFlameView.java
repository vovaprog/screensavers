package screensavers.fractalflame;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.view.View;

public class FractalFlameView extends View {

    private Bitmap bmp;

    private int[] output;

    private int pictureWidth, pictureHeight;


    private boolean firstInit = true;

    public FractalFlameView(Context context)
    {
        super(context);
    }

    private void init()
    {
        pictureWidth = getWidth();
        pictureHeight = getHeight();

        bmp =Bitmap.createBitmap(pictureWidth, pictureHeight, Bitmap.Config.ARGB_8888);

        output =new int[pictureWidth * pictureHeight];

        if(firstInit)
        {
            firstInit = false;
        }
        else
        {
            FractalFlame.fractaFlameDestroy();
        }

        FractalFlame.fractaFlameInit(pictureWidth, pictureHeight);
    }

    @Override
    protected void onSizeChanged(int w,int h,int oldw,int oldh)
    {
        super.onSizeChanged(w, h, oldw, oldh);

        init();
    }

    @Override
    protected void onDraw(Canvas canvas) {

        super.onDraw(canvas);

        if(bmp == null)
        {
            init();
        }

        FractalFlame.fractalFlameNextFrame(output);

        bmp.setPixels(output, 0, pictureWidth, 0, 0, pictureWidth, pictureHeight);

        canvas.drawBitmap(bmp, 0, 0, null);

        invalidate();
    }
}

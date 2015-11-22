
uniform sampler1D tex;
uniform vec2 c;
uniform int iterLimit;
uniform float bound;
uniform float checkDistanceSquare;

void main() 
{
	vec2 z;
	z.x = bound * 2.0 * (gl_TexCoord[0].x - 0.5);
	z.y = bound * 2.0 * (gl_TexCoord[0].y - 0.5);

	int i;
	for(i=0; i<iterLimit-1; i++) 
	{
		float x = (z.x * z.x - z.y * z.y) + c.x;
		float y = (z.y * z.x + z.x * z.y) + c.y;

		if(x * x + y * y > checkDistanceSquare)
		{
		    break;
		}

		z.x = x;
		z.y = y;
	}
	
	float clr=float(i) / float(iterLimit);
		
	gl_FragColor = texture1D(tex, clr);
}

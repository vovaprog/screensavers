// http://pixelshaders.com/examples/noise.html


uniform float time;
uniform sampler1D tex;

float random(float p) 
{
  return fract(sin(p)*10000.0);
}

float noise(vec2 p) 
{
  return random(p.x + p.y*10000.0);
}

vec2 sw(vec2 p) {return vec2( floor(p.x) , floor(p.y) );}
vec2 se(vec2 p) {return vec2( ceil(p.x)  , floor(p.y) );}
vec2 nw(vec2 p) {return vec2( floor(p.x) , ceil(p.y)  );}
vec2 ne(vec2 p) {return vec2( ceil(p.x)  , ceil(p.y)  );}

float smoothNoise(vec2 p) 
{
  vec2 inter = smoothstep(0., 1., fract(p));
  float s = mix(noise(sw(p)), noise(se(p)), inter.x);
  float n = mix(noise(nw(p)), noise(ne(p)), inter.x);
  return mix(s, n, inter.y);
  return noise(nw(p));
}

float movingNoise(vec2 p) 
{
  float total = 0.0;
  total += smoothNoise(p     - time);
  total += smoothNoise(p*2.0  + time) / 2.0;
  total += smoothNoise(p*4.0  - time) / 4.0;
  total += smoothNoise(p*8.0  + time) / 8.0;
  total += smoothNoise(p*16.0 - time) / 16.0;
  total /= 1.0 + 1.0/2.0 + 1.0/4.0 + 1.0/8.0 + 1.0/16.0;
  return total;
}

float nestedNoise(vec2 p) 
{
  float x = movingNoise(p);
  float y = movingNoise(p + 100.0);
  return movingNoise(p + vec2(x, y));
}

void main()
{
    vec2 p = vec2(gl_TexCoord[0].x,gl_TexCoord[0].y) * 6.0;
    float brightness = nestedNoise(p);
  
    brightness = brightness / 1.5; 
    gl_FragColor = texture1D(tex, brightness);
}


#version 150
uniform vec2 resolution;
uniform float firstPass;
uniform vec3 position;
uniform vec3 direction;
uniform vec3 upVector;
uniform float time;





uniform sampler2D colorTex;
uniform sampler2D depthRGBTex;

#define MAX_STEPS 100
#define INF 64

out vec3 color;
out vec3 depthRGB;


#define SCALE -1.5
#define MINRAD2 0.5

#define DIST_MULTIPLIER 1.0
#define MAX_DIST 4.0

#define iters 20

float minRad2 = clamp(MINRAD2, 1.0e-9, 1.0);
vec4 scale = vec4(SCALE, SCALE, SCALE, abs(SCALE)) / minRad2;
float absScalem1 = abs(SCALE - 1.0);
float AbsScaleRaisedTo1mIters = pow(abs(SCALE), float(1-iters));

float d(vec3 pos) {
  vec4 p = vec4(pos,1), p0 = p;  // p.w is the distance estimate

  for (int i=0; i<iters; i++) {    
    p.xyz = clamp(p.xyz, -1.0, 1.0) * 2.0 - p.xyz;  
    float r2 = dot(p.xyz, p.xyz);
    p *= clamp(max(minRad2/r2, minRad2), 0.0, 1.0);      
    p = p*scale + p0;
  }
  return ((length(p.xyz) - absScalem1) / p.w - AbsScaleRaisedTo1mIters) * DIST_MULTIPLIER;
}



float sdBox( vec3 p, vec3 b )
{
  vec3 d = abs(p) - b;
  return min(max(d.x,max(d.y,d.z)),0.0) +
         length(max(d,0.0));
}


float menger( vec3 p , float lod) {

	float d = sdBox(p,vec3(1.0));
	float s = 1.0;
	for( int m=0; m<lod; m++ )
	{
		vec3 a = mod( p*s, 2.0 )-1.0;
		s *= 3.0;
		vec3 r = abs(1.0 - 3.0*abs(a));

		float da = max(r.x,r.y);
		float db = max(r.y,r.z);
		float dc = max(r.z,r.x);
		float c = (min(da,min(db,dc))-1.0)/s;

		d = max(d,c);
	}
	return d;

}

float repetition(vec3 p, float lod) {	
	vec3 c = vec3(2.7, 3.7, 2.7);// 2);
	vec3 q = mod(p,c)-0.5*c;
	return menger(q, lod);
}


float map(vec3 p, float lod) {
	
	//if(p.y < 0)
		return repetition(p, lod);
	//else
	//	return 1000;
	
}
/*

vec3 getNormal(vec3 p ) {
	float eps = 0.0001;
	float center = map(p);
	vec3 n = vec3( center - map(vec3(p.x+eps,p.y,p.z)),
                   center - map(vec3(p.x,p.y+eps,p.z)),
                   center - map(vec3(p.x,p.y,p.z+eps)));
	return normalize(n);
}
*/
vec4 intersect(vec3 origin, vec3 direction)
{
	float rayLength = 0.005;	
	    
    float maxt = 100.0f;	
	float i;
	float dist =0;
	for (i = 0; i < MAX_STEPS; ++i)
	{
		vec3 point = origin + direction*rayLength;
		float len = rayLength;
		len = 50.0/len;
		//len = len;
		//len *= 5;
		len = 1+ floor(len);

		
		dist = map(origin + direction * rayLength, len);
		if(dist < 0.001*rayLength)
			return vec4(point, abs(i-10)/MAX_STEPS );

		rayLength += dist;		
	}	
	float steps = abs(i-10)/MAX_STEPS;
		
	return vec4(origin + direction*maxt, steps);
}

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

vec3 colorize(float c) {
	
	float hue = mix (0.75, 1.15, min(c * 1.2 - 0.05, 0.25));
	float sat = 1.0 - pow(c, 4.0);
	float lum = c;
	vec3 hsv = vec3(hue, sat, lum);
	vec3 rgb = hsv2rgb(hsv);
	return rgb;
}

/*
float ambient(vec3 position, vec3 normal) {
	float dist=0;
	normal *= -1;
	vec3 n;
	vec3 p;
	for(float i = 1; i < 5; ++i) {
		n = normal*i/50;
		p = position+n;
		dist += 1/pow(2, i) * (abs(map(p))/length(n));
	}	
	if(dist > 0.95)
		return 0.9;
	else 
		return dist;
}
*/


void main( void ) {

	vec2 uv = gl_FragCoord.xy / resolution;
	
	vec2 p = (uv * 2.0 - 1.0); p.x *= resolution.x / resolution.y;
	vec3 pos = vec3(position.x, position.y + 0.01, position.z) ;
	vec3 origin = pos;
	vec3 target = pos + direction;
		
	vec3 direction = normalize(target - origin);	
	vec3 right = normalize(cross(direction, upVector));	
	vec3 up = normalize(cross(right, direction));	
	vec3 rayDirection = normalize(p.x * right + p.y * up + 1.5 * direction);			
	vec4 intersectionPoint = intersect(origin, rayDirection);
	
    vec3 shit = vec3(0.0); 
	
	//if(intersectionPoint.w < 1){
		//shit +=  ambient(intersectionPoint.xyz, getNormal(intersectionPoint.xyz));//*(1.01 - intersectionPoint.w); 
	//}
	shit += 1.2*(colorize((intersectionPoint.w)) );
	
	color = shit;
	
}
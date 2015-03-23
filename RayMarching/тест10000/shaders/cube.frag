#version 150
uniform vec2 resolution;
uniform float firstPass;
uniform vec3 position;
uniform vec3 direction;
uniform vec3 upVector;
uniform vec3 deltaTranslate;
uniform float time;


uniform mat4 inverseMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;



uniform sampler2D colorTex;
uniform sampler2D depthRGBTex;

#define MAX_STEPS 131
#define SPEED_SCALE 0.25
#define INF 64

out vec3 color;
out vec3 depthRGB;


float sdBox( vec3 p, vec3 b )
{
  vec3 d = abs(p) - b;
  return min(max(d.x,max(d.y,d.z)),0.0) +
         length(max(d,0.0));
}


float menger( vec3 p ) {

	float d = sdBox(p,vec3(1.0));
	float s = 1.0;
	for( int m=0; m<5; m++ )
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

float repetition(vec3 p) {	
	vec3 c = vec3(2.7, 2.7, 2.7);// 2);
	vec3 q = mod(p,c)-0.5*c;
	return menger(q);
}

float map(vec3 p) {

	if(p.y < 0)
		return repetition(p);
	else
		return 1000;

}

vec3 getNormal(vec3 p ) {
	float eps = 0.0001;
	float center = map(p);
	vec3 n = vec3( center - map(vec3(p.x+eps,p.y,p.z)),
                   center - map(vec3(p.x,p.y+eps,p.z)),
                   center - map(vec3(p.x,p.y,p.z+eps)) );
	return normalize(n);
}

vec4 intersect(vec3 origin, vec3 direction)
{
	float rayLength = 0.005;	
	    
    float maxt = 120.0f;	
	float i;
	for (i = 0; i < MAX_STEPS; ++i)
	{
		vec3 point = origin + direction*rayLength;
		
		float dist = map(origin + direction * rayLength);
		if(dist < 0.001*rayLength)
			return vec4(point, abs(i-10)/120 );

		rayLength += dist;		
	}	
	float steps = abs(i-10)/120;
		
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

float ambient(vec3 position, vec3 normal) {
	float dist=0;
	normal *= -1;
	vec3 n;
	vec3 p;
	for(float i = 1; i < 5; ++i) {
		n = normal*i/50;
		p = position+n;
		dist += pow(0.5, i) * (abs(map(p))/length(n));
	}	
	return  dist;
}





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
	float d1 = 1.00;
	//if(firstPass == 0.0)
		//d1 = packColor(texture2D(depthRGBTex, vec2(1-uv.x, 1-uv.y) ).rgb);	
	vec4 intersectionPoint = intersect(origin, rayDirection*d1);
	
    vec3 shit = vec3(0.0); 
	shit += vec3(1.0,1.0,1.0) * 0.2* ambient(intersectionPoint.xyz, getNormal(intersectionPoint.xyz)); 
	if(intersectionPoint.w < 1){
		shit += vec3(0.2,1.0,1.5) * 0.4 * ambient(intersectionPoint.xyz, getNormal(intersectionPoint.xyz))*(1.01 - intersectionPoint.w); 
	}
	shit += colorize(intersectionPoint.w);//0.3*vec3(8.0*intersectionPoint.w, 0.0*intersectionPoint.w, 1.2);
	shit += colorize(length(intersectionPoint.xyz - origin)/120);
//	depthRGB = vec3(0.5); 
	//color = vec3(length(origin-intersectionPoint.xyz)/ (length(direction)*420.0) );
	//if(firstPass == 0.0)
	//	color = texture2D(colorTex, uv).rgb;
	//else
	//	color = vec3(0.0, 0.0, 0.5);
	depthRGB = shit;
	color = shit;
}
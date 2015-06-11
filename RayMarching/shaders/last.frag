#version 150
uniform vec2 resolution;
uniform float firstPass;
uniform vec3 position;
uniform vec3 direction;
uniform vec3 upVector;


uniform vec3 previousDirection;
uniform vec3 previousPosition;
uniform float time;
uniform mat4 previousViewMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 inverseMatrix;

uniform int frameindex;


uniform sampler2D colorTex;

#define MAX_STEPS 80
#define SPEED_SCALE 0.25
#define INF 64

out vec3 color;



//#define SCALE -2.4 + 0.5*cos(time*0.1)
#define SCALE 3// + 0.3*cos(time*0.25)
#define MINRAD2 0.5

#define DIST_MULTIPLIER 1.0
#define MAX_DIST 4.0


#define iters 10
#define color_iters 10


vec3  surfaceColor1 = vec3(0.15, 0.84, 0.51),
      surfaceColor2 = vec3(0.89, 0.15, 0.15),
      surfaceColor3 = vec3(0.25, 0.06, 0.43);

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

vec3 clr(vec3 pos) {
  vec3 p = pos, p0 = p;
  float trap = 1.0;

  for (int i=0; i<color_iters; i++) {
    p.xyz = clamp(p.xyz, -1.0, 1.0) * 2.0 - p.xyz;
    float r2 = dot(p.xyz, p.xyz);
    p *= clamp(max(minRad2/r2, minRad2), 0.0, 1.0);
    p = p*scale.xyz + p0.xyz;
    trap = min(trap, r2);
  }
  // c.x: log final distance (fractional iteration count)
  // c.y: spherical orbit trap at (0,0,0)
  vec2 c = clamp(vec2( 0.33*log(dot(p,p))-1.0, sqrt(trap) ), 0.0, 1.0);

  return mix(mix(surfaceColor1, surfaceColor2, c.y), surfaceColor3, c.x);
}



float sdBox( vec3 p, vec3 b )
{
  vec3 d = abs(p) - b;
  return min(max(d.x,max(d.y,d.z)),0.0) +
         length(max(d,0.0));
}

float menger(vec3 p) {

	float d = sdBox(p, vec3(1.0));
	float s = 1.0;
	for (int m = 0; m<4; m++)
	{
		vec3 a = mod(p*s, 2.0) - 1.0;
		s *= 3.0;
		vec3 r = abs(1.0 - 3.0*abs(a));

		float da = max(r.x, r.y);
		float db = max(r.y, r.z);
		float dc = max(r.z, r.x);
		float c = (min(da, min(db, dc)) - 1.0) / s;

		d = max(d, c);
	}
	return d;

}

float repetition(vec3 p) {	
	vec3 c = vec3(7.7, 7.7, 7.7) *1.5;
	vec3 q = mod(p,c)-0.5*c;
	return d(q);
}

float map(vec3 p) {
	
	//if(p.y < 0)
		return repetition(p);
	//else
	//	return 1000;
	//return menger(p);
}



vec4 intersect(vec3 origin, vec3 direction, float assist)
{
	float rayLength = 0.01;	
	if(assist > 0.01) 
	    rayLength = assist;

    float maxt = 100.0f;	
	float i;
	for (i = 0; i < MAX_STEPS; ++i)
	{
		vec3 point = origin + direction*rayLength;
		
		float dist = map(origin + direction * rayLength);
		if(dist < 0.001*rayLength)
			return vec4(point, abs(i)/MAX_STEPS );

		rayLength += dist;		
	}
	float steps = abs(i)/MAX_STEPS;
		
	return vec4(origin + direction*maxt, steps);
}


vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}


vec3 colorize(float c) {
	float h = time - floor(time);
	float hue = mix (0.25, 1.15, min(c * 1.2 - 0.05, 0.25));
	float sat = 1.0 - pow(c, 4.0);
	float lum = c;
	vec3 hsv = vec3(hue, sat, lum);
	vec3 rgb = hsv2rgb(hsv);
	return rgb;
}

vec3 getNormal(vec3 p ) {
	float eps = 0.0001;
	float center = map(p);
	vec3 n = vec3( center - map(vec3(p.x+eps,p.y,p.z)),
                   center - map(vec3(p.x,p.y+eps,p.z)),
                   center - map(vec3(p.x,p.y,p.z+eps)));
	return normalize(n);
}

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




void main(void) {

	vec2 uv = gl_FragCoord.xy / resolution;

	vec2 p = (uv * 2.0 - 1.0); p.x *= resolution.x / resolution.y;
	vec3 pos = vec3(position.x, position.y, position.z);
	vec3 origin = pos;
	vec3 target = pos + direction;

	vec3 direction = normalize(target - origin);
	vec3 right = normalize(cross(direction, upVector));
	vec3 up = normalize(cross(right, direction));
	vec3 rayDirection = normalize(p.x * right + p.y * up + 1.5 * direction);	

	vec2 uvn[4];
	vec2 pm[4];
	
	pm[0].x = (uv.x)*2.0 - 1.0;		
	pm[0].y = (1-uv.y)*2.0 - 1.0;
	
	mat4 invR = projectionMatrix * previousViewMatrix * inverse(projectionMatrix * viewMatrix);
	
	vec4 clip = invR * (vec4(pm[0], 1.0, 1));

	clip /= clip.w;
	uvn[0] = clip.xy;	
	uvn[0] = uvn[0]*0.5+0.5;	
	uvn[0].y = 1-uvn[0].y;
	

	//texture2D(colorTex, uvn[0]);
	
	
	//if (uvn[0].x > 1.0 || uvn[0].y > 1.0 || uvn[0].x < 0.0 || uvn[0].y < 0.0 || frameindex%3 == 0){
		
		vec4 intersectionPoint = intersect(origin, rayDirection, 0.0);
	
		//gl_FragDepth = length(origin-intersectionPoint.xyz)/100.0;
	
			color =-2.0*mix( surfaceColor1, -abs(rayDirection), intersectionPoint.w) +
				    1.19*surfaceColor3 +
					0.5*vec3(ambient(intersectionPoint.xyz, getNormal(intersectionPoint.xyz) ) + 0.1) ;

	//}
	
	//else {
	
	//	color = texture2D(colorTex, uvn[0]).xyz;
		
		
		//float depth = texture2D(depthTex, uvn[0]).z*100;

		//vec4 intersectionPoint = intersect(origin, rayDirection, depth);
		
		//gl_FragDepth = length(origin-intersectionPoint.xyz)/100.0;
		
		//color =-2.0*mix( surfaceColor1, -abs(rayDirection), intersectionPoint.w) +
		//		1.19*surfaceColor3 +
		//		0.5*vec3(ambient(intersectionPoint.xyz, getNormal(intersectionPoint.xyz) ) + 0.1) ;

	//}
	
					
		
	
	
}
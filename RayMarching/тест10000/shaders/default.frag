
uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;

#define MAX_STEPS 64
#define MAX_DEPTH 24.0
#define SPEED_SCALE 0.35
#define PI 3.141



float hash(float n)
{
  return fract(cos(n) * 41415.92653);
}

float noise( vec3 x )
{
  //x *= ;
  vec3 p  = floor(x);
  vec3 f  = smoothstep(0.0, 1.0, fract(x));
  float n = p.x + p.y*57.0 + 113.0*p.z;

  return mix(mix(mix( hash(n+  0.0), hash(n+  1.0),f.x),
    mix( hash(n+ 57.0), hash(n+ 58.0),f.x),f.y),
    mix(mix( hash(n+113.0), hash(n+114.0),f.x),
    mix( hash(n+170.0), hash(n+171.0),f.x),f.y),f.z);
}

mat3 m = mat3( 0.00,  1.60,  1.20, 
			  -1.60,  0.72, -0.96, 
			  -1.20, -0.96,  2.28 );

float fbm( vec3 p ) // experimented with and kind of modified
{

  float f = 0.5000 * noise( p ); p = m * p * 0.72;
  f += 0.2500 * noise( p ); p = m * p * 0.73;
  f += 0.1250 * noise( p ); p = m * p * 0.74;
  f += 0.0625 * noise( p ); p = m * p * 0.75; 
  f += 0.03125 * noise( p ); p = m * p;// * 0.76;
  //f += 0.015625 * noise( p );// p = m * p;
  //f += 0.0078125 * noise( p ); 	
  return f;
}


vec3 getNormal(vec3 p) {
	float eps = 0.01;
	vec3 n = vec3( noise(vec3(p.x-eps, p.y, p.z)) - noise(vec3(p.x+eps,p.y,p.z)),
                   2.0f*eps,
                   noise(vec3(p.x,p.y,p.z-eps)) - noise(vec3(p.x,p.y,p.z+eps)) );
	return normalize(n);
}


vec3 intersect(vec3 origin, vec3 direction)
{
	float rayLength = 0.1;	

	float delt = 0.05f;
    float mint = 0.001f;
    float maxt = 20.0f;
	float height;
	float lh = 0.0f;
    float ly = 0.0f;

	for (int i = 0; i < MAX_STEPS; ++i)
	{
		vec3 point = origin + direction*rayLength;
		//float height = 2*fbm(point);
		
		float height = 1.7*fbm(origin + direction * rayLength);
		if( height > point.y ){
			float resT = rayLength - delt + delt*(lh-ly)/(point.y-ly-height+lh);
			return origin + direction*resT;

		}
		lh = height;
        ly = point.y;
		delt = 0.12f*rayLength;
		rayLength += delt;
	}	
		
	return origin + direction*maxt;
}



void main( void ) {

	vec2 uv = gl_FragCoord.xy / resolution;
	vec2 p = (uv * 2.0 - 1.0); p.x *= resolution.x / resolution.y;
	
	vec3 col = vec3(0.0);
	vec3 origin = vec3(4.0* cos(SPEED_SCALE * time),
				       2,//2.0 + 1.0 * cos(SPEED_SCALE * time),
			   	       4.0 * sin(SPEED_SCALE * time));
	vec3 target = vec3(0,2,0);
		
	vec3 direction = normalize(target - origin);
	
	vec3 right = normalize(cross(direction, vec3(0,1,0)));	
	vec3 up = normalize(cross(right, direction));	
	vec3 rayDirection = normalize(p.x * right + p.y * up + 1.5 * direction);
	
	
	vec3 intersectionPoint = intersect(origin, rayDirection);
	if(length(intersectionPoint) < 40)
		col = getNormal(intersectionPoint);
	//col = vec3(0.5, intersect(origin, rayDirection).y/2.0, 0.5);
	
	gl_FragColor = vec4( col.z, col.x, col.y, 1.0 );

}
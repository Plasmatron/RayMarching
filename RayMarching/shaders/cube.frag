
uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;

#define MAX_STEPS 200
#define SPEED_SCALE 0.25
#define INF 64



float displacement1( vec3 p) {
	p *= 2;
	//p.xz *= 2;
	return 0.8*sin(p.x)*sin(p.y)*sin(p.z);
}



float sdBox( vec3 p, vec3 b )
{
  vec3 d = abs(p) - b;
  return min(max(d.x,max(d.y,d.z)),0.0) +
         length(max(d,0.0));
}


float torus(vec3 p ) {
	vec2 t = vec2(2.5, 1);
	vec2 q = vec2(length(p.xz) - t.x, p.y);
	return length(q) - t.y;

}

float sphere(vec3 p) {
	float s = 1;
	return length(p) - s;
}

float sdCross( in vec3 p )
{
  float da = sdBox(p.xyz,vec3(INF,1.0,1.0));
  float db = sdBox(p.yzx,vec3(1.0,INF,1.0));
  float dc = sdBox(p.zxy,vec3(1.0,1.0,INF));
  return min(da,min(db,dc));
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
	vec3 c = vec3(1.333, 1.333, 1.333);
	//vec3 c = vec3(1.52, /*2 + 1.5*cos(time*SPEED_SCALE*0.25), */ 2, 1.52 + 1.5*sin(time*SPEED_SCALE*0.25) );// 2);
	vec3 q = mod(p,c)-0.5*c;
	return menger(q);// max(sdBox(q, vec3(3,3,3) ),-sdCross(q));//displacement1(q) + torus(q);
}

float map(vec3 p) {
	//float sc = (time*SPEED_SCALE);
	return repetition(p);//max(sdBox(p, vec3(3,3,3) ),-sdCross(p)); 
}

vec3 getNormal(vec3 p) {
	float eps = 0.0001;
	vec3 n = vec3( map(vec3(p.x-eps, p.y, p.z)) - map(vec3(p.x+eps,p.y,p.z)),
                   map(vec3(p.x, p.y-eps, p.z)) - map(vec3(p.x,p.y+eps,p.z)),
                   map(vec3(p.x, p.y, p.z-eps)) - map(vec3(p.x,p.y,p.z+eps)) );
	return normalize(n);
}



vec4 intersect(vec3 origin, vec3 direction)
{
	float rayLength = 0.1;	

	float delt = 0.01f;
    float mint = 0.001f;
    float maxt = 60.0f;
	float height;
	float previousDist = 1000;
	float i;
	for (i = 0; i < MAX_STEPS; ++i)
	{
		vec3 point = origin + direction*rayLength;
		
		float dist = map(origin + direction * rayLength);
		if(dist < 0.003)
			return vec4(point, i/150);

		//if(previousDist < dist - 2.5) 
		//	break;

		rayLength += dist;
		previousDist = dist;
	}	
	float steps = i/150;
		
	return vec4(origin + direction*maxt, steps);
}

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

vec4 colorize(float c) {
	
	float hue = mix (0.46, 1.15, min(c * 1.2 - 0.05, 0.25));
	float sat = 1.0 - pow(c, 4.0);
	float lum = c;
	vec3 hsv = vec3(hue, sat, lum);
	vec3 rgb = hsv2rgb(hsv);
	return vec4(rgb, 1.0);	
}

float ambient(vec3 position, vec3 normal) {
	float dist=0;
	normal *= -1;
	vec3 n;
	vec3 p;
	for(int i = 1; i < 5; ++i) {
		n = normal*i/50;
		p = position+n;
		dist += pow(0.5, i) * (abs(map(p))/length(n));
	}
	//dist *= 0.5;
	return  dist;
}



void main( void ) {

	vec2 uv = gl_FragCoord.xy / resolution;
	
	vec2 p = (uv * 2.0 - 1.0); p.x *= resolution.x / resolution.y;
	
	vec3 col = vec3(0.5, 0.7, 0.05);
	vec3 origin = vec3(0.5+ 1.0*SPEED_SCALE*time, 
				       3.33,
			   	       0.0*cos(time*SPEED_SCALE) * 0.0);//2.0 * sin(SPEED_SCALE * time));
	vec3 target = vec3(0.5+1.0*SPEED_SCALE*time+1, 3.33 + 0.0*cos(SPEED_SCALE*time),  1.0*cos(SPEED_SCALE*time) *1.0);
		
	vec3 direction = normalize(target - origin);	
	vec3 right = normalize(cross(direction, vec3(0,cos(SPEED_SCALE*time),sin(SPEED_SCALE*time) )));	
	vec3 up = normalize(cross(right, direction));	
	vec3 rayDirection = normalize(p.x * right + p.y * up + 1.5 * direction);		
	vec4 intersectionPoint = intersect(origin, rayDirection);

	//if(length(intersectionPoint) < 15)
	
    vec4 shit = 0.5*vec4(ambient(intersectionPoint.xyz, getNormal(intersectionPoint.xyz))); //vec4( 0.5*(getNormal(intersectionPoint)+vec3(1,1,1)), 1.0);
	shit += 0.08*vec4(getNormal(intersectionPoint.xyz), 1.0) + 0.3*vec4(0.6, 1.2*intersectionPoint.w,5*intersectionPoint.w,1.0) +  1.5*colorize(0.1 + length(intersectionPoint.xyz - origin)/24.35);// - vec3(length(intersctionPoint)/20);
	//col = vec3(length(intersectionPoint)/10);
	//col = vec3(0.5, intersect(origin, rayDirection).y/2.0, 0.5);
	
	gl_FragColor = shit;//vec4( col.z*0.5, col.x*0.5, col.y*0.5, 1.0 );

}
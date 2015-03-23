#version 150
uniform vec2 resolution;
uniform float firstPass;
uniform vec3 position;
uniform vec3 direction;
uniform vec3 upVector;
uniform vec3 deltaTranslate;
uniform float time;


uniform mat4 previousViewMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;



uniform sampler2D colorTex;
uniform sampler2D depthTex;

#define MAX_STEPS 50
#define SPEED_SCALE 0.25
#define INF 64

out vec3 color;
//out vec3 depthRGB;


float sdBox( vec3 p, vec3 b )
{
  vec3 d = abs(p) - b;
  return min(max(d.x,max(d.y,d.z)),0.0) +
         length(max(d,0.0));
}

float menger(vec3 p) {

	float d = sdBox(p, vec3(1.0));
	float s = 1.0;
	for (int m = 0; m<5; m++)
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

float map(vec3 p) {
	/*
	if(p.y < 0)
		return repetition(p);
	else
		return 1000;*/
	return menger(p);
}


vec4 intersect(vec3 origin, vec3 direction)
{
	float rayLength = 0.01;	
	    
    float maxt = 120.0f;	
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




void main(void) {

	vec2 uv = gl_FragCoord.xy / resolution;

	vec2 p = (uv * 2.0 - 1.0); p.x *= resolution.x / resolution.y;
	vec3 pos = vec3(position.x, position.y + 0.01, position.z);
	vec3 origin = pos;
	vec3 target = pos + direction;

	vec3 direction = normalize(target - origin);
	vec3 right = normalize(cross(direction, upVector));
	vec3 up = normalize(cross(right, direction));
	vec3 rayDirection = normalize(p.x * right + p.y * up + 1.5 * direction);

	

	//mat4 invR = inverse(projectionMatrix * viewMatrix * inverse(previousViewMatrix) * inverse(projectionMatrix));
	//vec3 rayBegin = vec3(invR * vec4(uv, 0.01, 1.0));
	//vec3 rayEnd = vec3(invR * vec4(uv, 100, 1.0));	
	//vec3 ray = rayEnd - rayBegin;
	//vec2 uvStart = rayPlaneIntersection(ray, previousDirection, previousDirection * 0.01);
	//vec2 uvEnd = rayPlaneIntersection(ray, previousDirection, previousDirection * 100);	
	
	vec4 clip =  (projectionMatrix * previousViewMatrix * inverse(viewMatrix) * inverse(projectionMatrix) ) * vec4(uv.x, uv.y, 100, 1.0);	
	uv = vec2(floor(((clip.x)*resolution.x))/resolution.x, floor((clip.y)*resolution.y)/resolution.y);
	//color = vec3(intersectionPoint.w);
	float z = 0.0;
	//if (firstPass == 0.0){
	
	

	if (uv.x > 1 || uv.y > 1 || uv.x < 0 || uv.y < 0 || firstPass == 1.0){
		vec4 intersectionPoint = intersect(origin, rayDirection);
		gl_FragDepth = length(origin - intersectionPoint.xyz) / 2.0;
		color = vec3(length(origin - intersectionPoint.xyz) / 2.0);
	}
	else {		
		z = texture(depthTex, uv.xy).z;
		gl_FragDepth = z;
		color = vec3(z);
		
	}


	/* 
	}
	else{
		color = vec3(0.0);
		gl_FragDepth = length(origin - intersectionPoint.xyz) / 1.0;
	}
	*/

}
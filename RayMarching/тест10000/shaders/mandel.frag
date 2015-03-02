
uniform float time;
uniform vec2 mouse;
uniform vec2 resolution;

#define MAX_STEPS 200
#define SPEED_SCALE 0.25
#define INF 64

vec3 intersect(vec3 origin, vec3 direction)
{
	float rayLength = 0.1;	

	float delt = 0.01f;
    float mint = 0.001f;
    float maxt = 60.0f;
	float height;
	float previousDist = 1000;

	for (int i = 0; i < MAX_STEPS; ++i)
	{
		vec3 point = origin + direction*rayLength;
		
		float dist = map(origin + direction * rayLength);
		if(dist < rayLength*0.001)
			return point;

		if(previousDist < dist - 2.5) 
			break;

		rayLength += dist;
		previousDist = dist;
	}	
		
	return origin + direction*maxt;
}




void main( void ) {

	vec2 uv = gl_FragCoord.xy / resolution;	
	vec2 p = (uv * 2.0 - 1.0); p.x *= resolution.x / resolution.y;	
	vec3 col = vec3(0.5, 0.7, 0.05);
	vec3 origin = vec3(0,0,0);
	vec3 target = vec3(1,0,0);
		
	vec3 direction = normalize(target - origin);	
	vec3 right = normalize(cross(direction, vec3(0,1,0)));	
	vec3 up = normalize(cross(right, direction));	
	vec3 rayDirection = normalize(p.x * right + p.y * up + 1.5 * direction);		
	vec3 intersectionPoint = intersect(origin, rayDirection);

    vec4 shit = 0.5*vec4(ambient(intersectionPoint, getNormal(intersectionPoint))); //vec4( 0.5*(getNormal(intersectionPoint)+vec3(1,1,1)), 1.0);
	shit += 0.3*vec4(getNormal(intersectionPoint), 1.0) + colorize(0.2 + length(intersectionPoint - origin)/9.35);// - vec3(length(intersctionPoint)/20);

	gl_FragColor = shit;//vec4( col.z*0.5, col.x*0.5, col.y*0.5, 1.0 );

}
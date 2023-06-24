uniform sampler2D u_texture;
uniform float u_strength;


void main()
{
	vec2 dist = u_strength * 1.0 / vec2(textureSize(u_texture, 0));
	dist.x = -dist.x;

	vec4 pixLeft = texture2D(u_texture, gl_TexCoord[0].xy - dist);
	vec4 pixel = texture2D(u_texture, gl_TexCoord[0].xy);
	vec4 pixRight = texture2D(u_texture, gl_TexCoord[0].xy + dist);

	gl_FragColor = gl_Color * vec4(pixLeft.r, pixel.g, pixRight.b, pixel.a);
}
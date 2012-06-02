
// Color mapped (Triangles)

@mapParsVertex

#ifdef USE_MAP
  varying vec2 vUv;
  uniform vec4 offsetRepeat;
#endif

@mapParsFragment

#ifdef USE_MAP
  varying vec2 vUv;
  uniform sampler2D map;
#endif

@mapVertex

#ifdef USE_MAP
  vUv = uv * offsetRepeat.zw + offsetRepeat.xy;
#endif

@mapFragment

#ifdef USE_MAP
  #ifdef GAMMA_INPUT
    vec4 texelColor = texture2D(map, vUv);
    texelColor.xyz *= texelColor.xyz;
    gl_FragColor = gl_FragColor * texelColor;
  #else
    gl_FragColor = gl_FragColor * texture2D(map, vUv);
  #endif
#endif

// Basic shader

@basicVertexShader

=mapParsVertex

void main()
{
  vec4 mvPosition = modelViewMatrix * vec4(position, 1.0);
  =mapVertex
}

@basicFragmentShader

uniform vec3 diffuse;
uniform float opacity;

=mapParsFragment

void main()
{
  gl_FragColor = vec4(diffuse, opacity);

  =mapFragment
}

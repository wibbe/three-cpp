
# Define all the shader constans
define 'src/GLShaders.h' do
  snipet :prefixVertex, [
    "uniform mat4 objectMatrix;",
    "uniform mat4 modelViewMatrix;",
    "uniform mat4 projectionMatrix;",
    "uniform mat4 viewMatrix;",
    "uniform mat4 normalMatrix;",
    "uniform vec3 cameraPosition;",

    "attribute vec3 position;",
    "attribute vec3 normal;",
    "attribute vec2 uv;",
    "attribute vec2 uv2;",

    "#ifdef USE_COLOR",
      "attribute vec3 color;",
    "#endif",
  ]

  snipet :prefixFragment, [
    "uniform mat4 viewMatrix;",
    "uniform vec3 cameraPosition;"
  ]

  snipet :defaultVertex, [
    "gl_Position = projectionMatrix * mvPosition;"
  ]

  snipet :fogFragementParams, [
		"#ifdef USE_FOG",
			"uniform vec3 fogColor;",
			"#ifdef FOG_EXP2",
				"uniform float fogDensity;",
			"#else",
				"uniform float fogNear;",
				"uniform float fogFar;",
			"#endif",
		"#endif"
  ]

  snipet :fogFragment, [
		"#ifdef USE_FOG",
			"float depth = gl_FragCoord.z / gl_FragCoord.w;",
			"#ifdef FOG_EXP2",
				"const float LOG2 = 1.442695;",
				"float fogFactor = exp2( - fogDensity * fogDensity * depth * depth * LOG2 );",
				"fogFactor = 1.0 - clamp( fogFactor, 0.0, 1.0 );",
			"#else",
				"float fogFactor = smoothstep( fogNear, fogFar, depth );",
			"#endif",
			"gl_FragColor = mix( gl_FragColor, vec4( fogColor, gl_FragColor.w ), fogFactor );",
		"#endif"
  ]

  snipet :mapVertexParams, [
    "#ifdef USE_MAP",
      "varying vec2 vUV;",
      "uniform vec4 offsetRepeat;",
    "#endif"
  ]

  snipet :mapFragmentParams, [
    "#ifdef USE_MAP",
      "varying vUv;",
      "uniform sampler2D map;",
    "#endif"
  ]

  snipet :mapVertex, [
    "#ifdef USE_MAP",
      "vUv = uv * offsetRepeat.zw + offsetRepeat.xy;",
    "#endif"
  ]

	snipet :mapFragment, [
		"#ifdef USE_MAP",
			"#ifdef GAMMA_INPUT",
				"vec4 texelColor = texture2D(map, vUv);",
				"texelColor.xyz *= texelColor.xyz;",
				"gl_FragColor = gl_FragColor * texelColor;",
			"#else",
				"gl_FragColor = gl_FragColor * texture2D( map, vUv );",
			"#endif",
		"#endif"
	]

	snipet :colorFragmentParams, [
		"#ifdef USE_COLOR",
			"varying vec3 vColor;",
		"#endif"
	]

	snipet :colorVertexParams, [
		"#ifdef USE_COLOR",
			"varying vec3 vColor;",
		"#endif"
	]

	snipet :colorVertex, [
		"#ifdef USE_COLOR",
			"#ifdef GAMMA_INPUT",
				"vColor = color * color;",
			"#else",
				"vColor = color;",
			"#endif",
		"#endif"
	]

	snipet :colorFragment, [
		"#ifdef USE_COLOR",
			"gl_FragColor = gl_FragColor * vec4( vColor, opacity );",
		"#endif"
	]

	# Define the shaders

	shader :basicVertex, [
	  :prefixVertex,
	  :mapVertexParams,
	  :colorVertexParams,

	  "void main()",
	  "{",
	    "vec4 mvPosition = modelViewMatrix * vec4(position, 1.0);",
	    :mapVertex,
	    :colorVertex,
	    :defaultVertex,
	  "}"
	]

	shader :basicFragment, [
	  :prefixFragment,
	  "uniform vec3 diffuse",
	  "uniform float opacity",
	  :mapFragmentParams,
	  :colorFragmentParams,

	  "void main()",
	  "{",
	    "gl_FragColor = vec4(diffuse, opacity);",
	    :colorFragment,
	    :mapFragment,
	  "}"
	]

end


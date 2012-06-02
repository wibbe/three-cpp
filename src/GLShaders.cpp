
#include "GLShaders.h"
#include "StringHash.h"

namespace three {

  // Color Map (Triangles)

	static const char * mapParsVertex = {
		"#ifdef USE_MAP\n"
			"varying vec2 vUv;\n"
			"uniform vec4 offsetRepeat;\n"
		"#endif\n"
	};

	static const char * mapParsFragment = {
		"#ifdef USE_MAP\n"
			"varying vec2 vUv;\n"
			"uniform sampler2D map;\n"
		"#endif\n"
	};

	static const char * mapVertex = {
		"#ifdef USE_MAP\n"
			"vUv = uv * offsetRepeat.zw + offsetRepeat.xy;\n"
		"#endif\n"
	};

	static const char * mapFragment = {
		"#ifdef USE_MAP\n"
			"#ifdef GAMMA_INPUT\n"
				"vec4 texelColor = texture2D( map, vUv );\n"
				"texelColor.xyz *= texelColor.xyz;\n"
				"gl_FragColor = gl_FragColor * texelColor;\n"
			"#else\n"
				"gl_FragColor = gl_FragColor * texture2D( map, vUv );\n"
			"#endif\n"
		"#endif\n"
  };

}


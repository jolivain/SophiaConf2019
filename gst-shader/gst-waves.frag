#ifdef GL_ES
precision mediump float;
#endif

// gst-glshader uniforms
uniform float time;
uniform float width;
uniform float height;
uniform sampler2D tex;

void main(void) {
    vec2 uv = gl_FragCoord.xy / vec2(width, height);
    vec2 uv_offset = sin((uv.yx + 4. + time) * 3.1416) * 0.02;
    gl_FragColor = texture2D(tex, uv + uv_offset);
}

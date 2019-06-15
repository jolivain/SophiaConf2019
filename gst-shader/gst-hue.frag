#ifdef GL_ES
precision mediump float;
#endif

// gst-glshader element uniforms
uniform float time;
uniform float width;
uniform float height;
uniform sampler2D tex;

vec3 rgb2hsv(vec3 c) {
    vec4 k = vec4(0., -1. / 3., 2. / 3., -1.);
    vec4 p = mix(vec4(c.bg, k.wz), vec4(c.gb, k.xy), step(c.b, c.g));
    vec4 q = mix(vec4(p.xyw, c.r), vec4(c.r, p.yzx), step(p.x, c.r));
    float d = q.x - min(q.w, q.y);
    float e = 1.e-10;
    return vec3(abs(q.z + (q.w - q.y) / (6. * d + e)), d / (q.x + e), q.x);
}

vec3 hsv2rgb(vec3 hsv) {
    vec3 a = hsv.x * 6. + vec3(0., 4., 2.);
    vec3 b = mod(a, 6.) - 3.;
    vec3 c = abs(b) - 1.;
    vec3 rgb = clamp(c, 0., 1.);
    return hsv.z * mix(vec3(1.0), rgb, hsv.y);
}

void main(void) {
    vec2 uv = gl_FragCoord.xy / vec2(width, height);
    vec3 rgb = texture2D(tex, uv).rgb;
    vec3 hsv = rgb2hsv(rgb);
    float hue_offset = mod(time * 0.5, 1.);
    vec3 new_hsv = vec3(hsv.x + hue_offset, hsv.y, hsv.z);
    vec3 new_rgb = hsv2rgb(new_hsv);

    gl_FragColor = vec4(new_rgb, 1.);
}

#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture1;

void main() {
    //    if (gl_FragCoord.x < 800) {
    //        FragColor = vec4(1.0, 0.0, 0.0, 1.0); // 右侧显示红色
    //    } else {
    //        FragColor = vec4(0.0, 1.0, 0.0, 1.0); // 右侧显示绿色
    //    }
    if (gl_FrontFacing){
        FragColor = vec4(125.0/255, 16.0/255, 232.0/255, 1.0);
    } else {
        FragColor = vec4(0.7, 0.7, 0.7, 1.0);
    }
}
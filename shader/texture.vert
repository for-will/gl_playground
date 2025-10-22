#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;
out vec4 ourColor; // 为片段着色器指定一个颜色输出
out vec2 TexCoord;
void main() {
   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
   ourColor = vec4(aColor, 1.0); // 将ourColor设置为我们从顶点数据那里得到的输入颜色
   // vertexColor = vec4(0.5, 0.0, 0.0, 1.0); // 把输出变量设置为暗红色
    TexCoord = aTexCoord;
}
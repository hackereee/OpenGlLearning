in vec2 myTexcoord;
uniform sampler2D texture1;

void main(){
    gl_FragColor =  texture2D(texture1, myTexcoord);
}
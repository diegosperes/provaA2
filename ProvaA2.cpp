#include <gl/freeglut.h>
#include <math.h>
#include <stdio.h>
#include <iostream> 
#include <ctime> 
#include <cstdlib>
#include "figuras.cpp"

const int   LARGURA = 900;
const int   ALTURA  = 600;
const int   PX      = 10;
const int   PY      = 10;
const int   spacebar= 32;

// Taxa de execucao da funcao de recalculo em milissegundos
const int REFRESH = 30;

// Incremento do angulo quando o usuario pressiona as setas esquerda e direita
float fatorAngulo = 0.02;
float fatorDeslocamento = 1;

// Incremento do angulo quando o usuario pressiona as setas esquerda e direita
float incrAngulo = 0.0;

// Incremento no eixo XZ quando o usuario pressiona as setas para cima ou para baixo
float incrXZ = 0.0;

// Incremento no eixo Y quando o usuario pressiona 'c' ou 'b'
float incrY = 0.0;

// Camera
float eyeX = 0;
float eyeY = 10;
float eyeZ = 50;

// Angulo de rotacao
float angulo = 0.0;

// Deslocamentos iniciais do ponto de observacao em relacao a camera
float dx = -sin(angulo);
float dz = -cos(angulo);

// Ponto de observacao
float obsX = eyeX + dx;
float obsY = eyeY;
float obsZ = eyeZ + dz;

// Rotacao dos elementos da cena
float anguloCubo = 0;
float incrAnguloCubo = 1;

// Mensagem
char *str = "camera (%.2f, %.2f, %.2f)  Angulo: %.2f  Velocidade: %.2f  Farol: Poste: ";

// Escala da fonte
float tamFonte = 0.1;
float incrFonte = 0;

// Largura e altura da viewport
int largura_vp, altura_vp;

float mov, b, angCacamba, d = 0;

bool priPessoa = !priPessoa;

int randX[20], randZ[20], index;


void restauraProjecaoPerspectiva()
{
     // Muda para a matriz do modelo
     glMatrixMode(GL_MODELVIEW);
     
     // Restaura a matriz do modelo 3D
     glPopMatrix();
     
     // Muda para matriz de projecao
	glMatrixMode(GL_PROJECTION);
	
	// Restaura a matriz de projecao 3D
	glPopMatrix();

	// Volta para a matriz do modelo
	glMatrixMode(GL_MODELVIEW);
}

void iniciaProjecaoOrtografica2D() {
    
	// Muda para a matriz de projecao
	glMatrixMode(GL_PROJECTION);

	// Salva a matriz de projecao, pois nesse momento ela esta sendo usada para projetar nosso modelo 3D para a viewport (2D)
	glPushMatrix();

	// Agora que a matriz esta salva podemos limpa-la 
	glLoadIdentity();

	// Altera a projecao para ortografica 2D, onde cada ponto do plano corresponde a um pixel da viewport
	gluOrtho2D(0, largura_vp, altura_vp, 0);

	// Volta para a matriz do modelo
	glMatrixMode(GL_MODELVIEW);
	
	// Salva a matriz do modelo 3D
	glPushMatrix();
	
	// Agora que a matriz esta salva podemos limpa-la 
	glLoadIdentity();
}

void estrada(void){
	// Desenha a rua
   glBegin(GL_POLYGON);
   glColor3f(0.5f, 0.5f, 0.5f);
   glVertex3i(-10, 0, -1000);
   glVertex3i(-10, 0,  1000);
   glVertex3i( 10, 0,  1000);
   glVertex3i( 10, 0, -1000);
   glEnd();
}

void calcada(void){
	 // Desenha a calcada esquerdo
   glBegin(GL_POLYGON);
   glColor3f(0.3f, 0.3f, 0.3f);
   glVertex3i(-10, 0, -1000);
   glVertex3i(-15, 0, -1000);
   glVertex3i(-15, 0,  1000);
   glVertex3i(-10, 0,  1000);
   glEnd();
   
   // Desenha a calcada direita
   glBegin(GL_POLYGON);
   glColor3f(0.3f, 0.3f, 0.3f);
   glVertex3i(10, 0, -1000);
   glVertex3i(15, 0, -1000);
   glVertex3i(15, 0,  1000);
   glVertex3i(10, 0,  1000);
   glEnd();
}

void grama(void){
	//Desenha grama esquerda
	glBegin(GL_POLYGON);
	glColor3f(0.0f, 0.8f, 0.0f);
	glVertex3i(-15, 0, -1000);
	glVertex3i(-215, 0, -1000);
	glVertex3i(-215, 0, 1000);
	glVertex3i(-15, 0, 1000);
	glEnd();
	
	//Desenha grama direita
	glBegin(GL_POLYGON);
	glColor3f(0.0f, 0.8f, 0.0f);
	glVertex3i(15, 0, -1000);
	glVertex3i(215, 0, -1000);
	glVertex3i(215, 0, 1000);
	glVertex3i(15, 0, 1000);
	glEnd();
}

void chassi(void){
	//Desenha chassi caminhao
	glBegin(GL_POLYGON);
	glColor3f(0.8f, 0.8f, 0.0f);
	glVertex3i(6, 2, 0);
	glVertex3i(-6, 2, 0);
	glVertex3i(-6, 2, 20);
	glVertex3i(-4, 2, 20);
	glVertex3i(-4, 2, 28);
	glVertex3i(4, 2, 28);
	glVertex3i(4, 2, 20);
	glVertex3i(6, 2, 20);
	glEnd();
}

void eixo(void){
	glColor3f(1.0f, 0.0f, 0.0f);
	glPushMatrix();
	glRotatef(90, 0, 1, 0);
	drawCylinder(1,16,11,0.4);
	glPopMatrix();
}

void roda(void){
	glColor3i(1.0f, 1.0f, 1.0f);
	glPushMatrix();
	glRotatef(90, 0, 1, 0);
	glutWireTorus(1, 2, 20, 16);
	glPopMatrix();
}

void cabineEsq1(void){
	//Desenha Cabine lado esquerdo Parte 1
	glBegin(GL_POLYGON);
	glColor3f(0.8f, 0.5f, 0.0f);
	glVertex3i(-1, 8, -5);
	glVertex3i(-1, 3, -5);
	glVertex3i(-1, 3, 4);
	glVertex3i(-1, 8, 4);
	glEnd();
	
	//Desenha Cabine lado esquerdo Parte 2
	glBegin(GL_POLYGON);
	glVertex3i(-1, 12, 3);
	glVertex3i(-1, 8, 3);
	glVertex3i(-1, 8, 4);
	glVertex3i(-1, 12, 4);
	glEnd();
		
	//Desenha Cabine lado esquerdo Parte 3
	glBegin(GL_POLYGON);
	glVertex3i(-1, 13, -1);
	glVertex3i(-1, 12, -1);
	glVertex3i(-1, 12, 4);
	glVertex3i(-1, 13, 4);
	glEnd();
	
	//Desenha Cabine lado esquerdo Parte 4
	glBegin(GL_TRIANGLES);
	glVertex3i(-1, 13, -1);
	glVertex3i(-1, 8, -5);
	glVertex3i(-1, 8, -1);
	glEnd();
}

void cabineAtras(void){
	glBegin(GL_POLYGON);
	glColor3f(0.7f, 0.4f, 0.0f);
	glVertex3i(2.5, 13, 4);
	glVertex3i(2.5, 3, 4);
	glVertex3i(-4, 3, 4);
	glVertex3i(-4, 13, 4);
	glEnd();
}

void cabineDir(void){
	//Desenha Cabine lado esquerdo Parte 1
	glBegin(GL_POLYGON);
	glColor3f(0.8f, 0.5f, 0.0f);
	glVertex3i(-1, 8, -5);
	glVertex3i(-1, 3, -5);
	glVertex3i(-1, 3, 4);
	glVertex3i(-1, 8, 4);
	glEnd();
	
	//Desenha Cabine lado esquerdo Parte 2
	glBegin(GL_POLYGON);
	glVertex3i(-1, 12, 3);
	glVertex3i(-1, 8, 3);
	glVertex3i(-1, 8, 4);
	glVertex3i(-1, 12, 4);
	glEnd();
		
	//Desenha Cabine lado esquerdo Parte 3
	glBegin(GL_POLYGON);
	glVertex3i(-1, 13, -1);
	glVertex3i(-1, 12, -1);
	glVertex3i(-1, 12, 4);
	glVertex3i(-1, 13, 4);
	glEnd();
	
	//Desenha Cabine lado esquerdo Parte 4
	glBegin(GL_TRIANGLES);
	glVertex3i(-1, 13, -1);
	glVertex3i(-1, 8, -5);
	glVertex3i(-1, 8, -1);
	glEnd();
}

void cabineTeto(void){
	//Desenha Cabine Teto
	glBegin(GL_POLYGON);
	glColor3f(0.2f, 0.2f, 0.2f);
	glVertex3i(-4, 13, 5);
	glVertex3i(2, 13, 5);
	glVertex3i(2, 13, 0);
	glVertex3i(-4, 13, 0);
	glEnd();
}

void cabineFrente(void){
	//Desenha Cabine Frente
	glBegin(GL_POLYGON);
	glColor3f(0.7f, 0.4f, 0.0f);
	glVertex3i(2, 6, -5);
	glVertex3i(2, 1, -5);
	glVertex3i(-4, 1, -5);
	glVertex3i(-4, 6, -5);
	glEnd();
}

void cabineBaixo(void){
	//Desenha Cabine Baixo
	glBegin(GL_POLYGON);
	glColor3f(0.2f, 0.2f, 0.2f);
	glVertex3i(-4, 13, 5);
	glVertex3i(2, 13, 5);
	glVertex3i(2, 13, -5);
	glVertex3i(-4, 13, -5);
	glEnd();
}

void farol(void){
	//Desenha Farol
	glutSolidSphere(1, 32, 32);
}

void cacamba(void){
	//Desenha cacamba esquerda
	glBegin(GL_POLYGON);
	glVertex3i(-4, 13, 4);
	glVertex3i(-4, 3, 4);
	glVertex3i(-4, 3, 25);
	glVertex3i(-4, 13, 30);
	glEnd();
	
	//Desenha cacamba atr�s
	glBegin(GL_POLYGON);
	glVertex3i(-4, 13, 30);
	glVertex3i(-4, 3, 25);
	glVertex3i(4, 3, 25);
	glVertex3i(4, 13, 30);
	glEnd();
	
	//Desenha cacamba direita
	glBegin(GL_POLYGON);
	glVertex3i(4, 13, 30);
	glVertex3i(4, 3, 25);
	glVertex3i(4, 3, 4);
	glVertex3i(4, 13, 4);
	glEnd();
	
	//Desenha cacamba frente
	glBegin(GL_POLYGON);
	glVertex3i(4, 13, 4);
	glVertex3i(4, 3, 4);
	glVertex3i(-4, 3, 4);
	glVertex3i(-4, 13, 4);
	glEnd();
	
	//Desenha cacamba teto
	glBegin(GL_POLYGON);
	glVertex3i(4, 13, 4);
	glVertex3i(4, 13, 2);
	glVertex3i(-4, 13, 2);
	glVertex3i(-4, 13, 4);
	glEnd();
	
	//Desenha cacamba baixo
	glBegin(GL_POLYGON);
	glColor3i(1, 1, 1);
	glVertex3i(-4, 3, 4);
	glVertex3i(-4, 3, 25);
	glVertex3i(4, 3, 25);
	glVertex3i(4, 3, 4);
	glEnd();
}

void caminhao(void){
	float a = (180.0 * mov) / (M_PI * 3);
	
	glPushMatrix();
	glTranslatef(0, 1, 0);
	chassi();
	glPopMatrix();
	
	//Eixo da frente
	glPushMatrix();
	glTranslatef(0, 3, -4);
	eixo();
	glPopMatrix();
	
	//Eixo de tras
	glPushMatrix();
	glTranslatef(0, 3, 23.5);
	eixo();
	glPopMatrix();
	
	//Roda frente esquerda
	glPushMatrix();
	glTranslatef(-5, 3, -4);
	glRotatef(-a, 1, 0, 0);    
	roda();
	glPopMatrix();
	
	//Roda frente direita
	glPushMatrix();
	glTranslatef(5, 3, -4);
	glRotatef(-a, 1, 0, 0);    
	roda();
	glPopMatrix();
	
	//Roda tras esquerda
	glPushMatrix();
	glTranslatef(-5, 3, 23.5);
	glRotatef(-a, 1, 0, 0);    
	roda();
	glPopMatrix();
	
	//Roda tras direita
	glPushMatrix();
	glTranslatef(5, 3, 23.5);
	glRotatef(-a, 1, 0, 0);    
	roda();
	glPopMatrix();
	
	//Cabine esquerdo
	glPushMatrix();
	glTranslatef(-2, 0, -3);
	cabineEsq1();
	glPopMatrix();
	
	//Cabine atras
	glPushMatrix();
	glTranslatef(1, 0, -3);
	cabineAtras();
	glPopMatrix();
	
	//Cabine Direito
	glPushMatrix();
	glTranslatef(4, 0, -3);
	cabineDir();
	glPopMatrix();
	
	//Cabine Teto
	glPushMatrix();
	glTranslatef(1, 0, -4);
	cabineTeto();
	glPopMatrix();
	
	//Cabine frente
	glPushMatrix();
	glTranslatef(1, 2, -3);
	cabineFrente();
	glPopMatrix();
	
	//Cabine Teto
	glPushMatrix();
	glTranslatef(1, -9, -4);
	cabineTeto();
	glPopMatrix();
	
	//Farol Esquerdo
	glPushMatrix();
	glTranslatef(-1.5, 5, -7.5);
	farol();
	glPopMatrix();
	
	//Farol Direito
	glPushMatrix();
	glTranslatef(1.5, 5, -7.5);
	farol();
	glPopMatrix();
	
	//Ca�amba
	glPushMatrix();
	glTranslatef(0, 0, 25);
	glRotatef(angCacamba, 1, 0, 0);
	glTranslatef(0, 0, -25);
	cacamba();
	glPopMatrix();
	
}

void arvore(void){
	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	glColor3i(1, 0, 0);
	glutSolidCone(2, 5, 32, 32);
	glPopMatrix();
}

void desenha(void){
  
  	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  	
    // Limpa a matriz de transformacao GL_MODEL_VIEW
   glLoadIdentity();
   
   // Posiciona a camera
   gluLookAt(eyeX, eyeY, eyeZ, 
             obsX, obsY, obsZ,
             0.0, 1.0, 0.0);

	glEnable(GL_DEPTH_TEST);
	
	estrada();
	calcada();
	grama();
	
	
	
	
	// Define o deslocamento do caminh�o ao longo do eixo Z
    glPushMatrix();
    glTranslatef(0, 0, mov);	
	caminhao();	
	glPopMatrix();
	
	glPushMatrix();
	for(index=0; index<20; index++){
		glTranslatef(randX[index], 0, randZ[index]);
		arvore();
	}
	glPopMatrix();
	
	// Muda para projecao em 2D
   iniciaProjecaoOrtografica2D();

   // Renderizacao da mensagem em 2D
   char buffer[500];
   sprintf(buffer, str, eyeX, eyeY, eyeZ, angulo, tamFonte);
    
   int tam    = glutBitmapLength(GLUT_BITMAP_HELVETICA_18, (const unsigned char *) buffer);
   int textoX = (largura_vp - tam)/2;
   int textoY =  glutBitmapHeight(GLUT_BITMAP_HELVETICA_18);
    
   // Se posicao < ZERO entao o OpenGL nao renderiza !
   if (textoX < 0)
     textoX = 0;
	
	glColor3f(0,0,1);
   glRasterPos2i(textoX, textoY);
   glutBitmapString(GLUT_BITMAP_HELVETICA_18, (const unsigned char *) buffer);

   glBegin(GL_QUADS);
   glColor3f(1,1,1);
   glVertex2f(textoX, textoY);
   glVertex2f(textoX+tam, textoY);
   glVertex2f(textoX+tam, 0);
   glVertex2f(textoX, 0);
   glEnd();
   
   
   // Volta para a projecao 3D
   restauraProjecaoPerspectiva();
	
	
	 // Como estamos usando um buffer duplo, devemos informar que a renderizacao terminou
   glutSwapBuffers();
}

void redimensiona(int largura, int altura)
{
    // Armazena a largura e altura da viewport depois que o usuario redimensiona a janela
   largura_vp = largura;
   altura_vp = altura;
   
   glViewport(0, 0, largura, altura); 
   
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   
   gluPerspective(60, (float) largura / altura, 1, 2000);
    
   glMatrixMode(GL_MODELVIEW);
}

void teclado(unsigned char ch, int x, int y)
{
     if (ch == 'c') {
       incrY = 1;
     }
     else if (ch == 'b') {
       incrY = -1;
     }     
     else if (ch == 't') {
     	b -= 1;
	 }
	 else if (ch == 'f') {
	 	b += 1;
	 }
	 else if (ch == 'a') {
	 	d -= 0.5;
	 }
	 else if (ch == 's') {
	 	d += 0.5;
	 }
}

void liberaTeclado(unsigned char ch, int x, int y)
{
     if (ch == 'c' || ch == 'b')
       incrY = 0;
     else if (ch == 'f' || ch == 't')
     	b = 0;
    else if (ch == 'a' || ch =='s')
    	d = 0;
}

void tecladoEspecial(int tecla, int x, int y)
{    
     if (tecla == GLUT_KEY_RIGHT)
       incrAngulo = -fatorAngulo;
     else if (tecla == GLUT_KEY_LEFT)
       incrAngulo = fatorAngulo;
     
     if (tecla == GLUT_KEY_UP)
       incrXZ = fatorDeslocamento;
     else if (tecla == GLUT_KEY_DOWN)
       incrXZ = -fatorDeslocamento;
}

void liberaTecladoEspecial(int tecla, int x, int y)
{
     if (tecla == GLUT_KEY_RIGHT || tecla == GLUT_KEY_LEFT)
       incrAngulo = 0;
     
     if (tecla == GLUT_KEY_UP || tecla == GLUT_KEY_DOWN)
       incrXZ = 0;
}

void recalculaCena(int valor)
{    
     if (incrAngulo != 0)
     {
       angulo = angulo + incrAngulo;
       dx = -sin(angulo);
       dz = -cos(angulo);
       obsX = eyeX + dx;
       obsZ = eyeZ + dz;
     }
     
     if (incrXZ != 0)
     {
       eyeX = eyeX + incrXZ * dx;
       eyeZ = eyeZ + incrXZ * dz;
       obsX = eyeX + dx;
       obsZ = eyeZ + dz;
     }
     
     if (incrY != 0)
     {
     	if (eyeY < 1){
     		eyeY = 1;
		 }
		 else{
			eyeY = eyeY + incrY;	 	
		 }
       
       obsY = eyeY;
     }
     
     if (b != 0)
     	mov += b;
     	
    if (d != 0)
    	if (angCacamba < 0)
    		angCacamba = 0;
    	else if (angCacamba > 30)
    		angCacamba = 30;
    	else
    		angCacamba += d;

     
     anguloCubo += incrAnguloCubo;
     
     if (anguloCubo > 360)
        anguloCubo -= 360;
     
     glutPostRedisplay();
        
     glutTimerFunc(REFRESH, recalculaCena, 0);
}

void inicializa(void) 
{
     // Cor de fundo sera preta
   glClearColor (0.0, 0.0, 0.0, 0.0);
   
   //Arvores esquerda
	srand((unsigned)time(0)); 
    int random_integer; 
    int lowestX=-50, highestX=-150, lowestZ=-150, highestZ=150; 
    rangeX=(highestX-lowestX)+1, rangeZ=(highestZ-lowestZ)+1; 
    for(int index=0; index<20; index++){ 
        randX[index] = lowestX+int(rangeX*rand()/(RAND_MAX + 1.0)); 
        randZ[index] = lowestZ+int(rangeZ*rand()/(RAND_MAX + 1.0));  
    } 

}

int main(int argc, char* argv[])
{
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize(LARGURA, ALTURA); 
   glutInitWindowPosition(PX, PY);
   glutCreateWindow("Prova A2");
   inicializa();
   glutDisplayFunc(desenha); 
   glutReshapeFunc(redimensiona);
   glutKeyboardFunc(teclado);
   glutKeyboardUpFunc(liberaTeclado);
   glutSpecialFunc(tecladoEspecial);
   glutSpecialUpFunc(liberaTecladoEspecial);
   glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);
   glutTimerFunc(REFRESH, recalculaCena, 0);
   glutMainLoop();
}

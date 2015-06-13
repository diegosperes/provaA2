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
char *str = "Camera (%.2f, %.2f, %.2f)  Angulo: %.2f  Velocidade: %d  Farol: %c Poste: %c";

// Escala da fonte
float tamFonte = 0.1;
float incrFonte = 0;

//Velocidade de movimentacao
int velocidade = 1;

// Largura e altura da viewport
int largura_vp, altura_vp;

float mov, b, angCacamba, d = 0;

bool priPessoa = !priPessoa;

int i;

// Variaveis de luz
bool farolLuz = true;
bool posteLuz = true;

char posteChar = 'L';
char farolChar = 'L';

float posicaoLuz1[]  = {-1.5, 5, -7.5, 1};
float posicaoLuz2[] = {1.5, 5, -7.5, 1};
float posicaoLuz3[] = {3, 20, 0, 1};
float posicaoLuz4[] = {3, 20, -100, 1};
float posicaoLuz5[] = {3, 20, -200, 1};
float posicaoLuz6[] = {3, 20, -300, 1};
float posicaoLuz7[] = {3, 20, -400, 1};
float posicaoLuz8[] = {3, 20, -500, 1};
float direcaoSpot0[] = { 0, 0, -1};

float corEstrada[] = {0.5, 0.5, 0.5, 1.0};
float corCalcada[] = {0.3, 0.3, 0.3, 1.0};
float corGrama[] = {117/255.0, 155/255.0, 63/255.0, 1.0};
float corCeu[] = {123.0/255, 217.0/255, 234.0, 1.0};
float corCopa[] = {0, 1, 0, 1.0};
float corCaule[] = {128.0/255, 64.0/255, 0, 1.0};
float corPreto[] = {0, 0, 0, 1.0};
float corVermelho[] = {1, 0, 0, 1.0};
float corChassi[] = {157.0/255, 150.0/255, 52.0/255, 1.0};
float corFarolOn[] = {1, 1, 0, 1.0};
float corFarolOff[] = {0.5, 0.5, 0.5, 1.0};
float corPoste[] = {0.8, 0.8, 0.8, 1.0};
float corLuminaria[] = {0.8, 0.8, 0, 1.0};
float corCacamba[] = {255/255.0, 128/255.0, 0, 1.0};

float luzAmbiente[] = {0.3, 0.3, 0.3, 1.0}; 
float conSol = 1.0;
float luzSolAmbiente[] = {0.3, 0.3, 0.3, 1.0};
float luzSolDifusa[] = {conSol, conSol, conSol, 1.0};
float posicaoLuz0[] = {150.0, 150.0, -150.0, 0.0};
float desligado[]    = { 0, 0, 0, 0 };
float branco[]    = { 1, 1, 1, 1 };


void calculaNormal(float vertice1[], // (x1, y1, z1) do vertice 1
                   float vertice2[], // (x2, y2, z2) do vertice 2
                   float vertice3[], // (x3, y3, z3) do vertice 3
                   float normal[])   // (xn, yn, zn) da normal
{
  float Qx = vertice2[0] - vertice1[0];  // x2 – x1
  float Qy = vertice2[1] - vertice1[1];  // y2 – y1
  float Qz = vertice2[2] - vertice1[2];  // z2 – z1
  float Px = vertice3[0] - vertice2[0];  // x3 – x2
  float Py = vertice3[1] - vertice2[1];  // y3 – y2
  float Pz = vertice3[2] - vertice2[2];  // z3 – z2

  normal[0] = Qy*Pz - Qz*Py;
  normal[1] = Qz*Px - Qx*Pz; 
  normal[2] = Qx*Py - Qy*Px; 
}

void normaliza(float vetor[],       // (xn, yn, zn) não normalizado
               float normalizado[]) // (xn, yn, zn) normalizado
{
  float c = sqrt(vetor[0]*vetor[0] +  // xn * xn +
                 vetor[1]*vetor[1] +  // yn * yn +
                 vetor[2]*vetor[2]);  // zn * zn

  normalizado[0] = vetor[0] / c;
  normalizado[1] = vetor[1] / c;
  normalizado[2] = vetor[2] / c;
}

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
	//glColor3f(0.5f, 0.5f, 0.5f);
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, corEstrada);
	for (i=-600; i<=600; i+=10){
		glBegin(GL_POLYGON);
		glNormal3f(0, 1, 0);
		glVertex3i(-10, 0, i);
		glVertex3i(-10, 0, i+10);
		glVertex3i(10, 0, i+10);
		glVertex3i(10, 0, i);
		glEnd();
	}
}

void calcada(void){
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, corCalcada);
	for (i=-600; i<=600; i+= 10){
		// Desenha a calcada esquerdo
   		glBegin(GL_POLYGON);
   		glNormal3f(0, 1, 0);
   		glVertex3i(-15, 0, i);
   		glVertex3i(-15, 0, i+10);
   		glVertex3i(-10, 0,  i+10);
   		glVertex3i(-10, 0,  i);
   		glEnd();
   
    	// Desenha a calcada direita
   		glBegin(GL_POLYGON);
   		glNormal3f(0, 1, 0);
   		glVertex3i(10, 0, i);
   		glVertex3i(10, 0, i+10);
   		glVertex3i(15, 0,  i+10);
   		glVertex3i(15, 0,  i);
   		glEnd();
	}
}

void grama(void){
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, corGrama);
 	//Desenha grama esquerda
	glBegin(GL_POLYGON);
	glNormal3f(0, 1, 0);
	glVertex3i(-215, 0, -600);
	glVertex3i(-215, 0, 600);
	glVertex3i(-15, 0, 600);
	glVertex3i(-15, 0, -600);
	glEnd();
	
	//Desenha grama direita
	glBegin(GL_POLYGON);
	glNormal3f(0, 1, 0);
	glVertex3i(15, 0, -600);
	glVertex3i(15, 0, 600);
	glVertex3i(215, 0, 600);
	glVertex3i(215, 0, -600);
	glEnd();
}

void ceu(void){
	glMaterialfv(GL_FRONT_AND_BACK , GL_AMBIENT_AND_DIFFUSE, corCeu);
	glPushMatrix();
	//glColor3f(0.3f, 0.3f, 0.8f);
	glTranslatef(0, -300, 0);
	glutSolidSphere(650, 256, 256);
	glPopMatrix();
}

void chassi(void){
	glMaterialfv(GL_FRONT_AND_BACK , GL_AMBIENT_AND_DIFFUSE, corChassi);
	//Desenha chassi caminhao
	glBegin(GL_POLYGON);
	glNormal3f(0, 1, 0);
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
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, corVermelho);
	glPushMatrix();
	glRotatef(90, 0, 1, 0);
	drawCylinder(1,16,11,0.4);
	glPopMatrix();
}

void roda(void){
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, corPreto);
	glPushMatrix();
	glRotatef(90, 0, 1, 0);
	glutWireTorus(1, 2, 20, 16);
	glPopMatrix();
}

void cabineEsq1(void){
	glMaterialfv(GL_FRONT_AND_BACK , GL_AMBIENT_AND_DIFFUSE, corChassi);
	//Desenha Cabine lado esquerdo Parte 1
	glBegin(GL_POLYGON);
	glNormal3f(-1, 0, 0);
	glVertex3i(-1, 8, -5);
	glVertex3i(-1, 3, -5);
	glVertex3i(-1, 3, 4);
	glVertex3i(-1, 8, 4);
	glEnd();
	
	//Desenha Cabine lado esquerdo Parte 2
	glBegin(GL_POLYGON);
	glNormal3f(-1, 0, 0);
	glVertex3i(-1, 12, 3);
	glVertex3i(-1, 8, 3);
	glVertex3i(-1, 8, 4);
	glVertex3i(-1, 12, 4);
	glEnd();
		
	//Desenha Cabine lado esquerdo Parte 3
	glBegin(GL_POLYGON);
	glNormal3f(-1, 0, 0);
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
	glMaterialfv(GL_FRONT_AND_BACK , GL_AMBIENT_AND_DIFFUSE, corChassi);
	glBegin(GL_POLYGON);
	glNormal3f(0, 0, 1);
	glVertex3i(2.5, 13, 4);
	glVertex3i(2.5, 3, 4);
	glVertex3i(-4, 3, 4);
	glVertex3i(-4, 13, 4);
	glEnd();
}

void cabineDir(void){
	glMaterialfv(GL_FRONT_AND_BACK , GL_AMBIENT_AND_DIFFUSE, corChassi);
	//Desenha Cabine lado esquerdo Parte 1
	glBegin(GL_POLYGON);
	glNormal3f(1, 0, 0);
	glVertex3i(-1, 8, -5);
	glVertex3i(-1, 3, -5);
	glVertex3i(-1, 3, 4);
	glVertex3i(-1, 8, 4);
	glEnd();
	
	//Desenha Cabine lado esquerdo Parte 2
	glBegin(GL_POLYGON);
	glNormal3f(1, 0, 0);
	glVertex3i(-1, 12, 3);
	glVertex3i(-1, 8, 3);
	glVertex3i(-1, 8, 4);
	glVertex3i(-1, 12, 4);
	glEnd();
		
	//Desenha Cabine lado esquerdo Parte 3
	glBegin(GL_POLYGON);
	glNormal3f(1, 0, 0);
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
	glMaterialfv(GL_FRONT_AND_BACK , GL_AMBIENT_AND_DIFFUSE, corCalcada);
	//Desenha Cabine Teto
	glBegin(GL_POLYGON);
	glNormal3f(0, 1, 0);
	glVertex3i(-4, 13, 5);
	glVertex3i(2, 13, 5);
	glVertex3i(2, 13, 0);
	glVertex3i(-4, 13, 0);
	glEnd();
}

void cabineFrente(void){
	glMaterialfv(GL_FRONT_AND_BACK , GL_AMBIENT_AND_DIFFUSE, corChassi);
	//Desenha Cabine Frente
	glBegin(GL_POLYGON);
	glNormal3f(0, 0, -1);
	glVertex3i(2, 6, -5);
	glVertex3i(2, 1, -5);
	glVertex3i(-4, 1, -5);
	glVertex3i(-4, 6, -5);
	glEnd();
}

void cabineBaixo(void){
	glMaterialfv(GL_FRONT_AND_BACK , GL_AMBIENT_AND_DIFFUSE, corChassi);
	//Desenha Cabine Baixo
	glBegin(GL_POLYGON);
	glNormal3f(0, 1, 0);
	glVertex3i(-4, 13, 5);
	glVertex3i(2, 13, 5);
	glVertex3i(2, 13, -5);
	glVertex3i(-4, 13, -5);
	glEnd();
}

void farol(void){
	//Desenha Farol
	if(farolLuz)
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, corFarolOn);
	else
		glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, corFarolOff);
	//Desenha Farol
	glutSolidSphere(1, 32, 32);
}

void cacamba(void){	
glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, corCacamba);
	glBegin(GL_QUADS);
	//Desenha cacamba esquerda
	glNormal3f(-1, 0, 0);
	glVertex3i(-4, 13, 4);
	glVertex3i(-4, 3, 4);
	glVertex3i(-4, 3, 25);
	glVertex3i(-4, 13, 30);
	
	//Desenha cacamba atrás
	glNormal3f(0, 0, 1);
	glVertex3i(-4, 13, 30);
	glVertex3i(-4, 3, 25);
	glVertex3i(4, 3, 25);
	glVertex3i(4, 13, 30);
	
	//Desenha cacamba direita
	glNormal3f(1, 0, 0);
	glVertex3i(4, 13, 30);
	glVertex3i(4, 3, 25);
	glVertex3i(4, 3, 4);
	glVertex3i(4, 13, 4);
	
	//Desenha cacamba frente
	glNormal3f(0, 0, -1);
	glVertex3i(4, 13, 4);
	glVertex3i(4, 3, 4);
	glVertex3i(-4, 3, 4);
	glVertex3i(-4, 13, 4);
	
	//Desenha cacamba teto
	glNormal3f(0, 1, 0);
	glVertex3i(4, 13, 4);
	glVertex3i(4, 13, 2);
	glVertex3i(-4, 13, 2);
	glVertex3i(-4, 13, 4);
	
	//Desenha cacamba baixo
	glNormal3f(0, 1, 0);
	glVertex3i(-4, 3, 4);
	glVertex3i(-4, 3, 25);
	glVertex3i(4, 3, 25);
	glVertex3i(4, 3, 4);
	glEnd();
}

void pinheiro(){
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, corCopa);
	glRotatef(270, 1, 0, 0);
    glutSolidCone(5, 10, 10, 10);
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, corCaule);
    drawCylinder(6, 16, 10, 0.5);
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
	glTranslatef(-5.5, 3, -4);
	glRotatef(-a, 1, 0, 0);    
	roda();
	glPopMatrix();
	
	//Roda frente direita
	glPushMatrix();
	glTranslatef(5.5, 3, -4);
	glRotatef(-a, 1, 0, 0);    
	roda();
	glPopMatrix();
	
	//Roda tras esquerda
	glPushMatrix();
	glTranslatef(-5.5, 3, 23.5);
	glRotatef(-a, 1, 0, 0);    
	roda();
	glPopMatrix();
	
	//Roda tras direita
	glPushMatrix();
	glTranslatef(5.5, 3, 23.5);
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
	
	//Caçamba
	glPushMatrix();
	glTranslatef(0, 0.5, 25);
	glRotatef(angCacamba, 1, 0, 0);
	glTranslatef(0, 0, -25);
	cacamba();
	glPopMatrix();
	
}

void poste(void){
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, corPoste);
	glPushMatrix();
	glRotatef(-90, 1, 0, 0);
	drawCylinder(16, 16, 50, 1);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(10, 24.4,0);
	glRotatef(-90, 0, 1, 0);
	drawCylinder(16, 16, 20, 1);
	glPopMatrix();
	
	glPushMatrix();
	glTranslatef(20, 23.5, 0);
	glRotatef(90, 1, 0, 0);
	drawCylinder(16, 16, 3, 1);
	glPopMatrix();
	
	glMaterialfv(GL_FRONT_AND_BACK , GL_AMBIENT_AND_DIFFUSE, corLuminaria);
	glPushMatrix();
	glTranslatef(20, 20.5, 0);
	glRotatef(-90, 1, 0, 0);
	drawCone(4, 2, 16, 16);
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
	
	// Habilita a iluminacao
    glEnable(GL_LIGHTING);
    glShadeModel(GL_SMOOTH);
    
    //glEnable(GL_COLOR_MATERIAL);
    
    // Define a luz ambiente
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);
   glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);
   glLightModelf(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
   
   //Luz Sol
   glLightfv(GL_LIGHT0, GL_AMBIENT, luzSolAmbiente);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, luzSolDifusa);
   glLightfv(GL_LIGHT0, GL_POSITION, posicaoLuz0);
   
   glEnable(GL_LIGHT0);
   
   //Luz farol esquerdo
   glLightfv(GL_LIGHT1, GL_AMBIENT, desligado);
   glLightfv(GL_LIGHT1, GL_SPECULAR, desligado);
   glLightfv(GL_LIGHT1, GL_DIFFUSE, branco);
   glLightf( GL_LIGHT1, GL_SPOT_CUTOFF, 30.0);
   glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, direcaoSpot0);
   glLightf(GL_LIGHT1, GL_CONSTANT_ATTENUATION, 1); 
   glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.01); 
   glLightf(GL_LIGHT1, GL_QUADRATIC_ATTENUATION, 0.001);  
   glPushMatrix();
   glTranslatef(0, 0, mov);
   glLightfv(GL_LIGHT1, GL_POSITION, posicaoLuz1);
   glPopMatrix();
    
	//Luz farol direito
   glLightfv(GL_LIGHT2, GL_AMBIENT, desligado);
   glLightfv(GL_LIGHT2, GL_SPECULAR, desligado);
   glLightfv(GL_LIGHT2, GL_DIFFUSE, branco);
   glLightf( GL_LIGHT2, GL_SPOT_CUTOFF, 30.0);
   glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, direcaoSpot0);
   glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 1); 
   glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.01); 
   glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.001);  
   glPushMatrix();
   glTranslatef(0, 0, mov);
   glLightfv(GL_LIGHT2, GL_POSITION, posicaoLuz2);
   glPopMatrix();
   
   //Luz poste 1
   glLightfv(GL_LIGHT3, GL_POSITION, posicaoLuz3);
   glLightfv(GL_LIGHT3, GL_AMBIENT, desligado);
   glLightfv(GL_LIGHT3, GL_DIFFUSE, branco);
   glLightfv(GL_LIGHT3, GL_SPECULAR, desligado);
   glLightf(GL_LIGHT3, GL_CONSTANT_ATTENUATION, 1);
   glLightf(GL_LIGHT3, GL_LINEAR_ATTENUATION, 0.01);
   glLightf(GL_LIGHT3, GL_QUADRATIC_ATTENUATION, 0.001);
   
   
   
   //Luz poste 2
   glLightfv(GL_LIGHT4, GL_POSITION, posicaoLuz4);
   glLightfv(GL_LIGHT4, GL_AMBIENT, desligado);
   glLightfv(GL_LIGHT4, GL_DIFFUSE, branco);
   glLightfv(GL_LIGHT4, GL_SPECULAR, desligado);
   glLightf(GL_LIGHT4, GL_CONSTANT_ATTENUATION, 1);
   glLightf(GL_LIGHT4, GL_LINEAR_ATTENUATION, 0.01);
   glLightf(GL_LIGHT4, GL_QUADRATIC_ATTENUATION, 0.001);
   
  
  
   //Luz poste 3
   glLightfv(GL_LIGHT5, GL_POSITION, posicaoLuz5);
   glLightfv(GL_LIGHT5, GL_AMBIENT, desligado);
   glLightfv(GL_LIGHT5, GL_DIFFUSE, branco);
   glLightfv(GL_LIGHT5, GL_SPECULAR, desligado);
   glLightf(GL_LIGHT5, GL_CONSTANT_ATTENUATION, 1);
   glLightf(GL_LIGHT5, GL_LINEAR_ATTENUATION, 0.01);
   glLightf(GL_LIGHT5, GL_QUADRATIC_ATTENUATION, 0.001);
   
    //Luz poste 4
   glLightfv(GL_LIGHT6, GL_POSITION, posicaoLuz6);
   glLightfv(GL_LIGHT6, GL_AMBIENT, desligado);
   glLightfv(GL_LIGHT6, GL_DIFFUSE, branco);
   glLightfv(GL_LIGHT6, GL_SPECULAR, desligado);
   glLightf(GL_LIGHT6, GL_CONSTANT_ATTENUATION, 1);
   glLightf(GL_LIGHT6, GL_LINEAR_ATTENUATION, 0.01);
   glLightf(GL_LIGHT6, GL_QUADRATIC_ATTENUATION, 0.001);
   
    //Luz poste 5
   glLightfv(GL_LIGHT7, GL_POSITION, posicaoLuz7);
   glLightfv(GL_LIGHT7, GL_AMBIENT, desligado);
   glLightfv(GL_LIGHT7, GL_DIFFUSE, branco);
   glLightfv(GL_LIGHT7, GL_SPECULAR, desligado);
   glLightf(GL_LIGHT7, GL_CONSTANT_ATTENUATION, 1);
   glLightf(GL_LIGHT7, GL_LINEAR_ATTENUATION, 0.01);
   glLightf(GL_LIGHT7, GL_QUADRATIC_ATTENUATION, 0.001);

   
  
   // Liga/desliga luz poste
   if(posteLuz){
   	glEnable(GL_LIGHT3);
   	glEnable(GL_LIGHT4);
   	glEnable(GL_LIGHT5);
   	glEnable(GL_LIGHT6);
   	glEnable(GL_LIGHT7);
   	posteChar = 'L';
   }
   else{
   	glDisable(GL_LIGHT3);
   	glDisable(GL_LIGHT4);
   	glDisable(GL_LIGHT5);
   	glDisable(GL_LIGHT6);
   	glDisable(GL_LIGHT7);
   	posteChar = 'D';
   }
     
   // Liga/desliga luz farol
   if (farolLuz){
   	  glEnable(GL_LIGHT1);
      glEnable(GL_LIGHT2);
      farolChar = 'L';
   }   
   else{
   	  glDisable(GL_LIGHT1);
      glDisable(GL_LIGHT2);
      farolChar = 'D';
   }

	ceu();	
	estrada();
	calcada();
	grama();

	
	// Desenha poste lado esquerdo
	for(i=-1000; i<1000; i+=100){
		glPushMatrix();
		glTranslatef(-17, 0, -i);
		poste();
		glPopMatrix();
	}
	
	// Desenha poste lado direito
	for(i=-550; i<1000; i+=100){
		glPushMatrix();
		glTranslatef(17, 0, -i);
		glRotatef(180, 0, 1, 0);
		poste();
		glPopMatrix();
	}
	
    for (int x=-600; x < 600; x = x+15){
      glPushMatrix();
      glTranslatef(60, 4, -60 + x);
      pinheiro();
      glPopMatrix();
      
      glPushMatrix();
      glTranslatef(-60, 4, -60 + x);
      pinheiro();
      glPopMatrix();      
   }
	
	
	// Define o deslocamento do caminhão ao longo do eixo Z
    glPushMatrix();
    glTranslatef(0, 0, mov);	
	caminhao();	
	glPopMatrix();
	
	glDisable(GL_LIGHTING);
	
	// Muda para projecao em 2D
   iniciaProjecaoOrtografica2D();

   // Renderizacao da mensagem em 2D
   char buffer[500];
   sprintf(buffer, str, eyeX, eyeY, eyeZ, angulo, velocidade, farolChar, posteChar);
    
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
     	b -= velocidade;
	 }
	 else if (ch == 'f') {
	 	b += velocidade;
	 }
	 else if (ch == 'a') {
	 	d -= 0.5;
	 }
	 else if (ch == 's') {
	 	d += 0.5;
	 }
	else if(ch == '1') {
	 	farolLuz = ! farolLuz;
	 	glutPostRedisplay();
	 }
	 else if(ch == '2'){
	 	posteLuz = ! posteLuz;
	 	glutPostRedisplay();
	 }
	 else if(ch == 'l'){
	 	conSol -= 0.1;
	 	glutPostRedisplay();
	 }
	 else if(ch == '-' && velocidade <=9 && velocidade >1 ){
	 	velocidade-=1;
	 }
	 else if(ch == '+' && velocidade >=1 && velocidade <9){
	 	velocidade+=1;
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
     
     if (incrY != 0 && (eyeY + incrY) > 1 && (eyeY + incrY) < 300)
     {
	   eyeY = eyeY + incrY;
       obsY = eyeY;
     }
     
     if (b != 0)
     	mov += b;
     	
    if (d != 0)
    	if ((angCacamba + d) >= 0 && (angCacamba + d) <= 25)
    		angCacamba += d;
     
     glutPostRedisplay();
        
     glutTimerFunc(REFRESH, recalculaCena, 0);
}

void inicializa(void) 
{
     // Cor de fundo sera preta
   glClearColor (0.0, 0.0, 0.0, 0.0);


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

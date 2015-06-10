// ----------------------------------------------------------------------------

// drawRectangle(Xini, Xfim, deltaX, Zini, Zfim, deltaZ, Y, 0,  1, 0)
// drawRectangle(Xini, Xfim, deltaX, Zini, Zfim, deltaZ, Y, 0, -1, 0)
   
// drawRectangle(Xini, Xfim, deltaX, Yini, Yfim, deltaY, Z, 0, 0,  1)
// drawRectangle(Xini, Xfim, deltaX, Yini, Yfim, deltaY, Z, 0, 0, -1)
   
// drawRectangle(Zini, Zfim, deltaZ, Yini, Yfim, deltaY, X,  1, 0, 0)
// drawRectangle(Zini, Zfim, deltaZ, Yini, Yfim, deltaY, X, -1, 0, 0)

// ----------------------------------------------------------------------------
 
void drawRectangle(GLfloat iniP1, GLfloat fimP1, GLfloat fatorP1, 
                   GLfloat iniP2, GLfloat fimP2, GLfloat fatorP2, 
                   GLfloat posP3, 
                   GLint nx, GLint ny, GLint nz)
{
     GLint ordem[3];
     GLfloat i, j, vertices[4][3];
     
     if (nz != 0) {
       ordem[0] = 0;
       ordem[1] = 1;
       ordem[2] = 2;
     }
     else if (ny != 0) {
       ordem[0] = 0;
       ordem[1] = 2;
       ordem[2] = 1;
     }
     else if (nx != 0) {
       ordem[0] = 1;
       ordem[1] = 2;
       ordem[2] = 0;
       float aux;
       aux = iniP1;
       iniP1 = iniP2;
       iniP2 = aux;
       aux = fimP1;
       fimP1 = fimP2;
       fimP2 = aux;
       aux = fatorP1;
       fatorP1 = fatorP2;
       fatorP2 = aux;
     }
     
     glBegin(GL_QUADS);
     for(i = iniP1; i < fimP1; i += fatorP1)
       for(j = iniP2; j < fimP2; j += fatorP2) {
             GLfloat fator_i = i + fatorP1 > fimP1 ? fimP1-i : fatorP1;
             GLfloat fator_j = j + fatorP2 > fimP2 ? fimP2-j : fatorP2;
             vertices[0][ordem[0]] = i;
             vertices[0][ordem[1]] = j;
             vertices[0][ordem[2]] = posP3;
             vertices[1][ordem[0]] = i + fator_i;
             vertices[1][ordem[1]] = j;
             vertices[1][ordem[2]] = posP3;
             vertices[2][ordem[0]] = i + fator_i;
             vertices[2][ordem[1]] = j + fator_j;
             vertices[2][ordem[2]] = posP3;
             vertices[3][ordem[0]] = i;
             vertices[3][ordem[1]] = j + fator_j;
             vertices[3][ordem[2]] = posP3;


             glNormal3f(nx, ny, nz);
             glVertex3fv(&vertices[3][0]);
             glVertex3fv(&vertices[2][0]);
             glVertex3fv(&vertices[1][0]);
             glVertex3fv(&vertices[0][0]);
       }
       glEnd();
}

//----------------------------------------------------------------------
// numMajor: número de secoes na vertical
// numMinor: número de secoes na circunferencia
// height: altura
// radius: raio
//----------------------------------------------------------------------

void drawCylinder(int numMajor, int numMinor, GLdouble height, GLdouble radius)
{
  double majorStep = height / numMajor;
  double minorStep = 2.0 * M_PI / numMinor;
  int i, j;

  for (i = 0; i < numMajor; ++i) {
    GLfloat z0 = 0.5 * height - i * majorStep;
    GLfloat z1 = z0 - majorStep;

    glBegin(GL_TRIANGLE_STRIP);

    for (j = 0; j <= numMinor; ++j) {
      double a = j * minorStep;
      GLfloat x = radius * cos(a);
      GLfloat y = radius * sin(a);
      
      glNormal3f(x / radius, y / radius, 0.0);
      glTexCoord2f(j / (GLfloat) numMinor, i / (GLfloat) numMajor);
      glVertex3f(x, y, z0);

      glNormal3f(x / radius, y / radius, 0.0);
      glTexCoord2f(j / (GLfloat) numMinor, (i + 1) / (GLfloat) numMajor);
      glVertex3f(x, y, z1);
    }
    glEnd();
  }
}

void drawSolidCylinder(GLdouble height, GLdouble radius, int numMajor, int numMinor)
{
	drawCylinder(numMajor, numMinor, height, radius);
}

void drawWireCylinder(GLdouble height, GLdouble radius, int numMajor, int numMinor)
{
  double majorStep = height / numMajor;
  double minorStep = 2.0 * M_PI / numMinor;
  int i, j;

  for (i = 0; i < numMajor; ++i) {
    GLfloat z0 = 0.5 * height - i * majorStep;
    GLfloat z1 = z0 - majorStep;

    for (j = 0; j+1 <= numMinor; ++j) {
      double a   = j * minorStep;
      double a2  = (j+1) * minorStep;
      GLfloat x  = radius * cos(a);
      GLfloat y  = radius * sin(a);
      GLfloat x2 = radius * cos(a2);
      GLfloat y2 = radius * sin(a2);
      
      glBegin(GL_LINE_STRIP);
      glVertex3f(x2, y2, z0);
      glVertex3f(x, y, z0);
      glVertex3f(x, y, z1);
      glVertex3f(x2, y2, z1);
      glEnd();
    }
  }
}

//-----------------------------------------------------------------------------
// Desenha uma esfera
// radius: raio
// slices: secoes na longitude
// stacks: secoes na latitude
// ----------------------------------------------------------------------------

void drawSphere(GLdouble radius, GLint slices, GLint stacks) {
  
  GLUquadricObj *sphere = NULL;
  
  sphere = gluNewQuadric();
  gluQuadricDrawStyle(sphere, GLU_FILL);
  gluQuadricTexture(sphere, TRUE);
  gluQuadricNormals(sphere, GLU_SMOOTH);
  gluSphere(sphere, radius, slices, stacks);
  gluDeleteQuadric(sphere);
}

//-----------------------------------------------------------------------------
// Desenha um cilindro com base e topo de raio diferentes (secao do cone)
// base: raio da base
// top: raio do topo
// height: altura
// slices: secoes na longitude
// stacks: secoes na latitude
// ----------------------------------------------------------------------------

void drawCylinder(GLdouble base, GLdouble top, GLdouble height, GLint slices, GLint stacks) {
  
  GLUquadricObj *cylinder = NULL;
  
  cylinder = gluNewQuadric();
  gluQuadricDrawStyle(cylinder, GLU_FILL);
  gluQuadricTexture(cylinder, TRUE);
  gluQuadricNormals(cylinder, GLU_SMOOTH);
  gluCylinder(cylinder, base, top, height, slices, stacks);
  gluDeleteQuadric(cylinder);
}


//-----------------------------------------------------------------------------
// Desenha um cone
// base: raio da base
// height: altura
// slices: secoes na longitude
// stacks: secoes na latitude
// ----------------------------------------------------------------------------

void drawCone(GLdouble base, GLdouble height, GLint slices, GLint stacks) {
  
  drawCylinder(base, 0, height, slices, stacks);
}

//-----------------------------------------------------------------------------
// Desenha um cubo combinando com a textura definida
// size: lado do cubo
// minx: coordenada X minima da textura
// maxx: coordenada X maxima da textura
// miny: coordenada Y minima da textura
// maxy: coordenada Y maxima da textura
//-----------------------------------------------------------------------------

void drawBox(GLfloat size, GLfloat minx, GLfloat maxx, GLfloat miny, GLfloat maxy)
{
  static GLfloat n[6][3] =
  {
    {-1.0, 0.0, 0.0},
    {0.0, 1.0, 0.0},
    {1.0, 0.0, 0.0},
    {0.0, -1.0, 0.0},
    {0.0, 0.0, 1.0},
    {0.0, 0.0, -1.0}
  };
  static GLint faces[6][4] =
  {
    {0, 1, 2, 3},
    {2, 6, 7, 3},
    {5, 4, 7, 6},
    {0, 4, 5, 1},
    {1, 5, 6, 2},
    {4, 0, 3, 7}
  };
  GLfloat v[8][3];
  GLfloat c[4][2];
  GLint i;

  v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size / 2;
  v[4][0] = v[5][0] = v[6][0] = v[7][0] = size / 2;
  v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size / 2;
  v[2][1] = v[3][1] = v[6][1] = v[7][1] = size / 2;
  v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size / 2;
  v[1][2] = v[2][2] = v[5][2] = v[6][2] = size / 2;
  
  c[0][0] = minx;
  c[0][1] = miny;
  c[1][0] = maxx;
  c[1][1] = miny;
  c[2][0] = maxx;
  c[2][1] = maxy;
  c[3][0] = minx;
  c[3][1] = maxy;

  for (i = 5; i >= 0; i--) {
    glBegin(GL_QUADS);
    glNormal3fv(&n[i][0]);
    glTexCoord2f(c[0][0], c[0][1]);
    glVertex3fv(&v[faces[i][0]][0]);
    glTexCoord2f(c[1][0], c[1][1]);
    glVertex3fv(&v[faces[i][1]][0]);
    glTexCoord2f(c[2][0], c[2][1]);
    glVertex3fv(&v[faces[i][2]][0]);
    glTexCoord2f(c[3][0], c[3][1]);
    glVertex3fv(&v[faces[i][3]][0]);
    glEnd();
  }
}

//-----------------------------------------------------------------------------
// Desenha cubo e assum3 (0, 1) como coordenadas da textura em X e Y
// size: lado do cubo
// ----------------------------------------------------------------------------

void drawBox(GLfloat size)
{
     drawBox(size, 0, 1, 0, 1);
}

//-----------------------------------------------------------------------------
     


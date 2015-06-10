//--------------------------------------------------------------------------
// Estrutura para armazenamento da imagem e de algumas informacoes uteis
//--------------------------------------------------------------------------

struct Image {
 ILubyte *rawData;
 ILuint  id;
 ILint   width;
 ILint   height;
 ILuint  depth;
 ILuint  format;
};

//--------------------------------------------------------------------------
// Carrega uma imagem para a memoria
//--------------------------------------------------------------------------

ILuint LoadImage(char *filename, Image *image)
{
    ILuint newImageID;
    
    ilGenImages(1, &newImageID);
    ilBindImage(newImageID);
    
    if (!ilLoadImage(filename)) 
       return 0; 

    ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);

    image->id      = newImageID;
    image->rawData = ilGetData(); 
    image->width   = ilGetInteger(IL_IMAGE_WIDTH);
    image->height  = ilGetInteger(IL_IMAGE_HEIGHT);
    image->depth   = ilGetInteger(IL_IMAGE_DEPTH);
    image->format   = ilGetInteger(IL_IMAGE_FORMAT);
    
    return 1;
}

//--------------------------------------------------------------------------
// Escala uma imagem em memoria
//--------------------------------------------------------------------------

ILuint ScaleImage(Image *image, ILuint width, ILuint height)
{
    ilBindImage(image->id);
    
    if (! iluScale(width, height, image->depth))
      return 0;
      
    image->rawData = ilGetData(); 
    image->width   = ilGetInteger(IL_IMAGE_WIDTH);
    image->height  = ilGetInteger(IL_IMAGE_HEIGHT);
    image->depth   = ilGetInteger(IL_IMAGE_DEPTH);
    image->format  = ilGetInteger(IL_IMAGE_FORMAT);
    
    return 1;
}

//--------------------------------------------------------------------------
// Faz uma copia, em memoria, de uma imagem
//--------------------------------------------------------------------------

ILuint CopyImage(Image *src, Image *dest)
{
    ILuint newImageID;
    
    ilGenImages(1, &newImageID);
    ilBindImage(newImageID);
    
    if (!ilCopyImage(src->id)) 
       return 0; 

    dest->id      = newImageID;
    dest->rawData = ilGetData(); 
    dest->width   = ilGetInteger(IL_IMAGE_WIDTH);
    dest->height  = ilGetInteger(IL_IMAGE_HEIGHT);
    dest->depth   = ilGetInteger(IL_IMAGE_DEPTH);
    dest->format  = ilGetInteger(IL_IMAGE_FORMAT);
    
    return 1;
}

//--------------------------------------------------------------------------
// Salva uma imagem em memoria para um arquivo
//--------------------------------------------------------------------------

ILboolean SaveImage(Image *image, char * filename)
{
    ilBindImage(image->id);
    
   return ilSaveImage(filename) ;
}

//--------------------------------------------------------------------------
// Carrega uma imagem para a memoria e gera uma textura a partir dela (OpenGL)
//--------------------------------------------------------------------------

ILuint LoadImageTexture(char *filename)
{   
    return ilutGLLoadImage(filename);
}

//--------------------------------------------------------------------------
// Carrega uma imagem para a memoria e gera um mipmap a partir dela (OpenGL)
//--------------------------------------------------------------------------

ILuint LoadImageMipmaps(char *filename)
{
    ILuint newImageID;
    
    ilGenImages(1, &newImageID);
    ilBindImage(newImageID);
    
    if (!ilLoadImage(filename)) 
       return 0; 

    ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
    
    return ilutGLBindMipmaps();
}

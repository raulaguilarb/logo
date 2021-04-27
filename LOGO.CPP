#include <graphics.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <math.h>

#define ANCHOLINEA 100
#define ALTO 20

struct { char nombre[30];
	 char variable[30];
	 int tope;
	 char M[ALTO][ANCHOLINEA];
	 } bloque;

// declaraci¢n de las funciones
int ejecutalinea(char c[]);
int buscaarchivo(char archivo[]);
int ejecutaarchivo(char archivo[], char param[]);
void cambiar(char linea[],char variable[], char param[]);
void operar(char linea[]);
void haceroperacion(char c[]);
int estavacio(char c[]);
int hacerif(char c[]);
int determinarsi(char comp[]);
int ejecutabloque(char param[]);
int buscabloque(char c[]);


// variables globales
float or;
float xmax, ymax,cx,cy,fx,fy;
char error[400];

void main(void)
{
   int gdriver = DETECT, gmode, errorcode;
   char c1[ANCHOLINEA];
   or=90;
   initgraph(&gdriver, &gmode, "F:\\borlandc\\bgi");
   errorcode = graphresult();
   if (errorcode != grOk)
   {  printf("Error: %s\n", grapherrormsg(errorcode));
      getch();
      exit(1); }
   xmax = getmaxx();
   ymax = getmaxy();
   cx = xmax/2+50;
   cy = ymax/2;
   outtextxy(5,5," ::Bienvenido a LOGO:: ");
   do
   {
    strcpy(error,"");
    gotoxy(3,2);
    setviewport(5,15,280,100,1);  //estas instrucciones
    clearviewport();             //borran un pedazo
    setviewport(0,0,xmax,ymax,1);//de la pantalla
    printf(">> ");
    gets(c1);
    if (!ejecutalinea(c1))
     {
     printf(" %s\n Presione una tecla",error);
     getch();
     }

   }while((strcmp(c1,"BYE")!=0));
   closegraph();
}/* main */

int ejecutalinea( char c[])
{
   char c1[ANCHOLINEA],p2[ANCHOLINEA];
   float c2;
   int j=0,i=0;
   strupr(c);
   if (estavacio(c)) return 1;
   while ( c[i]!= ' ' && i<=strlen(c) && c[i]!= 10 && c[i]!= 13)
     {
     c1[i]=c[i];
     i++;
     }
   c1[i]=0;
   i++;
   while (i<=strlen(c))
    {
    p2[j]=c[i];
    i++;
    j++;
    }
   p2[j]=0;
   if (strcmp(c1,"BYE")==0) return (1);
   if (strcmp(c1,"IF")==0)
       return (hacerif(p2));
   else
     if (strcmp(c1,"FD")==0)
	{
	c2 = atof(p2);
	fy = cy - (sin(M_PI * or / 180 ) * c2);  // se usan las funciones sin() y cos() para calcular los nuevos puntos de acuerdo a la orientacion y el avance
	fx = cx + (cos(M_PI* or / 180 ) * c2);  // sin() y cos() trabajan en radianes no en grados es necesario convertir
	line(cx,cy,fx,fy); // se dibuja el avance con una linea
	cx = fx;
	cy = fy;
	}
     else
       if (strcmp(c1,"BK")==0)
	{
	c2 = atof(p2);
	fy = cy + (sin(M_PI * or / 180 ) * c2);  // se usan las funciones sin() y cos() para calcular los nuevos puntos de acuerdo a la orientacion y el avance
	fx = cx - (cos(M_PI* or / 180 ) * c2);  // sin() y cos() trabajan en radianes no en grados es necesario convertir
	line(cx,cy,fx,fy); // se dibuja el avance con una linea
	cx = fx;
	cy = fy;
	}
       else
	 if (strcmp(c1,"LT")==0)
	    {
	    c2 = atof(p2);
	    or = or+c2;
	    }
	 else
	   if (strcmp(c1,"CC")==0)
	    {
	    c2 = atof(p2);
	    circle(cx,cy,c2);
	    }
	   else
	    if (strcmp(c1,"RT")==0)
	     {
	     c2 = atof(p2);
	     or = or-c2;
	     }
	    else
	       if (buscabloque(c1))
		  {
		  return (ejecutabloque(p2));
		  }
	       else
		 if (buscaarchivo(c1))
		    {
		    return (ejecutaarchivo(c1,p2));
		    }
		 else
		    {
		    strcat(error,"\n No se reconoce este comando: ");
		    strcat(error,c1);
		    return(0);
		    }
return(1);
}

int buscaarchivo(char archivo[])
{
FILE *ma;
if ((ma = fopen(archivo, "r+")) == NULL)
   {
   strcat(error,"\n No se encuentra este archivo: ");
   strcat(error,archivo);
   return 0;
   }
fclose(ma);
return 1;
}

int ejecutaarchivo(char archivo[], char param[])
{
 int i;
 FILE *in;
 char encabezado[ANCHOLINEA],variable[ANCHOLINEA];
 char M[ALTO][ANCHOLINEA];
 int res,j,l,tope=0;
 in = fopen(archivo, "r+");
 if (in == NULL) return (0);
 fgets(encabezado,sizeof(encabezado),in);
 j=0;
 for (i=3;i<strlen(encabezado);i++)
   {
    variable[j]=encabezado[i];
    j++;
   }
 variable[j-1]=0;
 strcpy(bloque.nombre,archivo);
 strcpy(bloque.variable,variable);
 do{

   fgets(M[tope],sizeof(M[tope]),in);
   if (!feof(in))
   {
     l = strlen(M[tope]);
     M[tope][l-1]=0;
     strcpy(bloque.M[tope],M[tope]);
     cambiar(M[tope],variable,param);
     operar(M[tope]);
     tope++;
   }
 }while (!feof(in));
 fclose(in);
 bloque.tope = tope;
 i=0;
  do {
    if ((strcmp(M[i],"END ")!=0)&&(strcmp(M[i],"END")!=0))
    {
    res = ejecutalinea(M[i]);
    }
    i++;
  }while ((i<tope)&&(res));
  if (!res)
   {
   strcat(error,"\n Error en : [");
   strcat(error,M[i-1]);
   strcat(error,"]");
   }
 return(res);
}

void cambiar(char linea[],char variable[], char param[])
{  int i,j,k;
   char linres[ANCHOLINEA];
   char temp[ANCHOLINEA];
   k=0;
   strcat(linea," ");
   for(i=0;i<=strlen(linea);i++)
     {
     if (linea[i]==':')
	{
	j=0;
	while(((linea[i]!= ' ')&&(i<=strlen(linea)))&&(linea[i]!= ')') && (linea[i]!= EOF) )
	   {
	     temp[j]=linea[i];
	     linea[i]= ' ';
	     i++;
	     j++;
	   }
	temp[j]=0;
	if (strcmp(temp,variable)==0)
		{
		for(j=0;j< strlen(param);j++)
		  {
		  linres[k]=param[j];
		  k++;
		  }
		linres[k]=0;
		}
	}
     else{
     linres[k] = linea[i];
     k++;}
     }
     linres[k]=0;
     strcpy(linea,linres);
}


void operar(char linea[])
{  int i,j,k;
   char linres[ANCHOLINEA];
   char operacion[ANCHOLINEA];
   k=0;
   for(i=0;i<=strlen(linea);i++)
     {
     if (linea[i]=='(')
	{
	j=0;
	i++;
	while((linea[i]!= ')')&&(i<=strlen(linea)))
	   {
	     operacion[j]=linea[i];
	     linea[i]= ' ';
	     i++;
	     j++;
	   }
	operacion[j]=0;
	linea[i]=' ';
	haceroperacion(operacion);
	for(j=0;j< strlen(operacion);j++)
	  {
	  linres[k]=operacion[j];
	  k++;
	  }
	}
     linres[k] = linea[i];
     k++;
     }
   strcpy(linea,linres);
}

void haceroperacion(char c[])
{
   char c1[ANCHOLINEA];
   char c2[ANCHOLINEA];
   char c3[ANCHOLINEA];
   char signo;
   int j=0,i=0;
   int num1,num2,res=0;
   while ((((c[i]!= '+')&&(c[i]!='-'))&&((c[i]!= '*')&&(c[i]!= '/')))&&i<=strlen(c))
     {
     c1[i]=c[i];
     i++;
     }
   if (! (i<=strlen(c)))
	{
	strcpy(c3,"( ");
	strcat(c3,c);
	strcat(c3," )");
	strcpy(c,c3);
	return;
	}
   signo = c[i];
   c1[i]=0;
   i++;
   while (i<=strlen(c))
    {
    c2[j]=c[i];
    i++;
    j++;
    }
   c2[j]=0;
   num1 = atoi(c1);
   num2 = atoi(c2);
   switch(signo)
   {
   case '+':res = num1 + num2;
	    break;
   case '-':res = num1 - num2;
	    break;
   case '*':res = num1 * num2;
	    break;
   case '/':if (num2 != 0)
		 res = num1 / num2;
	    else
		 {
		 strcpy(c,"NaN"); // Not a Number
		 return;
		 }
	    break;
   }
   itoa(res,c,10);
}

int estavacio(char c[])
{
int i;
for(i=0;i<strlen(c);i++)
   {
   if (c[i] !=' ') return 0;
   }
return 1;
}


int hacerif(char c[])
{
   char c1[ANCHOLINEA],p2[ANCHOLINEA];
   char M[ALTO][ANCHOLINEA];
   int tope,res=0;
   int j=0,i=1;

   while ( c[i]!= ')' && i<=strlen(c) )
     {
     c1[i-1]=c[i];
     i++;
     }
   c1[i]=0;
   i+=3;
   while (i<=strlen(c))
    {
    p2[j]=c[i];
    i++;
    j++;
    }
   p2[j]=0;
   j=0;
   if (determinarsi(c1))
      {
      tope = 0;
      for(i=0;i<=strlen(p2);i++)
	{
	if (p2[i] == ',')
	    {
	    tope++;
	    j=0;
	    i++;
	    }
	else
	    {
	    M[tope][j]=p2[i];
	    j++;
	    M[tope][j]=0;
	    }
	}
	tope++;
	i=0;
	do {
	  res = ejecutalinea(M[i]);
	  i++;
	}while ((i<tope)&&(res));
	if (!res)
	   {
	   strcat(error,"\n Error en : [");
	   strcat(error,M[i-1]);
	   strcat(error,"]");
	   }
      }
      else
	 res = 1;
return(res);
}

int determinarsi(char c[])
{
   char c1[ANCHOLINEA];
   char c2[ANCHOLINEA];
   char oper[3];
   int j=0,i=0;
   int num1,num2,ret=0;
   while ( c[i]!= '>' && c[i]!='<' && c[i]!= '=' && i<=strlen(c))
     {
     c1[i]=c[i];
     i++;
     }
   c1[i]=0;
   oper[0] = c[i];
   i++;
   oper[1] = c[i];
   oper[2] = 0;
   i++;
   while (i<=strlen(c))
    {
    c2[j]=c[i];
    i++;
    j++;
    }
   c2[j]=0;
   num1 = atoi(c1);
   num2 = atoi(c2);
   if (strcmp(oper,"> ")==0)
	    if (num1 > num2)
		 ret = 1;
	    else
		 ret = 0;
   else
   if (strcmp(oper,"< ")==0)
	    if (num1 < num2)
		 ret = 1;
	    else
		 ret = 0;
   else
   if (strcmp(oper,"= ")==0)
	    if (num1 == num2)
		 ret = 1;
	    else
		 ret = 0;
   else
   if (strcmp(oper,">=")==0)
	    if (num1 >= num2)
		 ret = 1;
	    else
		 ret = 0;
   else
   if (strcmp(oper,"<=")==0)
	    if (num1 == num2)
		 ret = 1;
	    else
		 ret = 0;
return ret;
}

int buscabloque(char c[])
{
int ret=0;
if (strcmp(c,bloque.nombre)==0)
     {
     ret = 1;
     }
return (ret);
}

int ejecutabloque(char param[])
{
char M[ALTO][ANCHOLINEA];
int i=0,res=0;
do {
  strcpy(M[i],bloque.M[i]);
  cambiar(M[i],bloque.variable,param);
  operar(M[i]);
  res = ejecutalinea(M[i]);
  i++;
}while ((i< bloque.tope)&&(res));
  if (!res)
     {
     strcat(error,"\n Error en : [");
     strcat(error,M[i-1]);
     strcat(error,"]");
     }
return(res);
}
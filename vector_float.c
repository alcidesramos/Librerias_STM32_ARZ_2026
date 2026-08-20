unsigned int  n_mayor(float *vector,unsigned int size,unsigned int inici)
{
  unsigned int cu,pos;
  float mayor;
  
          pos=inici;
          mayor=vector[0];
    for (cu=inici+1;cu<size;cu++)
    {
      if (vector[cu]>mayor )
      {
        mayor=vector[cu] ;
        pos=cu;
      }
    }
    return(pos);
}


unsigned int  n_menor(float *vector,unsigned int size)
{
  unsigned int cu,pos;
  float menor;

          pos=0;
          menor=vector[0];
    for (cu=1;cu<size;cu++)
    {
      if (vector[cu]<menor )
      {
        menor=vector[cu] ;
        pos=cu;
      }
    }
    return(pos);
}
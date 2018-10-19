void pscount_dmp()
{
  struct mproc *mp;
  int i,n=0;
  if(getsysinfo(PM_PROC_NR, SI_PROC_TAB, mproc, sizeof(mproc))!=OK){
    printf("Error\n");
    return;
  }
  for(i=0;i<NR_PROCS;i++){
    mp=&mproc[i];
    if(mp->mp_pid==0 && i!=PM_PROC_NR) continue;
    n++;
  }
  
  printf("Number of running processes: %d\n",n);
} 
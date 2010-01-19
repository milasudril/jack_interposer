/*
 * This file, unlike checkers.c, is maintained manually because these
 * checks for some reason cannot be done manually
 */
int fprintf(FILE *stream, const char *format, ...)
{
  va_list ap;
  va_start(ap, format);
  return vfprintf(stream, format, ap);
}

int printf(const char *format, ...)
{
  va_list ap;
  va_start(ap, format);
  return vprintf(format, ap);
}

int pthread_cond_timedwait(pthread_cond_t * cond, pthread_mutex_t * mutex, const struct timespec* abstime)
{
  static int (*func)(pthread_cond_t*, pthread_mutex_t*, const struct timespec* abstime);

  if (in_rt)
  {
    printf("pthread_cond_timedwait() is called while in rt section\n");     
#if ABORT_ON_VIOLATION
    abort();
#endif
  }

  if(!func)
    //func = (int (*)(pthread_cond_t*, pthread_mutex_t*)) dlsym(RTLD_NEXT, "pthread_cond_wait");
    // see http://forums.novell.com/novell-product-support-forums/suse-linux-enterprise-server-sles/sles-configure-administer/385705-sles-10-2-java-hung-calling-pthread_cond_timedwait.html
    func = (int (*)()) dlvsym(RTLD_NEXT, "pthread_cond_timedwait", "GLIBC_2.3.2");
  if (func == NULL)
  {
    fprintf(stderr, "Error dlsym'ing\n");
    abort();
  }
  return(func(cond, mutex, abstime));
}


int pthread_cond_wait(pthread_cond_t * cond, pthread_mutex_t * mutex)
{
  static int (*func)(pthread_cond_t*, pthread_mutex_t*);

  if (in_rt)
  {
    printf("pthread_cond_wait() is called while in rt section\n");     
#if ABORT_ON_VIOLATION
    abort();
#endif
  }

  if(!func)
    //func = (int (*)(pthread_cond_t*, pthread_mutex_t*)) dlsym(RTLD_NEXT, "pthread_cond_wait");
    // see http://forums.novell.com/novell-product-support-forums/suse-linux-enterprise-server-sles/sles-configure-administer/385705-sles-10-2-java-hung-calling-pthread_cond_timedwait.html
    func = (int (*)(pthread_cond_t*, pthread_mutex_t*)) dlvsym(RTLD_NEXT, "pthread_cond_wait", "GLIBC_2.3.2");
  if (func == NULL)
  {
    fprintf(stderr, "Error dlsym'ing\n");
    abort();
  }
  return(func(cond, mutex));
}



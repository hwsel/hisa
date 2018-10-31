#include <assert.h>
//#include <atomic.h>
//#include <ldsodefs.h>
//#include <libintl.h>
#include <stdio.h>
#include <stdlib.h>
//#include <sysdep.h>
#include <unistd.h>
#include <sys/mman.h>


extern const char *__progname;

#include <wchar.h>
//#include <libio/iolibio.h>
#define fflush(s) _IO_fflush (s)

/* This function, when passed a string containing an asserted
   expression, a filename, and a line number, prints a message
   on the standard error stream of the form:
	a.c:10: foobar: Assertion `a == b' failed.
   It then aborts program execution via a call to `abort'.  */

#ifdef FATAL_PREPARE_INCLUDE
# include FATAL_PREPARE_INCLUDE
#endif


void
__assert_fail_base (const char *fmt, const char *assertion, const char *file,
		    unsigned int line, const char *function)
{
  char *str;

#ifdef FATAL_PREPARE
  FATAL_PREPARE;
#endif

  int total;
  if (__asprintf (&str, fmt,
		  __progname, __progname[0] ? ": " : "",
		  file, line,
		  function ? function : "", function ? ": " : "",
		  assertion, &total) >= 0)
    {
      /* Print the message.  */
      (void) __fxprintf (NULL, "%s", str);
      (void) fflush (stderr);

      total = (total + 1 + GLRO(dl_pagesize) - 1) & ~(GLRO(dl_pagesize) - 1);
      struct abort_msg_s *buf = __mmap (NULL, total, PROT_READ | PROT_WRITE,
					MAP_ANON | MAP_PRIVATE, -1, 0);
      if (__builtin_expect (buf != MAP_FAILED, 1))
	{
	  buf->size = total;
	  strcpy (buf->msg, str);

	  /* We have to free the old buffer since the application might
	     catch the SIGABRT signal.  */
	  struct abort_msg_s *old = atomic_exchange_acq (&__abort_msg, buf);

	  if (old != NULL)
	    __munmap (old, old->size);
	}

      free (str);
    }
  else
    {
      /* At least print a minimal message.  */
      static const char errstr[] = "Unexpected error.\n";
      __libc_write (STDERR_FILENO, errstr, sizeof (errstr) - 1);
    }

  abort ();
}

void
__assert_fail (const char *assertion, const char *file, unsigned int line,
	       const char *function)
{
  __assert_fail_base (_("%s%s%s:%u: %s%sAssertion `%s' failed.\n%n"),
		      assertion, file, line, function);
}

/*
 *  child.c
 *
 *  Copyright (C) 1999 Timo Felbinger
 *  Timo.Felbinger@quantum.physik.uni-potsdam.de, 1999
 *  
 *  Updated and changed by Mike Gevaert (Copyright 2008)
 *  mgevaert@gmail.com, 2008
 *
 *  functions to work with child processes
 *
 *  Note that some related low-level functions are in os_unix.c 
 *  or os_win32.c
 *  Some user call-able functions are in eval.c
 *
 */
//todo:
//
//fix bugs
//   win32 closing of pipe????
//   win32 cmdline
//   mch_can_exec
//   check to see if running in secure mode, then don't allow
//   figure out if ctrl+c should interupt
//   should it work like cscope? ie :child fork/read/write, etc?
//   check for memory leaks...
//style it up
//options
//   read_lines
//      call back?
//      block?
//      max number of lines to read
//      return w/ number of lines read?
//      line number to start appending
//   write_lines
//      line to write
//updated doc

#include "vim.h"

#undef DEBUG
#ifdef USE_CHILD

   CHILD *
child_find_by_name(buf, name)
   buf_T * buf;	  /* buffer in which to look for child */
   char_u * name;  /* name of child to look for */
{
   CHILD *p;
   for (p = buf->b_child; p ; p = p->c_next) 
      if (!strcmp(name, p->c_name))
	 return p;
   return NULL;
}


   void
child_kill_all( buf )
   buf_T *buf;    /* buffer in which to kill children */
{
   while (buf->b_child)
      kill_child(buf->b_child);
}

   int
child_list( buf )
   buf_T *buf;    /* name of buffer to list child */
{
   CHILD *pc;
   char_u mbuf[ 64 ];

   if (!buf->b_child){
      MSG( "buffer has no children" );
      return FALSE;
   }
   for (pc = buf->b_child; pc; pc = pc->c_next){
#ifdef HAVE_SNPRINTF
      snprintf( 
	    mbuf, 64
#else
	    /*
	     *  Those with ancient systems get what they deserve:
	     */
	    sprintf( 
	       mbuf
#endif
	       , "  %6ld:  %4di %4do %4de %s"
	       , (long)(pc->c_pid), pc->c_in, pc->c_out, pc->c_err  
	       , pc->c_name
	       );
	    mbuf[63] = NUL;
	    MSG( mbuf );
   }
return TRUE;
}

/*
 * read_line_fm_child:
 * reads a line from a child, either stderr or stdout, depending on 
 * what is passed in
 */
  int
child_read_lines( fd, buf, bp, be )
   int *fd;
   char_u **buf;  /* buffer for input*/
   char_u **bp;   /* pointer into buffer */
   char_u **be;   /* pointer to 1st byte after c_inbuf */
{
   size_t rsize;
   char_u *start, 
	  *newp;
   int ret, 
       linesadded = 0;
   //for select
   fd_set rfd;  //set of descriptors to read
   struct timeval tv;
   tv.tv_sec = tv.tv_usec = 0;

   if (*fd == -1){
      EMSG("read_line_fm_child(): child's filedescriptor has already been closed");
      goto read_line_fm_child_FAIL;
   }

   if (*buf == NULL){  //must have an uncompleted line already in bufffer
      if (!(*buf = (char_u *)malloc(CHILD_BSIZE))){
	 EMSG("read_line_fm_child(): alloc() failed for c_inbuf");
	 goto read_line_fm_child_FAIL;
      } else {
	 *bp = *buf;
	 *be = *buf + CHILD_BSIZE;
      }
   }

   //check if there is something to read
   FD_ZERO(&rfd);
   FD_SET(*fd, &rfd);
   //we need to repeatedly check if there's stuff to read, and then read it, 
   //expanding the buffer as we go, in case it's longer than expected
   while ((ret = child_select(*fd + 1, &rfd, NULL, NULL, &tv)) == 1){ //something to read
      errno = 0;
      rsize = vim_read(*fd, *bp, *be - *bp);
      if (errno || rsize < 0){
	 EMSG( "read_line_fm_child(): vim_read() failed on child's stdin, disconnecting..." );
	 close(*fd);
	 *fd = -1;
	 goto read_line_fm_child_FAIL;
      }
      if (rsize == 0){
	 EMSG("read_line_fm_child: child has closed the pipe, you need to kill the child");
	 close(*fd);
	 *fd = -1;
	 goto read_line_fm_child_FAIL;
      }else if (rsize == *be - *bp){  //have to expand buffer
//	 MSG("Expanding buffer");
	 newp = (char_u *)vim_realloc(*buf, 2 * (*be - *buf));
	 if (!newp){
	    EMSG("read_line_fm_child(): vim_realloc() failed");
	    goto read_line_fm_child_FAIL;
	 }
	 *bp = (*bp - *buf) + newp + rsize;
	 *be = newp + 2 * (*be - *buf); 
	 *buf = newp;
      }else{
	 *bp += rsize;
      }
      FD_ZERO(&rfd);
      FD_SET(*fd, &rfd);
#ifdef WIN32
      /* need to sleep, windows doesn't fill the buffer all the way
       * so this gives it a chance to refill */
      Sleep(250); //sleep 250 ms
#endif
   } //Nothing read, still might have old stuff to read in buffer

   if (ret == -1){
      EMSG("read_line_fm_child: select() failed.");
      goto read_line_fm_child_FAIL;
   }

   if (*buf == *bp){
      MSG("read_line_fm_child: nothing read");
   }else{
      newp = start = *buf;  //newp moves along line, looking for \n, start points to the char after the last \n
      while (newp <= *bp){
	 if (*newp == '\n'){  //end of a line of input
	    *newp = NUL;
	    child_append_line(curbuf, curwin->w_cursor.lnum + linesadded, 
		  start, STRLEN(start) + 1);
	    ++linesadded;
	    start = newp + 1;
	 }
	 newp++;
      } 
      if (start != *bp){ //there is data in buffer that wasn't \n terminated, move it to the front, and keep buffer
	 STRNCPY(*buf, start, *bp - start);
	 *bp  = *bp - start + *buf;
      }
      //we keep buffer around, to be read into next time
   }

   return OK;

read_line_fm_child_FAIL:
   if (*buf != NULL){
      vim_free(*buf);
      *buf = NULL;
      *bp  = NULL;
      *be  = NULL;
   }
   return FAIL;
}

/*
 * write_line_to_child:
 * Takes a child, and a line pointer, and writes this synchronously 
 * to the child
 */
   int
child_write_line_to( pc, lp )
   CHILD *pc;
   char_u *lp;
{
   size_t len;
   if (pc->c_in < 0){
      EMSG("write_line_to_child: stdin not available (disconnected after error?)");
      return FAIL;
   }
   len = strlen((char *)lp);
   if (vim_write( pc->c_in, lp, len ) != len){
      EMSG( "write_line_to_child: vim_write() #1 failed, disconnecting stdin");
      close(pc->c_in);
      pc->c_in = -1;
      return FAIL;
   }
   if (vim_write( pc->c_in, "\n", 1 ) < 1){
      EMSG("write_line_to_child: vim_write() #2 failed, disconnecting stdin");
      close(pc->c_in);
      pc->c_in = -1;
      return FAIL;
   }
   return OK;
}


/*
 *  some functions which should actually be somewhere else, as they are
 *  not really related to childs. However, I didn't find them anywhere,
 *  so I had to write them.
 *  they are pretty much copied from the perl interface in if_perl.xs
 */

   int
child_append_line( buf, lnum, line, llen )
   buf_T *buf;
   linenr_T  lnum;
   char_u   *line;
   colnr_T   llen;   /* length _including_ the NUL */
{
   int rv = FAIL;  //return var
   buf_T *savebuf = curbuf;  //save current buffer, incase it's different than buf
   win_T *win;

   if (lnum < 0 || lnum > buf->b_ml.ml_line_count)
      return rv;

   curbuf = buf;
   if (u_inssub(lnum + 1)){
      mark_adjust(lnum + 1, MAXLNUM, 1L, 0L);
      ml_append(lnum, line, llen, FALSE);
      changed();
      rv = TRUE;
   }
   curbuf = savebuf;
   update_curbuf(NOT_VALID);

   /* fix cursor position: */
   for (win = firstwin; win; win = win->w_next)
      if (win->w_buffer == buf)
	 if (win->w_cursor.lnum > lnum)
	    win->w_cursor.lnum++;

   c_need_refresh = TRUE;

   return rv;
}

#endif  /* defined( USE_CHILD ) */

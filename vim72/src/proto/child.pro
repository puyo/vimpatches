CHILD * child_find_by_name __ARGS(( buf_T *, char_u * ));
void child_kill_all __ARGS(( buf_T * ));
int child_list __ARGS(( buf_T * ));
void child_read_stderr __ARGS(( CHILD * ));
int child_append_line __ARGS(( BUF *, linenr_T, char_u *, colnr_T ));
char_u *child_get_line __ARGS(( BUF *, linenr_T ));
int child_write_line __ARGS(( CHILD *, char_u *));
int child_read_lines  __ARGS(( int *, char_u **, char_u **, char_u **));




typedef void ( *Process )( void* );


typedef struct Semaphore Semaphore;

struct Semaphore
{
    Process up, down;
    void *context;
    int count;
};


Semaphore *semaphore__new( Process up, Process down, void *context )
{
    Semaphore *s = new( Semaphore );

    s->count = 1;
    s->up = up;
    s->down = down;
    s->context = context;
}


void
semaphore__dec( Semaphore *s )
{
    if ( s->count && !--s->count)
        s->down( s->context );
}


void
semaphore__inc( Semaphore *s )
{
    if ( !s->count++ )
        s->up( s->context );
}
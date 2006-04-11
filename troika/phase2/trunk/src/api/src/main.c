/*******************************************************************************

Phase2 language API, Copyright (C) 2005 Joshua Shinavier.

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE. See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place, Suite 330, Boston, MA 02111-1307 USA

*******************************************************************************/

#include <getopt.h>

#include <Compiler.h>


    static int brief_flag;
    static int debug_flag;
    static int help_flag;
    static int quiet_flag;
    static int verbose_flag;
    static int version_flag;

static void
print_version()
{
    printf( "Phase2 %s\n", VERSION );
    printf( "Copyright (C) 2006 Joshua Shinavier\n" );
    printf( "The Phase2 programming language comes with ABSOLUTELY NO WARRANTY. This is free software, and you are welcome to redistribute it under certain conditions; type \"_license;\" for details.  Type \"_quit;\" to exit.\n" );
}


static void
print_help()
{
    printf( "You expect me to help you?\n" );
    printf( "Report bugs to parcour@gmail.com\n" );
}


static void
read_options ( int argc, char **argv, char *source_file )
{
    int c;

    for (;;)
    {
        static struct option long_options[] =
        {
            /* These options set a flag. */
            { "brief",      no_argument,        &brief_flag,    1   },
            { "debug",      no_argument,        &debug_flag,    1   },
            { "file",       required_argument,  0,              ( int ) 'f' },
            { "help",       no_argument,        &help_flag,     1   },
            { "quiet",      no_argument,        &quiet_flag,    1   },
            { "verbose",    no_argument,        &verbose_flag,  1   },
            { "version",    no_argument,        &version_flag,  1   },
            { 0,            0,                  0,              0   }
        };

        /* getopt_long stores the option index here. */
        int option_index = 0;

        c = getopt_long (argc, argv, "abc:d:f:", long_options, &option_index);

        /* Detect the end of the options. */
        if ( c == -1 )
            break;

        switch ( c )
        {
            case 0:
                /* If this option set a flag, do nothing else now. */
                if ( long_options[option_index].flag != 0 )
                    break;

                #if DEBUG__MAIN
                printf ( "option %s", long_options[option_index].name );
                if ( optarg )
                    printf ( " with arg %s", optarg );
                printf ( "\n" );
                #endif

                break;

            case 'f':
                #if DEBUG__MAIN
                printf ("option -f with value `%s'\n", optarg);
                #endif

                strcpy( source_file, optarg );
                break;

            case '?':
                /* getopt_long already printed an error message. */
                break;

            default:
                abort ();
        }
    }

    if ( help_flag )
    {
        print_help();
        exit( 0 );
    }

    else if ( version_flag )
    {
        print_version();
        exit( 0 );
    }

    /* Print any remaining command line arguments (not options). */
    if ( optind < argc )
    {
        printf ( "non-option ARGV-elements: " );
        while ( optind < argc)
            printf ( "%s ", argv[optind++] );
        printf( "\n" );
    }
}


/** Instantiates an Environment and attaches a Compiler to interact with the
    command line. */
int main( int argc, char *argv[] )
{
    int status = EXIT_SUCCESS;

    Environment *env;
    Compiler *compiler;

    static char source_file[0x100];
    *source_file = '\0';
    read_options ( argc, argv, source_file );

    if ( !( env = environment__new() ) )
        status = EXIT_FAILURE;

    else
    {
        if ( ( compiler = compiler__new( env ) ) )
        {
            if ( *source_file )
            {
                #if DEBUG__MAIN
                printf( "Loading namespace from file...\n" );
                #endif

                compiler__deserialize( compiler, source_file );
            }

            print_version();

            if ( compiler__parse( compiler ) )
                ERROR( "main: parse failed" );

            compiler__delete( compiler );
        }

        else
            status = EXIT_FAILURE;

        environment__delete( env );
    }

    /* See: http://www.jetcafe.org/~jim/c-style.html#Expressions */
    exit( status );
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */

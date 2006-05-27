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

#if USE_NCURSES
#    include <ncurses.h>
#endif

#include <Compiler.h>
#include "settings.h"


static char *alias;

static int brief_flag;
static int debug_flag;
static int help_flag;
static int quiet_flag;
static int verbose_flag;
static int version_flag;


static void
print_version()
{
    PRINT( "Phase2 %s\n", VERSION );
}


static void
print_copying()
{
    PRINT( "Copyright (C) 2006 Joshua Shinavier\n" );
    PRINT( "The Phase2 programming language comes with ABSOLUTELY NO WARRANTY.  This is free software, and you are welcome to redistribute it under certain conditions; type \"_license;\" for details.  Type \"_quit;\" to exit.\n" );
}


static void
print_usage()
{
    PRINT( "Usage:\t%s [options]...\n", alias );
    PRINT( "Options:\n" );
    PRINT( "  -f, --file <file>    load a namespace from <file>\n" );
    PRINT( "  -h, --help               print this help and exit\n" );
    PRINT( "  -q, --quiet              suppress output to stdout\n" );
    PRINT( "  -v, --version            print version number and exit\n" );
}


static void
print_bugs()
{
    PRINT( "Report bugs to %s.\n", PACKAGE_BUGREPORT );
}


static void
print_help()
{
    print_usage();
    print_bugs();
}


static void
read_options ( int argc, char **argv, char *source_file )
{
    int c;

    alias = argv[0];

    for (;;)
    {
        static struct option long_options[] =
        {
            /* These options set a flag. */
            { "brief",      no_argument,        &brief_flag,    1           },
            { "debug",      no_argument,        &debug_flag,    1           },
            { "file",       required_argument,  0,              ( int ) 'f' },
            { "help",       no_argument,        &help_flag,     ( int ) 'h' },
            { "quiet",      no_argument,        &quiet_flag,    ( int ) 'q' },
            { "verbose",    no_argument,        &verbose_flag,  1           },
            { "version",    no_argument,        &version_flag,  ( int ) 'v' },
            { 0,            0,                  0,              0           }
        };

        /* getopt_long stores the option index here. */
        int option_index = 0;

        c = getopt_long (argc, argv, "f:hqv", long_options, &option_index);

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
                PRINT( "option %s", long_options[option_index].name );
                if ( optarg )
                    PRINT ( " with arg %s", optarg );
                PRINT ( "\n" );
                #endif

                break;

            case 'f':

                #if DEBUG__MAIN
                PRINT("option -f with value `%s'\n", optarg);
                #endif

                strcpy( source_file, optarg );
                break;

            case 'h':

                help_flag = 1;
                break;

            case 'q':

                quiet_flag = 1;
                break;

            case 'v':

                version_flag = 1;
                break;

            case '?':

                /* getopt_long already printed an error message. */
                print_usage();
                abort();
                break;

            default:

                print_usage();
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
        PRINT( "non-option ARGV-elements: " );
        while ( optind < argc)
            PRINT ( "%s ", argv[optind++] );
        PRINT( "\n" );
        print_usage();
        abort();
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

#if USE_NCURSES
    initscr();
#endif

    read_options( argc, argv, source_file );

    if ( !( env = environment__new() ) )
        status = EXIT_FAILURE;

    else
    {
        if ( ( compiler = compiler__new( env, quiet_flag ) ) )
        {
            if ( *source_file )
            {
                if ( DEBUG__MAIN )
                    PRINT( "Loading namespace from file...\n" );

                compiler__deserialize( compiler, source_file );
            }

            if ( !quiet_flag )
            {
                print_version();
                print_copying();
                REFRESH;
            }

            if ( compiler__parse( compiler ) )
                ERROR( "main: parse failed" );

            compiler__delete( compiler );
        }

        else
            status = EXIT_FAILURE;

        environment__delete( env );
    }

#if USE_NCURSES
    endwin();
#endif

    /* See: http://www.jetcafe.org/~jim/c-style.html#Expressions */
    exit( status );
}


/* kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on */

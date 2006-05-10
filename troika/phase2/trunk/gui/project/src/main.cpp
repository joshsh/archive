#include <QtGui>

#include <widgets/P2MainWindow.h>
#include <P2Environment.h>
#include <P2Binder.h>

extern "C"
{
#include <stdio.h>
#include <getopt.h>
#include <Compiler.h>
}


////////////////////////////////////////////////////////////////////////////////

QList< P2MainWindow* > windows;


/** Create a new top-level widget. */
void newMainWindow( P2Binder &b )
{
    P2MainWindow *w = new P2MainWindow( b );
/*
            Qt::Window
            | Qt::WindowTitleHint
            | Qt::WindowSystemMenuHint
            | Qt::WindowMinMaxButtonsHint );
*/

    windows.append( w );

    w->show();

    // If the display area is small, maximize the first main window.
    #ifdef ARM_LINUX
    if ( windows.size() == 1 )
        w->showMaximized( );
    #endif
}


static void
delete_windows()
{
    while ( windows.size() )
        delete windows.takeFirst();
}


////////////////////////////////////////////////////////////////////////////////


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
    printf( "Phase2 GUI %s\n", VERSION );
}


static void
print_copying()
{
    printf( "Copyright (C) 2006 Joshua Shinavier\n" );
    printf( "The Phase2 GUI comes with ABSOLUTELY NO WARRANTY. This is free software, and you are welcome to redistribute it under certain conditions.  See COPYING for details.\n" );
}


static void
print_usage()
{
    printf( "Usage:\t%s [options]...\n", alias );
    printf( "Options:\n" );
    printf( "  -f, --file <file>    load a namespace from <file>\n" );
    printf( "  --help               print this help and exit\n" );
    printf( "  --version            print version number and exit\n" );
}


static void
print_bugs()
{
    printf( "Report bugs to %s.\n", PACKAGE_BUGREPORT );
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
            case 'f':

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
        printf ( "non-option ARGV-elements: " );
        while ( optind < argc)
            printf ( "%s ", argv[optind++] );
        printf( "\n" );
        print_usage();
        abort();
    }
}


////////////////////////////////////////////////////////////////////////////////


/** Main function for the Phase2 GUI. */
int
main( int argc, char **argv )
{
    static char source_file[0x100];
    *source_file = '\0';

#ifndef ARM_LINUX
    read_options ( argc, argv, source_file );
#endif

    if ( !quiet_flag )
    {
        print_version();
        print_copying();
    }

    P2Environment env( ( quiet_flag ) );

    if ( *source_file )
    {
        compiler__deserialize( env.getCompiler(), source_file );
        //compiler__deserialize( env.getCompiler(), "guitest.p2" );
        //compiler__deserialize( env.getCompiler(), "../api/examples/test.p2" );
    }

    P2Binder binder( &env );

    QApplication a( argc, argv );

//    QApplication::setStyle( new QWindowsStyle );
//    QApplication::setStyle( new QWindowsXPStyle );
//    QApplication::setStyle( new QMotifStyle );
//    QApplication::setStyle( new QMacStyle );
//    QApplication::setStyle( new QPlastiqueStyle );

//cout << "a.startDragDistance():" << a.startDragDistance() << endl;
//cout << "a.startDragTime():" << a.startDragTime() << endl;
//a.setStartDragTime( 5000 );

    // In the beginning there was a single top-level window.
    newMainWindow( binder );

    int ret = a.exec();
    //int ret = QApplication::exec();

    delete_windows();

    return ret;
}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on

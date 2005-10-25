/*+

p2_client.cpp

Semantic module for the Phase2 command line interface.

last edited: 9/5/05

*//*/////////////////////////////////////////////////////////////////////////////

Phase2 version 0.4, Copyright (C) 2005 Joshua Shinavier.

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

Joshua Shinavier
parcour@gmail.com
+1 509 747-6051

*////////////////////////////////////////////////////////////////////////////////

//   [unrelated] Kernighan ^ 1974 C tutorial: http://www.lysator.liu.se/c/bwk-tutor.html
// g++ -c p2_client.cpp -I../../gsoap-linux-2.7

using namespace std;

#include <iostream>
#include <fstream>

extern "C"
{
    #include "util/array.h"
    #include "util/hash_table.h"
    #include "util/term.h"

/*
    // Core Phase2 libraries.
    #include "p2.h"

    // These two are derived from the "core" libraries, but are not part of them.
    #include "p2_syntax.h"
    #include "p2_command.h"

    #include "sk/sk.h"
*/
    // Core Phase2 libraries.
    #include "p2.c"
    #include "p2_error.c"
    #include "p2_type.c"
    #include "p2_primitive.c"

    // These two are derived from the "core" libraries, but are not part of them.
    #include "p2_syntax.c"
    #include "p2_command.c"

    #include "sk/sk.c"

    // These two functions need to be global for the sake of p2_parse.y
    void P2_evaluate_expression(char *name, char *expr);
    void P2_evaluate_command(char *name, char *args);

    void yyparse();
}



// XML DOM and gSOAP dependencies (need -I$(GSOAP_PATH) when compiling).
#include "xml/xmldom.h"
#include "stdsoap2.h"
#include "p2.nsmap"

#include "p2_dataset.cpp" //!
//#include "p2_dataset.h"

//void P2_print(void *p);
void deserialize(char *xmlstr, DOM_Element *document);

HASH_TABLE *all_datasets_;
P2_data_set *active_dataset_;
DOM_Element *active_document_;

#include "import/primitives.cpp"  //!



// Consider making this library into a class, and replacing these two functions
// with its constructor and destructor.
P2_error P2_client_init();
P2_error P2_client_end();



void debug_print(P2_term *term)
{
    active_dataset_->debug_print(term);
}

/** USER COMMANDS */////////////////////////////////////////////////////////////



// Command definitions are in another file to avoid clutter.
#include "import/commands.cpp"



/** PARSER INTERFACE *//////////////////////////////////////////////////////////



// There must be a better way to pass a string through a stream...
void deserialize(char *xmlstr, DOM_Element *document)
{
    ofstream out("temp.xml");
    out << xmlstr;
    out.close();
    ifstream in("temp.xml");
    in >> (*document); // parse
    in.close();
}



// Note: check for NULL output (<== empty sequence).
P2_term *decode_parser_expression(DOM_Element *el)
{
    static char buffer[1000];
    P2_term *term, *subterm;

    // Simple term.
    if (strcmp(DOM_tagName(el), P2_P2_term))
    {
        // The buffer is necessary because of P2_parse_id.
        *buffer = '\0';
        char *text = DOM_text(el);
        if (text)
            strcat(buffer, text);
        term = P2_parse_token(buffer);
    }

    // Compound term;
    else
    {
        DOM_Element *cur = DOM_firstChild(el);
        term = NULL;
        while (cur)
        {
            subterm = decode_parser_expression(cur);

            // Null subterm; do nothing (empty sequences are tolerated but not translated).
            if (!subterm)
            {
                cout << "\t>> Warning: empty subterm. <<" << endl;
            }

            // Error in subterm; abort.
            else if ((P2_type) subterm->type == P2_error_type)
            {
                if (term)
                    P2_term__delete(term);
                term = subterm;  // Propagate error upward.
                break;
            }

            // Add subterm.
            else
            {
                if (!term)
                    term = subterm;
                else
                    //Note: Left-associativity of terms is imposed here.
                    term = P2_term__merge_la(term, subterm);
        }
            cur = DOM_nextSibling(cur);
        }
    }

    return term;
}



P2_term *resolve_nonlocal_ids(P2_term *term)
{
    P2_term *newseq = term;

    if ((P2_type) term->type == P2_id_type)
    {
        P2_id *id = (P2_id *) term->value;
        if (id->dataset_id)
        {
           P2_data_set *ds = (P2_data_set *) hash_table__lookup(all_datasets_, (void *) id->dataset_id);
           if (ds)
               newseq = ds->get(id->local_id);
        }
    }
    return newseq;
}



/*
  Process the expression and send it through the SK semantics module.
*/
void P2_evaluate_expression(char *name, char *xmlexpr)
{
//cout << "+ P2_evaluate_expression" << endl; cout.flush();
    // Instantiate a DOM object.
    struct soap soap;
    soap_init2(&soap, SOAP_IO_DEFAULT, SOAP_XML_TREE);
    soap_dom_element document(soap_new()); // create a DOM with a new soap environment
    soap_set_imode(document.soap, SOAP_DOM_TREE); // DOM tree w/o application data

    // Deserialize XML input.
    deserialize(xmlexpr, &document);

    #ifdef PARSER_DEBUG
        cout << endl << xmlexpr << endl;
        cout << document << endl; // print it
    #endif

    // Interpret the resulting XML.
    P2_term *term = decode_parser_expression(&document);

    // Destroy the DOM object.
    soap_destroy(document.soap); // delete entire DOM
    soap_end(document.soap); // clean up
    soap_done(document.soap); // detach the soap environment
    free(document.soap); // free the soap environment
    soap_end(&soap); // remove all temporary and deserialized data
    soap_done(&soap);

    P2_error err = P2_SUCCESS;

    if (!term)
    {
        cout << "\t>> Client error: received empty input sequence from parser. <<" << endl;
    }

    else if ((P2_type) term->type == P2_error_type)
    {
        char s[100];
        P2_print_error((P2_error) term->value, s);
        cout << "\t>> Error: " << s << " <<" << endl;
    }

    else
    {
        // Resolve identifiers.
        term = P2_term__replace_atoms(term, resolve_nonlocal_ids);
        //P2_term__forall_atoms(term, resolve_nonlocal_ids);
        term = P2_term__normalize_la(term);

        char *local_id = NULL;

        P2_term *id_seq = NULL;

        // Submit to the approprite data set.
        if (name != NULL)
        {
            id_seq = P2_parse_id(name);

            if ((P2_type) id_seq->type == P2_error_type)
            {
                printf("\t>> Warning: dictionary assignment failed (bad identifier). <<\n");
                err = P2_SUCCESS;
            }

            else
            {
                local_id = ((P2_id *) (id_seq->value))->local_id;
                err = active_dataset_->add(local_id, term);
            }
        }

        else
        {
            local_id = "$";
            //err = active_dataset_->add(NULL, term);
            err = active_dataset_->add(local_id, term);
        }

        if (!err)
        {
            cout << "\t >> ";
            active_dataset_->display(local_id);
            cout << " <<" << endl;
        }

        if (id_seq)
            P2_term__delete(id_seq);

        P2_term__delete(term);

    }

    if (err)
    {
        char s[100];
        P2_print_error(err, s);
        cout << "\t>> Error: " << s << " <<" << endl;
    }
//cout << "- P2_evaluate_expression" << endl; cout.flush();
}



/*
  Extract command arguments from the XML and call P2_execute_command.
*/
void P2_evaluate_command(char *name, char *xmlargs)
{
    int nargs;
    char **args;

    // Instantiate a DOM object.
    struct soap soap;
    soap_init2(&soap, SOAP_IO_DEFAULT, SOAP_XML_TREE);
    soap_dom_element document(soap_new()); // create a DOM with a new soap environment
    soap_set_imode(document.soap, SOAP_DOM_TREE); // DOM tree w/o application data

    // Deserialize XML input.
    deserialize(xmlargs, &document);

    #ifdef PARSER_DEBUG
        cout << document << endl; // print it
    #endif

    nargs = 0;
    DOM_Element *cur = DOM_firstChild(&document);
    while (cur != DOM_NULL)
    {
        nargs++;
        cur = DOM_nextSibling(cur);
    }
    args = (char **) malloc(nargs * sizeof(char *));
    cur = DOM_firstChild(&document);
    int i = 0;
    while (cur != DOM_NULL)
    {
        args[i] = strdup(DOM_text(cur));
        cur = DOM_nextSibling(cur);
        i++;
    }

    // Destroy the DOM object.
    soap_destroy(document.soap); // delete entire DOM
    soap_end(document.soap); // clean up
    soap_done(document.soap); // detach the soap environment
    free(document.soap); // free the soap environment
    soap_end(&soap); // remove all temporary and deserialized data
    soap_done(&soap);

    P2_error err = P2_execute_command(name, nargs, args);
    char s[100];
    if (err)
    {
        P2_print_error(err, s);
    cout << "\t>> Error: " << s << ". <<" << endl;
    }

    // Destroy argument list.
    for (int i=0; i<nargs; i++)   //{ printf("- %s\n", args[i]);
        free(args[i]);            //}
    free(args);
}



/////////////////////////////////////////////////////////////////////////////////



P2_error P2_client_init(char *filepath)
{
    P2_error err;

    if (!(err = P2_init())
      &&!(err = P2_syntax_init())
      &&!(err = P2_command_init())
      &&!(err = SK_init(NULL, debug_print))

      &&!(err = construct_commands())
        ) err = P2_SUCCESS;

    if (!err)
    {
        all_datasets_ = hash_table__new(20, 2.0, 2.0, STRING_DEFAULTS);
        if (filepath)
    {
        char **args = new (char *)[2];
        args[0] = "default";
        args[1] = filepath;
        err = P2_load(2, args);
        delete args;
        active_dataset_ = (P2_data_set *) hash_table__lookup(all_datasets_, (void *) "default");
    }

    else
    {
        active_dataset_ = new P2_data_set(SK_reduce);
            hash_table__add(all_datasets_, (void *) strdup("default"), (void *) active_dataset_);
    }
    construct_primitives();
    }

    return err;
}



// For P2_client_end.
void destroy_dataset(char *name, P2_data_set *ds)
{
    free(name);
    delete ds;
}



P2_error P2_client_end()
{
    P2_error err = P2_SUCCESS, err2;

    // Note: attend to data sets displaced by /load...
    hash_table__forall(all_datasets_, (void (*)(void *, void *)) destroy_dataset);
    hash_table__delete(all_datasets_);

    // (g++ suggested the extra parens).
    if ((err2 = P2_command_end()))
        err = err2;
    if ((err2 = P2_syntax_end()))
        err = err2;
    if ((err2 = P2_end()))
        err = err2;

    return err;
}



/** MAIN *//////////////////////////////////////////////////////////////////////



int main(int argv, char *args[])
{
    char s[100];
    P2_error err;

    if (argv < 2)
        // Create default data set.
        err = P2_client_init(NULL);
    else
        // Load from file.
        err = P2_client_init(args[1]);

    if (err)
    {
        P2_print_error(err, s);
        cout << "\t>> Initialization error: " << s << " <<\n";
        exit(1);
    }

    // Initialization succeeded.  Start up the interface.
    else
    {
        // Print GPL notice.
        cout << endl
             << "Phase2 version 0.4, Copyright (C) 2005 Joshua Shinavier." << endl
             << "The Phase2 programming language comes with ABSOLUTELY NO WARRANTY. This is free software, and you are welcome to redistribute it under certain conditions; type '/show license;' for details." << endl;

        // Show type indices for debugging.
        #ifdef DEBUG
        int types = P2_total_types();
        for (int i=1; i<=types; i++)
            cout << "type " << i << ": " << P2_type_name((P2_type) i) << endl;
        #endif

        // Hand over control to the (flex/bison) parser.
        yyparse();
    }
}



/*- end of file */

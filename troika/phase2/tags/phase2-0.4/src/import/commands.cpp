/**

  This file is an extension of p2_client.cpp.  It defines all user commands.

  Arguments contained in the COMMAND macro:

      * int nargs -- number of arguments
      * char **args -- an array of arguments as null-terminated strings

//*//////////////////////////////////////////////////////////////////////////////



/**
    /close <dataset_name> ;
*/
COMMAND(P2_close)
{
    P2_error err;

    if (nargs > 1)
        err = TOO_MANY_ARGUMENTS;
    else if (nargs < 1)
        err = TOO_FEW_ARGUMENTS;
    else
    {
        P2_data_set *ds = (P2_data_set *) hash_table__lookup(all_datasets_, (void *) args[0]);
        if (!ds)
            err = UNKNOWN_DATASET;
        else
        {
            // Note: name string is not deallocated.
            hash_table__remove(all_datasets_, (void *) args[0]);
            if (active_dataset_ == ds)
            {
                // For now, don't close the default data set!
                active_dataset_ = (P2_data_set *) hash_table__lookup(all_datasets_, (void *) "default");

                if (!active_dataset_)
                {
                    //...
                }
            }
            delete ds;

            err = P2_SUCCESS;
        }
    }

    return err;
}



/**
    /debug on|off ;
*/
COMMAND(P2_debug)
{
    P2_error err;

    if (nargs > 1)
        err = TOO_MANY_ARGUMENTS;
    else if (nargs < 1)
        err = TOO_FEW_ARGUMENTS;
    else if (!strcmp(args[0],"on"))
    {
        active_dataset_->debug = true;
        err = P2_SUCCESS;
    }
    else if (!strcmp(args[0],"off"))
    {
        active_dataset_->debug = false;
        err = P2_SUCCESS;
    }
    else
        err = BAD_ARGUMENT;

    return err;
}



/**
    /exit ;
*/
COMMAND(P2_exit)
{
    P2_error err = P2_SUCCESS;

    if (nargs)
        err = TOO_MANY_ARGUMENTS;

    else
    {
        P2_client_end();

        exit(0);
    }

    return err;  // Usually unreachable, but syntactically necessary.
}



/**
    /forget [dataset_id:]<local_id> ;
*/
COMMAND(P2_forget)
{
    P2_error err;

    if (nargs > 1)
        err = TOO_MANY_ARGUMENTS;
    else if (nargs < 1)
        err = TOO_FEW_ARGUMENTS;
    else
    {
        SEQUENCE *id_seq = P2_parse_identifier(args[0]);
    if ((P2_type) id_seq->type == P2_error_type)
        err = (P2_error) id_seq->value;
    else
    {
        P2_identifier *id = (P2_identifier *) id_seq->value;
        sequence__delete(id_seq);
        P2_data_set *ds;
        if (id->dataset_id)
                ds = (P2_data_set *) hash_table__lookup(all_datasets_, (void *) id->dataset_id);
            else
            ds = active_dataset_;
        if (!ds)
            err = UNKNOWN_DATASET;
        else
        {
                ds->remove(id->local_id);
                err = P2_SUCCESS;
            }
        P2_destroy((P2_atom) id, P2_id_type);
    }
    }

    return err;
}



/**
    /load <dataset_name> <path> ;
*/
COMMAND(P2_load)
{
    P2_error err;

    if (nargs > 2)
        err = TOO_MANY_ARGUMENTS;
    else if (nargs < 2)
        err = TOO_FEW_ARGUMENTS;
    else
    {
        // Instantiate a DOM object.
        struct soap soap;
        soap_init2(&soap, SOAP_IO_DEFAULT, SOAP_XML_TREE);
        soap_dom_element document(soap_new()); // create a DOM with a new soap environment
        soap_set_imode(document.soap, SOAP_DOM_TREE); // DOM tree w/o application data

        ifstream in(args[1]);
        in >> document; // parse
//ofstream out("trash.txt");
//DOM_print(&document, out, 0);
//out.close();
        in.close();

        P2_data_set *ds = new P2_data_set(SK_reduce, &document);
        hash_table__add(all_datasets_, (void *) strdup(args[0]), (void *) ds);

        // Destroy the DOM object.
        soap_destroy(document.soap); // delete entire DOM
        soap_end(document.soap); // clean up
        soap_done(document.soap); // detach the soap environment
        free(document.soap); // free the soap environment
        soap_end(&soap); // remove all temporary and deserialized data
        soap_done(&soap);

        err = P2_SUCCESS;
    }

    return err;
}



#ifdef MARK_AND_SWEEP
void dataset__mark_all(P2_data_set *ds)
{
    ds->mark_all();
}
/**
    /markandsweep ;
*/
COMMAND(P2_markandsweep)
{
    P2_error err = P2_SUCCESS;

    if (nargs)
        err = TOO_MANY_ARGUMENTS;

    else
    {
        // For all data sets
    hash_table__forall_targets(all_datasets_, (void (*)(void *)) dataset__mark_all);
    sequence__sweep();
    }

    return err;
}
#endif



/**
    /ref <dataset_name> ;
*/
COMMAND(P2_ref)
{
    P2_error err;

    if (nargs > 1)
        err = TOO_MANY_ARGUMENTS;
    else if (nargs < 1)
        err = TOO_FEW_ARGUMENTS;
    else
    {
        P2_data_set *ds = (P2_data_set *) hash_table__lookup(all_datasets_, (void *) args[0]);
        if (!ds)
        err = UNKNOWN_DATASET;
        else
        {
            active_dataset_ = ds;
            err = P2_SUCCESS;
        }
    }

    return err;
}



/**
    /save [dataset_name] <path> ;
*/
COMMAND(P2_save)
{
    P2_error err;

    if (nargs > 1)
        err = TOO_MANY_ARGUMENTS;
    else if (nargs < 1)
        err = TOO_FEW_ARGUMENTS;
    else
    {
        // Instantiate a DOM object.
        struct soap soap;
        soap_init2(&soap, SOAP_IO_DEFAULT, SOAP_XML_TREE);
        soap_dom_element document(soap_new()); // create a DOM with a new soap environment
        soap_set_imode(document.soap, SOAP_DOM_TREE); // DOM tree w/o application data

        DOM_Element *el = active_dataset_->encode(&document);
        ofstream out(args[0]);
        //out << *el;
        DOM_print(el, out, 0);
        out.close();

        // Destroy the DOM object.
        soap_destroy(document.soap); // delete entire DOM
        soap_end(document.soap); // clean up
        soap_done(document.soap); // detach the soap environment
        free(document.soap); // free the soap environment
        soap_end(&soap); // remove all temporary and deserialized data
        soap_done(&soap);

        err = P2_SUCCESS;
    }

    return err;
}



ARRAY *dataset_names_;
void add_name_to_array(void *name)
{
    array__enqueue(dataset_names_, name);
}
/**
    /show env|license|(dictionary [dataset_name]) ;
*/
COMMAND(P2_show)
{
    P2_error err = P2_SUCCESS;

    if (nargs > 2)
        err = TOO_MANY_ARGUMENTS;
    else if (nargs < 1)
        err = TOO_FEW_ARGUMENTS;
    else if (!strcmp(args[0], "dict"))
    {
        if (nargs == 1)
            active_dataset_->display_dictionary();

        else
        {
            P2_data_set *ds = (P2_data_set *) hash_table__lookup(all_datasets_, (void *) args[1]);
            if (!ds)
                err = UNKNOWN_DATASET;
            else
                ds->display_dictionary();
        }
    }
    else if (!strcmp(args[0], "license"))
    {
        char buff[500];
        ifstream in("gpl.txt");
        while (!in.eof())
        {
            in.getline(buff, 500);
            cout << buff << endl;
        }
        in.close();
    }
    else if (!strcmp(args[0], "env"))
    {
        int size = all_datasets_->size;
        dataset_names_ = array__new(size, 2.0);
        hash_table__forall_keys(all_datasets_, add_name_to_array);
    array__mergesort(dataset_names_, (int (*) (void *, void *)) strcmp);
        cout << size << " active data set(s):";
        cout << " " << (char *) array__get(dataset_names_, 0);
    for (int i=1; i<size; i++)
        cout << ", " << (char *) array__get(dataset_names_, i);
    array__delete(dataset_names_);
    cout << endl;
    #ifdef MARK_AND_SWEEP
        cout << "\"Mark and sweep\" buffer contains " << sequence__all_atoms() << " items." << endl;
    #endif
    }
    else
        err = BAD_ARGUMENT;

    return err;
}



/////////////////////////////////////////////////////////////////////////////////



P2_error construct_commands()
{
    P2_error err;

    // Name all available commands and add them to the "commands" dictionary.
    if (1
      &&!(err = P2_register_command("close", P2_close))
      &&!(err = P2_register_command("debug", P2_debug))
      &&!(err = P2_register_command("exit", P2_exit))
      &&!(err = P2_register_command("forget", P2_forget))
      &&!(err = P2_register_command("load", P2_load))
      #ifdef MARK_AND_SWEEP
      &&!(err = P2_register_command("markandsweep", P2_markandsweep))
      #endif
      &&!(err = P2_register_command("ref", P2_ref))
      &&!(err = P2_register_command("save", P2_save))
      &&!(err = P2_register_command("show", P2_show))
      ) err = P2_SUCCESS;

    return err;
}

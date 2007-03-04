var Wurfel = new Object();


Wurfel.initialize = function()
{
alert( "can you see me?" );



    try {
        var foo = this.getFoo();

        if (!foo.wrappedJSObject.initialize(java, true)) {
            alert(foo.wrappedJSObject.error);
        }
    } catch (e) {
        this._fail(e);
    }



/*
   var cl = new Packages.java.net.URLClassLoader(
       [
           new Packages.java.net.URL(
               'http://foo.net/bar.jar')
       ]
   );

   var aClass = Packages.java.lang.Class.forName("HelloWorld", true, cl);
   var aStaticMethod = aClass.getMethod("getGreeting", []);
   var greeting = aStaticMethod.invoke(null, []);
   alert(greeting);
*/

//    window.addEventListener("load", PiggyBank.chromeLoad, false);
//    window.addEventListener("unload", PiggyBank.chromeUnload, false);
}




Wurfel.doIt = function() {

    try {
        var foo = this.getFoo();

        var test = foo.wrappedJSObject.getTest();
        test.count();

        alert(test.getCount());
    } catch (e) {
        this._fail(e);
    }

//alert( "You done it." );
}


Wurfel.getFoo = function() {
    return Components.classes["@wurfel/wurfel;1"]
        .getService(Components.interfaces.nsIHelloWorld);
}


Wurfel._trace = function( msg )
{
    Components.classes["@mozilla.org/consoleservice;1"]
        .getService( Components.interfaces.nsIConsoleService )
            .logStringMessage( msg );
}


Wurfel._fail = function( e )
{
    var msg;

    if ( e.getMessage )
    {
        msg = e + ": " + e.getMessage() + "\n";
        while (e.getCause() != null)
        {
            e = e.getCause();
            msg += "caused by " + e + ": " + e.getMessage() + "\n";
        }
    }

    else
        msg = e;

    alert( msg );
}


// kate: space-indent on; indent-width 4; tab-width 4; replace-tabs on

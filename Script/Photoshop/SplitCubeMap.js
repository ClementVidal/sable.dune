/*
Script photoshop qui découpe automatiquement un document représentant une cube en 6 documents séparés.

Installation:
Placer le script dans InstallPhotoshop\Presets\Scripts

*/

var pixelSize = 0;

function Main()
{
    if (!app.documents.length > 0) 
    {    // stop if no document is opened.
        alert("Sorry, No Current Document");
    } 
    else
    {
        // Don't display dialogs
        app.displayDialogs = DialogModes.NO;
        
        pixelSize = prompt( "Face pixel pixelSize:", 512, "Dune" );
        
        if( pixelSize == null )
        {
            return;
        }
        
        pixelSize = parseInt( pixelSize );
        var origDoc = app.activeDocument;
        
        // Extract Faces 
        
        // Z+
        ExtractFace( origDoc, new Array( pixelSize, pixelSize ), "Z+" );
        // Z-
        ExtractFace( origDoc, new Array( pixelSize, pixelSize * 3 ), "Z-" );
        // Y+
        ExtractFace( origDoc, new Array( pixelSize, 0 ), "Y+" );
        // Y-
        ExtractFace( origDoc, new Array( pixelSize, pixelSize * 2 ), "Y-" );
        // X+
        ExtractFace( origDoc, new Array( pixelSize * 2, pixelSize ), "X+" );
        // X-
        ExtractFace( origDoc, new Array( 0, pixelSize ), "X-" );        
    }
    
}

function ExtractFace( doc, origin, name )
{
    var a = origin;
    var b = new Array(origin[0] + pixelSize, origin[1]);
    var c = new Array(origin[0] + pixelSize, origin[1] + pixelSize);
    var d = new Array(origin[0], origin[1] + pixelSize);        

    app.activeDocument = doc;
    doc.selection.deselect();
    doc.selection.select( new Array(a,b,c,d) );
    doc.selection.copy( );
    
    var newDoc = app.documents.add( pixelSize, pixelSize, 72, name, NewDocumentMode.RGB, DocumentFill.TRANSPARENT);
    app.activeDocument = newDoc;
    newDoc.paste();
}
 
Main();
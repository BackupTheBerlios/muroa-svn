#include "cnavapp.h"
#include <qpe/qpeapplication.h>
#include <stdlib.h>

int main( int argc, char ** argv )
{
    system("cardctl resume");


    QPEApplication a( argc, argv );

    CNavApp mw;
    a.showMainDocumentWidget( &mw );


    return a.exec();
}

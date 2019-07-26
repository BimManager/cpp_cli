/*
 *  main.cpp
 */

 #include "treeview.h"

 int    main(void)
 {
    MgrTreeView    ^tr;

    tr = gcnew MgrTreeView();
    Application::Run(tr);
    return (0);
 }
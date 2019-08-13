/*
 *  prmsmgr.cpp 
 */

#include "prmsmgr.h"
#include "prmsmgr_form.h"

using namespace Esta;
using namespace Esta::Commands;
using namespace Esta::ParametersMgnt;

UI::Result PrmMgrCmd::Execute(UI::ExternalCommandData ^commandData,
    String ^%message, DB::ElementSet ^elements)
{
    ParamManager        ^mgr;
    DB::Transaction     ^tr;
    Gui::FilePicker     ^dlg;
    
    mgr = gcnew ParamManager(commandData->Application->Application,
            commandData->Application->ActiveUIDocument->Document);
    dlg = gcnew Gui::FilePicker();
    dlg->DialogDismissed +=
        gcnew System::EventHandler(
            mgr, &ParamManager::RespondToEvent);
    dlg->ShowDialog();
    return (UI::Result::Succeeded);
}

ParamManager::ParamManager(Application ^app, Document ^doc)
{
    this->_app = app;
    this->_doc = doc;
}

String ^ParamManager::GenerateHeader(void)
{
    return (String::Format("# This is a Revit shared parameter file.\n"
        "# Do not edit manually.\n*META\tVERSION\tMINVERSION\n"
        "META\t2\t1\n*GROUP\tID\tNAME\n"
        "GROUP\t1\tExported Parameters\n"
        "*PARAM\tGUID\tNAME\tDATATYPE\tDATACATEGORY\tGROUP\tVISIBLE\t"
        "DESCRIPTION\tUSERMODIFIABLE\tPARAMETERGROUP\tKIND\tCATEGORIES\t"
        "VARIESACROSSGROUPS"));
}

/* Exports only a shared parameter */
void    ParamManager::ExportParameter(
            DB::DefinitionBindingMapIterator ^it,
            IO::StreamWriter ^sw,
            Document    ^doc
            )
{
    DB::InternalDefinition              ^def;
    DB::ElementBinding                  ^binding;
    DB::SharedParameterElement          ^param;

    def = (DB::InternalDefinition ^)it->Key;
    binding = (DB::ElementBinding ^)it->Current;
    if (((DB::InternalDefinition ^)def)->BuiltInParameter
                != DB::BuiltInParameter::INVALID)
        return ;
    param = RETURN_SP_ELEM(doc, def);
    if (param == nullptr)
        return ;
    sw->WriteLine(String::Format(FORMAT,
        param->GuidValue.ToString(), def->Name, def->ParameterType,
        String::Empty, PARAM_GROUP_NUMBER, 
        def->Visible ? "1" : "0", PARAM_DESCRIPTION_TXT, "1", 
        def->ParameterGroup,
        dynamic_cast<DB::TypeBinding ^>(binding) != nullptr ? 
        PARAM_TYPE : PARAM_INSTANCE,
        CategoriesToString(binding->Categories), 
        def->VariesAcrossGroups ? "1" : "0"));
}

void    ParamManager::ExportParameters(String ^filename, Document ^doc)
{
    IO::StreamWriter                    ^sw;
    CL::IEnumerator                     ^it;

    #ifdef DEBUG
    Debug::WriteLine("Document to export from: " + doc->Title);
    #endif

    sw = gcnew IO::StreamWriter(filename, true, System::Text::Encoding::Unicode);
    sw->WriteLine(ParamManager::GenerateHeader());
    if (!doc->IsFamilyDocument)
    {
        it = doc->ParameterBindings->ForwardIterator();
        while (it->MoveNext())
            this->ExportParameter((DB::DefinitionBindingMapIterator ^)it, sw, doc);
    }
    else
    {
        it = ParamManager::GetSharedParameterElements(doc)->GetEnumerator();
        while (it->MoveNext())
            this->ExportFamilyParameter(doc, 
                (DB::SharedParameterElement ^)it->Current, sw);
    }
    sw->Close();
}

int    ParamManager::ImportParameters(String ^filepath, Application ^app, Document ^doc)
{
    IO::StreamReader    ^sr;
    String              ^line;
    DB::Definitions     ^defs;
    String              ^spfile;
    char                isfamdoc;

    spfile = app->SharedParametersFilename;
    app->SharedParametersFilename = filepath;
    try 
    {
        app->OpenSharedParameterFile();
    }
    catch (Autodesk::Revit::Exceptions::InternalException ^ex)
    {
        return (OPENING_FILE_ERROR);
    }
    defs = app->OpenSharedParameterFile()->Groups
                ->Item[PARAM_GROUP_NAME]->Definitions;
    sr = gcnew IO::StreamReader(filepath, System::Text::Encoding::Unicode);
    isfamdoc = doc->IsFamilyDocument ? 1 : 0;
    while ((line = sr->ReadLine()) != nullptr)
    {
        if (line->StartsWith("PARAM"))
        {
            if (isfamdoc)
                this->BindFamilyParameter(doc, line, defs);
            else                    
                this->BindParameter(line, defs, doc);
        }
    }
    app->SharedParametersFilename = spfile;
    return (SUCCESS);
}

DB::CategorySet ^ParamManager::StringToCategories(String ^css, DB::Document ^doc)
{
    array<String ^> ^cats;
    DB::Category            ^cat;
    DB::CategorySet         ^set;
    size_t                  i;
    DB::BuiltInCategory     ecat;

    cats = css->Split(';');
    set = gcnew DB::CategorySet();
    i = -1;
    while (++i < cats->Length)
    {
        if (!TRY_PARSE_ENUM(cats[i], true, ecat))
            continue ;
        cat = doc->Settings->Categories->Item[ecat];
        if (cat->AllowsBoundParameters)                    
            set->Insert(cat);
    }
    return (set);
}

void    ParamManager::BindParameter(String ^line, DB::Definitions ^defs, Document ^doc)
{
    array<String ^>             ^tab;
    DB::ElementBinding          ^binding;
    DB::CategorySet             ^cats;
    DB::BuiltInParameterGroup   paramgrp;

    tab = line->Split('\t');
    cats = StringToCategories(tab[PARAM_CATEGORIES], doc);
    if (tab[PARAM_KIND] == "T")
        binding = gcnew DB::TypeBinding(cats);
    else            
        binding = gcnew DB::InstanceBinding(cats);
    if (TRY_PARSE_ENUM(tab[PARAM_PARAMETER_GROUP], true, paramgrp) && !cats->IsEmpty)
        doc->ParameterBindings
            ->Insert(defs->Item[tab[PARAM_NAME]], binding, paramgrp);
}

String  ^ParamManager::CategoriesToString(DB::CategorySet ^set)
{
    StrBuilder              ^gen;
    DB::CategorySetIterator ^it;

    gen = gcnew StrBuilder();
    it = set->ForwardIterator();
    while (it->MoveNext())
        gen->AppendFormat("{0};", ((DB::BuiltInCategory)
            (((DB::Category ^)it->Current)->Id->IntegerValue)).ToString());
    if (gen->Length > 1)
        gen->Remove(gen->Length - 1, 1);
    return (gen->ToString());
}

Document    ^ParamManager::GetDocument(ModelPath ^modelPath)
{
    DB::WorksetConfiguration    ^worksetConfig;
    DB::OpenOptions             ^openOpts;
    
    openOpts = gcnew DB::OpenOptions();
    worksetConfig = gcnew DB::WorksetConfiguration(
        DB::WorksetConfigurationOption::CloseAllWorksets);
    openOpts->SetOpenWorksetsConfiguration(worksetConfig);
    return (this->_app->OpenDocumentFile(modelPath, openOpts));
}

void    ParamManager::ImportFromFile(String ^filepath, Document ^doc)
{
    DB::Transaction                     ^tr;

    tr = gcnew DB::Transaction(doc);
    tr->Start("Binding shared parameters");
    if (this->ImportParameters(filepath, this->_app, doc) == SUCCESS) 
    {
        tr->Commit();
        UI::TaskDialog::Show("Status", "The import has been completed.");
        return ;
    }
    tr->RollBack();
    UI::TaskDialog::Show("Error", "The import has failed because " + 
        "the shared parameter file could not be processed or " + 
        "is not a valid shared parameter file.");
}

void     ParamManager::ExportToFile(String ^filepath, Document ^doc)
{
    this->ExportParameters(filepath, doc);
    UI::TaskDialog::Show("Status", "The export has been completed.");
}

void    ParamManager::ImportFromModel(ModelPath ^modelpath, Document ^activeDoc)
{
    Document    ^fromDoc;

    fromDoc = this->GetDocument(modelpath);
    if (fromDoc != nullptr)
    {
        String  ^tmpfile;

        tmpfile = CreateTempFile();
        this->ExportParameters(tmpfile, fromDoc);
        fromDoc->Close(false);
        this->ImportFromFile(tmpfile, activeDoc);
    }
}

void    ParamManager::RespondToEvent(System::Object ^s, System::EventArgs ^e)
{
    EventData::DismissedDialogEventArgs ^args;
    DB::Transaction                     ^tr;

    args = (EventData::DismissedDialogEventArgs ^)e;
    switch (args->GetAction())
    {
        case ACTION_IMPORT_FROM_FILE:
            ImportFromFile(args->GetFilepath(), this->_doc);
            break ;
        case ACTION_IMPORT_FROM_MODEL:
            ImportFromModel(args->GetModelPath(), this->_doc);
            break ;
        case ACTION_EXPORT_TO_FILE:
            ExportToFile(args->GetFilepath(), this->_doc);
            break ;
        case ACTION_EXPORT_TO_MODEL:
            break ;
    }
}

GCL::ICollection<DB::Element ^> 
    ^ParamManager::GetSharedParameterElements(DB::Document ^doc)
{
    DB::FilteredElementCollector    ^col;
    DB::ElementClassFilter          ^classFltr;

    col = gcnew DB::FilteredElementCollector(doc);
    classFltr = gcnew DB::ElementClassFilter(DB::SharedParameterElement::typeid);
    return (col->WherePasses(classFltr)->ToElements());
}

void    ParamManager::ExportFamilyParameter(Document ^doc,
    DB::SharedParameterElement ^spElem, IO::StreamWriter ^sw)
{
    DB::InternalDefinition  ^def;
    DB::FamilyParameter     ^fParam;

    def = spElem->GetDefinition();
    fParam = doc->FamilyManager->Parameter[def];
    sw->WriteLine(String::Format(FORMAT,
        spElem->GuidValue,
        def->Name, def->ParameterType, String::Empty,
        PARAM_GROUP_NUMBER, def->Visible ? "1" : "0", String::Empty,
        fParam != nullptr ? (fParam->UserModifiable ? "1" : "0") : PARAM_INVALID,
        def->ParameterGroup,
        fParam != nullptr ? (fParam->IsInstance ?
            PARAM_INSTANCE : PARAM_TYPE) : PARAM_INVALID,
        String::Empty, String::Empty));
}

void    ParamManager::BindFamilyParameter(Document ^doc,
            String ^line, DB::Definitions ^defs)
{
    array<String ^>             ^tab;
    DB::FamilyManager           ^fm;
    DB::BuiltInParameterGroup   pg;

    fm = doc->FamilyManager;
    tab = line->Split('\t');
    if (System::Enum::TryParse(tab[PARAM_PARAMETER_GROUP], pg) 
        && tab[PARAM_KIND] != "-1")
        fm->AddParameter(
            (DB::ExternalDefinition ^)defs->Item[tab[PARAM_NAME]],
            pg,
            tab[PARAM_KIND] == "I" ? 1 : 0);
}

String  ^Esta::CreateTempFile(void)
{
    IO::FileStream  ^fs;
    String          ^filename;

    filename = System::IO::Path::GetTempPath() + 
        System::Guid::NewGuid() + ".txt";
    fs = IO::File::Create(filename);
    fs->Close();
#ifdef DEBUG
    Debug::WriteLine("Filename: " + filename);
#endif
    return (filename);
}


/*DB::DefinitionFile ^CreateTempDefFile(AS::Application ^app)
{
    IO::FileStream      ^fs;
    String      ^filename;

    filename = System::IO::Path::GetTempPath() + 
        System::Guid::NewGuid() + ".txt";
    fs = IO::File::Create(filename);
    fs->Close();
    app->SharedParametersFilename = filename;
    return (app->OpenSharedParameterFile());
}*/

/*void    ParamManager::ExportFamilyParameter(
            DB::FamilyParameter ^param, IO::StreamWriter ^sw)
{
    DB::InternalDefinition  ^def;

    if (!param->IsShared)
        return ;
    def = (DB::InternalDefinition ^)param->Definition;
    sw->WriteLine(String::Format(FORMAT,
        param->GUID.ToString(),
        def->Name, def->ParameterType, String::Empty,
        "1", def->Visible ? "1" : "0", String::Empty,
        param->UserModifiable ? "1" : "0",
        def->ParameterGroup,
        param->IsInstance ? "I" : "T",
        String::Empty, String::Empty));
}*/
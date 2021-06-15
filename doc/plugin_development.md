# Plugin development {SpaceCreatorPluginPage}

## Creating a independent plugin

As SpaceCretor is using QtCreator, such a plugin is a normal QtCreator plugin. Developing such a plugin is described in [Creating Plugins](https://doc.qt.io/qtcreator-extending/creating-plugins.html).

One example of a "standalone" plugin is `src/qtcreator/asn1plugin`.

## Creating a SpaceCreator plugin

SpaceCreator is one plugin for QtCreator. In that plugin several editors are implemented. Here are the steps to add one more editor to SpaceCreator:

### Add directory/files

All editors have their implementation in a separte directory in `src/qtcreator/spacecreatorplugin`. There the relevant classes/files are:

* `*QtCEditor` is derived from `Core::IEditor` and is the class for one editor/file
* `*MainWidget` is a `QWidget` based class, containing the whole UI for one editor
* `*EditorDocument`  is derived from `Core::IDocument` and contains the no UI data/logic of one editor
* `*EditorFactory` is derived from  `Core::IEditorFactory` and creates one editor for one file
* `*EditorData`is ised to integrate into QtCreator for features like undo/redo

### Constants

In file `spacecreatorpluginconstants.h` several constants for QtCreator, menus and settings are needed.

### Add mime type

To support one file extenstion in the editor, that support needs to be marked/added in

* spacecreatorpluginconstants.h
* SpaceCreatorPlugin.json.in
* `*editordocument.cpp`

### Use the plugin

In `SpaceCreatorPlugin` add a factory object.

In `SpaceCreatorPlugin::initialize()` menu entries can be set up.

In `SpaceCreatorPlugin::addHelp()` help for the editor can be registered.

### QtCreator options/settings

To add a settings entry in the QtCreator settings (Menu -> Tools -> Options...), add the settings Ui in `src/qtcreator/spacecreatorplugin/options`.

Sub-class from `Core::IOptionsPage` and implement the public methods `QWidget *widget()`, `void apply()` and `void finish()`. And add and instance of it in `SpaceCreatorPlugin`. Nothing else is needed for the settings UI.

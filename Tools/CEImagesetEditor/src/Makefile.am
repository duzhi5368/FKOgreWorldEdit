INCLUDES = $(CEGUI_CFLAGS) $(WX_CPPFLAGS) -DEDITOR_DATA_DIR="$(pkgdatadir)"

bin_PROGRAMS = CEImagesetEditor
CEImagesetEditor_SOURCES = DialogAbout.cpp \
                           DialogResourceGroups.cpp \
                           ElasticBox.cpp \
                           ElasticBoxProperties.cpp \
                           ElasticBoxWindowRenderer.cpp \
                           CEImagesetEditor.cpp \
                           EditorDocument.cpp \
                           EditorFrame.cpp \
                           EditorGLCanvas.cpp \
                           EditorView.cpp \
                           ImagesetHandler.cpp \
                           PropertiesPanel.cpp \
                           wxPathCellEditor.cpp \
                           DialogAbout.h \
                           DialogResourceGroups.h \
                           ElasticBox.h \
                           ElasticBoxProperties.h \
                           ElasticBoxWindowRenderer.h \
                           CEGUIHelper.h \
                           CEImagesetEditor.h \
                           EditorDocument.h \
                           EditorFrame.h \
                           EditorGLCanvas.h \
                           EditorView.h \
                           ImagesetHandler.h \
                           PropertiesPanel.h \
                           wxPathCellEditor.h

CEImagesetEditor_LDADD = $(CEGUI_LIBS) $(WX_LIBS)

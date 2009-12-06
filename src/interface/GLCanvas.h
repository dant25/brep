#ifndef _glcanvas_
#define _glcanvas_

#include "wx/wx.h"
#include "wx/glcanvas.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <list>
#include <set>
#include "../Scene.h"
#include "../Camera.h"
#include "dialog/BRepEdit_Dialog.h"
#include "dialog/BRepRotate_Dialog.h"


enum Mode {EDIT, DRAW};


class GLCanvas : public wxGLCanvas
{
    friend class CSGFrame;
public:
	GLCanvas(wxWindow* parent, wxWindowID id = -1, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style=0, const wxString& name=_T("GLCanvas"), int* attribList = 0);
    ~GLCanvas();

    void init();
    void initEdit();
    void initDraw();

    void render();
    void renderBackground();
    void renderEditBackground();

    void setRenderSolid();
    void setRenderWireframe();


    void clear();

    void addCube(float minX, float minY, float minZ, float size);
    void addCorner(float minX, float minY, float minZ,
                    float midX, float midY, float midZ,
                    float maxX, float maxY, float maxZ);
    void addCylinder(float pX, float pY, float pZ, float radius, float height, int disc);
    void addSphere(float pX, float pY, float pZ, float radius, int disc);

    void setMeshSelect();

    void setFaceSelect();

    void setEdgeSelect();

    void setVertexSelect();

    void switchMode();
    void setStartLineLoop();
    void doneDrawing();

    void _addCube(wxCommandEvent& event);
    void _addCorner(wxCommandEvent& event);
    void _addCylinder(wxCommandEvent& event);
    void _addSphere(wxCommandEvent& event);
    void _deleteMesh(wxCommandEvent& event);
    void _move(wxCommandEvent& event);
    void _neighborhood(wxCommandEvent& event);
    void _rotate(wxCommandEvent& event);
    void _scale(wxCommandEvent& event);
    void _translate(wxCommandEvent& event);

protected:
		enum
		{
            ID_ADD_CUBE = 1000,
            ID_ADD_CORNER,
            ID_ADD_CYLINDER,
            ID_ADD_SPHERE,
            ID_DELETE_MESH,
            ID_MOVE,
            ID_NEIGHBORHOOD,
            ID_ROTATE,
            ID_SCALE,
            ID_TRANSLATE,
		};

private:
    void onPaint(wxPaintEvent& event);
    void onSize(wxSizeEvent& event);
    void onEraseBackground(wxEraseEvent& event);
    void onEnterWindow(wxMouseEvent& event);
    void onMouseLeftUp(wxMouseEvent& event);
    void onMouseLeftDown(wxMouseEvent& event);
    void onMouseMove(wxMouseEvent& event);
    void onMouseWheel(wxMouseEvent& event);
    void menu(wxMouseEvent& event);
    DECLARE_EVENT_TABLE()

    void selectPicking(int x, int y);
    void draw(int x, int y);
    bool selectMesh;
    bool selectFace;
    bool selectEdge;
    bool selectVertex;
    bool startLineLoop;

    int numPts;

    Mode currentMode;

private:
    std::set<Mesh*> meshList;
    std::set<Loop*> faceList;
    std::set<Edge*> edgeList;
    std::set<Vertex*> vertexList;

    wxGLContext *glContext;
    Scene scene;
    Scene drawScene;
    Camera camera;
    wxMenu* option_menu;
    wxMenu* option_menu_mesh;
};

#endif

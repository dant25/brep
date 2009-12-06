#include "GLCanvas.h"
#include <iostream>

BEGIN_EVENT_TABLE(GLCanvas, wxGLCanvas)
    EVT_RIGHT_DOWN(GLCanvas::menu)
    EVT_SIZE(GLCanvas::onSize)
    EVT_PAINT(GLCanvas::onPaint)
    EVT_ERASE_BACKGROUND(GLCanvas::onEraseBackground)
    EVT_ENTER_WINDOW(GLCanvas::onEnterWindow)
    EVT_LEFT_UP(GLCanvas::onMouseLeftUp)
    EVT_LEFT_DOWN(GLCanvas::onMouseLeftDown)
    EVT_MOTION(GLCanvas::onMouseMove)
    EVT_MOUSEWHEEL( GLCanvas::onMouseWheel )
    EVT_MENU(ID_ADD_CUBE, GLCanvas::_addCube)
    EVT_MENU(ID_ADD_CORNER, GLCanvas::_addCorner)
    EVT_MENU(ID_ADD_CYLINDER, GLCanvas::_addCylinder)
    EVT_MENU(ID_ADD_SPHERE, GLCanvas::_addSphere)
    EVT_MENU(ID_DELETE_MESH, GLCanvas::_deleteMesh)
    EVT_MENU(ID_EDIT, GLCanvas::_edit)
    EVT_MENU(ID_NEIGHBORHOOD, GLCanvas::_neighborhood)
    EVT_MENU(ID_ROTATE, GLCanvas::_rotate)
    EVT_MENU(ID_SCALE, GLCanvas::_scale)
    EVT_MENU(ID_TRANSLATE, GLCanvas::_translate)
END_EVENT_TABLE()

GLCanvas::GLCanvas(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style, const wxString& name, int* attribList) :
wxGLCanvas(parent, id, pos, size, style, name,  attribList)
{
    glContext = new wxGLContext(this);

    currentMode = EDIT;

    selectFace = false;
    selectMesh = false;
    selectEdge = false;
    selectVertex = true;

    startLineLoop = false;
    numPts = 0;

    //MENU
    option_menu = new wxMenu();
    option_menu_mesh = new wxMenu();
//------------------------------------------
    wxMenu* primitive_menu = new wxMenu();

    wxMenuItem* cube_menuItem;
    cube_menuItem = new wxMenuItem( primitive_menu, ID_ADD_CUBE, wxString( wxT("Cube") ) , wxEmptyString, wxITEM_NORMAL );
    primitive_menu->Append( cube_menuItem );

    wxMenuItem* cylinder_menuItem;
    cylinder_menuItem = new wxMenuItem( primitive_menu, ID_ADD_CYLINDER, wxString( wxT("Cylinder") ) , wxEmptyString, wxITEM_NORMAL );
    primitive_menu->Append( cylinder_menuItem );

    wxMenuItem* corner_menuItem;
    corner_menuItem = new wxMenuItem( primitive_menu, ID_ADD_CORNER, wxString( wxT("Corner") ) , wxEmptyString, wxITEM_NORMAL );
    primitive_menu->Append( corner_menuItem );

    wxMenuItem* sphere_menuItem;
    sphere_menuItem = new wxMenuItem( primitive_menu, ID_ADD_SPHERE, wxString( wxT("Sphere") ) , wxEmptyString, wxITEM_NORMAL );
    primitive_menu->Append( sphere_menuItem );

    option_menu->Append( -1, wxT("Add primitive"), primitive_menu );
//-------------------------------------------
    wxMenu* primitive_menu_mesh = new wxMenu();

    wxMenuItem* cubeMesh_menuItem;
    cubeMesh_menuItem = new wxMenuItem( primitive_menu, ID_ADD_CUBE, wxString( wxT("Cube") ) , wxEmptyString, wxITEM_NORMAL );
    primitive_menu_mesh->Append( cubeMesh_menuItem );

    wxMenuItem* cylinderMesh_menuItem;
    cylinderMesh_menuItem = new wxMenuItem( primitive_menu, ID_ADD_CYLINDER, wxString( wxT("Cylinder") ) , wxEmptyString, wxITEM_NORMAL );
    primitive_menu_mesh->Append( cylinderMesh_menuItem );

    wxMenuItem* cornerMesh_menuItem;
    cornerMesh_menuItem = new wxMenuItem( primitive_menu, ID_ADD_CORNER, wxString( wxT("Corner") ) , wxEmptyString, wxITEM_NORMAL );
    primitive_menu_mesh->Append( cornerMesh_menuItem );

    wxMenuItem* sphereMesh_menuItem;
    sphereMesh_menuItem = new wxMenuItem( primitive_menu, ID_ADD_SPHERE, wxString( wxT("Sphere") ) , wxEmptyString, wxITEM_NORMAL );
    primitive_menu_mesh->Append( sphereMesh_menuItem );

    option_menu_mesh->Append( -1, wxT("Add primitive"), primitive_menu_mesh );
//------------------------------------------
    wxMenuItem* rotate_menuItem;
    rotate_menuItem = new wxMenuItem( primitive_menu, ID_ROTATE, wxString( wxT("Rotate") ) , wxEmptyString, wxITEM_NORMAL );
    option_menu_mesh->Append( rotate_menuItem );

    wxMenuItem* scale_menuItem;
    scale_menuItem = new wxMenuItem( primitive_menu, ID_SCALE, wxString( wxT("Scale") ) , wxEmptyString, wxITEM_NORMAL );
    option_menu_mesh->Append( scale_menuItem );

    wxMenuItem* translate_menuItem;
    translate_menuItem = new wxMenuItem( primitive_menu, ID_TRANSLATE, wxString( wxT("Translate") ) , wxEmptyString, wxITEM_NORMAL );
    option_menu_mesh->Append( translate_menuItem );

    wxMenuItem* neighborhood_menuItem;
    neighborhood_menuItem = new wxMenuItem( primitive_menu, ID_NEIGHBORHOOD, wxString( wxT("Neighborhood") ) , wxEmptyString, wxITEM_NORMAL );
    option_menu->Append( neighborhood_menuItem );

    wxMenuItem* edit_menuItem;
    edit_menuItem = new wxMenuItem( primitive_menu, ID_EDIT, wxString( wxT("Edit") ) , wxEmptyString, wxITEM_NORMAL );
    option_menu->Append( edit_menuItem );

    wxMenuItem* delete_menuItem;
    delete_menuItem = new wxMenuItem( primitive_menu, ID_DELETE_MESH, wxString( wxT("Delete") ) , wxEmptyString, wxITEM_NORMAL );
    option_menu_mesh->Append( delete_menuItem );

}

GLCanvas::~GLCanvas()
{
    delete glContext;
    glContext = NULL;
}

void GLCanvas::switchMode() {
    if (currentMode == EDIT) {
        std::cout << "modo de desneho" << std::endl;
        currentMode = DRAW;
        initDraw();
    } else {
        std::cout << "modo de edição" << std::endl;
        currentMode = EDIT;
        initEdit();
    }
}

void GLCanvas::setStartLineLoop() {
    startLineLoop = true;
}

void GLCanvas::doneDrawing() {
    if(numPts > 0) {
        //Erro: uma face não foi fechada
        return;
    }

    //Copia os meshes de drawScene para scene
    std::list<Mesh*>::iterator iter;
    for(iter = drawScene.meshes.begin(); iter != drawScene.meshes.end(); ) {
        scene.meshes.push_back(*iter);
        scene.numMeshes++;
        iter = drawScene.meshes.erase(iter);
        drawScene.numMeshes--;
    }
    Refresh();
}

void GLCanvas::_addCube(wxCommandEvent& event)
{
    addCube(-2, -2, -2, 4);
    Refresh();
}

void GLCanvas::_addCorner(wxCommandEvent& event)
{
    addCorner(-2, -2, -2,
               1, 4, 2,
               2, 2, 2);
    Refresh();
}

void GLCanvas::_addCylinder(wxCommandEvent& event)
{
     addCylinder(0, 0, 0, 2, 4, 10);
     Refresh();
}

void GLCanvas::_addSphere(wxCommandEvent& event)
{
     addSphere(0, 0, 0, 4, 10);
     Refresh();
}

void GLCanvas::_deleteMesh(wxCommandEvent& event)
{
    std::list<Mesh*>::iterator iter;
    for(iter = meshList.begin(); iter != meshList.end(); iter++) {
        scene.removeSolid((*iter)->id);
    }
    meshList.clear();
}

void GLCanvas::_edit(wxCommandEvent& event)
{
    std::cout << "ASDasdasdas" << std::endl;
    BRepEdit_Dialog edit(this);
    if(edit.ShowModal() == wxID_OK)
    {
        edit.getX();
        edit.getY();
        edit.getZ();
    }
}

void GLCanvas::_neighborhood(wxCommandEvent& event)
{

}

void GLCanvas::_rotate(wxCommandEvent& event)
{
    BRepRotate_Dialog edit(this);
    if(edit.ShowModal() == wxID_OK)
    {
        edit.getX();
        edit.getY();
        edit.getZ();
        edit.getAngle();
    }
}

void GLCanvas::_scale(wxCommandEvent& event)
{
    BRepEdit_Dialog edit(this);
    if(edit.ShowModal() == wxID_OK)
    {
        edit.getX();
        edit.getY();
        edit.getZ();
    }
}

void GLCanvas::_translate(wxCommandEvent& event)
{
    BRepEdit_Dialog edit(this);
    if(edit.ShowModal() == wxID_OK)
    {
        edit.getX();
        edit.getY();
        edit.getZ();
    }
}

void GLCanvas::addCube(float minX, float minY, float minZ, float size)
{
    //v0, f0, s0
    scene.mvfs(minX, minY, minZ);
    //v1
    scene.smev(scene.numMeshes - 1, 0, 0, minX+size, minY, minZ);
    //v2
    scene.smev(scene.numMeshes - 1, 0, 1, minX+size, minY+size, minZ);
    //v3
    scene.smev(scene.numMeshes - 1, 0, 2, minX, minY+size, minZ);
    //f1 -> front
    scene.smef(scene.numMeshes - 1, 0, 3, 0);

    scene.sweep(scene.numMeshes - 1, 0, 0, 0, -size);

    return;
}

void GLCanvas::addCorner(float minX, float minY, float minZ,
                float midX, float midY, float midZ,
                float maxX, float maxY, float maxZ)
{
    //v0
    scene.mvfs(minX, minY, minZ);
    //v1
    scene.smev(scene.numMeshes - 1, 0, 0, maxX, minY, minZ);
    //v2
    scene.smev(scene.numMeshes - 1, 0, 1, maxX, maxY, minZ);
    //v3
    scene.smev(scene.numMeshes - 1, 0, 2, midX, maxY, minZ);
    //v4
    scene.smev(scene.numMeshes - 1, 0, 3, midX, midY, minZ);
    //v5
    scene.smev(scene.numMeshes - 1, 0, 4, minX, midY, minZ);
    //f1 -> front
    scene.smef(scene.numMeshes - 1, 0, 5, 0);
    scene.sweep(scene.numMeshes - 1, 0, 0.0, 0.0, maxZ - minZ);
}

void GLCanvas::addCylinder(float pX, float pY, float pZ, float radius, float height, int disc)
{
    int num = 0;

    scene.mvfs(radius*cos(0) + pX, pY, radius*sin(0) + pZ);

    for(float i = 2.0*M_PI/(float)disc; i <= 2.0*M_PI; i+= 2.0*M_PI/(float)disc)
    {
        if(i+ 2.0*M_PI/(float)disc >= 2.0*M_PI)
        {
            scene.smev(scene.numMeshes - 1, 0, num, radius*cos(i) + pX, pY, radius*sin(i) + pZ);
            num++;
            scene.smef(scene.numMeshes - 1, 0, num, 0);
        }
        else
        {
            scene.smev(scene.numMeshes - 1, 0, num, radius*cos(i) + pX, pY, radius*sin(i) + pZ);
            num++;
        }
    }

    scene.sweep(scene.numMeshes - 1, 0, 0.0, height, 0.0);
}

void GLCanvas::addSphere(float pX, float pY, float pZ, float radius, int disc) {
    float step = M_PI/disc;

    //          cos       sin
    scene.mvfs(-1.0*radius, 0, 0);
    for(int i = 0, idNum = 0; i < disc; i++, idNum++) {
        float angle = M_PI - step*(i+1);
        scene.smev(scene.numMeshes - 1, 0, idNum, radius*cos(angle), radius*sin(angle), 0.0);
    }
    scene.rsweep(scene.numMeshes -1 , 0, 2*disc);
}

void drawBlack(std::list<Mesh*> list)
{
    std::list<Mesh*>::iterator it;
    for (it = list.begin(); it != list.end(); )
    {
            (*it)->r = 0.6;
            (*it)->g = 0.6;
            (*it)->b = 0.6;
            it = list.erase(it);
    }
}

void drawBlack(std::list<Loop*> list)
{
    std::list<Loop*>::iterator it;
    for (it = list.begin(); it != list.end(); )
    {
            (*it)->r = 0.6;
            (*it)->g = 0.6;
            (*it)->b = 0.6;
            it = list.erase(it);
    }
}

void drawBlack(std::list<Edge*> list)
{
    std::list<Edge*>::iterator it;
    for (it = list.begin(); it != list.end(); )
    {
            (*it)->r = 0.0;
            (*it)->g = 0.0;
            (*it)->b = 0.0;
            it = list.erase(it);
    }
}

void drawBlack(std::list<Vertex*> list)
{
    std::list<Vertex*>::iterator it;
    for (it = list.begin(); it != list.end(); )
    {
            (*it)->r = 0.0;
            (*it)->g = 0.0;
            (*it)->b = 0.0;
            it = list.erase(it);
    }
}

void GLCanvas::setMeshSelect()
{
    if(selectFace == true)
        drawBlack(faceList);
    else if(selectEdge == true)
        drawBlack(edgeList);
    else if(selectVertex == true)
        drawBlack(vertexList);

    selectMesh=true;
    selectFace=false;
    selectEdge=false;
    selectVertex=false;

    faceList.clear();
    edgeList.clear();
    vertexList.clear();

    Refresh();
}

void GLCanvas::setFaceSelect()
{
    if(selectMesh == true)
        drawBlack(meshList);
    else if(selectEdge == true)
        drawBlack(edgeList);
    else if(selectVertex == true)
        drawBlack(vertexList);

    selectMesh=false;
    selectFace=true;
    selectEdge=false;
    selectVertex=false;

    meshList.clear();
    edgeList.clear();
    vertexList.clear();

    Refresh();
}

void GLCanvas::setEdgeSelect()
{
    if(selectMesh == true)
        drawBlack(meshList);
    else if(selectFace == true)
        drawBlack(faceList);
    else if(selectVertex == true)
        drawBlack(vertexList);

    selectMesh=false;
    selectFace=false;
    selectEdge=true;
    selectVertex=false;

    meshList.clear();
    faceList.clear();
    vertexList.clear();

    Refresh();
}

void GLCanvas::setVertexSelect()
{
    if(selectMesh == true)
        drawBlack(meshList);
    else if(selectFace == true)
        drawBlack(faceList);
    else if(selectEdge == true)
        drawBlack(edgeList);

    selectMesh=false;
    selectFace=false;
    selectEdge=false;
    selectVertex=true;

    meshList.clear();
    faceList.clear();
    edgeList.clear();

    Refresh();
}

void GLCanvas::menu(wxMouseEvent& event)
{
    if(selectMesh)
        PopupMenu(option_menu_mesh);
    else
        PopupMenu(option_menu);
}

void GLCanvas::init()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glEnable( GL_DEPTH_TEST );
    glClearDepth(1.0f);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glEnable( GL_LINE_SMOOTH );
    glLineWidth( 2.0 );
    glEnable( GL_POLYGON_SMOOTH );
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.0, 1.0);
    glEnable(GL_POINT_SMOOTH);
    glPointSize(5.0);

    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    glFrontFace(GL_CW);
    //glEnable(GL_CULL_FACE);
    //glCullFace(GL_BACK);

    //glPolygonMode(GL_BACK, GL_LINE);          //FILL
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  //WIRE


    //Inicialização da luz
    GLfloat lambient_model[4] = { 0.4, 0.4, 0.4, 1.0};
    GLfloat a[4] = { 0.5, 0.5, 0.5, 1.0};
    GLfloat d[4] = { 0.8, 0.8, 0.8, 0.80};
    GLfloat s[4] = { 1.0, 1.0, 1.0, 1.0};
    GLfloat p[4] = { 100.0, 100.0, 120.0, 0.0};

    glLightfv(GL_LIGHT0, GL_AMBIENT, a);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, d);
    glLightfv(GL_LIGHT0, GL_SPECULAR, s);
    glLightfv(GL_LIGHT0, GL_POSITION, p);
    glEnable(GL_LIGHT0);


    glEnable(GL_LIGHT_MODEL_AMBIENT);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lambient_model);

    //Inicialização do viewport
    int w, h;
    GetClientSize(&w, &h);
    glViewport(0, 0, (GLint) w , (GLint) h );
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();


    if(currentMode == EDIT)
        initEdit();
    else
        initDraw();
 }

 void GLCanvas::initEdit() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    float l = camera.frustum.left;
    float r = camera.frustum.right;
    float b = camera.frustum.bottom;
    float t = camera.frustum.top;
    float n = camera.frustum.near;
    float f = camera.frustum.far;
    glFrustum(camera.frustum.left, camera.frustum.right, camera.frustum.bottom, camera.frustum.top,
              camera.frustum.near, camera.frustum.far);
 }

void GLCanvas::initDraw() {
    int w, h;
    GetClientSize(&w, &h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, w, 0, h, -1, 1);
}

void GLCanvas::render()
{
    SetCurrent(*glContext);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if(currentMode == EDIT) {
        renderBackground();

        float m[16];
        camera.setupViewMatrix(m);
        glTranslatef(-camera.pos.x, -camera.pos.y, -camera.pos.z);
        glMultMatrixf(m);

        if(!scene.isEmpty()) {
            if (selectMesh)
                scene.render(MESHES);
            else
                scene.render(FACES);
            scene.render(POINTS);
            scene.render(LINES);
        }
    } else { //DRAW
        renderEditBackground();
        if(!drawScene.isEmpty()) {
            std::cout << "drawScene != empty" << std::endl;
            if (selectMesh)
                drawScene.render(MESHES);
            else
                drawScene.render(FACES);
            drawScene.render(POINTS);
            drawScene.render(LINES);
        }
        else {
            std::cout << "drawScene == empty" << std::endl;
        }
    }

    //glFlush();
    SwapBuffers();
    //Refresh();
}

void GLCanvas::renderBackground()
{
    // Save color, shading, fill mode, culling, matrixe mode, so it can be restored later
    glPushAttrib( GL_CURRENT_BIT | GL_LIGHTING_BIT | GL_POLYGON_BIT | GL_TRANSFORM_BIT | GL_DEPTH_BUFFER_BIT );

    // To guarantee we can fill a polygon
    glPolygonMode( GL_FRONT, GL_FILL );
    glDisable( GL_LIGHTING );
    glDisable( GL_CULL_FACE );

    // To guarantee we can do gradient color
    glShadeModel( GL_SMOOTH );

    // Make it be rendered always below objects
    glDisable( GL_DEPTH_TEST );

    // Save modelview matrix and reset current to identity
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Save projection matrix and reset current to identity
    glMatrixMode( GL_PROJECTION );
    glPushMatrix();
    glLoadIdentity();

    // Draw the gradient background ( degradê )
    glBegin(GL_QUADS);
        glColor3f(0.6431,0.5843,0.8705);
        glVertex2f(-1.0,-1.0);
        glVertex2f(1.0,-1.0);
        glColor3f(0.0862,0.0,0.4039);
        glVertex2f(1.0,1.0);
        glVertex2f(-1.0,1.0);
    glEnd();

    // Restore projection matrix
    glPopMatrix();

    // Restore modelview matrix
    glMatrixMode( GL_MODELVIEW );
    glPopMatrix();

    // Restore states changed
    glPopAttrib();
}

void GLCanvas::renderEditBackground() {
    // Save color, shading, fill mode, culling, matrixe mode, so it can be restored later
    glPushAttrib( GL_CURRENT_BIT | GL_LIGHTING_BIT | GL_POLYGON_BIT | GL_TRANSFORM_BIT | GL_DEPTH_BUFFER_BIT );

    // To guarantee we can fill a polygon
    glPolygonMode( GL_FRONT, GL_FILL );
    glDisable( GL_LIGHTING );
    glDisable( GL_CULL_FACE );

    // To guarantee we can do gradient color
    glShadeModel( GL_SMOOTH );

    // Make it be rendered always below objects
    glDisable( GL_DEPTH_TEST );

    // Save modelview matrix and reset current to identity
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    // Save projection matrix and reset current to identity
    glMatrixMode( GL_PROJECTION );
    glPushMatrix();
    glLoadIdentity();

    glBegin(GL_QUADS);
    glColor3f(0.7031,0.8143,0.5505);
        glVertex2f(-1.0,-1.0);
        glVertex2f(1.0,-1.0);
    glColor3f(0.3562, 0.55,0.35039);
        glVertex2f(1.0,1.0);
        glVertex2f(-1.0,1.0);
    glEnd();

    int disc = 32;
    float step = 2.0/(float)disc;
    glColor3f(1.0, 1.0, 1.0);
    glLineWidth(0.7);
    for(int i = 0; i < disc; i++){
        for(int j = 0; j < disc; j++) {
            glBegin(GL_LINE_LOOP);
                glVertex2f(-1 + step*i, -1 + step*j);
                glVertex2f(-1 + step*i, -1 + step*(j+1));
                glVertex2f(-1 + step*(i+1), -1 + step*(j+1));
                glVertex2f(-1 +step*(i+1), -1 +step*j);
            glEnd();
        }
    }
    glLineWidth(2.0);

    // Restore projection matrix
    glPopMatrix();

    // Restore modelview matrix
    glMatrixMode( GL_MODELVIEW );
    glPopMatrix();

    // Restore states changed
    glPopAttrib();
}

void GLCanvas::clear() {
    Refresh();
}

void GLCanvas::onMouseLeftUp(wxMouseEvent &event) {
    camera.reset();
    wxPoint mouse;
    event.GetPosition( &mouse.x, &mouse.y );

    wxPoint windowSize;
    GetClientSize( &windowSize.x, &windowSize.y );

    //TODO picking não funciona no modo DRAW
    if(currentMode == EDIT && (selectVertex || selectEdge || selectFace || selectMesh))
    {
        if( event.ShiftDown() )
        selectPicking(mouse.x, windowSize.y - mouse.y);
    }
    //else if (drawing)
    //    draw(mouse.x, windowSize.y - mouse.y);
    Refresh();
}

void GLCanvas::onMouseLeftDown(wxMouseEvent &event) {
    wxPoint windowSize;
    GetClientSize(&windowSize.x, &windowSize.y);
    wxPoint mouse;
    event.GetPosition(&mouse.x, &mouse.y);

    if(currentMode == DRAW) {
        draw(mouse.x, windowSize.y - mouse.y);
    }
    Refresh();
}

void GLCanvas::draw(int x, int y) {
    if(startLineLoop) {
        if(numPts == 0) {
            drawScene.mvfs(x, y, 0.0);
            numPts++;
        } else {
            Vertex *first = drawScene.getVertex(drawScene.numMeshes - 1, 0);
            if(fabs(x - first->x) < 3 && fabs(y - first->y) < 3) {
                drawScene.smef(drawScene.numMeshes -1, 0, numPts - 1, 0);
                std::cout << "smef" << std::endl;
                numPts = 0;
                //Força a criação de um novo mesh.
                drawScene.numMeshes++;
            } else {
                Mesh *m = drawScene.getSolid(drawScene.numMeshes - 1);
                std::list<Vertex*>::iterator vIter = m->vertices.begin();
                vIter++;
                for(; vIter != m->vertices.end(); vIter++)
                    if(fabs(x - (*vIter)->x) < 3 && fabs(y - (*vIter)->y)< 3)
                        return;

                drawScene.smev(drawScene.numMeshes - 1, 0, numPts - 1, x, y, 0.0);
                std::cout << "smev" << std::endl;
                numPts++;
            }
        }
    }
}

void GLCanvas::selectPicking(int x, int y) {
    GLuint buff[512] = {0};
    int hits, view[4];

    glSelectBuffer(512, buff);
    glGetIntegerv(GL_VIEWPORT, view);

    glRenderMode(GL_SELECT);
    glInitNames();

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluPickMatrix(x, y, 5.0, 5.0, view);
    glFrustum(camera.frustum.left, camera.frustum.right, camera.frustum.bottom, camera.frustum.top,
                  camera.frustum.near, camera.frustum.far);
    glMatrixMode(GL_MODELVIEW);
    SwapBuffers();

    if(selectMesh)
        scene.render(MESHES);
    else if(selectFace)
        scene.render(FACES);
    else if(selectEdge)
        scene.render(LINES);
    else if(selectVertex)
        scene.render(POINTS);

    glMatrixMode(GL_PROJECTION);
    glPopMatrix();

    hits = glRenderMode(GL_RENDER);

    std::cout << "hits: "  << hits << std::endl;
    int nearestMesh = buff[3];
    int nearestItem = buff[4];
    int nearestZ = buff[1];
    for (int i = 1; i < hits; i++) {
        if (nearestZ > buff[5*i+1]) {
            nearestZ = buff[5*i+1];
            nearestMesh = buff[5*i + 3];
            nearestItem = buff[5*i + 4];
        }
    }
    for (int i = 0; i < hits; i++) {
        std::cout << "hit: " << i << std::endl;
        std::cout << "\tNumber: " << buff[5*i] << std::endl;
        std::cout << "\tMin z: " << buff[5*i+1] << std::endl;
        std::cout << "\tMax z: " << buff[5*i+2] << std::endl;
        std::cout << "\tName mesh: " << buff[5*i+3] << std::endl;
        std::cout << "\tName item: " << buff[5*i+4] << std::endl;
    }

    std::cout << "mesh escolhido: " << nearestMesh << std::endl;
    std::cout << "face escolhido: " << nearestItem << std::endl;

    glMatrixMode(GL_MODELVIEW);
    if (hits == 0)
        return;
    Mesh *m = scene.getSolid(nearestMesh);

    if(selectMesh) {
        std::list<Mesh*>::iterator it;
        for (it = meshList.begin(); it != meshList.end(); ++it)
        {
            if((*it)->id == m->id)
            {
                m->r = 0.6;
                m->g = 0.6;
                m->b = 0.6;
                meshList.erase(it);
                Refresh();
                return;
            }
        }
        m->r = 1.0;
        m->g = 0.0;
        m->b = 0.0;
        meshList.push_back(m);
    } else if(selectFace) {
         Loop *l = scene.getLoop(m->id, nearestItem);

        std::list<Loop*>::iterator it;
        for (it = faceList.begin(); it != faceList.end(); ++it)
        {
            if((*it)->id == l->id)
            {
                l->r = 0.6;
                l->g = 0.6;
                l->b = 0.6;
                faceList.erase(it);
                Refresh();
                return;
            }
        }
         l->r = 1.0;
         l->g = 0.0;
         l->b = 0.0;
        faceList.push_back(l);
    } else if(selectEdge) {
        Edge *e = scene.getEdge(m->id, nearestItem);

        std::list<Edge*>::iterator it;
        for (it = edgeList.begin(); it != edgeList.end(); ++it)
        {
            if((*it)->id == e->id)
            {
                e->r = 0.0;
                e->g = 0.0;
                e->b = 0.0;
                edgeList.erase(it);
                Refresh();
                return;
            }
        }
        e->r = 1.0;
        e->g = 0.0;
        e->b = 0.0;
        edgeList.push_back(e);
    } else if(selectVertex) {
        Vertex *v = scene.getVertex(m->id, nearestItem);

        std::list<Vertex*>::iterator it;
        for (it = vertexList.begin(); it != vertexList.end(); ++it)
        {
            if((*it)->id == v->id)
            {
                v->r = 0.0;
                v->g = 0.0;
                v->b = 0.0;
                vertexList.erase(it);
                Refresh();
                return;
            }
        }
        v->r = 1.0;
        v->g = 0.0;
        v->b = 0.0;
        vertexList.push_back(v);
    }
    Refresh();
}

void GLCanvas::onMouseMove(wxMouseEvent &event)
{
    wxPoint mouse;
    event.GetPosition( &mouse.x, &mouse.y );

    wxPoint windowSize;
    GetClientSize( &windowSize.x, &windowSize.y );
    static wxString msg;

    if ( event.LeftIsDown() )
    {
        camera.updateRotation(mouse.x,mouse.y, windowSize.x, windowSize.y);
        Refresh();
        event.Skip();
        //static std::string temp;
        //temp = manipulator->update( mouse.x, mouse.y, windowSize.x, windowSize.y );
        //msg = wxString::FromAscii( temp.c_str() );
        //parent->SetStatusText( msg, 1 );
    }
    //else
    //{
    //    msg.Printf(_("Mouse Position: ( %d, %d )"), mouse.x, mouse.y );
        //parent->SetStatusText( msg, 1 );
    //}
}

void GLCanvas::onMouseWheel(wxMouseEvent& event)
{
    if ( event.ControlDown() )
    {
        int wheel = event.GetWheelRotation();
        if ( wheel > 0 )
            camera.zoom( 0.8 );
        else
            camera.zoom( 1.2 );
        //Atualiza o frustum
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glFrustum(camera.frustum.left, camera.frustum.right, camera.frustum.bottom, camera.frustum.top,
                  camera.frustum.near, camera.frustum.far);
        Refresh();
    }
}

void GLCanvas::onEnterWindow(wxMouseEvent& event)
{
    SetFocus();
}

void GLCanvas::onPaint( wxPaintEvent& event )
{
    wxPaintDC dc(this);
    render();
}

void GLCanvas::onSize(wxSizeEvent& event)
{
    wxGLCanvas::OnSize(event);

    int w, h;
    GetClientSize(&w, &h);

    glViewport(0, 0, (GLint) w , (GLint) h );

    //if (drawing) {
    //    glMatrixMode(GL_PROJECTION);
    //    glOrtho(0, w, 0, h, -1, 1);
    //}
    Refresh();
}

void GLCanvas::onEraseBackground(wxEraseEvent& event)
{
  //só para se livrar do flicker
}

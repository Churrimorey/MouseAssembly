#define _CRT_SECURE_NO_WARNINGS
#include "GUI.h"
double fps = 0;
int frameCount = 0;
double lastTime = 0;
void getFPS() {
    frameCount++;
    double currentTime = glutGet(GLUT_ELAPSED_TIME) * 0.001; // ???????????

    if (currentTime - lastTime > 1.0) { // ?????????
        fps = frameCount / (currentTime - lastTime);
        lastTime = currentTime;
        frameCount = 0;
        //fpsText = "FPS: " + std::to_string(fps);
    }
}

void renderCenterPoint(int Width, int Height, bool ifCenterPoint) {
    if (!ifCenterPoint) return;
    glPushMatrix();
    glViewport(0, 0, Width, Height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, Width, 0, Height);
    glMatrixMode(GL_MODELVIEW);
    Material::SetColor(1, 0, 0);
    glPointSize(5.0f);
    glBegin(GL_POINTS);
    glVertex3f((Width - 80) / 2 + 80, Height / 2, 0.0f);
    glEnd();
    glPopMatrix();
}

void renderAxis(int Width, int Height, float pitch, float yaw, int ifAxis) {
    if (!ifAxis) return;
    glPushMatrix();
    glViewport(0, 0, Width, Height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, Width, 0, Height, -200, 200);
    glMatrixMode(GL_MODELVIEW);

    /*glTranslatef(-30,0,-30);*/
    glTranslatef(Width / 4, Height / 4, 0);
    glTranslatef((Width - 80) / 2 + 80, Height / 2, 1.0f);    // ???????????
    glRotatef(yaw + 90, 0, 1, 0);
    glRotatef(-pitch, 1, 0, 0);
    // ??????
    float axis_length = 100;
    glLineWidth(5);
    glBegin(GL_LINE_STRIP);
    Material::SetColor(1, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(axis_length, 0, 0);
    glEnd();

    glPushMatrix();
    glColor3f(1.0, 0.0, 0.0); // ???????????
    glTranslatef(axis_length, 0, 0);
    glRotatef(90, 0, 1, 0);
    glutSolidCone(5.0, 10.0, 50, 50);
    glPopMatrix();

    glBegin(GL_LINE_STRIP);
    Material::SetColor(0, 1, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, axis_length, 0);
    glEnd();
    glPushMatrix();
    glTranslatef(0, axis_length, 0);
    glRotatef(-90, 1, 0, 0);
    glutSolidCone(5.0, 10.0, 50, 50); // ??????
    glPopMatrix();

    glBegin(GL_LINE_STRIP);
    Material::SetColor(0, 0, 1);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, axis_length);
    glEnd();
    glPushMatrix();
    glTranslatef(0, 0, axis_length);
    glRotatef(90, 0, 0, 0);
    glutSolidCone(5.0, 10.0, 50, 50); // ??????
    glPopMatrix();

    glPushMatrix();
    Material::SetColor(1, 1, 1);
    glutSolidSphere(5.0f, 100, 100);
    glPopMatrix();
    glPopMatrix();
}

/***************************************** myGlutDisplay() *****************/

extern int main_window;
GLUI_StaticText* text;
GLUI_List* hah;
GLUI_TextBox* moo;
GLUI* edit;
GLUI_Panel* objp;
GLUI_Listbox* objectList;
GLUI_Listbox* TexList;
float counter = 0;
const char camera[] =
"Camera Control\n"
"\n"
"Supporting up, down, left, right, back and forth movement, zoom in&out and return to normal size\n"
"\n"
"  * Press 'w' key: camera move forth\n"
"  * Press 's' key: camera move back\n"
"  * Press 'a' key: camera move left\n"
"  * Press 'd' key: camera move right\n"
"  * Press 'q' key: camera move up\n"
"  * Press 'e' key: camera move down\n"
"  * Press 'z' key: camera zoom in\n"
"  * Press 'e' key: camera zoom out\n"
"  * Press 'b' key: camera return to normal size\n"
"\n"
"Supporting perspective rotation\n\n"
"  * Move mouse: rotate perspective\n"
;

const char screenshot[] =
"Screenshot Control\n"
"\n"
"Supporting screenshots\n"
"\n"
"  * Press 'f' key: save photo\n";

const char run[] =
"Run Animation\n"
"\n"
"The whole process\n"
"\n"
"  *Left click every object\n";
extern int RenderObj[64]; //表示物体是否会被渲染
extern int choose; //选中哪个物体
extern std::vector<glm::vec3> ObjPos;
extern std::vector<glm::vec3> ObjFront;
extern std::vector<glm::vec3> ObjUp;
extern std::vector<float> ObjSize;
extern std::vector<float> ObjPitch;
extern std::vector<float> ObjYaw;

extern int ifShow;
extern glm::vec3 ObjPos_tmp;
extern float ObjSize_tmp;
extern float ObjPitch_tmp;
extern float ObjYaw_tmp;

extern int ObjTex_tmp;
extern int objTex[64];

int objitem=0;
extern std::string SavePath;
char* strPtr = NULL;
extern int ifAxis;
int axis = 0;
void control_cb(int control) {
    int item;

    GLUI_String text;
    item = hah->get_current_item();
    if (control == 1) {
        if (item == 0) {
            moo->set_text(camera);
        }
        if (item == 1) {
            moo->set_text(screenshot);
        }
    }
    if (control == 2) {
        if (axis == 0) {
            ifAxis = 0;
        }
        else {
            ifAxis = 1;
        }
    }
    if (control == 3) {
        std::string temp(strPtr);
        SavePath = temp;
    }
    if (control == 4) {

        choose = objitem;
        ObjPos_tmp.x = ObjPos[choose].x;
        ObjPos_tmp.y = ObjPos[choose].y;
        ObjPos_tmp.z = ObjPos[choose].z;
        ObjSize_tmp = ObjSize[choose];
        ObjPitch_tmp = ObjPitch[choose];
        ObjYaw_tmp = ObjYaw[choose];
        ifShow = 1 - RenderObj[choose];
        printf_s("render %d = %d\n", choose, RenderObj[choose]);
    }
    if (control == 5) {
        if (ObjTex_tmp == 9) { //wood
            printf_s("wood texture\n");
            objTex[choose] = 9;
        }
        else if (ObjTex_tmp == 10) { //brick
            printf_s("brick texture\n");
            objTex[choose] = 10;
        }
        else if (ObjTex_tmp == 11) { //metal
            printf_s("Metal texture\n");
            objTex[choose] = 11;
        }
    }
    glutPostRedisplay();
}
void textbox_cb(GLUI_Control* control) {
    printf("Got textbox callback\n");
}

void buttonCallback(int id) {
    if (id == 0) {
        ScreenCapture::captureScreenshot(SavePath);   //可换成希望保存到的路径和文件名
        std::cout << "Screenshot saved to " << SavePath << std::endl;
    }
}

void textCallback(int id) {
    // 在此回调函数中获取文本框的值并将其赋值给变量
    //strcpy(textInput, glui_text->get_text());
}
void myGlutIdle(void)
{
    /* According to the GLUT specification, the current window is
       undefined during an idle callback.  So we need to explicitly change
       it if necessary */
    if (glutGetWindow() != main_window)
        glutSetWindow(main_window);
    getFPS();
    counter = fps;
    edit->sync_live();
    glutPostRedisplay();
}
extern float camera_speed;
extern float sensitivity;
extern float zoom_speed;
GLUI_Checkbox* checkbox;
GLUI_Spinner* move_spinner;
GLUI_Spinner* zoom_spinner;
GLUI_Spinner* mouse_spinner;
//物体编辑
GLUI_Spinner* Posx_spinner;
GLUI_Spinner* Posy_spinner;
GLUI_Spinner* Posz_spinner;
GLUI_Spinner* Yaw_spinner;
GLUI_Spinner* Pitch_spinner;
GLUI_Spinner* Size_spinner;
void GUIinit() {
    edit = GLUI_Master.create_glui("Simple GUI", 0);
    //main_window = edit->get_glut_window_id();
    GLUI_Panel* ep = new GLUI_Panel(edit, "Instructions", true);
    new GLUI_StaticText(ep, "Readme");
    hah = new GLUI_List(ep, true, 1, control_cb);
    hah->add_item(0, "Camera Control");
    hah->add_item(1, "Screenshots Control");
    //new GLUI_StaticText(ep, "List obj files:");
    //fb = new GLUI_FileBrowser(ep, "", false, 7, control_cb);
    //fb->set_h(180);
    hah->set_h(80);
    new GLUI_Column(ep, false);
    moo = new GLUI_TextBox(ep, true, 1, textbox_cb);
    moo->set_text(camera);
    moo->set_h(300);
    moo->set_w(410);
    moo->disable();

    //物体属性设置
    objp = new GLUI_Panel(edit, "Edit", true);
    objectList = new GLUI_Listbox(objp, "Object",&objitem,4,control_cb);
    //objectList->set_w(410);
    objectList->add_item(0, "Table");
    objectList->add_item(1, "Chair");
    objectList->add_item(2, "Cup");
    GLUI_Panel * obj_panel = new GLUI_Rollout(objp, "Properties", false);
    new GLUI_Checkbox(obj_panel, "Hide", &ifShow);
    GLUI_Panel* edit_pos = new GLUI_Rollout(obj_panel, "Modify Position", false);
    Posx_spinner = new GLUI_Spinner(edit_pos, "Position x:", &ObjPos_tmp.x);
    Posx_spinner->set_speed(0.1);
    Posx_spinner->set_float_limits(-1000.0f, 1000.0f);
    Posy_spinner = new GLUI_Spinner(edit_pos, "Position y:", &ObjPos_tmp.y);
    Posy_spinner->set_speed(0.1);
    Posy_spinner->set_float_limits(-1000.0f, 1000.0f);
    Posz_spinner = new GLUI_Spinner(edit_pos, "Position z:", &ObjPos_tmp.z);
    Posz_spinner->set_speed(0.1);
    Posz_spinner->set_float_limits(-1000.0f, 1000.0f);

    GLUI_Panel* edit_rot = new GLUI_Rollout(obj_panel, "Modify Rotation", false);
    Yaw_spinner = new GLUI_Spinner(edit_rot, "Yaw Angle:", &ObjYaw_tmp);
    Yaw_spinner->set_float_limits(-180.0f, 180.0f);
    Pitch_spinner = new GLUI_Spinner(edit_rot, "Pitch Angle:", &ObjPitch_tmp);
    Pitch_spinner->set_float_limits(-90.0f, 90.0f);

    GLUI_Panel* edit_size = new GLUI_Rollout(obj_panel, "Modify Size", false);
    Size_spinner = new GLUI_Spinner(edit_size, "Size:", &ObjSize_tmp);
    Size_spinner->set_speed(0.1);
    Size_spinner->set_float_limits(0.01f,100.0f);

    GLUI_Panel* edit_texture = new GLUI_Rollout(obj_panel, "Change Texture", false);
    TexList = new GLUI_Listbox(edit_texture, "Texture", &ObjTex_tmp, 5, control_cb);
    //objectList->set_w(410);
    TexList->add_item(9, "Wood");
    TexList->add_item(10, "Brick");
    TexList->add_item(11, "Metal");

    //相机设置
    GLUI_Panel* sp = new GLUI_Panel(edit, "Camera", true);
    checkbox = new GLUI_Checkbox(sp, "Display Axis", &axis, 2, control_cb);
    move_spinner = new GLUI_Spinner(sp, "CameraSpeed:", &camera_speed);
    move_spinner->set_float_limits(2.5f, 30.0);
    zoom_spinner = new GLUI_Spinner(sp, "ZoomSpeed:", &zoom_speed);
    zoom_spinner->set_float_limits(0.01f, 2.0);
    mouse_spinner = new GLUI_Spinner(sp, "MouseSpeed:", &sensitivity);
    mouse_spinner->set_float_limits(0.01f, 0.5f);
    

    GLUI_Panel* framep = new GLUI_Panel(edit, "Frames Per Seconds", true);
    GLUI_EditText* counter_edittext =
        new GLUI_EditText(framep, "FPS:", &counter);
    counter_edittext->disable();

    GLUI_Panel* storep = new GLUI_Panel(edit, "Screenshots", true);
    const char* cstr = SavePath.c_str();
    strPtr = const_cast<char*>(cstr);
    GLUI_EditText* editText = new GLUI_EditText(storep, "Save Path:", strPtr,3,control_cb);
    editText->set_w(300);
    edit->add_button_to_panel(storep, "Screenshots", 0, buttonCallback);
    edit->set_main_gfx_window(main_window);
    GLUI_Master.set_glutIdleFunc(myGlutIdle);

}
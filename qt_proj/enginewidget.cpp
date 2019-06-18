#include "enginewidget.h"
#include "ui_enginewidget.h"
#include "../engine/std/texture.h"
#include "../engine/std/asset.h"
#include "../engine/gui/ttfont.h"

engineWidget::engineWidget(QWidget *parent) :
    QOpenGLWidget(parent),
    ui(new Ui::engineWidget)
{
    ui->setupUi(this);
    m_timerId = startTimer(30);
}

engineWidget::~engineWidget()
{
    SAFE_DELETE(ui);
    SAFE_DELETE(camera);
    SAFE_DELETE(light);
    SAFE_DELETE(shader);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1,&vbo);
}

void engineWidget::HandleClick(int evtid)
{
    SAFE_DELETE(shader);
    SAFE_DELETE(light);
    if(evtid == 0)
        light = new DirectLight(vec3(1), vec3(-1,0,-2));
    if(evtid == 1)
        light = new PointLight(vec3(1), vec3(0,0,-1), vec3(0,0,2), vec3(0.1,0.2,0.01));
    if(evtid == 2)
        light = new SpotLight(vec3(1), vec3(0,0,-1), vec3(0,0,2), vec3(1,0.1,0), 6, 8);
    InitShader();
    update();
}

void engineWidget::initializeGL()
{
    initializeOpenGLFunctions();
    camera = new Camera(glm::vec3(0.0f,0.0f,3.0f));
    light = new DirectLight(vec3(1.0f), vec3(-1,0,-2));
    skybox = new Skybox(camera, "mp_5dim");
    cube = (MeshData*)InitCube(vao, vbo);
    InitShader();
    Texture("textures/container", JPG, &texture1);
    Texture("textures/awesomeface",PNG, &texture2);
    TTFont::getInstance()->initial();
    label = new UILabel(vec2(30,560), vec3(1), 1);
}

void engineWidget::resizeGL(int w, int h)
{
    glViewport(0,0,w,h);
    update();
}

void engineWidget::paintGL()
{
    glDisable(GL_BLEND);
    glDisable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2f,0.2f,0.2f,1.0f);
    vec3 cubePositions[] = { glm::vec3( 0.0f,  0.0f,  -2.0f), glm::vec3(2.0f,  1.0f, -4.0f) };
    shader->use();
    shader->setFloat("scale", 1);
    light->Apply(shader);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glBindVertexArray(vao);
    for (unsigned int i = 0; i < 2; i++)
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = translate(model, cubePositions[i]);
        float angle = 64 * i * GetRuntime();
        model = glm::rotate(model, glm::radians(angle), vec3(1.0f, 0.3f, 0.5f));
        model = glm::scale(model, vec3(0.5f));
        shader->setMat4("model", model);
        glDrawArrays(DRAW_MODE, 0, 36);
    }
    glBindVertexArray(0);
    if(skybox) skybox->Draw();

    label->drawText("Hello, OpenGL");
}


void engineWidget::timerEvent(QTimerEvent *)
{
    update();
}

void engineWidget::InitShader()
{
    shader = new LightShader("light.vs","light.fs");
    shader->attach(light->getMacro().c_str());
    cube->Bind(shader);
    shader->use();
    shader->setInt("texture1", 0);
    shader->setInt("texture2", 1);
    camera->Attach(shader);
}


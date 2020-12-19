#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/filesystem.h>
#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
unsigned int loadTexture(const char *path);

unsigned int loadCubemap(vector<string> vector);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 10.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

//for turning reflectors on/off
int turnOn1 = 1;
int turnOn2 = 1;

glm::vec3 lightPos(0.0f, 3.0f, -1.0f);

int main()
{
    // glfw: initialize and configure
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    glEnable(GL_DEPTH_TEST);


    /*//providnost
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);*/


    // ALWAYS CHECK THIS WHEN YOUR TEXTURES LOOK STRANGE!
     //stbi_set_flip_vertically_on_load(true);

    Shader stageShader("resources/shaders/stage.vs", "resources/shaders/stage.fs");

    Shader centralLightShader("resources/shaders/centralLight.vs", "resources/shaders/centralLight.fs");

    Shader windowShader("resources/shaders/window.vs","resources/shaders/window.fs");
    //Shader windowShader2("resources/shaders/window.vs","resources/shaders/window2.fs");


    Shader ourShader("resources/shaders/1.model_loading.vs", "resources/shaders/1.model_loading.fs");

    Shader skyShader("resources/shaders/sky.vs","resources/shaders/sky.fs");

    Shader giftShader("resources/shaders/gift.vs", "resources/shaders/gift.fs");

    // models

    //Model ourModel(FileSystem::getPath("resources/objects/backpack/backpack.obj"));
    Model swing(FileSystem::getPath("resources/objects/swing/10549_ChildrenSwingSet_v1-LoD2.obj"));
    Model carousel(FileSystem::getPath("resources/objects/carousel/model.obj"));
    Model seesaw(FileSystem::getPath("resources/objects/seesaw/10547_Childrens_Seesaw_v2-L3.obj"));
    Model reflector("resources/objects/stage_down_light_texture_4/scene.gltf");
    Model gifts("resources/objects/stack_of_christmas_gifts/scene.gltf");

    float vertices[] = {
            // positions          // normals           // texture coords
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
            0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

            //front
           /* -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
            0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,*/

            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
            -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
            -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
            -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
            0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
            0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
            0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
            0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
            0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
            0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };

    float transparentVertices[] = {
            // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
            0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
            0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
            1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

            0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
            1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
            1.0f,  0.5f,  0.0f,  1.0f,  0.0f
    };

    float skyVertices[] = {
            // positions
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f,  1.0f
    };

    glm::vec3 spotLightPositions[] = {
            glm::vec3( -3.8f,-1.2f,0.0f),
            glm::vec3( 3.8f,-1.2f,0.0f)
    };

    unsigned int VBO, stageVAO;
    glGenVertexArrays(1, &stageVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(stageVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);


    unsigned int centralLightVAO;
    glGenVertexArrays(1, &centralLightVAO);
    glBindVertexArray(centralLightVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    unsigned int transparentVAO, transparentVBO;
    glGenVertexArrays(1, &transparentVAO);
    glGenBuffers(1, &transparentVBO);
    glBindVertexArray(transparentVAO);
    glBindBuffer(GL_ARRAY_BUFFER, transparentVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(transparentVertices), transparentVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glBindVertexArray(0);

    unsigned int skyVAO, skyVBO;
    glGenVertexArrays(1, &skyVAO);
    glGenBuffers(1, &skyVBO);
    glBindVertexArray(skyVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyVertices), &skyVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // load textures
    unsigned int diffuseMap = loadTexture(FileSystem::getPath("resources/textures/tepih.jpg").c_str());
    unsigned int specularMap = loadTexture(FileSystem::getPath("resources/textures/tepih2.jpg").c_str());

    unsigned int transparentTexture = loadTexture(FileSystem::getPath("resources/textures/window2.jpg").c_str());
    unsigned int transparentTexture2 = loadTexture(FileSystem::getPath("resources/textures/window3.jpg").c_str());

    giftShader.use();
    giftShader.setInt("material.diffuse", 0);
    giftShader.setInt("material.specular", 1);

    vector<std::string> faces
    {
        FileSystem::getPath("resources/textures/sky/right.jpg"),
        FileSystem::getPath("resources/textures/sky/left.jpg"),
        FileSystem::getPath("resources/textures/sky/top.jpg"),
        FileSystem::getPath("resources/textures/sky/bottom.jpg"),
        FileSystem::getPath("resources/textures/sky/front.jpg"),
        FileSystem::getPath("resources/textures/sky/back.jpg")
    };

    unsigned int cubemapTexture =  loadCubemap(faces);

    stageShader.use();
    stageShader.setInt("material.diffuse", 0);
    stageShader.setInt("material.specular", 1);
    skyShader.use();
    skyShader.setInt("skybox", 2);


    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        processInput(window);

        // render
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        giftShader.use();
        giftShader.setVec3("spotLights[0].position", spotLightPositions[0]);
        giftShader.setVec3("spotLights[0].direction", glm::vec3(3.8f, -1.85f, -1.0f));
        giftShader.setVec3("spotLights[0].ambient", 0.0f, 0.0f, 0.0f);
        giftShader.setVec3("spotLights[0].diffuse", 1.0f, 1.0f, 1.0f);
        giftShader.setVec3("spotLights[0].specular", 1.0f, 1.0f, 1.0f);
        giftShader.setFloat("spotLights[0].constant", 1.0f);
        giftShader.setFloat("spotLights[0].linear", 0.09);
        giftShader.setFloat("spotLights[0].quadratic", 0.032);
        if(turnOn1 == 1){
            giftShader.setFloat("spotLights[0].cutOff", glm::cos(glm::radians(10.0f)));
            giftShader.setFloat("spotLights[0].outerCutOff", glm::cos(glm::radians(12.0f)));
        }
        else{
            giftShader.setFloat("spotLights[0].cutOff", glm::cos(glm::radians(0.0f)));
            giftShader.setFloat("spotLights[0].outerCutOff", glm::cos(glm::radians(0.0f)));
        }
        giftShader.setVec3("viewPos", camera.Position);

        giftShader.setVec3("spotLights[1].position", spotLightPositions[1]);
        giftShader.setVec3("spotLights[1].direction", glm::vec3(-3.8f, -1.85f, -1.0f));
        giftShader.setVec3("spotLights[1].ambient", 0.0f, 0.0f, 0.0f);
        giftShader.setVec3("spotLights[1].diffuse", 1.0f, 1.0f, 1.0f);
        giftShader.setVec3("spotLights[1].specular", 1.0f, 1.0f, 1.0f);
        giftShader.setFloat("spotLights[1].constant", 1.0f);
        giftShader.setFloat("spotLights[1].linear", 0.09);
        giftShader.setFloat("spotLights[1].quadratic", 0.032);
        if(turnOn2 == 1){
            giftShader.setFloat("spotLights[1].cutOff", glm::cos(glm::radians(10.0f)));
            giftShader.setFloat("spotLights[1].outerCutOff", glm::cos(glm::radians(12.0f)));
        }
        else{
            giftShader.setFloat("spotLights[1].cutOff", glm::cos(glm::radians(0.0f)));
            giftShader.setFloat("spotLights[1].outerCutOff", glm::cos(glm::radians(0.0f)));
        }
   //     giftShader.setVec3("viewPos", camera.Position);


        // material properties
        giftShader.setFloat("material.shininess", 32.0f);


        stageShader.use();
        stageShader.setVec3("light.position", lightPos);
        stageShader.setVec3("viewPos", camera.Position);

        // light properties
        stageShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
        stageShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
        stageShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);


        stageShader.setFloat("material.shininess", 64.0f);

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        stageShader.setMat4("projection", projection);
        stageShader.setMat4("view", view);

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        model=translate(model, glm::vec3(0.0f, -2.0f, -1.0f));
        model=scale(model,glm::vec3(8.0f,2.0f,6.0f));

        stageShader.setMat4("model", model);

        // bind diffuse map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        // bind specular map
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);


        // render the stage
        glBindVertexArray(stageVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);



        centralLightShader.use();
        centralLightShader.setMat4("projection", projection);
        centralLightShader.setMat4("view", view);

        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
        centralLightShader.setMat4("model", model);

        glBindVertexArray(centralLightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        //first window

        glBindVertexArray(transparentVAO);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, transparentTexture);
        windowShader.use();
        windowShader.setInt("texture1", 3);
        windowShader.setMat4("projection", projection);
        windowShader.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model,glm::vec3(-4.0f,-2.0f,2.0f));
        model = scale(model,glm::vec3(4.0f,2.0f,6.0f));
        windowShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        model = glm::mat4(1.0f);
        model = glm::translate(model,glm::vec3(0.0f,-2.0f,2.0f));
        model = scale(model,glm::vec3(4.0f,2.0f,6.0f));
        windowShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        //second window
        /*glBindVertexArray(transparentVAO);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, transparentTexture2);
        windowShader2.use();
        windowShader2.setInt("texture1", 4);
        windowShader2.setMat4("projection", projection);
        windowShader2.setMat4("view", view);
        model = glm::mat4(1.0f);
        model = glm::translate(model,glm::vec3(0.0f,-2.0f,2.0f));
        model = scale(model,glm::vec3(4.0f,2.0f,6.0f));
        windowShader2.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 6);*/


        ourShader.use();

        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        //BACKPACK
        /*model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
        ourShader.setMat4("model", model);
        ourModel.Draw(ourShader);*/

        //SWING
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-2.5f, -1.0f, -1.5f)); // translate it down so it's at the center of the scene
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3( 0.006f));	// it's a bit too big for our scene, so scale it down
        ourShader.setMat4("model", model);
        swing.Draw(ourShader);

        //CAROUSEL
        float time=glfwGetTime();
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(1.5f,0.25f,-1.7f)); // translate it down so it's at the center of the scene
        model = glm::rotate(model,time,glm::vec3(0.0f,-1.0f,0.0f));
        model = glm::rotate(model, glm::radians(60.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(-4.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3( 0.2f));	// it's a bit too big for our scene, so scale it down
        ourShader.setMat4("model", model);
        carousel.Draw(ourShader);

        //SEESAW
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f,-1.0f,1.0f)); // translate it down so it's at the center of the scene
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3( 0.01f));	// it's a bit too big for our scene, so scale it down
        ourShader.setMat4("model", model);
        seesaw.Draw(ourShader);

        //REFLECTORS
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-3.8f,-1.2f,0.0f)); // translate it down so it's at the center of the scene
//        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
 //       model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::rotate(model, glm::radians(5.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3( 0.2f));	// it's a bit too big for our scene, so scale it down
        ourShader.setMat4("model", model);
        reflector.Draw(ourShader);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(3.8f,-1.2f,0.0f)); // translate it down so it's at the center of the scene
  //      model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
   //     model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::rotate(model, glm::radians(175.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3( 0.2f));	// it's a bit too big for our scene, so scale it down
        ourShader.setMat4("model", model);
        reflector.Draw(ourShader);

        giftShader.use();

        giftShader.setMat4("projection", projection);
        giftShader.setMat4("view", view);

        //GIFTS
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f,-3.0f,-1.0f));
        model = glm::scale(model, glm::vec3( 0.07f));
        giftShader.setMat4("model", model);
        gifts.Draw(giftShader);

        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        skyShader.use();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
        skyShader.setMat4("view", view);
        skyShader.setMat4("projection", projection);
        // skybox cube
        glBindVertexArray(skyVAO);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional
    glDeleteVertexArrays(1, &stageVAO);
    glDeleteVertexArrays(1, &centralLightVAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources
    glfwTerminate();
    return 0;
}

unsigned int loadCubemap(vector<string> faces) {

    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


    return textureID;
}


void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if(glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS){
        turnOn1 = -turnOn1;
    }
    if(glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        turnOn2 = -turnOn2;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions;
    glViewport(0, 0, width, height);
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // koordinate se krecu odozdo prema gore

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}


unsigned int loadTexture(char const * path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        /*glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT); // for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);*/
        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

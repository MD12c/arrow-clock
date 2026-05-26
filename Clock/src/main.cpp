#include "main.h"
int width = 400;
int height = 400;

int main()
{
	//std::cout << "Hello CMake." << std::endl;
	// Name of the window, width & height of the window, background color RGB
	Window VIEWPORT("Clocki", width, height, 0.0f, 0.0f, 0.0f);
	VIEWPORT.glfwSetup();

	Shader defShader("Assets/shaders/default.vert", "Assets/shaders/default.frag");
	defShader.Activate();

	GLint modelLoc = glGetUniformLocation(defShader.ID, "translated");
	GLint projLoc = glGetUniformLocation(defShader.ID, "projection");
	GLint colorLoc = glGetUniformLocation(defShader.ID, "Color");
	GLint useTextureLoc = glGetUniformLocation(defShader.ID, "useTexture");

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	glm::mat4 proj = glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

	struct ResizeData
	{
		GLint projLoc;
		glm::mat4 proj;
		GLuint shaderID;
		int &width;
		int &height;
	};
	ResizeData resizeData{projLoc, proj, defShader.ID, width, height};
	glfwSetWindowUserPointer(VIEWPORT.getWindow(), &resizeData);

	glfwSetFramebufferSizeCallback(VIEWPORT.getWindow(), [](GLFWwindow *win, int w, int h)
								   {
		glViewport(0, 0, w, h);
		glLineWidth(static_cast<GLfloat>(w)/70);
		auto* data = (ResizeData*)glfwGetWindowUserPointer(win);
		float aspect = (float)w / (float)h;
		data->width = w;
    	data->height = h;
		data->proj = glm::ortho(-aspect, aspect, -1.0f, 1.0f, -1.0f, 1.0f);
		glUseProgram(data->shaderID);
		glUniformMatrix4fv(data->projLoc, 1, GL_FALSE, glm::value_ptr(data->proj)); });

// Imgui imgui(VIEWPORT.getWindow());
// imgui.CreateContext();

// Clock
#pragma region
	GLfloat Clocki[20] = {
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom-left
		-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,	// top-left
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f,	// bottom-right
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f	// top-right
	};

	GLuint indices[6] = {
		0, 1, 2,
		1, 2, 3};

	VAO ClockiVAO;
	ClockiVAO.Bind();
	VBO ClockiVBO(Clocki, sizeof(Clocki));
	ClockiVBO.Bind();
	ClockiVAO.LinkAttrib(ClockiVBO, 0, 3, GL_FLOAT, 5 * sizeof(float), (void *)0);
	ClockiVAO.LinkAttrib(ClockiVBO, 1, 2, GL_FLOAT, 5 * sizeof(float), (void *)(sizeof(GLfloat) * 3));
	EBO ClockiEBO(indices, sizeof(indices));

	Texture ClockiTexture("Assets/Textures/clock2.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
	ClockiTexture.Bind();
	ClockiTexture.texUnit(defShader, "tex0", 0);

	auto bindClocki = [&]()
	{
		ClockiVAO.Bind();
		ClockiVBO.Bind();
		ClockiEBO.Bind();
		ClockiTexture.Bind();
	};
	auto unbindClocki = [&]()
	{
		ClockiTexture.Unbind();
		ClockiEBO.Unbind();
		ClockiVBO.Unbind();
		ClockiVAO.Unbind();
	};
#pragma endregion

// Minutes
#pragma region
	GLfloat MinuteHand[3] = {0.0f, 0.7f, 0.0f};
	glm::mat4 MinuteHandMat4 = model;

	VAO MinutesVAO;
	MinutesVAO.Bind();
	VBO MinutesVBO(MinuteHand, sizeof(MinuteHand));
	MinutesVBO.Bind();
	MinutesVAO.LinkAttrib(MinutesVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void *)0);
	glLineWidth(3.5f);
	glEnable(GL_LINE_SMOOTH);

	auto bindMinutes = [&]()
	{
		MinutesVAO.Bind();
		MinutesVBO.Bind();
	};
	auto unbindMinutes = [&]()
	{
		MinutesVBO.Unbind();
		MinutesVAO.Unbind();
	};
#pragma endregion

// Hours
#pragma region
	GLfloat HourHand[3] = {0.0f, 0.4f, 0.0f};
	glm::mat4 HourHandMat4 = model;

	VAO HoursVAO;
	HoursVAO.Bind();
	VBO HoursVBO(HourHand, sizeof(HourHand));
	HoursVBO.Bind();
	HoursVAO.LinkAttrib(HoursVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), (void *)0);
	glLineWidth(3.5f);
	glEnable(GL_LINE_SMOOTH);

	auto bindHours = [&]()
	{
		HoursVAO.Bind();
		HoursVBO.Bind();
	};
	auto unbindHours = [&]()
	{
		HoursVBO.Unbind();
		HoursVAO.Unbind();
	};
#pragma endregion

	while (!glfwWindowShouldClose(VIEWPORT.getWindow()))
	{
		VIEWPORT.glClearCurrentColor();
		// imgui.ShowDockSpace();
		glClear(GL_COLOR_BUFFER_BIT);

// Time
#pragma region
		glUniform1i(useTextureLoc, 0);
		auto now = std::chrono::system_clock::now();
		std::time_t t = std::chrono::system_clock::to_time_t(now);
		std::tm localTimeBuf;
		localtime_s(&localTimeBuf, &t);
		std::tm *localTime = &localTimeBuf;
		int hours = localTime->tm_hour;	 // 0-23
		int minutes = localTime->tm_min; // 0-59
#pragma endregion

		bindClocki();
		glUniform1i(useTextureLoc, 1);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(colorLoc, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 1.0f)));
		glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);
		unbindClocki();

		bindMinutes();
		MinuteHandMat4 = glm::rotate(model, glm::radians(-(GLfloat)(minutes / 60.0f) * 360.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniform3fv(colorLoc, 1, glm::value_ptr(glm::vec3(0.1f, 0.1f, 0.1f)));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(MinuteHandMat4));
		glDrawArrays(GL_LINES, 0, 3);
		unbindMinutes();

		bindHours();
		glUniform1i(useTextureLoc, 0);
		HourHandMat4 = glm::rotate(model, glm::radians(-(GLfloat)(hours / 24.0f) * 360.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		glUniform3fv(colorLoc, 1, glm::value_ptr(glm::vec3(0.7f, 0.0f, 0.0f)));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(HourHandMat4));
		glDrawArrays(GL_LINES, 0, 3);
		unbindHours();

		double mouseX, mouseY;
		glfwGetCursorPos(VIEWPORT.getWindow(), &mouseX, &mouseY);
		int margin = 10;
		if ((mouseX >= margin) && (mouseX - margin < width) && (mouseY >= -40) && (mouseY < height + margin))
		{
			glfwSetWindowAttrib(VIEWPORT.getWindow(), GLFW_DECORATED, GLFW_TRUE);
		}
		else
		{
			glfwSetWindowAttrib(VIEWPORT.getWindow(), GLFW_DECORATED, GLFW_FALSE);
		}

		// ImGui::Begin("Template");
		// 	ImGui::ShowDemoWindow();
		// ImGui::End();
		// imgui.RenderDockSpace();
		glfwSwapBuffers(VIEWPORT.getWindow());
		glfwPollEvents();
	}
	// ImGui_ImplOpenGL3_Shutdown();
	// ImGui_ImplGlfw_Shutdown();
	// ImGui::DestroyContext();
	defShader.Delete();
	ClockiTexture.Delete();
	ClockiVBO.Delete();
	ClockiVAO.Delete();

	MinutesVBO.Delete();
	MinutesVAO.Delete();

	HoursVBO.Delete();
	HoursVAO.Delete();
	return 0;
}
